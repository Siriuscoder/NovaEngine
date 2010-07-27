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
	real 	mRelLeft;
	real  	mRelTop;
	real  	mRelWidth;
	real  	mRelHeight;
	int 	mActLeft;
	int 	mActTop;
	int 	mActWidth;
	int 	mActHeight;
	int 	mZOrder;

	bool mClearScr;
	bool mActive;

	TCamerasList mCameras;

	CCamera * mActiveCamera;

public:

	CViewPort(CRenderTarget * tag, int left, 
		int top, int width, int height, int ZOrder);

	~CViewPort();

	void Update();

	real 	GetLeft (void) const
	{
		return mRelLeft;
	}

	real 	GetTop (void) const
	{
		return mRelTop;
	}

	real 	GetWidth (void) const
	{
		return mRelWidth;
	}

	real 	GetHeight (void) const
	{
		return mRelLeft;
	}

	int 	GetActualLeft (void) const
	{
		return mActLeft;
	}

	int 	GetActualTop (void) const
	{
		return mActTop;
	}

	int 	GetActualWidth (void) const
	{
		return mActWidth;
	}

	int 	GetActualHeight (void) const
	{
		return mActHeight;
	}

	int		GetZOrder(void) const
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

	void SetActiveCamera(nstring & name);

	CCameraPtr GetActiveCamera(void);

	CCameraPtr GetCameraByName(nstring & name);

	CCameraPtr CreateLinearCamera(nstring & name, nova::real Left, nova::real Right, nova::real Bottom, 
		nova::real Top, nova::real ZFar, nova::real ZNear);

	CCameraPtr CreatePerspectiveCamera(nstring & name, nova::real FOV, nova::real ZFar, 
		nova::real ZNear);

	void RemoveCamera(nstring & name);

	void RemoveAllCameras(void);
};

typedef CSmartPtr<CViewPort> CViewPortPtr;

}


