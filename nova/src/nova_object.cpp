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

#include "nova_object.h"

namespace nova
{


CWorldObject::CWorldObject(const nstring & name, const nstring & group) : isAttached(true),
	mEnabled(true), isMovable(true), mVisible(true), mLocationChanged(false),
	mOrientationChanged(false), mScaleChanged(false),
	CBase("CWorldObject")
{
	mName = name;
	mGroup = group;

	mLocalMatrix = Matrix4f::IDENTITY;
}

CWorldObject::CWorldObject() : isAttached(true),
	mEnabled(true), isMovable(true), mVisible(true), mLocationChanged(false),
	mOrientationChanged(false), mScaleChanged(false),
	CBase("CWorldObject")
{
	mName.clear();
	mGroup.clear();

	mLocalMatrix = Matrix4f::IDENTITY;
}

CWorldObject::~CWorldObject() {}

nstring CWorldObject::GetObjectName()
{
	return mName;
}

nstring CWorldObject::GetObjectGroup()
{
	return mGroup;
}

void CWorldObject::SetObjectName(nstring & name)
{
	mName = name;
}

void CWorldObject::SetObjectGroup(nstring & group)
{
	mGroup = group;
}

bool CWorldObject::GetAttachState()
{
	return isAttached;
}

bool CWorldObject::GetMovableState()
{
	return isMovable;
}

void CWorldObject::SetEnabled(bool flag)
{
	isMovable = flag;
}

void CWorldObject::SetVisible(bool flag)
{
	mVisible = flag;
}

/*
void CWorldObject::ExtractLocalMatrix()
{
	glPushAttrib(GL_TRANSFORM_BIT);

	glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadMatrixf(mLocalMatrix);

		switch(state)
		{
		case TR_TRANSFORM: 
			glTranslatef(mLocalPosition.X(), mLocalPosition.Y(),
				mLocalPosition.Z());
			break;
		case TR_ROTATE:
			{
				glRotatef(mLocalRotate.X(), 1, 0, 0);
				glRotatef(mLocalRotate.Y(), 0, 1, 0);
				glRotatef(mLocalRotate.Z(), 0, 0, 1);
			}
			break;
		case TR_SCALE:
			glScalef(mLocalScale.X(), mLocalScale.Y(),
				mLocalScale.Z());
			break;
		}

		glGetFloatv(GL_MODELVIEW_MATRIX, mLocalMatrix);
		glPopMatrix();

	glPopAttrib();


	nova::Matrix4f transform(
		CMathf::Cos(mLocalRotate.X()) * CMathf::Cos(mLocalRotate.Y()),

		CMathf::Cos(mLocalRotate.X()) * CMathf::Sin(mLocalRotate.Y()) * CMathf::Sin(mLocalRotate.Z()) - 
		CMathf::Sin(mLocalRotate.X()) * CMathf::Cos(mLocalRotate.Z()),

		CMathf::Cos(mLocalRotate.X()) * CMathf::Sin(mLocalRotate.Y()) * CMathf::Cos(mLocalRotate.Z()) + 
		CMathf::Sin(mLocalRotate.X()) * CMathf::Sin(mLocalRotate.Z()),

		CMathf::Cos(mLocalRotate.X()) * CMathf::Cos(mLocalRotate.Y()),

	nova::Matrix4f m_transform = nova::Matrix4f::IDENTITY;

	if(mLocationChanged)
	{
		nova::Matrix4f transform;
		transform.SetRow(0, nova::Vector4f(1.0f, 0.0f, 0.0f, 0.0f));
		transform.SetRow(1, nova::Vector4f(0.0f, 1.0f, 0.0f, 0.0f));
		transform.SetRow(2, nova::Vector4f(0.0f, 0.0f, 1.0f, 0.0f));
		transform.SetRow(3, nova::Vector4f(mLocalPosition.X(), 
			mLocalPosition.Y(), 
			mLocalPosition.Z(), 1.0f));
		
		m_transform = m_transform * transform;
		mLocationChanged = false;
	}

	if(mScaleChanged)
	{
		nova::Matrix4f transform;
		transform.SetRow(0, nova::Vector4f(mLocalScale.X(), 0.0f, 0.0f, 0.0f));
		transform.SetRow(1, nova::Vector4f(0.0f, mLocalScale.Y(), 0.0f, 0.0f));
		transform.SetRow(2, nova::Vector4f(0.0f, 0.0f, mLocalScale.Z(), 0.0f));
		transform.SetRow(3, nova::Vector4f(0.0f, 0.0f, 0.0f, 1.0f));

		m_transform = m_transform * transform;
		mScaleChanged = false;
	}

	if(mOrientationChanged)
	{
		nova::Matrix4f transform;
		transform.SetRow(0, nova::Vector4f(
			CMathf::Cos(mLocalRotate.X()) * CMathf::Cos(mLocalRotate.Y()),
			CMathf::Cos(mLocalRotate.X()) * CMathf::Sin(mLocalRotate.Y()) * CMathf::Sin(mLocalRotate.Z()) - 
			CMathf::Sin(mLocalRotate.X()) * CMathf::Cos(mLocalRotate.Z()),
			CMathf::Cos(mLocalRotate.X()) * CMathf::Sin(mLocalRotate.Y()) * CMathf::Cos(mLocalRotate.Z()) + 
			CMathf::Sin(mLocalRotate.X()) * CMathf::Sin(mLocalRotate.Z()), 0.0f));
		transform.SetRow(0, nova::Vector4f(
			CMathf::Sin(mLocalRotate.X()) * CMathf::Cos(mLocalRotate.Y()),
			CMathf::Sin(mLocalRotate.X()) * CMathf::Sin(mLocalRotate.Y()) * CMathf::Sin(mLocalRotate.Z()) + 
			CMathf::Cos(mLocalRotate.X()) * CMathf::Cos(mLocalRotate.Z()),
			CMathf::Sin(mLocalRotate.X()) * CMathf::Sin(mLocalRotate.Y()) * CMathf::Cos(mLocalRotate.Z()) -
			CMathf::Cos(mLocalRotate.X()) * CMathf::Sin(mLocalRotate.Z()), 0.0f));
		transform.SetRow(0, nova::Vector4f(
			- CMathf::Sin(mLocalRotate.Y()),
			CMathf::Cos(mLocalRotate.Y()) * CMathf::Sin(mLocalRotate.Z()),
			CMathf::Cos(mLocalRotate.Y()) * CMathf::Cos(mLocalRotate.Z()), 0.0f));
		transform.SetRow(0, nova::Vector4f(0.0f, 0.0f, 0.0f, 1.0f));
		
		m_transform = m_transform * transform;
		mOrientationChanged = false;
	}

	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CWorldObjectListener * lis = 
			dynamic_cast<CWorldObjectListener *>(GetListener(i));
		lis->ExtractLocalMatrixListener(this);
	}

	MatrixExtractionType(m_transform);
}
*/

void CWorldObject::LocationChanged(void)
{
	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CWorldObjectListener * lis = 
			dynamic_cast<CWorldObjectListener *>(GetListener(i));
		lis->LocationChangedListener(this);
	}

	mLocationChanged = true;
}


void CWorldObject::OrientationChanged(void)
{
	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CWorldObjectListener * lis = 
			dynamic_cast<CWorldObjectListener *>(GetListener(i));
		lis->OrientationChangedListener(this);
	}

	mOrientationChanged = true;
}

void CWorldObject::ScaleChanged(void)
{
	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CWorldObjectListener * lis = 
			dynamic_cast<CWorldObjectListener *>(GetListener(i));
		lis->ScaleChangedListener(this);
	}

	mScaleChanged = true;
}

}