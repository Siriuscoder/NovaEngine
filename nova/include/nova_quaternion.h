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

#include "nova_matrix3d.h"

namespace nova
{

template <class nReal>
class CQuaternion: public CMemoryManaged
{
public:
    // A CQuaternion is q = w + x*i + y*j + z*k where (w,x,y,z) is not
    // necessarily a unit length vector in 4D.

    // construction
    CQuaternion ();  // uninitialized
    CQuaternion (nReal fW, nReal fX, nReal fY, nReal fZ);
    CQuaternion (const CQuaternion& rkQ);

    // CQuaternion for the input rotation matrix
    CQuaternion (const CMatrix3<nReal>& rkRot);

    // CQuaternion for the rotation of the axis-angle pair
    CQuaternion (const CVector3<nReal>& rkAxis, nReal fAngle);

    // CQuaternion for the rotation matrix with specified columns
    CQuaternion (const CVector3<nReal> akRotColumn[3]);

    // member access:  0 = w, 1 = x, 2 = y, 3 = z
    inline operator const nReal* () const;
    inline operator nReal* ();
    inline nReal operator[] (nInt32 i) const;
    inline nReal& operator[] (nInt32 i);
    inline nReal W () const;
    inline nReal& W ();
    inline nReal X () const;
    inline nReal& X ();
    inline nReal Y () const;
    inline nReal& Y ();
    inline nReal Z () const;
    inline nReal& Z ();

    // assignment
    inline CQuaternion& operator= (const CQuaternion& rkQ);

    // comparison
    bool operator== (const CQuaternion& rkQ) const;
    bool operator!= (const CQuaternion& rkQ) const;
    bool operator<  (const CQuaternion& rkQ) const;
    bool operator<= (const CQuaternion& rkQ) const;
    bool operator>  (const CQuaternion& rkQ) const;
    bool operator>= (const CQuaternion& rkQ) const;

    // arithmetic operations
    inline CQuaternion operator+ (const CQuaternion& rkQ) const;
    inline CQuaternion operator- (const CQuaternion& rkQ) const;
    inline CQuaternion operator* (const CQuaternion& rkQ) const;
    inline CQuaternion operator* (nReal fScalar) const;
    inline CQuaternion operator/ (nReal fScalar) const;
    inline CQuaternion operator- () const;

    // arithmetic updates
    inline CQuaternion& operator+= (const CQuaternion& rkQ);
    inline CQuaternion& operator-= (const CQuaternion& rkQ);
    inline CQuaternion& operator*= (nReal fScalar);
    inline CQuaternion& operator/= (nReal fScalar);

    // conversion between quaternions, matrices, and axis-angle
    CQuaternion& FromRotationMatrix (const CMatrix3<nReal>& rkRot);
    void ToRotationMatrix (CMatrix3<nReal>& rkRot) const;
    CQuaternion& FromRotationMatrix (const CVector3<nReal> akRotColumn[3]);
    void ToRotationMatrix (CVector3<nReal> akRotColumn[3]) const;
    CQuaternion& FromAxisAngle (const CVector3<nReal>& rkAxis, nReal fAngle);
    void ToAxisAngle (CVector3<nReal>& rkAxis, nReal& rfAngle) const;

    // functions of a CQuaternion
    inline nReal Length () const;  // length of 4-tuple
    inline nReal SquaredLength () const;  // squared length of 4-tuple
    inline nReal Dot (const CQuaternion& rkQ) const;  // dot product of 4-tuples
    inline nReal Normalize ();  // make the 4-tuple unit length
    CQuaternion Inverse () const;  // apply to non-zero CQuaternion
    CQuaternion Conjugate () const;
    CQuaternion Exp () const;  // apply to CQuaternion with w = 0
    CQuaternion Log () const;  // apply to unit-length CQuaternion

    // rotation of a vector by a CQuaternion
    CVector3<nReal> Rotate (const CVector3<nReal>& rkVector) const;

    // spherical linear interpolation
    CQuaternion& Slerp (nReal fT, const CQuaternion& rkP, const CQuaternion& rkQ);

    CQuaternion& SlerpExtraSpins (nReal fT, const CQuaternion& rkP,
        const CQuaternion& rkQ, nInt32 iExtraSpins);

    // intermediate terms for spherical quadratic interpolation
    CQuaternion& Intermediate (const CQuaternion& rkQ0,
        const CQuaternion& rkQ1, const CQuaternion& rkQ2);

    // spherical quadratic interpolation
    CQuaternion& Squad (nReal fT, const CQuaternion& rkQ0,
        const CQuaternion& rkA0, const CQuaternion& rkA1,
        const CQuaternion& rkQ1);

    // Compute a CQuaternion that rotates unit-length vector V1 to unit-length
    // vector V2.  The rotation is about the axis perpendicular to both V1 and
    // V2, with angle of that between V1 and V2.  If V1 and V2 are parallel,
    // any axis of rotation will do, such as the permutation (z2,x2,y2), where
    // V2 = (x2,y2,z2).
    CQuaternion& Align (const CVector3<nReal>& rkV1, const CVector3<nReal>& rkV2);

    // Decompose a CQuaternion into q = q_twist * q_swing, where q is 'this'
    // CQuaternion.  If V1 is the input axis and V2 is the rotation of V1 by
    // q, q_swing represents the rotation about the axis perpendicular to
    // V1 and V2 (see CQuaternion::Align), and q_twist is a rotation about V1.
    void DecomposeTwistTimesSwing (const CVector3<nReal>& rkV1,
        CQuaternion& rkTwist, CQuaternion& rkSwing);

    // Decompose a CQuaternion into q = q_swing * q_twist, where q is 'this'
    // CQuaternion.  If V1 is the input axis and V2 is the rotation of V1 by
    // q, q_swing represents the rotation about the axis perpendicular to
    // V1 and V2 (see CQuaternion::Align), and q_twist is a rotation about V1.
    void DecomposeSwingTimesTwist (const CVector3<nReal>& rkV1,
        CQuaternion& rkSwing, CQuaternion& rkTwist);

    // *** Find closest quaternions with unconstrained angles.

    // Closest CQuaternion of the form (cx + sx*i).
    CQuaternion GetClosestX () const;

    // Closest CQuaternion of the form (cy + sy*j).
    CQuaternion GetClosestY () const;

    // Closest CQuaternion of the form (cz + sz*k).
    CQuaternion GetClosestZ () const;

    // Closest CQuaternion of the form (cx + sx*i)*(cy + sy*j).
    CQuaternion GetClosestXY () const;

    // Closest CQuaternion of the form (cy + sy*j)*(cx + sx*i).
    CQuaternion GetClosestYX () const;

    // Closest CQuaternion of the form (cz + sz*k)*(cx + sx*i).
    CQuaternion GetClosestZX () const;

    // Closest CQuaternion of the form (cx + sx*i)*(cz + sz*k).
    CQuaternion GetClosestXZ () const;

    // Closest CQuaternion of the form (cy + sy*j)*(cz + sz*k).
    CQuaternion GetClosestYZ () const;

    // Closest CQuaternion of the form (cz + sz*k)*(cy + sy*j).
    CQuaternion GetClosestZY () const;

    // Factor to (cx + sx*i)*(cy + sy*j)*(cz + sz*k).
    void FactorXYZ (nReal& rfCx, nReal& rfSx, nReal& rfCy, nReal& rfSy,
        nReal& rfCz, nReal& rfSz);

    // Factor to (cx + sx*i)*(cz + sz*k)*(cy + sy*j).
    void FactorXZY (nReal& rfCx, nReal& rfSx, nReal& rfCz, nReal& rfSz,
        nReal& rfCy, nReal& rfSy);

    // Factor to (cy + sy*j)*(cz + sz*k)*(cx + sx*i).
    void FactorYZX (nReal& rfCy, nReal& rfSy, nReal& rfCz, nReal& rfSz,
        nReal& rfCx, nReal& rfSx);

    // Factor to (cy + sy*j)*(cx + sx*i)*(cz + sz*k).
    void FactorYXZ (nReal& rfCy, nReal& rfSy, nReal& rfCx, nReal& rfSx,
        nReal& rfCz, nReal& rfSz);

    // Factor to (cz + sz*k)*(cx + sx*i)*(cy + sy*j).
    void FactorZXY (nReal& rfCz, nReal& rfSz, nReal& rfCx, nReal& rfSx,
        nReal& rfCy, nReal& rfSy);

    // Factor to (cz + sz*k)*(cy + sy*j)*(cx + sx*i).
    void FactorZYX (nReal& rfCz, nReal& rfSz, nReal& rfCy, nReal& rfSy,
        nReal& rfCx, nReal& rfSx);

    // *** Find closest quaternions with constrained angles.
    class Constraints
    {
    public:
        Constraints ()
        {
            // Members are uninitialized.
        }

        Constraints (nReal fMinAngle, nReal fMaxAngle)
        {
            SetAngles(fMinAngle,fMaxAngle);
        }

        void SetAngles (nReal fMinAngle, nReal fMaxAngle)
        {
            m_fMinAngle = fMinAngle;
            m_fMaxAngle = fMaxAngle;
            m_fCosMinAngle = CMath<nReal>::Cos(m_fMinAngle);
            m_fSinMinAngle = CMath<nReal>::Sin(m_fMinAngle);
            m_fCosMaxAngle = CMath<nReal>::Cos(m_fMaxAngle);
            m_fSinMaxAngle = CMath<nReal>::Sin(m_fMaxAngle);
            m_fDiffCosMaxMin = m_fCosMaxAngle - m_fCosMinAngle;
            m_fDiffSinMaxMin = m_fSinMaxAngle - m_fSinMinAngle;
            nReal fAvrAngle = ((nReal)0.5)*(m_fMinAngle + m_fMaxAngle);
            m_fCosAvrAngle = CMath<nReal>::Cos(fAvrAngle);
            m_fSinAvrAngle = CMath<nReal>::Sin(fAvrAngle);
        }

        bool IsValid (nReal fX, nReal fY) const
        {
            // (x,y) must be unit-length.

            // Test whether (x,y) satisfies the constraints.
            nReal fXm = fX - m_fCosMinAngle;
            nReal fYm = fY - m_fSinMinAngle;
            if (fXm*m_fDiffSinMaxMin >= fYm*m_fDiffCosMaxMin)
            {
                return true;
            }

            // Test whether (-x,-y) satisfies the constraints.
            nReal fXp = fX + m_fCosMinAngle;
            nReal fYp = fY + m_fSinMinAngle;
            if (fXp*m_fDiffSinMaxMin <= fYp*m_fDiffCosMaxMin)
            {
                return true;
            }

            return false;
        }

