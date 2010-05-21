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

#include "nova_scene_scrap.h"
#include "nova_scene.h"

namespace nova
{

CSceneScrap::CSceneScrap(CSceneManager * manager, bool enabled) : CBase("CSceneScrap"), mEnabled(true)
{
	if(manager)
		mSceneManager = manager;
	else
		throw NOVA_EXP("CSceneScrap::CSceneScrap: manager is bad ptr", MEM_ERROR);
}

CSceneScrap::~CSceneScrap()
{
	if(mSceneManager)
	{
		mSceneManager->DestroyScene();
		delete mSceneManager;
	}
}


/*
void CSceneScrap::AddObjects(const TObjects & obj)
{
	TObjects::const_iterator it = obj.begin();
	for(; it != obj.end(); ++it)
	{
		if(mSceneManager)
			mSceneManager->AddSingleObject(*it);
	}
}

void CSceneScrap::AddObject(CWorldObject *obj)
{
	mSceneManager->AddSingleObject(obj);
}
*/

void CSceneScrap::PrepareScrap(void)
{
	mSceneManager->BuildScene();
}

CSceneManager & CSceneScrap::GetSceneManager(void)
{
	return *mSceneManager;
}

void CSceneScrap::RenderScrap(CCamera * camera, CViewPort * view)
{
	if(mSceneManager)
	{
		mSceneManager->PrepareScene();
		mSceneManager->RenderScene(camera, view);
		CScene::GetSingelton().mRenderedFaces = mSceneManager->GetRenderedFaces();
		CScene::GetSingelton().mRenderedBatches = mSceneManager->GetRenderedBatches();
	}
}

bool CSceneScrap::IsEnabled(void)
{
	return mEnabled;
}

void CSceneScrap::SetEnabled(bool flag)
{
	mEnabled = flag;
}

}