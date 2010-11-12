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

#include "nova_octree_scene_node.h"

namespace nova
{

COctreeSceneNode::COctreeSceneNode(CSceneManager *scene) : CSceneNode(scene)
{

}

COctreeSceneNode::~COctreeSceneNode()
{

}

bool COctreeSceneNode::TestingMeshBox(void)
{
	if(mMeshBox->GetMeshDefinition().nVertexList.size() == 0)
		return false;

	if(mMeshBox->GetMeshDefinition().nIndexList.size() == 0)
		return false;

	return true;
}

void COctreeSceneNode::PrepareNodeImpl(void)
{
	if(!TestingMeshBox())
		throw NOVA_EXP("COctreeSceneNode::PrepareNode - testing mesh box faled..", BAD_OPERATION);

	// Preparing mesh
	// Generating normals to faces and sub mats info
	mMeshBox->GenerateNormalsToFaces();
	if(mMeshBox->GetMeshDefinition().nNormalList.size() == 0)
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

void COctreeSceneNode::ValidateNodeImpl(void)
{
}

void COctreeSceneNode::ReleaseNodeImpl(void)
{
}

void COctreeSceneNode::InValidateNodeImpl(void)
{
}

}