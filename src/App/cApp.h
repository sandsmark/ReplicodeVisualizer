/*
 *  cApp.h
 *  Visor
 *
 *  Created by mb on 5/29/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include <OIS/OIS.h>

namespace Visor
{
    class IComponent;
    class cVisualizerReplicode;
    class cQMainWindow;
    
    class cApp : public QApplication
    {
    protected:
        cQMainWindow *mMainWindow;
        
        cVisualizerReplicode *mVisualizer;
        
        void TestAddChildren( IComponent *iRoot, int iLevel );
        int mNumComponents;
        
        bool Init( void );
        void Destroy( void );
        
    public:
        cApp( int argc, char *argv[] );
        ~cApp();
        
        int Run();
        
//        void LoadImage( const char *iFile );        
    };
}
