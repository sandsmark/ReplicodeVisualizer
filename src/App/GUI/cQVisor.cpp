/*
 *  cQVisor.cpp
 *  Visor
 *
 *  Created by mb on 6/8/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include "cQVisor.h"
#include <Component/IComponent.h>
#include <Replicode/Hypertree/cVisualizerReplicode.h>
#include <Replicode/Hypertree/cVisualizerReplicodeNode.h>
#include <Replicode/Hypertree/cVisualizerReplicodeSettings.h>
#include <App/GUI/cQContextMenu.h>
#include <App/GUI/cQMainWindow.h>
#include <Replicode/Component/cComponentReplicode.h>
//#include <OpenGL/OpenGL.h>
#include <math.h>
using namespace Visor;

/**********************************************************************/
cQVisor::cQVisor( cQMainWindow &iMainWindow )
: mVisualizer( NULL ), mTrackNode( NULL ), mMainWindow( iMainWindow )
/**********************************************************************/
{
    setAttribute( Qt::WA_AcceptTouchEvents );
    
	mMouseMode = MODE_ROTATE;
    mCameraVelocity = Ogre::Vector3::ZERO;
    mCameraAngularVelocity = Ogre::Vector2::ZERO;   
    mAlpha = 1;
}

/**********************************************************************/
bool cQVisor::InitResources( void )
/**********************************************************************/
{
#ifdef Q_WS_MAC
    Ogre::String vPath = Ogre::macBundlePath() + "/Contents/Resources/";
#else
    Ogre::String vPath = "Resources/";
#endif
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation( vPath + "Materials/", "FileSystem" );
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation( vPath + "Fonts/", "FileSystem" );
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation( vPath, "FileSystem" );
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups(); 
    
    return true;
}

/**********************************************************************/
bool cQVisor::InitPost( void )
/**********************************************************************/
{
    // Setup camera
    mCamera->setPosition( Ogre::Vector3( 0, 0, -20 ) );
    mCamera->lookAt( Ogre::Vector3( 0, 0, 0 ) );
    mCamera->setNearClipDistance( 1 );
    mCamera->setFarClipDistance( 1000 );
    
    // Set some fancy clear color
//    Ogre::ColourValue vBGColor; vBGColor.setAsABGR( 0xccaa8844 );
    Ogre::ColourValue vBGColor = cVisualizerReplicodeSettings::Get().GetColor( cVisualizerReplicodeSettings::COLOR_VISOR_BACKGROUND );
    mViewport->setBackgroundColour( vBGColor );
    mScene->setFog( Ogre::FOG_LINEAR, vBGColor, 0.001f, 0, 1 );
    
    // Create hypertree visualizer
    mVisualizer = new cVisualizerReplicode( mScene );
    IComponent::GetRegistry().AddListener( *mVisualizer );
    
    return true;
}

/**********************************************************************/
void cQVisor::RenderFrame( void )
/**********************************************************************/
{
//    mViewport->setBackgroundColour( Ogre::ColourValue( Ogre::Math::RangeRandom( 0, 1 ), Ogre::Math::RangeRandom( 0, 1 ), Ogre::Math::RangeRandom( 0, 1 ), Ogre::Math::RangeRandom( 0, 1 ) ) );
    
    ASSERTTXT( mWindow, "Main window is invalid" );
    ASSERTTXT( mCamera, "Main camera is invalid" );
    
    static float vTick = 0;
    vTick += 0.01f;
  
    Ogre::ColourValue vBGColor = cVisualizerReplicodeSettings::Get().GetColor( cVisualizerReplicodeSettings::COLOR_VISOR_BACKGROUND );
    mViewport->setBackgroundColour( vBGColor );
    mScene->setFog( Ogre::FOG_LINEAR, vBGColor, 0.001f, 0, 1 );
    
    if ( mAlpha < 1 )
    {
        mAlpha += mTimer.elapsed() * 0.001f;
        if ( mAlpha > 1 ) mAlpha = 1;
        Ogre::Vector3 vCenter = mScene->getRootSceneNode()->getPosition();
        Ogre::Vector3 vDelta  = mAnimTarget - mScene->getRootSceneNode()->getPosition();
        vCenter = vCenter + vDelta * sinf( mAlpha * M_PI_2 );
        mScene->getRootSceneNode()->setPosition( vCenter );    

        /*
        float vZoom = ( mCamera->getPosition() - Ogre::Vector3::ZERO ).length();
        vZoom += ( 2.0f - vZoom ) * sinf( mAlpha * M_PI_2 );
        mCamera->setPosition( Ogre::Vector3::ZERO - mCamera->getDirection() * vZoom );
        */
    }
    
    Ogre::Vector3 mCameraTarget = Ogre::Vector3::ZERO;
    mCameraVelocity *= powf( 0.1f, mTimer.elapsed() * 0.001f );
    mCameraAngularVelocity *= powf( 0.1f, mTimer.elapsed() * 0.001f );
    float vZoom = ( mCamera->getPosition() - mCameraTarget ).length();
    mCamera->pitch( Ogre::Radian( mCameraAngularVelocity.y ) );
    mCamera->yaw( Ogre::Radian( mCameraAngularVelocity.x ) );
    
    Ogre::Vector3 vCenter = mScene->getRootSceneNode()->getPosition();
    vCenter -= mCamera->getRight() * mCameraVelocity.x;
    vCenter -= mCamera->getUp() * mCameraVelocity.y;
    vCenter -= mCamera->getDirection() * mCameraVelocity.z;
    mScene->getRootSceneNode()->setPosition( vCenter );    
    
    mTimer.start();
    
    /*
    if ( !mKeyboard->isModifierDown( OIS::Keyboard::Ctrl ) )
    {
        mCamera->setPosition( mCameraTarget - mCamera->getDirection() * vZoom );
        //        mCamera->lookAt( mVisualizer->FindNodeByComponent( gTestRoot )->GetPosition() + Ogre::Vector3( 0, -2, 0 ) );
    }
    else
    {
        //        mCameraTarget += mCamera->getRight() * mCameraVelocity.x;
        //        mCameraTarget += mCamera->getUp() * mCameraVelocity.y;
        //        mCameraTarget += mCamera->getDirection() * mCameraVelocity.z;
        mTreeOffset -= mCamera->getRight() * mCameraVelocity.x;
        mTreeOffset -= mCamera->getUp() * mCameraVelocity.y;
        mTreeOffset -= mCamera->getDirection() * mCameraVelocity.z;
        mScene->getRootSceneNode()->setPosition( mTreeOffset );    
        mCameraVelocity.z = 0;
    }
    */
    mCamera->setPosition( mCameraTarget - mCamera->getDirection() * vZoom );
    
    mCamera->setNearClipDistance( vZoom * 0.1f );
    mCamera->setFarClipDistance( vZoom * 100.0f );
    
    //    mScene->setFog( Ogre::FOG_LINEAR, Ogre::ColourValue( 0.2f, 0.5f, 0.7f, 0 ), 0.001f, vZoom - 10, vZoom + 15 );
    //mScene->setFog( Ogre::FOG_NONE ); //, Ogre::ColourValue( 0.2f, 0.5f, 0.7f, 0 ), 0.001f, vZoom - 10, vZoom + 15 );
    
    // Trigger render in next frame again
    update();
}

