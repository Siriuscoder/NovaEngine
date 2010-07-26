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

#include "nova_win32_context.h"

namespace nova
{

CWGLContext::CWGLContext() : mhdc(NULL), mhrc(NULL) {}


CWGLContext::CWGLContext(HDC mhdc, HGLRC mhrc)
{
	if(mhdc)
	{
		this->mhdc = mhdc;
	}
	else
		throw NOVA_EXP("CWGLContext::CWGLContext - input mhdc == NULL.", MEM_ERROR);

	this->mhrc = mhrc;
	if(mhdc && mhrc)
		mReady = true;
}

CWGLContext::~CWGLContext()
{
	mhdc = NULL;
	mhrc = NULL;
	mReady = false;
}

HDC CWGLContext::GetDeviceContext() const
{
	return mhdc;
}

HGLRC CWGLContext::GetRenderContext() const
{
	return mhrc;
}

void CWGLContext::SetCurrent()
{
	if(mhdc && mhrc)
	{
		if(!wglMakeCurrent(mhdc, mhrc))
			throw NOVA_EXP("CWGLContext::SetCurrent() failed - problem \
			with wglMakeCurrent function!", BAD_OPERATION);
	}
	else
		throw NOVA_EXP("CWGLContext::SetCurrent() failed - Null input hdc or hrc!", BAD_OPERATION);
	mCurrent = true;
}

void CWGLContext::EndCurrent()
{
	if(!wglMakeCurrent(NULL, NULL))	// Set The Current Active Rendering Context To Zero
		throw NOVA_EXP("CWGLContext::EndCurrent() failed - problem \
			with wglMakeCurrent function!", BAD_OPERATION);
	mCurrent = false;
}

CGLContext * CWGLContext::Clone()
{
	if(!mhdc)
		throw NOVA_EXP("CWGLContext::Clone() failed - Null input hdc", BAD_OPERATION);

	if(!mhrc)
		throw NOVA_EXP("CWGLContext::Clone() failed - Null input hrc", BAD_OPERATION);

	HGLRC newRC = wglCreateContext(mhdc);

	if(!newRC)
		throw NOVA_EXP("CWGLContext::Clone() failed - Can not create \
		new render context!", BAD_OPERATION);

	HGLRC oldrc = wglGetCurrentContext();
	HDC oldhdc = wglGetCurrentDC();

	wglMakeCurrent(NULL, NULL);

	if(!wglShareLists(mhrc, newRC))
	{
		wglDeleteContext(newRC);
		throw NOVA_EXP("CWGLContext::Clone() failed - \
			Can not share render contexts!", BAD_OPERATION);
	}

	wglMakeCurrent(oldhdc, oldrc);

	return new CWGLContext(mhdc, newRC);
}

HGLRC CWGLContext::CreateRenderContext(PIXELFORMATDESCRIPTOR * pfd, GLuint format)
{
	if(!mhdc)
		throw NOVA_EXP("CWGLContext::CreateRenderContext() failed - Null input hdc", BAD_OPERATION);

	if(!DescribePixelFormat(mhdc, format, sizeof(PIXELFORMATDESCRIPTOR), pfd))
		throw NOVA_EXP("CWGLContext::CreateRenderContext() detecting problem with \
			DescribePixelFormat function", BAD_OPERATION);	

	if(!SetPixelFormat(mhdc, format, pfd))
		throw NOVA_EXP("CWGLContext::CreateRenderContext() detecting problem with \
			SetPixelFormat function", BAD_OPERATION);	

	mhrc = wglCreateContext(mhdc);
	if(!mhrc)
		throw NOVA_EXP("CWGLContext::CreateRenderContext() detecting problem with \
			wglCreateContext function", BAD_OPERATION);	

	mReady = true;

	return mhrc;
}

void CWGLContext::DestroyThisContext()
{
	if(!mhrc)
		throw NOVA_EXP("CWGLContext::DestroyThisContext() failed - Null input hrc", BAD_OPERATION);

	if(!wglDeleteContext(mhrc))
		throw NOVA_EXP("CWGLContext::DestroyThisContext() Can not delete this context!", BAD_OPERATION);
	mhrc = NULL;
	mReady = false;
}

}