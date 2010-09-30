/*
 *  cQOgre.h
 *  QTTest
 *
 *  Created by mb on 6/6/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include <QtOpenGL/QGLWidget>
#include <Ogre/Ogre.h>

namespace Visor
{
        
    class cQOgre : public QGLWidget
    {
    //    Q_OBJECT
        
    protected:
        // Ogre stuff
        Ogre::Root *mRoot;
        Ogre::RenderWindow *mWindow;
        Ogre::Camera *mCamera;
        Ogre::Viewport *mViewport;
        Ogre::SceneManager *mScene;
        
        bool InitRoot( void );
        bool InitWindow( void );
        virtual bool InitResources( void ) { return true; }
        bool InitScene( void );
        virtual bool InitPost( void ) { return true; }
        
    public:
        cQOgre( QWidget *iParent = NULL );
        ~cQOgre();
        
        virtual void RenderFrame( void ) {}
        
        // QGLWidget methods
        virtual void initializeGL();
        virtual void paintGL();
        virtual void resizeGL( int iWidth, int iHeight );
        
        // QWidget methods
        virtual void paintEvent(QPaintEvent *e);
        virtual void resizeEvent(QResizeEvent *e);
        
        virtual QSize minimumSizeHint(void) const { return QSize( 128, 128 ); }    
    };
}
