/*
 *  cHighlighterReplicode.h
 *  Visor
 *
 *  Created by mb on 7/10/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once

#include <QtGui/QSyntaxHighlighter>

#include <QtCore/QHash>
#include <QtGui/QTextCharFormat>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class cHighlighterReplicode : public QSyntaxHighlighter
{
    Q_OBJECT
    
public:
    cHighlighterReplicode( QTextDocument *iParent = 0 );
    
protected:
    // QSyntaxHighlighter methods
    virtual void highlightBlock( const QString &iText );
    
private:
    struct sHighlightingRule
    {
        QRegExp mPattern;
        QTextCharFormat mFormat;
    };
    QVector<sHighlightingRule> mHighlightingRules;
    
    QRegExp mCommentStartExpression;
    QRegExp mCommentEndExpression;
    
    QTextCharFormat mKeywordFormat;
    QTextCharFormat mClassFormat;
    QTextCharFormat mSingleLineCommentFormat;
    QTextCharFormat mMultiLineCommentFormat;
    QTextCharFormat mQuotationFormat;
    QTextCharFormat mFunctionFormat;
};
