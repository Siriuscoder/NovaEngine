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
#include "nova_octree_scene_node.h"

namespace nova
{

NNodeType CSceneNode::GetNodeType(void)
{
	return mNodeType;
}

void CSceneNode::ValidateNode(void)
{
	for(nova::uint i = 0; i < GetListenersCount(); i++)
	{
		CSceneNodeListener * lis = 
			dynamic_cast<CSceneNodeListener *>(GetListener(i));
		lis->ValidateNodeListener(this);
	}

	ValidateNodeImpl();
	isValidated = true;
}

void CSceneNode::InValidateNode(void)
{
	for(nova::uint i = 0; i < GetListenersCount(); i++)
	{
		CSceneNodeListener * lis = 
			dynamic_cast<CSceneNodeListener *>(GetListener(i));
		lis->InValidateNodeListener(this);
	}

	isValidated = false;
}

void CSceneNode::ReleaseNode(void)
{
	if(mChildObject)
		delete mChildObject;
}

CSceneManager::CSceneManager(const nstring & scene_name, const nstring & group)
{
	ClearObjects();
	mRegisterGroup = group;
	mSceneName = scene_name;
}

CSceneManager::~CSceneManager()
{
	ClearObjects();
}

int CSceneManager::GetRenderedBatches(void)
{
	return mRenderedBatches;
}

int CSceneManager::GetRenderedFaces(void)
{
	return mRenderedFaces;
}

void CSceneManager::ClearObjects(void)
{

}

int CSceneManager::RenderСompoundObjects(void)
{
// to do render effects here

	return 0;
}

void CSceneManager::RenderScene(CCamera *camera, CViewPort *view)
{
	if(!camera || !view)
		return;

	for(nova::uint i = 0; i < GetListenersCount(); i++)
	{
		CSceneManagerListener * lis = 
			dynamic_cast<CSceneManagerListener *>(GetListener(i));
		lis->SceneRenderBeginListener(this);
	}

	mRenderedFaces = 0;
	mRenderedBatches = 0;
	mCurCamera = camera;
	mCurView = view;
	CRenderSystem * Renderer = NULL;

	Renderer = CRenderSystem::GetSingeltonPtr();
	Renderer->SetVeiwport((int)mCurView->GetActualLeft(), 
		(int)mCurView->GetActualTop(), 
		(int)mCurView->GetActualWidth(), 
		(int)mCurView->GetActualHeight());

	mCurCamera->Validate();

	RenderSceneImpl();

	RenderСompoundObjects();

	for(nova::uint i = 0; i < GetListenersCount(); i++)
	{
		CSceneManagerListener * lis = 
			dynamic_cast<CSceneManagerListener *>(GetListener(i));
		lis->SceneRenderEndListener(this);
	}
}

int CSceneManager::GetSceneType(void)
{
	return mSceneType;
}

nstring CSceneManager::GetSceneName(void)
{
	return mSceneName;
}

CTreeNode<CSceneNode*> *CSceneManager::ConstactSpecifiedNode(NNodeType type)
{
	return NULL;
}

CTreeNode<CSceneNode*> *CSceneManager::GetRootElement(void)
{
	return mSceneTree.GetRootElement();
}

void CSceneManager::SetRootElement(CSceneNode *elem)
{
	mSceneTree.CreateRoot(elem);
}

CTree<CSceneNode*> *CSceneManager::GetSceneTreePtr(void)
{
	return &mSceneTree;
}

nstring CSceneManager::GetSceneSlavesGroup(void)
{
	return mRegisterGroup;
}

}
