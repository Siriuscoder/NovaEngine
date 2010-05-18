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

template <class Real>
class CQuaternion: public CObjectConstructor
{
public:
    // A CQuaternion is q = w + x*i + y*j + z*k where (w,x,y,z) is not
    // necessarily a unit length vector in 4D.

    // construction
    CQuaternion ();  // uninitialized
    CQuaternion (Real fW, Real fX, Real fY, Real fZ);
    CQuaternion (const CQuaternion& rkQ);

    // CQuaternion for the input rotation matrix
    CQuaternion (const CMatrix3<Real>& rkRot);

    // CQuaternion for the rotation of the axis-angle pair
    CQuaternion (const CVector3<Real>& rkAxis, Real fAngle);

    // CQuaternion for the rotation matrix with specified columns
    CQuaternion (const CVector3<Real> akRotColumn[3]);

    // member access:  0 = w, 1 = x, 2 = y, 3 = z
    inline operator const Real* () const;
    inline operator Real* ();
    inline Real operator[] (int i) const;
    inline Real& operator[] (int i);
    inline Real W () const;
    inline Real& W ();
    inline Real X () const;
    inline Real& X ();
    inline Real Y () const;
    inline Real& Y ();
    inline Real Z () const;
    inline Real& Z ();

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
    inline CQuaternion operator* (Real fScalar) const;
    inline CQuaternion operator/ (Real fScalar) const;
    inline CQuaternion operator- () const;

    // arithmetic updates
    inline CQuaternion& operator+= (const CQuaternion& rkQ);
    inline CQuaternion& operator-= (const CQuaternion& rkQ);
    inline CQuaternion& operator*= (Real fScalar);
    inline CQuaternion& operator/= (Real fScalar);

    // conversion between quaternions, matrices, and axis-angle
    CQuaternion& FromRotationMatrix (const CMatrix3<Real>& rkRot);
    void ToRotationMatrix (CMatrix3<Real>& rkRot) const;
    CQuaternion& FromRotationMatrix (const CVector3<Real> akRotColumn[3]);
    void ToRotationMatrix (CVector3<Real> akRotColumn[3]) const;
    CQuaternion& FromAxisAngle (const CVector3<Real>& rkAxis, Real fAngle);
    void ToAxisAngle (CVector3<Real>& rkAxis, Real& rfAngle) const;

    // functions of a CQuaternion
    inline Real Length () const;  // length of 4-tuple
    inline Real SquaredLength () const;  // squared length of 4-tuple
    inline Real Dot (const CQuaternion& rkQ) const;  // dot product of 4-tuples
    inline Real Normalize ();  // make the 4-tuple unit length
    CQuaternion Inverse () const;  // apply to non-zero CQuaternion
    CQuaternion Conjugate () const;
    CQuaternion Exp () const;  // apply to CQuaternion with w = 0
    CQuaternion Log () const;  // apply to unit-length CQuaternion

    // rotation of a vector by a CQuaternion
    CVector3<Real> Rotate (const CVector3<Real>& rkVector) const;

    // spherical linear interpolation
    CQuaternion& Slerp (Real fT, const CQuaternion& rkP, const CQuaternion& rkQ);

    CQuaternion& SlerpExtraSpins (Real fT, const CQuaternion& rkP,
        const CQuaternion& rkQ, int iExtraSpins);

    // intermediate terms for spherical quadratic interpolation
    CQuaternion& Intermediate (const CQuaternion& rkQ0,
        const CQuaternion& rkQ1, const CQuaternion& rkQ2);

    // spherical quadratic interpolation
    CQuaternion& Squad (Real fT, const CQuaternion& rkQ0,
        const CQuaternion& rkA0, const CQuaternion& rkA1,
        const CQuaternion& rkQ1);

    // Compute a CQuaternion that rotates unit-length vector V1 to unit-length
    // vector V2.  The rotation is about the axis perpendicular to both V1 and
    // V2, with angle of that between V1 and V2.  If V1 and V2 are parallel,
    // any axis of rotation will do, such as the permutation (z2,x2,y2), where
    // V2 = (x2,y2,z2).
    CQuaternion& Align (const CVector3<Real>& rkV1, const CVector3<Real>& rkV2);

    // Decompose a CQuaternion into q = q_twist * q_swing, where q is 'this'
    // CQuaternion.  If V1 is the input axis and V2 is the rotation of V1 by
    // q, q_swing represents the rotation about the axis perpendicular to
    // V1 and V2 (see CQuaternion::Align), and q_twist is a rotation about V1.
    void DecomposeTwistTimesSwing (const CVector3<Real>& rkV1,
        CQuaternion& rkTwist, CQuaternion& rkSwing);

