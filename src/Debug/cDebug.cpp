/*
 *  cDebug.cpp
 *  Visor
 *
 *  Created by mb on 5/30/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#ifdef _WIN32
	#include "cDebug_Win32.cpp"
#elif defined( __APPLE_CC__ )
	#include "cDebug_OSX.cpp"
#endif
