﻿/***************************************************************************
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


class NOVA_EXPORT COctreeSceneManager : public CSceneManager
{
protected:

	nInt32 RenderSceneImpl(void);

	void PrepareSceneImpl(void);

	nInt32 PrepareRenderQueueImpl(void);

	void PrepareSceneFrameImpl(void);

	void BuildSceneImpl(void);

	void ClearObjectsImpl(void);

	void RenderNode(CTreeNode<CSceneNode*> *node);

	stl<nstring, std::pair<CStaticObject *, CMeshBoxPtr>>::map mObjectsMap;

public:

	COctreeSceneManager(const nstring &scene_name, const nstring & factory_name);

	~COctreeSceneManager() {}


	CTreeNode<CSceneNode*> *ConstactSpecifiedNode();

	CRenderableObject *AddRenderableResourceToScene(const nstring &resource_name);

};



}