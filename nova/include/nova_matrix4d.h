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

#include "nova_vector4d.h"
#include "nova_vector3d.h"

namespace nova
{

template <class Real>
class CMatrix4 : public CObjectConstructor
{
public:
    // If bZero is true, create the zero matrix.  Otherwise, create the
    // identity matrix.
    CMatrix4 (bool bZero = true);

    // copy constructor
    CMatrix4 (const CMatrix4& rkM);

    // input Mrc is in row r, column c.
    CMatrix4 (Real fM00, Real fM01, Real fM02, Real fM03,
             Real fM10, Real fM11, Real fM12, Real fM13,
             Real fM20, Real fM21, Real fM22, Real fM23,
             Real fM30, Real fM31, Real fM32, Real fM33);

    // Create a matrix from an array of numbers.  The input array is
    // interpreted based on the Boolean input as
    //   true:  entry[0..15]={m00,m01,m02,m03,m10,m11,m12,m13,m20,m21,m22,
    //                        m23,m30,m31,m32,m33} [row major]
    //   false: entry[0..15]={m00,m10,m20,m30,m01,m11,m21,m31,m02,m12,m22,
    //                        m32,m03,m13,m23,m33} [col major]
    CMatrix4 (const Real afEntry[16], bool bRowMajor);

    void MakeZero ();
    void MakeIdentity ();

    // member access
    inline operator const Real* () const;
    inline operator Real* ();
    inline const Real* operator[] (int iRow) const;
    inline Real* operator[] (int iRow);
    inline Real operator() (int iRow, int iCol) const;
    inline Real& operator() (int iRow, int iCol);
    void SetRow (int iRow, const CVector4<Real>& rkV);
    CVector4<Real> GetRow (int iRow) const;
    void SetColumn (int iCol, const CVector4<Real>& rkV);
    CVector4<Real> GetColumn (int iCol) const;
    void GetColumnMajor (Real* afCMajor) const;

    // assignment
    inline CMatrix4& operator= (const CMatrix4& rkM);

    // comparison
    bool operator== (const CMatrix4& rkM) const;
    bool operator!= (const CMatrix4& rkM) const;
    bool operator<  (const CMatrix4& rkM) const;
    bool operator<= (const CMatrix4& rkM) const;
    bool operator>  (const CMatrix4& rkM) const;
    bool operator>= (const CMatrix4& rkM) const;

    // arithmetic operations
    inline CMatrix4 operator+ (const CMatrix4& rkM) const;
    inline CMatrix4 operator- (const CMatrix4& rkM) const;
    inline CMatrix4 operator* (const CMatrix4& rkM) const;
    inline CMatrix4 operator* (Real fScalar) const;
    inline CMatrix4 operator/ (Real fScalar) const;
    inline CMatrix4 operator- () const;

    // arithmetic updates
    inline CMatrix4& operator+= (const CMatrix4& rkM);
    inline CMatrix4& operator-= (const CMatrix4& rkM);
    inline CMatrix4& operator*= (Real fScalar);
    inline CMatrix4& operator/= (Real fScalar);

    // matrix times vector
    inline CVector4<Real> operator* (const CVector4<Real>& rkV) const;  // M * v

    // other operations
    CMatrix4 Transpose () const;  // M^T
    CMatrix4 TransposeTimes (const CMatrix4& rkM) const;  // this^T * M
    CMatrix4 TimesTranspose (const CMatrix4& rkM) const;  // this * M^T
    CMatrix4 Inverse () const;
    CMatrix4 Adjoint () const;
    Real Determinant () const;
    Real QForm (const CVector4<Real>& rkU,
        const CVector4<Real>& rkV) const;  // u^T*M*v

    // projection matrices onto a specified plane
    void MakeObliqueProjection (const CVector3<Real>& rkNormal,
        const CVector3<Real>& rkPoint, const CVector3<Real>& rkDirection);
    void MakePerspectiveProjection (const CVector3<Real>& rkNormal,
        const CVector3<Real>& rkPoint, const CVector3<Real>& rkEye);

    // reflection matrix through a specified plane
    void MakeReflection (const CVector3<Real>& rkNormal,
        const CVector3<Real>& rkPoint);

    // special matrices
    NOVA_EXPORT static const CMatrix4 ZERO;
    NOVA_EXPORT static const CMatrix4 IDENTITY;

private:
    // support for comparisons
    int CompareArrays (const CMatrix4& rkM) const;

