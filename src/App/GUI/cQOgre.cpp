/*
 *  cQOgre.cpp
 *  QTTest
 *
 *  Created by mb on 6/6/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include "cQOgre.h"
#include <QtGui/qevent.h>
#ifdef Q_WS_MAC
	#include <OpenGL/OpenGL.h>
#else
	#include <wingdi.h>
#endif
using namespace Visor;

/**********************************************************************/
cQOgre::cQOgre( QWidget *iParent )
: QGLWidget( iParent ), mWindow( NULL )
/**********************************************************************/
{
    resize( 128, 128 );
    setAttribute( Qt::WA_PaintOnScreen );
    setAttribute( Qt::WA_NoBackground );
    
    InitRoot();
}

/**********************************************************************/
cQOgre::~cQOgre()
/**********************************************************************/
{
    mRoot->shutdown();
    delete mRoot;
    destroy();
}

/**********************************************************************/
bool cQOgre::InitRoot( void )
/**********************************************************************/
{
    // Basic config
#ifdef Q_WS_MAC
    Ogre::String vPath = Ogre::macBundlePath() + "/Contents/Resources/";    
#else
    Ogre::String vPath = "Resources/";    
#endif
    Ogre::String vA = vPath + "Config/Plugins.cfg";    
    Ogre::String vB = vPath + "Config/Visor.cfg";    
	Ogre::String vC = vPath + "../Logs/Visor.log";
	mRoot = OGRE_NEW Ogre::Root( vA, vB, vC );
  
    // Select rendersystem ( TODO: Select OpenGL )
    Ogre::RenderSystemList vRenderers = mRoot->getAvailableRenderers();
    Ogre::RenderSystem *vRender = *vRenderers.begin();
    mRoot->setRenderSystem( vRender );
    
    // Set size to widget size
    QString vSize = QString( "%1x%2" ).arg( width() ).arg( height() );
	std::string vFos = vSize.toStdString();
	const char *vFos2 = vFos.c_str();
    vRender->setConfigOption( "Video Mode", vFos2 );
    vRender->setConfigOption( "Full Screen", "No" );
    mRoot->saveConfig();
    
//    mRoot->showConfigDialog();
//    mRoot->saveConfig();
    
    // Init
    mRoot->initialise( false );
    
    return true;
}

/**********************************************************************/
bool cQOgre::InitWindow( void )
/**********************************************************************/
{
#ifdef Q_WS_MAC
    // Remember Qt's context
    CGLContextObj vContext = CGLGetCurrentContext();
#else
	HGLRC vContext = wglGetCurrentContext();
	HDC vDC = wglGetCurrentDC();
#endif
    
    // Use widget's window
    Ogre::NameValuePairList vParams;
	vParams[ "externalWindowHandle" ] = Ogre::StringConverter::toString( (size_t)winId() );
    vParams[ "macAPI" ] = "cocoa";
    vParams[ "macAPICocoaUseNSView" ] = "true";
    vParams[ "externalGLControl" ] = "true";
//    vParams[ "externalGLContext" ] = (unsigned long)vContext;
//    vParams[ "currentGLContext" ] = (unsigned long)vContext;
    
    mWindow = mRoot->createRenderWindow( "cQOgre", width(), height(), false, &vParams );    

	/*
    // Take over Ogre window
    WId vOgreWinId = 0;
    mWindow->getCustomAttribute( "WINDOW", &vOgreWinId );
    create( vOgreWinId );
	*/

#ifdef Q_WS_MAC
    // Force Qt context
    CGLSetCurrentContext( vContext );
#endif

    return true;
}

/**********************************************************************/
bool cQOgre::InitScene( void )
/**********************************************************************/
{
    mScene    = mRoot->createSceneManager( Ogre::ST_GENERIC );    
    mCamera   = mScene->createCamera( "MainCamera" );
    mViewport = mWindow->addViewport( mCamera );
    
    return true;
}

/**********************************************************************/
void cQOgre::initializeGL()
/**********************************************************************/
{
    InitWindow();
    InitResources();
    InitScene();
    InitPost();
}

/**********************************************************************/
void cQOgre::paintGL()
/**********************************************************************/
{
    RenderFrame();
    mRoot->renderOneFrame();
}

/**********************************************************************/
void cQOgre::resizeGL( int iWidth, int iHeight )
/**********************************************************************/
{
    mCamera->setAspectRatio( (float)width() / (float)height() );
	mWindow->resize( width(), height() );
    mWindow->windowMovedOrResized();
}

/**********************************************************************/
void cQOgre::resizeEvent( QResizeEvent *e )
/**********************************************************************/
{
    QGLWidget::resizeEvent( e );
//    resizeGL( e->size().width(), e->size().height() );
}

/**********************************************************************/
void cQOgre::paintEvent(QPaintEvent *)
/**********************************************************************/
{
    updateGL();
}