/**********************************************************************/
void cQVisor::Layout( void )
/**********************************************************************/
{
    mVisualizer->Layout();
    
	if ( ( mVisualizer->mBoundsMax - mVisualizer->mBoundsMin ).length() > 0.01f )
	{
		Ogre::Vector3 vTreeOffset = -( mVisualizer->mBoundsMin + mVisualizer->mBoundsMax ) * 0.5f;
		float vSize = ( mVisualizer->mBoundsMax - mVisualizer->mBoundsMin ).length();
		float vScale = 10.0f / vSize;
		mScene->getRootSceneNode()->setScale( Ogre::Vector3( vScale, vScale, vScale ) );
		vTreeOffset *= vScale;
		mScene->getRootSceneNode()->setPosition( vTreeOffset );    
	}
    
    TrackNode( mTrackNode );
    mAlpha = 1;
}

/**********************************************************************/
void cQVisor::mousePressEvent( QMouseEvent *iEvent )
/**********************************************************************/
{
//    if ( iEvent->button() != Qt::LeftButton ) return;

	mClick = true;
    mLastMousePos = iEvent->pos();
}

/**********************************************************************/
void cQVisor::mouseReleaseEvent( QMouseEvent *iEvent )
/**********************************************************************/
{
	if ( !mClick ) return;
    cVisualizerReplicodeNode *vNode = (cVisualizerReplicodeNode*)mVisualizer->FindClickedNode( *mViewport, iEvent->x(), iEvent->y() );
    
//    TrackNode( vNode );
    
    // Show source
//    mMainWindow.ShowSource( (cComponentReplicode*)vNode->GetComponent() );
}

/**********************************************************************/
void cQVisor::mouseDoubleClickEvent( QMouseEvent *iEvent )
/**********************************************************************/
{
    cVisualizerReplicodeNode *vNode = (cVisualizerReplicodeNode*)mVisualizer->FindClickedNode( *mViewport, iEvent->x(), iEvent->y() );    
    TrackNode( vNode );
}


/**********************************************************************/
void cQVisor::TrackNode( cVisualizerReplicodeNode *iNode )
/**********************************************************************/
{
    if ( iNode )
    {
        Ogre::Matrix4 vWorld = mScene->getRootSceneNode()->_getFullTransform();
        Ogre::Matrix3 vRotation; vWorld.extract3x3Matrix( vRotation );
        mAlpha = 0;
        mAnimTarget = vRotation * -iNode->GetPosition();
        
        mVisualizer->SetActNode( iNode->GetId() );
    }
    
    mTrackNode = iNode;
}


