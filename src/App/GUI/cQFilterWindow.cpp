/*
 *  cQFilterWindow.cpp
 *  Visor
 *
 *  Created by mb on 8/22/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include "cQFilterWindow.h"
#include <App/GUI/cQMainWindow.h>
#include <Replicode/Hypertree/cVisualizerReplicode.h>
#include <Replicode/Component/cComponentReplicode.h>
#include <App/GUI/cQVisor.h>
#include <Visualizer/Hypertree/cVisualizerHypertreeNode.h>
#include <Component/IComponent.h>
#include <Replicode/r_code/object.h>
#include <Replicode/r_exec/opcodes.h>
#include <Replicode/r_exec/group.h>
using namespace Visor;

/**********************************************************************/
cQFilterWindow::cQFilterWindow( cQMainWindow *iMainWindow )
: mMainWindow( iMainWindow )
/**********************************************************************/
{
    setFeatures( QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetVerticalTitleBar );

//    mLayout.setSizeConstraint( QLayout::SetMinimumSize );
//    mGrid.setSizeConstraint( QLayout::SetMinimumSize );

    mContainer.setLayout( &mLayout );
    mLayout.addLayout( &mGrid );
    
//    mScroll.setWidget( &mContainer );
    
    mFilter.setText( "Filter" );
    mLayout.addWidget( &mFilter );
    mReset.setText( "Reset" );
    mLayout.addWidget( &mReset );
    
    AddRange( VALUE_SALIENCY    , "Saliency"    , 0, 1, mGrid );
    AddRange( VALUE_RESILIENCE  , "Resilience"  , 0, 1, mGrid );
    AddRange( VALUE_ACTIVATION  , "Activation"  , 0, 1, mGrid );
    AddRange( VALUE_VISIBILITY  , "Visibility"  , 0, 1, mGrid );

    QString vChoices[] = { "saliency", "saliency threshold" };
    AddRange( VALUE_UPR         , "upr"         , 0, 1, mGrid );
    AddRange( VALUE_SLN_THR     , "sln_thr"     , 0, 1, mGrid );
    AddRange( VALUE_ACT_THR     , "act_thr"     , 0, 1, mGrid );
    AddRange( VALUE_VIS_THR     , "vis_thr"     , 0, 1, mGrid );
    AddRange( VALUE_C_SLN       , "c_sln"       , 0, 1, mGrid );
    AddRange( VALUE_C_SLN_THR   , "c_sln_thr"   , 0, 1, mGrid );
    AddRange( VALUE_C_ACT       , "c_act"       , 0, 1, mGrid );
    AddRange( VALUE_C_ACT_THR   , "c_act_thr"   , 0, 1, mGrid );
    AddRange( VALUE_DCY_PER     , "dcy_per"     , 0, 1, mGrid );
    AddList ( VALUE_DCY_TGT     , "dcy_tgt"     , vChoices, sizeof( vChoices ) / sizeof( *vChoices ), mGrid );
    AddRange( VALUE_DCY_PRD     , "dcy_prd"     , -FLT_MAX, FLT_MAX, mGrid );
    AddRange( VALUE_AVG_SLN     , "avg_sln"     , 0, 1, mGrid );
    AddRange( VALUE_HIGH_SLN    , "high_sln"    , 0, 1, mGrid );
    AddRange( VALUE_LOW_SLN     , "low_sln"     , 0, 1, mGrid );
    AddRange( VALUE_AVG_ACT     , "avg_act"     , 0, 1, mGrid );
    AddRange( VALUE_HIGH_ACT    , "high_act"    , 0, 1, mGrid );
    AddRange( VALUE_LOW_ACT     , "low_act"     , 0, 1, mGrid );
    AddRange( VALUE_HIGH_SLN_THR, "high_sln_thr", 0, 1, mGrid );
    AddRange( VALUE_LOW_SLN_THR , "low_sln_thr" , 0, 1, mGrid );
    AddRange( VALUE_SLN_NTF_PRD , "sln_ntf_prd" , -FLT_MAX, FLT_MAX, mGrid );
    AddRange( VALUE_HIGH_ACT_THR, "high_act_thr", 0, 1, mGrid );
    AddRange( VALUE_LOW_ACT_THR , "low_act_thr" , 0, 1, mGrid );
    AddRange( VALUE_ACT_NTF_PRD , "act_ntf_prd" , -FLT_MAX, FLT_MAX, mGrid );
    AddRange( VALUE_NTF_NEW     , "ntf_new"     , 0, 1, mGrid );
    AddRange( VALUE_LOW_RES_THR , "low_res_thr" , 0, 1, mGrid );
    AddRange( VALUE_PSLN_THR    , "psln_thr"    , 0, 1, mGrid );
    
    mScroll.setWidget( &mContainer );    
    setWidget( &mScroll );    
//    setWidget( &mContainer );    
    
    connect( &mFilter, SIGNAL( clicked(bool) ), this, SLOT( OnFilter(bool) ) );    
    connect( &mReset, SIGNAL( clicked(bool) ), this, SLOT( OnReset(bool) ) );    
}

