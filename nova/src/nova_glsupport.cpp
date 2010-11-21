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

#include "nova_glsupport.h"
#include "nova_string_utils.h"
//#include "nova_log.h"
#include "nova_render_window.h"

namespace nova
{

template<> CGLSupport * CSingelton<CGLSupport>::SingeltonObject = NULL;
CRenderWindow * CGLSupport::mRootWindow = NULL;

CGLSupport::CGLSupport() : mState(false), CBase("CGLSupport")
{
	ClearExtensionList();
}

CGLSupport::~CGLSupport()
{
	ClearExtensionList();
}

bool CGLSupport::CheckExtention(nstring & ext)
{
	nova::stl<nstring>::vector::iterator it;

	for(it = mExtensionsList.begin(); it != mExtensionsList.end(); ++it)
	{
		if((*it) == ext)
			return true;
	}

	return false;
}

void CGLSupport::InitGLExtentions()
{
	GLenum err = glewInit();
	if(err != GLEW_OK)
		throw NOVA_EXP("CGLSupport::InitGLExtentions() - \
		Detecting problem with using glewInit(). Can not initialize \
		OpenGL Extension Wrangler Library", BAD_OPERATION);

	char * vers = (char *)glGetString(GL_VERSION);
	if(!vers)
		throw NOVA_EXP("CGLSupport::InitGLExtentions() - \
		Detecting problem with using glGetString.", BAD_OPERATION);

	mGLVersion.append(vers);
	char *pInfoStr = NULL;
	mGPUVendor.append(((pInfoStr = (char *)glGetString(GL_VENDOR)) != NULL) ? pInfoStr : "none");
	mRenderer.append(((pInfoStr = (char *)glGetString(GL_RENDERER)) != NULL) ? pInfoStr : "none");
	mGlewVersion.append(((pInfoStr = (char *)glewGetString(GLEW_VERSION)) != NULL) ? pInfoStr : "none");
	mShadingLang.append(((pInfoStr = (char *)glGetString(GL_SHADING_LANGUAGE_VERSION)) != NULL) ? pInfoStr : "none");

	nstring extensions((char *)glGetString(GL_EXTENSIONS));

	mExtensionsList = nova::CStringUtils::Split( extensions );

	nstring temp;
	temp = "GPU Vendor: " + GetGPUVendor();
	CLog::GetInstance().PrintMessage(temp, CLog::LG_NO_TIME);

	temp = "OpenGL Version: " + GetGLVersion();
	CLog::GetInstance().PrintMessage(temp, CLog::LG_NO_TIME);

	temp = "Glew Version: " + GetGlewVersion();
	CLog::GetInstance().PrintMessage(temp, CLog::LG_NO_TIME);

	temp = "Renderer: " + GetRenderer();
	CLog::GetInstance().PrintMessage(temp, CLog::LG_NO_TIME);

	temp = "Shading Language Version: " + GetShadingLangVersion();
	CLog::GetInstance().PrintMessage(temp, CLog::LG_NO_TIME);
	CLog::GetInstance().PrintMessage("\n\n", CLog::LG_NO_TIME);

	CLog::GetInstance().PrintMessage("OpenGL Extensions\n", CLog::LG_NO_TIME);

	temp.clear();
	nova::nUInt32 i, j;
	for(i = 0, j = 0; i < mExtensionsList.size(); ++i, ++j)
	{
		temp += mExtensionsList[i] + " ";
		if(j > 2)
		{
			temp += "\n";
			j = 0;
		}
	}

	CLog::GetInstance().PrintMessage(temp, CLog::LG_NO_TIME);

	mState = true;
}

nstring CGLSupport::GetGPUVendor() const
{
	return mGPUVendor;
}

nstring CGLSupport::GetGlewVersion() const
{
	return mGlewVersion;
}

nstring CGLSupport::GetGLVersion() const
{
	return mGLVersion;
}

void CGLSupport::ClearExtensionList()
{
	mExtensionsList.clear();
}


nstring CGLSupport::GetRenderer() const
{
	return mRenderer;
}


nstring CGLSupport::GetShadingLangVersion() const
{
	return mShadingLang;
}

CGLContext::CGLContext() : mReady(false), mCurrent(false), CBase("CGLContext") {}

CGLContext::~CGLContext()
{
	mReady = false;
}

bool CGLContext::IsReady()
{
	return mReady;
}

bool CGLContext::IsCurrent()
{
	return mCurrent;
}

void CGLSupport::DestroyRootWindow()
{
	if(mRootWindow)
	{
		mRootWindow->Destroy();
		//delete mRootWindow;
	}
}

}
