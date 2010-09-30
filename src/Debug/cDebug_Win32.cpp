/*
 *  cDebug.cpp
 *  Visor
 *
 *  Created by mb on 5/30/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include <windows.h>
#include <stdio.h>
#include "cDebug.h"
using namespace Visor;

/**********************************************************************/
bool cDebug::Alert( const char *iMessage, const char *iHeader )
/**********************************************************************/
{
	return ( MessageBoxA( NULL, iMessage, iHeader, MB_OKCANCEL ) == IDOK );
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
	MessageBoxA( NULL, iMessage, iHeader, MB_OK );
}

