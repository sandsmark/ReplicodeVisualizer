/*
 *  cQVisor.h
 *  Visor
 *
 *  Created by mb on 6/8/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include <App/GUI/cQOgre.h>

namespace Visor
{
    class cVisualizerReplicode;
    class cVisualizerReplicodeNode;
    class cQMainWindow;
    
    class cQVisor : public cQOgre
    {
	public:
		enum eMouseMode
		{
			MODE_ROTATE = 0,
			MODE_MOVE   = 1,
		};

    protected:
        // QWidget methods
        virtual void mousePressEvent( QMouseEvent *iEvent );
        virtual void mouseReleaseEvent( QMouseEvent *iEvent );
        virtual void mouseDoubleClickEvent( QMouseEvent *iEvent );
        virtual void mouseMoveEvent( QMouseEvent *iEvent );
        virtual void wheelEvent( QWheelEvent *iEvent );
        virtual bool event( QEvent *iEvent );
        virtual void contextMenuEvent( QContextMenuEvent *iEvent );
        
    protected:
        cVisualizerReplicode *mVisualizer;
        QTime mTimer;
        cQMainWindow &mMainWindow;
        
        // Camera controls ( TODO: Create external class for camera handling )
        QPoint mLastMousePos;
        Ogre::Vector2 mCameraAngularVelocity;        
        Ogre::Vector3 mCameraVelocity;
        Ogre::Vector3 mAnimTarget;
        float mAlpha;
        cVisualizerReplicodeNode *mTrackNode;
		eMouseMode mMouseMode;
		bool mClick;
                
		void Rotate( const float iDX, const float iDY );
		void Move( const float iDX, const float iDY );
		void Zoom( const float iDelta );
        
        virtual bool InitResources( void );
        virtual bool InitPost( void );
        
    public:
        cQVisor( cQMainWindow &iMainWindow );
        
        cVisualizerReplicode *GetVisualizer( void ) const { return mVisualizer; }
        
        virtual void RenderFrame( void );
        void TrackNode( cVisualizerReplicodeNode *iNode );
        
        cQMainWindow &GetMainWindow( void ) const { return mMainWindow; }
        
        void Clean( void );
        
       Ogre::Viewport *GetViewport( void ) const { return mViewport; }
        
        void Layout( void );
		void SetMouseMode( eMouseMode iMouseMode ) { mMouseMode = iMouseMode; }
    };
}
