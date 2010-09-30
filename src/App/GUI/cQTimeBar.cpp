/*
 *  cQTimeBar.cpp
 *  Visor
 *
 *  Created by mb on 7/22/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include "cQTimeBar.h"
#include <App/GUI/cQMainWindow.h>
#include <App/GUI/cQVisor.h>
#include <Visualizer/Hypertree/cVisualizerHypertree.h>
using namespace Visor;

/**********************************************************************/
cQTimeBar::cQTimeBar( cQMainWindow *iMainWindow )
: mMainWindow( iMainWindow )
/**********************************************************************/
{
    mSlider = new QSlider( Qt::Horizontal, this );
//    mSlider->setFixedWidth( 64 );
    mSlider->setMinimum(  -1 );
    mSlider->setMaximum( -1 );
    mSlider->setValue( -1 );
    mSlider->setTickInterval( 1 );
    mSlider->setTickPosition( QSlider::TicksBelow );
    addWidget( mSlider );
    connect( mSlider, SIGNAL( valueChanged(int) ), this, SLOT( OnValueChanged(int) ) );
}

/**********************************************************************/
void cQTimeBar::AddImage( r_comp::Image *iImage )
/**********************************************************************/
{
    mImages.push_back( iImage );
    
    mSlider->setMinimum( 0 );
    mSlider->setMaximum( mImages.size() - 1 );
    mSlider->setValue( mImages.size() - 1 );
}

/**********************************************************************/
void cQTimeBar::OnValueChanged( int iValue )
/**********************************************************************/
{
    if ( iValue < 0 ) return;
    if ( iValue >= mImages.size() ) return;
    
    mMainWindow->SetImage( mImages[ iValue ], ( iValue > 0 ) ? mImages[ iValue - 1 ] : NULL );
}