/**********************************************************************/
void cQFilterWindow::AddRange( int iRow, const QString &iLabel, float iFrom, float iTo, QGridLayout &iGrid )
/**********************************************************************/
{
    iGrid.addWidget( new QLabel( iLabel ), iRow, 0 );
    
    QLineEdit *vFrom = new QLineEdit(); vFrom->setValidator( new QDoubleValidator( iFrom, iTo, 0, vFrom ) );
    vFrom->setFixedWidth( 64 );
//    vFrom->setText( QString::number( iFrom ) );
    iGrid.addWidget( vFrom, iRow, 1 );
    connect( vFrom, SIGNAL( valueChanged(int) ), this, SLOT( OnValueChanged(int) ) );

    QLineEdit *vTo = new QLineEdit(); vTo->setValidator( new QDoubleValidator( iFrom, iTo, 0, vTo ) );
    vTo->setFixedWidth( 64 );
//    vTo->setText( QString::number( iTo ) );
    iGrid.addWidget( vTo, iRow, 2 );
    connect( vTo, SIGNAL( valueChanged(int) ), this, SLOT( OnValueChanged(int) ) );
}

/**********************************************************************/
void cQFilterWindow::AddValue( int iRow, const QString &iCaption, float iValue, QGridLayout &iGrid )
/**********************************************************************/
{
    iGrid.addWidget( new QLabel( iCaption ), iRow, 0 );
    QLabel *vLabel = new QLabel( QString::number( iValue ) );
    iGrid.addWidget( vLabel, iRow, 1 );
}

/**********************************************************************/
void cQFilterWindow::AddText( int iRow, const QString &iCaption, const QString &iValue, QGridLayout &iGrid )
/**********************************************************************/
{
    iGrid.addWidget( new QLabel( iCaption ), iRow, 0 );
    QLabel *vLabel = new QLabel( iValue );
    iGrid.addWidget( vLabel, iRow, 1 );
}

/**********************************************************************/
void cQFilterWindow::OnValueChanged( int iValue )
/**********************************************************************/
{
    return;
    
    for ( int i=0; i<mGrid.rowCount(); i++ )
    {
        QSlider *vSlider = (QSlider*)( mGrid.itemAtPosition( i, 1 )->widget() );
        QLabel *vText    = ( QLabel*)( mGrid.itemAtPosition( i, 2 )->widget() );
        vText->setText( QString::number( vSlider->value() / 1000.0f ) );
        int a = 0;
    }
}

/**********************************************************************/
void cQFilterWindow::OnFilter( bool iChecked )
/**********************************************************************/
{
    cVisualizerReplicode *vVisualizer = mMainWindow->GetVisor()->GetVisualizer();
    const cVisualizerHypertree::tNodeHash &vNodes = vVisualizer->GetNodes();
    
    for ( std::hash_map<const IComponent*,cVisualizerHypertreeNode*,cComponentHash>::const_iterator it=vNodes.begin(); it != vNodes.end(); it++ )
    {
        it->second->Filter( cVisualizerHypertreeNode::FILTER_HIDE );
    }    
    
    for ( std::hash_map<const IComponent*,cVisualizerHypertreeNode*,cComponentHash>::const_iterator it=vNodes.begin(); it != vNodes.end(); it++ )
    {
        cVisualizerHypertreeNode *vNode = it->second;
        cComponentReplicode *vComponent = (cComponentReplicode*)vNode->GetComponent();
        
        if ( vComponent->GetType() == cComponentReplicode::TYPE_VIEW )
        {
            FilterView( vNode );
        }
        else if ( vComponent->GetType() == cComponentReplicode::TYPE_CODE )
        {
            r_code::Code *vCode = vComponent->GetCode();
            if ( vCode->code(0).asOpcode() == r_exec::Opcodes::Group )
            {
                FilterGroup( vNode );
            }
            else
            {
                FilterCode( vNode );
            }
        }        
    }    
}

