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
#include "nova_stable_precompiled_headers.h"

#include "nova_renderable_object.h"

namespace nova
{

CRenderableObject::CRenderableObject(const nstring & name, const nstring & group) :
	CWorldObject(name, group)
{


}


CRenderableObject::CRenderableObject()
{


}


CRenderableObject::~CRenderableObject() {}

CBoundingBox & CRenderableObject::GetBoundingBox()
{
	return mBoundingBox;
}

void CRenderableObject::SetBoundingBox(CBoundingBox & box)
{
	mBoundingBox = box;
}

void CRenderableObject::SetBoundingBoxParametres(Vector3f & xyzmin, Vector3f & xyzmax)
{
	mBoundingBox.SetVisible(false);

	mBoundingBox.CalcSides(xyzmin, xyzmax);
}

void CRenderableObject::ClearBoundingBox()
{
	mBoundingBox.NullBox();
}

void CRenderableObject::Frustumable(bool flag)
{
	mBoundingBox.SetEnabled(flag);
}

void CRenderableObject::PreRenderAction()
{
	for(nova::uint i = 0; i < GetListenersCount(); i++)
	{
		CRenderableObjectListener * lis = 
			dynamic_cast<CRenderableObjectListener *>(GetListener(i));
		lis->PreRenderActionListener(this);
	}
}

void CRenderableObject::PostRenderAction()
{
	for(nova::uint i = 0; i < GetListenersCount(); i++)
	{
		CRenderableObjectListener * lis = 
			dynamic_cast<CRenderableObjectListener *>(GetListener(i));
		lis->PostRenderActionListener(this);
	}
}



}