    Real m_afEntry[16];
};

// c * M
template <class Real>
inline CMatrix4<Real> operator* (Real fScalar, const CMatrix4<Real>& rkM);

// v^T * M
template <class Real>
inline CVector4<Real> operator* (const CVector4<Real>& rkV,
    const CMatrix4<Real>& rkM);

template <class Real>
CMatrix4<Real>::CMatrix4 (bool bZero)
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
CMatrix4<Real>::CMatrix4 (const CMatrix4& rkM)
{
    m_afEntry[ 0] = rkM.m_afEntry[ 0];
    m_afEntry[ 1] = rkM.m_afEntry[ 1];
    m_afEntry[ 2] = rkM.m_afEntry[ 2];
    m_afEntry[ 3] = rkM.m_afEntry[ 3];
    m_afEntry[ 4] = rkM.m_afEntry[ 4];
    m_afEntry[ 5] = rkM.m_afEntry[ 5];
    m_afEntry[ 6] = rkM.m_afEntry[ 6];
    m_afEntry[ 7] = rkM.m_afEntry[ 7];
    m_afEntry[ 8] = rkM.m_afEntry[ 8];
    m_afEntry[ 9] = rkM.m_afEntry[ 9];
    m_afEntry[10] = rkM.m_afEntry[10];
    m_afEntry[11] = rkM.m_afEntry[11];
    m_afEntry[12] = rkM.m_afEntry[12];
    m_afEntry[13] = rkM.m_afEntry[13];
    m_afEntry[14] = rkM.m_afEntry[14];
    m_afEntry[15] = rkM.m_afEntry[15];
}
//----------------------------------------------------------------------------
template <class Real>
CMatrix4<Real>::CMatrix4 (Real fM00, Real fM01, Real fM02, Real fM03,
    Real fM10, Real fM11, Real fM12, Real fM13, Real fM20, Real fM21,
    Real fM22, Real fM23, Real fM30, Real fM31, Real fM32, Real fM33)
{
    m_afEntry[ 0] = fM00;
    m_afEntry[ 1] = fM01;
    m_afEntry[ 2] = fM02;
    m_afEntry[ 3] = fM03;
    m_afEntry[ 4] = fM10;
    m_afEntry[ 5] = fM11;
    m_afEntry[ 6] = fM12;
    m_afEntry[ 7] = fM13;
    m_afEntry[ 8] = fM20;
    m_afEntry[ 9] = fM21;
    m_afEntry[10] = fM22;
    m_afEntry[11] = fM23;
    m_afEntry[12] = fM30;
    m_afEntry[13] = fM31;
    m_afEntry[14] = fM32;
    m_afEntry[15] = fM33;
}
//----------------------------------------------------------------------------
template <class Real>
CMatrix4<Real>::CMatrix4 (const Real afEntry[16], bool bRowMajor)
{
    if (bRowMajor)
    {
        m_afEntry[ 0] = afEntry[ 0];
        m_afEntry[ 1] = afEntry[ 1];
        m_afEntry[ 2] = afEntry[ 2];
        m_afEntry[ 3] = afEntry[ 3];
        m_afEntry[ 4] = afEntry[ 4];
        m_afEntry[ 5] = afEntry[ 5];
        m_afEntry[ 6] = afEntry[ 6];
        m_afEntry[ 7] = afEntry[ 7];
        m_afEntry[ 8] = afEntry[ 8];
        m_afEntry[ 9] = afEntry[ 9];
        m_afEntry[10] = afEntry[10];
        m_afEntry[11] = afEntry[11];
        m_afEntry[12] = afEntry[12];
        m_afEntry[13] = afEntry[13];
        m_afEntry[14] = afEntry[14];
        m_afEntry[15] = afEntry[15];
    }
    else
    {
        m_afEntry[ 0] = afEntry[ 0];
        m_afEntry[ 1] = afEntry[ 4];
        m_afEntry[ 2] = afEntry[ 8];
        m_afEntry[ 3] = afEntry[12];
        m_afEntry[ 4] = afEntry[ 1];
        m_afEntry[ 5] = afEntry[ 5];
        m_afEntry[ 6] = afEntry[ 9];
        m_afEntry[ 7] = afEntry[13];
        m_afEntry[ 8] = afEntry[ 2];
        m_afEntry[ 9] = afEntry[ 6];
        m_afEntry[10] = afEntry[10];
        m_afEntry[11] = afEntry[14];
        m_afEntry[12] = afEntry[ 3];
        m_afEntry[13] = afEntry[ 7];
        m_afEntry[14] = afEntry[11];
        m_afEntry[15] = afEntry[15];
    }
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix4<Real>::operator const Real* () const
{
    return m_afEntry;
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix4<Real>::operator Real* ()
{
    return m_afEntry;
}
//----------------------------------------------------------------------------
template <class Real>
inline const Real* CMatrix4<Real>::operator[] (int iRow) const
{
    return &m_afEntry[4*iRow];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real* CMatrix4<Real>::operator[] (int iRow)
{
    return &m_afEntry[4*iRow];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real CMatrix4<Real>::operator() (int iRow, int iCol) const
{
    return m_afEntry[iCol+4*iRow];
}
//----------------------------------------------------------------------------
template <class Real>
inline Real& CMatrix4<Real>::operator() (int iRow, int iCol)
{
    return m_afEntry[iCol+4*iRow];
}
//----------------------------------------------------------------------------
template <class Real>
void CMatrix4<Real>::MakeZero ()
{
    m_afEntry[ 0] = (Real)0.0;
    m_afEntry[ 1] = (Real)0.0;
    m_afEntry[ 2] = (Real)0.0;
    m_afEntry[ 3] = (Real)0.0;
    m_afEntry[ 4] = (Real)0.0;
    m_afEntry[ 5] = (Real)0.0;
    m_afEntry[ 6] = (Real)0.0;
    m_afEntry[ 7] = (Real)0.0;
    m_afEntry[ 8] = (Real)0.0;
    m_afEntry[ 9] = (Real)0.0;
    m_afEntry[10] = (Real)0.0;
    m_afEntry[11] = (Real)0.0;
    m_afEntry[12] = (Real)0.0;
    m_afEntry[13] = (Real)0.0;
    m_afEntry[14] = (Real)0.0;
    m_afEntry[15] = (Real)0.0;
}
//----------------------------------------------------------------------------
template <class Real>
void CMatrix4<Real>::MakeIdentity ()
{
    m_afEntry[ 0] = (Real)1.0;
    m_afEntry[ 1] = (Real)0.0;
    m_afEntry[ 2] = (Real)0.0;
    m_afEntry[ 3] = (Real)0.0;
    m_afEntry[ 4] = (Real)0.0;
    m_afEntry[ 5] = (Real)1.0;
    m_afEntry[ 6] = (Real)0.0;
    m_afEntry[ 7] = (Real)0.0;
    m_afEntry[ 8] = (Real)0.0;
    m_afEntry[ 9] = (Real)0.0;
    m_afEntry[10] = (Real)1.0;
    m_afEntry[11] = (Real)0.0;
    m_afEntry[12] = (Real)0.0;
    m_afEntry[13] = (Real)0.0;
    m_afEntry[14] = (Real)0.0;
    m_afEntry[15] = (Real)1.0;
}
//----------------------------------------------------------------------------
template <class Real>
void CMatrix4<Real>::SetRow (int iRow, const CVector4<Real>& rkV)
{
    int i0 = 4*iRow, i1 = i0+1, i2 = i1+1, i3 = i2+1;
    m_afEntry[i0] = rkV[0];
    m_afEntry[i1] = rkV[1];
    m_afEntry[i2] = rkV[2];
    m_afEntry[i3] = rkV[3];
}
//----------------------------------------------------------------------------
template <class Real>
CVector4<Real> CMatrix4<Real>::GetRow (int iRow) const
{
    int i0 = 4*iRow, i1 = i0+1, i2 = i1+1, i3 = i2+1;
    return CVector4<Real>(m_afEntry[i0],m_afEntry[i1],m_afEntry[i2],
        m_afEntry[i3]);
}
//----------------------------------------------------------------------------
template <class Real>
void CMatrix4<Real>::SetColumn (int iCol, const CVector4<Real>& rkV)
{
    m_afEntry[iCol] = rkV[0];
    m_afEntry[iCol+4] = rkV[1];
    m_afEntry[iCol+8] = rkV[2];
    m_afEntry[iCol+12] = rkV[3];
}
//----------------------------------------------------------------------------
template <class Real>
CVector4<Real> CMatrix4<Real>::GetColumn (int iCol) const
{
    return CVector4<Real>(m_afEntry[iCol],m_afEntry[iCol+4],m_afEntry[iCol+8],
        m_afEntry[iCol+12]);
}
//----------------------------------------------------------------------------
template <class Real>
void CMatrix4<Real>::GetColumnMajor (Real* afCMajor) const
{
    afCMajor[ 0] = m_afEntry[ 0];
    afCMajor[ 1] = m_afEntry[ 4];
    afCMajor[ 2] = m_afEntry[ 8];
    afCMajor[ 3] = m_afEntry[12];
    afCMajor[ 4] = m_afEntry[ 1];
    afCMajor[ 5] = m_afEntry[ 5];
    afCMajor[ 6] = m_afEntry[ 9];
    afCMajor[ 7] = m_afEntry[13];
    afCMajor[ 8] = m_afEntry[ 2];
    afCMajor[ 9] = m_afEntry[ 6];
    afCMajor[10] = m_afEntry[10];
    afCMajor[11] = m_afEntry[14];
    afCMajor[12] = m_afEntry[ 3];
    afCMajor[13] = m_afEntry[ 7];
    afCMajor[14] = m_afEntry[11];
    afCMajor[15] = m_afEntry[15];
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix4<Real>& CMatrix4<Real>::operator= (const CMatrix4& rkM)
{
    m_afEntry[ 0] = rkM.m_afEntry[ 0];
    m_afEntry[ 1] = rkM.m_afEntry[ 1];
    m_afEntry[ 2] = rkM.m_afEntry[ 2];
    m_afEntry[ 3] = rkM.m_afEntry[ 3];
    m_afEntry[ 4] = rkM.m_afEntry[ 4];
    m_afEntry[ 5] = rkM.m_afEntry[ 5];
    m_afEntry[ 6] = rkM.m_afEntry[ 6];
    m_afEntry[ 7] = rkM.m_afEntry[ 7];
    m_afEntry[ 8] = rkM.m_afEntry[ 8];
    m_afEntry[ 9] = rkM.m_afEntry[ 9];
    m_afEntry[10] = rkM.m_afEntry[10];
    m_afEntry[11] = rkM.m_afEntry[11];
    m_afEntry[12] = rkM.m_afEntry[12];
    m_afEntry[13] = rkM.m_afEntry[13];
    m_afEntry[14] = rkM.m_afEntry[14];
    m_afEntry[15] = rkM.m_afEntry[15];
    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
int CMatrix4<Real>::CompareArrays (const CMatrix4& rkM) const
{
    return memcmp(m_afEntry,rkM.m_afEntry,16*sizeof(Real));
}
//----------------------------------------------------------------------------
template <class Real>
bool CMatrix4<Real>::operator== (const CMatrix4& rkM) const
{
    return CompareArrays(rkM) == 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CMatrix4<Real>::operator!= (const CMatrix4& rkM) const
{
    return CompareArrays(rkM) != 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CMatrix4<Real>::operator<  (const CMatrix4& rkM) const
{
    return CompareArrays(rkM) < 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CMatrix4<Real>::operator<= (const CMatrix4& rkM) const
{
    return CompareArrays(rkM) <= 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CMatrix4<Real>::operator>  (const CMatrix4& rkM) const
{
    return CompareArrays(rkM) > 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool CMatrix4<Real>::operator>= (const CMatrix4& rkM) const
{
    return CompareArrays(rkM) >= 0;
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix4<Real> CMatrix4<Real>::operator+ (const CMatrix4& rkM) const
{
    return CMatrix4<Real>(
        m_afEntry[ 0] + rkM.m_afEntry[ 0],
        m_afEntry[ 1] + rkM.m_afEntry[ 1],
        m_afEntry[ 2] + rkM.m_afEntry[ 2],
        m_afEntry[ 3] + rkM.m_afEntry[ 3],
        m_afEntry[ 4] + rkM.m_afEntry[ 4],
        m_afEntry[ 5] + rkM.m_afEntry[ 5],
        m_afEntry[ 6] + rkM.m_afEntry[ 6],
        m_afEntry[ 7] + rkM.m_afEntry[ 7],
        m_afEntry[ 8] + rkM.m_afEntry[ 8],
        m_afEntry[ 9] + rkM.m_afEntry[ 9],
        m_afEntry[10] + rkM.m_afEntry[10],
        m_afEntry[11] + rkM.m_afEntry[11],
        m_afEntry[12] + rkM.m_afEntry[12],
        m_afEntry[13] + rkM.m_afEntry[13],
        m_afEntry[14] + rkM.m_afEntry[14],
        m_afEntry[15] + rkM.m_afEntry[15]);
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix4<Real> CMatrix4<Real>::operator- (const CMatrix4& rkM) const
{
    return CMatrix4<Real>(
        m_afEntry[ 0] - rkM.m_afEntry[ 0],
        m_afEntry[ 1] - rkM.m_afEntry[ 1],
        m_afEntry[ 2] - rkM.m_afEntry[ 2],
        m_afEntry[ 3] - rkM.m_afEntry[ 3],
        m_afEntry[ 4] - rkM.m_afEntry[ 4],
        m_afEntry[ 5] - rkM.m_afEntry[ 5],
        m_afEntry[ 6] - rkM.m_afEntry[ 6],
        m_afEntry[ 7] - rkM.m_afEntry[ 7],
        m_afEntry[ 8] - rkM.m_afEntry[ 8],
        m_afEntry[ 9] - rkM.m_afEntry[ 9],
        m_afEntry[10] - rkM.m_afEntry[10],
        m_afEntry[11] - rkM.m_afEntry[11],
        m_afEntry[12] - rkM.m_afEntry[12],
        m_afEntry[13] - rkM.m_afEntry[13],
        m_afEntry[14] - rkM.m_afEntry[14],
        m_afEntry[15] - rkM.m_afEntry[15]);
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix4<Real> CMatrix4<Real>::operator* (const CMatrix4& rkM) const
{
    return CMatrix4<Real>(
        m_afEntry[ 0]*rkM.m_afEntry[ 0] +
        m_afEntry[ 1]*rkM.m_afEntry[ 4] +
        m_afEntry[ 2]*rkM.m_afEntry[ 8] +
        m_afEntry[ 3]*rkM.m_afEntry[12],

        m_afEntry[ 0]*rkM.m_afEntry[ 1] +
        m_afEntry[ 1]*rkM.m_afEntry[ 5] +
        m_afEntry[ 2]*rkM.m_afEntry[ 9] +
        m_afEntry[ 3]*rkM.m_afEntry[13],

        m_afEntry[ 0]*rkM.m_afEntry[ 2] +
        m_afEntry[ 1]*rkM.m_afEntry[ 6] +
        m_afEntry[ 2]*rkM.m_afEntry[10] +
        m_afEntry[ 3]*rkM.m_afEntry[14],

        m_afEntry[ 0]*rkM.m_afEntry[ 3] +
        m_afEntry[ 1]*rkM.m_afEntry[ 7] +
        m_afEntry[ 2]*rkM.m_afEntry[11] +
        m_afEntry[ 3]*rkM.m_afEntry[15],

        m_afEntry[ 4]*rkM.m_afEntry[ 0] +
        m_afEntry[ 5]*rkM.m_afEntry[ 4] +
        m_afEntry[ 6]*rkM.m_afEntry[ 8] +
        m_afEntry[ 7]*rkM.m_afEntry[12],

        m_afEntry[ 4]*rkM.m_afEntry[ 1] +
        m_afEntry[ 5]*rkM.m_afEntry[ 5] +
        m_afEntry[ 6]*rkM.m_afEntry[ 9] +
        m_afEntry[ 7]*rkM.m_afEntry[13],

        m_afEntry[ 4]*rkM.m_afEntry[ 2] +
        m_afEntry[ 5]*rkM.m_afEntry[ 6] +
        m_afEntry[ 6]*rkM.m_afEntry[10] +
        m_afEntry[ 7]*rkM.m_afEntry[14],

        m_afEntry[ 4]*rkM.m_afEntry[ 3] +
        m_afEntry[ 5]*rkM.m_afEntry[ 7] +
        m_afEntry[ 6]*rkM.m_afEntry[11] +
        m_afEntry[ 7]*rkM.m_afEntry[15],

        m_afEntry[ 8]*rkM.m_afEntry[ 0] +
        m_afEntry[ 9]*rkM.m_afEntry[ 4] +
        m_afEntry[10]*rkM.m_afEntry[ 8] +
        m_afEntry[11]*rkM.m_afEntry[12],

        m_afEntry[ 8]*rkM.m_afEntry[ 1] +
        m_afEntry[ 9]*rkM.m_afEntry[ 5] +
        m_afEntry[10]*rkM.m_afEntry[ 9] +
        m_afEntry[11]*rkM.m_afEntry[13],

        m_afEntry[ 8]*rkM.m_afEntry[ 2] +
        m_afEntry[ 9]*rkM.m_afEntry[ 6] +
        m_afEntry[10]*rkM.m_afEntry[10] +
        m_afEntry[11]*rkM.m_afEntry[14],

        m_afEntry[ 8]*rkM.m_afEntry[ 3] +
        m_afEntry[ 9]*rkM.m_afEntry[ 7] +
        m_afEntry[10]*rkM.m_afEntry[11] +
        m_afEntry[11]*rkM.m_afEntry[15],

        m_afEntry[12]*rkM.m_afEntry[ 0] +
        m_afEntry[13]*rkM.m_afEntry[ 4] +
        m_afEntry[14]*rkM.m_afEntry[ 8] +
        m_afEntry[15]*rkM.m_afEntry[12],

        m_afEntry[12]*rkM.m_afEntry[ 1] +
        m_afEntry[13]*rkM.m_afEntry[ 5] +
        m_afEntry[14]*rkM.m_afEntry[ 9] +
        m_afEntry[15]*rkM.m_afEntry[13],

        m_afEntry[12]*rkM.m_afEntry[ 2] +
        m_afEntry[13]*rkM.m_afEntry[ 6] +
        m_afEntry[14]*rkM.m_afEntry[10] +
        m_afEntry[15]*rkM.m_afEntry[14],

        m_afEntry[12]*rkM.m_afEntry[ 3] +
        m_afEntry[13]*rkM.m_afEntry[ 7] +
        m_afEntry[14]*rkM.m_afEntry[11] +
        m_afEntry[15]*rkM.m_afEntry[15]);
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix4<Real> CMatrix4<Real>::operator* (Real fScalar) const
{
    return CMatrix4<Real>(
        fScalar*m_afEntry[ 0],
        fScalar*m_afEntry[ 1],
        fScalar*m_afEntry[ 2],
        fScalar*m_afEntry[ 3],
        fScalar*m_afEntry[ 4],
        fScalar*m_afEntry[ 5],
        fScalar*m_afEntry[ 6],
        fScalar*m_afEntry[ 7],
        fScalar*m_afEntry[ 8],
        fScalar*m_afEntry[ 9],
        fScalar*m_afEntry[10],
        fScalar*m_afEntry[11],
        fScalar*m_afEntry[12],
        fScalar*m_afEntry[13],
        fScalar*m_afEntry[14],
        fScalar*m_afEntry[15]);
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix4<Real> CMatrix4<Real>::operator/ (Real fScalar) const
{
    if (fScalar != (Real)0.0)
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        return CMatrix4<Real>(
            fInvScalar*m_afEntry[ 0],
            fInvScalar*m_afEntry[ 1],
            fInvScalar*m_afEntry[ 2],
            fInvScalar*m_afEntry[ 3],
            fInvScalar*m_afEntry[ 4],
            fInvScalar*m_afEntry[ 5],
            fInvScalar*m_afEntry[ 6],
            fInvScalar*m_afEntry[ 7],
            fInvScalar*m_afEntry[ 8],
            fInvScalar*m_afEntry[ 9],
            fInvScalar*m_afEntry[10],
            fInvScalar*m_afEntry[11],
            fInvScalar*m_afEntry[12],
            fInvScalar*m_afEntry[13],
            fInvScalar*m_afEntry[14],
            fInvScalar*m_afEntry[15]);
    }

    return CMatrix4<Real>(
        CMath<Real>::MAX_REAL,
        CMath<Real>::MAX_REAL,
        CMath<Real>::MAX_REAL,
        CMath<Real>::MAX_REAL,
        CMath<Real>::MAX_REAL,
        CMath<Real>::MAX_REAL,
        CMath<Real>::MAX_REAL,
        CMath<Real>::MAX_REAL,
        CMath<Real>::MAX_REAL,
        CMath<Real>::MAX_REAL,
        CMath<Real>::MAX_REAL,
        CMath<Real>::MAX_REAL,
        CMath<Real>::MAX_REAL,
        CMath<Real>::MAX_REAL,
        CMath<Real>::MAX_REAL,
        CMath<Real>::MAX_REAL);
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix4<Real> CMatrix4<Real>::operator- () const
{
    return CMatrix4<Real>(
        -m_afEntry[ 0],
        -m_afEntry[ 1],
        -m_afEntry[ 2],
        -m_afEntry[ 3],
        -m_afEntry[ 4],
        -m_afEntry[ 5],
        -m_afEntry[ 6],
        -m_afEntry[ 7],
        -m_afEntry[ 8],
        -m_afEntry[ 9],
        -m_afEntry[10],
        -m_afEntry[11],
        -m_afEntry[12],
        -m_afEntry[13],
        -m_afEntry[14],
        -m_afEntry[15]);
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix4<Real>& CMatrix4<Real>::operator+= (const CMatrix4& rkM)
{
    m_afEntry[ 0] += rkM.m_afEntry[ 0];
    m_afEntry[ 1] += rkM.m_afEntry[ 1];
    m_afEntry[ 2] += rkM.m_afEntry[ 2];
    m_afEntry[ 3] += rkM.m_afEntry[ 3];
    m_afEntry[ 4] += rkM.m_afEntry[ 4];
    m_afEntry[ 5] += rkM.m_afEntry[ 5];
    m_afEntry[ 6] += rkM.m_afEntry[ 6];
    m_afEntry[ 7] += rkM.m_afEntry[ 7];
    m_afEntry[ 8] += rkM.m_afEntry[ 8];
    m_afEntry[ 9] += rkM.m_afEntry[ 9];
    m_afEntry[10] += rkM.m_afEntry[10];
    m_afEntry[11] += rkM.m_afEntry[11];
    m_afEntry[12] += rkM.m_afEntry[12];
    m_afEntry[13] += rkM.m_afEntry[13];
    m_afEntry[14] += rkM.m_afEntry[14];
    m_afEntry[15] += rkM.m_afEntry[15];
    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix4<Real>& CMatrix4<Real>::operator-= (const CMatrix4& rkM)
{
    m_afEntry[ 0] -= rkM.m_afEntry[ 0];
    m_afEntry[ 1] -= rkM.m_afEntry[ 1];
    m_afEntry[ 2] -= rkM.m_afEntry[ 2];
    m_afEntry[ 3] -= rkM.m_afEntry[ 3];
    m_afEntry[ 4] -= rkM.m_afEntry[ 4];
    m_afEntry[ 5] -= rkM.m_afEntry[ 5];
    m_afEntry[ 6] -= rkM.m_afEntry[ 6];
    m_afEntry[ 7] -= rkM.m_afEntry[ 7];
    m_afEntry[ 8] -= rkM.m_afEntry[ 8];
    m_afEntry[ 9] -= rkM.m_afEntry[ 9];
    m_afEntry[10] -= rkM.m_afEntry[10];
    m_afEntry[11] -= rkM.m_afEntry[11];
    m_afEntry[12] -= rkM.m_afEntry[12];
    m_afEntry[13] -= rkM.m_afEntry[13];
    m_afEntry[14] -= rkM.m_afEntry[14];
    m_afEntry[15] -= rkM.m_afEntry[15];
    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix4<Real>& CMatrix4<Real>::operator*= (Real fScalar)
{
    m_afEntry[ 0] *= fScalar;
    m_afEntry[ 1] *= fScalar;
    m_afEntry[ 2] *= fScalar;
    m_afEntry[ 3] *= fScalar;
    m_afEntry[ 4] *= fScalar;
    m_afEntry[ 5] *= fScalar;
    m_afEntry[ 6] *= fScalar;
    m_afEntry[ 7] *= fScalar;
    m_afEntry[ 8] *= fScalar;
    m_afEntry[ 9] *= fScalar;
    m_afEntry[10] *= fScalar;
    m_afEntry[11] *= fScalar;
    m_afEntry[12] *= fScalar;
    m_afEntry[13] *= fScalar;
    m_afEntry[14] *= fScalar;
    m_afEntry[15] *= fScalar;
    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix4<Real>& CMatrix4<Real>::operator/= (Real fScalar)
{
    if (fScalar != (Real)0.0)
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        m_afEntry[ 0] *= fInvScalar;
        m_afEntry[ 1] *= fInvScalar;
        m_afEntry[ 2] *= fInvScalar;
        m_afEntry[ 3] *= fInvScalar;
        m_afEntry[ 4] *= fInvScalar;
        m_afEntry[ 5] *= fInvScalar;
        m_afEntry[ 6] *= fInvScalar;
        m_afEntry[ 7] *= fInvScalar;
        m_afEntry[ 8] *= fInvScalar;
        m_afEntry[ 9] *= fInvScalar;
        m_afEntry[10] *= fInvScalar;
        m_afEntry[11] *= fInvScalar;
        m_afEntry[12] *= fInvScalar;
        m_afEntry[13] *= fInvScalar;
        m_afEntry[14] *= fInvScalar;
        m_afEntry[15] *= fInvScalar;
    }
    else
    {
        m_afEntry[ 0] = CMath<Real>::MAX_REAL;
        m_afEntry[ 1] = CMath<Real>::MAX_REAL;
        m_afEntry[ 2] = CMath<Real>::MAX_REAL;
        m_afEntry[ 3] = CMath<Real>::MAX_REAL;
        m_afEntry[ 4] = CMath<Real>::MAX_REAL;
        m_afEntry[ 5] = CMath<Real>::MAX_REAL;
        m_afEntry[ 6] = CMath<Real>::MAX_REAL;
        m_afEntry[ 7] = CMath<Real>::MAX_REAL;
        m_afEntry[ 8] = CMath<Real>::MAX_REAL;
        m_afEntry[ 9] = CMath<Real>::MAX_REAL;
        m_afEntry[10] = CMath<Real>::MAX_REAL;
        m_afEntry[11] = CMath<Real>::MAX_REAL;
        m_afEntry[12] = CMath<Real>::MAX_REAL;
        m_afEntry[13] = CMath<Real>::MAX_REAL;
        m_afEntry[14] = CMath<Real>::MAX_REAL;
        m_afEntry[15] = CMath<Real>::MAX_REAL;
    }

    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector4<Real> CMatrix4<Real>::operator* (const CVector4<Real>& rkV) const
{
    return CVector4<Real>(
        m_afEntry[ 0]*rkV[0] +
        m_afEntry[ 1]*rkV[1] +
        m_afEntry[ 2]*rkV[2] +
        m_afEntry[ 3]*rkV[3],

        m_afEntry[ 4]*rkV[0] +
        m_afEntry[ 5]*rkV[1] +
        m_afEntry[ 6]*rkV[2] +
        m_afEntry[ 7]*rkV[3],

        m_afEntry[ 8]*rkV[0] +
        m_afEntry[ 9]*rkV[1] +
        m_afEntry[10]*rkV[2] +
        m_afEntry[11]*rkV[3],

        m_afEntry[12]*rkV[0] +
        m_afEntry[13]*rkV[1] +
        m_afEntry[14]*rkV[2] +
        m_afEntry[15]*rkV[3]);
}
//----------------------------------------------------------------------------
template <class Real>
CMatrix4<Real> CMatrix4<Real>::Transpose () const
{
    return CMatrix4<Real>(
        m_afEntry[ 0],
        m_afEntry[ 4],
        m_afEntry[ 8],
        m_afEntry[12],
        m_afEntry[ 1],
        m_afEntry[ 5],
        m_afEntry[ 9],
        m_afEntry[13],
        m_afEntry[ 2],
        m_afEntry[ 6],
        m_afEntry[10],
        m_afEntry[14],
        m_afEntry[ 3],
        m_afEntry[ 7],
        m_afEntry[11],
        m_afEntry[15]);
}
//----------------------------------------------------------------------------
template <class Real>
CMatrix4<Real> CMatrix4<Real>::TransposeTimes (const CMatrix4& rkM) const
{
    // P = A^T*B
    return CMatrix4<Real>(
        m_afEntry[ 0]*rkM.m_afEntry[ 0] +
        m_afEntry[ 4]*rkM.m_afEntry[ 4] +
        m_afEntry[ 8]*rkM.m_afEntry[ 8] +
        m_afEntry[12]*rkM.m_afEntry[12],

        m_afEntry[ 0]*rkM.m_afEntry[ 1] +
        m_afEntry[ 4]*rkM.m_afEntry[ 5] +
        m_afEntry[ 8]*rkM.m_afEntry[ 9] +
        m_afEntry[12]*rkM.m_afEntry[13],

        m_afEntry[ 0]*rkM.m_afEntry[ 2] +
        m_afEntry[ 4]*rkM.m_afEntry[ 6] +
        m_afEntry[ 8]*rkM.m_afEntry[10] +
        m_afEntry[12]*rkM.m_afEntry[14],

        m_afEntry[ 0]*rkM.m_afEntry[ 3] +
        m_afEntry[ 4]*rkM.m_afEntry[ 7] +
        m_afEntry[ 8]*rkM.m_afEntry[11] +
        m_afEntry[12]*rkM.m_afEntry[15],

        m_afEntry[ 1]*rkM.m_afEntry[ 0] +
        m_afEntry[ 5]*rkM.m_afEntry[ 4] +
        m_afEntry[ 9]*rkM.m_afEntry[ 8] +
        m_afEntry[13]*rkM.m_afEntry[12],

        m_afEntry[ 1]*rkM.m_afEntry[ 1] +
        m_afEntry[ 5]*rkM.m_afEntry[ 5] +
        m_afEntry[ 9]*rkM.m_afEntry[ 9] +
        m_afEntry[13]*rkM.m_afEntry[13],

        m_afEntry[ 1]*rkM.m_afEntry[ 2] +
        m_afEntry[ 5]*rkM.m_afEntry[ 6] +
        m_afEntry[ 9]*rkM.m_afEntry[10] +
        m_afEntry[13]*rkM.m_afEntry[14],

        m_afEntry[ 1]*rkM.m_afEntry[ 3] +
        m_afEntry[ 5]*rkM.m_afEntry[ 7] +
        m_afEntry[ 9]*rkM.m_afEntry[11] +
        m_afEntry[13]*rkM.m_afEntry[15],

        m_afEntry[ 2]*rkM.m_afEntry[ 0] +
        m_afEntry[ 6]*rkM.m_afEntry[ 4] +
        m_afEntry[10]*rkM.m_afEntry[ 8] +
        m_afEntry[14]*rkM.m_afEntry[12],

        m_afEntry[ 2]*rkM.m_afEntry[ 1] +
        m_afEntry[ 6]*rkM.m_afEntry[ 5] +
        m_afEntry[10]*rkM.m_afEntry[ 9] +
        m_afEntry[14]*rkM.m_afEntry[13],

        m_afEntry[ 2]*rkM.m_afEntry[ 2] +
        m_afEntry[ 6]*rkM.m_afEntry[ 6] +
        m_afEntry[10]*rkM.m_afEntry[10] +
        m_afEntry[14]*rkM.m_afEntry[14],

        m_afEntry[ 2]*rkM.m_afEntry[ 3] +
        m_afEntry[ 6]*rkM.m_afEntry[ 7] +
        m_afEntry[10]*rkM.m_afEntry[11] +
        m_afEntry[14]*rkM.m_afEntry[15],

        m_afEntry[ 3]*rkM.m_afEntry[ 0] +
        m_afEntry[ 7]*rkM.m_afEntry[ 4] +
        m_afEntry[11]*rkM.m_afEntry[ 8] +
        m_afEntry[15]*rkM.m_afEntry[12],

        m_afEntry[ 3]*rkM.m_afEntry[ 1] +
        m_afEntry[ 7]*rkM.m_afEntry[ 5] +
        m_afEntry[11]*rkM.m_afEntry[ 9] +
        m_afEntry[15]*rkM.m_afEntry[13],

        m_afEntry[ 3]*rkM.m_afEntry[ 2] +
        m_afEntry[ 7]*rkM.m_afEntry[ 6] +
        m_afEntry[11]*rkM.m_afEntry[10] +
        m_afEntry[15]*rkM.m_afEntry[14],

        m_afEntry[ 3]*rkM.m_afEntry[ 3] +
        m_afEntry[ 7]*rkM.m_afEntry[ 7] +
        m_afEntry[11]*rkM.m_afEntry[11] +
        m_afEntry[15]*rkM.m_afEntry[15]);
}
//----------------------------------------------------------------------------
template <class Real>
CMatrix4<Real> CMatrix4<Real>::TimesTranspose (const CMatrix4& rkM) const
{
    // P = A*B^T
    return CMatrix4<Real>(
        m_afEntry[ 0]*rkM.m_afEntry[ 0] +
        m_afEntry[ 1]*rkM.m_afEntry[ 1] +
        m_afEntry[ 2]*rkM.m_afEntry[ 2] +
        m_afEntry[ 3]*rkM.m_afEntry[ 3],

        m_afEntry[ 0]*rkM.m_afEntry[ 4] +
        m_afEntry[ 1]*rkM.m_afEntry[ 5] +
        m_afEntry[ 2]*rkM.m_afEntry[ 6] +
        m_afEntry[ 3]*rkM.m_afEntry[ 7],

        m_afEntry[ 0]*rkM.m_afEntry[ 8] +
        m_afEntry[ 1]*rkM.m_afEntry[ 9] +
        m_afEntry[ 2]*rkM.m_afEntry[10] +
        m_afEntry[ 3]*rkM.m_afEntry[11],

        m_afEntry[ 0]*rkM.m_afEntry[12] +
        m_afEntry[ 1]*rkM.m_afEntry[13] +
        m_afEntry[ 2]*rkM.m_afEntry[14] +
        m_afEntry[ 3]*rkM.m_afEntry[15],

        m_afEntry[ 4]*rkM.m_afEntry[ 0] +
        m_afEntry[ 5]*rkM.m_afEntry[ 1] +
        m_afEntry[ 6]*rkM.m_afEntry[ 2] +
        m_afEntry[ 7]*rkM.m_afEntry[ 3],

        m_afEntry[ 4]*rkM.m_afEntry[ 4] +
        m_afEntry[ 5]*rkM.m_afEntry[ 5] +
        m_afEntry[ 6]*rkM.m_afEntry[ 6] +
        m_afEntry[ 7]*rkM.m_afEntry[ 7],

        m_afEntry[ 4]*rkM.m_afEntry[ 8] +
        m_afEntry[ 5]*rkM.m_afEntry[ 9] +
        m_afEntry[ 6]*rkM.m_afEntry[10] +
        m_afEntry[ 7]*rkM.m_afEntry[11],

        m_afEntry[ 4]*rkM.m_afEntry[12] +
        m_afEntry[ 5]*rkM.m_afEntry[13] +
        m_afEntry[ 6]*rkM.m_afEntry[14] +
        m_afEntry[ 7]*rkM.m_afEntry[15],

        m_afEntry[ 8]*rkM.m_afEntry[ 0] +
        m_afEntry[ 9]*rkM.m_afEntry[ 1] +
        m_afEntry[10]*rkM.m_afEntry[ 2] +
        m_afEntry[11]*rkM.m_afEntry[ 3],

        m_afEntry[ 8]*rkM.m_afEntry[ 4] +
        m_afEntry[ 9]*rkM.m_afEntry[ 5] +
        m_afEntry[10]*rkM.m_afEntry[ 6] +
        m_afEntry[11]*rkM.m_afEntry[ 7],

        m_afEntry[ 8]*rkM.m_afEntry[ 8] +
        m_afEntry[ 9]*rkM.m_afEntry[ 9] +
        m_afEntry[10]*rkM.m_afEntry[10] +
        m_afEntry[11]*rkM.m_afEntry[11],

        m_afEntry[ 8]*rkM.m_afEntry[12] +
        m_afEntry[ 9]*rkM.m_afEntry[13] +
        m_afEntry[10]*rkM.m_afEntry[14] +
        m_afEntry[11]*rkM.m_afEntry[15],

        m_afEntry[12]*rkM.m_afEntry[ 0] +
        m_afEntry[13]*rkM.m_afEntry[ 1] +
        m_afEntry[14]*rkM.m_afEntry[ 2] +
        m_afEntry[15]*rkM.m_afEntry[ 3],

        m_afEntry[12]*rkM.m_afEntry[ 4] +
        m_afEntry[13]*rkM.m_afEntry[ 5] +
        m_afEntry[14]*rkM.m_afEntry[ 6] +
        m_afEntry[15]*rkM.m_afEntry[ 7],

        m_afEntry[12]*rkM.m_afEntry[ 8] +
        m_afEntry[13]*rkM.m_afEntry[ 9] +
        m_afEntry[14]*rkM.m_afEntry[10] +
        m_afEntry[15]*rkM.m_afEntry[11],

        m_afEntry[12]*rkM.m_afEntry[12] +
        m_afEntry[13]*rkM.m_afEntry[13] +
        m_afEntry[14]*rkM.m_afEntry[14] +
        m_afEntry[15]*rkM.m_afEntry[15]);
}
//----------------------------------------------------------------------------
template <class Real>
CMatrix4<Real> CMatrix4<Real>::Inverse () const
{
    Real fA0 = m_afEntry[ 0]*m_afEntry[ 5] - m_afEntry[ 1]*m_afEntry[ 4];
    Real fA1 = m_afEntry[ 0]*m_afEntry[ 6] - m_afEntry[ 2]*m_afEntry[ 4];
    Real fA2 = m_afEntry[ 0]*m_afEntry[ 7] - m_afEntry[ 3]*m_afEntry[ 4];
    Real fA3 = m_afEntry[ 1]*m_afEntry[ 6] - m_afEntry[ 2]*m_afEntry[ 5];
    Real fA4 = m_afEntry[ 1]*m_afEntry[ 7] - m_afEntry[ 3]*m_afEntry[ 5];
    Real fA5 = m_afEntry[ 2]*m_afEntry[ 7] - m_afEntry[ 3]*m_afEntry[ 6];
    Real fB0 = m_afEntry[ 8]*m_afEntry[13] - m_afEntry[ 9]*m_afEntry[12];
    Real fB1 = m_afEntry[ 8]*m_afEntry[14] - m_afEntry[10]*m_afEntry[12];
    Real fB2 = m_afEntry[ 8]*m_afEntry[15] - m_afEntry[11]*m_afEntry[12];
    Real fB3 = m_afEntry[ 9]*m_afEntry[14] - m_afEntry[10]*m_afEntry[13];
    Real fB4 = m_afEntry[ 9]*m_afEntry[15] - m_afEntry[11]*m_afEntry[13];
    Real fB5 = m_afEntry[10]*m_afEntry[15] - m_afEntry[11]*m_afEntry[14];

    Real fDet = fA0*fB5-fA1*fB4+fA2*fB3+fA3*fB2-fA4*fB1+fA5*fB0;
    if (CMath<Real>::FAbs(fDet) <= CMath<Real>::ZERO_TOLERANCE)
    {
        return CMatrix4<Real>::ZERO;
    }

    CMatrix4 kInv;
    kInv.m_afEntry[ 0] =
        + m_afEntry[ 5]*fB5 - m_afEntry[ 6]*fB4 + m_afEntry[ 7]*fB3;
    kInv.m_afEntry[ 4] =
        - m_afEntry[ 4]*fB5 + m_afEntry[ 6]*fB2 - m_afEntry[ 7]*fB1;
    kInv.m_afEntry[ 8] =
        + m_afEntry[ 4]*fB4 - m_afEntry[ 5]*fB2 + m_afEntry[ 7]*fB0;
    kInv.m_afEntry[12] =
        - m_afEntry[ 4]*fB3 + m_afEntry[ 5]*fB1 - m_afEntry[ 6]*fB0;
    kInv.m_afEntry[ 1] =
        - m_afEntry[ 1]*fB5 + m_afEntry[ 2]*fB4 - m_afEntry[ 3]*fB3;
    kInv.m_afEntry[ 5] =
        + m_afEntry[ 0]*fB5 - m_afEntry[ 2]*fB2 + m_afEntry[ 3]*fB1;
    kInv.m_afEntry[ 9] =
        - m_afEntry[ 0]*fB4 + m_afEntry[ 1]*fB2 - m_afEntry[ 3]*fB0;
    kInv.m_afEntry[13] =
        + m_afEntry[ 0]*fB3 - m_afEntry[ 1]*fB1 + m_afEntry[ 2]*fB0;
    kInv.m_afEntry[ 2] =
        + m_afEntry[13]*fA5 - m_afEntry[14]*fA4 + m_afEntry[15]*fA3;
    kInv.m_afEntry[ 6] =
        - m_afEntry[12]*fA5 + m_afEntry[14]*fA2 - m_afEntry[15]*fA1;
    kInv.m_afEntry[10] =
        + m_afEntry[12]*fA4 - m_afEntry[13]*fA2 + m_afEntry[15]*fA0;
    kInv.m_afEntry[14] =
        - m_afEntry[12]*fA3 + m_afEntry[13]*fA1 - m_afEntry[14]*fA0;
    kInv.m_afEntry[ 3] =
        - m_afEntry[ 9]*fA5 + m_afEntry[10]*fA4 - m_afEntry[11]*fA3;
    kInv.m_afEntry[ 7] =
        + m_afEntry[ 8]*fA5 - m_afEntry[10]*fA2 + m_afEntry[11]*fA1;
    kInv.m_afEntry[11] =
        - m_afEntry[ 8]*fA4 + m_afEntry[ 9]*fA2 - m_afEntry[11]*fA0;
    kInv.m_afEntry[15] =
        + m_afEntry[ 8]*fA3 - m_afEntry[ 9]*fA1 + m_afEntry[10]*fA0;

    Real fInvDet = ((Real)1.0)/fDet;
    kInv.m_afEntry[ 0] *= fInvDet;
    kInv.m_afEntry[ 1] *= fInvDet;
    kInv.m_afEntry[ 2] *= fInvDet;
    kInv.m_afEntry[ 3] *= fInvDet;
    kInv.m_afEntry[ 4] *= fInvDet;
    kInv.m_afEntry[ 5] *= fInvDet;
    kInv.m_afEntry[ 6] *= fInvDet;
    kInv.m_afEntry[ 7] *= fInvDet;
    kInv.m_afEntry[ 8] *= fInvDet;
    kInv.m_afEntry[ 9] *= fInvDet;
    kInv.m_afEntry[10] *= fInvDet;
    kInv.m_afEntry[11] *= fInvDet;
    kInv.m_afEntry[12] *= fInvDet;
    kInv.m_afEntry[13] *= fInvDet;
    kInv.m_afEntry[14] *= fInvDet;
    kInv.m_afEntry[15] *= fInvDet;

    return kInv;
}
//----------------------------------------------------------------------------
template <class Real>
CMatrix4<Real> CMatrix4<Real>::Adjoint () const
{
    Real fA0 = m_afEntry[ 0]*m_afEntry[ 5] - m_afEntry[ 1]*m_afEntry[ 4];
    Real fA1 = m_afEntry[ 0]*m_afEntry[ 6] - m_afEntry[ 2]*m_afEntry[ 4];
    Real fA2 = m_afEntry[ 0]*m_afEntry[ 7] - m_afEntry[ 3]*m_afEntry[ 4];
    Real fA3 = m_afEntry[ 1]*m_afEntry[ 6] - m_afEntry[ 2]*m_afEntry[ 5];
    Real fA4 = m_afEntry[ 1]*m_afEntry[ 7] - m_afEntry[ 3]*m_afEntry[ 5];
    Real fA5 = m_afEntry[ 2]*m_afEntry[ 7] - m_afEntry[ 3]*m_afEntry[ 6];
    Real fB0 = m_afEntry[ 8]*m_afEntry[13] - m_afEntry[ 9]*m_afEntry[12];
    Real fB1 = m_afEntry[ 8]*m_afEntry[14] - m_afEntry[10]*m_afEntry[12];
    Real fB2 = m_afEntry[ 8]*m_afEntry[15] - m_afEntry[11]*m_afEntry[12];
    Real fB3 = m_afEntry[ 9]*m_afEntry[14] - m_afEntry[10]*m_afEntry[13];
    Real fB4 = m_afEntry[ 9]*m_afEntry[15] - m_afEntry[11]*m_afEntry[13];
    Real fB5 = m_afEntry[10]*m_afEntry[15] - m_afEntry[11]*m_afEntry[14];

    return CMatrix4<Real>(
        + m_afEntry[ 5]*fB5 - m_afEntry[ 6]*fB4 + m_afEntry[ 7]*fB3,
        - m_afEntry[ 1]*fB5 + m_afEntry[ 2]*fB4 - m_afEntry[ 3]*fB3,
        + m_afEntry[13]*fA5 - m_afEntry[14]*fA4 + m_afEntry[15]*fA3,
        - m_afEntry[ 9]*fA5 + m_afEntry[10]*fA4 - m_afEntry[11]*fA3,
        - m_afEntry[ 4]*fB5 + m_afEntry[ 6]*fB2 - m_afEntry[ 7]*fB1,
        + m_afEntry[ 0]*fB5 - m_afEntry[ 2]*fB2 + m_afEntry[ 3]*fB1,
        - m_afEntry[12]*fA5 + m_afEntry[14]*fA2 - m_afEntry[15]*fA1,
        + m_afEntry[ 8]*fA5 - m_afEntry[10]*fA2 + m_afEntry[11]*fA1,
        + m_afEntry[ 4]*fB4 - m_afEntry[ 5]*fB2 + m_afEntry[ 7]*fB0,
        - m_afEntry[ 0]*fB4 + m_afEntry[ 1]*fB2 - m_afEntry[ 3]*fB0,
        + m_afEntry[12]*fA4 - m_afEntry[13]*fA2 + m_afEntry[15]*fA0,
        - m_afEntry[ 8]*fA4 + m_afEntry[ 9]*fA2 - m_afEntry[11]*fA0,
        - m_afEntry[ 4]*fB3 + m_afEntry[ 5]*fB1 - m_afEntry[ 6]*fB0,
        + m_afEntry[ 0]*fB3 - m_afEntry[ 1]*fB1 + m_afEntry[ 2]*fB0,
        - m_afEntry[12]*fA3 + m_afEntry[13]*fA1 - m_afEntry[14]*fA0,
        + m_afEntry[ 8]*fA3 - m_afEntry[ 9]*fA1 + m_afEntry[10]*fA0);
}
//----------------------------------------------------------------------------
template <class Real>
Real CMatrix4<Real>::Determinant () const
{
    Real fA0 = m_afEntry[ 0]*m_afEntry[ 5] - m_afEntry[ 1]*m_afEntry[ 4];
    Real fA1 = m_afEntry[ 0]*m_afEntry[ 6] - m_afEntry[ 2]*m_afEntry[ 4];
    Real fA2 = m_afEntry[ 0]*m_afEntry[ 7] - m_afEntry[ 3]*m_afEntry[ 4];
    Real fA3 = m_afEntry[ 1]*m_afEntry[ 6] - m_afEntry[ 2]*m_afEntry[ 5];
    Real fA4 = m_afEntry[ 1]*m_afEntry[ 7] - m_afEntry[ 3]*m_afEntry[ 5];
    Real fA5 = m_afEntry[ 2]*m_afEntry[ 7] - m_afEntry[ 3]*m_afEntry[ 6];
    Real fB0 = m_afEntry[ 8]*m_afEntry[13] - m_afEntry[ 9]*m_afEntry[12];
    Real fB1 = m_afEntry[ 8]*m_afEntry[14] - m_afEntry[10]*m_afEntry[12];
    Real fB2 = m_afEntry[ 8]*m_afEntry[15] - m_afEntry[11]*m_afEntry[12];
    Real fB3 = m_afEntry[ 9]*m_afEntry[14] - m_afEntry[10]*m_afEntry[13];
    Real fB4 = m_afEntry[ 9]*m_afEntry[15] - m_afEntry[11]*m_afEntry[13];
    Real fB5 = m_afEntry[10]*m_afEntry[15] - m_afEntry[11]*m_afEntry[14];
    Real fDet = fA0*fB5-fA1*fB4+fA2*fB3+fA3*fB2-fA4*fB1+fA5*fB0;
    return fDet;
}
//----------------------------------------------------------------------------
template <class Real>
Real CMatrix4<Real>::QForm (const CVector4<Real>& rkU,
    const CVector4<Real>& rkV) const
{
    return rkU.Dot((*this)*rkV);
}
//----------------------------------------------------------------------------
template <class Real>
void CMatrix4<Real>::MakeObliqueProjection (const CVector3<Real>& rkNormal,
    const CVector3<Real>& rkPoint, const CVector3<Real>& rkDirection)
{
    // The projection plane is Dot(N,X-P) = 0 where N is a 3-by-1 unit-length
    // normal vector and P is a 3-by-1 point on the plane.  The projection
    // is oblique to the plane, in the direction of the 3-by-1 vector D.
    // Necessarily Dot(N,D) is not zero for this projection to make sense.
    // Given a 3-by-1 point U, compute the intersection of the line U+t*D
    // with the plane to obtain t = -Dot(N,U-P)/Dot(N,D).  Then
    //
    //   projection(U) = P + [I - D*N^T/Dot(N,D)]*(U-P)
    //
    // A 4-by-4 homogeneous transformation representing the projection is
    //
    //       +-                               -+
    //   M = | D*N^T - Dot(N,D)*I   -Dot(N,P)D |
    //       |          0^T          -Dot(N,D) |
    //       +-                               -+
    //
    // where M applies to [U^T 1]^T by M*[U^T 1]^T.  The matrix is chosen so
    // that M[3][3] > 0 whenever Dot(N,D) < 0 (projection is onto the
    // "positive side" of the plane).

    Real fNdD = rkNormal.Dot(rkDirection);
    Real fNdP = rkNormal.Dot(rkPoint);
    m_afEntry[ 0] = rkDirection[0]*rkNormal[0] - fNdD;
    m_afEntry[ 1] = rkDirection[0]*rkNormal[1];
    m_afEntry[ 2] = rkDirection[0]*rkNormal[2];
    m_afEntry[ 3] = -fNdP*rkDirection[0];
    m_afEntry[ 4] = rkDirection[1]*rkNormal[0];
    m_afEntry[ 5] = rkDirection[1]*rkNormal[1] - fNdD;
    m_afEntry[ 6] = rkDirection[1]*rkNormal[2];
    m_afEntry[ 7] = -fNdP*rkDirection[1];
    m_afEntry[ 8] = rkDirection[2]*rkNormal[0];
    m_afEntry[ 9] = rkDirection[2]*rkNormal[1];
    m_afEntry[10] = rkDirection[2]*rkNormal[2] - fNdD;
    m_afEntry[11] = -fNdP*rkDirection[2];
    m_afEntry[12] = 0.0f;
    m_afEntry[13] = 0.0f;
    m_afEntry[14] = 0.0f;
    m_afEntry[15] = -fNdD;
}
//----------------------------------------------------------------------------
template <class Real>
void CMatrix4<Real>::MakePerspectiveProjection (const CVector3<Real>& rkNormal,
    const CVector3<Real>& rkPoint, const CVector3<Real>& rkEye)
{
    //     +-                                                 -+
    // M = | Dot(N,E-P)*I - E*N^T    -(Dot(N,E-P)*I - E*N^T)*E |
    //     |        -N^t                      Dot(N,E)         |
    //     +-                                                 -+
    //
    // where E is the eye point, P is a point on the plane, and N is a
    // unit-length plane normal.

    Real fNdEmP = rkNormal.Dot(rkEye-rkPoint);

    m_afEntry[ 0] = fNdEmP - rkEye[0]*rkNormal[0];
    m_afEntry[ 1] = -rkEye[0]*rkNormal[1];
    m_afEntry[ 2] = -rkEye[0]*rkNormal[2];
    m_afEntry[ 3] = -(m_afEntry[0]*rkEye[0] + m_afEntry[1]*rkEye[1] +
        m_afEntry[2]*rkEye[2]);
    m_afEntry[ 4] = -rkEye[1]*rkNormal[0];
    m_afEntry[ 5] = fNdEmP - rkEye[1]*rkNormal[1];
    m_afEntry[ 6] = -rkEye[1]*rkNormal[2];
    m_afEntry[ 7] = -(m_afEntry[4]*rkEye[0] + m_afEntry[5]*rkEye[1] +
        m_afEntry[6]*rkEye[2]);
    m_afEntry[ 8] = -rkEye[2]*rkNormal[0];
    m_afEntry[ 9] = -rkEye[2]*rkNormal[1];
    m_afEntry[10] = fNdEmP- rkEye[2]*rkNormal[2];
    m_afEntry[11] = -(m_afEntry[8]*rkEye[0] + m_afEntry[9]*rkEye[1] +
        m_afEntry[10]*rkEye[2]);
    m_afEntry[12] = -rkNormal[0];
    m_afEntry[13] = -rkNormal[1];
    m_afEntry[14] = -rkNormal[2];
    m_afEntry[15] = rkNormal.Dot(rkEye);
}
//----------------------------------------------------------------------------
template <class Real>
void CMatrix4<Real>::MakeReflection (const CVector3<Real>& rkNormal,
    const CVector3<Real>& rkPoint)
{
    //     +-                         -+
    // M = | I-2*N*N^T    2*Dot(N,P)*N |
    //     |     0^T            1      |
    //     +-                         -+
    //
    // where P is a point on the plane and N is a unit-length plane normal.

    Real fTwoNdP = ((Real)2.0)*(rkNormal.Dot(rkPoint));

    m_afEntry[ 0] = (Real)1.0 - ((Real)2.0)*rkNormal[0]*rkNormal[0];
    m_afEntry[ 1] = -((Real)2.0)*rkNormal[0]*rkNormal[1];
    m_afEntry[ 2] = -((Real)2.0)*rkNormal[0]*rkNormal[2];
    m_afEntry[ 3] = fTwoNdP*rkNormal[0];
    m_afEntry[ 4] = -((Real)2.0)*rkNormal[1]*rkNormal[0];
    m_afEntry[ 5] = (Real)1.0 - ((Real)2.0)*rkNormal[1]*rkNormal[1];
    m_afEntry[ 6] = -((Real)2.0)*rkNormal[1]*rkNormal[2];
    m_afEntry[ 7] = fTwoNdP*rkNormal[1];
    m_afEntry[ 8] = -((Real)2.0)*rkNormal[2]*rkNormal[0];
    m_afEntry[ 9] = -((Real)2.0)*rkNormal[2]*rkNormal[1];
    m_afEntry[10] = (Real)1.0 - ((Real)2.0)*rkNormal[2]*rkNormal[2];
    m_afEntry[11] = fTwoNdP*rkNormal[2];
    m_afEntry[12] = (Real)0.0;
    m_afEntry[13] = (Real)0.0;
    m_afEntry[14] = (Real)0.0;
    m_afEntry[15] = (Real)1.0;
}
//----------------------------------------------------------------------------
template <class Real>
inline CMatrix4<Real> operator* (Real fScalar, const CMatrix4<Real>& rkM)
{
    return rkM*fScalar;
}
//----------------------------------------------------------------------------
template <class Real>
inline CVector4<Real> operator* (const CVector4<Real>& rkV,
    const CMatrix4<Real>& rkM)
{
    return CVector4<Real>(
        rkV[0]*rkM[0][0]+rkV[1]*rkM[1][0]+rkV[2]*rkM[2][0]+rkV[3]*rkM[3][0],
        rkV[0]*rkM[0][1]+rkV[1]*rkM[1][1]+rkV[2]*rkM[2][1]+rkV[3]*rkM[3][1],
        rkV[0]*rkM[0][2]+rkV[1]*rkM[1][2]+rkV[2]*rkM[2][2]+rkV[3]*rkM[3][2],
        rkV[0]*rkM[0][3]+rkV[1]*rkM[1][3]+rkV[2]*rkM[2][3]+rkV[3]*rkM[3][3]);
}


typedef CMatrix4<real> Matrix4f;


}