/**********************************************************************/
void cQFilterWindow::OnReset( bool iChecked )
/**********************************************************************/
{
    cVisualizerReplicode *vVisualizer = mMainWindow->GetVisor()->GetVisualizer();
    const cVisualizerHypertree::tNodeHash &vNodes = vVisualizer->GetNodes();
    
    for ( std::hash_map<const IComponent*,cVisualizerHypertreeNode*,cComponentHash>::const_iterator it=vNodes.begin(); it != vNodes.end(); it++ )
    {
        it->second->Filter( cVisualizerHypertreeNode::FILTER_NEUTRAL );
    }    
}

/**********************************************************************/
void cQFilterWindow::AddList( int iRow, const QString &iLabel, QString *iChoices, size_t iNumChoices, QGridLayout &iGrid )
/**********************************************************************/
{
    iGrid.addWidget( new QLabel( iLabel ), iRow, 0 );
    
    QListWidget *vChoices = new QListWidget();
    vChoices->setSelectionMode( QAbstractItemView::MultiSelection );
//    vChoices->setSortingEnabled( true );
//    vChoices->addItem( "" );
    for ( size_t i=0; i<iNumChoices; i++ )
    {
        vChoices->addItem( iChoices[ i ] );
    }
    iGrid.addWidget( vChoices, iRow, 1, 1, 2 );
    connect( vChoices, SIGNAL( valueChanged(int) ), this, SLOT( OnValueChanged(int) ) );
}

/**********************************************************************/
void cQFilterWindow::FilterView( cVisualizerHypertreeNode *iNode )
/**********************************************************************/
{
    cComponentReplicode *vComponent = (cComponentReplicode*)iNode->GetComponent();
    
    r_code::View *vView = vComponent->GetView();
    ASSERTTXT( vView, "Component doesn't have a view!" );
    
    if ( !CheckInterval( vView->code( VIEW_SLN ).asFloat(), VALUE_SALIENCY  ) ) return;
    if ( !CheckInterval( vView->code( VIEW_RES ).asFloat(), VALUE_RESILIENCE ) ) return;
    if ( vView->code(0).asOpcode() == r_exec::Opcodes::PgmView )
    {
        if ( !CheckInterval( vView->code( IPGM_VIEW_ACT ).asFloat(), VALUE_ACTIVATION ) ) return;
    }
    else if ( vView->code(0).asOpcode() == r_exec::Opcodes::GroupView )
    {
        if ( !CheckInterval( vView->code( GRP_VIEW_VIS ).asFloat(), VALUE_VISIBILITY ) ) return;
    }
    
    iNode->Filter( cVisualizerHypertreeNode::FILTER_HIGHLIGHT );
    
    cComponentReplicode *vChild = (cComponentReplicode*)vComponent->GetChild(0);
    ASSERTTXT( vChild, "View doesn't have children!" );
    cVisualizerHypertreeNode *vChildNode = (cVisualizerHypertreeNode*)iNode->GetVisualizer()->FindNodeByComponent( vChild );
    ASSERTTXT( vChildNode, "Can't find node for component!" );
    if ( vChildNode->GetFilter() == cVisualizerHypertreeNode::FILTER_HIDE )
    {
        vChildNode->Filter( cVisualizerHypertreeNode::FILTER_NEUTRAL );
    }
}

