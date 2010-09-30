/*
 *  cQStatusBar.cpp
 *  Visor
 *
 *  Created by mb on 6/8/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include "cQStatusBar.h"
using namespace Visor;

/**********************************************************************/
cQStatusBar::cQStatusBar()
/**********************************************************************/
{
    mMessage = new QLabel( this );
    addPermanentWidget( mMessage );
}

/**********************************************************************/
cQStatusBar::~cQStatusBar()
/**********************************************************************/
{
    delete mMessage;
}

/**********************************************************************/
void cQStatusBar::SetMessage( const QString &iMessage )
/**********************************************************************/
{
    mMessage->setText( iMessage );
}
