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

#include "nova_bounding_box.h"

namespace nova
{

CBoundingBox::CBoundingBox()
{
	NullBox();
}

Vector3f & CBoundingBox::GetP1(void) const
{
	return mP1;
}

Vector3f & CBoundingBox::GetP2(void) const
{
	return mP2;
}

Vector3f & CBoundingBox::GetP3(void) const
{
	return mP3;
}

Vector3f & CBoundingBox::GetP4(void) const
{
	return mP4;
}

Vector3f & CBoundingBox::GetP5(void) const
{
	return mP5;
}

Vector3f & CBoundingBox::GetP6(void) const
{
	return mP6;
}

Vector3f & CBoundingBox::GetP7(void) const
{
	return mP7;
}

Vector3f & CBoundingBox::GetP8(void) const
{
	return mP8;
}

real CBoundingBox::GetLength()
{
	return mP7.X() - mP1.X();
}

real CBoundingBox::GetHeight()
{
	return mP7.Z() - mP1.Z();
}

real CBoundingBox::GetWidth()
{
	return mP7.Y() - mP1.Y();
}

void CBoundingBox::NullBox()
{
	mP1 = Vector3f::ZERO;
	mP2 = Vector3f::ZERO;
	mP3 = Vector3f::ZERO;
	mP4 = Vector3f::ZERO;
	mP5 = Vector3f::ZERO;
	mP6 = Vector3f::ZERO;
	mP7 = Vector3f::ZERO;
	mP8 = Vector3f::ZERO;
}

void CBoundingBox::CalcSides(Vector3f & xyzmin, Vector3f & xyzmax)
{
	mP1 = xyzmin;
	mP7 = xyzmax;

	nova::real len = GetLength();
	nova::real wid = GetWidth();
	nova::real he = GetHeight();

	mP2.X() = mP1.X();
	mP2.Y() = mP1.Y();
	mP2.Z() = mP1.Z() + he;

	mP3.X() = mP1.X();
	mP3.Y() = mP1.Y() + wid;
	mP3.Z() = mP1.Z() + he;

	mP4.X() = mP1.X();
	mP4.Y() = mP1.Y() + wid;
	mP4.Z() = mP1.Z();

	mP5.X() = mP1.X() + len;
	mP5.Y() = mP1.Y();
	mP5.Z() = mP1.Z();

	mP6.X() = mP1.X() + len;
	mP6.Y() = mP1.Y();
	mP6.Z() = mP1.Z() + he;

	mP8.X() = mP1.X() + len;
	mP8.Y() = mP1.Y() + wid;
	mP8.Z() = mP1.Z();
}


}