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
	for(nova::uint i = 0; i < mScraps.size(); ++i)
		delete mScraps[i];
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
	stl<CSceneScrap *>::vector::iterator it = mScraps.begin();
	mRenderedBatches = 0;
	mRenderedFaces = 0;

	for(; it != mScraps.end(); ++it)
	{
		if((*it)->IsEnabled())
			(*it)->RenderScrap(camera, view);
	}
}

CSceneScrap & CScene::GetScrap(nova::uint id)
{
	if(id < mScraps.size())
		return *(mScraps[id]);

	throw NOVA_EXP("CScene::GetScrap: id out of range..", BAD_OPERATION);
}

int CScene::AddScrap(CSceneScrap *scrap)
{
	if(scrap != NULL)
	{
		mScraps.push_back(scrap);
		return mScraps.size()-1;
	}

	return -1;
}

CSceneManager *CScene::FindScene(const nstring &name)
{
	for(uint i = 0; i < mScraps.size(); i++)
	{
		if(mScraps[i]->GetSceneManager().GetSceneName() == name)
			return &(mScraps[i]->GetSceneManager());
	}

	return NULL;
}


void CScene::DeleteScrap(nova::uint id)
{
	if(id < mScraps.size())
		delete mScraps[id];

	throw NOVA_EXP("CScene::DeleteScrap: id out of range..", BAD_OPERATION);
}

int CScene::AddScrap(CSceneManager *manager)
{
	return AddScrap(new CSceneScrap(manager, true));
}

}