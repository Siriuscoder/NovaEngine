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
#include "nova_sdl_support.h"

namespace nova
{

class NOVA_EXPORT CSDLWorkspace : public CRenderWindow
{
protected:

	SDL_Surface *mScreen;
	SDL_VideoInfo *mVideo;
	CSDLSupport *mSupport;

public:

	CSDLWorkspace(nstring & name, int priority, CGLSupport *creator);

	~CSDLWorkspace();

	virtual void PreUpdateAction();

	virtual void PostUpdateAction();

	void CreateRenderWindow(WindowInitialTarget & init);

	void Destroy();

	void SwapBuffers();

	void SaveContents(nstring & file);

	void PrepareTarget() {}
};


}