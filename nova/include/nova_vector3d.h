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
class CVector3 : public CObjectConstructor
{
public:
    // construction
    CVector3 ();  // uninitialized
    CVector3 (Real fX, Real fY, Real fZ);
    CVector3 (const Real* afTuple);
    CVector3 (const CVector3& rkV);

    // coordinate access
    inline operator const Real* () const;
    inline operator Real* ();
    inline Real operator[] (int i) const;
    inline Real& operator[] (int i);
    inline Real X () const;
    inline Real& X ();
    inline Real Y () const;
    inline Real& Y ();
    inline Real Z () const;
    inline Real& Z ();

    // assignment
    inline CVector3& operator= (const CVector3& rkV);

    // comparison
    bool operator== (const CVector3& rkV) const;
    bool operator!= (const CVector3& rkV) const;
    bool operator<  (const CVector3& rkV) const;
    bool operator<= (const CVector3& rkV) const;
    bool operator>  (const CVector3& rkV) const;
    bool operator>= (const CVector3& rkV) const;

    // arithmetic operations
    inline CVector3 operator+ (const CVector3& rkV) const;
    inline CVector3 operator- (const CVector3& rkV) const;
    inline CVector3 operator* (Real fScalar) const;
    inline CVector3 operator/ (Real fScalar) const;
    inline CVector3 operator- () const;

    // arithmetic updates
    inline CVector3& operator+= (const CVector3& rkV);
    inline CVector3& operator-= (const CVector3& rkV);
    inline CVector3& operator*= (Real fScalar);
    inline CVector3& operator/= (Real fScalar);

    // vector operations
    inline Real Length () const;
    inline Real SquaredLength () const;
    inline Real Dot (const CVector3& rkV) const;
    inline Real Normalize ();

    // The cross products are computed using the right-handed rule.  Be aware
    // that some graphics APIs use a left-handed rule.  If you have to compute
    // a cross product with these functions and send the result to the API
    // that expects left-handed, you will need to change sign on the vector
    // (replace each component value c by -c).
    inline CVector3 Cross (const CVector3& rkV) const;
    inline CVector3 UnitCross (const CVector3& rkV) const;

    // Compute the barycentric coordinates of the point with respect to the
    // tetrahedron <V0,V1,V2,V3>, P = b0*V0 + b1*V1 + b2*V2 + b3*V3, where
    // b0 + b1 + b2 + b3 = 1.
    void GetBarycentrics (const CVector3& rkV0, const CVector3& rkV1,
        const CVector3& rkV2, const CVector3& rkV3, Real afBary[4]) const;

    // Gram-Schmidt orthonormalization.  Take linearly independent vectors
    // U, V, and W and compute an orthonormal set (unit length, mutually
    // perpendicular).
    static void Orthonormalize (CVector3& rkU, CVector3& rkV, CVector3& rkW);
    static void Orthonormalize (CVector3* akV);

    // Input W must be a nonzero vector. The output is an orthonormal basis
    // {U,V,W}.  The input W is normalized by this function.  If you know
    // W is already unit length, use GenerateComplementBasis to compute U
    // and V.
    static void GenerateOrthonormalBasis (CVector3& rkU, CVector3& rkV,
        CVector3& rkW);

    // Input W must be a unit-length vector.  The output vectors {U,V} are
    // unit length and mutually perpendicular, and {U,V,W} is an orthonormal
    // basis.
    static void GenerateComplementBasis (CVector3& rkU, CVector3& rkV,
        const CVector3& rkW);

    // Compute the extreme values.
    static void ComputeExtremes (int iVQuantity, const CVector3* akPoint,
        CVector3& rkMin, CVector3& rkMax);

