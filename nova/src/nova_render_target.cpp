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

#include "nova_render_target.h"
#include "nova_engine.h"

namespace nova
{


CRenderTarget::CRenderTarget() : mPriority(0), mWidth(0), 
	mHeight(0), mColourDepth(0), mColourBits(0), mIsDepthBuffered(true),
	mActive(true), mAutoUpdate(true), CBase("CRenderTarget")
{
	mName.clear();
	mBackColor = CColorRGBA::BLACK;
}

CViewPortPtr CRenderTarget::AddViewport(nInt32 left, 
		nInt32 top, nInt32 width, nInt32 height, nInt32 ZOrder)
{
	CViewPortPtr viewport(new CViewPort(this, left, top,
		width, height, ZOrder));

	pair<nInt32, CViewPortPtr> view;
	view.first = ZOrder;
	view.second = viewport;

	mViewPortList.insert(view);

	return viewport;
}

void CRenderTarget::RemoveAllViewPorts()
{
	TViewPortList::iterator it;

	for(it = mViewPortList.begin(); it != mViewPortList.end(); ++it)
		(*it).second.Free();

	mViewPortList.clear();
}

void CRenderTarget::RemoveViewPort(nInt32 zorder)
{
	TViewPortList::iterator it;

	if((it = mViewPortList.find(zorder)) != mViewPortList.end())
	{
		(*it).second.Free();
		mViewPortList.erase(zorder);
	}
	else
		throw NOVA_EXP("CRenderTarget::RemoveViewPort - viewport on this zorder not exist..", BAD_OPERATION);
}

CViewPortPtr CRenderTarget::GetViewport(nInt32 zorder)
{
	stl<nInt32, CViewPortPtr>::map::iterator it;

	if((it = mViewPortList.find(zorder)) != mViewPortList.end())
		return (*it).second;

	else
		throw NOVA_EXP("CRenderTarget::GetViewport - viewport on this zorder not exist..", BAD_OPERATION);

	return CViewPortPtr();
}

void CRenderTarget::UpdateSource()
{
// Updating all viewports
	stl<nInt32, CViewPortPtr>::map::iterator it;

	CRenderSystem::GetSingelton().ClearBackBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, mBackColor);

	it = mViewPortList.begin();
	//mStats.rendered_triangles = 0;
	CNovaEngine::GetSingelton().GetStatsPtr().rendered_triangles = 0;

	for(; it != mViewPortList.end(); ++it)
	{
		(*it).second->Update();
		CNovaEngine::GetSingelton().GetStatsPtr().rendered_triangles += 
			CScene::GetSingelton().GetRenderedFaces();
		CNovaEngine::GetSingelton().GetStatsPtr().rendered_batches += 
			CScene::GetSingelton().GetRenderedBatches();
	}
}

CRenderTarget::~CRenderTarget()
{
	//ResetStats();
	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CRenderTargetListener * lis = 
			dynamic_cast<CRenderTargetListener *>(GetListener(i));
		lis->DestroyActionListener(this);
	}
}

void CRenderTarget::Update()
{
	if(IsActive())
	{
		PreUpdateAction();

		UpdateSource();

		PostUpdateAction();

		SwapBuffers();
	}
}

nova::nUInt32 CRenderTarget::GetViewPortCount()
{
	return mViewPortList.size();
}

bool CRenderTarget::IsReady()
{
	return this->mReady;
}

void CRenderTarget::PreUpdateAction()
{
	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CRenderTargetListener * lis = 
			dynamic_cast<CRenderTargetListener *>(GetListener(i));
		lis->PreRenderActionEvent(this);
	}
}

void CRenderTarget::PostUpdateAction()
{	
	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CRenderTargetListener * lis = 
			dynamic_cast<CRenderTargetListener *>(GetListener(i));
		lis->PostRenderActionEvent(this);
	}
}

void CRenderTarget::SetBackColor(CColorRGBA & color)
{
	mBackColor = color;
}

}