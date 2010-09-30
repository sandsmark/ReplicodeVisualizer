/*
 *  cDebug.h
 *  Visor
 *
 *  Created by mb on 5/30/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once

#ifdef _WIN32
	#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define ASSERT( iExpression )   \
    if ( !iExpression ) abort();

#define ASSERT_STRINGIFY( iLine ) #iLine
#define ASSERT_TOSTRING( iLine ) ASSERT_STRINGIFY( iLine )
#define ASSERT_LINE ASSERT_TOSTRING( __LINE__ )

#define ASSERTTXT( iExpression, iMessage )   \
    if ( !( iExpression ) ) \
    {   \
        if ( cDebug::Alert( iMessage, __PRETTY_FUNCTION__, ASSERT_LINE ) )    \
        {   \
            abort();    \
        }   \
    }

#define FAILTXT( iExpression, iMessage )   \
    if ( cDebug::Alert( iMessage, __PRETTY_FUNCTION__, ASSERT_LINE ) )    \
    {   \
        abort();    \
    }

//if ( !iExpression ) { cDebug::Alert( iMessage, "Assertion failed" ); abort(); }

#define SAFE_DELETE( iObject ) { delete iObject; iObject = NULL; }

namespace Visor
{
    class cDebug
    {
    public:
        static bool Alert( const char *iMessage, const char *iHeader = "Alert" );
        static bool Alert( const char *iMessage, const char *iFunction, const char *iLine );

        static void Notify( const char *iMessage, const char *iHeader = "Notification" );
    };
}
