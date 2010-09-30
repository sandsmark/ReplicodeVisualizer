/*
 *  cQNameDialog.h
 *  Visor
 *
 *  Created by mb on 6/27/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once;

namespace Visor
{
    class cQNameDialog : public QDialog
    {
    protected:
        QLineEdit *mText;
        QDialogButtonBox *mButtons;
        
    public:
        cQNameDialog( const QString &iName );
        ~cQNameDialog();
        
        QString GetName( void ) const { return mText->text(); }
    };
}
