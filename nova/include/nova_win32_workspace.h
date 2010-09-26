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
#pragma once

#include "nova_render_window.h"
#include "nova_win32_context.h"

namespace nova
{

class NOVA_EXPORT CWin32Workspace : public CRenderWindow
{
private:

	void SwitchNextContext();

	GLuint mBestPixelFormat;
	bool mBestFormatFinded;

protected:

	HWND	mHwnd;
	static HINSTANCE mModuleHandle;
	static bool mWinClassRegistred;

	void CreateWindowClass(void);

	void SetScreenSettings(void);

	void DestroyWinClass(void);

	bool CreateBestPixelFormat(PIXELFORMATDESCRIPTOR & pfd);

public:

	CWin32Workspace(nstring & name, int priority, CGLSupport * creator);

	~CWin32Workspace();

	virtual void PreUpdateAction();

	virtual void PostUpdateAction();

	void CreateRenderWindow(TWindowInitialTarget & init);

	GLuint GetBestPixelFormat();

	void Destroy();

	void SwapBuffers();

	void SaveContents(nstring & file) {}

	void PrepareTarget() {}
};




}