        nReal m_fMinAngle;       // in [-PI/2,PI/2]
        nReal m_fMaxAngle;       // in [m_fMinAngle/2,PI/2]
        nReal m_fCosMinAngle;    // = cos(m_fMinAngle)
        nReal m_fSinMinAngle;    // = sin(m_fMinAngle)
        nReal m_fCosMaxAngle;    // = cos(m_fMaxAngle)
        nReal m_fSinMaxAngle;    // = sin(m_fMaxAngle)
        nReal m_fDiffCosMaxMin;  // = cos(m_fMaxAngle) - cos(m_fMinAngle)
        nReal m_fDiffSinMaxMin;  // = sin(m_fMaxAngle) - sin(m_fMinAngle)
        nReal m_fCosAvrAngle;    // = cos((m_fMinAngle + m_fMaxAngle)/2)
        nReal m_fSinAvrAngle;    // = sin((m_fMinAngle + mM_faxAngle)/2)
    };

    // Closest constrained CQuaternion of the form (cx + sx*i).
    CQuaternion GetClosestX (const Constraints& rkXCon) const;

    // Closest constrained CQuaternion of the form (cy + sy*j).
    CQuaternion GetClosestY (const Constraints& rkYCon) const;

    // Closest constrained CQuaternion of the form (cz + sz*k).
    CQuaternion GetClosestZ (const Constraints& rkZCon) const;

    // Closest constrained CQuaternion of the form (cx + sx*i)*(cy + sy*j).
    CQuaternion GetClosestXY (const Constraints& rkXCon,
        const Constraints& rkYCon) const;

    // Closest constrained CQuaternion of the form (cy + sy*j)*(cx + sx*i).
    CQuaternion GetClosestYX (const Constraints& rkYCon,
        const Constraints& rkXCon) const;

    // Closest constrained CQuaternion of the form (cz + sz*k)*(cx + sx*i).
    CQuaternion GetClosestZX (const Constraints& rkZCon,
        const Constraints& rkXCon) const;

    // Closest constrained CQuaternion of the form (cx + sx*i)*(cz + sz*k).
    CQuaternion GetClosestXZ (const Constraints& rkXCon,
        const Constraints& rkZCon) const;

    // Closest constrained CQuaternion of the form (cz + sz*k)*(cy + sy*j).
    CQuaternion GetClosestZY (const Constraints& rkZCon,
        const Constraints& rkYCon) const;

    // Closest constrained CQuaternion of the form (cy + sy*j)*(cz + sz*k).
    CQuaternion GetClosestYZ (const Constraints& rkYCon,
        const Constraints& rkZCon) const;

    // special values
	// нет вращения 
    NOVA_EXPORT static const CQuaternion IDENTITY;
	// нулевой
    NOVA_EXPORT static const CQuaternion ZERO;

private:
    // support for comparisons
    nInt32 CompareArrays (const CQuaternion& rkQ) const;

    // Closest unconstrained CQuaternion of the form:
    //   (cx + sx*i) when iAxis = 1,
    //   (cy + sy*j) when iAxis = 2,
    //   (cz + sz*k) when iAxis = 3
    CQuaternion GetClosest (nInt32 iAxis) const;

    // Closest constrained CQuaternion of the form:
    //   (cx + sx*i) when iAxis = 1,
    //   (cy + sy*j) when iAxis = 2,
    //   (cz + sz*k) when iAxis = 3
    CQuaternion GetClosest (nInt32 iAxis, const Constraints& rkCon) const;

    // support for FromRotationMatrix
    NOVA_EXPORT static nInt32 ms_iNext[3];

    // support for closest quaternions
    NOVA_EXPORT static nReal ms_fTolerance;
    NOVA_EXPORT static nReal ms_fRootTwo;
    NOVA_EXPORT static nReal ms_fRootHalf;

