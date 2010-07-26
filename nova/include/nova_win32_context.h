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
#pragma once

#include "nova_wgl_support.h"

namespace nova
{

class NOVA_EXPORT CWGLContext : public CGLContext
{
private:

	HDC mhdc;
	HGLRC mhrc;

public:

	CWGLContext(HDC mhdc, HGLRC mhrc = NULL);
	CWGLContext();

	CWGLContext(const CWGLContext & other)
	{
		mhdc = other.mhdc;
		mhrc = other.mhrc;

#ifdef DEBUG_BUILD
		CLog & log = CLog::GetInstance();
		log.PrintMessage("(!)Copy wgl context...", CLog::LG_WITH_TIME);
#endif
	}

	~CWGLContext();

	HDC GetDeviceContext() const;

	HGLRC GetRenderContext() const;

	void SetCurrent();

	void EndCurrent();

	CGLContext * Clone();

	HGLRC CreateRenderContext(PIXELFORMATDESCRIPTOR * pfd, GLuint format);

	void DestroyThisContext();
};

}