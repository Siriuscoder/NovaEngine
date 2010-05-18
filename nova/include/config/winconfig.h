/***************************************************************************
 *   Copyright (C) 2007 by SIRIUS										   *
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
// Compiled in Windows XP 2008
#pragma once

#define OS						"Windows XP"
// Programmer 
#define PR						"SIRIUS"
// SIRIUS
#define LS						"GPL"
// OpenLisense

// DEBUG

// CXX flags and options
#define CXX						"C++ microsoft compiler"
#define CXX_FLAGS				""
/* /Od /I "F:\source\Game\GE\GE\include" 
 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" 
 /D "_MBCS" /Gm /EHsc /RTC1 /MDd /Fo"Debug\\" 
 /Fd"Debug\vc80.pdb" /W3 /nologo /c /Wp64 /ZI /TP /errorReport:prompt
*/
#define CXX_LINKER_FLAGS		""

/*
/OUT:"..\BUILD\GE.exe" /INCREMENTAL /NOLOGO 
/LIBPATH:"F:\source\Game\GE\GE\lib" 
/MANIFEST /MANIFESTFILE:"Debug\GE.exe.intermediate.manifest" 
/DEBUG /PDB:"f:\source\Game\GE\BUILD\GE.pdb" 
/SUBSYSTEM:WINDOWS /MACHINE:X86 /ERRORREPORT:PROMPT 

OpenGL32.lib GLu32.lib GLaux.lib glew32.lib 
glew32s.lib Utils.lib  kernel32.lib user32.lib 
gdi32.lib winspool.lib comdlg32.lib advapi32.lib 
shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib 
odbccp32.lib

*/

#define DEVELOP					"Sirius@Soft (C) 2009 "

#ifdef _DEBUG
#	define VERSION_ST				"Nova 0.1.1 alpha debug "
#	define PACKAGE_STRING			"Nova 0.1.1 alpha debug "
#else
#	define VERSION_ST				"Nova 0.1.1 alpha releaze "
#	define PACKAGE_STRING			"Nova 0.1.1 alpha releaze "
#endif

#define PACKAGE_NAME			"Nova Engine"

#define PACKAGE_VERSION			"0.1.1"

#define YEAR					"2009"

#ifdef _WINDLL
// Билдим длл-ку
#define NOVA_MAKING_DLL
#endif

#   define HAVE_SDL_SDL_H 1
#   define HAVE_ZLIB_H 1
#	define HAVE_PTHREAD_H 1
#	define HAVE_LIBXML2_LIBXML_TREE_H 1
#	define HAVE_GL_GLEW_H 1
#	define HAVE_IL_IL_H 1

// Using section
#	define USE_THREADS

// Memory Allocator config
//#	define USE_WINNI_FAST_ALLOCATOR
#	define USE_NED_OGRE_ALLOCATOR
//#	define USE_MALLOC_FREE_SYSTEM_ALLOCATOR
