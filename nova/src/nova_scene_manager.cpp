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

#include "nova_scene_manager.h"

namespace nova
{

void CSceneNode::ValidateNode(void)
{
	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CSceneNodeListener * lis = 
			dynamic_cast<CSceneNodeListener *>(GetListener(i));
		lis->ValidateNodeListener(this);
	}

	ValidateNodeImpl();
	isValidated = true;
}

bool CSceneNode::IsValidated(void)
{
	return isValidated;
}

void CSceneNode::InValidateNode(void)
{
	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CSceneNodeListener * lis = 
			dynamic_cast<CSceneNodeListener *>(GetListener(i));
		lis->InValidateNodeListener(this);
	}

	InValidateNodeImpl();
	isValidated = false;
}

void CSceneNode::PrepareNode(void)
{
	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CSceneNodeListener * lis = 
			dynamic_cast<CSceneNodeListener *>(GetListener(i));
		lis->PrepareNodeListener(this);
	}

	PrepareNodeImpl();
}

void CSceneNode::ReleaseNode(void)
{
	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CSceneNodeListener * lis = 
			dynamic_cast<CSceneNodeListener *>(GetListener(i));
		lis->ReleaseNodeListener(this);
	}

	InValidateNode();
	ReleaseNodeImpl();
}

void CSceneNode::RenderNode(void)
{
	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CSceneNodeListener * lis = 
			dynamic_cast<CSceneNodeListener *>(GetListener(i));
		lis->RenderNodeListener(this);
	}

	RenderNodeImpl();
}

CBoundingBox &CSceneNode::GetBoundingBox(void)
{
	return mBoundingBox;
}

void CSceneNode::SetBoundingBox(const CBoundingBox &box)
{
	mBoundingBox = box;
}

void CSceneNode::BuildNode(void)
{
	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CSceneNodeListener * lis = 
			dynamic_cast<CSceneNodeListener *>(GetListener(i));
		lis->BuildNodeListener(this);
	}

	BuildNodeImpl();
}

bool CSceneNode::IsVisible(void)
{
	return  mpSceneObject.IsNull() ? false : mpSceneObject->IsVisible();
}

CSceneManager::CSceneManager(const nstring & scene_name, const nstring & group) : 
	mSceneName(scene_name), mRegisterGroup(group)
{
	isEnabled = true;
}

CSceneManager::~CSceneManager()
{
	//ReleaseObjects(); // Error R6025: Pure virual function call ..fuck =)
} 

nInt32 CSceneManager::GetRenderedBatches(void)
{
	return mRenderedBatches;
}

nInt32 CSceneManager::GetRenderedFaces(void)
{
	return mRenderedFaces;
}

void CSceneManager::RenderScene(CCamera *camera, CViewPort *view)
{
	if(!camera || !view)
		return;

	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CSceneManagerListener * lis = 
			dynamic_cast<CSceneManagerListener *>(GetListener(i));
		lis->SceneRenderBeginListener(this);
	}

	mRenderedFaces = 0;
	mRenderedBatches = 0;
	mCurCamera = camera;
	mCurView = view;

	RenderSceneImpl();
	RenderСompoundObjects();

	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CSceneManagerListener * lis = 
			dynamic_cast<CSceneManagerListener *>(GetListener(i));
		lis->SceneRenderEndListener(this);
	}
}

nstring CSceneManager::GetSceneName(void)
{
	return mSceneName;
}

CTreeNode<CSceneManager::TNodeType> *CSceneManager::GetRootElement(void)
{
	return mSceneTree.GetRootElement();
}

void CSceneManager::SetRootElement(TNodeType elem)
{
	mSceneTree.CreateRoot(elem);
}

CTree<CSceneManager::TNodeType> &CSceneManager::GetSceneTree(void)
{
	return mSceneTree;
}

nstring CSceneManager::GetSceneSlavesGroup(void)
{
	return mRegisterGroup;
}

void CSceneManager::PrepareScene(void)
{
	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CSceneManagerListener * lis = 
			dynamic_cast<CSceneManagerListener *>(GetListener(i));
		lis->PrepareSceneListener(this);
	}

	PrepareSceneImpl();
}

nInt32 CSceneManager::PrepareRenderQueue(void)
{
	return PrepareRenderQueueImpl();
}

void CSceneManager::PrepareSceneFrame(void)
{
	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CSceneManagerListener * lis = 
			dynamic_cast<CSceneManagerListener *>(GetListener(i));
		lis->PrepareSceneFrameListener(this);
	}

	PrepareSceneFrameImpl();
}

void CSceneManager::BuildScene(void)
{
	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CSceneManagerListener * lis = 
			dynamic_cast<CSceneManagerListener *>(GetListener(i));
		lis->BuildSceneListener(this);
	}

	BuildSceneImpl();
}

void CSceneManager::DestroyScene(void)
{
	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CSceneManagerListener * lis = 
			dynamic_cast<CSceneManagerListener *>(GetListener(i));
		lis->DestroySceneListener(this);
	}

	
	DestroySceneNode(GetRootElement());
	mSceneTree.FreeAll();
}

void CSceneManager::DestroySceneNode(CTreeNode<CSceneManager::TNodeType> *node)
{
	if(node)
	{
		for(nInt32 i = 0; i < node->GetChildrenLen(); i++)
			DestroySceneNode(node->GetNode(i));

		if(!node->GetData().IsNull())
		{
			node->GetData()->ReleaseNode();
		}
	}
}

CSceneManager::TNodeType CSceneManager::CreateRenderableNodeFromResource(const nstring &resource_name)
{
	return CSceneManager::TNodeType(NULL);
}

void CSceneManager::SerializeSceneToXml(xmlTextWriterPtr xmlWriter)
{
	SerializeSceneToXmlImpl(xmlWriter);
}

void CSceneManager::DeSerializeSceneFromXml(xmlNodePtr node)
{
	DeSerializeSceneFromXmlImpl(node);
}

}
