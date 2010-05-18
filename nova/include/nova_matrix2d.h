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

#include "nova_vector2d.h"

namespace nova
{

template <class Real>
class CMatrix2: public CObjectConstructor
{
public:
    // If bZero is true, create the zero matrix.  Otherwise, create the
    // identity matrix.
    CMatrix2 (bool bZero = true);

    // copy constructor
    CMatrix2 (const CMatrix2& rkM);

    // input Mrc is in row r, column c.
    CMatrix2 (Real fM00, Real fM01, Real fM10, Real fM11);

    // Create a matrix from an array of numbers.  The input array is
    // interpreted based on the Boolean input as
    //   true:  entry[0..3] = {m00,m01,m10,m11}  [row major]
    //   false: entry[0..3] = {m00,m10,m01,m11}  [column major]
    CMatrix2 (const Real afEntry[4], bool bRowMajor);

    // Create matrices based on vector input.  The Boolean is interpreted as
    //   true: vectors are columns of the matrix
    //   false: vectors are rows of the matrix
    CMatrix2 (const CVector2<Real>& rkU, const CVector2<Real>& rkV,
        bool bColumns);
    CMatrix2 (const CVector2<Real>* akV, bool bColumns);

    // create a diagonal matrix
    CMatrix2 (Real fM00, Real fM11);

    // create a rotation matrix (positive angle - counterclockwise)
    CMatrix2 (Real fAngle);

    // create a tensor product U*V^T
    CMatrix2 (const CVector2<Real>& rkU, const CVector2<Real>& rkV);

    // create various matrices
    void MakeZero ();
    void MakeIdentity ();
    void MakeDiagonal (Real fM00, Real fM11);
    void FromAngle (Real fAngle);
    void MakeTensorProduct (const CVector2<Real>& rkU,
        const CVector2<Real>& rkV);

    // member access
    inline operator const Real* () const;
    inline operator Real* ();
    inline const Real* operator[] (int iRow) const;
    inline Real* operator[] (int iRow);
    inline Real operator() (int iRow, int iCol) const;
    inline Real& operator() (int iRow, int iCol);
    void SetRow (int iRow, const CVector2<Real>& rkV);
    CVector2<Real> GetRow (int iRow) const;
    void SetColumn (int iCol, const CVector2<Real>& rkV);
    CVector2<Real> GetColumn (int iCol) const;
    void GetColumnMajor (Real* afCMajor) const;

    // assignment
    inline CMatrix2& operator= (const CMatrix2& rkM);

    // comparison
    bool operator== (const CMatrix2& rkM) const;
    bool operator!= (const CMatrix2& rkM) const;
    bool operator<  (const CMatrix2& rkM) const;
    bool operator<= (const CMatrix2& rkM) const;
    bool operator>  (const CMatrix2& rkM) const;
    bool operator>= (const CMatrix2& rkM) const;

    // arithmetic operations
    inline CMatrix2 operator+ (const CMatrix2& rkM) const;
    inline CMatrix2 operator- (const CMatrix2& rkM) const;
    inline CMatrix2 operator* (const CMatrix2& rkM) const;
    inline CMatrix2 operator* (Real fScalar) const;
    inline CMatrix2 operator/ (Real fScalar) const;
    inline CMatrix2 operator- () const;

    // arithmetic updates
    inline CMatrix2& operator+= (const CMatrix2& rkM);
    inline CMatrix2& operator-= (const CMatrix2& rkM);
    inline CMatrix2& operator*= (Real fScalar);
    inline CMatrix2& operator/= (Real fScalar);

    // matrix times vector
    inline CVector2<Real> operator* (const CVector2<Real>& rkV) const;  // M * v

    // other operations
    CMatrix2 Transpose () const;  // M^T
    CMatrix2 TransposeTimes (const CMatrix2& rkM) const;  // this^T * M
    CMatrix2 TimesTranspose (const CMatrix2& rkM) const;  // this * M^T
    CMatrix2 Inverse () const;
    CMatrix2 Adjoint () const;
    Real Determinant () const;
    Real QForm (const CVector2<Real>& rkU,
        const CVector2<Real>& rkV) const;  // u^T*M*v

