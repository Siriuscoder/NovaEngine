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
// Wild Magic Source Code
// David Eberly
// http://www.geometrictools.com
// Copyright (c) 1998-2008
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.  The license is available for reading at
// either of the locations:
//     http://www.gnu.org/copyleft/lgpl.html
//     http://www.geometrictools.com/License/WildMagicLicense.pdf
#include "nova_stable_precompiled_headers.h"

#include "nova_math.h"

namespace nova
{

template<> const real CMath<real>::EPSILON = FLT_EPSILON;
template<> const real CMath<real>::ZERO_TOLERANCE = 1e-06f;
template<> const real CMath<real>::MAX_REAL = FLT_MAX;
template<> const real CMath<real>::PI = (real)(4.0*atan(1.0));
template<> const real CMath<real>::TWO_PI = 2.0f*CMath<real>::PI;
template<> const real CMath<real>::HALF_PI = 0.5f*CMath<real>::PI;
template<> const real CMath<real>::INV_PI = 1.0f/CMath<real>::PI;
template<> const real CMath<real>::INV_TWO_PI = 1.0f/CMath<real>::TWO_PI;
template<> const real CMath<real>::DEG_TO_RAD = CMath<real>::PI/180.0f;
template<> const real CMath<real>::RAD_TO_DEG = 180.0f/CMath<real>::PI;
template<> const real CMath<real>::LN_2 = CMath<real>::Log(2.0f);
template<> const real CMath<real>::LN_10 = CMath<real>::Log(10.0f);
template<> const real CMath<real>::INV_LN_2 = 1.0f/CMath<real>::LN_2;
template<> const real CMath<real>::INV_LN_10 = 1.0f/CMath<real>::LN_10;

}