/**********************************************************************/
void cQVisor::mouseMoveEvent( QMouseEvent *iEvent )
/**********************************************************************/
{
//    if ( iEvent->button() != Qt::LeftButton ) return;
    
    QPointF vDelta = iEvent->pos() - mLastMousePos;
	if ( vDelta.manhattanLength() < 2 ) return;
	vDelta *= -0.001f;
    mLastMousePos = iEvent->pos();
	mClick = false;

	switch ( mMouseMode )
	{
	case MODE_ROTATE:
		Rotate( vDelta.x(), vDelta.y() );
		break;
	case MODE_MOVE:
		Move( vDelta.x(), vDelta.y() );
		break;
	}
}

/**********************************************************************/
bool cQVisor::event( QEvent *iEvent )
/**********************************************************************/
{
    static float   vStartingRadius;
    static QPointF vStartingCenter;
    static int vLastNumPoints;
    
    switch ( iEvent->type() )
    {
    case QEvent::TouchEnd:
            vLastNumPoints = 0;
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
        {
            QTouchEvent *vEvent = (QTouchEvent*)iEvent;
            QList<QTouchEvent::TouchPoint> vPoints = vEvent->touchPoints();

            // Find center
            int vNumActive = 0;
            QPointF vCenter( 0, 0 );
            foreach ( const QTouchEvent::TouchPoint &vPoint, vPoints )
            {
                if ( vPoint.state() == Qt::TouchPointReleased ) continue;
                
                vCenter += vPoint.normalizedPos();
                vNumActive++;
            }
            vCenter /= vNumActive;
            
            // Find radius
            float vRadius = 0;
            foreach ( const QTouchEvent::TouchPoint &vPoint, vPoints )
            {
                if ( vPoint.state() == Qt::TouchPointReleased ) continue;
                
                float vDistance = QVector2D( vCenter - vPoint.normalizedPos() ).length();
                if ( vDistance > vRadius ) vRadius = vDistance;
            }
            
            printf( "%d touches ( %d active )\n", vPoints.size(), vNumActive );
            if ( iEvent->type() == QEvent::TouchBegin )
            {
//                vLastNumPoints  = vPoints.size();
                vLastNumPoints  = -1;
            }
            
//            if ( vLastNumPoints != vPoints.size() )
            if ( vLastNumPoints != vNumActive )
            {
                printf( "%d -> %d\n", vLastNumPoints, vPoints.size() );
                vStartingRadius = vRadius;
                vStartingCenter = vCenter;
                printf( "New radius: %f\n", vStartingRadius );
            }

            float vDeltaRadius = ( vStartingRadius - vRadius ) * 10.0f;
            QPointF vDeltaPos = ( vStartingCenter - vCenter ) * 1.0f;

            printf( "Delta radius is %f\n", vDeltaRadius );
            
//            if ( vPoints.size() == 3 )
            if ( fabsf( vDeltaRadius ) < 0.01f )
            {
                if ( vNumActive == 2 )
                {
					Rotate( vDeltaPos.x(), vDeltaPos.y() );
                }
                else if ( vNumActive == 3 )
                {
					Move( vDeltaPos.x(), vDeltaPos.y() );
                }
            }
            else
            {
                if ( vNumActive >= 2 )
                {
					Zoom( vDeltaRadius );
                }
            }
            
            vStartingCenter = vCenter;
            vStartingRadius = vRadius;
//            vLastNumPoints  = vPoints.size();
            vLastNumPoints  = vNumActive;
            
            mTrackNode = NULL;
        }
        break;
    default:
        return cQOgre::event( iEvent );    
    }
    
    return true;
}


/**********************************************************************/
void cQVisor::wheelEvent( QWheelEvent *iEvent )
/**********************************************************************/
{
	float vDelta = iEvent->delta(); // - mLastMousePos;
	vDelta *= -0.0004f;
	Zoom( vDelta );
}

/**********************************************************************/
void cQVisor::Rotate( const float iDX, const float iDY )
/**********************************************************************/
{
    mCameraAngularVelocity.x += iDX;
    mCameraAngularVelocity.y += iDY;
}

/**********************************************************************/
void cQVisor::Move( const float iDX, const float iDY )
/**********************************************************************/
{
    float vZoom = ( mCamera->getPosition() - Ogre::Vector3::ZERO ).length();
    mCameraVelocity.x += iDX * vZoom * 0.1f;
    mCameraVelocity.y -= iDY * vZoom * 0.1f;
}

/**********************************************************************/
void cQVisor::Zoom( const float iDelta )
/**********************************************************************/
{
    float vZoom = ( mCamera->getPosition() - Ogre::Vector3::ZERO ).length();
    vZoom *= powf( 2, iDelta );
    mCamera->setPosition( Ogre::Vector3::ZERO - mCamera->getDirection() * vZoom );
    mCamera->lookAt( Ogre::Vector3::ZERO );                
}

/**********************************************************************/
void cQVisor::contextMenuEvent( QContextMenuEvent *iEvent )
/**********************************************************************/
{
    cVisualizerReplicodeNode *vNode = (cVisualizerReplicodeNode*)mVisualizer->FindClickedNode( *mViewport, iEvent->x(), iEvent->y() );
    
    cQContextMenu vMenu( this, vNode );
    vMenu.exec( iEvent->globalPos() );    
}

/**********************************************************************/
void cQVisor::Clean( void )
/**********************************************************************/
{
    mVisualizer->Clean();
}
