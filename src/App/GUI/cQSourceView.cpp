/*
 *  cQSourceView.cpp
 *  Visor
 *
 *  Created by mb on 7/10/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include "cQSourceView.h"
#include <Replicode/r_code/object.h>
#include <Replicode/r_code/atom.h>
#include <Replicode/r_code/utils.h>
#include <Replicode/r_exec/group.h>
#include <Replicode/r_exec/opcodes.h>
#include <Replicode/r_comp/decompiler.h>
#include <Replicode/Component/cComponentReplicode.h>
#include <App/GUI/cQMainWindow.h>
#include <App/GUI/cQVisor.h>
#include <Replicode/Hypertree/cVisualizerReplicode.h>
#include <Visualizer/Hypertree/cVisualizerHypertreeNode.h>
using namespace Visor;

/**********************************************************************/
cQSourceView::cQSourceView( cQMainWindow *iMainWindow, cComponentReplicode *iComponent )
: mComponent( iComponent ), mMainWindow( iMainWindow )
/**********************************************************************/
{
    InitLayout();
    
    if ( iComponent->GetType() == cComponentReplicode::TYPE_VIEW )
    {
        InitView( iComponent->GetView() );
        InitSource( ((cComponentReplicode*)iComponent->GetChild( 0 ))->GetSource().c_str() );
    }
    else if ( iComponent->GetType() == cComponentReplicode::TYPE_CODE )
    {
        r_code::Code *vCode = iComponent->GetCode();
        if ( vCode->code(0).asOpcode() == r_exec::Opcodes::Group )
        {
            InitGroup( (r_exec::Group*)vCode );
        }
        else
        {
            InitSource( iComponent->GetSource().c_str() );
        }
    }
    
    mCode = iComponent->GetCode();

    /*
    cQVisor *vVisor = mMainWindow->GetVisor();
    cVisualizerReplicode *vVisualizer = vVisor->GetVisualizer();
    cVisualizerHypertreeNode *vNode = vVisualizer->FindNodeByComponent( iComponent );
    Ogre::Viewport *vViewport = vVisor->GetViewport();
    
    float vX = this->geometry().right();
    float vY = ( this->geometry().top() + this->geometry().bottom() ) / 2;
    vX -= vVisor->geometry().left();
    vY -= vVisor->geometry().top();
    vX /= vVisor->geometry().width();
    vY /= vVisor->geometry().height();
    vX = vX * 2 - 1;
    vY = vY * 2 - 1;
    vNode->SetConnection( Ogre::Vector2( vX, vY ) );
    vVisualizer->UpdateConnections();
    */
    
    connect( this, SIGNAL( topLevelChanged(bool) ), this, SLOT( OnFloat(bool) ) );    
}

/**********************************************************************/
void cQSourceView::InitLayout( void )
/**********************************************************************/
{
    setFeatures( QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetVerticalTitleBar );
    
    mContainer.setLayout( &mOuterLayout );
    mOuterLayout.addLayout( &mLayout );
    mLayout.addLayout( &mGrid );
    
    mButtonFocus = new QPushButton( "Focus" );
    mButtonFocus->setFixedWidth( 16 );
    mLayout.addWidget( mButtonFocus );
    connect( mButtonFocus, SIGNAL( clicked(bool) ), this, SLOT( OnFocus(bool) ) );    

    mButtonConnect = new QPushButton( ">" );
    mButtonConnect->setCheckable( true );
    mButtonConnect->setChecked( true );
    mOuterLayout.addWidget( mButtonConnect );
    connect( mButtonConnect, SIGNAL( clicked(bool) ), this, SLOT( OnConnect(bool) ) );    
    
    setWidget( &mContainer );    
}

/**********************************************************************/
void cQSourceView::AddBar( int iRow, const QString &iLabel, float iValue, float iThreshold, QGridLayout &iGrid )
/**********************************************************************/
{
    iGrid.addWidget( new QLabel( iLabel ), iRow, 0 );
    QProgressBar *vBar = new QProgressBar(); vBar->setRange( 0, 1000 );
    vBar->setValue( iValue * 1000 ); vBar->setTextVisible( false );
    iGrid.addWidget( vBar, iRow, 1 );
    QProgressBar *vBarThreshold = new QProgressBar(); vBarThreshold->setRange( 0, 1000 );
    vBarThreshold->setValue( iThreshold * 1000 ); vBarThreshold->setTextVisible( false );
    vBarThreshold->setStyleSheet( "QProgressBar { margin-left: 0px; } QProgressBar::chunk { background-color: rgba(255,0,0,50%); }" );
    vBarThreshold->setMinimumHeight( 4 ); vBarThreshold->setMaximumHeight( 4 );
    iGrid.addWidget( vBarThreshold, iRow, 1 );
    QLabel *vBarValue = new QLabel( QString::number( iValue ) );
    iGrid.addWidget( vBarValue, iRow, 2 );
}

