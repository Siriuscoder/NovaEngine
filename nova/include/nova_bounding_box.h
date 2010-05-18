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

#include "nova_object.h"

namespace nova
{

class NOVA_EXPORT CBoundingBox : public CWorldObject
{
private:

	mutable Vector3f mP1;
	mutable Vector3f mP2;
	mutable Vector3f mP3;
	mutable Vector3f mP4;
	mutable Vector3f mP5;
	mutable Vector3f mP6;
	mutable Vector3f mP7;
	mutable Vector3f mP8;

	/* Mesh */

public:

	CBoundingBox();
	~CBoundingBox() {}

	void CalcSides(Vector3f & xyzmin, Vector3f & xyzmax);

	Vector3f & GetP1(void) const;

	Vector3f & GetP2(void) const;

	Vector3f & GetP3(void) const;

	Vector3f & GetP4(void) const;

	Vector3f & GetP5(void) const;

	Vector3f & GetP6(void) const;

	Vector3f & GetP7(void) const;

	Vector3f & GetP8(void) const;

	real GetLength();

	real GetHeight();

	real GetWidth();

	void NullBox();

};



}