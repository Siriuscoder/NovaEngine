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

template <class nReal>
class CPlane: public CObjectConstructor
{
public:
    // The plane is represented as Dot(N,X) = c where N is a unit-length
    // normal vector, c is the plane constant, and X is any point on the
    // plane.  The user must ensure that the normal vector satisfies this
    // condition.

    CPlane ();  // uninitialized
    CPlane (const CPlane& rkPlane);

    // specify N and c directly
    CPlane (const CVector3<nReal>& rkNormal, nReal fConstant);

    // N is specified, c = Dot(N,P) where P is on the plane
    CPlane (const CVector3<nReal>& rkNormal, const CVector3<nReal>& rkP);

    // N = Cross(P1-P0,P2-P0)/Length(Cross(P1-P0,P2-P0)), c = Dot(N,P0) where
    // P0, P1, P2 are points on the plane.
    CPlane (const CVector3<nReal>& rkP0, const CVector3<nReal>& rkP1,
        const CVector3<nReal>& rkP2);

    // assignment
    CPlane& operator= (const CPlane& rkPlane);

    // The "positive side" of the plane is the half space to which the plane
    // normal points.  The "negative side" is the other half space.  The
    // function returns +1 for the positive side, -1 for the negative side,
    // and 0 for the point being on the plane.
    nInt32 WhichSide (const CVector3<nReal>& rkP) const;

    // Compute d = Dot(N,Q)-c where N is the plane normal and c is the plane
    // constant.  This is a signed distance.  The sign of the return value is
    // positive if the point is on the positive side of the plane, negative if
    // the point is on the negative side, and zero if the point is on the
    // plane.
    nReal DistanceTo (const CVector3<nReal>& rkQ) const;

    CVector3<nReal> Normal;
    nReal Constant;
};

typedef CPlane<nova::nReal> CPlanef;

template <class nReal>
CPlane<nReal>::CPlane ()
{
    // uninitialized
}
//----------------------------------------------------------------------------
template <class nReal>
CPlane<nReal>::CPlane (const CPlane& rkPlane)
    :
    Normal(rkPlane.Normal)
{
    Constant = rkPlane.Constant;
}
//----------------------------------------------------------------------------
template <class nReal>
CPlane<nReal>::CPlane (const CVector3<nReal>& rkNormal, nReal fConstant)
    :
    Normal(rkNormal)
{
    Constant = fConstant;
}
//----------------------------------------------------------------------------
template <class nReal>
CPlane<nReal>::CPlane (const CVector3<nReal>& rkNormal, const CVector3<nReal>& rkP)
    :
    Normal(rkNormal)
{
    Constant = rkNormal.Dot(rkP);
}
//----------------------------------------------------------------------------
template <class nReal>
CPlane<nReal>::CPlane (const CVector3<nReal>& rkP0, const CVector3<nReal>& rkP1,
    const CVector3<nReal>& rkP2)
{
    CVector3<nReal> kEdge1 = rkP1 - rkP0;
    CVector3<nReal> kEdge2 = rkP2 - rkP0;
    Normal = kEdge1.UnitCross(kEdge2);
    Constant = Normal.Dot(rkP0);
}
//----------------------------------------------------------------------------
template <class nReal>
CPlane<nReal>& CPlane<nReal>::operator= (const CPlane& rkPlane)
{
    Normal = rkPlane.Normal;
    Constant = rkPlane.Constant;
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CPlane<nReal>::DistanceTo (const CVector3<nReal>& rkP) const
{
    return Normal.Dot(rkP) - Constant;
}
//----------------------------------------------------------------------------
template <class nReal>
nInt32 CPlane<nReal>::WhichSide (const CVector3<nReal>& rkQ) const
{
    nReal fDistance = DistanceTo(rkQ);

    if (fDistance < (nReal)0.0)
    {
        return -1;
    }

    if (fDistance > (nReal)0.0)
    {
        return +1;
    }

    return 0;
}

}