/**********************************************************************/
void cQSourceView::AddValue( int iRow, const QString &iCaption, float iValue, QGridLayout &iGrid )
/**********************************************************************/
{
    iGrid.addWidget( new QLabel( iCaption ), iRow, 0 );
    QLabel *vLabel = new QLabel( QString::number( iValue ) );
    iGrid.addWidget( vLabel, iRow, 1 );
}

/**********************************************************************/
void cQSourceView::AddText( int iRow, const QString &iCaption, const QString &iValue, QGridLayout &iGrid )
/**********************************************************************/
{
    iGrid.addWidget( new QLabel( iCaption ), iRow, 0 );
    QLabel *vLabel = new QLabel( iValue );
    iGrid.addWidget( vLabel, iRow, 1 );
}

/**********************************************************************/
void cQSourceView::InitView( r_code::View *iView )
/**********************************************************************/
{
    if ( !iView ) return;
    
    while ( mGrid.takeAt( 0 ) )
    {
        delete mGrid.takeAt( 0 );
    }
    
    r_exec::Group *vGroup = (r_exec::Group*)iView->references[0];
    
    // Injection time
    uint64 vTime = r_code::Timestamp::Get( &iView->code( iView->code( VIEW_IJT ).asIndex() ) );
    mGrid.addWidget( new QLabel( "Injection" ), 0, 0 );
    QLabel *vInjection = new QLabel( QString::number( vTime / 1000 ) + "ms" );
    mGrid.addWidget( vInjection, 0, 1 );
    
    // Saliency
    AddBar( 1, "Saliency", iView->code( VIEW_SLN ).asFloat(), 0.5f /*vGroup->get_sln_thr()*/, mGrid );
    
    // Resilience
    AddValue( 2, "Resilience", iView->code( VIEW_RES ).asFloat(), mGrid );
        
    if ( iView->code(0).asOpcode() == r_exec::Opcodes::View )
    {
    }
    else if ( iView->code(0).asOpcode() == r_exec::Opcodes::PgmView )
    {
        // Activation
        AddBar( 3, "Activation", iView->code( IPGM_VIEW_ACT ).asFloat(), vGroup->get_act_thr(), mGrid );
    }
    else if ( iView->code(0).asOpcode() == r_exec::Opcodes::GroupView )
    {
        // Visibility
        AddBar( 3, "Visibility", iView->code( GRP_VIEW_VIS ).asFloat(), vGroup->get_vis_thr(), mGrid );
    }
}

/**********************************************************************/
void cQSourceView::InitSource( const QString &iSource )
/**********************************************************************/
{
    new cHighlighterReplicode( mEditor.document() );
    mLayout.addWidget( &mEditor );
    mEditor.setText( iSource );
}

/**********************************************************************/
void cQSourceView::Update ( const cComponentReplicode *iComponent )
/**********************************************************************/
{    
    r_code::View *vView = iComponent->GetView();
    if ( !vView ) return;
    
    InitView( vView );
}

/**********************************************************************/
void cQSourceView::closeEvent( QCloseEvent * event )
/**********************************************************************/
{    
    Dispatch( &IListenerSourceView::OnSourceViewClosed, this );
    
    cQVisor *vVisor = mMainWindow->GetVisor();
    cVisualizerReplicode *vVisualizer = vVisor->GetVisualizer();
    cVisualizerHypertreeNode *vNode = vVisualizer->FindNodeByComponent( mComponent );
    vNode->RemoveConnection();
    vVisualizer->UpdateConnections();
    
    QDockWidget::closeEvent( event );
}

/**********************************************************************/
void cQSourceView::OnFloat( bool iFloating )
/**********************************************************************/
{    
    if ( iFloating )
    {
        QRect vRect = parentWidget()->geometry();
        vRect.setWidth( 200 );
        vRect.setHeight( vRect.height() - 64 );
        setGeometry( vRect );
    }
}