    // special vectors
    NOVA_EXPORT static const CVector3 ZERO;    // (0,0,0)
    NOVA_EXPORT static const CVector3 UNIT_X;  // (1,0,0)
    NOVA_EXPORT static const CVector3 UNIT_Y;  // (0,1,0)
    NOVA_EXPORT static const CVector3 UNIT_Z;  // (0,0,1)
    NOVA_EXPORT static const CVector3 ONE;     // (1,1,1)

private:
    // support for comparisons
    int CompareArrays (const CVector3& rkV) const;

    Real m_afTuple[3];
};

// arithmetic operations
template <class Real>
CVector3<Real> operator* (Real fScalar, const CVector3<Real>& rkV);

// debugging output
template <class Real>
std::ostream& operator<< (std::ostream& rkOStr, const CVector3<Real>& rkV);

template <class Real>
CVector3<Real>::CVector3 ()
{
    // uninitialized for performance in array construction
}
//----------------------------------------------------------------------------
template <class Real>
CVector3<Real>::CVector3 (Real fX, Real fY, Real fZ)
{
    m_afTuple[0] = fX;
    m_afTuple[1] = fY;
    m_afTuple[2] = fZ;
}
//----------------------------------------------------------------------------
template <class Real>
CVector3<Real>::CVector3 (const Real* afTuple)
{
    m_afTuple[0] = afTuple[0];
    m_afTuple[1] = afTuple[1];
    m_afTuple[2] = afTuple[2];
}
//----------------------------------------------------------------------------
template <class Real>
CVector3<Real>::CVector3 (const CVector3& rkV)
{
    m_afTuple[0] = rkV.m_afTuple[0];
    m_afTuple[1] = rkV.m_afTuple[1];
    m_afTuple[2] = rkV.m_afTuple[2];
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector3<Real>::operator const Real* () const
{
    return m_afTuple;
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector3<Real>::operator Real* ()
{
    return m_afTuple;
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CVector3<Real>::operator[] (int i) const
{
    return m_afTuple[i];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real& CVector3<Real>::operator[] (int i)
{
    return m_afTuple[i];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CVector3<Real>::X () const
{
    return m_afTuple[0];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real& CVector3<Real>::X ()
{
    return m_afTuple[0];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CVector3<Real>::Y () const
{
    return m_afTuple[1];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real& CVector3<Real>::Y ()
{
    return m_afTuple[1];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CVector3<Real>::Z () const
{
    return m_afTuple[2];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real& CVector3<Real>::Z ()
{
    return m_afTuple[2];
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector3<Real>& CVector3<Real>::operator= (const CVector3& rkV)
{
    m_afTuple[0] = rkV.m_afTuple[0];
    m_afTuple[1] = rkV.m_afTuple[1];
    m_afTuple[2] = rkV.m_afTuple[2];
    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
int CVector3<Real>::CompareArrays (const CVector3& rkV) const
{
    return memcmp(m_afTuple,rkV.m_afTuple,3*sizeof(Real));
}
//----------------------------------------------------------------------------
template <class Real>
bool CVector3<Real>::operator== (const CVector3& rkV) const
{
    return CompareArrays(rkV) == 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CVector3<Real>::operator!= (const CVector3& rkV) const
{
    return CompareArrays(rkV) != 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CVector3<Real>::operator< (const CVector3& rkV) const
{
    return CompareArrays(rkV) < 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CVector3<Real>::operator<= (const CVector3& rkV) const
{
    return CompareArrays(rkV) <= 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CVector3<Real>::operator> (const CVector3& rkV) const
{
    return CompareArrays(rkV) > 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CVector3<Real>::operator>= (const CVector3& rkV) const
{
    return CompareArrays(rkV) >= 0;
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector3<Real> CVector3<Real>::operator+ (const CVector3& rkV) const
{
    return CVector3(
        m_afTuple[0]+rkV.m_afTuple[0],
        m_afTuple[1]+rkV.m_afTuple[1],
        m_afTuple[2]+rkV.m_afTuple[2]);
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector3<Real> CVector3<Real>::operator- (const CVector3& rkV) const
{
    return CVector3(
        m_afTuple[0]-rkV.m_afTuple[0],
        m_afTuple[1]-rkV.m_afTuple[1],
        m_afTuple[2]-rkV.m_afTuple[2]);
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector3<Real> CVector3<Real>::operator* (Real fScalar) const
{
    return CVector3(
        fScalar*m_afTuple[0],
        fScalar*m_afTuple[1],
        fScalar*m_afTuple[2]);
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector3<Real> CVector3<Real>::operator/ (Real fScalar) const
{
    CVector3 kQuot;

    if (fScalar != (Real)0.0)
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        kQuot.m_afTuple[0] = fInvScalar*m_afTuple[0];
        kQuot.m_afTuple[1] = fInvScalar*m_afTuple[1];
        kQuot.m_afTuple[2] = fInvScalar*m_afTuple[2];
    }
    else
    {
        kQuot.m_afTuple[0] = CMath<Real>::MAX_REAL;
        kQuot.m_afTuple[1] = CMath<Real>::MAX_REAL;
        kQuot.m_afTuple[2] = CMath<Real>::MAX_REAL;
    }

    return kQuot;
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector3<Real> CVector3<Real>::operator- () const
{
    return CVector3(
        -m_afTuple[0],
        -m_afTuple[1],
        -m_afTuple[2]);
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector3<Real> operator* (Real fScalar, const CVector3<Real>& rkV)
{
    return CVector3<Real>(
        fScalar*rkV[0],
        fScalar*rkV[1],
        fScalar*rkV[2]);
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector3<Real>& CVector3<Real>::operator+= (const CVector3& rkV)
{
    m_afTuple[0] += rkV.m_afTuple[0];
    m_afTuple[1] += rkV.m_afTuple[1];
    m_afTuple[2] += rkV.m_afTuple[2];
    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector3<Real>& CVector3<Real>::operator-= (const CVector3& rkV)
{
    m_afTuple[0] -= rkV.m_afTuple[0];
    m_afTuple[1] -= rkV.m_afTuple[1];
    m_afTuple[2] -= rkV.m_afTuple[2];
    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector3<Real>& CVector3<Real>::operator*= (Real fScalar)
{
    m_afTuple[0] *= fScalar;
    m_afTuple[1] *= fScalar;
    m_afTuple[2] *= fScalar;
    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector3<Real>& CVector3<Real>::operator/= (Real fScalar)
{
    if (fScalar != (Real)0.0)
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        m_afTuple[0] *= fInvScalar;
        m_afTuple[1] *= fInvScalar;
        m_afTuple[2] *= fInvScalar;
    }
    else
    {
        m_afTuple[0] = CMath<Real>::MAX_REAL;
        m_afTuple[1] = CMath<Real>::MAX_REAL;
        m_afTuple[2] = CMath<Real>::MAX_REAL;
    }

    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CVector3<Real>::Length () const
{
    return CMath<Real>::Sqrt(
        m_afTuple[0]*m_afTuple[0] +
        m_afTuple[1]*m_afTuple[1] +
        m_afTuple[2]*m_afTuple[2]);
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CVector3<Real>::SquaredLength () const
{
    return
        m_afTuple[0]*m_afTuple[0] +
        m_afTuple[1]*m_afTuple[1] +
        m_afTuple[2]*m_afTuple[2];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CVector3<Real>::Dot (const CVector3& rkV) const
{
    return
        m_afTuple[0]*rkV.m_afTuple[0] +
        m_afTuple[1]*rkV.m_afTuple[1] +
        m_afTuple[2]*rkV.m_afTuple[2];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CVector3<Real>::Normalize ()
{
    Real fLength = Length();

    if (fLength > CMath<Real>::ZERO_TOLERANCE)
    {
        Real fInvLength = ((Real)1.0)/fLength;
        m_afTuple[0] *= fInvLength;
        m_afTuple[1] *= fInvLength;
        m_afTuple[2] *= fInvLength;
    }
    else
    {
        fLength = (Real)0.0;
        m_afTuple[0] = (Real)0.0;
        m_afTuple[1] = (Real)0.0;
        m_afTuple[2] = (Real)0.0;
    }

    return fLength;
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector3<Real> CVector3<Real>::Cross (const CVector3& rkV) const
{
    return CVector3(
        m_afTuple[1]*rkV.m_afTuple[2] - m_afTuple[2]*rkV.m_afTuple[1],
        m_afTuple[2]*rkV.m_afTuple[0] - m_afTuple[0]*rkV.m_afTuple[2],
        m_afTuple[0]*rkV.m_afTuple[1] - m_afTuple[1]*rkV.m_afTuple[0]);
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector3<Real> CVector3<Real>::UnitCross (const CVector3& rkV) const
{
    CVector3 kCross(
        m_afTuple[1]*rkV.m_afTuple[2] - m_afTuple[2]*rkV.m_afTuple[1],
        m_afTuple[2]*rkV.m_afTuple[0] - m_afTuple[0]*rkV.m_afTuple[2],
        m_afTuple[0]*rkV.m_afTuple[1] - m_afTuple[1]*rkV.m_afTuple[0]);
    kCross.Normalize();
    return kCross;
}
//----------------------------------------------------------------------------
template <class Real>
void CVector3<Real>::GetBarycentrics (const CVector3<Real>& rkV0,
    const CVector3<Real>& rkV1, const CVector3<Real>& rkV2,
    const CVector3<Real>& rkV3, Real afBary[4]) const
{
    // Compute the vectors relative to V3 of the tetrahedron.
    CVector3<Real> akDiff[4] =
    {
        rkV0 - rkV3,
        rkV1 - rkV3,
        rkV2 - rkV3,
        *this - rkV3
    };

    // If the vertices have large magnitude, the linear system of
    // equations for computing barycentric coordinates can be
    // ill-conditioned.  To avoid this, uniformly scale the tetrahedron
    // edges to be of order 1.  The scaling of all differences does not
    // change the barycentric coordinates.
    Real fMax = (Real)0.0;
    int i;
    for (i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
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
        for (i = 0; i < 4; i++)
        {
            akDiff[i] *= fInvMax;
        }
    }

    Real fDet = akDiff[0].Dot(akDiff[1].Cross(akDiff[2]));
    CVector3<Real> kE1cE2 = akDiff[1].Cross(akDiff[2]);
    CVector3<Real> kE2cE0 = akDiff[2].Cross(akDiff[0]);
    CVector3<Real> kE0cE1 = akDiff[0].Cross(akDiff[1]);
    if (CMath<Real>::FAbs(fDet) > CMath<Real>::ZERO_TOLERANCE)
    {
        Real fInvDet = ((Real)1.0)/fDet;
        afBary[0] = akDiff[3].Dot(kE1cE2)*fInvDet;
        afBary[1] = akDiff[3].Dot(kE2cE0)*fInvDet;
        afBary[2] = akDiff[3].Dot(kE0cE1)*fInvDet;
        afBary[3] = (Real)1.0 - afBary[0] - afBary[1] - afBary[2];
    }
    else
    {
        // The tetrahedron is potentially flat.  Determine the face of
        // maximum area and compute barycentric coordinates with respect
        // to that face.
        CVector3<Real> kE02 = rkV0 - rkV2;
        CVector3<Real> kE12 = rkV1 - rkV2;
        if (fInvMax != (Real)0.0)
        {
            kE02 *= fInvMax;
            kE12 *= fInvMax;
        }

        CVector3<Real> kE02cE12 = kE02.Cross(kE12);
        Real fMaxSqrArea = kE02cE12.SquaredLength();
        int iMaxIndex = 3;
        Real fSqrArea = kE0cE1.SquaredLength();
        if (fSqrArea > fMaxSqrArea)
        {
            iMaxIndex = 0;
            fMaxSqrArea = fSqrArea;
        }
        fSqrArea = kE1cE2.SquaredLength();
        if (fSqrArea > fMaxSqrArea)
        {
            iMaxIndex = 1;
            fMaxSqrArea = fSqrArea;
        }
        fSqrArea = kE2cE0.SquaredLength();
        if (fSqrArea > fMaxSqrArea)
        {
            iMaxIndex = 2;
            fMaxSqrArea = fSqrArea;
        }

        if (fMaxSqrArea > CMath<Real>::ZERO_TOLERANCE)
        {
            Real fInvSqrArea = ((Real)1.0)/fMaxSqrArea;
            CVector3<Real> kTmp;
            if (iMaxIndex == 0)
            {
                kTmp = akDiff[3].Cross(akDiff[1]);
                afBary[0] = kE0cE1.Dot(kTmp)*fInvSqrArea;
                kTmp = akDiff[0].Cross(akDiff[3]);
                afBary[1] = kE0cE1.Dot(kTmp)*fInvSqrArea;
                afBary[2] = (Real)0.0;
                afBary[3] = (Real)1.0 - afBary[0] - afBary[1];
            }
            else if (iMaxIndex == 1)
            {
                afBary[0] = (Real)0.0;
                kTmp = akDiff[3].Cross(akDiff[2]);
                afBary[1] = kE1cE2.Dot(kTmp)*fInvSqrArea;
                kTmp = akDiff[1].Cross(akDiff[3]);
                afBary[2] = kE1cE2.Dot(kTmp)*fInvSqrArea;
                afBary[3] = (Real)1.0 - afBary[1] - afBary[2];
            }
            else if (iMaxIndex == 2)
            {
                kTmp = akDiff[2].Cross(akDiff[3]);
                afBary[0] = kE2cE0.Dot(kTmp)*fInvSqrArea;
                afBary[1] = (Real)0.0;
                kTmp = akDiff[3].Cross(akDiff[0]);
                afBary[2] = kE2cE0.Dot(kTmp)*fInvSqrArea;
                afBary[3] = (Real)1.0 - afBary[0] - afBary[2];
            }
            else
            {
                akDiff[3] = *this - rkV2;
                if (fInvMax != (Real)0.0)
                {
                    akDiff[3] *= fInvMax;
                }

                kTmp = akDiff[3].Cross(kE12);
                afBary[0] = kE02cE12.Dot(kTmp)*fInvSqrArea;
                kTmp = kE02.Cross(akDiff[3]);
                afBary[1] = kE02cE12.Dot(kTmp)*fInvSqrArea;
                afBary[2] = (Real)1.0 - afBary[0] - afBary[1];
                afBary[3] = (Real)0.0;
            }
        }
        else
        {
            // The tetrahedron is potentially a sliver.  Determine the edge of
            // maximum length and compute barycentric coordinates with respect
            // to that edge.
            Real fMaxSqrLength = akDiff[0].SquaredLength();
            iMaxIndex = 0;  // <V0,V3>
            Real fSqrLength = akDiff[1].SquaredLength();
            if (fSqrLength > fMaxSqrLength)
            {
                iMaxIndex = 1;  // <V1,V3>
                fMaxSqrLength = fSqrLength;
            }
            fSqrLength = akDiff[2].SquaredLength();
            if (fSqrLength > fMaxSqrLength)
            {
                iMaxIndex = 2;  // <V2,V3>
                fMaxSqrLength = fSqrLength;
            }
            fSqrLength = kE02.SquaredLength();
            if (fSqrLength > fMaxSqrLength)
            {
                iMaxIndex = 3;  // <V0,V2>
                fMaxSqrLength = fSqrLength;
            }
            fSqrLength = kE12.SquaredLength();
            if (fSqrLength > fMaxSqrLength)
            {
                iMaxIndex = 4;  // <V1,V2>
                fMaxSqrLength = fSqrLength;
            }
            CVector3<Real> kE01 = rkV0 - rkV1;
            fSqrLength = kE01.SquaredLength();
            if (fSqrLength > fMaxSqrLength)
            {
                iMaxIndex = 5;  // <V0,V1>
                fMaxSqrLength = fSqrLength;
            }

            if (fMaxSqrLength > CMath<Real>::ZERO_TOLERANCE)
            {
                Real fInvSqrLength = ((Real)1.0)/fMaxSqrLength;
                if (iMaxIndex == 0)
                {
                    // P-V3 = t*(V0-V3)
                    afBary[0] = akDiff[3].Dot(akDiff[0])*fInvSqrLength;
                    afBary[1] = (Real)0.0;
                    afBary[2] = (Real)0.0;
                    afBary[3] = (Real)1.0 - afBary[0];
                }
                else if (iMaxIndex == 1)
                {
                    // P-V3 = t*(V1-V3)
                    afBary[0] = (Real)0.0;
                    afBary[1] = akDiff[3].Dot(akDiff[1])*fInvSqrLength;
                    afBary[2] = (Real)0.0;
                    afBary[3] = (Real)1.0 - afBary[1];
                }
                else if (iMaxIndex == 2)
                {
                    // P-V3 = t*(V2-V3)
                    afBary[0] = (Real)0.0;
                    afBary[1] = (Real)0.0;
                    afBary[2] = akDiff[3].Dot(akDiff[2])*fInvSqrLength;
                    afBary[3] = (Real)1.0 - afBary[2];
                }
                else if (iMaxIndex == 3)
                {
                    // P-V2 = t*(V0-V2)
                    akDiff[3] = *this - rkV2;
                    if (fInvMax != (Real)0.0)
                    {
                        akDiff[3] *= fInvMax;
                    }

                    afBary[0] = akDiff[3].Dot(kE02)*fInvSqrLength;
                    afBary[1] = (Real)0.0;
                    afBary[2] = (Real)1.0 - afBary[0];
                    afBary[3] = (Real)0.0;
                }
                else if (iMaxIndex == 4)
                {
                    // P-V2 = t*(V1-V2)
                    akDiff[3] = *this - rkV2;
                    if (fInvMax != (Real)0.0)
                    {
                        akDiff[3] *= fInvMax;
                    }

                    afBary[0] = (Real)0.0;
                    afBary[1] = akDiff[3].Dot(kE12)*fInvSqrLength;
                    afBary[2] = (Real)1.0 - afBary[1];
                    afBary[3] = (Real)0.0;
                }
                else
                {
                    // P-V1 = t*(V0-V1)
                    akDiff[3] = *this - rkV1;
                    if (fInvMax != (Real)0.0)
                    {
                        akDiff[3] *= fInvMax;
                    }

                    afBary[0] = akDiff[3].Dot(kE01)*fInvSqrLength;
                    afBary[1] = (Real)1.0 - afBary[0];
                    afBary[2] = (Real)0.0;
                    afBary[3] = (Real)0.0;
                }
            }
            else
            {
                // The tetrahedron is a nearly a point, just return equal
                // weights.
                afBary[0] = (Real)0.25;
                afBary[1] = afBary[0];
                afBary[2] = afBary[0];
                afBary[3] = afBary[0];
            }
        }
    }
}
//----------------------------------------------------------------------------
template <class Real>
void CVector3<Real>::Orthonormalize (CVector3& rkU, CVector3& rkV, CVector3& rkW)
{
    // If the input vectors are v0, v1, and v2, then the Gram-Schmidt
    // orthonormalization produces vectors u0, u1, and u2 as follows,
    //
    //   u0 = v0/|v0|
    //   u1 = (v1-(u0*v1)u0)/|v1-(u0*v1)u0|
    //   u2 = (v2-(u0*v2)u0-(u1*v2)u1)/|v2-(u0*v2)u0-(u1*v2)u1|
    //
    // where |A| indicates length of vector A and A*B indicates dot
    // product of vectors A and B.

    // compute u0
    rkU.Normalize();

    // compute u1
    Real fDot0 = rkU.Dot(rkV); 
    rkV -= fDot0*rkU;
    rkV.Normalize();

    // compute u2
    Real fDot1 = rkV.Dot(rkW);
    fDot0 = rkU.Dot(rkW);
    rkW -= fDot0*rkU + fDot1*rkV;
    rkW.Normalize();
}
//----------------------------------------------------------------------------
template <class Real>
void CVector3<Real>::Orthonormalize (CVector3* akV)
{
    Orthonormalize(akV[0],akV[1],akV[2]);
}
//----------------------------------------------------------------------------
template <class Real>
void CVector3<Real>::GenerateOrthonormalBasis (CVector3& rkU, CVector3& rkV,
    CVector3& rkW)
{
    rkW.Normalize();
    GenerateComplementBasis(rkU,rkV,rkW);
}
//----------------------------------------------------------------------------
template <class Real>
void CVector3<Real>::GenerateComplementBasis (CVector3& rkU, CVector3& rkV,
    const CVector3& rkW)
{
    Real fInvLength;

    if (CMath<Real>::FAbs(rkW.m_afTuple[0]) >=
        CMath<Real>::FAbs(rkW.m_afTuple[1]) )
    {
        // W.x or W.z is the largest magnitude component, swap them
        fInvLength = CMath<Real>::InvSqrt(rkW.m_afTuple[0]*rkW.m_afTuple[0] +
            rkW.m_afTuple[2]*rkW.m_afTuple[2]);
        rkU.m_afTuple[0] = -rkW.m_afTuple[2]*fInvLength;
        rkU.m_afTuple[1] = (Real)0.0;
        rkU.m_afTuple[2] = +rkW.m_afTuple[0]*fInvLength;
        rkV.m_afTuple[0] = rkW.m_afTuple[1]*rkU.m_afTuple[2];
        rkV.m_afTuple[1] = rkW.m_afTuple[2]*rkU.m_afTuple[0] -
            rkW.m_afTuple[0]*rkU.m_afTuple[2];
        rkV.m_afTuple[2] = -rkW.m_afTuple[1]*rkU.m_afTuple[0];
    }
    else
    {
        // W.y or W.z is the largest magnitude component, swap them
        fInvLength = CMath<Real>::InvSqrt(rkW.m_afTuple[1]*rkW.m_afTuple[1] +
            rkW.m_afTuple[2]*rkW.m_afTuple[2]);
        rkU.m_afTuple[0] = (Real)0.0;
        rkU.m_afTuple[1] = +rkW.m_afTuple[2]*fInvLength;
        rkU.m_afTuple[2] = -rkW.m_afTuple[1]*fInvLength;
        rkV.m_afTuple[0] = rkW.m_afTuple[1]*rkU.m_afTuple[2] -
            rkW.m_afTuple[2]*rkU.m_afTuple[1];
        rkV.m_afTuple[1] = -rkW.m_afTuple[0]*rkU.m_afTuple[2];
        rkV.m_afTuple[2] = rkW.m_afTuple[0]*rkU.m_afTuple[1];
    }
}
//----------------------------------------------------------------------------
template <class Real>
void CVector3<Real>::ComputeExtremes (int iVQuantity, const CVector3* akPoint,
    CVector3& rkMin, CVector3& rkMax)
{
    assert(iVQuantity > 0 && akPoint);

    rkMin = akPoint[0];
    rkMax = rkMin;
    for (int i = 1; i < iVQuantity; i++)
    {
        const CVector3<Real>& rkPoint = akPoint[i];
        for (int j = 0; j < 3; j++)
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
std::ostream& operator<< (std::ostream& rkOStr, const CVector3<Real>& rkV)
{
     return rkOStr << rkV.X() << ' ' << rkV.Y() << ' ' << rkV.Z();
}


typedef CVector3<real> Vector3f;

}