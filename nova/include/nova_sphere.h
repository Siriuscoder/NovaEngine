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
//
// Version: 4.0.0 (2006/06/28)
#pragma once

#include "nova_vector3d.h"

namespace nova
{

template <class Real>
class CSphere: public CObjectConstructor
{
public:
    // The sphere is represented as |X-C| = R where C is the center and R is
    // the radius.

    CSphere ();  // uninitialized
    CSphere (const CVector3<Real>& rkCenter, Real fRadius);
    CSphere (const CSphere& rkSphere);

    // assignment
    CSphere& operator= (const CSphere& rkSphere);

    CVector3<Real> Center;
    Real Radius;
};

template <class Real>
CSphere<Real>::CSphere ()
{
    // uninitialized
}
//----------------------------------------------------------------------------
template <class Real>
CSphere<Real>::CSphere (const CVector3<Real>& rkCenter, Real fRadius)
    :
    Center(rkCenter),
    Radius(fRadius)
{
}
//----------------------------------------------------------------------------
template <class Real>
CSphere<Real>::CSphere (const CSphere& rkSphere)
    :
    Center(rkSphere.Center),
    Radius(rkSphere.Radius)
{
}
//----------------------------------------------------------------------------
template <class Real>
CSphere<Real>& CSphere<Real>::operator= (const CSphere& rkSphere)
{
    Center = rkSphere.Center;
    Radius = rkSphere.Radius;
    return *this;
}

typedef CSphere<nova::real> CSpheref;

}