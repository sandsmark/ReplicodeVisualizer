/*
 *  cQNameDialog.cpp
 *  Visor
 *
 *  Created by mb on 6/27/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include "cQNameDialog.h"
using namespace Visor;

/**********************************************************************/
cQNameDialog::cQNameDialog( const QString &iName )
/**********************************************************************/
{
    setWindowTitle( "Set name" );
    
    QVBoxLayout *vLayout = new QVBoxLayout( this );    
    mText = new QLineEdit( this );
    vLayout->addWidget( mText );
    vLayout->addStretch();
    
    mButtons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
    vLayout->addWidget( mButtons );
    
    connect( mButtons, SIGNAL( accepted() ), this, SLOT( accept() ) );
    connect( mButtons, SIGNAL( rejected() ), this, SLOT( reject() ) );
    
    setLayout( vLayout );
}

/**********************************************************************/
cQNameDialog::~cQNameDialog()
/**********************************************************************/
{
    delete mText;
    delete mButtons;
}
