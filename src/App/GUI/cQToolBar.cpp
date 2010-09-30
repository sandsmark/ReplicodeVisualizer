/*
 *  cQToolBar.cpp
 *  Visor
 *
 *  Created by mb on 6/8/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include "cQToolBar.h"
#include <App/GUI/cQMainWindow.h>
#include <App/GUI/cQVisor.h>
#include <Visualizer/Hypertree/cVisualizerHypertree.h>
using namespace Visor;

/**********************************************************************/
cQToolBar::cQToolBar( cQMainWindow *iMainWindow )
: mMainWindow( iMainWindow )
/**********************************************************************/
{
#ifdef Q_WS_MAC
    Ogre::String vPath = Ogre::macBundlePath() + "/Contents/Resources/";    
#else
    Ogre::String vPath = "Resources/";    
#endif

    setIconSize( QSize( 32, 32 ) );
    addAction( QIcon( QPixmap( ( vPath + "Icons/Rotate.png" ).c_str() ) ), "Rotate", (QObject*)mMainWindow, SLOT( OnRotate() )  );
    addAction( QIcon( QPixmap( ( vPath + "Icons/Move.png" ).c_str() ) ), "Move", (QObject*)mMainWindow, SLOT( OnMove() )  );
    addAction( "Shot", (QObject*)mMainWindow, SLOT( OnShot() )  );
    addAction( "Layout", (QObject*)mMainWindow, SLOT( OnLayout() ) );
    
    mSlider = new QSlider( this );
    mSlider->setFixedWidth( 64 );
    mSlider->setMinimum( 200 );
    mSlider->setMaximum( 800 );
    mSlider->setValue( 200 );
    addWidget( mSlider );
    connect( mSlider, SIGNAL( sliderMoved(int) ), (QObject*)mMainWindow, SLOT( OnSliderMoved(int) ) );
}
