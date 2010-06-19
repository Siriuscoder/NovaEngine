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

#include "nova_tree_scene_node.h"

namespace nova
{

CMeshSceneNode::CMeshSceneNode(CSceneManager *scene, NNodeType type) : CSceneNode(scene, type)
{

}

CWorldObject* CMeshSceneNode::ConstractWorldObject(const nstring &name)
{
	if(!mParentSceneManager)
		throw NOVA_EXP("CMeshSceneNode::ConstractWorldObject - mParentSceneManager is bad ptr!", MEM_ERROR); 

	mChildObject = new CMovableObject(name, mParentSceneManager->GetSceneSlavesGroup());
	
	return mChildObject;
}

void CMeshSceneNode::SetMeshBox(CMeshBoxPtr &mesh)
{
	if(mesh.IsNull())
		throw NOVA_EXP("CMeshSceneNode::SetMeshBox - mesh is null ptr!", MEM_ERROR); 
	mMeshBox = mesh;
}

void CMeshSceneNode::SetMeshBoxFromResource(const nstring &name)
{
	CMeshBoxPtr mesh = CMeshManager::GetSingelton().GetResourceFromHash(name);

	if(mesh.IsNull())
		throw NOVA_EXP("CMeshSceneNode::SetMeshBoxFromResource - resource manager return null prt, possibly resource not found..", MEM_ERROR);

	mMeshBox = mesh;
}


bool CMeshSceneNode::TestingMeshBox(void)
{
	if(mMeshBox->GetVertexesLen() == 0)
		return false;

	if(mMeshBox->GetTrianglesLen() == 0)
		return false;

	if(mMeshBox->GetMaterials().size() == 0)
		return false;

	return true;
}

void CMeshSceneNode::PrepareNode(void)
{
	if(!TestingMeshBox())
		throw NOVA_EXP("CMeshSceneNode::PrepareNode - testing mesh box faled..", BAD_OPERATION);

	// Preparing mesh
	// Generating normals to faces and sub mats info
	mMeshBox->GenerateNormalsToFaces();
	if(mMeshBox->GetNormalsLen() == 0)
		//Generating normals to vertexes
		mMeshBox->CalculateNormals();

	// Sorting faces by material id
	// using fast qsort algorithm
	mMeshBox->SortFaceIndexByMaterials();

	// we mast construct batches list for rendering all node
	// for rendering all node we mast have one vertex buffer, but many index buffers
	// one batch - one index buffer (buffers splitting by materials and we mast switch the texture id)
	PreparingBatchList();
}

void CMeshSceneNode::PreparingBatchList()
{

}

}