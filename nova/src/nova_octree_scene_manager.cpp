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

#include "nova_octree_scene_manager.h"
#include "nova_octree_scene_node.h"

namespace nova
{

COctreeSceneManager::COctreeSceneManager(const nstring &scene_name, const nstring & factory_name) : CSceneManager(scene_name, factory_name)
{
	
}

void COctreeSceneManager::RenderNode(CTreeNode<CSceneNode*> *node)
{
	if(node && mCurCamera)
	{
		CSceneNode *pNode = node->GetData();
		if(mCurCamera->BoxInFrustum(pNode->GetBoundingBox()))
		{
			if(!pNode->IsValidated())
				pNode->ValidateNode();

			for(int i = 0; i < node->GetChildrenLen(); i++)
				RenderNode(node->GetNode(i));
		}
	}
}

int COctreeSceneManager::RenderSceneImpl()
{
	RenderNode(mSceneTree.GetRootElement());
	return 0;
}

void COctreeSceneManager::PrepareSceneImpl(void)
{

}

int COctreeSceneManager::PrepareRenderQueueImpl(void)
{

	return 0;
}

void COctreeSceneManager::PrepareSceneFrameImpl(void)
{

}

void COctreeSceneManager::BuildSceneImpl(void)
{

}

void COctreeSceneManager::ClearObjectsImpl(void)
{

}


CTreeNode<CSceneNode*> *COctreeSceneManager::ConstactSpecifiedNode()
{
	COctreeSceneNode *pSceneNode = new COctreeSceneNode(this);
	CTreeNode<CSceneNode*> *pNode = new CTreeNode<CSceneNode*>(pSceneNode);

	return pNode;
}

}