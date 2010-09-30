/*
 *  cQTimeBar.h
 *  Visor
 *
 *  Created by mb on 7/22/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once

namespace r_comp
{
    class Image;
}

namespace Visor
{
    class cQMainWindow;
    
    class cQTimeBar : public QToolBar
    {
        Q_OBJECT
        
    protected:
        cQMainWindow *mMainWindow;
        QSlider *mSlider;
        
        std::vector<r_comp::Image*> mImages;
        
    public:
        cQTimeBar( cQMainWindow *iMainWindow );     
        
        QSlider *GetSlider( void ) const { return mSlider; }
        void AddImage( r_comp::Image *iImage );
        
        r_comp::Image *GetLastImage( void ) const { return mImages[ mImages.size() - 1 ]; }
        
    public slots:
        void OnValueChanged( int iValue );
    };
}
