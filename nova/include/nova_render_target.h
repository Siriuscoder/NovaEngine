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
#pragma once

#include "nova_viewport.h"
#include "nova_cpu_timer.h"

namespace nova
{

class NOVA_EXPORT CRenderTargetListener : public CEventListener
{
public:

	virtual void PreRenderActionEvent(CRenderTarget *target) {}

	virtual void PostRenderActionEvent(CRenderTarget *target) {}

	virtual void DestroyActionListener(CRenderTarget *target) {}
};





class NOVA_EXPORT CRenderTarget : public CBase, public CListenerInterface
{
public:

	typedef nova::stl<nInt32, CViewPortPtr>::map TViewPortList;

protected:

	nstring mName;
	nova::nByte 	mPriority;
	nova::nUInt32 	mWidth;
	nova::nUInt32 	mHeight;
	nova::nUInt32 	mColourDepth;
	nova::nUInt32  mColourBits;
	bool 	mIsDepthBuffered;
	bool 	mActive;
	bool 	mAutoUpdate;
	bool mReady;
	CColorRGBA mBackColor;

	//ViewportList 	mViewportList
	TViewPortList mViewPortList;

	virtual void UpdateSource(); 

public:

	CRenderTarget();
	virtual ~CRenderTarget();

	CViewPortPtr AddViewport(nInt32 left, 
		nInt32 top, nInt32 width, nInt32 height, nInt32 ZOrder);

	CViewPortPtr GetViewport(nInt32 zorder);

	void RemoveAllViewPorts();

	void RemoveViewPort(nInt32 zorder);

	nova::nUInt32 GetViewPortCount();

	inline nova::nUInt32 GetWidth()
	{
		return mWidth;
	}

	inline nova::nUInt32 GetHeight()
	{
		return mHeight;
	}

	inline nova::nUInt32 GetColourDepth()
	{
		return mColourDepth;
	}

	inline nova::nUInt32 GetColourBits()
	{
		return mColourBits;
	}

	inline bool IsActive()
	{
		return mActive;
	}

	inline void SetActive(bool flag)
	{
		mActive = flag;
	}

	inline bool IsAutoUpdate()
	{
		return mAutoUpdate;
	}

	inline void SetAutoUpdate(bool flag)
	{
		mAutoUpdate = flag;
	}

	inline nova::nUInt32 GetPriority()
	{
		return mPriority;
	}

	inline void SetPriority(nova::nUInt32 pr)
	{
		mPriority = pr;
	}

	inline nstring GetTagertName()
	{
		return mName;
	}

	inline void SetTargetName(nstring & name)
	{
		mName = name;
	}

	bool IsReady();

	void Update();

	virtual void PreUpdateAction();
	virtual void PostUpdateAction();

	virtual void SwapBuffers() = 0;

	virtual void PrepareTarget() = 0;

	void SetBackColor(CColorRGBA & color);
};

typedef CSmartPtr<CRenderTarget>  CRenderTargetPtr;

}