    nReal m_afTuple[4];
};

template <class nReal>
inline CQuaternion<nReal> operator* (nReal fScalar, const CQuaternion<nReal>& rkQ);

template <class nReal>
CQuaternion<nReal>::CQuaternion ()
{
    // uninitialized for performance in array construction
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal>::CQuaternion (nReal fW, nReal fX, nReal fY, nReal fZ)
{
    m_afTuple[0] = fW;
    m_afTuple[1] = fX;
    m_afTuple[2] = fY;
    m_afTuple[3] = fZ;
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal>::CQuaternion (const CQuaternion& rkQ)
{
    m_afTuple[0] = rkQ.m_afTuple[0];
    m_afTuple[1] = rkQ.m_afTuple[1];
    m_afTuple[2] = rkQ.m_afTuple[2];
    m_afTuple[3] = rkQ.m_afTuple[3];
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal>::CQuaternion (const CMatrix3<nReal>& rkRot)
{
    FromRotationMatrix(rkRot);
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal>::CQuaternion (const CVector3<nReal>& rkAxis, nReal fAngle)
{
    FromAxisAngle(rkAxis,fAngle);
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal>::CQuaternion (const CVector3<nReal> akRotColumn[3])
{
    FromRotationMatrix(akRotColumn);
}
//----------------------------------------------------------------------------
template <class nReal>
inline CQuaternion<nReal>::operator const nReal* () const
{
    return m_afTuple;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CQuaternion<nReal>::operator nReal* ()
{
    return m_afTuple;
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal CQuaternion<nReal>::operator[] (nInt32 i) const
{
    return m_afTuple[i];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal& CQuaternion<nReal>::operator[] (nInt32 i)
{
    return m_afTuple[i];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal CQuaternion<nReal>::W () const
{
    return m_afTuple[0];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal& CQuaternion<nReal>::W ()
{
    return m_afTuple[0];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal CQuaternion<nReal>::X () const
{
    return m_afTuple[1];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal& CQuaternion<nReal>::X ()
{
    return m_afTuple[1];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal CQuaternion<nReal>::Y () const
{
    return m_afTuple[2];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal& CQuaternion<nReal>::Y ()
{
    return m_afTuple[2];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal CQuaternion<nReal>::Z () const
{
    return m_afTuple[3];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal& CQuaternion<nReal>::Z ()
{
    return m_afTuple[3];
}
//----------------------------------------------------------------------------
template <class nReal>
inline CQuaternion<nReal>& CQuaternion<nReal>::operator= (const CQuaternion& rkQ)
{
    m_afTuple[0] = rkQ.m_afTuple[0];
    m_afTuple[1] = rkQ.m_afTuple[1];
    m_afTuple[2] = rkQ.m_afTuple[2];
    m_afTuple[3] = rkQ.m_afTuple[3];
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
nInt32 CQuaternion<nReal>::CompareArrays (const CQuaternion& rkQ) const
{
    return memcmp(m_afTuple,rkQ.m_afTuple,4*sizeof(nReal));
}
//----------------------------------------------------------------------------
template <class nReal>
bool CQuaternion<nReal>::operator== (const CQuaternion& rkQ) const
{
    return CompareArrays(rkQ) == 0;
}
//----------------------------------------------------------------------------
template <class nReal>
bool CQuaternion<nReal>::operator!= (const CQuaternion& rkQ) const
{
    return CompareArrays(rkQ) != 0;
}
//----------------------------------------------------------------------------
template <class nReal>
bool CQuaternion<nReal>::operator< (const CQuaternion& rkQ) const
{
    return CompareArrays(rkQ) < 0;
}
//----------------------------------------------------------------------------
template <class nReal>
bool CQuaternion<nReal>::operator<= (const CQuaternion& rkQ) const
{
    return CompareArrays(rkQ) <= 0;
}
//----------------------------------------------------------------------------
template <class nReal>
bool CQuaternion<nReal>::operator> (const CQuaternion& rkQ) const
{
    return CompareArrays(rkQ) > 0;
}
//----------------------------------------------------------------------------
template <class nReal>
bool CQuaternion<nReal>::operator>= (const CQuaternion& rkQ) const
{
    return CompareArrays(rkQ) >= 0;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CQuaternion<nReal> CQuaternion<nReal>::operator+ (
    const CQuaternion& rkQ) const
{
    CQuaternion kSum;
    for (nInt32 i = 0; i < 4; i++)
    {
        kSum.m_afTuple[i] = m_afTuple[i] + rkQ.m_afTuple[i];
    }
    return kSum;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CQuaternion<nReal> CQuaternion<nReal>::operator- (
    const CQuaternion& rkQ) const
{
    CQuaternion kDiff;
    for (nInt32 i = 0; i < 4; i++)
    {
        kDiff.m_afTuple[i] = m_afTuple[i] - rkQ.m_afTuple[i];
    }
    return kDiff;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CQuaternion<nReal> CQuaternion<nReal>::operator* (
    const CQuaternion& rkQ) const
{
    // NOTE:  Multiplication is not generally commutative, so in most
    // cases p*q != q*p.

    CQuaternion kProd;

    kProd.m_afTuple[0] =
        m_afTuple[0]*rkQ.m_afTuple[0] -
        m_afTuple[1]*rkQ.m_afTuple[1] -
        m_afTuple[2]*rkQ.m_afTuple[2] -
        m_afTuple[3]*rkQ.m_afTuple[3];

    kProd.m_afTuple[1] =
        m_afTuple[0]*rkQ.m_afTuple[1] +
        m_afTuple[1]*rkQ.m_afTuple[0] +
        m_afTuple[2]*rkQ.m_afTuple[3] -
        m_afTuple[3]*rkQ.m_afTuple[2];

    kProd.m_afTuple[2] =
        m_afTuple[0]*rkQ.m_afTuple[2] +
        m_afTuple[2]*rkQ.m_afTuple[0] +
        m_afTuple[3]*rkQ.m_afTuple[1] -
        m_afTuple[1]*rkQ.m_afTuple[3];

    kProd.m_afTuple[3] =
        m_afTuple[0]*rkQ.m_afTuple[3] +
        m_afTuple[3]*rkQ.m_afTuple[0] +
        m_afTuple[1]*rkQ.m_afTuple[2] -
        m_afTuple[2]*rkQ.m_afTuple[1];

    return kProd;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CQuaternion<nReal> CQuaternion<nReal>::operator* (nReal fScalar) const
{
    CQuaternion kProd;
    for (nInt32 i = 0; i < 4; i++)
    {
        kProd.m_afTuple[i] = fScalar*m_afTuple[i];
    }
    return kProd;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CQuaternion<nReal> CQuaternion<nReal>::operator/ (nReal fScalar) const
{
    CQuaternion kQuot;
    nInt32 i;

    if (fScalar != (nReal)0.0)
    {
        nReal fInvScalar = ((nReal)1.0)/fScalar;
        for (i = 0; i < 4; i++)
        {
            kQuot.m_afTuple[i] = fInvScalar*m_afTuple[i];
        }
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            kQuot.m_afTuple[i] = CMath<nReal>::MAX_REAL;
        }
    }

    return kQuot;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CQuaternion<nReal> CQuaternion<nReal>::operator- () const
{
    CQuaternion kNeg;
    for (nInt32 i = 0; i < 4; i++)
    {
        kNeg.m_afTuple[i] = -m_afTuple[i];
    }
    return kNeg;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CQuaternion<nReal> operator* (nReal fScalar, const CQuaternion<nReal>& rkQ)
{
    CQuaternion<nReal> kProd;
    for (nInt32 i = 0; i < 4; i++)
    {
        kProd[i] = fScalar*rkQ[i];
    }
    return kProd;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CQuaternion<nReal>& CQuaternion<nReal>::operator+= (const CQuaternion& rkQ)
{
    for (nInt32 i = 0; i < 4; i++)
    {
        m_afTuple[i] += rkQ.m_afTuple[i];
    }
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CQuaternion<nReal>& CQuaternion<nReal>::operator-= (const CQuaternion& rkQ)
{
    for (nInt32 i = 0; i < 4; i++)
    {
        m_afTuple[i] -= rkQ.m_afTuple[i];
    }
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CQuaternion<nReal>& CQuaternion<nReal>::operator*= (nReal fScalar)
{
    for (nInt32 i = 0; i < 4; i++)
    {
        m_afTuple[i] *= fScalar;
    }
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CQuaternion<nReal>& CQuaternion<nReal>::operator/= (nReal fScalar)
{
    nInt32 i;

    if (fScalar != (nReal)0.0)
    {
        nReal fInvScalar = ((nReal)1.0)/fScalar;
        for (i = 0; i < 4; i++)
        {
            m_afTuple[i] *= fInvScalar;
        }
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            m_afTuple[i] = CMath<nReal>::MAX_REAL;
        }
    }

    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal>& CQuaternion<nReal>::FromRotationMatrix (
    const CMatrix3<nReal>& rkRot)
{
    // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
    // article "CQuaternion Calculus and Fast Animation".

    nReal fTrace = rkRot(0,0) + rkRot(1,1) + rkRot(2,2);
    nReal fRoot;

    if (fTrace > (nReal)0.0)
    {
        // |w| > 1/2, may as well choose w > 1/2
        fRoot = CMath<nReal>::Sqrt(fTrace + (nReal)1.0);  // 2w
        m_afTuple[0] = ((nReal)0.5)*fRoot;
        fRoot = ((nReal)0.5)/fRoot;  // 1/(4w)
        m_afTuple[1] = (rkRot(2,1)-rkRot(1,2))*fRoot;
        m_afTuple[2] = (rkRot(0,2)-rkRot(2,0))*fRoot;
        m_afTuple[3] = (rkRot(1,0)-rkRot(0,1))*fRoot;
    }
    else
    {
        // |w| <= 1/2
        nInt32 i = 0;
        if ( rkRot(1,1) > rkRot(0,0) )
        {
            i = 1;
        }
        if ( rkRot(2,2) > rkRot(i,i) )
        {
            i = 2;
        }
        nInt32 j = ms_iNext[i];
        nInt32 k = ms_iNext[j];

        fRoot = CMath<nReal>::Sqrt(rkRot(i,i)-rkRot(j,j)-rkRot(k,k)+(nReal)1.0);
        nReal* apfQuat[3] = { &m_afTuple[1], &m_afTuple[2], &m_afTuple[3] };
        *apfQuat[i] = ((nReal)0.5)*fRoot;
        fRoot = ((nReal)0.5)/fRoot;
        m_afTuple[0] = (rkRot(k,j)-rkRot(j,k))*fRoot;
        *apfQuat[j] = (rkRot(j,i)+rkRot(i,j))*fRoot;
        *apfQuat[k] = (rkRot(k,i)+rkRot(i,k))*fRoot;
    }

    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
void CQuaternion<nReal>::ToRotationMatrix (CMatrix3<nReal>& rkRot) const
{
    nReal fTx  = ((nReal)2.0)*m_afTuple[1];
    nReal fTy  = ((nReal)2.0)*m_afTuple[2];
    nReal fTz  = ((nReal)2.0)*m_afTuple[3];
    nReal fTwx = fTx*m_afTuple[0];
    nReal fTwy = fTy*m_afTuple[0];
    nReal fTwz = fTz*m_afTuple[0];
    nReal fTxx = fTx*m_afTuple[1];
    nReal fTxy = fTy*m_afTuple[1];
    nReal fTxz = fTz*m_afTuple[1];
    nReal fTyy = fTy*m_afTuple[2];
    nReal fTyz = fTz*m_afTuple[2];
    nReal fTzz = fTz*m_afTuple[3];

    rkRot(0,0) = (nReal)1.0-(fTyy+fTzz);
    rkRot(0,1) = fTxy-fTwz;
    rkRot(0,2) = fTxz+fTwy;
    rkRot(1,0) = fTxy+fTwz;
    rkRot(1,1) = (nReal)1.0-(fTxx+fTzz);
    rkRot(1,2) = fTyz-fTwx;
    rkRot(2,0) = fTxz-fTwy;
    rkRot(2,1) = fTyz+fTwx;
    rkRot(2,2) = (nReal)1.0-(fTxx+fTyy);
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal>& CQuaternion<nReal>::FromRotationMatrix (
    const CVector3<nReal> akRotColumn[3])
{
    CMatrix3<nReal> kRot;
    for (nInt32 iCol = 0; iCol < 3; iCol++)
    {
        kRot(0,iCol) = akRotColumn[iCol][0];
        kRot(1,iCol) = akRotColumn[iCol][1];
        kRot(2,iCol) = akRotColumn[iCol][2];
    }
    return FromRotationMatrix(kRot);
}
//----------------------------------------------------------------------------
template <class nReal>
void CQuaternion<nReal>::ToRotationMatrix (CVector3<nReal> akRotColumn[3]) const
{
    CMatrix3<nReal> kRot;
    ToRotationMatrix(kRot);
    for (nInt32 iCol = 0; iCol < 3; iCol++)
    {
        akRotColumn[iCol][0] = kRot(0,iCol);
        akRotColumn[iCol][1] = kRot(1,iCol);
        akRotColumn[iCol][2] = kRot(2,iCol);
    }
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal>& CQuaternion<nReal>::FromAxisAngle (
    const CVector3<nReal>& rkAxis, nReal fAngle)
{
    // assert:  axis[] is unit length
    //
    // The CQuaternion representing the rotation is
    //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

    nReal fHalfAngle = ((nReal)0.5)*fAngle;
    nReal fSin = CMath<nReal>::Sin(fHalfAngle);
    m_afTuple[0] = CMath<nReal>::Cos(fHalfAngle);
    m_afTuple[1] = fSin*rkAxis[0];
    m_afTuple[2] = fSin*rkAxis[1];
    m_afTuple[3] = fSin*rkAxis[2];

    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
void CQuaternion<nReal>::ToAxisAngle (CVector3<nReal>& rkAxis, nReal& rfAngle)
    const
{
    // The CQuaternion representing the rotation is
    //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

    nReal fSqrLength = m_afTuple[1]*m_afTuple[1] + m_afTuple[2]*m_afTuple[2]
        + m_afTuple[3]*m_afTuple[3];

    if (fSqrLength > CMath<nReal>::ZERO_TOLERANCE)
    {
        rfAngle = ((nReal)2.0)*CMath<nReal>::ACos(m_afTuple[0]);
        nReal fInvLength = CMath<nReal>::InvSqrt(fSqrLength);
        rkAxis[0] = m_afTuple[1]*fInvLength;
        rkAxis[1] = m_afTuple[2]*fInvLength;
        rkAxis[2] = m_afTuple[3]*fInvLength;
    }
    else
    {
        // angle is 0 (mod 2*pi), so any axis will do
        rfAngle = (nReal)0.0;
        rkAxis[0] = (nReal)1.0;
        rkAxis[1] = (nReal)0.0;
        rkAxis[2] = (nReal)0.0;
    }
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal CQuaternion<nReal>::Length () const
{
    return CMath<nReal>::Sqrt(
        m_afTuple[0]*m_afTuple[0] +
        m_afTuple[1]*m_afTuple[1] +
        m_afTuple[2]*m_afTuple[2] +
        m_afTuple[3]*m_afTuple[3]);
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal CQuaternion<nReal>::SquaredLength () const
{
    return
        m_afTuple[0]*m_afTuple[0] +
        m_afTuple[1]*m_afTuple[1] +
        m_afTuple[2]*m_afTuple[2] +
        m_afTuple[3]*m_afTuple[3];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal CQuaternion<nReal>::Dot (const CQuaternion& rkQ) const
{
    nReal fDot = (nReal)0.0;
    for (nInt32 i = 0; i < 4; i++)
    {
        fDot += m_afTuple[i]*rkQ.m_afTuple[i];
    }
    return fDot;
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal CQuaternion<nReal>::Normalize ()
{
    nReal fLength = Length();

    if (fLength > CMath<nReal>::ZERO_TOLERANCE)
    {
        nReal fInvLength = ((nReal)1.0)/fLength;
        m_afTuple[0] *= fInvLength;
        m_afTuple[1] *= fInvLength;
        m_afTuple[2] *= fInvLength;
        m_afTuple[3] *= fInvLength;
    }
    else
    {
        fLength = (nReal)0.0;
        m_afTuple[0] = (nReal)0.0;
        m_afTuple[1] = (nReal)0.0;
        m_afTuple[2] = (nReal)0.0;
        m_afTuple[3] = (nReal)0.0;
    }

    return fLength;
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal> CQuaternion<nReal>::Inverse () const
{
    CQuaternion kInverse;

    nReal fNorm = (nReal)0.0;
    nInt32 i;
    for (i = 0; i < 4; i++)
    {
        fNorm += m_afTuple[i]*m_afTuple[i];
    }

    if (fNorm > (nReal)0.0)
    {
        nReal fInvNorm = ((nReal)1.0)/fNorm;
        kInverse.m_afTuple[0] = m_afTuple[0]*fInvNorm;
        kInverse.m_afTuple[1] = -m_afTuple[1]*fInvNorm;
        kInverse.m_afTuple[2] = -m_afTuple[2]*fInvNorm;
        kInverse.m_afTuple[3] = -m_afTuple[3]*fInvNorm;
    }
    else
    {
        // return an invalid result to flag the error
        for (i = 0; i < 4; i++)
        {
            kInverse.m_afTuple[i] = (nReal)0.0;
        }
    }

    return kInverse;
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal> CQuaternion<nReal>::Conjugate () const
{
    return CQuaternion(m_afTuple[0],-m_afTuple[1],-m_afTuple[2],
        -m_afTuple[3]);
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal> CQuaternion<nReal>::Exp () const
{
    // If q = A*(x*i+y*j+z*k) where (x,y,z) is unit length, then
    // exp(q) = cos(A)+sin(A)*(x*i+y*j+z*k).  If sin(A) is near zero,
    // use exp(q) = cos(A)+A*(x*i+y*j+z*k) since A/sin(A) has limit 1.

    CQuaternion kResult;

    nReal fAngle = CMath<nReal>::Sqrt(m_afTuple[1]*m_afTuple[1] +
        m_afTuple[2]*m_afTuple[2] + m_afTuple[3]*m_afTuple[3]);

    nReal fSin = CMath<nReal>::Sin(fAngle);
    kResult.m_afTuple[0] = CMath<nReal>::Cos(fAngle);

    nInt32 i;

    if (CMath<nReal>::FAbs(fSin) >= CMath<nReal>::ZERO_TOLERANCE)
    {
        nReal fCoeff = fSin/fAngle;
        for (i = 1; i <= 3; i++)
        {
            kResult.m_afTuple[i] = fCoeff*m_afTuple[i];
        }
    }
    else
    {
        for (i = 1; i <= 3; i++)
        {
            kResult.m_afTuple[i] = m_afTuple[i];
        }
    }

    return kResult;
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal> CQuaternion<nReal>::Log () const
{
    // If q = cos(A)+sin(A)*(x*i+y*j+z*k) where (x,y,z) is unit length, then
    // log(q) = A*(x*i+y*j+z*k).  If sin(A) is near zero, use log(q) =
    // sin(A)*(x*i+y*j+z*k) since sin(A)/A has limit 1.

    CQuaternion kResult;
    kResult.m_afTuple[0] = (nReal)0.0;

    nInt32 i;

    if (CMath<nReal>::FAbs(m_afTuple[0]) < (nReal)1.0)
    {
        nReal fAngle = CMath<nReal>::ACos(m_afTuple[0]);
        nReal fSin = CMath<nReal>::Sin(fAngle);
        if (CMath<nReal>::FAbs(fSin) >= CMath<nReal>::ZERO_TOLERANCE)
        {
            nReal fCoeff = fAngle/fSin;
            for (i = 1; i <= 3; i++)
            {
                kResult.m_afTuple[i] = fCoeff*m_afTuple[i];
            }
            return kResult;
        }
    }

    for (i = 1; i <= 3; i++)
    {
        kResult.m_afTuple[i] = m_afTuple[i];
    }
    return kResult;
}
//----------------------------------------------------------------------------
template <class nReal>
CVector3<nReal> CQuaternion<nReal>::Rotate (const CVector3<nReal>& rkVector)
    const
{
    // Given a vector u = (x0,y0,z0) and a unit length CQuaternion
    // q = <w,x,y,z>, the vector v = (x1,y1,z1) which represents the
    // rotation of u by q is v = q*u*q^{-1} where * indicates CQuaternion
    // multiplication and where u is treated as the CQuaternion <0,x0,y0,z0>.
    // Note that q^{-1} = <w,-x,-y,-z>, so no nReal work is required to
    // invert q.  Now
    //
    //   q*u*q^{-1} = q*<0,x0,y0,z0>*q^{-1}
    //     = q*(x0*i+y0*j+z0*k)*q^{-1}
    //     = x0*(q*i*q^{-1})+y0*(q*j*q^{-1})+z0*(q*k*q^{-1})
    //
    // As 3-vectors, q*i*q^{-1}, q*j*q^{-1}, and 2*k*q^{-1} are the columns
    // of the rotation matrix computed in CQuaternion<nReal>::ToRotationMatrix.
    // The vector v is obtained as the product of that rotation matrix with
    // vector u.  As such, the CQuaternion representation of a rotation
    // matrix requires less space than the matrix and more time to compute
    // the rotated vector.  Typical space-time tradeoff...

    CMatrix3<nReal> kRot;
    ToRotationMatrix(kRot);
    return kRot*rkVector;
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal>& CQuaternion<nReal>::Slerp (nReal fT, const CQuaternion& rkP,
    const CQuaternion& rkQ)
{
    nReal fCos = rkP.Dot(rkQ);
    nReal fAngle = CMath<nReal>::ACos(fCos);

    if (CMath<nReal>::FAbs(fAngle) >= CMath<nReal>::ZERO_TOLERANCE)
    {
        nReal fSin = CMath<nReal>::Sin(fAngle);
        nReal fInvSin = ((nReal)1.0)/fSin;
        nReal fTAngle = fT*fAngle;
        nReal fCoeff0 = CMath<nReal>::Sin(fAngle - fTAngle)*fInvSin;
        nReal fCoeff1 = CMath<nReal>::Sin(fTAngle)*fInvSin;

        // Profiling showed that the old line of code,
        //   *this = fCoeff0*rkP + fCoeff1*rkQ;
        // was using a large number of cycles, more so than the sin and cos
        // function calls.  The following inlined code is much faster.
        m_afTuple[0] = fCoeff0*rkP.m_afTuple[0] + fCoeff1*rkQ.m_afTuple[0];
        m_afTuple[1] = fCoeff0*rkP.m_afTuple[1] + fCoeff1*rkQ.m_afTuple[1];
        m_afTuple[2] = fCoeff0*rkP.m_afTuple[2] + fCoeff1*rkQ.m_afTuple[2];
        m_afTuple[3] = fCoeff0*rkP.m_afTuple[3] + fCoeff1*rkQ.m_afTuple[3];
    }
    else
    {
        // Based on the problem with the code in the previous block, inlining
        // the old code
        //   *this = rkP;
        m_afTuple[0] = rkP.m_afTuple[0];
        m_afTuple[1] = rkP.m_afTuple[1];
        m_afTuple[2] = rkP.m_afTuple[2];
        m_afTuple[3] = rkP.m_afTuple[3];
    }

    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal>& CQuaternion<nReal>::SlerpExtraSpins (nReal fT,
    const CQuaternion& rkP, const CQuaternion& rkQ, nInt32 iExtraSpins)
{
    nReal fCos = rkP.Dot(rkQ);
    nReal fAngle = CMath<nReal>::ACos(fCos);

    if (CMath<nReal>::FAbs(fAngle) >= CMath<nReal>::ZERO_TOLERANCE)
    {
        nReal fSin = CMath<nReal>::Sin(fAngle);
        nReal fPhase = CMath<nReal>::PI*iExtraSpins*fT;
        nReal fInvSin = ((nReal)1.0)/fSin;
        nReal fCoeff0 = CMath<nReal>::Sin(((nReal)1.0-fT)*fAngle-fPhase)*fInvSin;
        nReal fCoeff1 = CMath<nReal>::Sin(fT*fAngle + fPhase)*fInvSin;
        *this = fCoeff0*rkP + fCoeff1*rkQ;
    }
    else
    {
        *this = rkP;
    }

    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal>& CQuaternion<nReal>::Intermediate (const CQuaternion& rkQ0,
    const CQuaternion& rkQ1, const CQuaternion& rkQ2)
{
    // assert:  Q0, Q1, Q2 all unit-length
    CQuaternion kQ1Inv = rkQ1.Conjugate();
    CQuaternion kP0 = kQ1Inv*rkQ0;
    CQuaternion kP2 = kQ1Inv*rkQ2;
    CQuaternion kArg = -((nReal)0.25)*(kP0.Log()+kP2.Log());
    CQuaternion kA = rkQ1*kArg.Exp();
    *this = kA;

    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal>& CQuaternion<nReal>::Squad (nReal fT, const CQuaternion& rkQ0,
    const CQuaternion& rkA0, const CQuaternion& rkA1, const CQuaternion& rkQ1)
{
    nReal fSlerpT = ((nReal)2.0)*fT*((nReal)1.0-fT);
    CQuaternion kSlerpP = Slerp(fT,rkQ0,rkQ1);
    CQuaternion kSlerpQ = Slerp(fT,rkA0,rkA1);
    return Slerp(fSlerpT,kSlerpP,kSlerpQ);
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal>& CQuaternion<nReal>::Align (const CVector3<nReal>& rkV1,
    const CVector3<nReal>& rkV2)
{
    // If V1 and V2 are not parallel, the axis of rotation is the unit-length
    // vector U = Cross(V1,V2)/Length(Cross(V1,V2)).  The angle of rotation,
    // A, is the angle between V1 and V2.  The CQuaternion for the rotation is
    // q = cos(A/2) + sin(A/2)*(ux*i+uy*j+uz*k) where U = (ux,uy,uz).
    //
    // (1) Rather than extract A = acos(Dot(V1,V2)), multiply by 1/2, then
    //     compute sin(A/2) and cos(A/2), we reduce the computational costs by
    //     computing the bisector B = (V1+V2)/Length(V1+V2), so cos(A/2) =
    //     Dot(V1,B).
    //
    // (2) The rotation axis is U = Cross(V1,B)/Length(Cross(V1,B)), but
    //     Length(Cross(V1,B)) = Length(V1)*Length(B)*sin(A/2) = sin(A/2), in
    //     which case sin(A/2)*(ux*i+uy*j+uz*k) = (cx*i+cy*j+cz*k) where
    //     C = Cross(V1,B).
    //
    // If V1 = V2, then B = V1, cos(A/2) = 1, and U = (0,0,0).  If V1 = -V2,
    // then B = 0.  This can happen even if V1 is approximately -V2 using
    // floating point arithmetic, since CVector3::Normalize checks for
    // closeness to zero and returns the zero vector accordingly.  The test
    // for exactly zero is usually not recommend for floating point
    // arithmetic, but the implementation of CVector3::Normalize guarantees
    // the comparison is robust.  In this case, the A = pi and any axis
    // perpendicular to V1 may be used as the rotation axis.

    CVector3<nReal> kBisector = rkV1 + rkV2;
    kBisector.Normalize();

    nReal fCosHalfAngle = rkV1.Dot(kBisector);
    CVector3<nReal> kCross;

    m_afTuple[0] = fCosHalfAngle;

    if (fCosHalfAngle != (nReal)0.0)
    {
        kCross = rkV1.Cross(kBisector);
        m_afTuple[1] = kCross.X();
        m_afTuple[2] = kCross.Y();
        m_afTuple[3] = kCross.Z();
    }
    else
    {
        nReal fInvLength;
        if (CMath<nReal>::FAbs(rkV1[0]) >= CMath<nReal>::FAbs(rkV1[1]))
        {
            // V1.x or V1.z is the largest magnitude component
            fInvLength = CMath<nReal>::InvSqrt(rkV1[0]*rkV1[0] +
                rkV1[2]*rkV1[2]);

            m_afTuple[1] = -rkV1[2]*fInvLength;
            m_afTuple[2] = (nReal)0.0;
            m_afTuple[3] = +rkV1[0]*fInvLength;
        }
        else
        {
            // V1.y or V1.z is the largest magnitude component
            fInvLength = CMath<nReal>::InvSqrt(rkV1[1]*rkV1[1] +
                rkV1[2]*rkV1[2]);

            m_afTuple[1] = (nReal)0.0;
            m_afTuple[2] = +rkV1[2]*fInvLength;
            m_afTuple[3] = -rkV1[1]*fInvLength;
        }
    }

    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
void CQuaternion<nReal>::DecomposeTwistTimesSwing (
    const CVector3<nReal>& rkV1, CQuaternion& rkTwist, CQuaternion& rkSwing)
{
    CVector3<nReal> kV2 = Rotate(rkV1);
    rkSwing = Align(rkV1,kV2);
    rkTwist = (*this)*rkSwing.Conjugate();
}
//----------------------------------------------------------------------------
template <class nReal>
void CQuaternion<nReal>::DecomposeSwingTimesTwist (
    const CVector3<nReal>& rkV1, CQuaternion& rkSwing, CQuaternion& rkTwist)
{
    CVector3<nReal> kV2 = Rotate(rkV1);
    rkSwing = Align(rkV1,kV2);
    rkTwist = rkSwing.Conjugate()*(*this);
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal> CQuaternion<nReal>::GetClosest (nInt32 iAxis) const
{
    // The appropriate nonzero components will be set later.
    CQuaternion kQ((nReal)0,(nReal)0,(nReal)0,(nReal)0);
    nReal fP0 = m_afTuple[0];
    nReal fP1 = m_afTuple[iAxis];
    nReal fSqrLength = fP0*fP0 + fP1*fP1;
    if (fSqrLength > ms_fTolerance)
    {
        // A unique closest point.
        nReal fInvLength = CMath<nReal>::InvSqrt(fSqrLength);
        kQ[0] = fP0*fInvLength;
        kQ[iAxis] = fP1*fInvLength;
    }
    else
    {
        // Infinitely many solutions, choose the one for theta = 0.
        kQ[0] = (nReal)1;
        kQ[iAxis] = (nReal)0;
    }
    return kQ;
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal> CQuaternion<nReal>::GetClosestX () const
{
    return GetClosest(1);
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal> CQuaternion<nReal>::GetClosestY () const
{
    return GetClosest(2);
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal> CQuaternion<nReal>::GetClosestZ () const
{
    return GetClosest(3);
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal> CQuaternion<nReal>::GetClosestXY () const
{
    CQuaternion kQ;

    nReal fDet = m_afTuple[0]*m_afTuple[3] - m_afTuple[1]*m_afTuple[2];
    if(CMath<nReal>::FAbs(fDet) < (nReal)0.5 - ms_fTolerance)
    {
        nReal fDiscr = (nReal)1 - ((nReal)4)*fDet*fDet;
        fDiscr = CMath<nReal>::Sqrt(CMath<nReal>::FAbs(fDiscr));
        nReal fA = m_afTuple[0]*m_afTuple[1] + m_afTuple[2]*m_afTuple[3];
        nReal fB = m_afTuple[0]*m_afTuple[0] - m_afTuple[1]*m_afTuple[1] +
            m_afTuple[2]*m_afTuple[2] - m_afTuple[3]*m_afTuple[3];

        nReal fC0, fS0, fC1, fS1, fInvLength;

        if (fB >= (nReal)0)
        {
            fC0 = ((nReal)0.5)*(fDiscr + fB);
            fS0 = fA;
        }
        else
        {
            fC0 = fA;
            fS0 = ((nReal)0.5)*(fDiscr - fB);
        }
        fInvLength = CMath<nReal>::InvSqrt(fC0*fC0 + fS0*fS0);
        fC0 *= fInvLength;
        fS0 *= fInvLength;

        fC1 = m_afTuple[0]*fC0 + m_afTuple[1]*fS0;
        fS1 = m_afTuple[2]*fC0 + m_afTuple[3]*fS0;
        fInvLength = CMath<nReal>::InvSqrt(fC1*fC1 + fS1*fS1);
        fC1 *= fInvLength;
        fS1 *= fInvLength;

        kQ[0] = fC0*fC1;
        kQ[1] = fS0*fC1;
        kQ[2] = fC0*fS1;
        kQ[3] = fS0*fS1;
    }
    else
    {
        nReal fInvLength = CMath<nReal>::InvSqrt(CMath<nReal>::FAbs(fDet));
        kQ[0] = m_afTuple[0]*fInvLength;
        kQ[1] = m_afTuple[1]*fInvLength;
        kQ[2] = (nReal)0;
        kQ[3] = (nReal)0;
    }

    return kQ;
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal> CQuaternion<nReal>::GetClosestYX () const
{
    CQuaternion kAlt(m_afTuple[0],m_afTuple[1],m_afTuple[2],-m_afTuple[3]);
    CQuaternion kQ = kAlt.GetClosestXY();
    kQ[3] = -kQ[3];
    return kQ;
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal> CQuaternion<nReal>::GetClosestZX () const
{
    CQuaternion kAlt(m_afTuple[0],m_afTuple[1],m_afTuple[3],m_afTuple[2]);
    CQuaternion kQ = kAlt.GetClosestXY();
    nReal fSave = kQ[2];
    kQ[2] = kQ[3];
    kQ[3] = fSave;
    return kQ;
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal> CQuaternion<nReal>::GetClosestXZ () const
{
    CQuaternion kAlt(m_afTuple[0],m_afTuple[1],-m_afTuple[3],m_afTuple[2]);
    CQuaternion kQ = kAlt.GetClosestXY();
    nReal fSave = kQ[2];
    kQ[2] = kQ[3];
    kQ[3] = -fSave;
    return kQ;
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal> CQuaternion<nReal>::GetClosestYZ () const
{
    CQuaternion kAlt(m_afTuple[0],m_afTuple[2],m_afTuple[3],m_afTuple[1]);
    CQuaternion kQ = kAlt.GetClosestXY();
    nReal fSave = kQ[3];
    kQ[3] = kQ[2];
    kQ[2] = kQ[1];
    kQ[1] = fSave;
    return kQ;
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal> CQuaternion<nReal>::GetClosestZY () const
{
    CQuaternion kAlt(m_afTuple[0],m_afTuple[2],m_afTuple[3],-m_afTuple[1]);
    CQuaternion kQ = kAlt.GetClosestXY();
    nReal fSave = kQ[3];
    kQ[3] = kQ[2];
    kQ[2] = kQ[1];
    kQ[1] = -fSave;
    return kQ;
}
//----------------------------------------------------------------------------
template <class nReal>
void CQuaternion<nReal>::FactorXYZ (nReal& rfCx, nReal& rfSx, nReal& rfCy,
    nReal& rfSy, nReal& rfCz, nReal& rfSz)
{
    nReal fA = m_afTuple[0]*m_afTuple[1] - m_afTuple[2]*m_afTuple[3];
    nReal fB = ((nReal)0.5)*(
          m_afTuple[0]*m_afTuple[0]
        - m_afTuple[1]*m_afTuple[1]
        - m_afTuple[2]*m_afTuple[2]
        + m_afTuple[3]*m_afTuple[3]);

    nReal fLength = CMath<nReal>::Sqrt(fA*fA + fB*fB);
    if (fLength > ms_fTolerance)
    {
        nReal fInvLength = ((nReal)1)/fLength;
        nReal fSigma0 = fA * fInvLength;
        nReal fGamma0 = fB * fInvLength;
        if (fGamma0 >= (nReal)0)
        {
            rfCx = CMath<nReal>::Sqrt(((nReal)0.5)*((nReal)1 + fGamma0));
            rfSx = ((nReal)0.5)*fSigma0/rfCx;
        }
        else
        {
            rfSx = CMath<nReal>::Sqrt(((nReal)0.5)*((nReal)1 - fGamma0));
            rfCx = ((nReal)0.5)*fSigma0/rfSx;
        }

        nReal fTmp0 = rfCx*m_afTuple[0] + rfSx*m_afTuple[1];
        nReal fTmp1 = rfCx*m_afTuple[3] - rfSx*m_afTuple[2];
        fInvLength = CMath<nReal>::InvSqrt(fTmp0*fTmp0 + fTmp1*fTmp1);
        rfCz = fTmp0 * fInvLength;
        rfSz = fTmp1 * fInvLength;

        if(CMath<nReal>::FAbs(rfCz) >= CMath<nReal>::FAbs(rfSz))
        {
            fInvLength = ((nReal)1)/rfCz;
            rfCy = fTmp0 * fInvLength;
            rfSy = (rfCx*m_afTuple[2] + rfSx*m_afTuple[3]) * fInvLength;
        }
        else
        {
            fInvLength = ((nReal)1)/rfSz;
            rfCy = fTmp1 * fInvLength;
            rfSy = (rfCx*m_afTuple[1] - rfSx*m_afTuple[0]) * fInvLength;
        }
    }
    else
    {
        // Infinitely many solutions.  Choose one of them.
        if(m_afTuple[0]*m_afTuple[2] + m_afTuple[1]*m_afTuple[3] > (nReal)0)
        {
            // p = (p0,p1,p0,p1)
            rfCx = (nReal)1;
            rfSx = (nReal)0;
            rfCy = ms_fRootHalf;
            rfSy = ms_fRootHalf;
            rfCz = ms_fRootTwo * m_afTuple[0];
            rfSz = ms_fRootTwo * m_afTuple[1];
        }
        else
        {
            // p = (p0,p1,-p0,-p1)
            rfCx = (nReal)1;
            rfSx = (nReal)0;
            rfCy = ms_fRootHalf;
            rfSy = -ms_fRootHalf;
            rfCz = ms_fRootTwo * m_afTuple[0];
            rfSz = -ms_fRootTwo * m_afTuple[1];
        }
    }
}
//----------------------------------------------------------------------------
template <class nReal>
void CQuaternion<nReal>::FactorXZY (nReal& rfCx, nReal& rfSx, nReal& rfCz,
    nReal& rfSz, nReal& rfCy, nReal& rfSy)
{
    CQuaternion pkAlt(m_afTuple[0],m_afTuple[1],m_afTuple[3],-m_afTuple[2]);
    pkAlt.FactorXYZ(rfCx,rfSx,rfCz,rfSz,rfCy,rfSy);
    rfSy = -rfSy;
}
//----------------------------------------------------------------------------
template <class nReal>
void CQuaternion<nReal>::FactorYZX (nReal& rfCy, nReal& rfSy, nReal& rfCz,
    nReal& rfSz, nReal& rfCx, nReal& rfSx)
{
    CQuaternion pkAlt(m_afTuple[0],-m_afTuple[2],m_afTuple[3],-m_afTuple[1]);
    pkAlt.FactorXYZ(rfCy,rfSy,rfCz,rfSz,rfCx,rfSx);
    rfSx = -rfSx;
    rfSy = -rfSy;
}
//----------------------------------------------------------------------------
template <class nReal>
void CQuaternion<nReal>::FactorYXZ (nReal& rfCy, nReal& rfSy, nReal& rfCx,
    nReal& rfSx, nReal& rfCz, nReal& rfSz)
{
    CQuaternion pkAlt(m_afTuple[0],-m_afTuple[2],m_afTuple[1],m_afTuple[3]);
    pkAlt.FactorXYZ(rfCy,rfSy,rfCx,rfSx,rfCz,rfSz);
    rfSy = -rfSy;
}
//----------------------------------------------------------------------------
template <class nReal>
void CQuaternion<nReal>::FactorZXY (nReal& rfCz, nReal& rfSz, nReal& rfCx,
    nReal& rfSx, nReal& rfCy, nReal& rfSy)
{
    CQuaternion pkAlt(m_afTuple[0],-m_afTuple[3],m_afTuple[1],-m_afTuple[2]);
    pkAlt.FactorXYZ(rfCz,rfSz,rfCx,rfSx,rfCy,rfSy);
    rfSy = -rfSy;
    rfSz = -rfSz;
}
//----------------------------------------------------------------------------
template <class nReal>
void CQuaternion<nReal>::FactorZYX (nReal& rfCz, nReal& rfSz, nReal& rfCy,
    nReal& rfSy, nReal& rfCx, nReal& rfSx)
{
    CQuaternion pkAlt(m_afTuple[0],m_afTuple[3],-m_afTuple[2],m_afTuple[1]);
    pkAlt.FactorXYZ(rfCz,rfSz,rfCy,rfSy,rfCx,rfSx);
    rfSy = -rfSy;
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal> CQuaternion<nReal>::GetClosest (nInt32 iAxis,
    const Constraints& rkCon) const
{
    CQuaternion kQ((nReal)0,(nReal)0,(nReal)0,(nReal)0);

    nReal fP0 = m_afTuple[0];
    nReal fP1 = m_afTuple[iAxis];
    nReal fSqrLength = fP0*fP0 + fP1*fP1;
    if (fSqrLength > ms_fTolerance)
    {
        nReal fInvLength = CMath<nReal>::InvSqrt(fSqrLength);
        fP0 *= fInvLength;
        fP1 *= fInvLength;
        if (rkCon.IsValid(fP0,fP1))
        {
            // The maximum occurs at an interior point.
            kQ[0] = fP0;
            kQ[iAxis] = fP1;
        }
        else
        {
            // The maximum occurs at a boundary point.
            nReal fCsMin = rkCon.m_fCosMinAngle;
            nReal fSnMin = rkCon.m_fSinMinAngle;
            nReal fDotMinAngle = fP0*fCsMin + fP1*fSnMin;
            if (fDotMinAngle < (nReal)0)
            {
                fCsMin = -fCsMin;
                fSnMin = -fSnMin;
                fDotMinAngle = -fDotMinAngle;
            }

            nReal fCsMax = rkCon.m_fCosMaxAngle;
            nReal fSnMax = rkCon.m_fSinMaxAngle;
            nReal fDotMaxAngle = fP0*fCsMax + fP1*fSnMax;
            if (fDotMaxAngle < (nReal)0)
            {
                fCsMax = -fCsMax;
                fSnMax = -fSnMax;
                fDotMaxAngle = -fDotMaxAngle;
            }

            if (fDotMaxAngle >= fDotMinAngle)
            {
                kQ[0] = fCsMax;
                kQ[iAxis] = fSnMax;
            }
            else
            {
                kQ[0] = fCsMin;
                kQ[iAxis] = fSnMin;
            }
        }
    }
    else
    {
        // Infinitely many solutions, choose one that satisfies the angle
        // constraints.
        kQ[0] = rkCon.m_fCosAvrAngle;
        kQ[iAxis] = rkCon.m_fSinAvrAngle;
    }

    return kQ;
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal> CQuaternion<nReal>::GetClosestX (const Constraints& rkXCon)
    const
{
    return GetClosest(1,rkXCon);
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal> CQuaternion<nReal>::GetClosestY (const Constraints& rkYCon)
    const
{
    return GetClosest(2,rkYCon);
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal> CQuaternion<nReal>::GetClosestZ (const Constraints& rkZCon)
    const
{
    return GetClosest(3,rkZCon);
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal> CQuaternion<nReal>::GetClosestXY (const Constraints& rkXCon,
    const Constraints& rkYCon) const
{
    CQuaternion kQ, kTmp;
    nReal fC0, fS0, fC1, fS1, fInvLength;

    nReal fDet = m_afTuple[0]*m_afTuple[3] - m_afTuple[1]*m_afTuple[2];
    if (CMath<nReal>::FAbs(fDet) < (nReal)0.5 - ms_fTolerance)
    {
        nReal fDiscr = CMath<nReal>::Sqrt(CMath<nReal>::FAbs((nReal)1 -
            ((nReal)4)*fDet*fDet));
        nReal fA = m_afTuple[0]*m_afTuple[1] + m_afTuple[2]*m_afTuple[3];
        nReal fB = m_afTuple[0]*m_afTuple[0] - m_afTuple[1]*m_afTuple[1]
            + m_afTuple[2]*m_afTuple[2] - m_afTuple[3]*m_afTuple[3];

        if (fB >= (nReal)0)
        {
            fC0 = ((nReal)0.5)*(fDiscr + fB);
            fS0 = fA;
        }
        else
        {
            fC0 = fA;
            fS0 = ((nReal)0.5)*(fDiscr - fB);
        }
        fInvLength = CMath<nReal>::InvSqrt(fC0*fC0 + fS0*fS0);
        fC0 *= fInvLength;
        fS0 *= fInvLength;

        fC1 = m_afTuple[0]*fC0 + m_afTuple[1]*fS0;
        fS1 = m_afTuple[2]*fC0 + m_afTuple[3]*fS0;
        fInvLength = CMath<nReal>::InvSqrt(fC1*fC1 + fS1*fS1);
        fC1 *= fInvLength;
        fS1 *= fInvLength;

        if (rkXCon.IsValid(fC0,fS0) && rkYCon.IsValid(fC1,fS1))
        {
            // The maximum occurs at an interior point.
            kQ[0] = fC0*fC1;
            kQ[1] = fS0*fC1;
            kQ[2] = fC0*fS1;
            kQ[3] = fS0*fS1;
        }
        else
        {
            // The maximum occurs at a boundary point.
            CQuaternion kR(rkXCon.m_fCosMinAngle,rkXCon.m_fSinMinAngle,
                (nReal)0,(nReal)0);
            CQuaternion kRInv(rkXCon.m_fCosMinAngle,-rkXCon.m_fSinMinAngle,
                (nReal)0,(nReal)0);
            CQuaternion kProd = kRInv*(*this);
            kTmp = kProd.GetClosest(2,rkYCon);
            nReal fDotOptAngle = kProd.Dot(kTmp);
            kQ = kR*kTmp;

            kR = CQuaternion(rkXCon.m_fCosMaxAngle,rkXCon.m_fSinMaxAngle,
                (nReal)0,(nReal)0);
            kRInv = CQuaternion(rkXCon.m_fCosMaxAngle,-rkXCon.m_fSinMaxAngle,
                (nReal)0,(nReal)0);
            kProd = kRInv*(*this);
            kTmp = kProd.GetClosest(2,rkYCon);
            nReal fDotAngle = kProd.Dot(kTmp);
            if (fDotAngle > fDotOptAngle)
            {
                kQ = kR*kTmp;
                fDotOptAngle = fDotAngle;
            }

            kR = CQuaternion(rkYCon.m_fCosMinAngle,(nReal)0,
                rkYCon.m_fSinMinAngle,(nReal)0);
            kRInv = CQuaternion(rkYCon.m_fCosMinAngle,(nReal)0,
                -rkYCon.m_fSinMinAngle,(nReal)0);
            kProd = (*this)*kRInv;
            kTmp = kProd.GetClosest(1,rkXCon);
            fDotAngle = kProd.Dot(kTmp);
            if (fDotAngle > fDotOptAngle)
            {
                kQ = kTmp*kR;
                fDotOptAngle = fDotAngle;
            }

            kR = CQuaternion(rkYCon.m_fCosMaxAngle,(nReal)0,
                rkYCon.m_fSinMaxAngle,(nReal)0);
            kRInv = CQuaternion(rkYCon.m_fCosMaxAngle,(nReal)0,
                -rkYCon.m_fSinMaxAngle,(nReal)0);
            kProd = (*this)*kRInv;
            kTmp = kProd.GetClosest(1,rkXCon);
            fDotAngle = kProd.Dot(kTmp);
            if (fDotAngle > fDotOptAngle)
            {
                kQ = kTmp*kR;
                fDotOptAngle = fDotAngle;
            }
        }
    }
    else
    {
        // Infinitely many solutions, choose one that satisfies the angle
        // constraints.
        nReal fMinAngle, fMaxAngle, fAngle;
        Constraints kCon;

        if (fDet > (nReal)0)
        {
            fMinAngle = rkXCon.m_fMinAngle - rkYCon.m_fMaxAngle;
            fMaxAngle = rkXCon.m_fMaxAngle - rkYCon.m_fMinAngle;
            kCon.SetAngles(fMinAngle,fMaxAngle);

            kTmp = GetClosest(1,kCon);

            fAngle = CMath<nReal>::ATan2(kTmp[1],kTmp[0]);
            if (fAngle < fMinAngle || fAngle > fMaxAngle)
            {
                fAngle -= 
                    (kTmp[1] >= (nReal)0 ? CMath<nReal>::PI : -CMath<nReal>::PI);
                // assert(fMinAngle <= fAngle && fAngle <= fMaxAngle);
            }

            if (fAngle <= rkXCon.m_fMaxAngle - rkYCon.m_fMaxAngle)
            {
                fC1 = rkYCon.m_fCosMaxAngle;
                fS1 = rkYCon.m_fSinMaxAngle;
                fAngle = rkYCon.m_fMaxAngle + fAngle;
                fC0 = CMath<nReal>::Cos(fAngle);
                fS0 = CMath<nReal>::Sin(fAngle);
            }
            else
            {
                fC0 = rkXCon.m_fCosMaxAngle;
                fS0 = rkXCon.m_fSinMaxAngle;
                fAngle = rkXCon.m_fMaxAngle - fAngle;
                fC1 = CMath<nReal>::Cos(fAngle);
                fS1 = CMath<nReal>::Sin(fAngle);
            }
        }
        else
        {
            fMinAngle = rkXCon.m_fMinAngle + rkYCon.m_fMinAngle;
            fMaxAngle = rkXCon.m_fMaxAngle + rkYCon.m_fMaxAngle;
            kCon.SetAngles(fMinAngle,fMaxAngle);

            kTmp = GetClosest(1,kCon);

            fAngle = CMath<nReal>::ATan2(kTmp[1],kTmp[0]);
            if (fAngle < fMinAngle || fAngle > fMaxAngle)
            {
                fAngle -=
                    (kTmp[1] >= (nReal)0 ? CMath<nReal>::PI : -CMath<nReal>::PI);
                // assert(fMinAngle <= fAngle && fAngle <= fMaxAngle);
            }

            if (fAngle >= rkXCon.m_fMinAngle + rkYCon.m_fMaxAngle)
            {
                fC1 = rkYCon.m_fCosMaxAngle;
                fS1 = rkYCon.m_fSinMaxAngle;
                fAngle = fAngle - rkYCon.m_fMaxAngle;
                fC0 = CMath<nReal>::Cos(fAngle);
                fS0 = CMath<nReal>::Sin(fAngle);
            }
            else
            {
                fC0 = rkXCon.m_fCosMaxAngle;
                fS0 = rkXCon.m_fSinMaxAngle;
                fAngle = fAngle - rkXCon.m_fMaxAngle;
                fC1 = CMath<nReal>::Cos(fAngle);
                fS1 = CMath<nReal>::Sin(fAngle);
            }
        }

        kQ[0] = fC0*fC1;
        kQ[1] = fS0*fC1;
        kQ[2] = fC0*fS1;
        kQ[3] = fS0*fS1;
        if (Dot(kQ) < (nReal)0)
        {
            kQ = -kQ;
        }
    }

    return kQ;
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal> CQuaternion<nReal>::GetClosestYX (const Constraints& rkYCon,
    const Constraints& rkXCon) const
{
    CQuaternion pkAlt(m_afTuple[0],m_afTuple[1],m_afTuple[2],-m_afTuple[3]);
    CQuaternion kQ = pkAlt.GetClosestXY(rkXCon,rkYCon);
    kQ[3] = -kQ[3];
    return kQ;
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal> CQuaternion<nReal>::GetClosestZX (const Constraints& rkZCon,
    const Constraints& rkXCon) const
{
    CQuaternion kQ, kTmp;
    nReal fC2, fS2, fC0, fS0, fInvLength;

    nReal fDet = m_afTuple[0]*m_afTuple[2] - m_afTuple[1]*m_afTuple[3];
    if (CMath<nReal>::FAbs(fDet) < (nReal)0.5 - ms_fTolerance)
    {
        nReal fDiscr = CMath<nReal>::Sqrt(CMath<nReal>::FAbs((nReal)1 -
            ((nReal)4)*fDet*fDet));
        nReal fA = m_afTuple[0]*m_afTuple[3] + m_afTuple[1]*m_afTuple[2];
        nReal fB = m_afTuple[0]*m_afTuple[0] + m_afTuple[1]*m_afTuple[1]
            - m_afTuple[2]*m_afTuple[2] - m_afTuple[3]*m_afTuple[3];

        if (fB >= (nReal)0)
        {
            fC2 = ((nReal)0.5)*(fDiscr + fB);
            fS2 = fA;
        }
        else
        {
            fC2 = fA;
            fS2 = ((nReal)0.5)*(fDiscr - fB);
        }
        fInvLength = CMath<nReal>::InvSqrt(fC2*fC2 + fS2*fS2);
        fC2 *= fInvLength;
        fS2 *= fInvLength;

        fC0 = m_afTuple[0]*fC2 + m_afTuple[3]*fS2;
        fS0 = m_afTuple[1]*fC2 + m_afTuple[2]*fS2;
        fInvLength = CMath<nReal>::InvSqrt(fC0*fC0 + fS0*fS0);
        fC0 *= fInvLength;
        fS0 *= fInvLength;

        if (rkZCon.IsValid(fC2,fS2) && rkXCon.IsValid(fC0,fS0))
        {
            // The maximum occurs at an interior point.
            kQ[0] = fC2*fC0;
            kQ[1] = fC2*fS0;
            kQ[2] = fS2*fS0;
            kQ[3] = fS2*fC0;
        }
        else
        {
            // The maximum occurs at a boundary point.
            CQuaternion kR(rkZCon.m_fCosMinAngle,(nReal)0,(nReal)0,
                rkZCon.m_fSinMinAngle);
            CQuaternion kRInv(rkZCon.m_fCosMinAngle,(nReal)0,(nReal)0,
                -rkZCon.m_fSinMinAngle);
            CQuaternion kProd = kRInv*(*this);
            kTmp = kProd.GetClosest(1,rkXCon);
            nReal fDotOptAngle = kProd.Dot(kTmp);
            kQ = kR*kTmp;

            kR = CQuaternion(rkZCon.m_fCosMaxAngle,(nReal)0,(nReal)0,
                rkZCon.m_fSinMaxAngle);
            kRInv = CQuaternion(rkZCon.m_fCosMaxAngle,(nReal)0,(nReal)0,
                -rkZCon.m_fSinMaxAngle);
            kProd = kRInv*(*this);
            kTmp = kProd.GetClosest(1,rkXCon);
            nReal fDotAngle = kProd.Dot(kTmp);
            if (fDotAngle > fDotOptAngle)
            {
                kQ = kR*kTmp;
                fDotOptAngle = fDotAngle;
            }

            kR = CQuaternion(rkXCon.m_fCosMinAngle,rkXCon.m_fSinMinAngle,
                (nReal)0,(nReal)0);
            kRInv = CQuaternion(rkXCon.m_fCosMinAngle,-rkXCon.m_fSinMinAngle,
                (nReal)0,(nReal)0);
            kProd = (*this)*kRInv;
            kTmp = kProd.GetClosest(3,rkZCon);
            fDotAngle = kProd.Dot(kTmp);
            if (fDotAngle > fDotOptAngle)
            {
                kQ = kTmp*kR;
                fDotOptAngle = fDotAngle;
            }

            kR = CQuaternion(rkXCon.m_fCosMaxAngle,rkXCon.m_fSinMaxAngle,
                (nReal)0,(nReal)0);
            kRInv = CQuaternion(rkXCon.m_fCosMaxAngle,-rkXCon.m_fSinMaxAngle,
                (nReal)0,(nReal)0);
            kProd = (*this)*kRInv;
            kTmp = kProd.GetClosest(3,rkZCon);
            fDotAngle = kProd.Dot(kTmp);
            if (fDotAngle > fDotOptAngle)
            {
                kQ = kTmp*kR;
                fDotOptAngle = fDotAngle;
            }
        }
    }
    else
    {
        // Infinitely many solutions, choose one that satisfies the angle
        // constraints.
        nReal fMinAngle, fMaxAngle, fAngle;
        Constraints kCon;

        if (fDet > (nReal)0)
        {
            fMinAngle = rkXCon.m_fMinAngle - rkZCon.m_fMaxAngle;
            fMaxAngle = rkXCon.m_fMaxAngle - rkZCon.m_fMinAngle;
            kCon.SetAngles(fMinAngle,fMaxAngle);

            kTmp = GetClosest(1,kCon);

            fAngle = CMath<nReal>::ATan2(kTmp[1],kTmp[0]);
            if (fAngle < fMinAngle || fAngle > fMaxAngle)
            {
                fAngle -=
                    (kTmp[1] >= (nReal)0 ? CMath<nReal>::PI : -CMath<nReal>::PI);
                // assert(fMinAngle <= fAngle && fAngle <= fMaxAngle);
            }

            if (fAngle <= rkXCon.m_fMaxAngle - rkZCon.m_fMaxAngle)
            {
                fC2 = rkZCon.m_fCosMaxAngle;
                fS2 = rkZCon.m_fSinMaxAngle;
                fAngle = rkZCon.m_fMaxAngle + fAngle;
                fC0 = CMath<nReal>::Cos(fAngle);
                fS0 = CMath<nReal>::Sin(fAngle);
            }
            else
            {
                fC0 = rkXCon.m_fCosMaxAngle;
                fS0 = rkXCon.m_fSinMaxAngle;
                fAngle = rkXCon.m_fMaxAngle - fAngle;
                fC2 = CMath<nReal>::Cos(fAngle);
                fS2 = CMath<nReal>::Sin(fAngle);
            }
        }
        else
        {
            fMinAngle = rkXCon.m_fMinAngle + rkZCon.m_fMinAngle;
            fMaxAngle = rkXCon.m_fMaxAngle + rkZCon.m_fMaxAngle;
            kCon.SetAngles(fMinAngle,fMaxAngle);

            kTmp = GetClosest(1,kCon);

            fAngle = CMath<nReal>::ATan2(kTmp[1],kTmp[0]);
            if (fAngle < fMinAngle || fAngle > fMaxAngle)
            {
                fAngle -=
                    (kTmp[1] >= (nReal)0 ? CMath<nReal>::PI : -CMath<nReal>::PI);
                // assert(fMinAngle <= fAngle && fAngle <= fMaxAngle);
            }

            if (fAngle >= rkXCon.m_fMinAngle + rkZCon.m_fMaxAngle)
            {
                fC2 = rkZCon.m_fCosMaxAngle;
                fS2 = rkZCon.m_fSinMaxAngle;
                fAngle = fAngle - rkZCon.m_fMaxAngle;
                fC0 = CMath<nReal>::Cos(fAngle);
                fS0 = CMath<nReal>::Sin(fAngle);
            }
            else
            {
                fC0 = rkXCon.m_fCosMaxAngle;
                fS0 = rkXCon.m_fSinMaxAngle;
                fAngle = fAngle - rkXCon.m_fMaxAngle;
                fC2 = CMath<nReal>::Cos(fAngle);
                fS2 = CMath<nReal>::Sin(fAngle);
            }
        }

        kQ[0] = fC2*fC0;
        kQ[1] = fC2*fS0;
        kQ[2] = fS2*fS0;
        kQ[3] = fS2*fC0;
        if (Dot(kQ) < (nReal)0)
        {
            kQ = -kQ;
        }
    }

    return kQ;
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal> CQuaternion<nReal>::GetClosestXZ (const Constraints& rkXCon,
    const Constraints& rkZCon) const
{
    CQuaternion pkAlt(m_afTuple[0],m_afTuple[1],-m_afTuple[2],m_afTuple[3]);
    CQuaternion kQ = pkAlt.GetClosestZX(rkZCon,rkXCon);
    kQ[2] = -kQ[2];
    return kQ;
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal> CQuaternion<nReal>::GetClosestZY (const Constraints& rkZCon,
    const Constraints& rkYCon) const
{
    CQuaternion kQ, kTmp;
    nReal fC2, fS2, fC1, fS1, fInvLength;

    nReal fDet = m_afTuple[0]*m_afTuple[1] + m_afTuple[2]*m_afTuple[3];
    if (CMath<nReal>::FAbs(fDet) < (nReal)0.5 - ms_fTolerance)
    {
        nReal fDiscr = CMath<nReal>::Sqrt(CMath<nReal>::FAbs((nReal)1 -
            ((nReal)4)*fDet*fDet));
        nReal fA = m_afTuple[0]*m_afTuple[3] - m_afTuple[1]*m_afTuple[2];
        nReal fB = m_afTuple[0]*m_afTuple[0] - m_afTuple[1]*m_afTuple[1]
            + m_afTuple[2]*m_afTuple[2] - m_afTuple[3]*m_afTuple[3];

        if (fB >= (nReal)0)
        {
            fC2 = ((nReal)0.5)*(fDiscr + fB);
            fS2 = fA;
        }
        else
        {
            fC2 = fA;
            fS2 = ((nReal)0.5)*(fDiscr - fB);
        }
        fInvLength = CMath<nReal>::InvSqrt(fC2*fC2 + fS2*fS2);
        fC2 *= fInvLength;
        fS2 *= fInvLength;

        fC1 = m_afTuple[0]*fC2 + m_afTuple[3]*fS2;
        fS1 = m_afTuple[2]*fC2 - m_afTuple[1]*fS2;
        fInvLength = CMath<nReal>::InvSqrt(fC1*fC1 + fS1*fS1);
        fC1 *= fInvLength;
        fS1 *= fInvLength;

        if (rkZCon.IsValid(fC2,fS2) && rkYCon.IsValid(fC1,fS1))
        {
            // The maximum occurs at an interior point.
            kQ[0] = fC2*fC1;
            kQ[1] = -fS2*fS1;
            kQ[2] = fC2*fS1;
            kQ[3] = fS2*fC1;
        }
        else
        {
            // The maximum occurs at a boundary point.
            CQuaternion kR(rkZCon.m_fCosMinAngle,(nReal)0,(nReal)0,
                rkZCon.m_fSinMinAngle);
            CQuaternion kRInv(rkZCon.m_fCosMinAngle,(nReal)0,(nReal)0,
                -rkZCon.m_fSinMinAngle);
            CQuaternion kProd = kRInv*(*this);
            kTmp = kProd.GetClosest(2,rkYCon);
            nReal fDotOptAngle = kProd.Dot(kTmp);
            kQ = kR*kTmp;

            kR = CQuaternion(rkZCon.m_fCosMaxAngle,(nReal)0,(nReal)0,
                rkZCon.m_fSinMaxAngle);
            kRInv = CQuaternion(rkZCon.m_fCosMaxAngle,(nReal)0,(nReal)0,
                -rkZCon.m_fSinMaxAngle);
            kProd = kRInv*(*this);
            kTmp = kProd.GetClosest(2,rkYCon);
            nReal fDotAngle = kProd.Dot(kTmp);
            if (fDotAngle > fDotOptAngle)
            {
                kQ = kR*kTmp;
                fDotOptAngle = fDotAngle;
            }

            kR = CQuaternion(rkYCon.m_fCosMinAngle,(nReal)0,
                rkYCon.m_fSinMinAngle,(nReal)0);
            kRInv = CQuaternion(rkYCon.m_fCosMinAngle,(nReal)0,
                -rkYCon.m_fSinMinAngle,(nReal)0);
            kProd = (*this)*kRInv;
            kTmp = kProd.GetClosest(3,rkZCon);
            fDotAngle = kProd.Dot(kTmp);
            if (fDotAngle > fDotOptAngle)
            {
                kQ = kTmp*kR;
                fDotOptAngle = fDotAngle;
            }

            kR = CQuaternion(rkYCon.m_fCosMaxAngle,(nReal)0,
                rkYCon.m_fSinMaxAngle,(nReal)0);
            kRInv = CQuaternion(rkYCon.m_fCosMaxAngle,(nReal)0,
                -rkYCon.m_fSinMaxAngle,(nReal)0);
            kProd = (*this)*kRInv;
            kTmp = kProd.GetClosest(3,rkZCon);
            fDotAngle = kProd.Dot(kTmp);
            if (fDotAngle > fDotOptAngle)
            {
                kQ = kTmp*kR;
                fDotOptAngle = fDotAngle;
            }
        }
    }
    else
    {
        // Infinitely many solutions, choose one that satisfies the angle
        // constraints.
        nReal fMinAngle, fMaxAngle, fAngle;
        Constraints kCon;

        if (fDet < (nReal)0)
        {
            fMinAngle = rkYCon.m_fMinAngle - rkZCon.m_fMaxAngle;
            fMaxAngle = rkYCon.m_fMaxAngle - rkZCon.m_fMinAngle;
            kCon.SetAngles(fMinAngle,fMaxAngle);

            kTmp = GetClosest(2,kCon);

            fAngle = CMath<nReal>::ATan2(kTmp[2],kTmp[0]);
            if (fAngle < fMinAngle || fAngle > fMaxAngle)
            {
                fAngle -=
                    (kTmp[2] >= (nReal)0 ? CMath<nReal>::PI : -CMath<nReal>::PI);
                // assert(fMinAngle <= fAngle && fAngle <= fMaxAngle);
            }

            if (fAngle <= rkYCon.m_fMaxAngle - rkZCon.m_fMaxAngle)
            {
                fC2 = rkZCon.m_fCosMaxAngle;
                fS2 = rkZCon.m_fSinMaxAngle;
                fAngle = rkZCon.m_fMaxAngle + fAngle;
                fC1 = CMath<nReal>::Cos(fAngle);
                fS1 = CMath<nReal>::Sin(fAngle);
            }
            else
            {
                fC1 = rkYCon.m_fCosMaxAngle;
                fS1 = rkYCon.m_fSinMaxAngle;
                fAngle = rkYCon.m_fMaxAngle - fAngle;
                fC2 = CMath<nReal>::Cos(fAngle);
                fS2 = CMath<nReal>::Sin(fAngle);
            }
        }
        else
        {
            fMinAngle = rkYCon.m_fMinAngle + rkZCon.m_fMinAngle;
            fMaxAngle = rkYCon.m_fMaxAngle + rkZCon.m_fMaxAngle;
            kCon.SetAngles(fMinAngle,fMaxAngle);

            kTmp = GetClosest(2,kCon);

            fAngle = CMath<nReal>::ATan2(kTmp[2],kTmp[0]);
            if (fAngle < fMinAngle || fAngle > fMaxAngle)
            {
                fAngle -=
                    (kTmp[2] >= (nReal)0 ? CMath<nReal>::PI : -CMath<nReal>::PI);
                // assert(fMinAngle <= fAngle && fAngle <= fMaxAngle);
            }

            if (fAngle >= rkYCon.m_fMinAngle + rkZCon.m_fMaxAngle)
            {
                fC2 = rkZCon.m_fCosMaxAngle;
                fS2 = rkZCon.m_fSinMaxAngle;
                fAngle = fAngle - rkZCon.m_fMaxAngle;
                fC1 = CMath<nReal>::Cos(fAngle);
                fS1 = CMath<nReal>::Sin(fAngle);
            }
            else
            {
                fC1 = rkYCon.m_fCosMaxAngle;
                fS1 = rkYCon.m_fSinMaxAngle;
                fAngle = fAngle - rkYCon.m_fMaxAngle;
                fC2 = CMath<nReal>::Cos(fAngle);
                fS2 = CMath<nReal>::Sin(fAngle);
            }
        }

        kQ[0] = fC2*fC1;
        kQ[1] = -fS2*fS1;
        kQ[2] = fC2*fS1;
        kQ[3] = fS2*fC1;
        if (Dot(kQ) < (nReal)0)
        {
            kQ = -kQ;
        }
    }

    return kQ;
}
//----------------------------------------------------------------------------
template <class nReal>
CQuaternion<nReal> CQuaternion<nReal>::GetClosestYZ (const Constraints& rkYCon,
    const Constraints& rkZCon) const
{
    CQuaternion pkAlt(m_afTuple[0],-m_afTuple[1],m_afTuple[2],m_afTuple[3]);
    CQuaternion kQ = pkAlt.GetClosestZY(rkZCon,rkYCon);
    kQ[1] = -kQ[1];
    return kQ;
}

typedef CQuaternion<nReal> Quaternionf;


}