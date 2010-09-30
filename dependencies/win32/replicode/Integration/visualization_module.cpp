#include "visualization_module.h"

//#include <App/cApp.h>
#ifdef WINDOWS
	#include <io.h>
#endif

LOAD_MODULE(Visualizer)

void Visualizer::start()
{
    initialize();
    OUTPUT<<"Visualizer "<<"started"<<std::endl;
}

void Visualizer::initialize()
{
    // Listen for connections
    struct sockaddr_in vAddress;
    memset( &vAddress, 0, sizeof( vAddress ) );
    vAddress.sin_family = AF_INET;
    vAddress.sin_addr.s_addr = htonl( INADDR_LOOPBACK );
    mListenWatch = ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    OUTPUT << mListenWatch << " " << errno << "\n";
    
    vAddress.sin_port = htons( 0x4242 );
    int vRes = ::bind( mListenWatch, (struct sockaddr*)&vAddress, sizeof( vAddress ) );
    
    vRes = ::listen( mListenWatch, SOMAXCONN );
    OUTPUT << vRes << " " << errno << "\n";
    
#ifdef WINDOWS
	unsigned long vNonBlocking = 1;
	ioctlsocket( mListenWatch, FIONBIO, &vNonBlocking );
#else
    int vFlags = fcntl( mListenWatch, F_GETFL, 0 );
    if ( vFlags == -1 ) vFlags = 0;
    fcntl( mListenWatch, F_SETFL, vFlags | O_NONBLOCK );
#endif
    
    mListenSnapshot = ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    OUTPUT << mListenSnapshot << " " << errno << "\n";
    
    vAddress.sin_port = htons( 0x4243 );
    vRes = ::bind( mListenSnapshot, (struct sockaddr*)&vAddress, sizeof( vAddress ) );
    OUTPUT << vRes << " " << errno << "\n";
    
    vRes = ::listen( mListenSnapshot, SOMAXCONN );
    OUTPUT << vRes << " " << errno << "\n";
    
    std::ifstream vTest( "test.image", std::ios::binary | std::ios::in );
    vTest.seekg( 0, std::ios_base::end );
    mImageSize = vTest.tellg();;
    mImage = new uint8[ mImageSize ];
    vTest.seekg( 0, std::ios_base::beg );
    vTest.get( (char*)mImage, mImageSize );
    vTest.close();
    
#ifdef WINDOWS
	mSnapshotThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)&ProcessSnapshots, this, 0, NULL );
#else
    pthread_attr_t vAttributes;
    pthread_create( &mSnapshotThread, NULL, (void*(*)(void*))(ProcessSnapshots), this );            
#endif
                  
    OUTPUT<<"Visualizer "<<"initialized"<<std::endl;
}

void Visualizer::finalize()
{
    delete [] mImage; mImage = NULL;
//    delete mApp;
    OUTPUT<<"Visualizer "<<"finalized"<<std::endl;
}

#ifdef WINDOWS
DWORD Visualizer::ProcessSnapshots( Visualizer *iVisualizer )
#else
void *Visualizer::ProcessSnapshots( Visualizer *iVisualizer )
#endif
{
    OUTPUT << "Processing started.\n";
    for ( ;; )
    {
        int vClient = ::accept( iVisualizer->mListenSnapshot, NULL, NULL );
        if ( vClient == -1 )
        {
            OUTPUT << "Client error " << errno << "\n";
            continue;
        }
        
        OUTPUT << "Client connected.\n";
        send( vClient, (char*)&iVisualizer->mImageSize, 4, 0 );
        send( vClient, (char*)iVisualizer->mImage, iVisualizer->mImageSize, 0 );
        OUTPUT << "Image sent.\n";
        close( vClient );
    }
    
    return NULL;
}
