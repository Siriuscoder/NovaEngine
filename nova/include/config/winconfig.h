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

#define DEVELOP					"Sirius@Soft (C) 2011"

#define TOSTR(x)	#x

#define PACKAGE_NAME			"Nova Graphic Render Engine"
#define FILE_VERSION			0,0,1,1
#define PACKAGE_VERSION			"0.0.1.1"

#ifdef _DEBUG
#	define VERSION_ST				"Nova " PACKAGE_VERSION " develop debug"
#	define PACKAGE_STRING			"Nova " PACKAGE_VERSION " develop debug"
#else
#	define VERSION_ST				"Nova " PACKAGE_VERSION " develop releaze"
#	define PACKAGE_STRING			"Nova " PACKAGE_VERSION " develop releaze"
#endif

#define YEAR					"2009"

#ifdef _NOVADLL
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
