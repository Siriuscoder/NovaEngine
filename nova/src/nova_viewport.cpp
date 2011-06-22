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

#include "nova_viewport.h"
#include "nova_render_target.h"
#include "nova_scene.h"

namespace nova
{

CViewPort::CViewPort(CRenderTarget * tag, nInt32 left, 
	nInt32 top, nInt32 width, nInt32 height, nInt32 ZOrder) : mTarget(tag), mActLeft(left),
	mActTop(top), mActWidth(width), mActHeight(height), mZOrder(ZOrder), mActiveCamera(NULL),
	CBase("CViewPort")
{
	mRelLeft = ((mActLeft) ? ((float)tag->GetWidth() / (float)mActLeft) : 0.0f);
	mRelTop = ((mActTop) ? ((float)tag->GetHeight() / (float)mActTop) : 0.0f);
	mRelWidth = (float)tag->GetWidth() / (float)mActWidth;
	mRelHeight = (float)tag->GetHeight() / (float)mActHeight;
	mActive = true;
}

CViewPort::~CViewPort()
{
	RemoveAllCameras();
}

void CViewPort::Update()
{
	if(IsActive())
	{
		if(mActiveCamera)
		{
			CScene::GetSingelton().RenderAllScene(mActiveCamera, this);
		}
	}
}

void CViewPort::SetActiveCamera(const nstring & name)
{
	TCamerasList::iterator it;

	if((it = mCameras.find(name)) != mCameras.end())
	{
		mActiveCamera = (*it).second.GetPtr();
	}
	else
		throw NOVA_EXP("CViewPort::SetActiveCamera - can not find camera! wrong name..", BAD_OPERATION);
}

CCameraPtr CViewPort::GetActiveCamera(void)
{
	if(mActiveCamera)
	{
		TCamerasList::iterator it;

		if((it = mCameras.find(mActiveCamera->GetObjectName())) != mCameras.end())
		{
			return (*it).second;
		}
	}
	else
		throw NOVA_EXP("CViewPort::GetActiveCamera - Active Camera not exist..", BAD_OPERATION);

	return CCameraPtr();
}

CCameraPtr CViewPort::GetCameraByName(const nstring & name)
{
	TCamerasList::iterator it;

	if((it = mCameras.find(name)) != mCameras.end())
	{
		return (*it).second;
	}
	else
		throw NOVA_EXP("CViewPort::GetCameraByName - Can not find this camera..", BAD_OPERATION);
}

CCameraPtr CViewPort::CreateLinearCamera(const nstring & name, nova::nReal Left, nova::nReal Right, 
	nova::nReal Bottom, nova::nReal Top, nova::nReal ZFar, nova::nReal ZNear)
{
	mActiveCamera = new CCamera(Left, Right, Bottom, Top, ZFar, ZNear, this);
	LOG_MESSAGE(nstring("Apply linear camera ") + name);

	CCameraPtr camera(mActiveCamera);
	camera->SetObjectName(name);
	camera->SetObjectGroup("Linear camera");

	pair<nstring, CCameraPtr> l_pair;
	l_pair.first = name;
	l_pair.second = camera;

	mCameras.insert(l_pair);
	return camera;
}


CCameraPtr CViewPort::CreatePerspectiveCamera(const nstring & name, nova::nReal FOV, nova::nReal ZFar, 
	nova::nReal ZNear)
{
	mActiveCamera = new CCamera(FOV, ZFar, ZNear, this);
	LOG_MESSAGE(nstring("Apply perspective camera ") + name);

	CCameraPtr camera(mActiveCamera);
	camera->SetObjectName(name);
	camera->SetObjectGroup("Perspective camera");

	pair<nstring, CCameraPtr> l_pair;
	l_pair.first = name;
	l_pair.second = camera;

	mCameras.insert(l_pair);
	return camera;
}

void CViewPort::RemoveCamera(const nstring & name)
{
	TCamerasList::iterator it;

	if((it = mCameras.find(name)) != mCameras.end())
	{
		(*it).second.Free();
		mCameras.erase(it);
	}
	else
		throw NOVA_EXP("CViewPort::RemoveCamera - Can not find this camera..", BAD_OPERATION);
}

void CViewPort::RemoveAllCameras(void)
{
	TCamerasList::iterator it;
	it = mCameras.begin();

	for(; it != mCameras.end(); ++it)
	{
		(*it).second.Free();
	}

	mCameras.clear();
}

}
