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
#include "nova_stable_precompiled_headers.h"

#include "nova_octree_scene_node.h"

namespace nova
{

COctreeSceneNode::COctreeSceneNode(CSceneManager *scene) : CSceneNode(scene)
{

}

CWorldObject* COctreeSceneNode::ConstractWorldObject(const nstring &name)
{
	if(!mParentSceneManager)
		throw NOVA_EXP("COctreeSceneNode::ConstractWorldObject - mParentSceneManager is bad ptr!", MEM_ERROR); 

	mChildObject = new CMovableObject(name, mParentSceneManager->GetSceneSlavesGroup());
	
	return mChildObject;
}

void COctreeSceneNode::SetMeshBox(CMeshBoxPtr &mesh)
{
	if(mesh.IsNull())
		throw NOVA_EXP("COctreeSceneNode::SetMeshBox - mesh is null ptr!", MEM_ERROR); 
	mMeshBox = mesh;
}

void COctreeSceneNode::SetMeshBoxFromResource(const nstring &name)
{
	CMeshBoxPtr mesh = CMeshManager::GetSingelton().GetResourceFromHash(name);

	if(mesh.IsNull())
		throw NOVA_EXP("COctreeSceneNode::SetMeshBoxFromResource - resource manager return null prt, possibly resource not found..", MEM_ERROR);

	mMeshBox = mesh;
}


bool COctreeSceneNode::TestingMeshBox(void)
{
	if(mMeshBox->GetVertexesLen() == 0)
		return false;

	if(mMeshBox->GetTrianglesLen() == 0)
		return false;

	if(mMeshBox->GetMaterials().size() == 0)
		return false;

	return true;
}

void COctreeSceneNode::PrepareNode(void)
{
	if(!TestingMeshBox())
		throw NOVA_EXP("COctreeSceneNode::PrepareNode - testing mesh box faled..", BAD_OPERATION);

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

void COctreeSceneNode::PreparingBatchList()
{
/*
Задача разрубить целиковый объект на конечное число кусков,
да и таких, чтобы кусок выводился за одино обращение к вертексному буферу
вотс мои сообрежения:

Объект целиком в VBO буфере
|--------------------------------------------------------------|
-> массив индексов
|-----------------------------------------| (каждый элемент указывает на 3 точки из массива геометрии, треугольник)
Нужно:
1. Отсортировать массив индексов по материалам
2. Залить геометрию, текстурные координаты, индексы в VBO
3. Выделить начальные конечные участки с непрерывным состоянием буфера
-> |--------|  |--------------------|  |---|
4. Каждый из них будет отдельным батчем, создаем батч лист.
5. Рисуем это дело

*/
}

}