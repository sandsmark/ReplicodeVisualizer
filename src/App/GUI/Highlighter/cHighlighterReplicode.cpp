/*
 *  cHighlighterReplicode.cpp
 *  Visor
 *
 *  Created by mb on 7/10/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include "cHighlighterReplicode.h"
#include <QtGui/QtGui>

/**********************************************************************/
cHighlighterReplicode::cHighlighterReplicode( QTextDocument *iParent )
: QSyntaxHighlighter( iParent )
/**********************************************************************/
{
    sHighlightingRule vRule;
    
    mKeywordFormat.setForeground( Qt::darkBlue );
    mKeywordFormat.setFontWeight( QFont::Bold );
    QStringList vKeywordPatterns;
    vKeywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
    << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
    << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
    << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
    << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
    << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
    << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
    << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
    << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
    << "\\bvoid\\b" << "\\bvolatile\\b";
    
    foreach ( const QString &vPattern, vKeywordPatterns )
    {
        vRule.mPattern = QRegExp( vPattern );
        vRule.mFormat = mKeywordFormat;
        mHighlightingRules.append( vRule );
    }
    
    mClassFormat.setFontWeight( QFont::Bold );
    mClassFormat.setForeground( Qt::darkMagenta );
    vRule.mPattern = QRegExp("\\bQ[A-Za-z]+\\b");
    vRule.mFormat  = mClassFormat;
    mHighlightingRules.append( vRule );
    
    mSingleLineCommentFormat.setForeground( Qt::darkGreen );
    vRule.mPattern = QRegExp( "//[^\n]*" );
    vRule.mFormat  = mSingleLineCommentFormat;
    mHighlightingRules.append( vRule );
    
    mMultiLineCommentFormat.setForeground( Qt::gray );
    
    mQuotationFormat.setForeground( Qt::red );
    vRule.mPattern = QRegExp("\".*\"");
    vRule.mFormat  = mQuotationFormat;
    mHighlightingRules.append( vRule );

    mFunctionFormat.setFontItalic( true );
    mFunctionFormat.setForeground( Qt::blue );
    vRule.mPattern = QRegExp( "\\b[A-Za-z0-9_]+(?=\\()" );
    vRule.mFormat = mFunctionFormat;
    mHighlightingRules.append( vRule );
    
    mCommentStartExpression = QRegExp("/\\*");
    mCommentEndExpression   = QRegExp("\\*/");
}

/**********************************************************************/
void cHighlighterReplicode::highlightBlock( const QString &iText )
/**********************************************************************/
{
    foreach ( const sHighlightingRule &vRule, mHighlightingRules )
    {
        QRegExp vExpression( vRule.mPattern );
        int vIndex = vExpression.indexIn( iText );
        while ( vIndex >= 0 )
        {
            int vLength = vExpression.matchedLength();
            setFormat( vIndex, vLength, vRule.mFormat );
            vIndex = vExpression.indexIn( iText, vIndex + vLength );
        }
    }

    setCurrentBlockState(0);

    int vStartIndex = 0;
    if ( previousBlockState() != 1 )
    {
        vStartIndex = mCommentStartExpression.indexIn( iText );
    }
    
    while ( vStartIndex >= 0 )
    {
        int vEndIndex = mCommentEndExpression.indexIn( iText, vStartIndex );
        int vCommentLength;
        if ( vEndIndex == -1 )
        {
            setCurrentBlockState( 1 );
            vCommentLength = iText.length() - vStartIndex;
        } else {
            vCommentLength = vEndIndex - vStartIndex
            + mCommentEndExpression.matchedLength();
        }
        setFormat( vStartIndex, vCommentLength, mMultiLineCommentFormat );
        vStartIndex = mCommentStartExpression.indexIn( iText, vStartIndex + vCommentLength );
    }
}
