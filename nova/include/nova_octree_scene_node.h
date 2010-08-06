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
#include "nova_material.h"
#include "nova_hardware_texture_buffer.h"
#include "nova_hardware_vertex_buffer.h"
#include "nova_movable_object.h"
#include "nova_math.h"
#include "nova_matrix3d.h"
#include "nova_matrix4d.h"


namespace nova
{

class NOVA_EXPORT COctreeSceneNode : public CSceneNode
{
public:

	typedef struct _batch_st
	{
		CHardwareVertexBufferPtr Vbo;
		CHardwareIndexBufferPtr VboIndex;
		CMaterialPtr Material;
		int start_index;
		int end_index;
	} TBatchStruct;

	typedef stl<TBatchStruct>::vector TBatchList;

protected:

	TBatchList mBatchList;
	CMeshBoxPtr mMeshBox;

	void ValidateNodeImpl(void);
	void ReleaseNodeImpl(void);

	bool TestingMeshBox(void);

	void PreparingBatchList(void);

public:

	COctreeSceneNode(CSceneManager *scene, NNodeType type);

	~COctreeSceneNode();

	CWorldObject* ConstractWorldObject(const nstring &name);

	void BuildWorldObjectFromMeshBox(void);

	void PrepareNode(void);

	void SetMeshBox(CMeshBoxPtr &mesh);

	void SetMeshBoxFromResource(const nstring &name);

};

}