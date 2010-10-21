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

template <class nReal>
class CMatrix2: public CObjectConstructor
{
public:
    // If bZero is true, create the zero matrix.  Otherwise, create the
    // identity matrix.
    CMatrix2 (bool bZero = true);

    // copy constructor
    CMatrix2 (const CMatrix2& rkM);

    // input Mrc is in row r, column c.
    CMatrix2 (nReal fM00, nReal fM01, nReal fM10, nReal fM11);

    // Create a matrix from an array of numbers.  The input array is
    // interpreted based on the Boolean input as
    //   true:  entry[0..3] = {m00,m01,m10,m11}  [row major]
    //   false: entry[0..3] = {m00,m10,m01,m11}  [column major]
    CMatrix2 (const nReal afEntry[4], bool bRowMajor);

    // Create matrices based on vector input.  The Boolean is interpreted as
    //   true: vectors are columns of the matrix
    //   false: vectors are rows of the matrix
    CMatrix2 (const CVector2<nReal>& rkU, const CVector2<nReal>& rkV,
        bool bColumns);
    CMatrix2 (const CVector2<nReal>* akV, bool bColumns);

    // create a diagonal matrix
    CMatrix2 (nReal fM00, nReal fM11);

    // create a rotation matrix (positive angle - counterclockwise)
    CMatrix2 (nReal fAngle);

    // create a tensor product U*V^T
    CMatrix2 (const CVector2<nReal>& rkU, const CVector2<nReal>& rkV);

    // create various matrices
    void MakeZero ();
    void MakeIdentity ();
    void MakeDiagonal (nReal fM00, nReal fM11);
    void FromAngle (nReal fAngle);
    void MakeTensorProduct (const CVector2<nReal>& rkU,
        const CVector2<nReal>& rkV);

    // member access
    inline operator const nReal* () const;
    inline operator nReal* ();
    inline const nReal* operator[] (nInt32 iRow) const;
    inline nReal* operator[] (nInt32 iRow);
    inline nReal operator() (nInt32 iRow, nInt32 iCol) const;
    inline nReal& operator() (nInt32 iRow, nInt32 iCol);
    void SetRow (nInt32 iRow, const CVector2<nReal>& rkV);
    CVector2<nReal> GetRow (nInt32 iRow) const;
    void SetColumn (nInt32 iCol, const CVector2<nReal>& rkV);
    CVector2<nReal> GetColumn (nInt32 iCol) const;
    void GetColumnMajor (nReal* afCMajor) const;

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
    inline CMatrix2 operator* (nReal fScalar) const;
    inline CMatrix2 operator/ (nReal fScalar) const;
    inline CMatrix2 operator- () const;

    // arithmetic updates
    inline CMatrix2& operator+= (const CMatrix2& rkM);
    inline CMatrix2& operator-= (const CMatrix2& rkM);
    inline CMatrix2& operator*= (nReal fScalar);
    inline CMatrix2& operator/= (nReal fScalar);

    // matrix times vector
    inline CVector2<nReal> operator* (const CVector2<nReal>& rkV) const;  // M * v

    // other operations
    CMatrix2 Transpose () const;  // M^T
    CMatrix2 TransposeTimes (const CMatrix2& rkM) const;  // this^T * M
    CMatrix2 TimesTranspose (const CMatrix2& rkM) const;  // this * M^T
    CMatrix2 Inverse () const;
    CMatrix2 Adjoint () const;
    nReal Determinant () const;
    nReal QForm (const CVector2<nReal>& rkU,
        const CVector2<nReal>& rkV) const;  // u^T*M*v

    // The matrix must be a rotation for these functions to be valid.  The
    // last function uses Gram-Schmidt orthonormalization applied to the
    // columns of the rotation matrix.  The angle must be in radians, not
    // degrees.
    void ToAngle (nReal& rfAngle) const;
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
    nInt32 CompareArrays (const CMatrix2& rkM) const;

    // matrix stored in row-major order
    nReal m_afEntry[4];
};

// c * M
template <class nReal>
inline CMatrix2<nReal> operator* (nReal fScalar, const CMatrix2<nReal>& rkM);

// v^T * M
template <class nReal>
inline CVector2<nReal> operator* (const CVector2<nReal>& rkV,
    const CMatrix2<nReal>& rkM);

