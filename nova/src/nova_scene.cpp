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

#include "nova_scene.h"
#include "nova_resource_manager.h"

namespace nova
{

template<> CScene * CSingelton<CScene>::SingeltonObject = NULL;

CScene::CScene() : CBase("CScene")
{
	mScraps.clear();
}

CScene::~CScene() 
{
	DeleteAllScraps();
}

void CScene::DeleteAllScraps(void)
{
	stl<CSceneManager *>::list::iterator it = mScraps.begin();

	for(; it != mScraps.end(); it++)
		delete (*it);

	mScraps.clear();
}

int CScene::GetRenderedBatches(void)
{
	return mRenderedBatches;
}

int CScene::GetRenderedFaces(void)
{
	return mRenderedFaces;
}

void CScene::RenderAllScene(CCamera * camera, CViewPort * view)
{
	if(!camera || !view)
		return;

	stl<CSceneManager *>::list::iterator it = mScraps.begin();
	mRenderedBatches = 0;
	mRenderedFaces = 0;

	CRenderSystem * Renderer = NULL;

	Renderer = CRenderSystem::GetSingeltonPtr();
	Renderer->SetVeiwport((int)view->GetActualLeft(), 
		(int)view->GetActualTop(), 
		(int)view->GetActualWidth(), 
		(int)view->GetActualHeight());

	camera->Validate();

	for(; it != mScraps.end(); ++it)
	{
		if((*it)->IsEnabled())
		{
			(*it)->PrepareSceneFrame();
			(*it)->RenderScene(camera, view);
			mRenderedFaces += (*it)->GetRenderedFaces();
			mRenderedBatches += (*it)->GetRenderedBatches();
		}
	}
}

CSceneManager *CScene::FindScene(const nstring &name)
{
	stl<CSceneManager *>::list::iterator it = mScraps.begin();

	for(; it != mScraps.end(); it++)
	{
		if((*it)->GetSceneName() == name)
			return (*it);
	}

	return NULL;
}

int CScene::AddScrap(CSceneManager *manager)
{
	if(manager)
	{
		mScraps.push_back(manager);
		return mScraps.size()-1;
	}

	throw NOVA_EXP("CScene::AddScrap: manager pointer refer to null ptr..", MEM_ERROR);
}

int CScene::LoadSceneForce(const CFilesPackage &rPack, bool withResorces)
{

	return 0;
}

int CScene::LoadSceneForce(const nstring &pckFile, bool withResorces)
{
	return 0;
}

int CScene::LoadSceneInBackgroundMode(const CFilesPackage &rPack, bool withResorces)
{
	return 0;
}

int CScene::LoadSceneInBackgroundMode(const nstring &pckFile, bool withResorces)
{
	return 0;
}

}