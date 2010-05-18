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

#include "nova_error.h"
#include "nova_singelton.h"

namespace nova
{

class CRenderWindow;

class NOVA_EXPORT CGLSupport : public CSingelton<CGLSupport>,  public CBase
{
protected:

	nova::stl<nstring>::vector mExtensionsList;

	static CRenderWindow * mRootWindow;

private:

	nstring mGPUVendor;
	nstring mGlewVersion;
	nstring mGLVersion;
	nstring mRenderer;
	nstring mShadingLang;

	bool mState;

public:

	CGLSupport();
	virtual ~CGLSupport();

	bool CheckExtention(nstring & ext);

	void InitGLExtentions();

	nstring GetGPUVendor() const;

	nstring GetGlewVersion() const;

	nstring GetGLVersion() const;

	nstring GetRenderer() const;

	nstring GetShadingLangVersion() const;

	void ClearExtensionList();

	static CRenderWindow * GetRootWindow()
	{
		return mRootWindow;
	}

	bool GetState(void) const
	{
		return mState;
	}


	virtual void Start() = 0;

	virtual void Stop() = 0;

	virtual void * GetFuncAddress(const nstring & procname) = 0;

	virtual bool SupportPBuffers() = 0;

// Creating Pbuffers and Render windows //

	virtual CRenderWindow * CreateRenderWindowTarget(nstring & name, int priority) = 0;

	virtual void DestroyRootWindow();
	
};


class NOVA_EXPORT CGLContext : public CBase
{
protected:

	bool mReady;

	bool mCurrent;

public:

	CGLContext();
	virtual ~CGLContext();

	bool IsReady();

	bool IsCurrent();

	virtual void SetCurrent() = 0;

	virtual void EndCurrent() = 0;

	virtual CGLContext * Clone() = 0;
};





}