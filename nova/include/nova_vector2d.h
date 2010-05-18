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
#pragma once

#include "nova_math.h"

namespace nova
{

template <class Real>
class CVector2 : public CObjectConstructor
{
public:
    // construction
	CVector2 ();  // uninitialized
    CVector2 (Real fX, Real fY);
    CVector2 (const Real* afTuple);
    CVector2 (const CVector2& rkV);

    // coordinate access
    inline operator const Real* () const;
    inline operator Real* ();
    inline Real operator[] (int i) const;
    inline Real& operator[] (int i);
    inline Real X () const;
    inline Real& X ();
    inline Real Y () const;
    inline Real& Y ();

    // assignment
    inline CVector2& operator= (const CVector2& rkV);

    // comparison
    bool operator== (const CVector2& rkV) const;
    bool operator!= (const CVector2& rkV) const;
    bool operator<  (const CVector2& rkV) const;
    bool operator<= (const CVector2& rkV) const;
    bool operator>  (const CVector2& rkV) const;
    bool operator>= (const CVector2& rkV) const;

    // arithmetic operations
    inline CVector2 operator+ (const CVector2& rkV) const;
    inline CVector2 operator- (const CVector2& rkV) const;
    inline CVector2 operator* (Real fScalar) const;
    inline CVector2 operator/ (Real fScalar) const;
    inline CVector2 operator- () const;

    // arithmetic updates
    inline CVector2& operator+= (const CVector2& rkV);
    inline CVector2& operator-= (const CVector2& rkV);
    inline CVector2& operator*= (Real fScalar);
    inline CVector2& operator/= (Real fScalar);

    // vector operations
    inline Real Length () const;
    inline Real SquaredLength () const;
    inline Real Dot (const CVector2& rkV) const;
    inline Real Normalize ();

    // returns (y,-x)
    inline CVector2 Perp () const;

    // returns (y,-x)/sqrt(x*x+y*y)
    inline CVector2 UnitPerp () const;

    // returns DotPerp((x,y),(V.x,V.y)) = x*V.y - y*V.x
    inline Real DotPerp (const CVector2& rkV) const;

    // Compute the barycentric coordinates of the point with respect to the
    // triangle <V0,V1,V2>, P = b0*V0 + b1*V1 + b2*V2, where b0 + b1 + b2 = 1.
    void GetBarycentrics (const CVector2& rkV0, const CVector2& rkV1,
        const CVector2& rkV2, Real afBary[3]) const;

    // Gram-Schmidt orthonormalization.  Take linearly independent vectors U
    // and V and compute an orthonormal set (unit length, mutually
    // perpendicular).
    static void Orthonormalize (CVector2& rkU, CVector2& rkV);

    // Input V must be a nonzero vector.  The output is an orthonormal basis
    // {U,V}.  The input V is normalized by this function.  If you know V is
    // already unit length, use U = V.Perp().
    static void GenerateOrthonormalBasis (CVector2& rkU, CVector2& rkV);

    // Compute the extreme values.
    static void ComputeExtremes (int iVQuantity, const CVector2* akPoint,
        CVector2& rkMin, CVector2& rkMax);

    // special vectors
    NOVA_EXPORT static const CVector2 ZERO;    // (0,0)
    NOVA_EXPORT static const CVector2 UNIT_X;  // (1,0)
    NOVA_EXPORT static const CVector2 UNIT_Y;  // (0,1)
    NOVA_EXPORT static const CVector2 ONE;     // (1,1)

private:
    // support for comparisons
    int CompareArrays (const CVector2& rkV) const;

