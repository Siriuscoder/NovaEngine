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

#include "nova_movable_object.h"

namespace nova
{

CMovableObject::CMovableObject(const nstring & name, const nstring & group) :
	CRenderableObject(name, group)
{
	mPosition = Vector3f::ZERO;
	mOrientation = Quaternionf::IDENTITY;
	mScale = Vector3f::ONE;
}

CMovableObject::CMovableObject()
{
	mPosition = Vector3f::ZERO;
	mOrientation = Quaternionf::IDENTITY;
	mScale = Vector3f::ONE;
}

CMovableObject::~CMovableObject()
{

}

void CMovableObject::SetLocation(real x, real y, real z)
{
	mPosition = nova::Vector3f(x, y, z);

	LocationChanged();
}

void CMovableObject::SetOrientation(const nova::Quaternionf & quat)
{
	mOrientation = quat;

	OrientationChanged();
}

void CMovableObject::SetScale(real x, real y, real z)
{
	mScale = nova::Vector3f(x, y, z);

	ScaleChanged();
}

Vector3f & CMovableObject::GetLocation(void) const
{
	return mPosition;
}

Quaternionf & CMovableObject::GetOrientation(void) const
{
	return mOrientation;
}

Vector3f & CMovableObject::SetScale(void) const
{
	return mScale;
}

void CMovableObject::Move(real x, real y, real z)
{
	mPosition += Vector3f(x, y, z);

	LocationChanged();
}

void CMovableObject::Rotate(Quaternionf & quat)
{
	mOrientation = mOrientation * quat;
	mOrientation.Normalize();

	OrientationChanged();
}

void CMovableObject::Scale(real x, real y, real z)
{
	mScale = Vector3f(x, y, z);

	ScaleChanged();
}

Matrix4f CMovableObject::ComputeLocalMatrix(void)
{
	nova::Matrix3f temp;
	nova::Vector3f vec;
	nova::Matrix4f transform;

	mOrientation.ToRotationMatrix(temp);


	for(int i = 0; i < 4; ++i)
	{
		if(i < 3)
		{
			vec = temp.GetRow(i);
			mLocalMatrix.SetRow(i, nova::Vector4f(
				vec.X(), vec.Y(), vec.Z(), 0.0f));
		}
		else
		{
			Vector4f vec4(0.0f, 0.0f, 0.0f, 1.0f);
			mLocalMatrix.SetRow(i, vec4);
		}
	}

	transform.SetRow(0, nova::Vector4f(mScale.X(), 0.0f, 0.0f, 0.0f));
	transform.SetRow(1, nova::Vector4f(0.0f, mScale.Y(), 0.0f, 0.0f));
	transform.SetRow(2, nova::Vector4f(0.0f, 0.0f, mScale.Z(), 0.0f));
	transform.SetRow(3, nova::Vector4f(0.0f, 0.0f, 0.0f, 1.0f));
	mLocalMatrix = mLocalMatrix * transform;

	transform.SetRow(0, nova::Vector4f(1.0f, 0.0f, 0.0f, 0.0f));
	transform.SetRow(1, nova::Vector4f(0.0f, 1.0f, 0.0f, 0.0f));
	transform.SetRow(2, nova::Vector4f(0.0f, 0.0f, 1.0f, 0.0f));
	transform.SetRow(3, nova::Vector4f(mPosition.X(), 
	mPosition.Y(), 
	mPosition.Z(), 1.0f));
	mLocalMatrix = mLocalMatrix * transform;


	mLocationChanged = false;
	mScaleChanged = false;
	mOrientationChanged = false;

	return mLocalMatrix;
}

}