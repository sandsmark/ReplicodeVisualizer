/*
 *  cQMainWindow.cpp
 *  Visor
 *
 *  Created by mb on 6/8/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include "cQMainWindow.h"
#include <App/GUI/cQToolBar.h>
#include <App/GUI/cQMenuBar.h>
#include <App/GUI/cQTimeBar.h>
#include <App/GUI/cQStatusBar.h>
#include <App/GUI/cQVisor.h>
#include <App/GUI/cQSourceView.h>
#include <App/GUI/cQFilteRWindow.h>
#include <App/GUI/cQSettingsDialog.h>
#include <Replicode/Component/cComponentReplicode.h>
#include <Replicode/Hypertree/cVisualizerReplicode.h>
#include <Replicode/Hypertree/cVisualizerReplicodeNode.h>
#include <Replicode/Hypertree/cVisualizerReplicodeSettings.h>
#include <ReplicodeIntegration/r_mem_class.h>
#include <replicode/r_comp/decompiler.h>
#ifdef WINDOWS
	#include <hash_map>
	#include <winsock2.h>
	#define EWOULDBLOCK WSAEWOULDBLOCK
#else
	#include <ext/hash_map>
	#include <sys/socket.h>
#endif
using namespace Visor;

/**********************************************************************/
cQMainWindow::cQMainWindow()
: mWatching( false ), mWatchTimer( NULL )
, mImage( NULL ), mMaxImageSize( 0 ), mActImageSize( 0 )
/**********************************************************************/
{
    QSettings vSettings( QSettings::UserScope, "CMLabs", "Visor" );
    cVisualizerReplicodeSettings::Get().Init( vSettings );
    
    mStatusBar      = new cQStatusBar();
    mVisor          = new cQVisor( *this );
    mToolBar        = new cQToolBar( this );
    mTimeBar        = new cQTimeBar( this );
    mFilterWindow   = new cQFilterWindow( this );
    mSettingsDialog = new cQSettingsDialog();
    
	mMenuBar   = new cQMenuBar( this );
    
	setMenuBar( mMenuBar );
    addToolBar( Qt::RightToolBarArea, mToolBar );
    setStatusBar( mStatusBar );
    setCentralWidget( mVisor );
    addToolBar( Qt::TopToolBarArea, mTimeBar );

    addDockWidget( Qt::RightDockWidgetArea, mFilterWindow );
    addDockWidget( Qt::LeftDockWidgetArea , mSettingsDialog );
    
    mFilterWindow->hide();
    mSettingsDialog->hide();
    
    restoreGeometry( vSettings.value( "WindowGeometry" ).toByteArray() );
    restoreState   ( vSettings.value(    "WindowState" ).toByteArray() );

#ifdef WINDOWS
	WSADATA vInfo;
	WSAStartup(MAKEWORD(1, 1), &vInfo);
#endif
}

/**********************************************************************/
cQMainWindow::~cQMainWindow()
/**********************************************************************/
{
    delete mMenuBar;
    delete mTimeBar;
    delete mToolBar;
    delete mStatusBar;
    delete mVisor;
    delete mSettingsDialog;
}

/**********************************************************************/
void cQMainWindow::OnLayout()
/**********************************************************************/
{
    mVisor->Layout();
}

/**********************************************************************/
void cQMainWindow::closeEvent( QCloseEvent *iEvent )
/**********************************************************************/
{
    QSettings vSettings( QSettings::UserScope, "CMLabs", "Visor" );
    vSettings.setValue( "WindowGeometry", saveGeometry() );
    vSettings.setValue(    "WindowState",    saveState() );
    
    cVisualizerReplicodeSettings::Get().Save( vSettings );
    
    QMainWindow::closeEvent( iEvent );
}

/**********************************************************************/
void cQMainWindow::OnSliderMoved( int iValue )
/**********************************************************************/
{
    mVisor->GetVisualizer()->SetSeparation( iValue * 0.01f ); 
    mVisor->Layout();
}

/**********************************************************************/
void cQMainWindow::OnRotate()
/**********************************************************************/
{
	mVisor->SetMouseMode( cQVisor::MODE_ROTATE );
}

/**********************************************************************/
void cQMainWindow::OnMove()
/**********************************************************************/
{
	mVisor->SetMouseMode( cQVisor::MODE_MOVE );
}

#if 0
/**********************************************************************/
void cQMainWindow::contextMenuEvent( QContextMenuEvent *iEvent )
/**********************************************************************/
{
    cQContextMenu vMenu( this );
    vMenu.exec( iEvent->globalPos() );    
}
#endif

#if 0
/**********************************************************************/
void cQMainWindow::ShowSource( const QString &iSource, const QString &iTitle )
/**********************************************************************/
{
    addDockWidget( Qt::LeftDockWidgetArea, new cQSourceView( iSource, iTitle ) );
}
#endif

