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
	TObjectsMap::iterator it = mObjectsMap.begin();

	for(; it != mObjectsMap.end(); ++it)
	{
		for(nova::uint i = 0; i < (*it).second.size(); ++i)
			delete (*it).second[i];
		(*it).second.clear();
	}

	mObjectsMap.clear();
}

void CSceneManager::AttachSingleObjectToResource(CWorldObject *obj, const nstring & resource)
{
/*
	if(!obj)
		NOVA_EXP("CSceneManager::AttachSingleObject: obj is null..", MEM_ERROR);

	if(CMeshManager::GetSingelton().GetResourceFromHash(mesh).IsNull())
		NOVA_EXP("CSceneManager::AttachSingleObject: Resource factory returned null \
			pointer.. (this mesh not found)", BAD_OPERATION);

	TObjectsMap::iterator it;
	if((it = mObjectsMap.find(mesh)) != mObjectsMap.end())
		(*it).second.push_back(obj);
	else
	{
		std::pair<nstring, stl<CWorldObject *>::vector> link;
		TObjectsMap::mapped_type vec;

		link.first = mesh;

		vec.push_back(obj);
		link.second = vec;
	
		mObjectsMap.insert(link);
	}
*/
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
}

int CSceneManager::GetSceneType(void)
{
	return mSceneType;
}

nstring CSceneManager::GetSceneName(void)
{
	return mSceneName;
}

}
