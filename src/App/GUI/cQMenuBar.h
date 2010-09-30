/*
 *  cQMenuBar.h
 *  Visor
 *
 *  Created by mb on 7/22/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once

namespace Visor
{
    class cQMainWindow;
    
    class cQMenuBar : public QMenuBar
    {
        Q_OBJECT
        
    protected:
        cQMainWindow *mMainWindow;
        QAction *mWatching;
        QAction *mFilter;
        QAction *mSettings;
        
    public:
        cQMenuBar( cQMainWindow *iMainWindow );
        
    public slots:
        void OnLoadImage();
        void OnWatch();
        void OnFilter();
        void OnSettings();
    };
    
}
