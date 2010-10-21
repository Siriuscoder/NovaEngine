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

#include "nova_movable_object.h"
#include "nova_plane.h"
#include "nova_sphere.h"

namespace nova
{

class CFrustum;

class NOVA_EXPORT CFrustumListener : public CRenderableObjectListener
{
public:

	virtual void ExtractFrustumListener(CFrustum * object) {}
};

class NOVA_EXPORT CFrustum : public CMovableObject
{
protected:

	mutable Matrix4f mProjection;
	CPlanef	mFrustumPlanes[6];

	bool mActive;

public:

	CFrustum();
	~CFrustum() {}

	void ExtractFrustum();

	bool PointInFrustum(Vector3f & point);

	bool SphereInFrustum(CSpheref & sphere);

	nova::nReal DistanceInFrustumSphere(CSpheref & sphere);

	bool BoxInFrustum(CBoundingBox & box);

	void ClearFrustumPlanes();

	CPlanef GetPlane(nova::uint plane);
	
	Matrix4f & GetProjectionMatrix() const;

	void SetActive(bool flag);
	
};

}

