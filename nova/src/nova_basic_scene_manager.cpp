/***************************************************************************
 *   Copyright (C) 2011 by Sirius										   *
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

#include "nova_basic_scene_manager.h"

namespace nova
{


CBasicSceneNode::CBasicSceneNode(CMovableObject *pObj, CSceneManager *scene) :
	CSceneNode(pObj, scene)
{

}

void CBasicSceneNode::SetMeshBox(CMeshBoxPtr &mesh)
{
	mMeshBox = mesh;
}

nInt32 CBasicSceneManager::RenderSceneImpl(void)
{
	return RenderNode(GetRootElement());
}

void CBasicSceneManager::PrepareNode(CTreeNode<CSceneNode*> *node)
{
	if(node)
	{
		CBasicSceneNode *curNode = dynamic_cast<CBasicSceneNode *>(node->GetData());
		// Preparing mesh
		// Generating normals to faces and sub mats info
		curNode->GetMeshBox()->GenerateNormalsToFaces();
		if(curNode->GetMeshBox()->GetMeshDefinition().nNormalList.size() == 0)
		//Generating normals to vertexes
			curNode->GetMeshBox()->CalculateNormals();

		// Sorting faces by material id
		// using fast qsort algorithm
		curNode->GetMeshBox()->SortFaceIndexByMaterials();

		for(nInt32 i = 0; i < node->GetChildrenLen(); i++)
		{
			// next layer
			PrepareNode(node->GetNode(i));
		}
	}
}


void CBasicSceneManager::PrepareSceneImpl(void)
{
	PrepareNode(GetRootElement());
}

nInt32 CBasicSceneManager::PrepareRenderQueueImpl(void)
{

	return 0;
}

void CBasicSceneManager::PrepareSceneFrameImpl(void)
{

}

void CBasicSceneManager::BuildSceneImpl(void)
{

}

void CBasicSceneManager::ReleaseObjectsImpl(void)
{
	ReleaseNodeRenderableObject(GetRootElement());
}

void CBasicSceneManager::ReleaseNodeRenderableObject(CTreeNode<CSceneNode*> *node)
{
	if(node)
	{
		for(nInt32 i = 0; i < node->GetChildrenLen(); i++)
		{
			ReleaseNodeRenderableObject(node->GetNode(i));
		}

		delete node->GetData()->GetObjectInterface();
	}
}

nInt32 CBasicSceneManager::RenderNode(CTreeNode<CSceneNode*> *node)
{
	nova::nUInt32 nodesRenderedCount = 0;
	if(node)
	{
		for(nInt32 i = 0; i < node->GetChildrenLen(); i++)
		{
			node->GetData()->ValidateNode();
			node->GetData()->RenderNode();
			nodesRenderedCount++;

			RenderNode(node->GetNode(i));
			node->GetData()->InValidateNode();
		}
	}

	return nodesRenderedCount;
}

CSceneNode *CBasicSceneManager::AddRenderableResourceToScene(const nstring &resource_name)
{
	CMeshBoxPtr mesh = CMeshManager::GetSingelton().GetResourceFromHash(resource_name);
	if(!mesh.IsNull())
	{
		CMovableObject *obj = new CMovableObject(mesh->GetResName(), GetSceneSlavesGroup());
		CBasicSceneNode *target = new CBasicSceneNode(obj, this);
		target->SetMeshBox(mesh);

		return target;
	}

	return NULL;
}

}