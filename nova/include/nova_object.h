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
#include "nova_matrix4d.h"
#include "nova_event_listener.h"


namespace nova
{

class CWorldObject;

class NOVA_EXPORT CWorldObjectListener : public CEventListener
{
public:

	virtual void LocationChangedListener(CWorldObject * object) {}

	virtual void OrientationChangedListener(CWorldObject * object) {}

	virtual void ScaleChangedListener(CWorldObject * object) {}
};

// CWorldObject
//		 |				\		\			\
// CRenderableObject  CLight  CFrustum	CBoundingBox
//	     |			\				|
// CMeshObject	 CPaticleSystem	  CCamera

class NOVA_EXPORT CWorldObject : public CBase, public CListenerInterface
{
protected:
	nstring mName;
	nstring mGroup;

	bool isAttached;
	bool mEnabled;
	bool isMovable;
	bool mVisible;

	bool mLocationChanged;
	bool mOrientationChanged;
	bool mScaleChanged;

	mutable nova::Matrix4f mLocalMatrix;

	void LocationChanged(void);

	void OrientationChanged(void);

	void ScaleChanged(void);

// Creator (ObjectFactory)
// SceneManager

public:

	CWorldObject(const nstring & name, const nstring & group);
	CWorldObject();
	virtual ~CWorldObject();

	nstring GetObjectName();
	nstring GetObjectGroup();

	void SetObjectName(nstring & name);
	void SetObjectGroup(nstring & group);

	bool GetAttachState();

	bool GetMovableState();

	inline bool IsEnabled()
	{
		return mEnabled;
	}

	inline bool IsVisible()
	{
		return mVisible;
	}

	void SetEnabled(bool flag);

	void SetVisible(bool flag);
};

}




