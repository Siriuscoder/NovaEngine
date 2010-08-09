/***************************************************************************
 *   Copyright (C) 2008 by SIRIUS										   *
 *   SiriusStarNick@yandex.ru											   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program;					                           *
 ***************************************************************************/
#pragma once

#if !defined(DOXYGEN_IGNORE)

#define GIT_BASIC_SCENE_MAN_BRANCH

#define USING_STL

#define USE_XML

#define NOVA_ARCHITECTURE_64	2
#define NOVA_ARCHITECTURE_32	1

#if !defined(__cplusplus)
#	error C++ compiler required.
#endif


#if defined(__WIN32__) || defined(_WINDOWS)
#	define NOVA_PLATFORM_WINDOWS

#	include "config/winconfig.h"

	// Windows compiler support.  Tested with Microsoft Visual C++,
	// Borland C++ Builder, and MinGW GCC.

	// Stuff for Visual C++ only
#	if defined(_MSC_VER)

		// Disable whining about using 'this' as a member initializer on VC++.
#		pragma warning(disable: 4355)
		// Disable whining about implicit conversions to bool
#		pragma warning(disable: 4800)
		// Disable nagging about new "secure" functions like strncpy_s()
#		pragma warning(disable: 4996)
		// Disable complaints about STL data members: VC++ believes
		// these need to be __declspec(dllexport) for some reason.
#		pragma warning(disable: 4251)

#		pragma warning(disable: 4290)
#	endif

	// Define DLL import/export tags for Windows compilers, where we build
	// the library into a DLL, for LGPL license compatibility reasons.
	// (This is based on a similar mechanism in wxWindows.)
#	define WIN_GRAPHIC_OUTPUT
#	define WIN_BUILD

#	ifdef _RELEASE
#		pragma warning(disable: 4275)
#	endif

	#ifdef NOVA_MAKING_DLL
		// When making the DLL, export tagged symbols, so they appear
		// in the import library.
		#define NOVA_EXPORT __declspec(dllexport)
	#elif !defined(NOVA_NO_DLL)
		// We must be _using_ the DLL, so import symbols instead.
		#define NOVA_EXPORT __declspec(dllimport)

	#else
		// Not making a DLL at all, so no-op these declspecs
		#define NOVA_EXPORT
#	endif
#	include <Winsock2.h>

#	include <windows.h> //windows header files
#	include <windowsx.h>

#	define WIN32_LEAN_AND_MEAN
#	define STDCALL__ _stdcall
#	define THISCALL__ __thiscall

#	ifdef min
#		undef min
#	endif

#	ifdef max
#		undef max
#	endif

// некоторые полезные нам определения здесь
#	define MAX_LOADSTRING 100 // строка ресурсов
// нажата ли клавиша?
#	define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code)& 0x80000000)? 1 : 0)
// отпушенна ли клавиша?
#	define KEYUP(vk_code) ((GetAsyncKeyState(vk_code)& 0x80000000)? 0 : 1)

#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || \
	defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define NOVA_ARCHITECTURE	NOVA_ARCHITECTURE_64
#else
#   define NOVA_ARCHITECTURE	NOVA_ARCHITECTURE_32
#endif


#else
	// If not Windows, we assume some sort of Unixy build environment,
	// where autotools is used.  (This includes Cygwin!)  #include the
	// config.h file only if this file was included from a non-header
	// file, because headers must not be dependent on config.h.
#	define NOVA_UNIX_BUILD
#	if defined(HAVE_CONFIG_H)
#		include "config/config.h"
#	endif
#	if defined(HAVE_SYS_STAT_H)
#		include <sys/types.h>
#	endif

#	include <sys/socket.h>
#	include <sys/time.h>
#	include <netinet/in.h>
#	include <netinet/tcp.h>
#	include <netdb.h>
#	include <arpa/inet.h>
#	include <fcntl.h>
#	include <signal.h>
#	include <errno.h>
#	include <unistd.h>
#   include <float.h>

#	define SDL_GRAPHIC_OUTPUT

#	define STDCALL__
#	define THISCALL__

	// Make DLL stuff a no-op on this platform.
#	define NOVA_EXPORT

#if __WORDSIZE == 64
#   define NOVA_ARCHITECTURE	NOVA_ARCHITECTURE_64
#else
#   define NOVA_ARCHITECTURE	NOVA_ARCHITECTURE_32
#endif

#endif

#if defined(WIN_BUILD) && defined(NOVA_UNIX_BUILD)
#	error Do not use this codes in curent OS!!!
#endif

#define NOVA_FLOAT_PRECISION	1
#define NOVA_DOUBLE_PRECISION	2

#define NOVA_PRECISION NOVA_FLOAT_PRECISION

#ifdef _DEBUG
#	define DEBUG_BUILD
#else
#	define RELEASE_BUILD
#endif

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>
#include <io.h>
#include <stdexcept>
#include <exception>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>


#ifdef USING_STL
#	include <vector>
#	include <map>
#	include <string>
#	include <set>
#	include <list>
#	include <deque>
#	include <queue>
#	include <bitset>


#	include <algorithm>
#	include <functional>
#	include <limits>
using namespace std;
#endif

#ifdef USE_CODE_BLOCKS
#   define HAVE_SDL_SDL_H 1
#   define HAVE_ZLIB_H 1
#	define HAVE_PTHREAD_H 1
#	define HAVE_LIBXML2_LIBXML_TREE_H 1
#   define HAVE_GL_GLEW_H

