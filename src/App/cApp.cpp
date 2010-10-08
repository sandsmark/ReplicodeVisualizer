/*
 *  cApp.cpp
 *  Visor
 *
 *  Created by mb on 5/29/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include "cApp.h"
#ifdef Q_WS_MAC
	#include <Ogre/OSX/macUtils.h>
#endif
#include <Component/IComponent.h>
#include <QtGui/QtGui>

#include <App/GUI/cQMainWindow.h>
#include <App/GUI/cQStatusBar.h>
#include <App/GUI/cQVisor.h>
#include <Component/cComponentRegistry.h>
#include <Replicode/Hypertree/cVisualizerReplicode.h>
#include <Replicode/Hypertree/cVisualizerReplicodeNode.h>
#include <Replicode/Component/cComponentReplicode.h>
#include <Replicode/r_exec/init.h>
using namespace Visor;

#ifdef WINDOWS
	#include <fstream>
#else
	#include <fstream.h>
#endif

/**********************************************************************/
cApp::cApp( int argc, char *argv[] )
: QApplication( argc, argv ), mVisualizer( NULL )
//  mRoot( NULL ), mWindow( NULL ), mScene( NULL ), mViewport( NULL )
/**********************************************************************/
{
}

/**********************************************************************/
cApp::~cApp()
/**********************************************************************/
{
    Destroy();
}

/**********************************************************************/
bool cApp::Init( void )
/**********************************************************************/
{    
    mMainWindow = new cQMainWindow;
    mMainWindow->show();
    
#ifdef Q_WS_MAC
    Ogre::String vPath = Ogre::macBundlePath() + "/Contents/Resources/";
#else
    Ogre::String vPath = "Resources/";
#endif
    
    r_exec::Init( NULL, Time::Get, Ogre::String( vPath + "user.classes.replicode" ).c_str() );
    
    return true;    
}

/**********************************************************************/
void cApp::Destroy( void )
/**********************************************************************/
{
    SAFE_DELETE( mMainWindow );
    return;
    
    SAFE_DELETE( mVisualizer );
//    SAFE_DELETE( mRoot );
}

IComponent *gTestRoot = NULL;
/**********************************************************************/
void cApp::TestAddChildren( IComponent *iRoot, int iLevel )
/**********************************************************************/
{
//    if ( iLevel > 4 ) return;
//    if ( iLevel > 6 ) return;
//    if ( iLevel > 7 ) return;
    if ( iLevel > 9 ) return;
//    if ( iLevel > 10 ) return;
    
//    uint32 vNumChildren = rand() % 20;
//    uint32 vSubtract = ( iLevel - 1 ) * 2;
    size_t vNumChildren = rand() % 6;
    if ( vNumChildren == 0 && iLevel == 1 ) vNumChildren = 1;
    size_t vSubtract = 0; //( iLevel - 1 ) * 2;
    vNumChildren = ( vNumChildren < vSubtract ) ? ( 0 ) : ( vNumChildren - vSubtract );
    for ( size_t i=0; i<vNumChildren; i++ )
    {
        IComponent *iChild = new IComponent;
        if ( mNumComponents == 1000 ) gTestRoot = iChild;
        iRoot->AddChild( iChild );
        TestAddChildren( iChild, iLevel + 1 );
        mNumComponents++;
    }
}

