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

#include "nova_camera.h"

namespace nova
{

class CRenderTarget;

class NOVA_EXPORT CViewPort : public CBase
{
public:

	typedef stl<nstring, CCameraPtr>::map TCamerasList; 
protected:

//mCamera
	CRenderTarget * 	mTarget;
	nReal 	mRelLeft;
	nReal  	mRelTop;
	nReal  	mRelWidth;
	nReal  	mRelHeight;
	nInt32 	mActLeft;
	nInt32 	mActTop;
	nInt32 	mActWidth;
	nInt32 	mActHeight;
	nInt32 	mZOrder;

	bool mClearScr;
	bool mActive;

	TCamerasList mCameras;

	CCamera * mActiveCamera;

public:

	CViewPort(CRenderTarget * tag, nInt32 left, 
		nInt32 top, nInt32 width, nInt32 height, nInt32 ZOrder);

	~CViewPort();

	void Update();

	nReal 	GetLeft (void) const
	{
		return mRelLeft;
	}

	nReal 	GetTop (void) const
	{
		return mRelTop;
	}

	nReal 	GetWidth (void) const
	{
		return mRelWidth;
	}

	nReal 	GetHeight (void) const
	{
		return mRelLeft;
	}

	nInt32 	GetActualLeft (void) const
	{
		return mActLeft;
	}

	nInt32 	GetActualTop (void) const
	{
		return mActTop;
	}

	nInt32 	GetActualWidth (void) const
	{
		return mActWidth;
	}

	nInt32 	GetActualHeight (void) const
	{
		return mActHeight;
	}

	nInt32		GetZOrder(void) const
	{
		return mZOrder;
	}

	inline void SetClearScr(bool flag)
	{
		mClearScr = flag;
	}

	inline void SetActive(bool flag)
	{
		mActive = flag;
	}

	inline bool IsActive()
	{
		return mActive;
	}

	void SetActiveCamera(const nstring & name);

	CCameraPtr GetActiveCamera(void);

	CCameraPtr GetCameraByName(const nstring & name);

	CCameraPtr CreateLinearCamera(const nstring & name, nova::nReal Left, nova::nReal Right, nova::nReal Bottom, 
		nova::nReal Top, nova::nReal ZFar, nova::nReal ZNear);

	CCameraPtr CreatePerspectiveCamera(const nstring & name, nova::nReal FOV, nova::nReal ZFar, 
		nova::nReal ZNear);

	void RemoveCamera(const nstring & name);

	void RemoveAllCameras(void);
};

typedef CSmartPtr<CViewPort> CViewPortPtr;

}