#   ifndef HAVE_CONFIG_H
/* Name of package */
#   define PACKAGE "NovaEngine"

/* Define to the address where bug reports for this package should be sent. */
#   define PACKAGE_BUGREPORT "SiriusNick@gmail.com"

/* Define to the full name of this package. */
#   define PACKAGE_NAME "NovaEngine"

/* Define to the full name and version of this package. */
#   define PACKAGE_STRING "NovaEngine 1.0.0"

/* Define to the one symbol short name of this package. */
#   define PACKAGE_TARNAME "novaengine"

/* Define to the version of this package. */
#   define PACKAGE_VERSION "1.0.0"
#   endif
#endif

#include "sys/nova_ogl.h"
/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2006 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* Main include header for the SDL library */
#ifdef HAVE_SDL_SDL_H
#	include "SDL.h"
#endif

#ifdef HAVE_ZLIB_H
#	include "zlib.h"
#endif

/*  Copyright 1996-2001, 2003, 2006 by                                     */
/*  David Turner, Robert Wilhelm, and Werner Lemberg.                      */
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

#ifdef USE_XML
#ifdef HAVE_LIBXML2_LIBXML_TREE_H
#		include <libxml/parser.h>
#		include <libxml/tree.h>
#endif
#endif

#ifdef USE_THREADS
#	ifdef HAVE_PTHREAD_H
#	include <pthread.h>
#	endif
#else
#	define USE_THREADS 0
#endif

#ifdef HAVE_IL_IL_H
#	include <IL/il.h>
#	include <IL/ilu.h>
#	define USING_DEVIL
#endif



namespace nova 
{

/*//матрица преобразования
typedef float TTransform[M_BASIS_4D][M_BASIS_4D];

  //однородные координаты вершины
typedef float TVertex[M_BASIS_4D];

  //однородные координаты вектора
typedef float TVectorf[M_BASIS_4D];
typedef float TVectord[M_BASIS_4D];

  // Массив граней
typedef int TFaces[M_TRIANGLES];

  // Массив нормалей к граням ( к вершинам)
typedef float TNormals[M_BASIS_3D];

typedef TNormals TMatfaces;

  // Массив подматериалов
typedef int TSubmatmn[2];
*/

///////////////////////////////////////////////////
  /////////////////////////////////////////////////
  //
// типы

// Figure out how to get large integer support on this system.  Suppress
// refman documentation for these typedefs, as they're system-dependent.
#if defined(_MSC_VER)
// It's VC++, so we'll use Microsoft's 64-bit integer types
typedef unsigned __int64 ulonglong;
typedef __int64 longlong;

typedef unsigned __int16 word; // (16 bit word)

/// \brief Contraction for 'const char*'
typedef const __int8 cchar;

typedef unsigned __int8 byte; // (8 bit word)

typedef unsigned __int32 uint; // (32 bit uint)
/// \brief Contraction for 'unsigned long'
typedef unsigned __int32 ulong; // (32 bit long)

typedef nova::ulong ulong_ptr, *pulong_ptr;

#else
// No better idea, so assume the C99 convention.  If your compiler
// doesn't support this, please provide a patch to extend this ifdef, or
// define NO_LONG_LONGS.

typedef uint64_t ulonglong;
typedef int64_t longlong;

typedef uint16_t word; // (16 bit word)

/// \brief Contraction for 'const char*'
typedef const char cchar;

typedef uint8_t byte; // (8 bit word)

typedef uint32_t uint; // (32 bit uint)
/// \brief Contraction for 'unsigned long'
typedef uint32_t ulong; // (32 bit long)

typedef nova::ulong ulong_ptr, *pulong_ptr;

#endif



// Float/Double precision
#if NOVA_PRECISION == NOVA_FLOAT_PRECISION
typedef float real;
#elif NOVA_PRECISION == NOVA_DOUBLE_PRECISION
typedef double real;
#else
#	error You mast choose correct precision...
#endif


#ifdef NOVA_UNIX_BUILD

#define MAKEWORD(a, b)      ((nova::word)(((nova::byte)(((nova::ulong_ptr)(a)) & 0xff)) | ((nova::word)((nova::byte)(((nova::ulong_ptr)(b)) & 0xff))) << 8))
#define MAKELONG(a, b)      ((nova::long)(((nova::word)(((nova::ulong_ptr)(a)) & 0xffff)) | ((nova::ulong)((nova::word)(((nova::ulong_ptr)(b)) & 0xffff))) << 16))
#define LOWORD(l)           ((nova::word)(((nova::ulong_ptr)(l)) & 0xffff))
#define HIWORD(l)           ((nova::word)((((nova::ulong_ptr)(l)) >> 16) & 0xffff))
#define LOBYTE(w)           ((nova::byte)(((nova::ulong_ptr)(w)) & 0xff))
#define HIBYTE(w)           ((nova::byte)((((nova::ulong_ptr)(w)) >> 8) & 0xff))

typedef struct tagPOINT
{
    int  x;
    int  y;
} POINT, *PPOINT;

#ifdef HAVE_MEMSET
#define ZeroMemory(x, y)	memset((x), 0, (y))
#endif

typedef nova::ulong COLORREF;

#endif

}

#endif