    // The matrix must be a rotation for these functions to be valid.  The
    // last function uses Gram-Schmidt orthonormalization applied to the
    // columns of the rotation matrix.  The angle must be in radians, not
    // degrees.
    void ToAngle (Real& rfAngle) const;
    void Orthonormalize ();

    // The matrix must be symmetric.  Factor M = R * D * R^T where
    // R = [u0|u1] is a rotation matrix with columns u0 and u1 and
    // D = diag(d0,d1) is a diagonal matrix whose diagonal entries are d0 and
    // d1.  The eigenvector u[i] corresponds to eigenvector d[i].  The
    // eigenvalues are ordered as d0 <= d1.
    void EigenDecomposition (CMatrix2& rkRot, CMatrix2& rkDiag) const;

    NOVA_EXPORT static const CMatrix2 ZERO;
    NOVA_EXPORT static const CMatrix2 IDENTITY;

private:
    // support for comparisons
    int CompareArrays (const CMatrix2& rkM) const;

    // matrix stored in row-major order
    Real m_afEntry[4];
};

// c * M
template <class Real>
inline CMatrix2<Real> operator* (Real fScalar, const CMatrix2<Real>& rkM);

// v^T * M
template <class Real>
inline CVector2<Real> operator* (const CVector2<Real>& rkV,
    const CMatrix2<Real>& rkM);

template <class Real>
CMatrix2<Real>::CMatrix2 (bool bZero)
{
    if (bZero)
    {
        MakeZero();
    }
    else
    {
        MakeIdentity();
    }
}
//----------------------------------------------------------------------------
template <class Real>
CMatrix2<Real>::CMatrix2 (const CMatrix2& rkM)
{
    m_afEntry[0] = rkM.m_afEntry[0];
    m_afEntry[1] = rkM.m_afEntry[1];
    m_afEntry[2] = rkM.m_afEntry[2];
    m_afEntry[3] = rkM.m_afEntry[3];
}
//----------------------------------------------------------------------------
template <class Real>
CMatrix2<Real>::CMatrix2 (Real fM00, Real fM01, Real fM10, Real fM11)
{
    m_afEntry[0] = fM00;
    m_afEntry[1] = fM01;
    m_afEntry[2] = fM10;
    m_afEntry[3] = fM11;
}
//----------------------------------------------------------------------------
template <class Real>
CMatrix2<Real>::CMatrix2 (const Real afEntry[4], bool bRowMajor)
{
    if (bRowMajor)
    {
        m_afEntry[0] = afEntry[0];
        m_afEntry[1] = afEntry[1];
        m_afEntry[2] = afEntry[2];
        m_afEntry[3] = afEntry[3];
    }
    else
    {
        m_afEntry[0] = afEntry[0];
        m_afEntry[1] = afEntry[2];
        m_afEntry[2] = afEntry[1];
        m_afEntry[3] = afEntry[3];
    }
}
//----------------------------------------------------------------------------
template <class Real>
CMatrix2<Real>::CMatrix2 (const CVector2<Real>& rkU, const CVector2<Real>& rkV,
    bool bColumns)
{
    if (bColumns)
    {
        m_afEntry[0] = rkU[0];
        m_afEntry[1] = rkV[0];
        m_afEntry[2] = rkU[1];
        m_afEntry[3] = rkV[1];
    }
    else
    {
        m_afEntry[0] = rkU[0];
        m_afEntry[1] = rkU[1];
        m_afEntry[2] = rkV[0];
        m_afEntry[3] = rkV[1];
    }
}
//----------------------------------------------------------------------------
template <class Real>
CMatrix2<Real>::CMatrix2 (const CVector2<Real>* akV, bool bColumns)
{
    if (bColumns)
    {
        m_afEntry[0] = akV[0][0];
        m_afEntry[1] = akV[1][0];
        m_afEntry[2] = akV[0][1];
        m_afEntry[3] = akV[1][1];
    }
    else
    {
        m_afEntry[0] = akV[0][0];
        m_afEntry[1] = akV[0][1];
        m_afEntry[2] = akV[1][0];
        m_afEntry[3] = akV[1][1];
    }
}
//----------------------------------------------------------------------------
template <class Real>
CMatrix2<Real>::CMatrix2 (Real fM00, Real fM11)
{
    MakeDiagonal(fM00,fM11);
}
//----------------------------------------------------------------------------
template <class Real>
CMatrix2<Real>::CMatrix2 (Real fAngle)
{
    FromAngle(fAngle);
}
//----------------------------------------------------------------------------
template <class Real>
CMatrix2<Real>::CMatrix2 (const CVector2<Real>& rkU, const CVector2<Real>& rkV)
{
    MakeTensorProduct(rkU,rkV);
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix2<Real>::operator const Real* () const
{
    return m_afEntry;
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix2<Real>::operator Real* ()
{
    return m_afEntry;
}
//----------------------------------------------------------------------------
template <class Real>
inline const Real* CMatrix2<Real>::operator[] (int iRow) const
{
    return &m_afEntry[2*iRow];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real* CMatrix2<Real>::operator[] (int iRow)
{
    return &m_afEntry[2*iRow];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CMatrix2<Real>::operator() (int iRow, int iCol) const
{
    return m_afEntry[iCol + 2*iRow];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real& CMatrix2<Real>::operator() (int iRow, int iCol)
{
    return m_afEntry[iCol + 2*iRow];
}
//----------------------------------------------------------------------------
template <class Real>
void CMatrix2<Real>::MakeZero ()
{
    m_afEntry[0] = (Real)0.0;
    m_afEntry[1] = (Real)0.0;
    m_afEntry[2] = (Real)0.0;
    m_afEntry[3] = (Real)0.0;
}
//----------------------------------------------------------------------------
template <class Real>
void CMatrix2<Real>::MakeIdentity ()
{
    m_afEntry[0] = (Real)1.0;
    m_afEntry[1] = (Real)0.0;
    m_afEntry[2] = (Real)0.0;
    m_afEntry[3] = (Real)1.0;
}
//----------------------------------------------------------------------------
template <class Real>
void CMatrix2<Real>::MakeDiagonal (Real fM00, Real fM11)
{
    m_afEntry[0] = fM00;
    m_afEntry[1] = (Real)0.0;
    m_afEntry[2] = (Real)0.0;
    m_afEntry[3] = fM11;
}
//----------------------------------------------------------------------------
template <class Real>
void CMatrix2<Real>::FromAngle (Real fAngle)
{
    m_afEntry[0] = CMath<Real>::Cos(fAngle);
    m_afEntry[2] = CMath<Real>::Sin(fAngle);
    m_afEntry[1] = -m_afEntry[2];
    m_afEntry[3] =  m_afEntry[0];
}
//----------------------------------------------------------------------------
template <class Real>
void CMatrix2<Real>::MakeTensorProduct (const CVector2<Real>& rkU,
    const CVector2<Real>& rkV)
{
    m_afEntry[0] = rkU[0]*rkV[0];
    m_afEntry[1] = rkU[0]*rkV[1];
    m_afEntry[2] = rkU[1]*rkV[0];
    m_afEntry[3] = rkU[1]*rkV[1];
}
//----------------------------------------------------------------------------
template <class Real>
void CMatrix2<Real>::SetRow (int iRow, const CVector2<Real>& rkV)
{
    int i0 = 2*iRow ,i1 = i0+1;
    m_afEntry[i0] = rkV[0];
    m_afEntry[i1] = rkV[1];
}
//----------------------------------------------------------------------------
template <class Real>
CVector2<Real> CMatrix2<Real>::GetRow (int iRow) const
{
    int i0 = 2*iRow ,i1 = i0+1;
    return CVector2<Real>(m_afEntry[i0],m_afEntry[i1]);
}
//----------------------------------------------------------------------------
template <class Real>
void CMatrix2<Real>::SetColumn (int iCol, const CVector2<Real>& rkV)
{
    m_afEntry[iCol] = rkV[0];
    m_afEntry[iCol+2] = rkV[1];
}
//----------------------------------------------------------------------------
template <class Real>
CVector2<Real> CMatrix2<Real>::GetColumn (int iCol) const
{
    return CVector2<Real>(m_afEntry[iCol],m_afEntry[iCol+2]);
}
//----------------------------------------------------------------------------
template <class Real>
void CMatrix2<Real>::GetColumnMajor (Real* afCMajor) const
{
    afCMajor[0] = m_afEntry[0];
    afCMajor[1] = m_afEntry[2];
    afCMajor[2] = m_afEntry[1];
    afCMajor[3] = m_afEntry[3];
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix2<Real>& CMatrix2<Real>::operator= (const CMatrix2& rkM)
{
    m_afEntry[0] = rkM.m_afEntry[0];
    m_afEntry[1] = rkM.m_afEntry[1];
    m_afEntry[2] = rkM.m_afEntry[2];
    m_afEntry[3] = rkM.m_afEntry[3];
    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
int CMatrix2<Real>::CompareArrays (const CMatrix2& rkM) const
{
    return memcmp(m_afEntry,rkM.m_afEntry,4*sizeof(Real));
}
//----------------------------------------------------------------------------
template <class Real>
bool CMatrix2<Real>::operator== (const CMatrix2& rkM) const
{
    return CompareArrays(rkM) == 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CMatrix2<Real>::operator!= (const CMatrix2& rkM) const
{
    return CompareArrays(rkM) != 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CMatrix2<Real>::operator<  (const CMatrix2& rkM) const
{
    return CompareArrays(rkM) < 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CMatrix2<Real>::operator<= (const CMatrix2& rkM) const
{
    return CompareArrays(rkM) <= 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CMatrix2<Real>::operator>  (const CMatrix2& rkM) const
{
    return CompareArrays(rkM) > 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CMatrix2<Real>::operator>= (const CMatrix2& rkM) const
{
    return CompareArrays(rkM) >= 0;
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix2<Real> CMatrix2<Real>::operator+ (const CMatrix2& rkM) const
{
    return CMatrix2<Real>(
        m_afEntry[0] + rkM.m_afEntry[0],
        m_afEntry[1] + rkM.m_afEntry[1],
        m_afEntry[2] + rkM.m_afEntry[2],
        m_afEntry[3] + rkM.m_afEntry[3]);
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix2<Real> CMatrix2<Real>::operator- (const CMatrix2& rkM) const
{
    return CMatrix2<Real>(
        m_afEntry[0] - rkM.m_afEntry[0],
        m_afEntry[1] - rkM.m_afEntry[1],
        m_afEntry[2] - rkM.m_afEntry[2],
        m_afEntry[3] - rkM.m_afEntry[3]);
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix2<Real> CMatrix2<Real>::operator* (const CMatrix2& rkM) const
{
    return CMatrix2<Real>(
        m_afEntry[0]*rkM.m_afEntry[0] + m_afEntry[1]*rkM.m_afEntry[2],
        m_afEntry[0]*rkM.m_afEntry[1] + m_afEntry[1]*rkM.m_afEntry[3],
        m_afEntry[2]*rkM.m_afEntry[0] + m_afEntry[3]*rkM.m_afEntry[2],
        m_afEntry[2]*rkM.m_afEntry[1] + m_afEntry[3]*rkM.m_afEntry[3]);
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix2<Real> CMatrix2<Real>::operator* (Real fScalar) const
{
    return CMatrix2<Real>(
        fScalar*m_afEntry[0],
        fScalar*m_afEntry[1],
        fScalar*m_afEntry[2],
        fScalar*m_afEntry[3]);
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix2<Real> CMatrix2<Real>::operator/ (Real fScalar) const
{
    if (fScalar != (Real)0.0)
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        return CMatrix2<Real>(
            fInvScalar*m_afEntry[0],
            fInvScalar*m_afEntry[1],
            fInvScalar*m_afEntry[2],
            fInvScalar*m_afEntry[3]);
    }

    return CMatrix2<Real>(
        CMath<Real>::MAX_REAL,
        CMath<Real>::MAX_REAL,
        CMath<Real>::MAX_REAL,
        CMath<Real>::MAX_REAL);
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix2<Real> CMatrix2<Real>::operator- () const
{
    return CMatrix2<Real>(
        -m_afEntry[0],
        -m_afEntry[1],
        -m_afEntry[2],
        -m_afEntry[3]);
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix2<Real>& CMatrix2<Real>::operator+= (const CMatrix2& rkM)
{
    m_afEntry[0] += rkM.m_afEntry[0];
    m_afEntry[1] += rkM.m_afEntry[1];
    m_afEntry[2] += rkM.m_afEntry[2];
    m_afEntry[3] += rkM.m_afEntry[3];
    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix2<Real>& CMatrix2<Real>::operator-= (const CMatrix2& rkM)
{
    m_afEntry[0] -= rkM.m_afEntry[0];
    m_afEntry[1] -= rkM.m_afEntry[1];
    m_afEntry[2] -= rkM.m_afEntry[2];
    m_afEntry[3] -= rkM.m_afEntry[3];
    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix2<Real>& CMatrix2<Real>::operator*= (Real fScalar)
{
    m_afEntry[0] *= fScalar;
    m_afEntry[1] *= fScalar;
    m_afEntry[2] *= fScalar;
    m_afEntry[3] *= fScalar;
    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix2<Real>& CMatrix2<Real>::operator/= (Real fScalar)
{
    if (fScalar != (Real)0.0)
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        m_afEntry[0] *= fInvScalar;
        m_afEntry[1] *= fInvScalar;
        m_afEntry[2] *= fInvScalar;
        m_afEntry[3] *= fInvScalar;
    }
    else
    {
        m_afEntry[0] = CMath<Real>::MAX_REAL;
        m_afEntry[1] = CMath<Real>::MAX_REAL;
        m_afEntry[2] = CMath<Real>::MAX_REAL;
        m_afEntry[3] = CMath<Real>::MAX_REAL;
    }

    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector2<Real> CMatrix2<Real>::operator* (const CVector2<Real>& rkV) const
{
    return CVector2<Real>(
        m_afEntry[0]*rkV[0] + m_afEntry[1]*rkV[1],
        m_afEntry[2]*rkV[0] + m_afEntry[3]*rkV[1]);
}
//----------------------------------------------------------------------------
template <class Real>
CMatrix2<Real> CMatrix2<Real>::Transpose () const
{
    return CMatrix2<Real>(
        m_afEntry[0],
        m_afEntry[2],
        m_afEntry[1],
        m_afEntry[3]);
}
//----------------------------------------------------------------------------
template <class Real>
CMatrix2<Real> CMatrix2<Real>::TransposeTimes (const CMatrix2& rkM) const
{
    // P = A^T*B
    return CMatrix2<Real>(
        m_afEntry[0]*rkM.m_afEntry[0] + m_afEntry[2]*rkM.m_afEntry[2],
        m_afEntry[0]*rkM.m_afEntry[1] + m_afEntry[2]*rkM.m_afEntry[3],
        m_afEntry[1]*rkM.m_afEntry[0] + m_afEntry[3]*rkM.m_afEntry[2],
        m_afEntry[1]*rkM.m_afEntry[1] + m_afEntry[3]*rkM.m_afEntry[3]);
}
//----------------------------------------------------------------------------
template <class Real>
CMatrix2<Real> CMatrix2<Real>::TimesTranspose (const CMatrix2& rkM) const
{
    // P = A*B^T
    return CMatrix2<Real>(
        m_afEntry[0]*rkM.m_afEntry[0] + m_afEntry[1]*rkM.m_afEntry[1],
        m_afEntry[0]*rkM.m_afEntry[2] + m_afEntry[1]*rkM.m_afEntry[3],
        m_afEntry[2]*rkM.m_afEntry[0] + m_afEntry[3]*rkM.m_afEntry[1],
        m_afEntry[2]*rkM.m_afEntry[2] + m_afEntry[3]*rkM.m_afEntry[3]);
}
//----------------------------------------------------------------------------
template <class Real>
CMatrix2<Real> CMatrix2<Real>::Inverse () const
{
    CMatrix2<Real> kInverse;

    Real fDet = m_afEntry[0]*m_afEntry[3] - m_afEntry[1]*m_afEntry[2];
    if (CMath<Real>::FAbs(fDet) > CMath<Real>::ZERO_TOLERANCE)
    {
        Real fInvDet = ((Real)1.0)/fDet;
        kInverse.m_afEntry[0] =  m_afEntry[3]*fInvDet;
        kInverse.m_afEntry[1] = -m_afEntry[1]*fInvDet;
        kInverse.m_afEntry[2] = -m_afEntry[2]*fInvDet;
        kInverse.m_afEntry[3] =  m_afEntry[0]*fInvDet;
    }
    else
    {
        kInverse.m_afEntry[0] = (Real)0.0;
        kInverse.m_afEntry[1] = (Real)0.0;
        kInverse.m_afEntry[2] = (Real)0.0;
        kInverse.m_afEntry[3] = (Real)0.0;
    }

    return kInverse;
}
//----------------------------------------------------------------------------
template <class Real>
CMatrix2<Real> CMatrix2<Real>::Adjoint () const
{
    return CMatrix2<Real>(
        m_afEntry[3],
        -m_afEntry[1],
        -m_afEntry[2],
        m_afEntry[0]);
}
//----------------------------------------------------------------------------
template <class Real>
Real CMatrix2<Real>::Determinant () const
{
    return m_afEntry[0]*m_afEntry[3] - m_afEntry[1]*m_afEntry[2];
}
//----------------------------------------------------------------------------
template <class Real>
Real CMatrix2<Real>::QForm (const CVector2<Real>& rkU,
    const CVector2<Real>& rkV) const
{
    return rkU.Dot((*this)*rkV);
}
//----------------------------------------------------------------------------
template <class Real>
void CMatrix2<Real>::ToAngle (Real& rfAngle) const
{
    // assert:  matrix is a rotation
    rfAngle = CMath<Real>::ATan2(m_afEntry[2],m_afEntry[0]);
}
//----------------------------------------------------------------------------
template <class Real>
void CMatrix2<Real>::Orthonormalize ()
{
    // Algorithm uses Gram-Schmidt orthogonalization.  If 'this' matrix is
    // M = [m0|m1], then orthonormal output matrix is Q = [q0|q1],
    //
    //   q0 = m0/|m0|
    //   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
    //
    // where |V| indicates length of vector V and A*B indicates dot
    // product of vectors A and B.

    // compute q0
    Real fInvLength = CMath<Real>::InvSqrt(m_afEntry[0]*m_afEntry[0] +
        m_afEntry[2]*m_afEntry[2]);

    m_afEntry[0] *= fInvLength;
    m_afEntry[2] *= fInvLength;

    // compute q1
    Real fDot0 = m_afEntry[0]*m_afEntry[1] + m_afEntry[2]*m_afEntry[3];
    m_afEntry[1] -= fDot0*m_afEntry[0];
    m_afEntry[3] -= fDot0*m_afEntry[2];

    fInvLength = CMath<Real>::InvSqrt(m_afEntry[1]*m_afEntry[1] +
        m_afEntry[3]*m_afEntry[3]);

    m_afEntry[1] *= fInvLength;
    m_afEntry[3] *= fInvLength;
}
//----------------------------------------------------------------------------
template <class Real>
void CMatrix2<Real>::EigenDecomposition (CMatrix2& rkRot, CMatrix2& rkDiag) const
{
    Real fTrace = m_afEntry[0] + m_afEntry[3];
    Real fDiff = m_afEntry[0] - m_afEntry[3];
    Real fDiscr = CMath<Real>::Sqrt(fDiff*fDiff +
        ((Real)4.0)*m_afEntry[1]*m_afEntry[1]);
    Real fEVal0 = ((Real)0.5)*(fTrace-fDiscr);
    Real fEVal1 = ((Real)0.5)*(fTrace+fDiscr);
    rkDiag.MakeDiagonal(fEVal0,fEVal1);

    Real fCos, fSin;
    if (fDiff >= (Real)0.0)
    {
        fCos = m_afEntry[1];
        fSin = fEVal0 - m_afEntry[0];
    }
    else
    {
        fCos = fEVal0 - m_afEntry[3];
        fSin = m_afEntry[1];
    }
    Real fTmp = CMath<Real>::InvSqrt(fCos*fCos + fSin*fSin);
    fCos *= fTmp;
    fSin *= fTmp;

    rkRot.m_afEntry[0] = fCos;
    rkRot.m_afEntry[1] = -fSin;
    rkRot.m_afEntry[2] = fSin;
    rkRot.m_afEntry[3] = fCos;
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix2<Real> operator* (Real fScalar, const CMatrix2<Real>& rkM)
{
    return rkM*fScalar;
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector2<Real> operator* (const CVector2<Real>& rkV,
    const CMatrix2<Real>& rkM)
{
    return CVector2<Real>(
        rkV[0]*rkM[0][0] + rkV[1]*rkM[1][0],
        rkV[0]*rkM[0][1] + rkV[1]*rkM[1][1]);
}

typedef CMatrix2<real> Matrix2f;


}