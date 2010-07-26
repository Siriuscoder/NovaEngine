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

#include "nova_renderable_object.h"
#include "nova_quaternion.h"
#include "nova_mesh_box.h"

namespace nova
{

class NOVA_EXPORT CMovableObject : public CRenderableObject
{
protected:

	mutable nova::Vector3f mPosition;
	mutable nova::Vector3f mScale;
	mutable nova::Quaternionf mOrientation;

public:

	CMovableObject(const nstring & name, const nstring & group);

	CMovableObject();

	virtual ~CMovableObject();

	void SetLocation(real x, real y, real z);

	void SetOrientation(const nova::Quaternionf & quat);

	void SetScale(real x, real y, real z);

	Vector3f & GetLocation(void) const;

	Quaternionf & GetOrientation(void) const;

	Vector3f & SetScale(void) const;

	void Move(real x, real y, real z);

	void Rotate(Quaternionf & quat);

	void Scale(real x, real y, real z);

	Matrix4f ComputeLocalMatrix(void);
};

}