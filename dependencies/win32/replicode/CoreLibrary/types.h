//	types.h
//
//	Author: Eric Nivel, Thor List
//
//	BSD license:
//	Copyright (c) 2008, Eric Nivel, Thor List
//	All rights reserved.
//	Redistribution and use in source and binary forms, with or without
//	modification, are permitted provided that the following conditions are met:
//
//   - Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//   - Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//   - Neither the name of Eric Nivel or Thor List nor the
//     names of their contributors may be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
//	THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
//	EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//	DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
//	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef core_types_h
#define core_types_h

#include	<cstddef>

#if defined(WIN32)
	#define	WINDOWS
	#define	ARCH_32
#elif defined(WIN64)
	#define	WINDOWS
	#define	ARCH_64
#elif defined(__GNUC__)

#if __GNUC__ == 4
#if __GNUC_MINOR__ < 2
#error "GNU C++ 4.2 or later is required to compile this program"
#endif /* __GNUC_MINOR__ */
#endif /* __GNUC__ */

	#if defined(__x86_64)
		#define	ARCH_64
	#elif defined(__i386)
		#define	ARCH_32
	#endif
	#if defined(__linux)
		#define	LINUX
	#endif
    #if defined(__APPLE_CC__)
        #define	APPLE
    #endif
#endif

#if defined WINDOWS
	#define	WIN32_LEAN_AND_MEAN
	#define	_WIN32_WINNT	0x0501	//	i.e. win xp sp2
	#include	<windows.h>
	#include	<winsock2.h>

	#include	<unordered_set>
	#include	<unordered_map>
	#define		UNORDERED_MAP		std::tr1::unordered_map
	#define		UNORDERED_SET		std::tr1::unordered_set
	#define		UNORDERED_MULTIMAP	std::tr1::unordered_multimap
	#define		UNORDERED_MULTISET	std::tr1::unordered_multiset

	#if defined	CORELIBRARY_EXPORTS
		#define core_dll	__declspec(dllexport)
	#else
		#define core_dll	__declspec(dllimport)
	#endif
	#define	dll_export	__declspec(dllexport)
	#define	dll_import	__declspec(dllimport)

	#define UINT64_C(x) x
	
	#pragma	warning(disable:	4530)	//	warning: exception disabled
	#pragma	warning(disable:	4996)	//	warning: this function may be unsafe
	#pragma	warning(disable:	4800)	//	warning: forcing value to bool
	#pragma	warning(disable:	4251)	//	warning: class xxx needs to have dll-interface to be used by clients of class yyy
#elif defined LINUX
	#define core_dll
	#include <iostream>
	#include <string>
	#include <pthread.h>
	#include <semaphore.h>
	#include <signal.h>
	#include <limits.h>

	#include	<vector>
	#include	<unordered_map>
	#include	<unordered_set>
	#define		UNORDERED_MAP	std::unordered_map
	#define		UNORDERED_SET	std::unordered_set
	#define		UNORDERED_MULTIMAP	std::unordered_multimap
	#define		UNORDERED_MULTISET	std::unordered_multiset

	#define dll_export __attribute((visibility("default")))
	#define dll_import __attribute((visibility("default")))
	#define cdecl

	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netinet/ip.h>
	#include <arpa/inet.h>
	#define SOCKET_ERROR	-1
	#define INVALID_SOCKET	-1
	#define closesocket(X)	close(X)
#elif defined APPLE
    #define core_dll
    #include <iostream>
    #include <string>
    #include <pthread.h>
    #include <semaphore.h>
    #include <signal.h>
    #include <limits.h>

    #include	<vector>
    #include	<tr1/unordered_map>
    #include	<tr1/unordered_set>
    #define		UNORDERED_MAP	std::tr1::unordered_map
    #define		UNORDERED_SET	std::tr1::unordered_set
    #define		UNORDERED_MULTIMAP	std::tr1::unordered_multimap
    #define		UNORDERED_MULTISET	std::tr1::unordered_multiset

    #define dll_export __attribute((visibility("default")))
    #define dll_import __attribute((visibility("default")))
    #define cdecl

    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netinet/ip.h>
    #include <arpa/inet.h>
    #define SOCKET_ERROR	-1
    #define INVALID_SOCKET	-1
    #define closesocket(X)	close(X)
