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

#include "nova_scene_manager.h"
#include "nova_octree_scene_manager.h"

namespace nova
{

class NOVA_EXPORT CSceneScrap : public CBase
{
public:

	typedef stl<CWorldObject *>::vector TObjects;

private:

	CSceneManager * mSceneManager;
	bool mEnabled;

public:

	CSceneScrap(CSceneManager * manager, bool enabled);

	~CSceneScrap();
/*
	void AddObjects(const TObjects & obj);

	void AddObject(CWorldObject *obj);
*/

	void PrepareScrap(void);

	CSceneManager & GetSceneManager(void);

	void RenderScrap(CCamera * camera, CViewPort * view);

	bool IsEnabled(void);

	void SetEnabled(bool flag);
	
};

}