/**********************************************************************/
void cQSourceView::InitGroup( r_exec::Group *iGroup )
/**********************************************************************/
{
    AddValue(  0, "upr", iGroup->get_upr(), mGroupGrid );
    AddBar  (  1, "sln_thr", iGroup->get_sln_thr(), 0, mGroupGrid );
    AddBar  (  2, "act_thr", iGroup->get_act_thr(), 0, mGroupGrid );
    AddBar  (  3, "vis_thr", iGroup->get_vis_thr(), 0, mGroupGrid );
    AddBar  (  4, "c_sln", iGroup->get_c_sln(), 0, mGroupGrid );
    AddBar  (  5, "c_sln_thr", iGroup->get_c_sln_thr(), 0, mGroupGrid );
    AddBar  (  6, "c_act", iGroup->get_c_act(), 0, mGroupGrid );
    AddBar  (  7, "c_act_thr", iGroup->get_c_act_thr(), 0, mGroupGrid );
    AddBar  (  8, "dcy_per", iGroup->code( GRP_DCY_PER ).asFloat(), 0, mGroupGrid );
    AddText (  9, "dcy_tgt", ( iGroup->code( GRP_DCY_TGT ).asFloat() == 0 ) ? "saliency" : "saliency threshold", mGroupGrid );
    AddValue( 10, "dcy_prd", iGroup->code( GRP_DCY_PRD ).asFloat(), mGroupGrid );
    AddBar  ( 11, "avg_sln", iGroup->get_avg_sln(), 0, mGroupGrid );
    AddBar  ( 12, "high_sln", iGroup->get_high_sln(), 0, mGroupGrid );
    AddBar  ( 13, "low_sln", iGroup->get_low_sln(), 0, mGroupGrid );
    AddBar  ( 14, "avg_act", iGroup->get_avg_act(), 0, mGroupGrid );
    AddBar  ( 15, "high_act", iGroup->get_high_act(), 0, mGroupGrid );
    AddBar  ( 16, "low_act", iGroup->get_low_act(), 0, mGroupGrid );
    AddBar  ( 17, "high_sln_thr", iGroup->get_high_sln_thr(), 0, mGroupGrid );
    AddBar  ( 18, "low_sln_thr", iGroup->get_low_sln_thr(), 0, mGroupGrid );
    AddValue( 19, "sln_ntf_prd", iGroup->get_sln_ntf_prd(), mGroupGrid );
    AddBar  ( 20, "high_act_thr", iGroup->get_high_act_thr(), 0, mGroupGrid );
    AddBar  ( 21, "low_act_thr", iGroup->get_low_act_thr(), 0, mGroupGrid );
    AddValue( 22, "act_ntf_prd", iGroup->get_act_ntf_prd(), mGroupGrid );
    AddBar  ( 23, "ntf_new", iGroup->get_ntf_new(), 0, mGroupGrid );
    AddBar  ( 24, "low_res_thr", iGroup->get_low_res_thr(), 0, mGroupGrid );
// TODO: What to do with ntf_grp?
    AddBar  ( 25, "psln_thr", iGroup->get_psln_thr(), 0, mGroupGrid );
    
    mLayout.addLayout( &mGroupGrid );
}

/**********************************************************************/
void cQSourceView::resizeEvent( QResizeEvent * event )
/**********************************************************************/
{    
    QDockWidget::resizeEvent( event );
    
    if ( !mButtonConnect->isChecked() ) return;
    
    cQVisor *vVisor = mMainWindow->GetVisor();
    cVisualizerReplicode *vVisualizer = vVisor->GetVisualizer();
    cVisualizerHypertreeNode *vNode = vVisualizer->FindNodeByComponent( mComponent );
    
    float vX = this->geometry().right();
    float vY = ( this->geometry().top() + this->geometry().bottom() ) / 2;
    vX -= vVisor->geometry().left();
    vY -= vVisor->geometry().top();
    vX /= vVisor->geometry().width();
    vY /= vVisor->geometry().height();
    vX = vX *  2 - 1;
    vY = vY * -2 + 1;
    vNode->SetConnection( Ogre::Vector2( vX, vY ) );
    vVisualizer->UpdateConnections();
}

/**********************************************************************/
void cQSourceView::OnFocus( bool iChecked )
/**********************************************************************/
{    
    cQVisor *vVisor = mMainWindow->GetVisor();
    cVisualizerReplicode *vVisualizer = vVisor->GetVisualizer();
    cVisualizerReplicodeNode *vNode = (cVisualizerReplicodeNode*)vVisualizer->FindNodeByComponent( mComponent );
    vVisor->TrackNode( vNode );
}

/**********************************************************************/
void cQSourceView::OnConnect( bool iChecked )
/**********************************************************************/
{    
    if ( !iChecked )
    {
        cQVisor *vVisor = mMainWindow->GetVisor();
        cVisualizerReplicode *vVisualizer = vVisor->GetVisualizer();
        cVisualizerHypertreeNode *vNode = vVisualizer->FindNodeByComponent( mComponent );
        vNode->RemoveConnection();
        vVisualizer->UpdateConnections();
    }
    else
    {
        resizeEvent( NULL );
    }

}

