п»ї/***************************************************************************
 *   Copyright (C) 2009 by SIRIUS										   *
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

#include "nova_error.h"

namespace nova
{

class NOVA_EXPORT COpenGLExtentions
{
public:
	static void InitWGLExtention(int excode);
	static void InitXGLExtention(int excode);
	static void InitExtention(int excode);

	static bool ExtensionPresent(char * str);

	static char * GetGLVersion();
	static char * GetGLVendor();
	static char * GetGLRenderer();
	static char * GetGLExtensions();
	static char * GetGLShadingLanguageVersion();

#ifdef WIN_GRAPHIC_OUTPUT
	static char * GetWGLExtensions(HDC winhdc);
#endif

#ifdef NOVA_UNIX_BUILD
	static char * GetXGLExtensions();
#endif

	static void InitGLEXTSeparateSpecularColor();
};

}