/**********************************************************************/
void cQFilterWindow::FilterGroup( cVisualizerHypertreeNode *iNode )
/**********************************************************************/
{
    cComponentReplicode *vComponent = (cComponentReplicode*)iNode->GetComponent();
    r_exec::Group *vGroup = (r_exec::Group*)vComponent->GetCode();

    if ( !CheckInterval( vGroup->get_upr(), VALUE_UPR ) ) return;
    if ( !CheckInterval( vGroup->get_sln_thr(), VALUE_SLN_THR ) ) return;
    if ( !CheckInterval( vGroup->get_act_thr(), VALUE_ACT_THR ) ) return;
    if ( !CheckInterval( vGroup->get_vis_thr(), VALUE_VIS_THR ) ) return;
    if ( !CheckInterval( vGroup->get_c_sln(), VALUE_C_SLN ) ) return;
    if ( !CheckInterval( vGroup->get_c_sln_thr(), VALUE_C_SLN_THR ) ) return;
    if ( !CheckInterval( vGroup->get_c_act(), VALUE_C_ACT ) ) return;
    if ( !CheckInterval( vGroup->get_c_act_thr(), VALUE_C_ACT_THR ) ) return;
    if ( !CheckInterval( vGroup->code( GRP_DCY_PER ).asFloat(), VALUE_DCY_PER ) ) return;
        
    const QList<QListWidgetItem*> vChoices = GetValueChoice( VALUE_DCY_TGT );
    if ( vChoices.size() == 1 )
    {
        if ( vGroup->code( GRP_DCY_TGT ).asFloat() == 0 )
        {
            if ( GetValueChoiceIndex( VALUE_DCY_TGT, 0 ) != 0 ) return;
        }
        else
        {
            if ( GetValueChoiceIndex( VALUE_DCY_TGT, 0 ) != 1 ) return;
        }
    }
        
    if ( !CheckInterval( vGroup->code( GRP_DCY_PRD ).asFloat(), VALUE_DCY_PRD ) ) return;
    if ( !CheckInterval( vGroup->get_avg_sln(), VALUE_AVG_SLN ) ) return;
    if ( !CheckInterval( vGroup->get_high_sln(), VALUE_HIGH_SLN ) ) return;
    if ( !CheckInterval( vGroup->get_low_sln(), VALUE_LOW_SLN ) ) return;
    if ( !CheckInterval( vGroup->get_avg_act(), VALUE_AVG_ACT ) ) return;
    if ( !CheckInterval( vGroup->get_high_act(), VALUE_HIGH_ACT ) ) return;
    if ( !CheckInterval( vGroup->get_low_act(), VALUE_LOW_ACT ) ) return;
    if ( !CheckInterval( vGroup->get_high_sln_thr(), VALUE_HIGH_SLN_THR ) ) return;
    if ( !CheckInterval( vGroup->get_low_sln_thr(), VALUE_LOW_SLN_THR ) ) return;
    if ( !CheckInterval( vGroup->get_sln_ntf_prd(), VALUE_SLN_NTF_PRD ) ) return;
    if ( !CheckInterval( vGroup->get_high_act_thr(), VALUE_HIGH_ACT_THR ) ) return;
    if ( !CheckInterval( vGroup->get_low_act_thr(), VALUE_LOW_ACT_THR ) ) return;
    if ( !CheckInterval( vGroup->get_act_ntf_prd(), VALUE_ACT_NTF_PRD ) ) return;
    if ( !CheckInterval( vGroup->get_ntf_new(), VALUE_NTF_NEW ) ) return;
    if ( !CheckInterval( vGroup->get_low_res_thr(), VALUE_LOW_RES_THR ) ) return;
    if ( !CheckInterval( vGroup->get_psln_thr(), VALUE_PSLN_THR ) ) return;    
    
    iNode->Filter( cVisualizerHypertreeNode::FILTER_HIGHLIGHT );
}

/**********************************************************************/
void cQFilterWindow::FilterCode( cVisualizerHypertreeNode *iNode )
/**********************************************************************/
{
    return;
    
//    cComponentReplicode *vComponent = (cComponentReplicode*)iNode->GetComponent();
//    iNode->Filter( cVisualizerHypertreeNode::FILTER_HIGHLIGHT );
}

/**********************************************************************/
float cQFilterWindow::GetValueFrom( const eValue iValue ) const
/**********************************************************************/
{
    QLineEdit *vText = (QLineEdit*)( mGrid.itemAtPosition( (int)iValue, 1 )->widget() );
    if ( vText->text().length() == 0 ) return -FLT_MAX;
    return vText->text().toFloat( NULL );
}

/**********************************************************************/
float cQFilterWindow::GetValueTo( const eValue iValue ) const
/**********************************************************************/
{
    QLineEdit *vText = (QLineEdit*)( mGrid.itemAtPosition( (int)iValue, 2 )->widget() );
    if ( vText->text().length() == 0 ) return FLT_MAX;
    return vText->text().toFloat( NULL );
}

/**********************************************************************/
const QList<QListWidgetItem*> cQFilterWindow::GetValueChoice( const eValue iValue ) const
/**********************************************************************/
{
    QListWidget *vList = ( QListWidget*)( mGrid.itemAtPosition( (int)iValue, 1 )->widget() );

    return vList->selectedItems();
}

/**********************************************************************/
int cQFilterWindow::GetValueChoiceIndex( const eValue iValue, int iItemIndex ) const
/**********************************************************************/
{
    QListWidget *vList = ( QListWidget*)( mGrid.itemAtPosition( (int)iValue, 1 )->widget() );
    
    return vList->row( vList->selectedItems()[ iItemIndex ] );
}
            
/**********************************************************************/
bool cQFilterWindow::CheckInterval( const float iNumber, const eValue iValue ) const
/**********************************************************************/
{
    return ( iNumber >= GetValueFrom( iValue ) && iNumber <= GetValueTo( iValue ) );
}