/**********************************************************************/
int cApp::Run()
/**********************************************************************/
{        
    if ( Init() )
    {
#if 0
        // Create some components
        if ( false )
        {
            srand(1);
            mNumComponents = 1;
            IComponent *vRoot = new IComponent;
            TestAddChildren( vRoot, 1 );
        }
        else 
        {
//            LoadImage( "/Users/mb/Code/Humanobs/Src/SVN/Visor/bin/Debug/Visor.app/Contents/Resources/test.visualizer.replicode.image" );
            LoadImage( "/Users/mb/Code/Humanobs/Src/SVN/Visor/bin/Debug/Visor.app/Contents/Resources/test.8.replicode.image" );
        }

        char vTemp[ 256 ]; sprintf( vTemp, "%d components", mMainWindow->GetVisor()->GetVisualizer()->GetNumNodes() );
        ((cQStatusBar*)mMainWindow->statusBar())->SetMessage( vTemp );

        mMainWindow->OnLayout();
#endif
        
        exec();
        
        /*
//        printf( "Component name: %s\n", IComponent::GetTypeInfo().GetName() );

        mVisualizer = new cVisualizerHypertree( mScene );
        IComponent::GetRegistry().AddListener( *mVisualizer );
        
        mNumComponents = 1;
        IComponent *vRoot = new IComponent;
        TestAddChildren( vRoot, 1 );
        
        mVisualizer->SetFocus( vRoot );
        mVisualizer->Layout();
        
        mTreeOffset = -( mVisualizer->mBoundsMin + mVisualizer->mBoundsMax ) * 0.5f;
        float vSize = ( mVisualizer->mBoundsMax - mVisualizer->mBoundsMin ).length();
        float vScale = 10.0f / vSize;
        mScene->getRootSceneNode()->setScale( Ogre::Vector3( vScale, vScale, vScale ) );
        mTreeOffset *= vScale;
        mScene->getRootSceneNode()->setPosition( mTreeOffset );    
        
        char Temp[ 256 ]; sprintf( Temp, "%d components have been created", mNumComponents );
        cDebug::Notify( Temp );
        
        mRoot->startRendering();
         */
    }
    
    Destroy();
    
    return 0;
}

struct cPointerHash
{
    static const size_t bucket_size = 4;
    static const size_t min_buckets = 8;
    
    size_t operator()( const void *iComponent ) const { return (size_t)iComponent; }
    bool operator()( const void *iC1, const void *iC2 ) const { return false; }
};

#include <ReplicodeIntegration/r_mem_class.h>
#include <replicode/r_comp/decompiler.h>
#ifdef WINDOWS
	#include <hash_map>
#else
	#include <ext/hash_map>
