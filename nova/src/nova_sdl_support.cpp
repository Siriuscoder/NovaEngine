п»ї/***************************************************************************
 *   Copyright (C) 2009 by Sirius										   *
 *	 Vdov Nikita Sergeevich	(c)											   *
 *	 siriusnick@gmail.com												   *
 *																		   *
 *	 This source file is part of NovaEngine								   *
 *	 (Object-oriented Game Development Engine)							   *
 *																		   *
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

#include "nova_sdl_support.h"
#include "nova_string_utils.h"
#include "nova_sdl_workspace.h"

namespace nova
{

void CSDLSupport::Start()
{
	CLog::GetInstance().PrintMessage(
	"\n\n\
	|===========================================|\n \
	|===========================================|\n \
	|                                           |\n \
	|	Starting SDL Rendering Subsystem    |\n \
	|                                           |\n \
	|===========================================|\n \
	|===========================================|\n\n");

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0)
		throw NOVA_EXP((nstring("Couldn't initialize SDL: ")+nstring(SDL_GetError())).c_str(), BAD_OPERATION);

	atexit(SDL_Quit);
	SDL_EnableUNICODE(1);
}


void CSDLSupport::Stop()
{
	CLog::GetInstance().PrintMessage(
	"\n\n\
	|===========================================|\n \
	|===========================================|\n \
	|                                           |\n \
	|	Stoping SDL Rendering Subsystem		|\n \
	|                                           |\n \
	|===========================================|\n \
	|===========================================|\n\n");
	SDL_Quit();
}

void * CSDLSupport::GetFuncAddress(const nstring & procname)
{
	return SDL_GL_GetProcAddress(procname.c_str());
}

bool CSDLSupport::SupportPBuffers()
{
#ifdef WIN_BUILD
	nstring temp("WGL_EXT_pbuffer");
	return (CheckExtention(temp) && wglewIsSupported("WGL_EXT_pbuffer")) == GL_TRUE;
#else
	return (glxewIsSupported("GLX_SGIX_PBUFFER")) == GL_TRUE;
#endif
}

#ifdef WIN_BUILD

void CSDLSupport::InitWGLExtensions()
{
	nova::stl<nstring>::vector exten;


	HDC hdc = wglGetCurrentDC();
	if(!hdc)
		throw NOVA_EXP("CWGLSupport::InitWGLExtensions - Couldn't take device context!", BAD_OPERATION);

	if(WGL_ARB_extensions_string)
	{
		nstring temp(wglGetExtensionsStringARB(hdc));
		exten = nova::CStringUtils::Split( temp );

		CLog::GetInstance().PrintMessage("WGL Extensions\n", CLog::LG_NO_TIME);

		nstring fullext;
		nova::uint i, j;
		for(i = 0, j = 0; i < exten.size(); ++i, ++j)
		{
			fullext += exten[i] + " ";
			if(j > 6)
				fullext += "\n";

			mExtensionsList.push_back(exten[i]);
		}

		CLog::GetInstance().PrintMessage(fullext, CLog::LG_NO_TIME);
	}
	else
		throw NOVA_EXP("CWGLSupport::InitWGLExtensions - \
		WGL_ARB_extensions_string not supported...", BAD_OPERATION);
}

#endif

CRenderWindow * CSDLSupport::CreateRenderWindowTarget(nstring & name, int priority)
{
	CLog::GetInstance().PrintMessage("(!)Creating sdl window... ");

	CSDLWorkspace * space;

	space = new CSDLWorkspace(name, priority, this);

	if(!GetRootWindow())
		mRootWindow = dynamic_cast<CRenderWindow *>(space);
	else
		throw NOVA_EXP("CSDLSupport::CreateRenderWindowTarget - \
		Root windows already created!", BAD_OPERATION);

	CLog::GetInstance().PrintMessage("(!)Success!\n");

	return space;
}

}
