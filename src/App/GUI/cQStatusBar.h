/*
 *  cQStatusBar.h
 *  Visor
 *
 *  Created by mb on 6/8/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

namespace Visor
{
    class cQStatusBar : public QStatusBar
    {
    protected:
        QLabel *mMessage;
        
    public:
        cQStatusBar();
        ~cQStatusBar();
        
        void SetMessage( const QString &iMessage );
    };
}
