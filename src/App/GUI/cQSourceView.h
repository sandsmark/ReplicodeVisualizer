/*
 *  cQSourceView.h
 *  Visor
 *
 *  Created by mb on 7/10/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once

#include <App/GUI/Highlighter/cHighlighterReplicode.h>
#include <Event/IEventDispatcher.h>
#include <App/GUI/Events/IListenerSourceView.h>

namespace r_exec
{
    class Group;
}

namespace r_code
{
    class Code;
    class View;
}

namespace r_comp
{
    class Decompiler;
}

namespace Visor
{
    class cComponentReplicode;
    class cQMainWindow;
    
    class cQSourceView : public QDockWidget, public IEventDispatcher< IListenerSourceView >
    {
        Q_OBJECT
        
    protected:
        // QWidget events
        virtual void closeEvent( QCloseEvent * event );
        virtual void resizeEvent( QResizeEvent *event );
        
    protected:
        QWidget mContainer;
        QProgressBar mProgress;
        QTextEdit mEditor;
        QHBoxLayout mOuterLayout;
        QVBoxLayout mLayout;
        QGridLayout mGrid;
        QGridLayout mGroupGrid;
        r_code::Code *mCode;
        cComponentReplicode *mComponent;
        cQMainWindow *mMainWindow;
        QPushButton *mButtonFocus;
        QPushButton *mButtonConnect;
        
        void AddBar( int iRow, const QString &iLabel, float iValue, float iThreshold, QGridLayout &iGrid );
        void AddValue( int iRow, const QString &iCaption, float iValue, QGridLayout &iGrid );
        void AddText( int iRow, const QString &iCaption, const QString &iValue, QGridLayout &iGrid );
        
        void InitLayout( void );
        void InitView( r_code::View *iView );
        void InitSource( const QString &iSource );
        void InitGroup( r_exec::Group *iGroup );
        
    public:
        cQSourceView( cQMainWindow *iMainWindow, cComponentReplicode *iComponent );
        
        void Update( const cComponentReplicode *iComponent );
        
        r_code::Code *GetCode( void ) const { return mCode; }
        cComponentReplicode *GetComponent( void ) const { return mComponent; }
        
        virtual QSize sizeHint () const { return QSize( 200, 2000 ); }
        
    public slots:
        void OnFloat( bool iFloating );
        void OnFocus( bool iChecked );
        void OnConnect( bool iChecked );
    };
}