/**********************************************************************/
void cQMainWindow::ShowSource( cComponentReplicode *iComponent )
/**********************************************************************/
{
    for ( size_t i=0; i<mSources.size(); i++ )
    {
        if ( mSources[i]->GetCode() == iComponent->GetCode() )
        {
            mSources[i]->Update( iComponent );
            return;
        }
    }
    
    cQSourceView *vSource = new cQSourceView( this, iComponent );
    vSource->AddListener( *this );
    mSources.push_back( vSource );
    addDockWidget( Qt::LeftDockWidgetArea, vSource );
    
    vSource->setWindowTitle( QString::number( mSources.size() ) );
    cVisualizerReplicodeNode *vNode = (cVisualizerReplicodeNode*)GetVisor()->GetVisualizer()->FindNodeByComponent( iComponent );
//    vNode->SetCodeReference( Ogre::String( QString::number( mSources.size() ).toLocal8Bit() ) );
    
    /*
    if ( mSources.size() > 1 )
    {
        tabifyDockWidget( mSources[0], vSource );
    }
    */
}

/**********************************************************************/
void cQMainWindow::OnSourceViewClosed( cQSourceView *iSourceView )
/**********************************************************************/
{
    for ( size_t i=0; i<mSources.size(); i++ )
    {
        if ( mSources[i] == iSourceView )
        {   // Delete reference
            cVisualizerReplicodeNode *vNode = (cVisualizerReplicodeNode*)GetVisor()->GetVisualizer()->FindNodeByComponent( mSources[i]->GetComponent() );
            vNode->SetCodeReference( "" );
            
            mSources.erase( mSources.begin() + i );
        }
    }
}

/**********************************************************************/
struct cPointerHash
{
    static const size_t bucket_size = 4;
    static const size_t min_buckets = 8;
    
    size_t operator()( const void *iComponent ) const { return (size_t)iComponent; }
    bool operator()( const void *iC1, const void *iC2 ) const { return false; }
};
/**********************************************************************/

/**********************************************************************/
void cQMainWindow::LoadImage( const char *iFile )
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
    
    mTimeBar->AddImage( vCompilerImage );
}

/**********************************************************************/
void cQMainWindow::LoadImageFromMemory( const char *iData, size_t iSize )
/**********************************************************************/
{
    uint32	map_size   = *((uint32*)(iData+0));
    uint32	code_size  = *((uint32*)(iData+4));
    uint32	reloc_size = *((uint32*)(iData+8));
    ImageMessage *vImage = (ImageMessage*)ImageMessage::Build( map_size, code_size, reloc_size );
    int vSize = vImage->getSize() * sizeof(word32);
    ASSERTTXT( vSize == ( iSize - 12 ), "Image size should match size of data in file!" );
    memcpy( vImage->data(), iData + 12, iSize - 12 );
    
    r_comp::Image *vCompilerImage = new r_comp::Image();
    vCompilerImage->load<ImageMessage>( vImage );
    delete vImage;
    
    mTimeBar->AddImage( vCompilerImage );    
}


/**********************************************************************/
void cQMainWindow::SetImage( r_comp::Image *iImage, r_comp::Image *iPrevImage )
/**********************************************************************/
{    
    // We have to delete all current components
    GetVisor()->Clean();
    
    r_code::vector< r_code::Code* > vObjects;
    iImage->getObjects< r_exec::LObject >( vObjects );
    
	r_comp::Decompiler	vDecompiler;
	vDecompiler.init( &r_exec::Metadata);
	vDecompiler.decompile_references( iImage );

#ifdef WINDOWS
//    typedef stdext::hash_map<r_code::Code*,cComponentReplicode*,cPointerHash> tComponentMap;
    typedef stdext::hash_map<r_code::Code*,cComponentReplicode*> tComponentMap;
#else
    typedef __gnu_cxx::hash_map<r_code::Code*,cComponentReplicode*,cPointerHash> tComponentMap;
#endif
	tComponentMap vComponentMap;
    
#ifdef WINDOWS
//    typedef stdext::hash_map<r_code::View*,cComponentReplicode*,cPointerHash> tViewMap;
    typedef stdext::hash_map<r_code::View*,cComponentReplicode*> tViewMap;
#else
    typedef __gnu_cxx::hash_map<r_code::View*,cComponentReplicode*,cPointerHash> tViewMap;
#endif
	tViewMap vViewMap;
    
    /*
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
    */
    
    // Create components
    for ( size_t i=0; i<vObjects.size(); i++ )
    {
        std::ostringstream	vSourceCode;
        r_code::Code *vCode = vObjects[i];
        
        vDecompiler.decompile_object( i, &vSourceCode, 0 );
        
        cComponentReplicode *vComponent = new cComponentReplicode( vCode, vSourceCode.str() );
        vComponentMap[ vCode ] = vComponent;
        
        for ( UNORDERED_SET<r_code::View*,r_code::View::Hash,r_code::View::Equal>::const_iterator it = vCode->views.begin(); it != vCode->views.end(); it++ )
        {
            r_code::View *vView = *it;
            int vOID = vView->code( VIEW_OID ).asOpcode();
            cComponentReplicode *vViewComponent = new cComponentReplicode( vView, !HasView( iPrevImage, vOID ) );        
            mImageViews[ iImage ].insert( vOID );
            vViewMap[ vView ] = vViewComponent;
            vViewComponent->AddChild( vComponent );
        }
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
        }
    }

    char vTemp[ 256 ]; sprintf( vTemp, "%d components", GetVisor()->GetVisualizer()->GetNumNodes() );
    mStatusBar->SetMessage( vTemp );    
    
	mVisor->TrackNode( NULL );

    OnLayout();
}

