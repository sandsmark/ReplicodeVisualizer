/*
 *  cQMainWindow.h
 *  Visor
 *
 *  Created by mb on 6/8/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include <App/GUI/Events/IListenerSourceView.h>

namespace r_comp
{
    class Image;
}

namespace r_code
{
    class Code;
}

#ifdef _WIN32
namespace std
{
    using namespace stdext;
}
#elif defined( __APPLE_CC__ )
#include <ext/hash_map>
namespace std
{
    using namespace __gnu_cxx;
}
#endif

namespace Visor
{
    class cQToolBar;
    class cQMenuBar;
    class cQTimeBar;
    class cQStatusBar;
    class cQVisor;
    class cComponentReplicode;
    class cQSourceView;
    class cQFilterWindow;
    class cQSettingsDialog;
    
    struct cImageHash
    {
		static const size_t bucket_size = 4;
		static const size_t min_buckets = 8;
        
        size_t operator()( const r_comp::Image *iComponent ) const { return (size_t)iComponent; }
        bool operator()( const r_comp::Image *iC1, const r_comp::Image *iC2 ) const { return false; }
    };    
    
    class cQMainWindow : public QMainWindow, public IListenerSourceView
    {
        Q_OBJECT

    protected:
        // QMainWindow methods
        virtual void closeEvent( QCloseEvent *iEvent );
//        virtual void contextMenuEvent( QContextMenuEvent *iEvent );
        
    protected:
        cQToolBar           *mToolBar;
        cQTimeBar           *mTimeBar;
        cQStatusBar       *mStatusBar;
        cQVisor               *mVisor;
        cQMenuBar           *mMenuBar;  
        cQFilterWindow *mFilterWindow;  
        cQSettingsDialog *mSettingsDialog;  
        
        bool mWatching;
        QTimer *mWatchTimer;
        
        char *mImage;
        size_t mMaxImageSize;
        size_t mActImageSize;
                
        std::vector<cQSourceView*> mSources;
        
        int mServer;
        
#ifdef _WIN32
		typedef std::hash_map<const r_comp::Image *, std::set<int> > tImageViewHash;
#else
		typedef std::hash_map<const r_comp::Image *, std::set<int>, cImageHash> tImageViewHash;
#endif
       
        tImageViewHash mImageViews;
        
        bool HasView( const r_comp::Image *iImage, int iOID );
        
    public:
        cQMainWindow();
        ~cQMainWindow();
        
        cQVisor *GetVisor( void ) const { return mVisor; }
//        void ShowSource( const QString &iSource, const QString &iTitle = "Source View" );
        void ShowSource( cComponentReplicode *iComponent );

        void LoadImage( const char *iFile );
        void LoadImageFromMemory( const char *iData, size_t iSize );
        void SetImage( r_comp::Image *iImage, r_comp::Image *iPrevImage );
        
        void SetWatching( bool iWatching );
        void ShowFilter( bool iVisible );
        void ShowSettings( bool iVisible );
        
        // IListenerSourceView
        virtual void OnSourceViewClosed( cQSourceView *iSourceView );
        
    public slots:
        void OnLayout();
        void OnSliderMoved( int iValue );
		void OnRotate();
		void OnMove();
		void OnWatchTimer();
		void OnShot();
    };
}
