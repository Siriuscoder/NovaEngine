/***************************************************************************
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

#include "nova_wgl_support.h"
#include "nova_string_utils.h"

#include "nova_win32_workspace.h"

namespace nova
{


CWGLSupport::CWGLSupport() {}
CWGLSupport::~CWGLSupport() 
{
	mRootWindow = NULL;
}

void CWGLSupport::InitWGLExtensions(HDC hdc)
{
	nova::stl<nstring>::vector exten;


	if(WGL_ARB_extensions_string)
	{
		nstring temp(wglGetExtensionsStringARB(hdc));
		exten = nova::CStringUtils::Split( temp );

		CLog::GetInstance().PrintMessage("WGL Extensions\n", CLog::LG_NO_TIME);

		nstring fullext;
		nova::nUInt32 i, j;
		for(i = 0, j = 0; i < exten.size(); ++i, ++j)
		{
			fullext += exten[i] + " ";
			if(j > 2)
			{
				fullext += "\n";
				j = 0;
			}

			mExtensionsList.push_back(exten[i]);
		}

		CLog::GetInstance().PrintMessage(fullext, CLog::LG_NO_TIME);
	}
	else
		throw NOVA_EXP("CWGLSupport::InitWGLExtensions - \
		WGL_ARB_extensions_string not supported...", BAD_OPERATION);
}

void CWGLSupport::Start()
{
	CLog::GetInstance().PrintMessage(
	"\n\n\
	|===========================================|\n \
	|===========================================|\n \
	|                                           |\n \
	|	Starting WGL Rendering SubSystem    |\n \
	|                                           |\n \
	|===========================================|\n \
	|===========================================|\n\n");
}


void CWGLSupport::Stop()
{
	CLog::GetInstance().PrintMessage(
	"\n\n\
	|===========================================|\n \
	|===========================================|\n \
	|                                           |\n \
	|	Stoping WGL Rendering SubSystem     |\n \
	|                                           |\n \
	|===========================================|\n \
	|===========================================|\n\n");
}

void * CWGLSupport::GetFuncAddress(const nstring & procname)
{
	return wglGetProcAddress(procname.c_str());
}

bool CWGLSupport::SupportPBuffers()
{
	nstring temp("WGL_EXT_pbuffer");
	return (CheckExtention(temp) && wglewIsSupported("WGL_EXT_pbuffer")) == GL_TRUE;
}

CRenderWindow * CWGLSupport::CreateRenderWindowTarget(nstring & name, int priority)
{
	CLog::GetInstance().PrintMessage("(!)Creating wgl window... ");

	CWin32Workspace * space;

	space = new CWin32Workspace(name, priority, this);

	if(!GetRootWindow())
		mRootWindow = dynamic_cast<CRenderWindow *>(space);
	else
		throw NOVA_EXP("CWGLSupport::CreateRenderWindowTarget - \
		Root windows already created!", BAD_OPERATION);

	CLog::GetInstance().PrintMessage("(!)Success!\n");

	return space;
}

}