#endif
#if 0
//#include <replicode/integration/r_mem_class.h>
/**********************************************************************/
void cApp::LoadImage( const char *iFile )
/**********************************************************************/
{
    std::ifstream vInput( iFile, std::ios::binary | std::ios::in );
    if ( !vInput.good() ) return;
    
    ImageMessage *vImage = (ImageMessage*)ImageMessage::Read( vInput );
    vInput.close();
    
//    vImage->trace();
    
    r_comp::Image *vCompilerImage = new r_comp::Image();
    vCompilerImage->load<ImageMessage>( vImage );
    delete vImage;
    
    r_code::vector< r_code::Code* > vObjects;
    vCompilerImage->getObjects< r_exec::LObject >( vObjects );
    
	r_comp::Decompiler	vDecompiler;
	vDecompiler.init( &r_exec::Metadata);
	vDecompiler.decompile_references( vCompilerImage );
//	vDecompiler.decompile( vCompilerImage, &vSourceCode );
//    vDecompiler.decompile_object( 0, &vSourceCode );
//    printf( "%s\n", vSourceCode.str().c_str() );
    
#ifdef WINDOWS
    typedef stdext::hash_map<r_code::Code*,cComponentReplicode*,cPointerHash> tComponentMap;
#else
    typedef __gnu_cxx::hash_map<r_code::Code*,cComponentReplicode*,cPointerHash> tComponentMap;
#endif
    tComponentMap vComponentMap;

#ifdef WINDOWS
    typedef stdext::hash_map<r_code::View*,cComponentReplicode*,cPointerHash> tViewMap;
#else
    typedef __gnu_cxx::hash_map<r_code::View*,cComponentReplicode*,cPointerHash> tViewMap;
#endif
    tViewMap vViewMap;
    
    // The root is the only one object with no parent ( means: with no views )!!!
    
    r_exec::Group *vRoot;
    
    // Find root
    for ( size_t i=0; i<vObjects.size(); i++ )
    {
        r_code::Code *vCode = vObjects[i];
        if ( vCode->get_axiom() == SysObject::ROOT_GRP )
        {
            vRoot = (r_exec::Group*)vCode;
            break;
        }
    }

    // Create components
    for ( size_t i=0; i<vObjects.size(); i++ )
    {
        std::ostringstream	vSourceCode;
        r_code::Code *vCode = vObjects[i];
        
        // HACK!!!
//        if ( i < 6 )
        {
            vDecompiler.decompile_object( i, &vSourceCode );
        }

        cComponentReplicode *vComponent = new cComponentReplicode( vCode, vSourceCode.str() );
        vComponentMap[ vCode ] = vComponent;
        
        for ( UNORDERED_SET<r_code::View*,r_code::View::Hash,r_code::View::Equal>::const_iterator it = vCode->views.begin(); it != vCode->views.end(); it++ )
        {
            r_code::View *vView = *it;
            cComponentReplicode *vViewComponent = new cComponentReplicode( vView );        
            vViewMap[ vView ] = vViewComponent;
            vViewComponent->AddChild( vComponent );
//            vComponent->AddChild( vViewComponent );
        }
             
        /*
        // Show views if it's a group
        uint16 vOpcode = vCode->code(0).asOpcode();
        uint16 vGroupOpcode = r_exec::Opcodes::Group;
        if ( vCode->code(0).asOpcode() == r_exec::Opcodes::Group )
        {   // Create views
            FOR_ALL_VIEWS_BEGIN( vRoot, it )
                r_exec::View *vView = it->second;
                cComponentReplicode *vViewComponent = new cComponentReplicode( vView, "" );        
                vViewMap[ vView ] = vViewComponent;
                vComponent->AddChild( vViewComponent );
            FOR_ALL_VIEWS_END
        }
        */
    }

    // Link components to views
    for ( tViewMap::iterator it = vViewMap.begin(); it != vViewMap.end(); it++ )
    {
        cComponentReplicode *vComponent = it->second;
        r_code::View *vView = vComponent->GetView();
        
        { // Link from owner group
            r_code::Code *vOwner = vView->references[0];
            tComponentMap::iterator vRefIt = vComponentMap.find( vOwner );
            if ( vRefIt == vComponentMap.end() ) continue;
            
            ASSERTTXT( vRefIt != vComponentMap.end(), "Referenced node should be in the image" );                        
            
            vRefIt->second->AddChild( vComponent );
        }
        
        // Link to view's origin
        r_code::Code *vOrigin = vView->references[1];
        if ( vOrigin )
        {   // Link to origin
            tComponentMap::iterator vRefIt = vComponentMap.find( vOrigin );
            ASSERTTXT( vRefIt != vComponentMap.end(), "Referenced node should be in the image" );            
            
            vComponent->AddChild( vRefIt->second );
        }
        
        { // Link to view's object
            r_code::Code *vCode = vView->object;
            tComponentMap::iterator vRefIt = vComponentMap.find( vCode );
            ASSERTTXT( vRefIt != vComponentMap.end(), "Referenced node should be in the image" );            
        
            vComponent->AddChild( vRefIt->second );
        }
    }

    /*
     */
    
    // Link components to other components
    for ( tComponentMap::iterator it = vComponentMap.begin(); it != vComponentMap.end(); it++ )
    {
        cComponentReplicode *vComponent = it->second;
        r_code::Code *vCode = vComponent->GetCode();
        
        for ( size_t r=0; r<vCode->references_size(); r++ )
        {
            r_code::Code *vRef = vCode->get_reference( r );
            
            tComponentMap::iterator vRefIt = vComponentMap.find( vRef );
            ASSERTTXT( vRefIt != vComponentMap.end(), "Referenced node should be in the image" );
            
            vComponent->AddChild( vRefIt->second );
//            vRefIt->second->AddChild( vComponent );
        }
    }
    
    delete vCompilerImage;
    
    /*
    char vTemp[ 256 ]; sprintf( vTemp, "%d components", mMainWindow->GetVisor()->GetVisualizer()->GetNumNodes() );
    ((cQStatusBar*)mMainWindow->statusBar())->SetMessage( vTemp );    
    
    mMainWindow->OnLayout();
    */
}
#endif

