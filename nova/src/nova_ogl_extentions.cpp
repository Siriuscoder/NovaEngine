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
#include "nova_stable_precompiled_headers.h"

#include "nova_ogl_extentions.h"

namespace nova
{

char * COpenGLExtentions::GetGLVersion()
{
	return (char *)(glGetString(GL_VERSION));
}

char * COpenGLExtentions::GetGLVendor()
{
	return (char *)(glGetString(GL_VENDOR));
}

char * COpenGLExtentions::GetGLRenderer()
{
	return (char *)(glGetString(GL_RENDERER));
}

char * COpenGLExtentions::GetGLExtensions()
{
	return (char *)(glGetString(GL_EXTENSIONS));
}

char * COpenGLExtentions::GetGLShadingLanguageVersion()
{
	InitExtention(GL_ARB_shading_language_100);
	return (char *)(glGetString(GL_SHADING_LANGUAGE_VERSION));
}

#ifdef WIN_GRAPHIC_OUTPUT
char * COpenGLExtentions::GetWGLExtensions(HDC winhdc)
{
	if(winhdc)
	{
		InitExtention(WGL_ARB_extensions_string);
		return const_cast<char *>(wglGetExtensionsStringARB(winhdc));
	}

	return NULL;
}
#endif


#ifdef NOVA_UNIX_BUILD
char * COpenGLExtentions::GetXGLExtensions()
{
	Display *dpDisplay;

	// Connect to X Server
	dpDisplay = XOpenDisplay(NULL);
	if(dpDisplay == NULL)
		throw nova::NovaExp("COpenGLExtentions::GetXGLExtensions() - can not connect to X server!", BAD_OPERATION);

	if(!glXQueryExtension(dpDisplay, NULL, NULL))
		return NULL;

	char * str = glXQueryExtensionsString(dpDisplay, DefaultScreen(dpDisplay));
	XCloseDisplay(dpDisplay);

	return str;
}
#endif

void COpenGLExtentions::InitExtention(int excode)
{
	if(excode)
	{
		char res[50];
		sprintf(res, "%s %d", "Can not init this extintion-> ", excode);
		throw nova::NovaExp(res, BAD_OPERATION);
	}
}

void  COpenGLExtentions::InitWGLExtention(int excode)
{

}

void COpenGLExtentions::InitXGLExtention(int excode)
{

}

void COpenGLExtentions::InitGLEXTSeparateSpecularColor()
{
	try {
		InitExtention(GL_EXT_separate_specular_color);

		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL_EXT, GL_SEPARATE_SPECULAR_COLOR_EXT);
	} catch(nova::NovaExp &) {
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	}
}

bool COpenGLExtentions::ExtensionPresent(char * str)
{
	if(strstr(GetGLExtensions(), str))
		return true;

	return false;
}

}