/**********************************************************************/
void cQMainWindow::SetWatching( bool iWatching )
/**********************************************************************/
{
    if ( iWatching == mWatching ) return;
        
    if ( iWatching )
    {
        mServer = ::socket( AF_INET, SOCK_STREAM, 0 );

        struct sockaddr_in vAddress;
        memset( &vAddress, 0, sizeof( vAddress ) );
        vAddress.sin_family = AF_INET;
        vAddress.sin_addr.s_addr = htonl( INADDR_LOOPBACK );
        vAddress.sin_port = htons( 0x4242 );
        if ( ::connect( mServer, (struct sockaddr*)&vAddress, sizeof( vAddress ) ) < 0 )
        {
            ASSERTTXT( false, "Can't connect to server!" );
            return;
        }

#ifdef WINDOWS
		unsigned long vNonBlocking = 1;
		ioctlsocket( mServer, FIONBIO, &vNonBlocking );
#else
        int vFlags = fcntl( mServer, F_GETFL, 0 );
        if ( vFlags == -1 ) vFlags = 0;
        fcntl( mServer, F_SETFL, vFlags | O_NONBLOCK );
#endif


        mWatchTimer = new QTimer( this );
        connect( mWatchTimer, SIGNAL( timeout() ), this, SLOT( OnWatchTimer() ) );
        mWatchTimer->start( 1000 );
    }
    else
    {
        ::closesocket( mServer );
        mWatchTimer->stop();
        delete mWatchTimer;
    }
    
    mWatching = iWatching;    
}

/**********************************************************************/
void cQMainWindow::OnWatchTimer()
/**********************************************************************/
{
    if ( mActImageSize < mMaxImageSize )
    {
        size_t vReceived = ::recv( mServer, mImage + mActImageSize, mMaxImageSize - mActImageSize, 0 );
        if ( vReceived > 0 )
        {
            mActImageSize += vReceived;
        }
        else if ( vReceived == -1 )
        {
            ASSERTTXT( errno == EWOULDBLOCK, "Network error has occured!" );
        }

        
        if ( mActImageSize == mMaxImageSize )
        {
            LoadImageFromMemory( mImage, mActImageSize );
            mMaxImageSize = 0;
            mActImageSize = 0;
            delete [] mImage;
        }
        
        return;
    }
    
    uint32 vNextImageSize;
    size_t vReceived = ::recv( mServer, (char*)&vNextImageSize, 4, 0 );
    if ( vReceived == 4 )
    {
        delete [] mImage;
        mMaxImageSize = vNextImageSize;
        mActImageSize = 0;
        mImage = new char[ mMaxImageSize ];
    }
    else if ( vReceived < 0 )
    {
    }
    else
    {
        ASSERTTXT( vReceived > 0, "ImageSize is less than 4 bytes" );
    }
}

/**********************************************************************/
void cQMainWindow::ShowFilter( bool iVisible )
/**********************************************************************/
{
    mFilterWindow->setVisible( iVisible );
}

/**********************************************************************/
void cQMainWindow::ShowSettings( bool iVisible )
/**********************************************************************/
{
    mSettingsDialog->setVisible( iVisible );
}

/**********************************************************************/
void cQMainWindow::OnShot()
/**********************************************************************/
{
    int vServer = ::socket( AF_INET, SOCK_STREAM, 0 );
    
    struct sockaddr_in vAddress;
    memset( &vAddress, 0, sizeof( vAddress ) );
    vAddress.sin_family = AF_INET;
    vAddress.sin_addr.s_addr = htonl( INADDR_LOOPBACK );
    vAddress.sin_port = htons( 0x4243 );
    if ( ::connect( vServer, (struct sockaddr*)&vAddress, sizeof( vAddress ) ) < 0 )
    {
        ASSERTTXT( false, "Can't connect to server!" );
        return;
    }

    uint32 vImageSize;
    size_t vReceived = ::recv( vServer, (char*)&vImageSize, 4, 0 );
    ASSERTTXT( vReceived == 4, "Can't read image size from network!" );
    
    char *vImage = new char[ vImageSize ];
    size_t vActSize = 0;
    while ( vActSize < vImageSize )
    {
        size_t vReceived = ::recv( vServer, (char*)( vImage + vActSize ), ( vImageSize - vActSize ), 0 );
        ASSERTTXT( vReceived > 0, "Can't read image from network!" );
        vActSize += vReceived;
    }
    
    LoadImageFromMemory( vImage, vImageSize );
    delete [] vImage;
    
    ::closesocket( vServer );
}

/**********************************************************************/
bool cQMainWindow::HasView( const r_comp::Image *iImage, int iOID )
/**********************************************************************/
{
    return ( mImageViews[ iImage ].find( iOID ) != mImageViews[ iImage ].end() );
}