#else
	#error "This is a new platform"
#endif

#define	NEWLINE	'\n'

namespace	core{

#define	WORD32_MASK					0xFFFFFFFF

#if defined	ARCH_32

	typedef	long						word32;
	typedef	short						word16;

	typedef	char						int8;
	typedef	unsigned	char			uint8;
	typedef	short						int16;
	typedef	unsigned	short			uint16;
	typedef	long						int32;
#define uint32 uint32_replicode
	typedef	unsigned	long			uint32_replicode;
	typedef	long		long			int64;
	typedef	unsigned	long	long	uint64;
	typedef	float						float32;
	typedef	double						float64;

	typedef	word32						word;
	typedef	word16						half_word;

	#define	HALF_WORD_SHIFT				16
	#define	HALF_WORD_HIGH_MASK			0xFFFF0000
	#define	HALF_WORD_LOW_MASK			0x0000FFFF
	#define	WORD_MASK					0xFFFFFFFF

#elif defined	ARCH_64

	typedef	int							word32;
	typedef	long						word64;

	typedef	char						int8;
	typedef	unsigned	char			uint8;
	typedef	short						int16;
	typedef	unsigned	short			uint16;
	typedef	int							int32;
	typedef	unsigned	int				uint32;
	typedef	long						int64;
	typedef	unsigned	long			uint64;
	typedef	float						float32;
	typedef	double						float64;

	typedef	word64						word;
	typedef	word32						half_word;

	#define	HALF_WORD_SHIFT				32
	#define	HALF_WORD_HIGH_MASK			0xFFFFFFFF00000000
	#define	HALF_WORD_LOW_MASK			0x00000000FFFFFFFF
	#define	WORD_MASK					0xFFFFFFFFFFFFFFFF
#else
    #error "Unknown architecture!"
#endif

#if defined	WINDOWS
	typedef	HINSTANCE						shared_object;
	typedef	HANDLE							thread;
	#define thread_ret						core::uint32
	#define thread_ret_val(ret)					return ret;
	typedef	LPTHREAD_START_ROUTINE			thread_function;
	#define	thread_function_call			WINAPI
	typedef	SOCKET							socket;
	typedef	HANDLE							semaphore;
	typedef	HANDLE							mutex;
	typedef	CRITICAL_SECTION				critical_section;
	typedef	HANDLE							timer;
	#define	signal_handler_function_call	WINAPI
	typedef	PHANDLER_ROUTINE				signal_handler;
#elif defined	LINUX
	typedef void *							shared_object;
	typedef pthread_t						thread;
	#define thread_ret						void *
	#define thread_ret_val(ret)				pthread_exit((thread_ret)ret);
	typedef thread_ret (*thread_function)(void *);
	#define thread_function_call
	typedef int								socket;
	typedef struct sockaddr					SOCKADDR;
	typedef	sem_t							semaphore;
	typedef pthread_mutex_t					mutex;
	typedef pthread_mutex_t					critical_section;
	typedef timer_t							timer;
	#define signal_handler_function_call
	typedef sighandler_t					signal_handler;
	#define stricmp							strcasecmp
#elif defined APPLE
	typedef void *							shared_object;
	typedef pthread_t						thread;
    #define thread_ret						void *
    #define thread_ret_val(ret)				pthread_exit((thread_ret)ret);
	typedef thread_ret (*thread_function)(void *);
    #define thread_function_call
	typedef int								socket;
	typedef struct sockaddr					SOCKADDR;
	typedef	sem_t							*semaphore;
	typedef pthread_mutex_t					mutex;
	typedef pthread_mutex_t					critical_section;
//	typedef timer_t							timer;
    #define signal_handler_function_call
//	typedef sighandler_t					signal_handler;
	typedef void *                          signal_handler;
    #define stricmp							strcasecmp
    
#define SIGRTMIN 0
    
#else
    #error "This is a new platform"
#endif

}


#endif
