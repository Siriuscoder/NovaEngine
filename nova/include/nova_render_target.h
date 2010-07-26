п»ї/***************************************************************************
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

	typedef nova::stl<int, CViewPortPtr>::map TViewPortList;

protected:

	nstring mName;
	nova::byte 	mPriority;
	nova::uint 	mWidth;
	nova::uint 	mHeight;
	nova::uint 	mColourDepth;
	nova::uint  mColourBits;
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

	CViewPortPtr AddViewport(int left, 
		int top, int width, int height, int ZOrder);

	CViewPortPtr GetViewport(int zorder);

	void RemoveAllViewPorts();

	void RemoveViewPort(int zorder);

	nova::uint GetViewPortCount();

	inline nova::uint GetWidth()
	{
		return mWidth;
	}

	inline nova::uint GetHeight()
	{
		return mHeight;
	}

	inline nova::uint GetColourDepth()
	{
		return mColourDepth;
	}

	inline nova::uint GetColourBits()
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

	inline nova::uint GetPriority()
	{
		return mPriority;
	}

	inline void SetPriority(nova::uint pr)
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
