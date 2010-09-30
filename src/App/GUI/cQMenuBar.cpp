/*
 *  cQMenuBar.cpp
 *  Visor
 *
 *  Created by mb on 7/22/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include "cQMenuBar.h"
#include <App/GUI/cQMainWindow.h>
using namespace Visor;

/**********************************************************************/
cQMenuBar::cQMenuBar( cQMainWindow *iMainWindow )
: QMenuBar( iMainWindow ), mMainWindow( iMainWindow )
/**********************************************************************/
{
    QMenu *vFileMenu = addMenu( "&File" );
    vFileMenu->addAction( "Load Image", this, SLOT( OnLoadImage() ), QKeySequence( Qt::CTRL + Qt::Key_O ) );
    mWatching = vFileMenu->addAction( "Watching", this, SLOT( OnWatch() ) );
    mWatching->setCheckable( true );
    mFilter = vFileMenu->addAction( "Filter", this, SLOT( OnFilter() ) );
    mFilter->setCheckable( true );
    mSettings = vFileMenu->addAction( "Colors", this, SLOT( OnSettings() ) );
    mSettings->setCheckable( true );
}

/**********************************************************************/
void cQMenuBar::OnLoadImage()
/**********************************************************************/
{
    QString vFileName = QFileDialog::getOpenFileName( this, "Open Image", "", "Replicode Images (*.image)" );
    if ( vFileName.size() == 0 ) return;
    
    mMainWindow->LoadImage( vFileName.toLocal8Bit() );
}

/**********************************************************************/
void cQMenuBar::OnWatch()
/**********************************************************************/
{
    mMainWindow->SetWatching( mWatching->isChecked() );
}

/**********************************************************************/
void cQMenuBar::OnFilter()
/**********************************************************************/
{
    mMainWindow->ShowFilter( mFilter->isChecked() );
}

/**********************************************************************/
void cQMenuBar::OnSettings()
/**********************************************************************/
{
    mMainWindow->ShowSettings( mSettings->isChecked() );
}