    // Decompose a CQuaternion into q = q_swing * q_twist, where q is 'this'
    // CQuaternion.  If V1 is the input axis and V2 is the rotation of V1 by
    // q, q_swing represents the rotation about the axis perpendicular to
    // V1 and V2 (see CQuaternion::Align), and q_twist is a rotation about V1.
    void DecomposeSwingTimesTwist (const CVector3<Real>& rkV1,
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
    void FactorXYZ (Real& rfCx, Real& rfSx, Real& rfCy, Real& rfSy,
        Real& rfCz, Real& rfSz);

    // Factor to (cx + sx*i)*(cz + sz*k)*(cy + sy*j).
    void FactorXZY (Real& rfCx, Real& rfSx, Real& rfCz, Real& rfSz,
        Real& rfCy, Real& rfSy);

    // Factor to (cy + sy*j)*(cz + sz*k)*(cx + sx*i).
    void FactorYZX (Real& rfCy, Real& rfSy, Real& rfCz, Real& rfSz,
        Real& rfCx, Real& rfSx);

    // Factor to (cy + sy*j)*(cx + sx*i)*(cz + sz*k).
    void FactorYXZ (Real& rfCy, Real& rfSy, Real& rfCx, Real& rfSx,
        Real& rfCz, Real& rfSz);

    // Factor to (cz + sz*k)*(cx + sx*i)*(cy + sy*j).
    void FactorZXY (Real& rfCz, Real& rfSz, Real& rfCx, Real& rfSx,
        Real& rfCy, Real& rfSy);

    // Factor to (cz + sz*k)*(cy + sy*j)*(cx + sx*i).
    void FactorZYX (Real& rfCz, Real& rfSz, Real& rfCy, Real& rfSy,
        Real& rfCx, Real& rfSx);

    // *** Find closest quaternions with constrained angles.
    class Constraints
    {
    public:
        Constraints ()
        {
            // Members are uninitialized.
        }

        Constraints (Real fMinAngle, Real fMaxAngle)
        {
            SetAngles(fMinAngle,fMaxAngle);
        }

        void SetAngles (Real fMinAngle, Real fMaxAngle)
        {
            m_fMinAngle = fMinAngle;
            m_fMaxAngle = fMaxAngle;
            m_fCosMinAngle = CMath<Real>::Cos(m_fMinAngle);
            m_fSinMinAngle = CMath<Real>::Sin(m_fMinAngle);
            m_fCosMaxAngle = CMath<Real>::Cos(m_fMaxAngle);
            m_fSinMaxAngle = CMath<Real>::Sin(m_fMaxAngle);
            m_fDiffCosMaxMin = m_fCosMaxAngle - m_fCosMinAngle;
            m_fDiffSinMaxMin = m_fSinMaxAngle - m_fSinMinAngle;
            Real fAvrAngle = ((Real)0.5)*(m_fMinAngle + m_fMaxAngle);
            m_fCosAvrAngle = CMath<Real>::Cos(fAvrAngle);
            m_fSinAvrAngle = CMath<Real>::Sin(fAvrAngle);
        }

        bool IsValid (Real fX, Real fY) const
        {
            // (x,y) must be unit-length.

            // Test whether (x,y) satisfies the constraints.
            Real fXm = fX - m_fCosMinAngle;
            Real fYm = fY - m_fSinMinAngle;
            if (fXm*m_fDiffSinMaxMin >= fYm*m_fDiffCosMaxMin)
            {
                return true;
            }

            // Test whether (-x,-y) satisfies the constraints.
            Real fXp = fX + m_fCosMinAngle;
            Real fYp = fY + m_fSinMinAngle;
            if (fXp*m_fDiffSinMaxMin <= fYp*m_fDiffCosMaxMin)
            {
                return true;
            }

            return false;
        }

        Real m_fMinAngle;       // in [-PI/2,PI/2]
        Real m_fMaxAngle;       // in [m_fMinAngle/2,PI/2]
        Real m_fCosMinAngle;    // = cos(m_fMinAngle)
        Real m_fSinMinAngle;    // = sin(m_fMinAngle)
        Real m_fCosMaxAngle;    // = cos(m_fMaxAngle)
        Real m_fSinMaxAngle;    // = sin(m_fMaxAngle)
        Real m_fDiffCosMaxMin;  // = cos(m_fMaxAngle) - cos(m_fMinAngle)
        Real m_fDiffSinMaxMin;  // = sin(m_fMaxAngle) - sin(m_fMinAngle)
        Real m_fCosAvrAngle;    // = cos((m_fMinAngle + m_fMaxAngle)/2)
        Real m_fSinAvrAngle;    // = sin((m_fMinAngle + mM_faxAngle)/2)
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
    int CompareArrays (const CQuaternion& rkQ) const;

    // Closest unconstrained CQuaternion of the form:
    //   (cx + sx*i) when iAxis = 1,
    //   (cy + sy*j) when iAxis = 2,
    //   (cz + sz*k) when iAxis = 3
    CQuaternion GetClosest (int iAxis) const;

    // Closest constrained CQuaternion of the form:
    //   (cx + sx*i) when iAxis = 1,
    //   (cy + sy*j) when iAxis = 2,
    //   (cz + sz*k) when iAxis = 3
    CQuaternion GetClosest (int iAxis, const Constraints& rkCon) const;

    // support for FromRotationMatrix
    NOVA_EXPORT static int ms_iNext[3];

    // support for closest quaternions
    NOVA_EXPORT static Real ms_fTolerance;
    NOVA_EXPORT static Real ms_fRootTwo;
    NOVA_EXPORT static Real ms_fRootHalf;

    Real m_afTuple[4];
};

template <class Real>
inline CQuaternion<Real> operator* (Real fScalar, const CQuaternion<Real>& rkQ);

template <class Real>
CQuaternion<Real>::CQuaternion ()
{
    // uninitialized for performance in array construction
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real>::CQuaternion (Real fW, Real fX, Real fY, Real fZ)
{
    m_afTuple[0] = fW;
    m_afTuple[1] = fX;
    m_afTuple[2] = fY;
    m_afTuple[3] = fZ;
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real>::CQuaternion (const CQuaternion& rkQ)
{
    m_afTuple[0] = rkQ.m_afTuple[0];
    m_afTuple[1] = rkQ.m_afTuple[1];
    m_afTuple[2] = rkQ.m_afTuple[2];
    m_afTuple[3] = rkQ.m_afTuple[3];
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real>::CQuaternion (const CMatrix3<Real>& rkRot)
{
    FromRotationMatrix(rkRot);
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real>::CQuaternion (const CVector3<Real>& rkAxis, Real fAngle)
{
    FromAxisAngle(rkAxis,fAngle);
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real>::CQuaternion (const CVector3<Real> akRotColumn[3])
{
    FromRotationMatrix(akRotColumn);
}
//----------------------------------------------------------------------------
template <class Real>
inline CQuaternion<Real>::operator const Real* () const
{
    return m_afTuple;
}
//----------------------------------------------------------------------------
template <class Real>
inline CQuaternion<Real>::operator Real* ()
{
    return m_afTuple;
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CQuaternion<Real>::operator[] (int i) const
{
    return m_afTuple[i];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real& CQuaternion<Real>::operator[] (int i)
{
    return m_afTuple[i];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CQuaternion<Real>::W () const
{
    return m_afTuple[0];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real& CQuaternion<Real>::W ()
{
    return m_afTuple[0];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CQuaternion<Real>::X () const
{
    return m_afTuple[1];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real& CQuaternion<Real>::X ()
{
    return m_afTuple[1];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CQuaternion<Real>::Y () const
{
    return m_afTuple[2];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real& CQuaternion<Real>::Y ()
{
    return m_afTuple[2];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CQuaternion<Real>::Z () const
{
    return m_afTuple[3];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real& CQuaternion<Real>::Z ()
{
    return m_afTuple[3];
}
//----------------------------------------------------------------------------
template <class Real>
inline CQuaternion<Real>& CQuaternion<Real>::operator= (const CQuaternion& rkQ)
{
    m_afTuple[0] = rkQ.m_afTuple[0];
    m_afTuple[1] = rkQ.m_afTuple[1];
    m_afTuple[2] = rkQ.m_afTuple[2];
    m_afTuple[3] = rkQ.m_afTuple[3];
    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
int CQuaternion<Real>::CompareArrays (const CQuaternion& rkQ) const
{
    return memcmp(m_afTuple,rkQ.m_afTuple,4*sizeof(Real));
}
//----------------------------------------------------------------------------
template <class Real>
bool CQuaternion<Real>::operator== (const CQuaternion& rkQ) const
{
    return CompareArrays(rkQ) == 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CQuaternion<Real>::operator!= (const CQuaternion& rkQ) const
{
    return CompareArrays(rkQ) != 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CQuaternion<Real>::operator< (const CQuaternion& rkQ) const
{
    return CompareArrays(rkQ) < 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CQuaternion<Real>::operator<= (const CQuaternion& rkQ) const
{
    return CompareArrays(rkQ) <= 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CQuaternion<Real>::operator> (const CQuaternion& rkQ) const
{
    return CompareArrays(rkQ) > 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CQuaternion<Real>::operator>= (const CQuaternion& rkQ) const
{
    return CompareArrays(rkQ) >= 0;
}
//----------------------------------------------------------------------------
template <class Real>
inline CQuaternion<Real> CQuaternion<Real>::operator+ (
    const CQuaternion& rkQ) const
{
    CQuaternion kSum;
    for (int i = 0; i < 4; i++)
    {
        kSum.m_afTuple[i] = m_afTuple[i] + rkQ.m_afTuple[i];
    }
    return kSum;
}
//----------------------------------------------------------------------------
template <class Real>
inline CQuaternion<Real> CQuaternion<Real>::operator- (
    const CQuaternion& rkQ) const
{
    CQuaternion kDiff;
    for (int i = 0; i < 4; i++)
    {
        kDiff.m_afTuple[i] = m_afTuple[i] - rkQ.m_afTuple[i];
    }
    return kDiff;
}
//----------------------------------------------------------------------------
template <class Real>
inline CQuaternion<Real> CQuaternion<Real>::operator* (
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
template <class Real>
inline CQuaternion<Real> CQuaternion<Real>::operator* (Real fScalar) const
{
    CQuaternion kProd;
    for (int i = 0; i < 4; i++)
    {
        kProd.m_afTuple[i] = fScalar*m_afTuple[i];
    }
    return kProd;
}
//----------------------------------------------------------------------------
template <class Real>
inline CQuaternion<Real> CQuaternion<Real>::operator/ (Real fScalar) const
{
    CQuaternion kQuot;
    int i;

    if (fScalar != (Real)0.0)
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        for (i = 0; i < 4; i++)
        {
            kQuot.m_afTuple[i] = fInvScalar*m_afTuple[i];
        }
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            kQuot.m_afTuple[i] = CMath<Real>::MAX_REAL;
        }
    }

    return kQuot;
}
//----------------------------------------------------------------------------
template <class Real>
inline CQuaternion<Real> CQuaternion<Real>::operator- () const
{
    CQuaternion kNeg;
    for (int i = 0; i < 4; i++)
    {
        kNeg.m_afTuple[i] = -m_afTuple[i];
    }
    return kNeg;
}
//----------------------------------------------------------------------------
template <class Real>
inline CQuaternion<Real> operator* (Real fScalar, const CQuaternion<Real>& rkQ)
{
    CQuaternion<Real> kProd;
    for (int i = 0; i < 4; i++)
    {
        kProd[i] = fScalar*rkQ[i];
    }
    return kProd;
}
//----------------------------------------------------------------------------
template <class Real>
inline CQuaternion<Real>& CQuaternion<Real>::operator+= (const CQuaternion& rkQ)
{
    for (int i = 0; i < 4; i++)
    {
        m_afTuple[i] += rkQ.m_afTuple[i];
    }
    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
inline CQuaternion<Real>& CQuaternion<Real>::operator-= (const CQuaternion& rkQ)
{
    for (int i = 0; i < 4; i++)
    {
        m_afTuple[i] -= rkQ.m_afTuple[i];
    }
    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
inline CQuaternion<Real>& CQuaternion<Real>::operator*= (Real fScalar)
{
    for (int i = 0; i < 4; i++)
    {
        m_afTuple[i] *= fScalar;
    }
    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
inline CQuaternion<Real>& CQuaternion<Real>::operator/= (Real fScalar)
{
    int i;

    if (fScalar != (Real)0.0)
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        for (i = 0; i < 4; i++)
        {
            m_afTuple[i] *= fInvScalar;
        }
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            m_afTuple[i] = CMath<Real>::MAX_REAL;
        }
    }

    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real>& CQuaternion<Real>::FromRotationMatrix (
    const CMatrix3<Real>& rkRot)
{
    // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
    // article "CQuaternion Calculus and Fast Animation".

    Real fTrace = rkRot(0,0) + rkRot(1,1) + rkRot(2,2);
    Real fRoot;

    if (fTrace > (Real)0.0)
    {
        // |w| > 1/2, may as well choose w > 1/2
        fRoot = CMath<Real>::Sqrt(fTrace + (Real)1.0);  // 2w
        m_afTuple[0] = ((Real)0.5)*fRoot;
        fRoot = ((Real)0.5)/fRoot;  // 1/(4w)
        m_afTuple[1] = (rkRot(2,1)-rkRot(1,2))*fRoot;
        m_afTuple[2] = (rkRot(0,2)-rkRot(2,0))*fRoot;
        m_afTuple[3] = (rkRot(1,0)-rkRot(0,1))*fRoot;
    }
    else
    {
        // |w| <= 1/2
        int i = 0;
        if ( rkRot(1,1) > rkRot(0,0) )
        {
            i = 1;
        }
        if ( rkRot(2,2) > rkRot(i,i) )
        {
            i = 2;
        }
        int j = ms_iNext[i];
        int k = ms_iNext[j];

        fRoot = CMath<Real>::Sqrt(rkRot(i,i)-rkRot(j,j)-rkRot(k,k)+(Real)1.0);
        Real* apfQuat[3] = { &m_afTuple[1], &m_afTuple[2], &m_afTuple[3] };
        *apfQuat[i] = ((Real)0.5)*fRoot;
        fRoot = ((Real)0.5)/fRoot;
        m_afTuple[0] = (rkRot(k,j)-rkRot(j,k))*fRoot;
        *apfQuat[j] = (rkRot(j,i)+rkRot(i,j))*fRoot;
        *apfQuat[k] = (rkRot(k,i)+rkRot(i,k))*fRoot;
    }

    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
void CQuaternion<Real>::ToRotationMatrix (CMatrix3<Real>& rkRot) const
{
    Real fTx  = ((Real)2.0)*m_afTuple[1];
    Real fTy  = ((Real)2.0)*m_afTuple[2];
    Real fTz  = ((Real)2.0)*m_afTuple[3];
    Real fTwx = fTx*m_afTuple[0];
    Real fTwy = fTy*m_afTuple[0];
    Real fTwz = fTz*m_afTuple[0];
    Real fTxx = fTx*m_afTuple[1];
    Real fTxy = fTy*m_afTuple[1];
    Real fTxz = fTz*m_afTuple[1];
    Real fTyy = fTy*m_afTuple[2];
    Real fTyz = fTz*m_afTuple[2];
    Real fTzz = fTz*m_afTuple[3];

    rkRot(0,0) = (Real)1.0-(fTyy+fTzz);
    rkRot(0,1) = fTxy-fTwz;
    rkRot(0,2) = fTxz+fTwy;
    rkRot(1,0) = fTxy+fTwz;
    rkRot(1,1) = (Real)1.0-(fTxx+fTzz);
    rkRot(1,2) = fTyz-fTwx;
    rkRot(2,0) = fTxz-fTwy;
    rkRot(2,1) = fTyz+fTwx;
    rkRot(2,2) = (Real)1.0-(fTxx+fTyy);
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real>& CQuaternion<Real>::FromRotationMatrix (
    const CVector3<Real> akRotColumn[3])
{
    CMatrix3<Real> kRot;
    for (int iCol = 0; iCol < 3; iCol++)
    {
        kRot(0,iCol) = akRotColumn[iCol][0];
        kRot(1,iCol) = akRotColumn[iCol][1];
        kRot(2,iCol) = akRotColumn[iCol][2];
    }
    return FromRotationMatrix(kRot);
}
//----------------------------------------------------------------------------
template <class Real>
void CQuaternion<Real>::ToRotationMatrix (CVector3<Real> akRotColumn[3]) const
{
    CMatrix3<Real> kRot;
    ToRotationMatrix(kRot);
    for (int iCol = 0; iCol < 3; iCol++)
    {
        akRotColumn[iCol][0] = kRot(0,iCol);
        akRotColumn[iCol][1] = kRot(1,iCol);
        akRotColumn[iCol][2] = kRot(2,iCol);
    }
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real>& CQuaternion<Real>::FromAxisAngle (
    const CVector3<Real>& rkAxis, Real fAngle)
{
    // assert:  axis[] is unit length
    //
    // The CQuaternion representing the rotation is
    //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

    Real fHalfAngle = ((Real)0.5)*fAngle;
    Real fSin = CMath<Real>::Sin(fHalfAngle);
    m_afTuple[0] = CMath<Real>::Cos(fHalfAngle);
    m_afTuple[1] = fSin*rkAxis[0];
    m_afTuple[2] = fSin*rkAxis[1];
    m_afTuple[3] = fSin*rkAxis[2];

    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
void CQuaternion<Real>::ToAxisAngle (CVector3<Real>& rkAxis, Real& rfAngle)
    const
{
    // The CQuaternion representing the rotation is
    //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

    Real fSqrLength = m_afTuple[1]*m_afTuple[1] + m_afTuple[2]*m_afTuple[2]
        + m_afTuple[3]*m_afTuple[3];

    if (fSqrLength > CMath<Real>::ZERO_TOLERANCE)
    {
        rfAngle = ((Real)2.0)*CMath<Real>::ACos(m_afTuple[0]);
        Real fInvLength = CMath<Real>::InvSqrt(fSqrLength);
        rkAxis[0] = m_afTuple[1]*fInvLength;
        rkAxis[1] = m_afTuple[2]*fInvLength;
        rkAxis[2] = m_afTuple[3]*fInvLength;
    }
    else
    {
        // angle is 0 (mod 2*pi), so any axis will do
        rfAngle = (Real)0.0;
        rkAxis[0] = (Real)1.0;
        rkAxis[1] = (Real)0.0;
        rkAxis[2] = (Real)0.0;
    }
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CQuaternion<Real>::Length () const
{
    return CMath<Real>::Sqrt(
        m_afTuple[0]*m_afTuple[0] +
        m_afTuple[1]*m_afTuple[1] +
        m_afTuple[2]*m_afTuple[2] +
        m_afTuple[3]*m_afTuple[3]);
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CQuaternion<Real>::SquaredLength () const
{
    return
        m_afTuple[0]*m_afTuple[0] +
        m_afTuple[1]*m_afTuple[1] +
        m_afTuple[2]*m_afTuple[2] +
        m_afTuple[3]*m_afTuple[3];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CQuaternion<Real>::Dot (const CQuaternion& rkQ) const
{
    Real fDot = (Real)0.0;
    for (int i = 0; i < 4; i++)
    {
        fDot += m_afTuple[i]*rkQ.m_afTuple[i];
    }
    return fDot;
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CQuaternion<Real>::Normalize ()
{
    Real fLength = Length();

    if (fLength > CMath<Real>::ZERO_TOLERANCE)
    {
        Real fInvLength = ((Real)1.0)/fLength;
        m_afTuple[0] *= fInvLength;
        m_afTuple[1] *= fInvLength;
        m_afTuple[2] *= fInvLength;
        m_afTuple[3] *= fInvLength;
    }
    else
    {
        fLength = (Real)0.0;
        m_afTuple[0] = (Real)0.0;
        m_afTuple[1] = (Real)0.0;
        m_afTuple[2] = (Real)0.0;
        m_afTuple[3] = (Real)0.0;
    }

    return fLength;
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real> CQuaternion<Real>::Inverse () const
{
    CQuaternion kInverse;

    Real fNorm = (Real)0.0;
    int i;
    for (i = 0; i < 4; i++)
    {
        fNorm += m_afTuple[i]*m_afTuple[i];
    }

    if (fNorm > (Real)0.0)
    {
        Real fInvNorm = ((Real)1.0)/fNorm;
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
            kInverse.m_afTuple[i] = (Real)0.0;
        }
    }

    return kInverse;
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real> CQuaternion<Real>::Conjugate () const
{
    return CQuaternion(m_afTuple[0],-m_afTuple[1],-m_afTuple[2],
        -m_afTuple[3]);
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real> CQuaternion<Real>::Exp () const
{
    // If q = A*(x*i+y*j+z*k) where (x,y,z) is unit length, then
    // exp(q) = cos(A)+sin(A)*(x*i+y*j+z*k).  If sin(A) is near zero,
    // use exp(q) = cos(A)+A*(x*i+y*j+z*k) since A/sin(A) has limit 1.

    CQuaternion kResult;

    Real fAngle = CMath<Real>::Sqrt(m_afTuple[1]*m_afTuple[1] +
        m_afTuple[2]*m_afTuple[2] + m_afTuple[3]*m_afTuple[3]);

    Real fSin = CMath<Real>::Sin(fAngle);
    kResult.m_afTuple[0] = CMath<Real>::Cos(fAngle);

    int i;

    if (CMath<Real>::FAbs(fSin) >= CMath<Real>::ZERO_TOLERANCE)
    {
        Real fCoeff = fSin/fAngle;
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
template <class Real>
CQuaternion<Real> CQuaternion<Real>::Log () const
{
    // If q = cos(A)+sin(A)*(x*i+y*j+z*k) where (x,y,z) is unit length, then
    // log(q) = A*(x*i+y*j+z*k).  If sin(A) is near zero, use log(q) =
    // sin(A)*(x*i+y*j+z*k) since sin(A)/A has limit 1.

    CQuaternion kResult;
    kResult.m_afTuple[0] = (Real)0.0;

    int i;

    if (CMath<Real>::FAbs(m_afTuple[0]) < (Real)1.0)
    {
        Real fAngle = CMath<Real>::ACos(m_afTuple[0]);
        Real fSin = CMath<Real>::Sin(fAngle);
        if (CMath<Real>::FAbs(fSin) >= CMath<Real>::ZERO_TOLERANCE)
        {
            Real fCoeff = fAngle/fSin;
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
template <class Real>
CVector3<Real> CQuaternion<Real>::Rotate (const CVector3<Real>& rkVector)
    const
{
    // Given a vector u = (x0,y0,z0) and a unit length CQuaternion
    // q = <w,x,y,z>, the vector v = (x1,y1,z1) which represents the
    // rotation of u by q is v = q*u*q^{-1} where * indicates CQuaternion
    // multiplication and where u is treated as the CQuaternion <0,x0,y0,z0>.
    // Note that q^{-1} = <w,-x,-y,-z>, so no real work is required to
    // invert q.  Now
    //
    //   q*u*q^{-1} = q*<0,x0,y0,z0>*q^{-1}
    //     = q*(x0*i+y0*j+z0*k)*q^{-1}
    //     = x0*(q*i*q^{-1})+y0*(q*j*q^{-1})+z0*(q*k*q^{-1})
    //
    // As 3-vectors, q*i*q^{-1}, q*j*q^{-1}, and 2*k*q^{-1} are the columns
    // of the rotation matrix computed in CQuaternion<Real>::ToRotationMatrix.
    // The vector v is obtained as the product of that rotation matrix with
    // vector u.  As such, the CQuaternion representation of a rotation
    // matrix requires less space than the matrix and more time to compute
    // the rotated vector.  Typical space-time tradeoff...

    CMatrix3<Real> kRot;
    ToRotationMatrix(kRot);
    return kRot*rkVector;
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real>& CQuaternion<Real>::Slerp (Real fT, const CQuaternion& rkP,
    const CQuaternion& rkQ)
{
    Real fCos = rkP.Dot(rkQ);
    Real fAngle = CMath<Real>::ACos(fCos);

    if (CMath<Real>::FAbs(fAngle) >= CMath<Real>::ZERO_TOLERANCE)
    {
        Real fSin = CMath<Real>::Sin(fAngle);
        Real fInvSin = ((Real)1.0)/fSin;
        Real fTAngle = fT*fAngle;
        Real fCoeff0 = CMath<Real>::Sin(fAngle - fTAngle)*fInvSin;
        Real fCoeff1 = CMath<Real>::Sin(fTAngle)*fInvSin;

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
template <class Real>
CQuaternion<Real>& CQuaternion<Real>::SlerpExtraSpins (Real fT,
    const CQuaternion& rkP, const CQuaternion& rkQ, int iExtraSpins)
{
    Real fCos = rkP.Dot(rkQ);
    Real fAngle = CMath<Real>::ACos(fCos);

    if (CMath<Real>::FAbs(fAngle) >= CMath<Real>::ZERO_TOLERANCE)
    {
        Real fSin = CMath<Real>::Sin(fAngle);
        Real fPhase = CMath<Real>::PI*iExtraSpins*fT;
        Real fInvSin = ((Real)1.0)/fSin;
        Real fCoeff0 = CMath<Real>::Sin(((Real)1.0-fT)*fAngle-fPhase)*fInvSin;
        Real fCoeff1 = CMath<Real>::Sin(fT*fAngle + fPhase)*fInvSin;
        *this = fCoeff0*rkP + fCoeff1*rkQ;
    }
    else
    {
        *this = rkP;
    }

    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real>& CQuaternion<Real>::Intermediate (const CQuaternion& rkQ0,
    const CQuaternion& rkQ1, const CQuaternion& rkQ2)
{
    // assert:  Q0, Q1, Q2 all unit-length
    CQuaternion kQ1Inv = rkQ1.Conjugate();
    CQuaternion kP0 = kQ1Inv*rkQ0;
    CQuaternion kP2 = kQ1Inv*rkQ2;
    CQuaternion kArg = -((Real)0.25)*(kP0.Log()+kP2.Log());
    CQuaternion kA = rkQ1*kArg.Exp();
    *this = kA;

    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real>& CQuaternion<Real>::Squad (Real fT, const CQuaternion& rkQ0,
    const CQuaternion& rkA0, const CQuaternion& rkA1, const CQuaternion& rkQ1)
{
    Real fSlerpT = ((Real)2.0)*fT*((Real)1.0-fT);
    CQuaternion kSlerpP = Slerp(fT,rkQ0,rkQ1);
    CQuaternion kSlerpQ = Slerp(fT,rkA0,rkA1);
    return Slerp(fSlerpT,kSlerpP,kSlerpQ);
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real>& CQuaternion<Real>::Align (const CVector3<Real>& rkV1,
    const CVector3<Real>& rkV2)
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

    CVector3<Real> kBisector = rkV1 + rkV2;
    kBisector.Normalize();

    Real fCosHalfAngle = rkV1.Dot(kBisector);
    CVector3<Real> kCross;

    m_afTuple[0] = fCosHalfAngle;

    if (fCosHalfAngle != (Real)0.0)
    {
        kCross = rkV1.Cross(kBisector);
        m_afTuple[1] = kCross.X();
        m_afTuple[2] = kCross.Y();
        m_afTuple[3] = kCross.Z();
    }
    else
    {
        Real fInvLength;
        if (CMath<Real>::FAbs(rkV1[0]) >= CMath<Real>::FAbs(rkV1[1]))
        {
            // V1.x or V1.z is the largest magnitude component
            fInvLength = CMath<Real>::InvSqrt(rkV1[0]*rkV1[0] +
                rkV1[2]*rkV1[2]);

            m_afTuple[1] = -rkV1[2]*fInvLength;
            m_afTuple[2] = (Real)0.0;
            m_afTuple[3] = +rkV1[0]*fInvLength;
        }
        else
        {
            // V1.y or V1.z is the largest magnitude component
            fInvLength = CMath<Real>::InvSqrt(rkV1[1]*rkV1[1] +
                rkV1[2]*rkV1[2]);

            m_afTuple[1] = (Real)0.0;
            m_afTuple[2] = +rkV1[2]*fInvLength;
            m_afTuple[3] = -rkV1[1]*fInvLength;
        }
    }

    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
void CQuaternion<Real>::DecomposeTwistTimesSwing (
    const CVector3<Real>& rkV1, CQuaternion& rkTwist, CQuaternion& rkSwing)
{
    CVector3<Real> kV2 = Rotate(rkV1);
    rkSwing = Align(rkV1,kV2);
    rkTwist = (*this)*rkSwing.Conjugate();
}
//----------------------------------------------------------------------------
template <class Real>
void CQuaternion<Real>::DecomposeSwingTimesTwist (
    const CVector3<Real>& rkV1, CQuaternion& rkSwing, CQuaternion& rkTwist)
{
    CVector3<Real> kV2 = Rotate(rkV1);
    rkSwing = Align(rkV1,kV2);
    rkTwist = rkSwing.Conjugate()*(*this);
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real> CQuaternion<Real>::GetClosest (int iAxis) const
{
    // The appropriate nonzero components will be set later.
    CQuaternion kQ((Real)0,(Real)0,(Real)0,(Real)0);
    Real fP0 = m_afTuple[0];
    Real fP1 = m_afTuple[iAxis];
    Real fSqrLength = fP0*fP0 + fP1*fP1;
    if (fSqrLength > ms_fTolerance)
    {
        // A unique closest point.
        Real fInvLength = CMath<Real>::InvSqrt(fSqrLength);
        kQ[0] = fP0*fInvLength;
        kQ[iAxis] = fP1*fInvLength;
    }
    else
    {
        // Infinitely many solutions, choose the one for theta = 0.
        kQ[0] = (Real)1;
        kQ[iAxis] = (Real)0;
    }
    return kQ;
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real> CQuaternion<Real>::GetClosestX () const
{
    return GetClosest(1);
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real> CQuaternion<Real>::GetClosestY () const
{
    return GetClosest(2);
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real> CQuaternion<Real>::GetClosestZ () const
{
    return GetClosest(3);
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real> CQuaternion<Real>::GetClosestXY () const
{
    CQuaternion kQ;

    Real fDet = m_afTuple[0]*m_afTuple[3] - m_afTuple[1]*m_afTuple[2];
    if(CMath<Real>::FAbs(fDet) < (Real)0.5 - ms_fTolerance)
    {
        Real fDiscr = (Real)1 - ((Real)4)*fDet*fDet;
        fDiscr = CMath<Real>::Sqrt(CMath<Real>::FAbs(fDiscr));
        Real fA = m_afTuple[0]*m_afTuple[1] + m_afTuple[2]*m_afTuple[3];
        Real fB = m_afTuple[0]*m_afTuple[0] - m_afTuple[1]*m_afTuple[1] +
            m_afTuple[2]*m_afTuple[2] - m_afTuple[3]*m_afTuple[3];

        Real fC0, fS0, fC1, fS1, fInvLength;

        if (fB >= (Real)0)
        {
            fC0 = ((Real)0.5)*(fDiscr + fB);
            fS0 = fA;
        }
        else
        {
            fC0 = fA;
            fS0 = ((Real)0.5)*(fDiscr - fB);
        }
        fInvLength = CMath<Real>::InvSqrt(fC0*fC0 + fS0*fS0);
        fC0 *= fInvLength;
        fS0 *= fInvLength;

        fC1 = m_afTuple[0]*fC0 + m_afTuple[1]*fS0;
        fS1 = m_afTuple[2]*fC0 + m_afTuple[3]*fS0;
        fInvLength = CMath<Real>::InvSqrt(fC1*fC1 + fS1*fS1);
        fC1 *= fInvLength;
        fS1 *= fInvLength;

        kQ[0] = fC0*fC1;
        kQ[1] = fS0*fC1;
        kQ[2] = fC0*fS1;
        kQ[3] = fS0*fS1;
    }
    else
    {
        Real fInvLength = CMath<Real>::InvSqrt(CMath<Real>::FAbs(fDet));
        kQ[0] = m_afTuple[0]*fInvLength;
        kQ[1] = m_afTuple[1]*fInvLength;
        kQ[2] = (Real)0;
        kQ[3] = (Real)0;
    }

    return kQ;
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real> CQuaternion<Real>::GetClosestYX () const
{
    CQuaternion kAlt(m_afTuple[0],m_afTuple[1],m_afTuple[2],-m_afTuple[3]);
    CQuaternion kQ = kAlt.GetClosestXY();
    kQ[3] = -kQ[3];
    return kQ;
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real> CQuaternion<Real>::GetClosestZX () const
{
    CQuaternion kAlt(m_afTuple[0],m_afTuple[1],m_afTuple[3],m_afTuple[2]);
    CQuaternion kQ = kAlt.GetClosestXY();
    Real fSave = kQ[2];
    kQ[2] = kQ[3];
    kQ[3] = fSave;
    return kQ;
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real> CQuaternion<Real>::GetClosestXZ () const
{
    CQuaternion kAlt(m_afTuple[0],m_afTuple[1],-m_afTuple[3],m_afTuple[2]);
    CQuaternion kQ = kAlt.GetClosestXY();
    Real fSave = kQ[2];
    kQ[2] = kQ[3];
    kQ[3] = -fSave;
    return kQ;
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real> CQuaternion<Real>::GetClosestYZ () const
{
    CQuaternion kAlt(m_afTuple[0],m_afTuple[2],m_afTuple[3],m_afTuple[1]);
    CQuaternion kQ = kAlt.GetClosestXY();
    Real fSave = kQ[3];
    kQ[3] = kQ[2];
    kQ[2] = kQ[1];
    kQ[1] = fSave;
    return kQ;
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real> CQuaternion<Real>::GetClosestZY () const
{
    CQuaternion kAlt(m_afTuple[0],m_afTuple[2],m_afTuple[3],-m_afTuple[1]);
    CQuaternion kQ = kAlt.GetClosestXY();
    Real fSave = kQ[3];
    kQ[3] = kQ[2];
    kQ[2] = kQ[1];
    kQ[1] = -fSave;
    return kQ;
}
//----------------------------------------------------------------------------
template <class Real>
void CQuaternion<Real>::FactorXYZ (Real& rfCx, Real& rfSx, Real& rfCy,
    Real& rfSy, Real& rfCz, Real& rfSz)
{
    Real fA = m_afTuple[0]*m_afTuple[1] - m_afTuple[2]*m_afTuple[3];
    Real fB = ((Real)0.5)*(
          m_afTuple[0]*m_afTuple[0]
        - m_afTuple[1]*m_afTuple[1]
        - m_afTuple[2]*m_afTuple[2]
        + m_afTuple[3]*m_afTuple[3]);

    Real fLength = CMath<Real>::Sqrt(fA*fA + fB*fB);
    if (fLength > ms_fTolerance)
    {
        Real fInvLength = ((Real)1)/fLength;
        Real fSigma0 = fA * fInvLength;
        Real fGamma0 = fB * fInvLength;
        if (fGamma0 >= (Real)0)
        {
            rfCx = CMath<Real>::Sqrt(((Real)0.5)*((Real)1 + fGamma0));
            rfSx = ((Real)0.5)*fSigma0/rfCx;
        }
        else
        {
            rfSx = CMath<Real>::Sqrt(((Real)0.5)*((Real)1 - fGamma0));
            rfCx = ((Real)0.5)*fSigma0/rfSx;
        }

        Real fTmp0 = rfCx*m_afTuple[0] + rfSx*m_afTuple[1];
        Real fTmp1 = rfCx*m_afTuple[3] - rfSx*m_afTuple[2];
        fInvLength = CMath<Real>::InvSqrt(fTmp0*fTmp0 + fTmp1*fTmp1);
        rfCz = fTmp0 * fInvLength;
        rfSz = fTmp1 * fInvLength;

        if(CMath<Real>::FAbs(rfCz) >= CMath<Real>::FAbs(rfSz))
        {
            fInvLength = ((Real)1)/rfCz;
            rfCy = fTmp0 * fInvLength;
            rfSy = (rfCx*m_afTuple[2] + rfSx*m_afTuple[3]) * fInvLength;
        }
        else
        {
            fInvLength = ((Real)1)/rfSz;
            rfCy = fTmp1 * fInvLength;
            rfSy = (rfCx*m_afTuple[1] - rfSx*m_afTuple[0]) * fInvLength;
        }
    }
    else
    {
        // Infinitely many solutions.  Choose one of them.
        if(m_afTuple[0]*m_afTuple[2] + m_afTuple[1]*m_afTuple[3] > (Real)0)
        {
            // p = (p0,p1,p0,p1)
            rfCx = (Real)1;
            rfSx = (Real)0;
            rfCy = ms_fRootHalf;
            rfSy = ms_fRootHalf;
            rfCz = ms_fRootTwo * m_afTuple[0];
            rfSz = ms_fRootTwo * m_afTuple[1];
        }
        else
        {
            // p = (p0,p1,-p0,-p1)
            rfCx = (Real)1;
            rfSx = (Real)0;
            rfCy = ms_fRootHalf;
            rfSy = -ms_fRootHalf;
            rfCz = ms_fRootTwo * m_afTuple[0];
            rfSz = -ms_fRootTwo * m_afTuple[1];
        }
    }
}
//----------------------------------------------------------------------------
template <class Real>
void CQuaternion<Real>::FactorXZY (Real& rfCx, Real& rfSx, Real& rfCz,
    Real& rfSz, Real& rfCy, Real& rfSy)
{
    CQuaternion pkAlt(m_afTuple[0],m_afTuple[1],m_afTuple[3],-m_afTuple[2]);
    pkAlt.FactorXYZ(rfCx,rfSx,rfCz,rfSz,rfCy,rfSy);
    rfSy = -rfSy;
}
//----------------------------------------------------------------------------
template <class Real>
void CQuaternion<Real>::FactorYZX (Real& rfCy, Real& rfSy, Real& rfCz,
    Real& rfSz, Real& rfCx, Real& rfSx)
{
    CQuaternion pkAlt(m_afTuple[0],-m_afTuple[2],m_afTuple[3],-m_afTuple[1]);
    pkAlt.FactorXYZ(rfCy,rfSy,rfCz,rfSz,rfCx,rfSx);
    rfSx = -rfSx;
    rfSy = -rfSy;
}
//----------------------------------------------------------------------------
template <class Real>
void CQuaternion<Real>::FactorYXZ (Real& rfCy, Real& rfSy, Real& rfCx,
    Real& rfSx, Real& rfCz, Real& rfSz)
{
    CQuaternion pkAlt(m_afTuple[0],-m_afTuple[2],m_afTuple[1],m_afTuple[3]);
    pkAlt.FactorXYZ(rfCy,rfSy,rfCx,rfSx,rfCz,rfSz);
    rfSy = -rfSy;
}
//----------------------------------------------------------------------------
template <class Real>
void CQuaternion<Real>::FactorZXY (Real& rfCz, Real& rfSz, Real& rfCx,
    Real& rfSx, Real& rfCy, Real& rfSy)
{
    CQuaternion pkAlt(m_afTuple[0],-m_afTuple[3],m_afTuple[1],-m_afTuple[2]);
    pkAlt.FactorXYZ(rfCz,rfSz,rfCx,rfSx,rfCy,rfSy);
    rfSy = -rfSy;
    rfSz = -rfSz;
}
//----------------------------------------------------------------------------
template <class Real>
void CQuaternion<Real>::FactorZYX (Real& rfCz, Real& rfSz, Real& rfCy,
    Real& rfSy, Real& rfCx, Real& rfSx)
{
    CQuaternion pkAlt(m_afTuple[0],m_afTuple[3],-m_afTuple[2],m_afTuple[1]);
    pkAlt.FactorXYZ(rfCz,rfSz,rfCy,rfSy,rfCx,rfSx);
    rfSy = -rfSy;
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real> CQuaternion<Real>::GetClosest (int iAxis,
    const Constraints& rkCon) const
{
    CQuaternion kQ((Real)0,(Real)0,(Real)0,(Real)0);

    Real fP0 = m_afTuple[0];
    Real fP1 = m_afTuple[iAxis];
    Real fSqrLength = fP0*fP0 + fP1*fP1;
    if (fSqrLength > ms_fTolerance)
    {
        Real fInvLength = CMath<Real>::InvSqrt(fSqrLength);
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
            Real fCsMin = rkCon.m_fCosMinAngle;
            Real fSnMin = rkCon.m_fSinMinAngle;
            Real fDotMinAngle = fP0*fCsMin + fP1*fSnMin;
            if (fDotMinAngle < (Real)0)
            {
                fCsMin = -fCsMin;
                fSnMin = -fSnMin;
                fDotMinAngle = -fDotMinAngle;
            }

            Real fCsMax = rkCon.m_fCosMaxAngle;
            Real fSnMax = rkCon.m_fSinMaxAngle;
            Real fDotMaxAngle = fP0*fCsMax + fP1*fSnMax;
            if (fDotMaxAngle < (Real)0)
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
template <class Real>
CQuaternion<Real> CQuaternion<Real>::GetClosestX (const Constraints& rkXCon)
    const
{
    return GetClosest(1,rkXCon);
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real> CQuaternion<Real>::GetClosestY (const Constraints& rkYCon)
    const
{
    return GetClosest(2,rkYCon);
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real> CQuaternion<Real>::GetClosestZ (const Constraints& rkZCon)
    const
{
    return GetClosest(3,rkZCon);
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real> CQuaternion<Real>::GetClosestXY (const Constraints& rkXCon,
    const Constraints& rkYCon) const
{
    CQuaternion kQ, kTmp;
    Real fC0, fS0, fC1, fS1, fInvLength;

    Real fDet = m_afTuple[0]*m_afTuple[3] - m_afTuple[1]*m_afTuple[2];
    if (CMath<Real>::FAbs(fDet) < (Real)0.5 - ms_fTolerance)
    {
        Real fDiscr = CMath<Real>::Sqrt(CMath<Real>::FAbs((Real)1 -
            ((Real)4)*fDet*fDet));
        Real fA = m_afTuple[0]*m_afTuple[1] + m_afTuple[2]*m_afTuple[3];
        Real fB = m_afTuple[0]*m_afTuple[0] - m_afTuple[1]*m_afTuple[1]
            + m_afTuple[2]*m_afTuple[2] - m_afTuple[3]*m_afTuple[3];

        if (fB >= (Real)0)
        {
            fC0 = ((Real)0.5)*(fDiscr + fB);
            fS0 = fA;
        }
        else
        {
            fC0 = fA;
            fS0 = ((Real)0.5)*(fDiscr - fB);
        }
        fInvLength = CMath<Real>::InvSqrt(fC0*fC0 + fS0*fS0);
        fC0 *= fInvLength;
        fS0 *= fInvLength;

        fC1 = m_afTuple[0]*fC0 + m_afTuple[1]*fS0;
        fS1 = m_afTuple[2]*fC0 + m_afTuple[3]*fS0;
        fInvLength = CMath<Real>::InvSqrt(fC1*fC1 + fS1*fS1);
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
                (Real)0,(Real)0);
            CQuaternion kRInv(rkXCon.m_fCosMinAngle,-rkXCon.m_fSinMinAngle,
                (Real)0,(Real)0);
            CQuaternion kProd = kRInv*(*this);
            kTmp = kProd.GetClosest(2,rkYCon);
            Real fDotOptAngle = kProd.Dot(kTmp);
            kQ = kR*kTmp;

            kR = CQuaternion(rkXCon.m_fCosMaxAngle,rkXCon.m_fSinMaxAngle,
                (Real)0,(Real)0);
            kRInv = CQuaternion(rkXCon.m_fCosMaxAngle,-rkXCon.m_fSinMaxAngle,
                (Real)0,(Real)0);
            kProd = kRInv*(*this);
            kTmp = kProd.GetClosest(2,rkYCon);
            Real fDotAngle = kProd.Dot(kTmp);
            if (fDotAngle > fDotOptAngle)
            {
                kQ = kR*kTmp;
                fDotOptAngle = fDotAngle;
            }

            kR = CQuaternion(rkYCon.m_fCosMinAngle,(Real)0,
                rkYCon.m_fSinMinAngle,(Real)0);
            kRInv = CQuaternion(rkYCon.m_fCosMinAngle,(Real)0,
                -rkYCon.m_fSinMinAngle,(Real)0);
            kProd = (*this)*kRInv;
            kTmp = kProd.GetClosest(1,rkXCon);
            fDotAngle = kProd.Dot(kTmp);
            if (fDotAngle > fDotOptAngle)
            {
                kQ = kTmp*kR;
                fDotOptAngle = fDotAngle;
            }

            kR = CQuaternion(rkYCon.m_fCosMaxAngle,(Real)0,
                rkYCon.m_fSinMaxAngle,(Real)0);
            kRInv = CQuaternion(rkYCon.m_fCosMaxAngle,(Real)0,
                -rkYCon.m_fSinMaxAngle,(Real)0);
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
        Real fMinAngle, fMaxAngle, fAngle;
        Constraints kCon;

        if (fDet > (Real)0)
        {
            fMinAngle = rkXCon.m_fMinAngle - rkYCon.m_fMaxAngle;
            fMaxAngle = rkXCon.m_fMaxAngle - rkYCon.m_fMinAngle;
            kCon.SetAngles(fMinAngle,fMaxAngle);

            kTmp = GetClosest(1,kCon);

            fAngle = CMath<Real>::ATan2(kTmp[1],kTmp[0]);
            if (fAngle < fMinAngle || fAngle > fMaxAngle)
            {
                fAngle -= 
                    (kTmp[1] >= (Real)0 ? CMath<Real>::PI : -CMath<Real>::PI);
                // assert(fMinAngle <= fAngle && fAngle <= fMaxAngle);
            }

            if (fAngle <= rkXCon.m_fMaxAngle - rkYCon.m_fMaxAngle)
            {
                fC1 = rkYCon.m_fCosMaxAngle;
                fS1 = rkYCon.m_fSinMaxAngle;
                fAngle = rkYCon.m_fMaxAngle + fAngle;
                fC0 = CMath<Real>::Cos(fAngle);
                fS0 = CMath<Real>::Sin(fAngle);
            }
            else
            {
                fC0 = rkXCon.m_fCosMaxAngle;
                fS0 = rkXCon.m_fSinMaxAngle;
                fAngle = rkXCon.m_fMaxAngle - fAngle;
                fC1 = CMath<Real>::Cos(fAngle);
                fS1 = CMath<Real>::Sin(fAngle);
            }
        }
        else
        {
            fMinAngle = rkXCon.m_fMinAngle + rkYCon.m_fMinAngle;
            fMaxAngle = rkXCon.m_fMaxAngle + rkYCon.m_fMaxAngle;
            kCon.SetAngles(fMinAngle,fMaxAngle);

            kTmp = GetClosest(1,kCon);

            fAngle = CMath<Real>::ATan2(kTmp[1],kTmp[0]);
            if (fAngle < fMinAngle || fAngle > fMaxAngle)
            {
                fAngle -=
                    (kTmp[1] >= (Real)0 ? CMath<Real>::PI : -CMath<Real>::PI);
                // assert(fMinAngle <= fAngle && fAngle <= fMaxAngle);
            }

            if (fAngle >= rkXCon.m_fMinAngle + rkYCon.m_fMaxAngle)
            {
                fC1 = rkYCon.m_fCosMaxAngle;
                fS1 = rkYCon.m_fSinMaxAngle;
                fAngle = fAngle - rkYCon.m_fMaxAngle;
                fC0 = CMath<Real>::Cos(fAngle);
                fS0 = CMath<Real>::Sin(fAngle);
            }
            else
            {
                fC0 = rkXCon.m_fCosMaxAngle;
                fS0 = rkXCon.m_fSinMaxAngle;
                fAngle = fAngle - rkXCon.m_fMaxAngle;
                fC1 = CMath<Real>::Cos(fAngle);
                fS1 = CMath<Real>::Sin(fAngle);
            }
        }

        kQ[0] = fC0*fC1;
        kQ[1] = fS0*fC1;
        kQ[2] = fC0*fS1;
        kQ[3] = fS0*fS1;
        if (Dot(kQ) < (Real)0)
        {
            kQ = -kQ;
        }
    }

    return kQ;
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real> CQuaternion<Real>::GetClosestYX (const Constraints& rkYCon,
    const Constraints& rkXCon) const
{
    CQuaternion pkAlt(m_afTuple[0],m_afTuple[1],m_afTuple[2],-m_afTuple[3]);
    CQuaternion kQ = pkAlt.GetClosestXY(rkXCon,rkYCon);
    kQ[3] = -kQ[3];
    return kQ;
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real> CQuaternion<Real>::GetClosestZX (const Constraints& rkZCon,
    const Constraints& rkXCon) const
{
    CQuaternion kQ, kTmp;
    Real fC2, fS2, fC0, fS0, fInvLength;

    Real fDet = m_afTuple[0]*m_afTuple[2] - m_afTuple[1]*m_afTuple[3];
    if (CMath<Real>::FAbs(fDet) < (Real)0.5 - ms_fTolerance)
    {
        Real fDiscr = CMath<Real>::Sqrt(CMath<Real>::FAbs((Real)1 -
            ((Real)4)*fDet*fDet));
        Real fA = m_afTuple[0]*m_afTuple[3] + m_afTuple[1]*m_afTuple[2];
        Real fB = m_afTuple[0]*m_afTuple[0] + m_afTuple[1]*m_afTuple[1]
            - m_afTuple[2]*m_afTuple[2] - m_afTuple[3]*m_afTuple[3];

        if (fB >= (Real)0)
        {
            fC2 = ((Real)0.5)*(fDiscr + fB);
            fS2 = fA;
        }
        else
        {
            fC2 = fA;
            fS2 = ((Real)0.5)*(fDiscr - fB);
        }
        fInvLength = CMath<Real>::InvSqrt(fC2*fC2 + fS2*fS2);
        fC2 *= fInvLength;
        fS2 *= fInvLength;

        fC0 = m_afTuple[0]*fC2 + m_afTuple[3]*fS2;
        fS0 = m_afTuple[1]*fC2 + m_afTuple[2]*fS2;
        fInvLength = CMath<Real>::InvSqrt(fC0*fC0 + fS0*fS0);
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
            CQuaternion kR(rkZCon.m_fCosMinAngle,(Real)0,(Real)0,
                rkZCon.m_fSinMinAngle);
            CQuaternion kRInv(rkZCon.m_fCosMinAngle,(Real)0,(Real)0,
                -rkZCon.m_fSinMinAngle);
            CQuaternion kProd = kRInv*(*this);
            kTmp = kProd.GetClosest(1,rkXCon);
            Real fDotOptAngle = kProd.Dot(kTmp);
            kQ = kR*kTmp;

            kR = CQuaternion(rkZCon.m_fCosMaxAngle,(Real)0,(Real)0,
                rkZCon.m_fSinMaxAngle);
            kRInv = CQuaternion(rkZCon.m_fCosMaxAngle,(Real)0,(Real)0,
                -rkZCon.m_fSinMaxAngle);
            kProd = kRInv*(*this);
            kTmp = kProd.GetClosest(1,rkXCon);
            Real fDotAngle = kProd.Dot(kTmp);
            if (fDotAngle > fDotOptAngle)
            {
                kQ = kR*kTmp;
                fDotOptAngle = fDotAngle;
            }

            kR = CQuaternion(rkXCon.m_fCosMinAngle,rkXCon.m_fSinMinAngle,
                (Real)0,(Real)0);
            kRInv = CQuaternion(rkXCon.m_fCosMinAngle,-rkXCon.m_fSinMinAngle,
                (Real)0,(Real)0);
            kProd = (*this)*kRInv;
            kTmp = kProd.GetClosest(3,rkZCon);
            fDotAngle = kProd.Dot(kTmp);
            if (fDotAngle > fDotOptAngle)
            {
                kQ = kTmp*kR;
                fDotOptAngle = fDotAngle;
            }

            kR = CQuaternion(rkXCon.m_fCosMaxAngle,rkXCon.m_fSinMaxAngle,
                (Real)0,(Real)0);
            kRInv = CQuaternion(rkXCon.m_fCosMaxAngle,-rkXCon.m_fSinMaxAngle,
                (Real)0,(Real)0);
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
        Real fMinAngle, fMaxAngle, fAngle;
        Constraints kCon;

        if (fDet > (Real)0)
        {
            fMinAngle = rkXCon.m_fMinAngle - rkZCon.m_fMaxAngle;
            fMaxAngle = rkXCon.m_fMaxAngle - rkZCon.m_fMinAngle;
            kCon.SetAngles(fMinAngle,fMaxAngle);

            kTmp = GetClosest(1,kCon);

            fAngle = CMath<Real>::ATan2(kTmp[1],kTmp[0]);
            if (fAngle < fMinAngle || fAngle > fMaxAngle)
            {
                fAngle -=
                    (kTmp[1] >= (Real)0 ? CMath<Real>::PI : -CMath<Real>::PI);
                // assert(fMinAngle <= fAngle && fAngle <= fMaxAngle);
            }

            if (fAngle <= rkXCon.m_fMaxAngle - rkZCon.m_fMaxAngle)
            {
                fC2 = rkZCon.m_fCosMaxAngle;
                fS2 = rkZCon.m_fSinMaxAngle;
                fAngle = rkZCon.m_fMaxAngle + fAngle;
                fC0 = CMath<Real>::Cos(fAngle);
                fS0 = CMath<Real>::Sin(fAngle);
            }
            else
            {
                fC0 = rkXCon.m_fCosMaxAngle;
                fS0 = rkXCon.m_fSinMaxAngle;
                fAngle = rkXCon.m_fMaxAngle - fAngle;
                fC2 = CMath<Real>::Cos(fAngle);
                fS2 = CMath<Real>::Sin(fAngle);
            }
        }
        else
        {
            fMinAngle = rkXCon.m_fMinAngle + rkZCon.m_fMinAngle;
            fMaxAngle = rkXCon.m_fMaxAngle + rkZCon.m_fMaxAngle;
            kCon.SetAngles(fMinAngle,fMaxAngle);

            kTmp = GetClosest(1,kCon);

            fAngle = CMath<Real>::ATan2(kTmp[1],kTmp[0]);
            if (fAngle < fMinAngle || fAngle > fMaxAngle)
            {
                fAngle -=
                    (kTmp[1] >= (Real)0 ? CMath<Real>::PI : -CMath<Real>::PI);
                // assert(fMinAngle <= fAngle && fAngle <= fMaxAngle);
            }

            if (fAngle >= rkXCon.m_fMinAngle + rkZCon.m_fMaxAngle)
            {
                fC2 = rkZCon.m_fCosMaxAngle;
                fS2 = rkZCon.m_fSinMaxAngle;
                fAngle = fAngle - rkZCon.m_fMaxAngle;
                fC0 = CMath<Real>::Cos(fAngle);
                fS0 = CMath<Real>::Sin(fAngle);
            }
            else
            {
                fC0 = rkXCon.m_fCosMaxAngle;
                fS0 = rkXCon.m_fSinMaxAngle;
                fAngle = fAngle - rkXCon.m_fMaxAngle;
                fC2 = CMath<Real>::Cos(fAngle);
                fS2 = CMath<Real>::Sin(fAngle);
            }
        }

        kQ[0] = fC2*fC0;
        kQ[1] = fC2*fS0;
        kQ[2] = fS2*fS0;
        kQ[3] = fS2*fC0;
        if (Dot(kQ) < (Real)0)
        {
            kQ = -kQ;
        }
    }

    return kQ;
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real> CQuaternion<Real>::GetClosestXZ (const Constraints& rkXCon,
    const Constraints& rkZCon) const
{
    CQuaternion pkAlt(m_afTuple[0],m_afTuple[1],-m_afTuple[2],m_afTuple[3]);
    CQuaternion kQ = pkAlt.GetClosestZX(rkZCon,rkXCon);
    kQ[2] = -kQ[2];
    return kQ;
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real> CQuaternion<Real>::GetClosestZY (const Constraints& rkZCon,
    const Constraints& rkYCon) const
{
    CQuaternion kQ, kTmp;
    Real fC2, fS2, fC1, fS1, fInvLength;

    Real fDet = m_afTuple[0]*m_afTuple[1] + m_afTuple[2]*m_afTuple[3];
    if (CMath<Real>::FAbs(fDet) < (Real)0.5 - ms_fTolerance)
    {
        Real fDiscr = CMath<Real>::Sqrt(CMath<Real>::FAbs((Real)1 -
            ((Real)4)*fDet*fDet));
        Real fA = m_afTuple[0]*m_afTuple[3] - m_afTuple[1]*m_afTuple[2];
        Real fB = m_afTuple[0]*m_afTuple[0] - m_afTuple[1]*m_afTuple[1]
            + m_afTuple[2]*m_afTuple[2] - m_afTuple[3]*m_afTuple[3];

        if (fB >= (Real)0)
        {
            fC2 = ((Real)0.5)*(fDiscr + fB);
            fS2 = fA;
        }
        else
        {
            fC2 = fA;
            fS2 = ((Real)0.5)*(fDiscr - fB);
        }
        fInvLength = CMath<Real>::InvSqrt(fC2*fC2 + fS2*fS2);
        fC2 *= fInvLength;
        fS2 *= fInvLength;

        fC1 = m_afTuple[0]*fC2 + m_afTuple[3]*fS2;
        fS1 = m_afTuple[2]*fC2 - m_afTuple[1]*fS2;
        fInvLength = CMath<Real>::InvSqrt(fC1*fC1 + fS1*fS1);
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
            CQuaternion kR(rkZCon.m_fCosMinAngle,(Real)0,(Real)0,
                rkZCon.m_fSinMinAngle);
            CQuaternion kRInv(rkZCon.m_fCosMinAngle,(Real)0,(Real)0,
                -rkZCon.m_fSinMinAngle);
            CQuaternion kProd = kRInv*(*this);
            kTmp = kProd.GetClosest(2,rkYCon);
            Real fDotOptAngle = kProd.Dot(kTmp);
            kQ = kR*kTmp;

            kR = CQuaternion(rkZCon.m_fCosMaxAngle,(Real)0,(Real)0,
                rkZCon.m_fSinMaxAngle);
            kRInv = CQuaternion(rkZCon.m_fCosMaxAngle,(Real)0,(Real)0,
                -rkZCon.m_fSinMaxAngle);
            kProd = kRInv*(*this);
            kTmp = kProd.GetClosest(2,rkYCon);
            Real fDotAngle = kProd.Dot(kTmp);
            if (fDotAngle > fDotOptAngle)
            {
                kQ = kR*kTmp;
                fDotOptAngle = fDotAngle;
            }

            kR = CQuaternion(rkYCon.m_fCosMinAngle,(Real)0,
                rkYCon.m_fSinMinAngle,(Real)0);
            kRInv = CQuaternion(rkYCon.m_fCosMinAngle,(Real)0,
                -rkYCon.m_fSinMinAngle,(Real)0);
            kProd = (*this)*kRInv;
            kTmp = kProd.GetClosest(3,rkZCon);
            fDotAngle = kProd.Dot(kTmp);
            if (fDotAngle > fDotOptAngle)
            {
                kQ = kTmp*kR;
                fDotOptAngle = fDotAngle;
            }

            kR = CQuaternion(rkYCon.m_fCosMaxAngle,(Real)0,
                rkYCon.m_fSinMaxAngle,(Real)0);
            kRInv = CQuaternion(rkYCon.m_fCosMaxAngle,(Real)0,
                -rkYCon.m_fSinMaxAngle,(Real)0);
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
        Real fMinAngle, fMaxAngle, fAngle;
        Constraints kCon;

        if (fDet < (Real)0)
        {
            fMinAngle = rkYCon.m_fMinAngle - rkZCon.m_fMaxAngle;
            fMaxAngle = rkYCon.m_fMaxAngle - rkZCon.m_fMinAngle;
            kCon.SetAngles(fMinAngle,fMaxAngle);

            kTmp = GetClosest(2,kCon);

            fAngle = CMath<Real>::ATan2(kTmp[2],kTmp[0]);
            if (fAngle < fMinAngle || fAngle > fMaxAngle)
            {
                fAngle -=
                    (kTmp[2] >= (Real)0 ? CMath<Real>::PI : -CMath<Real>::PI);
                // assert(fMinAngle <= fAngle && fAngle <= fMaxAngle);
            }

            if (fAngle <= rkYCon.m_fMaxAngle - rkZCon.m_fMaxAngle)
            {
                fC2 = rkZCon.m_fCosMaxAngle;
                fS2 = rkZCon.m_fSinMaxAngle;
                fAngle = rkZCon.m_fMaxAngle + fAngle;
                fC1 = CMath<Real>::Cos(fAngle);
                fS1 = CMath<Real>::Sin(fAngle);
            }
            else
            {
                fC1 = rkYCon.m_fCosMaxAngle;
                fS1 = rkYCon.m_fSinMaxAngle;
                fAngle = rkYCon.m_fMaxAngle - fAngle;
                fC2 = CMath<Real>::Cos(fAngle);
                fS2 = CMath<Real>::Sin(fAngle);
            }
        }
        else
        {
            fMinAngle = rkYCon.m_fMinAngle + rkZCon.m_fMinAngle;
            fMaxAngle = rkYCon.m_fMaxAngle + rkZCon.m_fMaxAngle;
            kCon.SetAngles(fMinAngle,fMaxAngle);

            kTmp = GetClosest(2,kCon);

            fAngle = CMath<Real>::ATan2(kTmp[2],kTmp[0]);
            if (fAngle < fMinAngle || fAngle > fMaxAngle)
            {
                fAngle -=
                    (kTmp[2] >= (Real)0 ? CMath<Real>::PI : -CMath<Real>::PI);
                // assert(fMinAngle <= fAngle && fAngle <= fMaxAngle);
            }

            if (fAngle >= rkYCon.m_fMinAngle + rkZCon.m_fMaxAngle)
            {
                fC2 = rkZCon.m_fCosMaxAngle;
                fS2 = rkZCon.m_fSinMaxAngle;
                fAngle = fAngle - rkZCon.m_fMaxAngle;
                fC1 = CMath<Real>::Cos(fAngle);
                fS1 = CMath<Real>::Sin(fAngle);
            }
            else
            {
                fC1 = rkYCon.m_fCosMaxAngle;
                fS1 = rkYCon.m_fSinMaxAngle;
                fAngle = fAngle - rkYCon.m_fMaxAngle;
                fC2 = CMath<Real>::Cos(fAngle);
                fS2 = CMath<Real>::Sin(fAngle);
            }
        }

        kQ[0] = fC2*fC1;
        kQ[1] = -fS2*fS1;
        kQ[2] = fC2*fS1;
        kQ[3] = fS2*fC1;
        if (Dot(kQ) < (Real)0)
        {
            kQ = -kQ;
        }
    }

    return kQ;
}
//----------------------------------------------------------------------------
template <class Real>
CQuaternion<Real> CQuaternion<Real>::GetClosestYZ (const Constraints& rkYCon,
    const Constraints& rkZCon) const
{
    CQuaternion pkAlt(m_afTuple[0],-m_afTuple[1],m_afTuple[2],m_afTuple[3]);
    CQuaternion kQ = pkAlt.GetClosestZY(rkZCon,rkYCon);
    kQ[1] = -kQ[1];
    return kQ;
}

typedef CQuaternion<real> Quaternionf;


}