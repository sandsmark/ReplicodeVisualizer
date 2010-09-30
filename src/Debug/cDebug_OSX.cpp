/*
 *  cDebug.cpp
 *  Visor
 *
 *  Created by mb on 5/30/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include <string.h>
#include "cDebug.h"
using namespace Visor;

/**********************************************************************/
bool cDebug::Alert( const char *iMessage, const char *iHeader )
/**********************************************************************/
{
    CFOptionFlags vFlags;
    CFUserNotificationDisplayAlert( 0, 0, NULL, NULL, NULL, CFStringCreateWithCStringNoCopy( NULL, iHeader, kCFStringEncodingASCII, kCFAllocatorNull ), CFStringCreateWithCStringNoCopy( NULL, iMessage, kCFStringEncodingASCII, kCFAllocatorNull ), NULL, CFSTR( "Ignore" ), NULL, &vFlags );
    
    return ( vFlags == 0 );
}

/**********************************************************************/
bool cDebug::Alert( const char *iMessage, const char *iFunction, const char *iLine )
/**********************************************************************/
{
    char *vTemp = new char[ strlen( iFunction ) + strlen( iLine ) + 7 ];
    sprintf( vTemp, "%s line %s", iFunction, iLine );
    
    bool vResult = Alert( iMessage, vTemp );
    
    delete [] vTemp;
    
    return vResult;
}

/**********************************************************************/
void cDebug::Notify( const char *iMessage, const char *iHeader )
/**********************************************************************/
{
//    CFUserNotificationDisplayNotice( 0, 0, NULL, NULL, NULL, CFStringCreateWithCStringNoCopy( NULL, iHeader, kCFStringEncodingASCII, kCFAllocatorNull ), CFStringCreateWithCStringNoCopy( NULL, iMessage, kCFStringEncodingASCII, kCFAllocatorNull ), NULL );
    CFOptionFlags vFlags;
    CFUserNotificationDisplayAlert( 0, 0, NULL, NULL, NULL, CFStringCreateWithCStringNoCopy( NULL, iHeader, kCFStringEncodingASCII, kCFAllocatorNull ), CFStringCreateWithCStringNoCopy( NULL, iMessage, kCFStringEncodingASCII, kCFAllocatorNull ), NULL, NULL, NULL, &vFlags );
}