template <class nReal>
CMatrix2<nReal>::CMatrix2 (bool bZero)
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
template <class nReal>
CMatrix2<nReal>::CMatrix2 (const CMatrix2& rkM)
{
    m_afEntry[0] = rkM.m_afEntry[0];
    m_afEntry[1] = rkM.m_afEntry[1];
    m_afEntry[2] = rkM.m_afEntry[2];
    m_afEntry[3] = rkM.m_afEntry[3];
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix2<nReal>::CMatrix2 (nReal fM00, nReal fM01, nReal fM10, nReal fM11)
{
    m_afEntry[0] = fM00;
    m_afEntry[1] = fM01;
    m_afEntry[2] = fM10;
    m_afEntry[3] = fM11;
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix2<nReal>::CMatrix2 (const nReal afEntry[4], bool bRowMajor)
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
template <class nReal>
CMatrix2<nReal>::CMatrix2 (const CVector2<nReal>& rkU, const CVector2<nReal>& rkV,
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
template <class nReal>
CMatrix2<nReal>::CMatrix2 (const CVector2<nReal>* akV, bool bColumns)
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
template <class nReal>
CMatrix2<nReal>::CMatrix2 (nReal fM00, nReal fM11)
{
    MakeDiagonal(fM00,fM11);
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix2<nReal>::CMatrix2 (nReal fAngle)
{
    FromAngle(fAngle);
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix2<nReal>::CMatrix2 (const CVector2<nReal>& rkU, const CVector2<nReal>& rkV)
{
    MakeTensorProduct(rkU,rkV);
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix2<nReal>::operator const nReal* () const
{
    return m_afEntry;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix2<nReal>::operator nReal* ()
{
    return m_afEntry;
}
//----------------------------------------------------------------------------
template <class nReal>
inline const nReal* CMatrix2<nReal>::operator[] (nInt32 iRow) const
{
    return &m_afEntry[2*iRow];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal* CMatrix2<nReal>::operator[] (nInt32 iRow)
{
    return &m_afEntry[2*iRow];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal CMatrix2<nReal>::operator() (nInt32 iRow, nInt32 iCol) const
{
    return m_afEntry[iCol + 2*iRow];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal& CMatrix2<nReal>::operator() (nInt32 iRow, nInt32 iCol)
{
    return m_afEntry[iCol + 2*iRow];
}
//----------------------------------------------------------------------------
template <class nReal>
void CMatrix2<nReal>::MakeZero ()
{
    m_afEntry[0] = (nReal)0.0;
    m_afEntry[1] = (nReal)0.0;
    m_afEntry[2] = (nReal)0.0;
    m_afEntry[3] = (nReal)0.0;
}
//----------------------------------------------------------------------------
template <class nReal>
void CMatrix2<nReal>::MakeIdentity ()
{
    m_afEntry[0] = (nReal)1.0;
    m_afEntry[1] = (nReal)0.0;
    m_afEntry[2] = (nReal)0.0;
    m_afEntry[3] = (nReal)1.0;
}
//----------------------------------------------------------------------------
template <class nReal>
void CMatrix2<nReal>::MakeDiagonal (nReal fM00, nReal fM11)
{
    m_afEntry[0] = fM00;
    m_afEntry[1] = (nReal)0.0;
    m_afEntry[2] = (nReal)0.0;
    m_afEntry[3] = fM11;
}
//----------------------------------------------------------------------------
template <class nReal>
void CMatrix2<nReal>::FromAngle (nReal fAngle)
{
    m_afEntry[0] = CMath<nReal>::Cos(fAngle);
    m_afEntry[2] = CMath<nReal>::Sin(fAngle);
    m_afEntry[1] = -m_afEntry[2];
    m_afEntry[3] =  m_afEntry[0];
}
//----------------------------------------------------------------------------
template <class nReal>
void CMatrix2<nReal>::MakeTensorProduct (const CVector2<nReal>& rkU,
    const CVector2<nReal>& rkV)
{
    m_afEntry[0] = rkU[0]*rkV[0];
    m_afEntry[1] = rkU[0]*rkV[1];
    m_afEntry[2] = rkU[1]*rkV[0];
    m_afEntry[3] = rkU[1]*rkV[1];
}
//----------------------------------------------------------------------------
template <class nReal>
void CMatrix2<nReal>::SetRow (nInt32 iRow, const CVector2<nReal>& rkV)
{
    nInt32 i0 = 2*iRow ,i1 = i0+1;
    m_afEntry[i0] = rkV[0];
    m_afEntry[i1] = rkV[1];
}
//----------------------------------------------------------------------------
template <class nReal>
CVector2<nReal> CMatrix2<nReal>::GetRow (nInt32 iRow) const
{
    nInt32 i0 = 2*iRow ,i1 = i0+1;
    return CVector2<nReal>(m_afEntry[i0],m_afEntry[i1]);
}
//----------------------------------------------------------------------------
template <class nReal>
void CMatrix2<nReal>::SetColumn (nInt32 iCol, const CVector2<nReal>& rkV)
{
    m_afEntry[iCol] = rkV[0];
    m_afEntry[iCol+2] = rkV[1];
}
//----------------------------------------------------------------------------
template <class nReal>
CVector2<nReal> CMatrix2<nReal>::GetColumn (nInt32 iCol) const
{
    return CVector2<nReal>(m_afEntry[iCol],m_afEntry[iCol+2]);
}
//----------------------------------------------------------------------------
template <class nReal>
void CMatrix2<nReal>::GetColumnMajor (nReal* afCMajor) const
{
    afCMajor[0] = m_afEntry[0];
    afCMajor[1] = m_afEntry[2];
    afCMajor[2] = m_afEntry[1];
    afCMajor[3] = m_afEntry[3];
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix2<nReal>& CMatrix2<nReal>::operator= (const CMatrix2& rkM)
{
    m_afEntry[0] = rkM.m_afEntry[0];
    m_afEntry[1] = rkM.m_afEntry[1];
    m_afEntry[2] = rkM.m_afEntry[2];
    m_afEntry[3] = rkM.m_afEntry[3];
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
nInt32 CMatrix2<nReal>::CompareArrays (const CMatrix2& rkM) const
{
    return memcmp(m_afEntry,rkM.m_afEntry,4*sizeof(nReal));
}
//----------------------------------------------------------------------------
template <class nReal>
bool CMatrix2<nReal>::operator== (const CMatrix2& rkM) const
{
    return CompareArrays(rkM) == 0;
}
//----------------------------------------------------------------------------
template <class nReal>
bool CMatrix2<nReal>::operator!= (const CMatrix2& rkM) const
{
    return CompareArrays(rkM) != 0;
}
//----------------------------------------------------------------------------
template <class nReal>
bool CMatrix2<nReal>::operator<  (const CMatrix2& rkM) const
{
    return CompareArrays(rkM) < 0;
}
//----------------------------------------------------------------------------
template <class nReal>
bool CMatrix2<nReal>::operator<= (const CMatrix2& rkM) const
{
    return CompareArrays(rkM) <= 0;
}
//----------------------------------------------------------------------------
template <class nReal>
bool CMatrix2<nReal>::operator>  (const CMatrix2& rkM) const
{
    return CompareArrays(rkM) > 0;
}
//----------------------------------------------------------------------------
template <class nReal>
bool CMatrix2<nReal>::operator>= (const CMatrix2& rkM) const
{
    return CompareArrays(rkM) >= 0;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix2<nReal> CMatrix2<nReal>::operator+ (const CMatrix2& rkM) const
{
    return CMatrix2<nReal>(
        m_afEntry[0] + rkM.m_afEntry[0],
        m_afEntry[1] + rkM.m_afEntry[1],
        m_afEntry[2] + rkM.m_afEntry[2],
        m_afEntry[3] + rkM.m_afEntry[3]);
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix2<nReal> CMatrix2<nReal>::operator- (const CMatrix2& rkM) const
{
    return CMatrix2<nReal>(
        m_afEntry[0] - rkM.m_afEntry[0],
        m_afEntry[1] - rkM.m_afEntry[1],
        m_afEntry[2] - rkM.m_afEntry[2],
        m_afEntry[3] - rkM.m_afEntry[3]);
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix2<nReal> CMatrix2<nReal>::operator* (const CMatrix2& rkM) const
{
    return CMatrix2<nReal>(
        m_afEntry[0]*rkM.m_afEntry[0] + m_afEntry[1]*rkM.m_afEntry[2],
        m_afEntry[0]*rkM.m_afEntry[1] + m_afEntry[1]*rkM.m_afEntry[3],
        m_afEntry[2]*rkM.m_afEntry[0] + m_afEntry[3]*rkM.m_afEntry[2],
        m_afEntry[2]*rkM.m_afEntry[1] + m_afEntry[3]*rkM.m_afEntry[3]);
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix2<nReal> CMatrix2<nReal>::operator* (nReal fScalar) const
{
    return CMatrix2<nReal>(
        fScalar*m_afEntry[0],
        fScalar*m_afEntry[1],
        fScalar*m_afEntry[2],
        fScalar*m_afEntry[3]);
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix2<nReal> CMatrix2<nReal>::operator/ (nReal fScalar) const
{
    if (fScalar != (nReal)0.0)
    {
        nReal fInvScalar = ((nReal)1.0)/fScalar;
        return CMatrix2<nReal>(
            fInvScalar*m_afEntry[0],
            fInvScalar*m_afEntry[1],
            fInvScalar*m_afEntry[2],
            fInvScalar*m_afEntry[3]);
    }

    return CMatrix2<nReal>(
        CMath<nReal>::MAX_REAL,
        CMath<nReal>::MAX_REAL,
        CMath<nReal>::MAX_REAL,
        CMath<nReal>::MAX_REAL);
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix2<nReal> CMatrix2<nReal>::operator- () const
{
    return CMatrix2<nReal>(
        -m_afEntry[0],
        -m_afEntry[1],
        -m_afEntry[2],
        -m_afEntry[3]);
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix2<nReal>& CMatrix2<nReal>::operator+= (const CMatrix2& rkM)
{
    m_afEntry[0] += rkM.m_afEntry[0];
    m_afEntry[1] += rkM.m_afEntry[1];
    m_afEntry[2] += rkM.m_afEntry[2];
    m_afEntry[3] += rkM.m_afEntry[3];
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix2<nReal>& CMatrix2<nReal>::operator-= (const CMatrix2& rkM)
{
    m_afEntry[0] -= rkM.m_afEntry[0];
    m_afEntry[1] -= rkM.m_afEntry[1];
    m_afEntry[2] -= rkM.m_afEntry[2];
    m_afEntry[3] -= rkM.m_afEntry[3];
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix2<nReal>& CMatrix2<nReal>::operator*= (nReal fScalar)
{
    m_afEntry[0] *= fScalar;
    m_afEntry[1] *= fScalar;
    m_afEntry[2] *= fScalar;
    m_afEntry[3] *= fScalar;
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix2<nReal>& CMatrix2<nReal>::operator/= (nReal fScalar)
{
    if (fScalar != (nReal)0.0)
    {
        nReal fInvScalar = ((nReal)1.0)/fScalar;
        m_afEntry[0] *= fInvScalar;
        m_afEntry[1] *= fInvScalar;
        m_afEntry[2] *= fInvScalar;
        m_afEntry[3] *= fInvScalar;
    }
    else
    {
        m_afEntry[0] = CMath<nReal>::MAX_REAL;
        m_afEntry[1] = CMath<nReal>::MAX_REAL;
        m_afEntry[2] = CMath<nReal>::MAX_REAL;
        m_afEntry[3] = CMath<nReal>::MAX_REAL;
    }

    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CVector2<nReal> CMatrix2<nReal>::operator* (const CVector2<nReal>& rkV) const
{
    return CVector2<nReal>(
        m_afEntry[0]*rkV[0] + m_afEntry[1]*rkV[1],
        m_afEntry[2]*rkV[0] + m_afEntry[3]*rkV[1]);
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix2<nReal> CMatrix2<nReal>::Transpose () const
{
    return CMatrix2<nReal>(
        m_afEntry[0],
        m_afEntry[2],
        m_afEntry[1],
        m_afEntry[3]);
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix2<nReal> CMatrix2<nReal>::TransposeTimes (const CMatrix2& rkM) const
{
    // P = A^T*B
    return CMatrix2<nReal>(
        m_afEntry[0]*rkM.m_afEntry[0] + m_afEntry[2]*rkM.m_afEntry[2],
        m_afEntry[0]*rkM.m_afEntry[1] + m_afEntry[2]*rkM.m_afEntry[3],
        m_afEntry[1]*rkM.m_afEntry[0] + m_afEntry[3]*rkM.m_afEntry[2],
        m_afEntry[1]*rkM.m_afEntry[1] + m_afEntry[3]*rkM.m_afEntry[3]);
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix2<nReal> CMatrix2<nReal>::TimesTranspose (const CMatrix2& rkM) const
{
    // P = A*B^T
    return CMatrix2<nReal>(
        m_afEntry[0]*rkM.m_afEntry[0] + m_afEntry[1]*rkM.m_afEntry[1],
        m_afEntry[0]*rkM.m_afEntry[2] + m_afEntry[1]*rkM.m_afEntry[3],
        m_afEntry[2]*rkM.m_afEntry[0] + m_afEntry[3]*rkM.m_afEntry[1],
        m_afEntry[2]*rkM.m_afEntry[2] + m_afEntry[3]*rkM.m_afEntry[3]);
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix2<nReal> CMatrix2<nReal>::Inverse () const
{
    CMatrix2<nReal> kInverse;

    nReal fDet = m_afEntry[0]*m_afEntry[3] - m_afEntry[1]*m_afEntry[2];
    if (CMath<nReal>::FAbs(fDet) > CMath<nReal>::ZERO_TOLERANCE)
    {
        nReal fInvDet = ((nReal)1.0)/fDet;
        kInverse.m_afEntry[0] =  m_afEntry[3]*fInvDet;
        kInverse.m_afEntry[1] = -m_afEntry[1]*fInvDet;
        kInverse.m_afEntry[2] = -m_afEntry[2]*fInvDet;
        kInverse.m_afEntry[3] =  m_afEntry[0]*fInvDet;
    }
    else
    {
        kInverse.m_afEntry[0] = (nReal)0.0;
        kInverse.m_afEntry[1] = (nReal)0.0;
        kInverse.m_afEntry[2] = (nReal)0.0;
        kInverse.m_afEntry[3] = (nReal)0.0;
    }

    return kInverse;
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix2<nReal> CMatrix2<nReal>::Adjoint () const
{
    return CMatrix2<nReal>(
        m_afEntry[3],
        -m_afEntry[1],
        -m_afEntry[2],
        m_afEntry[0]);
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMatrix2<nReal>::Determinant () const
{
    return m_afEntry[0]*m_afEntry[3] - m_afEntry[1]*m_afEntry[2];
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMatrix2<nReal>::QForm (const CVector2<nReal>& rkU,
    const CVector2<nReal>& rkV) const
{
    return rkU.Dot((*this)*rkV);
}
//----------------------------------------------------------------------------
template <class nReal>
void CMatrix2<nReal>::ToAngle (nReal& rfAngle) const
{
    // assert:  matrix is a rotation
    rfAngle = CMath<nReal>::ATan2(m_afEntry[2],m_afEntry[0]);
}
//----------------------------------------------------------------------------
template <class nReal>
void CMatrix2<nReal>::Orthonormalize ()
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
    nReal fInvLength = CMath<nReal>::InvSqrt(m_afEntry[0]*m_afEntry[0] +
        m_afEntry[2]*m_afEntry[2]);

    m_afEntry[0] *= fInvLength;
    m_afEntry[2] *= fInvLength;

    // compute q1
    nReal fDot0 = m_afEntry[0]*m_afEntry[1] + m_afEntry[2]*m_afEntry[3];
    m_afEntry[1] -= fDot0*m_afEntry[0];
    m_afEntry[3] -= fDot0*m_afEntry[2];

    fInvLength = CMath<nReal>::InvSqrt(m_afEntry[1]*m_afEntry[1] +
        m_afEntry[3]*m_afEntry[3]);

    m_afEntry[1] *= fInvLength;
    m_afEntry[3] *= fInvLength;
}
//----------------------------------------------------------------------------
template <class nReal>
void CMatrix2<nReal>::EigenDecomposition (CMatrix2& rkRot, CMatrix2& rkDiag) const
{
    nReal fTrace = m_afEntry[0] + m_afEntry[3];
    nReal fDiff = m_afEntry[0] - m_afEntry[3];
    nReal fDiscr = CMath<nReal>::Sqrt(fDiff*fDiff +
        ((nReal)4.0)*m_afEntry[1]*m_afEntry[1]);
    nReal fEVal0 = ((nReal)0.5)*(fTrace-fDiscr);
    nReal fEVal1 = ((nReal)0.5)*(fTrace+fDiscr);
    rkDiag.MakeDiagonal(fEVal0,fEVal1);

    nReal fCos, fSin;
    if (fDiff >= (nReal)0.0)
    {
        fCos = m_afEntry[1];
        fSin = fEVal0 - m_afEntry[0];
    }
    else
    {
        fCos = fEVal0 - m_afEntry[3];
        fSin = m_afEntry[1];
    }
    nReal fTmp = CMath<nReal>::InvSqrt(fCos*fCos + fSin*fSin);
    fCos *= fTmp;
    fSin *= fTmp;

    rkRot.m_afEntry[0] = fCos;
    rkRot.m_afEntry[1] = -fSin;
    rkRot.m_afEntry[2] = fSin;
    rkRot.m_afEntry[3] = fCos;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix2<nReal> operator* (nReal fScalar, const CMatrix2<nReal>& rkM)
{
    return rkM*fScalar;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CVector2<nReal> operator* (const CVector2<nReal>& rkV,
    const CMatrix2<nReal>& rkM)
{
    return CVector2<nReal>(
        rkV[0]*rkM[0][0] + rkV[1]*rkM[1][0],
        rkV[0]*rkM[0][1] + rkV[1]*rkM[1][1]);
}

typedef CMatrix2<nReal> Matrix2f;


}