    Real m_afTuple[2];
};

// arithmetic operations
template <class Real>
CVector2<Real> operator* (Real fScalar, const CVector2<Real>& rkV);

// debugging output
template <class Real>
std::ostream& operator<< (std::ostream& rkOStr, const CVector2<Real>& rkV);

template <class Real>
CVector2<Real>::CVector2 ()
{
    // uninitialized for performance in array construction
}
//----------------------------------------------------------------------------
template <class Real>
CVector2<Real>::CVector2 (Real fX, Real fY)
{
    m_afTuple[0] = fX;
    m_afTuple[1] = fY;
}
//----------------------------------------------------------------------------
template <class Real>
CVector2<Real>::CVector2 (const Real* afTuple)
{
    m_afTuple[0] = afTuple[0];
    m_afTuple[1] = afTuple[1];
}
//----------------------------------------------------------------------------
template <class Real>
CVector2<Real>::CVector2 (const CVector2& rkV)
{
    m_afTuple[0] = rkV.m_afTuple[0];
    m_afTuple[1] = rkV.m_afTuple[1];
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector2<Real>::operator const Real* () const
{
    return m_afTuple;
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector2<Real>::operator Real* ()
{
    return m_afTuple;
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CVector2<Real>::operator[] (int i) const
{
    return m_afTuple[i];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real& CVector2<Real>::operator[] (int i)
{
    return m_afTuple[i];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CVector2<Real>::X () const
{
    return m_afTuple[0];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real& CVector2<Real>::X ()
{
    return m_afTuple[0];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CVector2<Real>::Y () const
{
    return m_afTuple[1];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real& CVector2<Real>::Y ()
{
    return m_afTuple[1];
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector2<Real>& CVector2<Real>::operator= (const CVector2& rkV)
{
    m_afTuple[0] = rkV.m_afTuple[0];
    m_afTuple[1] = rkV.m_afTuple[1];
    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
int CVector2<Real>::CompareArrays (const CVector2& rkV) const
{
    return memcmp(m_afTuple,rkV.m_afTuple,2*sizeof(Real));
}
//----------------------------------------------------------------------------
template <class Real>
bool CVector2<Real>::operator== (const CVector2& rkV) const
{
    return CompareArrays(rkV) == 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CVector2<Real>::operator!= (const CVector2& rkV) const
{
    return CompareArrays(rkV) != 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CVector2<Real>::operator< (const CVector2& rkV) const
{
    return CompareArrays(rkV) < 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CVector2<Real>::operator<= (const CVector2& rkV) const
{
    return CompareArrays(rkV) <= 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CVector2<Real>::operator> (const CVector2& rkV) const
{
    return CompareArrays(rkV) > 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CVector2<Real>::operator>= (const CVector2& rkV) const
{
    return CompareArrays(rkV) >= 0;
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector2<Real> CVector2<Real>::operator+ (const CVector2& rkV) const
{
    return CVector2(
        m_afTuple[0]+rkV.m_afTuple[0],
        m_afTuple[1]+rkV.m_afTuple[1]);
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector2<Real> CVector2<Real>::operator- (const CVector2& rkV) const
{
    return CVector2(
        m_afTuple[0]-rkV.m_afTuple[0],
        m_afTuple[1]-rkV.m_afTuple[1]);
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector2<Real> CVector2<Real>::operator* (Real fScalar) const
{
    return CVector2(
        fScalar*m_afTuple[0],
        fScalar*m_afTuple[1]);
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector2<Real> CVector2<Real>::operator/ (Real fScalar) const
{
    CVector2 kQuot;

    if (fScalar != (Real)0.0)
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        kQuot.m_afTuple[0] = fInvScalar*m_afTuple[0];
        kQuot.m_afTuple[1] = fInvScalar*m_afTuple[1];
    }
    else
    {
        kQuot.m_afTuple[0] = CMath<Real>::MAX_REAL;
        kQuot.m_afTuple[1] = CMath<Real>::MAX_REAL;
    }

    return kQuot;
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector2<Real> CVector2<Real>::operator- () const
{
    return CVector2(
        -m_afTuple[0],
        -m_afTuple[1]);
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector2<Real> operator* (Real fScalar, const CVector2<Real>& rkV)
{
    return CVector2<Real>(
        fScalar*rkV[0],
        fScalar*rkV[1]);
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector2<Real>& CVector2<Real>::operator+= (const CVector2& rkV)
{
    m_afTuple[0] += rkV.m_afTuple[0];
    m_afTuple[1] += rkV.m_afTuple[1];
    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector2<Real>& CVector2<Real>::operator-= (const CVector2& rkV)
{
    m_afTuple[0] -= rkV.m_afTuple[0];
    m_afTuple[1] -= rkV.m_afTuple[1];
    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector2<Real>& CVector2<Real>::operator*= (Real fScalar)
{
    m_afTuple[0] *= fScalar;
    m_afTuple[1] *= fScalar;
    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector2<Real>& CVector2<Real>::operator/= (Real fScalar)
{
    if (fScalar != (Real)0.0)
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        m_afTuple[0] *= fInvScalar;
        m_afTuple[1] *= fInvScalar;
    }
    else
    {
        m_afTuple[0] = CMath<Real>::MAX_REAL;
        m_afTuple[1] = CMath<Real>::MAX_REAL;
    }

    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CVector2<Real>::Length () const
{
    return CMath<Real>::Sqrt(
        m_afTuple[0]*m_afTuple[0] +
        m_afTuple[1]*m_afTuple[1]);
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CVector2<Real>::SquaredLength () const
{
    return
        m_afTuple[0]*m_afTuple[0] +
        m_afTuple[1]*m_afTuple[1];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CVector2<Real>::Dot (const CVector2& rkV) const
{
    return
        m_afTuple[0]*rkV.m_afTuple[0] +
        m_afTuple[1]*rkV.m_afTuple[1];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CVector2<Real>::Normalize ()
{
    Real fLength = Length();

    if (fLength > CMath<Real>::ZERO_TOLERANCE)
    {
        Real fInvLength = ((Real)1.0)/fLength;
        m_afTuple[0] *= fInvLength;
        m_afTuple[1] *= fInvLength;
    }
    else
    {
        fLength = (Real)0.0;
        m_afTuple[0] = (Real)0.0;
        m_afTuple[1] = (Real)0.0;
    }

    return fLength;
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector2<Real> CVector2<Real>::Perp () const
{
    return CVector2(m_afTuple[1],-m_afTuple[0]);
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector2<Real> CVector2<Real>::UnitPerp () const
{
    CVector2 kPerp(m_afTuple[1],-m_afTuple[0]);
    kPerp.Normalize();
    return kPerp;
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CVector2<Real>::DotPerp (const CVector2& rkV) const
{
    return m_afTuple[0]*rkV.m_afTuple[1] - m_afTuple[1]*rkV.m_afTuple[0];
}
//----------------------------------------------------------------------------
template <class Real>
void CVector2<Real>::GetBarycentrics (const CVector2& rkV0, const CVector2& rkV1,
    const CVector2& rkV2, Real afBary[3]) const
{
    // Compute the vectors relative to V2 of the triangle.
    CVector2 akDiff[3] =
    {
        rkV0 - rkV2,
        rkV1 - rkV2,
        *this - rkV2
    };

    // If the vertices have large magnitude, the linear system of equations
    // for computing barycentric coordinates can be ill-conditioned.  To avoid
    // this, uniformly scale the triangle edges to be of order 1.  The scaling
    // of all differences does not change the barycentric coordinates.
    Real fMax = (Real)0.0;
    int i;
    for (i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            Real fValue = CMath<Real>::FAbs(akDiff[i][j]);
            if (fValue > fMax)
            {
                fMax = fValue;
            }
        }
    }

    // Scale down only large data.
    Real fInvMax = (Real)0.0;
    if (fMax > (Real)1.0)
    {
        fInvMax = ((Real)1.0)/fMax;
        for (i = 0; i < 3; i++)
        {
            akDiff[i] *= fInvMax;
        }
    }

    Real fDet = akDiff[0].DotPerp(akDiff[1]);
    if (CMath<Real>::FAbs(fDet) > CMath<Real>::ZERO_TOLERANCE)
    {
        Real fInvDet = ((Real)1.0)/fDet;
        afBary[0] = akDiff[2].DotPerp(akDiff[1])*fInvDet;
        afBary[1] = akDiff[0].DotPerp(akDiff[2])*fInvDet;
        afBary[2] = (Real)1.0 - afBary[0] - afBary[1];
    }
    else
    {
        // The triangle is a sliver.  Determine the longest edge and
        // compute barycentric coordinates with respect to that edge.
        CVector2 kE2 = rkV0 - rkV1;
        if (fInvMax != (Real)0.0)
        {
            kE2 *= fInvMax;
        }

        Real fMaxSqrLength = kE2.SquaredLength();
        int iMaxIndex = 2;
        Real fSqrLength = akDiff[1].SquaredLength();
        if (fSqrLength > fMaxSqrLength)
        {
            iMaxIndex = 1;
            fMaxSqrLength = fSqrLength;
        }
        fSqrLength = akDiff[0].SquaredLength();
        if (fSqrLength > fMaxSqrLength)
        {
            iMaxIndex = 0;
            fMaxSqrLength = fSqrLength;
        }

        if (fMaxSqrLength > CMath<Real>::ZERO_TOLERANCE)
        {
            Real fInvSqrLength = ((Real)1.0)/fMaxSqrLength;
            if (iMaxIndex == 0)
            {
                // P-V2 = t(V0-V2)
                afBary[0] = akDiff[2].Dot(akDiff[0])*fInvSqrLength;
                afBary[1] = (Real)0.0;
                afBary[2] = (Real)1.0 - afBary[0];
            }
            else if (iMaxIndex == 1)
            {
                // P-V2 = t(V1-V2)
                afBary[0] = (Real)0.0;
                afBary[1] = akDiff[2].Dot(akDiff[1])*fInvSqrLength;
                afBary[2] = (Real)1.0 - afBary[1];
            }
            else
            {
                // P-V1 = t(V0-V1)
                akDiff[2] = *this - rkV1;
                if (fInvMax != (Real)0.0)
                {
                    akDiff[2] *= fInvMax;
                }

                afBary[0] = akDiff[2].Dot(kE2)*fInvSqrLength;
                afBary[1] = (Real)1.0 - afBary[0];
                afBary[2] = (Real)0.0;
            }
        }
        else
        {
            // The triangle is a nearly a point, just return equal weights.
            afBary[0] = ((Real)1.0)/(Real)3.0;
            afBary[1] = afBary[0];
            afBary[2] = afBary[0];
        }
    }
}
//----------------------------------------------------------------------------
template <class Real>
void CVector2<Real>::Orthonormalize (CVector2& rkU, CVector2& rkV)
{
    // If the input vectors are v0 and v1, then the Gram-Schmidt
    // orthonormalization produces vectors u0 and u1 as follows,
    //
    //   u0 = v0/|v0|
    //   u1 = (v1-(u0*v1)u0)/|v1-(u0*v1)u0|
    //
    // where |A| indicates length of vector A and A*B indicates dot
    // product of vectors A and B.

    // compute u0
    rkU.Normalize();

    // compute u1
    Real fDot0 = rkU.Dot(rkV); 
    rkV -= rkU*fDot0;
    rkV.Normalize();
}
//----------------------------------------------------------------------------
template <class Real>
void CVector2<Real>::GenerateOrthonormalBasis (CVector2& rkU, CVector2& rkV)
{
    rkV.Normalize();
    rkU = rkV.Perp();
}
//----------------------------------------------------------------------------
template <class Real>
void CVector2<Real>::ComputeExtremes (int iVQuantity, const CVector2* akPoint,
    CVector2& rkMin, CVector2& rkMax)
{
    assert(iVQuantity > 0 && akPoint);

    rkMin = akPoint[0];
    rkMax = rkMin;
    for (int i = 1; i < iVQuantity; i++)
    {
        const CVector2<Real>& rkPoint = akPoint[i];
        for (int j = 0; j < 2; j++)
        {
            if (rkPoint[j] < rkMin[j])
            {
                rkMin[j] = rkPoint[j];
            }
            else if (rkPoint[j] > rkMax[j])
            {
                rkMax[j] = rkPoint[j];
            }
        }
    }
}
//----------------------------------------------------------------------------
template <class Real>
std::ostream& operator<< (std::ostream& rkOStr, const CVector2<Real>& rkV)
{
     return rkOStr << rkV.X() << ' ' << rkV.Y();
}

typedef CVector2<nova::real> Vector2f;

}