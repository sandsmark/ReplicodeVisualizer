/*
 *  cQFilterWindow.h
 *  Visor
 *
 *  Created by mb on 8/22/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once

namespace Visor
{
    class cQMainWindow;
    class cVisualizerHypertreeNode;
    
    class cQFilterWindow : public QDockWidget
    {
        Q_OBJECT
        
    protected:
        enum eValue
        {
            VALUE_SALIENCY     =  0,
            VALUE_RESILIENCE   =  1,
            VALUE_ACTIVATION   =  2,
            VALUE_VISIBILITY   =  3,

            VALUE_UPR          =  4,
            VALUE_SLN_THR      =  5,
            VALUE_ACT_THR      =  6,
            VALUE_VIS_THR      =  7,
            VALUE_C_SLN        =  8,
            VALUE_C_SLN_THR    =  9,
            VALUE_C_ACT        = 10,
            VALUE_C_ACT_THR    = 11,
            VALUE_DCY_PER      = 12,
            VALUE_DCY_TGT      = 13,
            VALUE_DCY_PRD      = 14,
            VALUE_AVG_SLN      = 15,
            VALUE_HIGH_SLN     = 16,
            VALUE_LOW_SLN      = 17,
            VALUE_AVG_ACT      = 18,
            VALUE_HIGH_ACT     = 19,
            VALUE_LOW_ACT      = 20,
            VALUE_HIGH_SLN_THR = 21,
            VALUE_LOW_SLN_THR  = 22,
            VALUE_SLN_NTF_PRD  = 23,
            VALUE_HIGH_ACT_THR = 24,
            VALUE_LOW_ACT_THR  = 25,
            VALUE_ACT_NTF_PRD  = 26,
            VALUE_NTF_NEW      = 27,
            VALUE_LOW_RES_THR  = 28,
            VALUE_PSLN_THR     = 29,
        };
    
    protected:
        QScrollArea mScroll;
        QWidget mContainer;
        QVBoxLayout mLayout;
        QGridLayout mGrid;
        QPushButton mFilter;
        QPushButton mReset;
        
        cQMainWindow *mMainWindow;
        
        void AddRange( int iRow, const QString &iLabel, float iFrom, float iTo, QGridLayout &iGrid );
        void AddValue( int iRow, const QString &iCaption, float iValue, QGridLayout &iGrid );
        void AddText( int iRow, const QString &iCaption, const QString &iValue, QGridLayout &iGrid );
        void AddList( int iRow, const QString &iLabel, QString *iChoices, size_t iNumChoices, QGridLayout &iGrid );
        
        void FilterView ( cVisualizerHypertreeNode *iNode );
        void FilterGroup( cVisualizerHypertreeNode *iNode );
        void FilterCode ( cVisualizerHypertreeNode *iNode );
        
        float GetValueFrom( const eValue iValue ) const;
        float GetValueTo( const eValue iValue ) const;
        const QList<QListWidgetItem*> GetValueChoice( const eValue iValue ) const;
        int GetValueChoiceIndex( const eValue iValue, const int iItemIndex ) const;
        
        bool CheckInterval( const float iNumber, const eValue iValue ) const;
        
    public:
        cQFilterWindow( cQMainWindow *iMainWindow );
        
//        virtual QSize sizeHint () const { return QSize( 200, 2000 ); }        
        
    public slots:
        void OnValueChanged( int iValue );        
        void OnFilter( bool iChecked );        
        void OnReset( bool iChecked );        
    };

}
