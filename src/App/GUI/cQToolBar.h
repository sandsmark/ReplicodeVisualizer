/*
 *  cQToolBar.h
 *  Visor
 *
 *  Created by mb on 6/8/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

namespace Visor
{
    class cQMainWindow;
    
    class cQToolBar : public QToolBar
    {
    protected:
        cQMainWindow *mMainWindow;
        QSlider *mSlider;
        
    public:
        cQToolBar( cQMainWindow *iMainWindow );     
        
        QSlider *GetSlider( void ) const { return mSlider; }
    };
}
