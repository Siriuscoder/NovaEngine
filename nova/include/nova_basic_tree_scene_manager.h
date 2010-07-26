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

#include "nova_scene_manager.h"

namespace nova
{

class CManualSceneManager;

class NOVA_EXPORT CManualSceneManagerListener : public CSceneManagerListener
{
public:

	virtual void PrepareSceneListener(CManualSceneManager *scene) {}

	virtual void PrepareSceneFrameListener(CManualSceneManager *scene) {}

	virtual void BuildSceneManager(CManualSceneManager *scene) {}

	virtual void DestroySceneListener(CManualSceneManager *scene) {}
};


class NOVA_EXPORT CManualSceneManager : public CSceneManager
{
protected:

	void RenderSceneImpl(void);

public:

	CManualSceneManager(const nstring &scene_name, const nstring & factory_name) : CSceneManager(scene_name, factory_name) {}

	~CManualSceneManager();

	void PrepareScene(void);

	void PrepareRenderQueue(void);

	void PrepareSceneFrame(void);

	void BuildScene(void);

	void DestroyScene(void);
};



}