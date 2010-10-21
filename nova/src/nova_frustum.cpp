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

#include "nova_frustum.h"

namespace nova
{

CFrustum::CFrustum()
{
	ClearFrustumPlanes();
	mProjection = Matrix4f::IDENTITY;
}

void CFrustum::ExtractFrustum()
{
	if(!mActive)
		return;

	Matrix4f clip = Matrix4f::IDENTITY;
	nova::nReal len;

	clip = mProjection * mLocalMatrix;

   /* Находим A, B, C, D для ПРАВОЙ плоскости */
	mFrustumPlanes[0].Normal[0] = clip(0, 3) - clip(0, 0);
	mFrustumPlanes[0].Normal[1] = clip(1, 3) - clip(1, 0);
	mFrustumPlanes[0].Normal[2] = clip(2, 3) - clip(2, 0);
	mFrustumPlanes[0].Constant =  clip(3, 3) - clip(3, 0);

	/* Приводим уравнение плоскости к нормальному виду */
	len = mFrustumPlanes[0].Normal.Normalize();
	mFrustumPlanes[0].Constant /= len;

	/* Находим A, B, C, D для ЛЕВОЙ плоскости */
	mFrustumPlanes[1].Normal[0] = clip(0, 3) + clip(0, 0);
	mFrustumPlanes[1].Normal[1] = clip(1, 3) + clip(1, 0);
	mFrustumPlanes[1].Normal[2] = clip(2, 3) + clip(2, 0);
	mFrustumPlanes[1].Constant  = clip(3, 3) + clip(3, 0);

	/* Приводим уравнение плоскости к нормальному виду */
	len = mFrustumPlanes[1].Normal.Normalize();
	mFrustumPlanes[1].Constant /= len;

	/* Находим A, B, C, D для НИЖНЕЙ плоскости */
	mFrustumPlanes[2].Normal[0] = clip(0, 3) + clip(0, 1);
	mFrustumPlanes[2].Normal[1] = clip(1, 3) + clip(1, 1);
	mFrustumPlanes[2].Normal[2] = clip(2, 3) + clip(2, 1);
	mFrustumPlanes[2].Constant  = clip(3, 3) + clip(3, 1);

	/* Приводим уравнение плоскости к нормальному */
	len = mFrustumPlanes[2].Normal.Normalize();
	mFrustumPlanes[2].Constant /= len;

	/* ВЕРХНЯЯ плоскость */
	mFrustumPlanes[3].Normal[0] = clip(0, 3) - clip(0, 1);
	mFrustumPlanes[3].Normal[1] = clip(1, 3) - clip(1, 1);
	mFrustumPlanes[3].Normal[2] = clip(2, 3) - clip(2, 1);
	mFrustumPlanes[3].Constant  = clip(3, 3) - clip(3, 1);

	/* Нормальный вид */
	len = mFrustumPlanes[3].Normal.Normalize();
	mFrustumPlanes[3].Constant /= len;

	/* ЗАДНЯЯ плоскость */
	mFrustumPlanes[4].Normal[0] = clip(0, 3) - clip(0, 2);
	mFrustumPlanes[4].Normal[1] = clip(1, 3) - clip(1, 2);
	mFrustumPlanes[4].Normal[2] = clip(2, 3) - clip(2, 2);
	mFrustumPlanes[4].Constant  = clip(3, 3) - clip(3, 2);

	/* Нормальный вид */
	len = mFrustumPlanes[4].Normal.Normalize();
	mFrustumPlanes[4].Constant /= len;

	/* ПЕРЕДНЯЯ плоскость */
	mFrustumPlanes[5].Normal[0] = clip(0, 3) + clip(0, 2);
	mFrustumPlanes[5].Normal[1] = clip(1, 3) + clip(1, 2);
	mFrustumPlanes[5].Normal[2] = clip(2, 3) + clip(2, 2);
	mFrustumPlanes[5].Constant  = clip(3, 3) + clip(3, 2);

	/* Нормальный вид */
	len = mFrustumPlanes[5].Normal.Normalize();
	mFrustumPlanes[5].Constant /= len;

	for(nova::uint i = 0; i < GetListenersCount(); i++)
	{
		CFrustumListener * lis = 
			dynamic_cast<CFrustumListener *>(GetListener(i));
		lis->ExtractFrustumListener(this);
	}
}

bool CFrustum::PointInFrustum(Vector3f & point)
{
	if(!mActive)
		return true;

	for(int face = 0; face < 6; face++)
	{
		if(mFrustumPlanes[face].WhichSide(point) <= 0)
			return false;
	}

	return true;
}

bool CFrustum::SphereInFrustum(CSpheref & sphere)
{
	if(!mActive)
		return true;

	for(int face = 0; face < 6; face++)
	{
		if(mFrustumPlanes[face].DistanceTo(sphere.Center) <= -sphere.Radius)
			return false;
	}

	return true;
}

nova::nReal CFrustum::DistanceInFrustumSphere(CSpheref & sphere)
{
	if(!mActive)
		return true;

	nova::nReal dist;

	for(int face = 0; face < 6; face++)
	{
		dist = mFrustumPlanes[face].DistanceTo(sphere.Center);
		if(dist <= -sphere.Radius)
			return 0;
	}

	return dist + sphere.Radius;
}

bool CFrustum::BoxInFrustum(CBoundingBox & box)
{
	if(!mActive)
		return true;

	for(int face = 0; face < 6; face++)
	{
		if(mFrustumPlanes[face].WhichSide(box.GetP1()) > 0)
			continue;
		if(mFrustumPlanes[face].WhichSide(box.GetP2()) > 0)
			continue;
		if(mFrustumPlanes[face].WhichSide(box.GetP3()) > 0)
			continue;
		if(mFrustumPlanes[face].WhichSide(box.GetP4()) > 0)
			continue;
		if(mFrustumPlanes[face].WhichSide(box.GetP5()) > 0)
			continue;
		if(mFrustumPlanes[face].WhichSide(box.GetP6()) > 0)
			continue;
		if(mFrustumPlanes[face].WhichSide(box.GetP7()) > 0)
			continue;
		if(mFrustumPlanes[face].WhichSide(box.GetP8()) > 0)
			continue;

		return false;
	}

	return true;
}

void CFrustum::ClearFrustumPlanes()
{
	for(int i = 0; i < 6; ++i)
	{
		mFrustumPlanes[i].Normal = Vector3f::ZERO;
		mFrustumPlanes[i].Constant = 0;
	}
}

CPlanef CFrustum::GetPlane(nova::uint plane)
{
	if(plane >= 6)
		throw NOVA_EXP("CFrustum::GetPlane - plane must be in 0..5.", BAD_OPERATION);

	return mFrustumPlanes[plane];
}
	
Matrix4f & CFrustum::GetProjectionMatrix() const
{
	return mProjection;
}

void CFrustum::SetActive(bool flag)
{
	mActive = flag;
}


}

