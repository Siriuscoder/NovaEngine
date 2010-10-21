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

#include "nova_vector3d.h"

namespace nova
{

template <class nReal>
class CMatrix3: public CObjectConstructor
{
public:
    // If bZero is true, create the zero matrix.  Otherwise, create the
    // identity matrix.
    CMatrix3 (bool bZero = true);

    // copy constructor
    CMatrix3 (const CMatrix3& rkM);

    // input Mrc is in row r, column c.
    CMatrix3 (nReal fM00, nReal fM01, nReal fM02,
             nReal fM10, nReal fM11, nReal fM12,
             nReal fM20, nReal fM21, nReal fM22);

    // Create a matrix from an array of numbers.  The input array is
    // interpreted based on the Boolean input as
    //   true:  entry[0..8]={m00,m01,m02,m10,m11,m12,m20,m21,m22} [row major]
    //   false: entry[0..8]={m00,m10,m20,m01,m11,m21,m02,m12,m22} [col major]
    CMatrix3 (const nReal afEntry[9], bool bRowMajor);

    // Create matrices based on vector input.  The Boolean is interpreted as
    //   true: vectors are columns of the matrix
    //   false: vectors are rows of the matrix
    CMatrix3 (const CVector3<nReal>& rkU, const CVector3<nReal>& rkV,
        const CVector3<nReal>& rkW, bool bColumns);
    CMatrix3 (const CVector3<nReal>* akV, bool bColumns);

    // create a diagonal matrix
    CMatrix3 (nReal fM00, nReal fM11, nReal fM22);

    // Create rotation matrices (positive angle - counterclockwise).  The
    // angle must be in radians, not degrees.
    CMatrix3 (const CVector3<nReal>& rkAxis, nReal fAngle);

    // create a tensor product U*V^T
    CMatrix3 (const CVector3<nReal>& rkU, const CVector3<nReal>& rkV);

    // create various matrices
    CMatrix3& MakeZero ();
    CMatrix3& MakeIdentity ();
    CMatrix3& MakeDiagonal (nReal fM00, nReal fM11, nReal fM22);
    CMatrix3& FromAxisAngle (const CVector3<nReal>& rkAxis, nReal fAngle);
    CMatrix3& MakeTensorProduct (const CVector3<nReal>& rkU,
        const CVector3<nReal>& rkV);

    // member access
    inline operator const nReal* () const;
    inline operator nReal* ();
    inline const nReal* operator[] (nInt32 iRow) const;
    inline nReal* operator[] (nInt32 iRow);
    inline nReal operator() (nInt32 iRow, nInt32 iCol) const;
    inline nReal& operator() (nInt32 iRow, nInt32 iCol);
    void SetRow (nInt32 iRow, const CVector3<nReal>& rkV);
    CVector3<nReal> GetRow (nInt32 iRow) const;
    void SetColumn (nInt32 iCol, const CVector3<nReal>& rkV);
    CVector3<nReal> GetColumn (nInt32 iCol) const;
    void GetColumnMajor (nReal* afCMajor) const;

    // assignment
    inline CMatrix3& operator= (const CMatrix3& rkM);

    // comparison
    bool operator== (const CMatrix3& rkM) const;
    bool operator!= (const CMatrix3& rkM) const;
    bool operator<  (const CMatrix3& rkM) const;
    bool operator<= (const CMatrix3& rkM) const;
    bool operator>  (const CMatrix3& rkM) const;
    bool operator>= (const CMatrix3& rkM) const;

    // arithmetic operations
    inline CMatrix3 operator+ (const CMatrix3& rkM) const;
    inline CMatrix3 operator- (const CMatrix3& rkM) const;
    inline CMatrix3 operator* (const CMatrix3& rkM) const;
    inline CMatrix3 operator* (nReal fScalar) const;
    inline CMatrix3 operator/ (nReal fScalar) const;
    inline CMatrix3 operator- () const;

    // arithmetic updates
    inline CMatrix3& operator+= (const CMatrix3& rkM);
    inline CMatrix3& operator-= (const CMatrix3& rkM);
    inline CMatrix3& operator*= (nReal fScalar);
    inline CMatrix3& operator/= (nReal fScalar);

    // matrix times vector
    inline CVector3<nReal> operator* (const CVector3<nReal>& rkV) const;  // M * v

    // other operations
    CMatrix3 Transpose () const;  // M^T
    CMatrix3 TransposeTimes (const CMatrix3& rkM) const;  // this^T * M
    CMatrix3 TimesTranspose (const CMatrix3& rkM) const;  // this * M^T
    CMatrix3 Inverse () const;
    CMatrix3 Adjoint () const;
    nReal Determinant () const;
    nReal QForm (const CVector3<nReal>& rkU,
        const CVector3<nReal>& rkV) const;  // u^T*M*v
    CMatrix3 TimesDiagonal (const CVector3<nReal>& rkDiag) const;  // M*D
    CMatrix3 DiagonalTimes (const CVector3<nReal>& rkDiag) const;  // D*M

    // The matrix must be a rotation for these functions to be valid.  The
    // last function uses Gram-Schmidt orthonormalization applied to the
    // columns of the rotation matrix.  The angle must be in radians, not
    // degrees.
    void ToAxisAngle (CVector3<nReal>& rkAxis, nReal& rfAngle) const;
    void Orthonormalize ();

    // The matrix must be symmetric.  Factor M = R * D * R^T where
    // R = [u0|u1|u2] is a rotation matrix with columns u0, u1, and u2 and
    // D = diag(d0,d1,d2) is a diagonal matrix whose diagonal entries are d0,
    // d1, and d2.  The eigenvector u[i] corresponds to eigenvector d[i].
    // The eigenvalues are ordered as d0 <= d1 <= d2.
    void EigenDecomposition (CMatrix3& rkRot, CMatrix3& rkDiag) const;

    // Create rotation matrices from Euler angles.
    CMatrix3& FromEulerAnglesXYZ (nReal fXAngle, nReal fYAngle, nReal fZAngle);
    CMatrix3& FromEulerAnglesXZY (nReal fXAngle, nReal fZAngle, nReal fYAngle);
    CMatrix3& FromEulerAnglesYXZ (nReal fYAngle, nReal fXAngle, nReal fZAngle);
    CMatrix3& FromEulerAnglesYZX (nReal fYAngle, nReal fZAngle, nReal fXAngle);
    CMatrix3& FromEulerAnglesZXY (nReal fZAngle, nReal fXAngle, nReal fYAngle);
    CMatrix3& FromEulerAnglesZYX (nReal fZAngle, nReal fYAngle, nReal fXAngle);

    // TODO.  From EulerAnglesUVU for all combinations of U and V.

    // Extract Euler angles from rotation matrices.
    enum EulerResult
    {
        // The solution is unique.
        EA_UNIQUE,

        // The solution is not unique.  A sum of angles is constant.
        EA_NOT_UNIQUE_SUM,

        // The solution is not unique.  A difference of angles is constant.
        EA_NOT_UNIQUE_DIF
    };

    // The return values are in the specified ranges:
    //   xAngle in [-pi,pi], yAngle in [-pi/2,pi/2], zAngle in [-pi,pi]
    // When the solution is not unique, zAngle = 0 is returned.  Generally,
    // the set of solutions is
    //   EA_NOT_UNIQUE_SUM:  zAngle + xAngle = c
    //   EA_NOT_UNIQUE_DIF:  zAngle - xAngle = c
    // for some angle c.
    EulerResult ToEulerAnglesXYZ (nReal& rfXAngle, nReal& rfYAngle,
        nReal& rfZAngle) const;

    // The return values are in the specified ranges:
    //   xAngle in [-pi,pi], zAngle in [-pi/2,pi/2], yAngle in [-pi,pi]
    // When the solution is not unique, yAngle = 0 is returned.  Generally,
    // the set of solutions is
    //   EA_NOT_UNIQUE_SUM:  yAngle + xAngle = c
    //   EA_NOT_UNIQUE_DIF:  yAngle - xAngle = c
    // for some angle c.
    EulerResult ToEulerAnglesXZY (nReal& rfXAngle, nReal& rfZAngle,
        nReal& rfYAngle) const;

    // The return values are in the specified ranges:
    //   yAngle in [-pi,pi], xAngle in [-pi/2,pi/2], zAngle in [-pi,pi]
    // When the solution is not unique, zAngle = 0 is returned.  Generally,
    // the set of solutions is
    //   EA_NOT_UNIQUE_SUM:  zAngle + yAngle = c
    //   EA_NOT_UNIQUE_DIF:  zAngle - yAngle = c
    // for some angle c.
    EulerResult ToEulerAnglesYXZ (nReal& rfYAngle, nReal& rfXAngle,
        nReal& rfZAngle) const;

    // The return values are in the specified ranges:
    //   yAngle in [-pi,pi], zAngle in [-pi/2,pi/2], xAngle in [-pi,pi]
    // When the solution is not unique, xAngle = 0 is returned.  Generally,
    // the set of solutions is
    //   EA_NOT_UNIQUE_SUM:  xAngle + yAngle = c
    //   EA_NOT_UNIQUE_DIF:  xAngle - yAngle = c
    // for some angle c.
    EulerResult ToEulerAnglesYZX (nReal& rfYAngle, nReal& rfZAngle,
        nReal& rfXAngle) const;

    // The return values are in the specified ranges:
    //   zAngle in [-pi,pi], xAngle in [-pi/2,pi/2], yAngle in [-pi,pi]
    // When the solution is not unique, yAngle = 0 is returned.  Generally,
    // the set of solutions is
    //   EA_NOT_UNIQUE_SUM:  yAngle + zAngle = c
    //   EA_NOT_UNIQUE_DIF:  yAngle - zAngle = c
    // for some angle c.
    EulerResult ToEulerAnglesZXY (nReal& rfZAngle, nReal& rfXAngle,
        nReal& rfYAngle) const;

    // The return values are in the specified ranges:
    //   zAngle in [-pi,pi], yAngle in [-pi/2,pi/2], xAngle in [-pi,pi]
    // When the solution is not unique, xAngle = 0 is/ returned.  Generally,
    // the set of solutions is
    //   EA_NOT_UNIQUE_SUM:  xAngle + zAngle = c
    //   EA_NOT_UNIQUE_DIF:  xAngle - zAngle = c
    // for some angle c.
    EulerResult ToEulerAnglesZYX (nReal& rfZAngle, nReal& rfYAngle,
        nReal& rfXAngle) const;

    // The return values are in the specified ranges:
    //   x0Angle in [-pi,pi], yAngle in [0,pi], x1Angle in [-pi,pi]
    // When the solution is not unique, x1Angle = 0 is returned.  Generally,
    // the set of solutions is
    //   EA_NOT_UNIQUE_SUM:  x1Angle + x0Angle = c
    //   EA_NOT_UNIQUE_DIF:  x1Angle - x0Angle = c
    // for some angle c.
    EulerResult ToEulerAnglesXYX (nReal& rfX0Angle, nReal& rfYAngle,
        nReal& rfX1Angle) const;

    // The return values are in the specified ranges:
    //   x0Angle in [-pi,pi], zAngle in [0,pi], x1Angle in [-pi,pi]
    // When the solution is not unique, x1Angle = 0 is returned.  Generally,
    // the set of solutions is
    //   EA_NOT_UNIQUE_SUM:  x1Angle + x0Angle = c
    //   EA_NOT_UNIQUE_DIF:  x1Angle - x0Angle = c
    // for some angle c.
    EulerResult ToEulerAnglesXZX (nReal& rfX0Angle, nReal& rfZAngle,
        nReal& rfX1Angle) const;

    // The return values are in the specified ranges:
    //   y0Angle in [-pi,pi], xAngle in [0,pi], y1Angle in [-pi,pi]
    // When the solution is not unique, y1Angle = 0 is returned.  Generally,
    // the set of solutions is
    //   EA_NOT_UNIQUE_SUM:  y1Angle + y0Angle = c
    //   EA_NOT_UNIQUE_DIF:  y1Angle - y0Angle = c
    // for some angle c.
    EulerResult ToEulerAnglesYXY (nReal& rfY0Angle, nReal& rfXAngle,
        nReal& rfY1Angle) const;

    // The return values are in the specified ranges:
    //   y0Angle in [-pi,pi], zAngle in [0,pi], y1Angle in [-pi,pi]
    // When the solution is not unique, y1Angle = 0 is returned.  Generally,
    // the set of solutions is
    //   EA_NOT_UNIQUE_SUM:  y1Angle + y0Angle = c
    //   EA_NOT_UNIQUE_DIF:  y1Angle - y0Angle = c
    // for some angle c.
    EulerResult ToEulerAnglesYZY (nReal& rfY0Angle, nReal& rfZAngle,
        nReal& rfY1Angle) const;

    // The return values are in the specified ranges:
    //   z0Angle in [-pi,pi], xAngle in [0,pi], z1Angle in [-pi,pi]
    // When the solution is not unique, z1Angle = 0 is returned.  Generally,
    // the set of solutions is
    //   EA_NOT_UNIQUE_SUM:  z1Angle + z0Angle = c
    //   EA_NOT_UNIQUE_DIF:  z1Angle - z0Angle = c
    // for some angle c.
    EulerResult ToEulerAnglesZXZ (nReal& rfZ0Angle, nReal& rfXAngle,
        nReal& rfZ1Angle) const;

    // The return values are in the specified ranges:
    //   z0Angle in [-pi,pi], yAngle in [0,pi], z1Angle in [-pi,pi]
    // When the solution is not unique, z1Angle = 0 is returned.  Generally,
    // the set of solutions is
    //   EA_NOT_UNIQUE_SUM:  z1Angle + z0Angle = c
    //   EA_NOT_UNIQUE_DIF:  z1Angle - z0Angle = c
    // for some angle c.
    EulerResult ToEulerAnglesZYZ (nReal& rfZ0Angle, nReal& rfYAngle,
        nReal& rfZ1Angle) const;

    // SLERP (spherical linear interpolation) without quaternions.  Computes
    // R(t) = R0*(Transpose(R0)*R1)^t.  If Q is a rotation matrix with
    // unit-length axis U and angle A, then Q^t is a rotation matrix with
    // unit-length axis U and rotation angle t*A.
    CMatrix3& Slerp (nReal fT, const CMatrix3& rkR0, const CMatrix3& rkR1);

    // Singular value decomposition, M = L*D*Transpose(R), where L and R are
    // orthogonal and D is a diagonal matrix whose diagonal entries are
    // nonnegative.
    void SingularValueDecomposition (CMatrix3& rkL, CMatrix3& rkD,
        CMatrix3& rkRTranspose) const;

    // For debugging purposes.  Take the output of SingularValueDecomposition
    // and multiply to see if you get M.
    void SingularValueComposition (const CMatrix3& rkL, const CMatrix3& rkD,
        const CMatrix3& rkRTranspose);

    // Polar decomposition, M = Q*S, where Q is orthogonal and S is symmetric.
    // This uses the singular value decomposition:
    //   M = L*D*Transpose(R) = (L*Transpose(R))*(R*D*Transpose(R)) = Q*S
    // where Q = L*Transpose(R) and S = R*D*Transpose(R).
    void PolarDecomposition (CMatrix3& rkQ, CMatrix3& rkS);

    // Factor M = Q*D*U with orthogonal Q, diagonal D, upper triangular U.
    void QDUDecomposition (CMatrix3& rkQ, CMatrix3& rkD, CMatrix3& rkU) const;

    // special matrices
    NOVA_EXPORT static const CMatrix3 ZERO;
    NOVA_EXPORT static const CMatrix3 IDENTITY;

private:
    // Support for eigendecomposition.  The Tridiagonalize function applies
    // a Householder transformation to the matrix.  If that transformation
    // is the identity (the matrix is already tridiagonal), then the return
    // value is 'false'.  Otherwise, the transformation is a reflection and
    // the return value is 'true'.  The QLAlgorithm returns 'true' iff the
    // QL iteration scheme converged.
    bool Tridiagonalize (nReal afDiag[3], nReal afSubd[2]);
    bool QLAlgorithm (nReal afDiag[3], nReal afSubd[2]);

    // support for singular value decomposition
    static void Bidiagonalize (CMatrix3& rkA, CMatrix3& rkL, CMatrix3& rkR);
    static void GolubKahanStep (CMatrix3& rkA, CMatrix3& rkL, CMatrix3& rkR);

    // support for comparisons
    nInt32 CompareArrays (const CMatrix3& rkM) const;

    nReal m_afEntry[9];
};

// c * M
template <class nReal>
inline CMatrix3<nReal> operator* (nReal fScalar, const CMatrix3<nReal>& rkM);

// v^T * M
template <class nReal>
inline CVector3<nReal> operator* (const CVector3<nReal>& rkV,
    const CMatrix3<nReal>& rkM);

template <class nReal>
CMatrix3<nReal>::CMatrix3 (bool bZero)
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
CMatrix3<nReal>::CMatrix3 (const CMatrix3& rkM)
{
    m_afEntry[0] = rkM.m_afEntry[0];
    m_afEntry[1] = rkM.m_afEntry[1];
    m_afEntry[2] = rkM.m_afEntry[2];
    m_afEntry[3] = rkM.m_afEntry[3];
    m_afEntry[4] = rkM.m_afEntry[4];
    m_afEntry[5] = rkM.m_afEntry[5];
    m_afEntry[6] = rkM.m_afEntry[6];
    m_afEntry[7] = rkM.m_afEntry[7];
    m_afEntry[8] = rkM.m_afEntry[8];
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal>::CMatrix3 (nReal fM00, nReal fM01, nReal fM02, nReal fM10, nReal fM11,
    nReal fM12, nReal fM20, nReal fM21, nReal fM22)
{
    m_afEntry[0] = fM00;
    m_afEntry[1] = fM01;
    m_afEntry[2] = fM02;
    m_afEntry[3] = fM10;
    m_afEntry[4] = fM11;
    m_afEntry[5] = fM12;
    m_afEntry[6] = fM20;
    m_afEntry[7] = fM21;
    m_afEntry[8] = fM22;
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal>::CMatrix3 (const nReal afEntry[9], bool bRowMajor)
{
    if (bRowMajor)
    {
        m_afEntry[0] = afEntry[0];
        m_afEntry[1] = afEntry[1];
        m_afEntry[2] = afEntry[2];
        m_afEntry[3] = afEntry[3];
        m_afEntry[4] = afEntry[4];
        m_afEntry[5] = afEntry[5];
        m_afEntry[6] = afEntry[6];
        m_afEntry[7] = afEntry[7];
        m_afEntry[8] = afEntry[8];
    }
    else
    {
        m_afEntry[0] = afEntry[0];
        m_afEntry[1] = afEntry[3];
        m_afEntry[2] = afEntry[6];
        m_afEntry[3] = afEntry[1];
        m_afEntry[4] = afEntry[4];
        m_afEntry[5] = afEntry[7];
        m_afEntry[6] = afEntry[2];
        m_afEntry[7] = afEntry[5];
        m_afEntry[8] = afEntry[8];
    }
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal>::CMatrix3 (const CVector3<nReal>& rkU, const CVector3<nReal>& rkV,
    const CVector3<nReal>& rkW, bool bColumns)
{
    if (bColumns)
    {
        m_afEntry[0] = rkU[0];
        m_afEntry[1] = rkV[0];
        m_afEntry[2] = rkW[0];
        m_afEntry[3] = rkU[1];
        m_afEntry[4] = rkV[1];
        m_afEntry[5] = rkW[1];
        m_afEntry[6] = rkU[2];
        m_afEntry[7] = rkV[2];
        m_afEntry[8] = rkW[2];
    }
    else
    {
        m_afEntry[0] = rkU[0];
        m_afEntry[1] = rkU[1];
        m_afEntry[2] = rkU[2];
        m_afEntry[3] = rkV[0];
        m_afEntry[4] = rkV[1];
        m_afEntry[5] = rkV[2];
        m_afEntry[6] = rkW[0];
        m_afEntry[7] = rkW[1];
        m_afEntry[8] = rkW[2];
    }
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal>::CMatrix3 (const CVector3<nReal>* akV, bool bColumns)
{
    if (bColumns)
    {
        m_afEntry[0] = akV[0][0];
        m_afEntry[1] = akV[1][0];
        m_afEntry[2] = akV[2][0];
        m_afEntry[3] = akV[0][1];
        m_afEntry[4] = akV[1][1];
        m_afEntry[5] = akV[2][1];
        m_afEntry[6] = akV[0][2];
        m_afEntry[7] = akV[1][2];
        m_afEntry[8] = akV[2][2];
    }
    else
    {
        m_afEntry[0] = akV[0][0];
        m_afEntry[1] = akV[0][1];
        m_afEntry[2] = akV[0][2];
        m_afEntry[3] = akV[1][0];
        m_afEntry[4] = akV[1][1];
        m_afEntry[5] = akV[1][2];
        m_afEntry[6] = akV[2][0];
        m_afEntry[7] = akV[2][1];
        m_afEntry[8] = akV[2][2];
    }
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal>::CMatrix3 (nReal fM00, nReal fM11, nReal fM22)
{
    MakeDiagonal(fM00,fM11,fM22);
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal>::CMatrix3 (const CVector3<nReal>& rkAxis, nReal fAngle)
{
    FromAxisAngle(rkAxis,fAngle);
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal>::CMatrix3 (const CVector3<nReal>& rkU, const CVector3<nReal>& rkV)
{
    MakeTensorProduct(rkU,rkV);
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix3<nReal>::operator const nReal* () const
{
    return m_afEntry;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix3<nReal>::operator nReal* ()
{
    return m_afEntry;
}
//----------------------------------------------------------------------------
template <class nReal>
inline const nReal* CMatrix3<nReal>::operator[] (nInt32 iRow) const
{
    return &m_afEntry[3*iRow];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal* CMatrix3<nReal>::operator[] (nInt32 iRow)
{
    return &m_afEntry[3*iRow];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal CMatrix3<nReal>::operator() (nInt32 iRow, nInt32 iCol) const
{
    return m_afEntry[iCol+3*iRow];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal& CMatrix3<nReal>::operator() (nInt32 iRow, nInt32 iCol)
{
    return m_afEntry[iCol+3*iRow];
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal>& CMatrix3<nReal>::MakeZero ()
{
    m_afEntry[0] = (nReal)0.0;
    m_afEntry[1] = (nReal)0.0;
    m_afEntry[2] = (nReal)0.0;
    m_afEntry[3] = (nReal)0.0;
    m_afEntry[4] = (nReal)0.0;
    m_afEntry[5] = (nReal)0.0;
    m_afEntry[6] = (nReal)0.0;
    m_afEntry[7] = (nReal)0.0;
    m_afEntry[8] = (nReal)0.0;
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal>& CMatrix3<nReal>::MakeIdentity ()
{
    m_afEntry[0] = (nReal)1.0;
    m_afEntry[1] = (nReal)0.0;
    m_afEntry[2] = (nReal)0.0;
    m_afEntry[3] = (nReal)0.0;
    m_afEntry[4] = (nReal)1.0;
    m_afEntry[5] = (nReal)0.0;
    m_afEntry[6] = (nReal)0.0;
    m_afEntry[7] = (nReal)0.0;
    m_afEntry[8] = (nReal)1.0;
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal>& CMatrix3<nReal>::MakeDiagonal (nReal fM00, nReal fM11, nReal fM22)
{
    m_afEntry[0] = fM00;
    m_afEntry[1] = (nReal)0.0;
    m_afEntry[2] = (nReal)0.0;
    m_afEntry[3] = (nReal)0.0;
    m_afEntry[4] = fM11;
    m_afEntry[5] = (nReal)0.0;
    m_afEntry[6] = (nReal)0.0;
    m_afEntry[7] = (nReal)0.0;
    m_afEntry[8] = fM22;
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal>& CMatrix3<nReal>::FromAxisAngle (const CVector3<nReal>& rkAxis,
    nReal fAngle)
{
    nReal fCos = CMath<nReal>::Cos(fAngle);
    nReal fSin = CMath<nReal>::Sin(fAngle);
    nReal fOneMinusCos = ((nReal)1.0)-fCos;
    nReal fX2 = rkAxis[0]*rkAxis[0];
    nReal fY2 = rkAxis[1]*rkAxis[1];
    nReal fZ2 = rkAxis[2]*rkAxis[2];
    nReal fXYM = rkAxis[0]*rkAxis[1]*fOneMinusCos;
    nReal fXZM = rkAxis[0]*rkAxis[2]*fOneMinusCos;
    nReal fYZM = rkAxis[1]*rkAxis[2]*fOneMinusCos;
    nReal fXSin = rkAxis[0]*fSin;
    nReal fYSin = rkAxis[1]*fSin;
    nReal fZSin = rkAxis[2]*fSin;
    
    m_afEntry[0] = fX2*fOneMinusCos+fCos;
    m_afEntry[1] = fXYM-fZSin;
    m_afEntry[2] = fXZM+fYSin;
    m_afEntry[3] = fXYM+fZSin;
    m_afEntry[4] = fY2*fOneMinusCos+fCos;
    m_afEntry[5] = fYZM-fXSin;
    m_afEntry[6] = fXZM-fYSin;
    m_afEntry[7] = fYZM+fXSin;
    m_afEntry[8] = fZ2*fOneMinusCos+fCos;

    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal>& CMatrix3<nReal>::MakeTensorProduct (const CVector3<nReal>& rkU,
    const CVector3<nReal>& rkV)
{
    m_afEntry[0] = rkU[0]*rkV[0];
    m_afEntry[1] = rkU[0]*rkV[1];
    m_afEntry[2] = rkU[0]*rkV[2];
    m_afEntry[3] = rkU[1]*rkV[0];
    m_afEntry[4] = rkU[1]*rkV[1];
    m_afEntry[5] = rkU[1]*rkV[2];
    m_afEntry[6] = rkU[2]*rkV[0];
    m_afEntry[7] = rkU[2]*rkV[1];
    m_afEntry[8] = rkU[2]*rkV[2];
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
void CMatrix3<nReal>::SetRow (nInt32 iRow, const CVector3<nReal>& rkV)
{
    nInt32 i0 = 3*iRow, i1 = i0+1, i2 = i1+1;
    m_afEntry[i0] = rkV[0];
    m_afEntry[i1] = rkV[1];
    m_afEntry[i2] = rkV[2];
}
//----------------------------------------------------------------------------
template <class nReal>
CVector3<nReal> CMatrix3<nReal>::GetRow (nInt32 iRow) const
{
    nInt32 i0 = 3*iRow, i1 = i0+1, i2 = i1+1;
    return CVector3<nReal>(m_afEntry[i0],m_afEntry[i1],m_afEntry[i2]);
}
//----------------------------------------------------------------------------
template <class nReal>
void CMatrix3<nReal>::SetColumn (nInt32 iCol, const CVector3<nReal>& rkV)
{
    m_afEntry[iCol] = rkV[0];
    m_afEntry[iCol+3] = rkV[1];
    m_afEntry[iCol+6] = rkV[2];
}
//----------------------------------------------------------------------------
template <class nReal>
CVector3<nReal> CMatrix3<nReal>::GetColumn (nInt32 iCol) const
{
    return CVector3<nReal>(m_afEntry[iCol],m_afEntry[iCol+3],m_afEntry[iCol+6]);
}
//----------------------------------------------------------------------------
template <class nReal>
void CMatrix3<nReal>::GetColumnMajor (nReal* afCMajor) const
{
    afCMajor[0] = m_afEntry[0];
    afCMajor[1] = m_afEntry[3];
    afCMajor[2] = m_afEntry[6];
    afCMajor[3] = m_afEntry[1];
    afCMajor[4] = m_afEntry[4];
    afCMajor[5] = m_afEntry[7];
    afCMajor[6] = m_afEntry[2];
    afCMajor[7] = m_afEntry[5];
    afCMajor[8] = m_afEntry[8];
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix3<nReal>& CMatrix3<nReal>::operator= (const CMatrix3& rkM)
{
    m_afEntry[0] = rkM.m_afEntry[0];
    m_afEntry[1] = rkM.m_afEntry[1];
    m_afEntry[2] = rkM.m_afEntry[2];
    m_afEntry[3] = rkM.m_afEntry[3];
    m_afEntry[4] = rkM.m_afEntry[4];
    m_afEntry[5] = rkM.m_afEntry[5];
    m_afEntry[6] = rkM.m_afEntry[6];
    m_afEntry[7] = rkM.m_afEntry[7];
    m_afEntry[8] = rkM.m_afEntry[8];
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
nInt32 CMatrix3<nReal>::CompareArrays (const CMatrix3& rkM) const
{
    return memcmp(m_afEntry,rkM.m_afEntry,9*sizeof(nReal));
}
//----------------------------------------------------------------------------
template <class nReal>
bool CMatrix3<nReal>::operator== (const CMatrix3& rkM) const
{
    return CompareArrays(rkM) == 0;
}
//----------------------------------------------------------------------------
template <class nReal>
bool CMatrix3<nReal>::operator!= (const CMatrix3& rkM) const
{
    return CompareArrays(rkM) != 0;
}
//----------------------------------------------------------------------------
template <class nReal>
bool CMatrix3<nReal>::operator<  (const CMatrix3& rkM) const
{
    return CompareArrays(rkM) < 0;
}
//----------------------------------------------------------------------------
template <class nReal>
bool CMatrix3<nReal>::operator<= (const CMatrix3& rkM) const
{
    return CompareArrays(rkM) <= 0;
}
//----------------------------------------------------------------------------
template <class nReal>
bool CMatrix3<nReal>::operator>  (const CMatrix3& rkM) const
{
    return CompareArrays(rkM) > 0;
}
//----------------------------------------------------------------------------
template <class nReal>
bool CMatrix3<nReal>::operator>= (const CMatrix3& rkM) const
{
    return CompareArrays(rkM) >= 0;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix3<nReal> CMatrix3<nReal>::operator+ (const CMatrix3& rkM) const
{
    return CMatrix3<nReal>(
        m_afEntry[0] + rkM.m_afEntry[0],
        m_afEntry[1] + rkM.m_afEntry[1],
        m_afEntry[2] + rkM.m_afEntry[2],
        m_afEntry[3] + rkM.m_afEntry[3],
        m_afEntry[4] + rkM.m_afEntry[4],
        m_afEntry[5] + rkM.m_afEntry[5],
        m_afEntry[6] + rkM.m_afEntry[6],
        m_afEntry[7] + rkM.m_afEntry[7],
        m_afEntry[8] + rkM.m_afEntry[8]);
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix3<nReal> CMatrix3<nReal>::operator- (const CMatrix3& rkM) const
{
    return CMatrix3<nReal>(
        m_afEntry[0] - rkM.m_afEntry[0],
        m_afEntry[1] - rkM.m_afEntry[1],
        m_afEntry[2] - rkM.m_afEntry[2],
        m_afEntry[3] - rkM.m_afEntry[3],
        m_afEntry[4] - rkM.m_afEntry[4],
        m_afEntry[5] - rkM.m_afEntry[5],
        m_afEntry[6] - rkM.m_afEntry[6],
        m_afEntry[7] - rkM.m_afEntry[7],
        m_afEntry[8] - rkM.m_afEntry[8]);
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix3<nReal> CMatrix3<nReal>::operator* (const CMatrix3& rkM) const
{
    return CMatrix3<nReal>(
        m_afEntry[0]*rkM.m_afEntry[0] +
        m_afEntry[1]*rkM.m_afEntry[3] +
        m_afEntry[2]*rkM.m_afEntry[6],

        m_afEntry[0]*rkM.m_afEntry[1] +
        m_afEntry[1]*rkM.m_afEntry[4] +
        m_afEntry[2]*rkM.m_afEntry[7],

        m_afEntry[0]*rkM.m_afEntry[2] +
        m_afEntry[1]*rkM.m_afEntry[5] +
        m_afEntry[2]*rkM.m_afEntry[8],

        m_afEntry[3]*rkM.m_afEntry[0] +
        m_afEntry[4]*rkM.m_afEntry[3] +
        m_afEntry[5]*rkM.m_afEntry[6],

        m_afEntry[3]*rkM.m_afEntry[1] +
        m_afEntry[4]*rkM.m_afEntry[4] +
        m_afEntry[5]*rkM.m_afEntry[7],

        m_afEntry[3]*rkM.m_afEntry[2] +
        m_afEntry[4]*rkM.m_afEntry[5] +
        m_afEntry[5]*rkM.m_afEntry[8],

        m_afEntry[6]*rkM.m_afEntry[0] +
        m_afEntry[7]*rkM.m_afEntry[3] +
        m_afEntry[8]*rkM.m_afEntry[6],

        m_afEntry[6]*rkM.m_afEntry[1] +
        m_afEntry[7]*rkM.m_afEntry[4] +
        m_afEntry[8]*rkM.m_afEntry[7],

        m_afEntry[6]*rkM.m_afEntry[2] +
        m_afEntry[7]*rkM.m_afEntry[5] +
        m_afEntry[8]*rkM.m_afEntry[8]);
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix3<nReal> CMatrix3<nReal>::operator* (nReal fScalar) const
{
    return CMatrix3<nReal>(
        fScalar*m_afEntry[0],
        fScalar*m_afEntry[1],
        fScalar*m_afEntry[2],
        fScalar*m_afEntry[3],
        fScalar*m_afEntry[4],
        fScalar*m_afEntry[5],
        fScalar*m_afEntry[6],
        fScalar*m_afEntry[7],
        fScalar*m_afEntry[8]);
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix3<nReal> CMatrix3<nReal>::operator/ (nReal fScalar) const
{
    if (fScalar != (nReal)0.0)
    {
        nReal fInvScalar = ((nReal)1.0)/fScalar;
        return CMatrix3<nReal>(
            fInvScalar*m_afEntry[0],
            fInvScalar*m_afEntry[1],
            fInvScalar*m_afEntry[2],
            fInvScalar*m_afEntry[3],
            fInvScalar*m_afEntry[4],
            fInvScalar*m_afEntry[5],
            fInvScalar*m_afEntry[6],
            fInvScalar*m_afEntry[7],
            fInvScalar*m_afEntry[8]);
    }

    return CMatrix3<nReal>(
        CMath<nReal>::MAX_REAL,
        CMath<nReal>::MAX_REAL,
        CMath<nReal>::MAX_REAL,
        CMath<nReal>::MAX_REAL,
        CMath<nReal>::MAX_REAL,
        CMath<nReal>::MAX_REAL,
        CMath<nReal>::MAX_REAL,
        CMath<nReal>::MAX_REAL,
        CMath<nReal>::MAX_REAL);
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix3<nReal> CMatrix3<nReal>::operator- () const
{
    return CMatrix3<nReal>(
        -m_afEntry[0],
        -m_afEntry[1],
        -m_afEntry[2],
        -m_afEntry[3],
        -m_afEntry[4],
        -m_afEntry[5],
        -m_afEntry[6],
        -m_afEntry[7],
        -m_afEntry[8]);
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix3<nReal>& CMatrix3<nReal>::operator+= (const CMatrix3& rkM)
{
    m_afEntry[0] += rkM.m_afEntry[0];
    m_afEntry[1] += rkM.m_afEntry[1];
    m_afEntry[2] += rkM.m_afEntry[2];
    m_afEntry[3] += rkM.m_afEntry[3];
    m_afEntry[4] += rkM.m_afEntry[4];
    m_afEntry[5] += rkM.m_afEntry[5];
    m_afEntry[6] += rkM.m_afEntry[6];
    m_afEntry[7] += rkM.m_afEntry[7];
    m_afEntry[8] += rkM.m_afEntry[8];
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix3<nReal>& CMatrix3<nReal>::operator-= (const CMatrix3& rkM)
{
    m_afEntry[0] -= rkM.m_afEntry[0];
    m_afEntry[1] -= rkM.m_afEntry[1];
    m_afEntry[2] -= rkM.m_afEntry[2];
    m_afEntry[3] -= rkM.m_afEntry[3];
    m_afEntry[4] -= rkM.m_afEntry[4];
    m_afEntry[5] -= rkM.m_afEntry[5];
    m_afEntry[6] -= rkM.m_afEntry[6];
    m_afEntry[7] -= rkM.m_afEntry[7];
    m_afEntry[8] -= rkM.m_afEntry[8];
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix3<nReal>& CMatrix3<nReal>::operator*= (nReal fScalar)
{
    m_afEntry[0] *= fScalar;
    m_afEntry[1] *= fScalar;
    m_afEntry[2] *= fScalar;
    m_afEntry[3] *= fScalar;
    m_afEntry[4] *= fScalar;
    m_afEntry[5] *= fScalar;
    m_afEntry[6] *= fScalar;
    m_afEntry[7] *= fScalar;
    m_afEntry[8] *= fScalar;
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix3<nReal>& CMatrix3<nReal>::operator/= (nReal fScalar)
{
    if (fScalar != (nReal)0.0)
    {
        nReal fInvScalar = ((nReal)1.0)/fScalar;
        m_afEntry[0] *= fInvScalar;
        m_afEntry[1] *= fInvScalar;
        m_afEntry[2] *= fInvScalar;
        m_afEntry[3] *= fInvScalar;
        m_afEntry[4] *= fInvScalar;
        m_afEntry[5] *= fInvScalar;
        m_afEntry[6] *= fInvScalar;
        m_afEntry[7] *= fInvScalar;
        m_afEntry[8] *= fInvScalar;
    }
    else
    {
        m_afEntry[0] = CMath<nReal>::MAX_REAL;
        m_afEntry[1] = CMath<nReal>::MAX_REAL;
        m_afEntry[2] = CMath<nReal>::MAX_REAL;
        m_afEntry[3] = CMath<nReal>::MAX_REAL;
        m_afEntry[4] = CMath<nReal>::MAX_REAL;
        m_afEntry[5] = CMath<nReal>::MAX_REAL;
        m_afEntry[6] = CMath<nReal>::MAX_REAL;
        m_afEntry[7] = CMath<nReal>::MAX_REAL;
        m_afEntry[8] = CMath<nReal>::MAX_REAL;
    }

    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CVector3<nReal> CMatrix3<nReal>::operator* (const CVector3<nReal>& rkV) const
{
    return CVector3<nReal>(
        m_afEntry[0]*rkV[0] + m_afEntry[1]*rkV[1] + m_afEntry[2]*rkV[2],
        m_afEntry[3]*rkV[0] + m_afEntry[4]*rkV[1] + m_afEntry[5]*rkV[2],
        m_afEntry[6]*rkV[0] + m_afEntry[7]*rkV[1] + m_afEntry[8]*rkV[2]);
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal> CMatrix3<nReal>::Transpose () const
{
    return CMatrix3<nReal>(
        m_afEntry[0],
        m_afEntry[3],
        m_afEntry[6],
        m_afEntry[1],
        m_afEntry[4],
        m_afEntry[7],
        m_afEntry[2],
        m_afEntry[5],
        m_afEntry[8]);
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal> CMatrix3<nReal>::TransposeTimes (const CMatrix3& rkM) const
{
    // P = A^T*B
    return CMatrix3<nReal>(
        m_afEntry[0]*rkM.m_afEntry[0] +
        m_afEntry[3]*rkM.m_afEntry[3] +
        m_afEntry[6]*rkM.m_afEntry[6],

        m_afEntry[0]*rkM.m_afEntry[1] +
        m_afEntry[3]*rkM.m_afEntry[4] +
        m_afEntry[6]*rkM.m_afEntry[7],

        m_afEntry[0]*rkM.m_afEntry[2] +
        m_afEntry[3]*rkM.m_afEntry[5] +
        m_afEntry[6]*rkM.m_afEntry[8],

        m_afEntry[1]*rkM.m_afEntry[0] +
        m_afEntry[4]*rkM.m_afEntry[3] +
        m_afEntry[7]*rkM.m_afEntry[6],

        m_afEntry[1]*rkM.m_afEntry[1] +
        m_afEntry[4]*rkM.m_afEntry[4] +
        m_afEntry[7]*rkM.m_afEntry[7],

        m_afEntry[1]*rkM.m_afEntry[2] +
        m_afEntry[4]*rkM.m_afEntry[5] +
        m_afEntry[7]*rkM.m_afEntry[8],

        m_afEntry[2]*rkM.m_afEntry[0] +
        m_afEntry[5]*rkM.m_afEntry[3] +
        m_afEntry[8]*rkM.m_afEntry[6],

        m_afEntry[2]*rkM.m_afEntry[1] +
        m_afEntry[5]*rkM.m_afEntry[4] +
        m_afEntry[8]*rkM.m_afEntry[7],

        m_afEntry[2]*rkM.m_afEntry[2] +
        m_afEntry[5]*rkM.m_afEntry[5] +
        m_afEntry[8]*rkM.m_afEntry[8]);
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal> CMatrix3<nReal>::TimesTranspose (const CMatrix3& rkM) const
{
    // P = A*B^T
    return CMatrix3<nReal>(
        m_afEntry[0]*rkM.m_afEntry[0] +
        m_afEntry[1]*rkM.m_afEntry[1] +
        m_afEntry[2]*rkM.m_afEntry[2],

        m_afEntry[0]*rkM.m_afEntry[3] +
        m_afEntry[1]*rkM.m_afEntry[4] +
        m_afEntry[2]*rkM.m_afEntry[5],

        m_afEntry[0]*rkM.m_afEntry[6] +
        m_afEntry[1]*rkM.m_afEntry[7] +
        m_afEntry[2]*rkM.m_afEntry[8],

        m_afEntry[3]*rkM.m_afEntry[0] +
        m_afEntry[4]*rkM.m_afEntry[1] +
        m_afEntry[5]*rkM.m_afEntry[2],

        m_afEntry[3]*rkM.m_afEntry[3] +
        m_afEntry[4]*rkM.m_afEntry[4] +
        m_afEntry[5]*rkM.m_afEntry[5],

        m_afEntry[3]*rkM.m_afEntry[6] +
        m_afEntry[4]*rkM.m_afEntry[7] +
        m_afEntry[5]*rkM.m_afEntry[8],

        m_afEntry[6]*rkM.m_afEntry[0] +
        m_afEntry[7]*rkM.m_afEntry[1] +
        m_afEntry[8]*rkM.m_afEntry[2],

        m_afEntry[6]*rkM.m_afEntry[3] +
        m_afEntry[7]*rkM.m_afEntry[4] +
        m_afEntry[8]*rkM.m_afEntry[5],

        m_afEntry[6]*rkM.m_afEntry[6] +
        m_afEntry[7]*rkM.m_afEntry[7] +
        m_afEntry[8]*rkM.m_afEntry[8]);
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal> CMatrix3<nReal>::Inverse () const
{
    // Invert a 3x3 using cofactors.  This is faster than using a generic
    // Gaussian elimination because of the loop overhead of such a method.

    CMatrix3 kInverse;

    kInverse.m_afEntry[0] =
        m_afEntry[4]*m_afEntry[8] - m_afEntry[5]*m_afEntry[7];
    kInverse.m_afEntry[1] =
        m_afEntry[2]*m_afEntry[7] - m_afEntry[1]*m_afEntry[8];
    kInverse.m_afEntry[2] =
        m_afEntry[1]*m_afEntry[5] - m_afEntry[2]*m_afEntry[4];
    kInverse.m_afEntry[3] =
        m_afEntry[5]*m_afEntry[6] - m_afEntry[3]*m_afEntry[8];
    kInverse.m_afEntry[4] =
        m_afEntry[0]*m_afEntry[8] - m_afEntry[2]*m_afEntry[6];
    kInverse.m_afEntry[5] =
        m_afEntry[2]*m_afEntry[3] - m_afEntry[0]*m_afEntry[5];
    kInverse.m_afEntry[6] =
        m_afEntry[3]*m_afEntry[7] - m_afEntry[4]*m_afEntry[6];
    kInverse.m_afEntry[7] =
        m_afEntry[1]*m_afEntry[6] - m_afEntry[0]*m_afEntry[7];
    kInverse.m_afEntry[8] =
        m_afEntry[0]*m_afEntry[4] - m_afEntry[1]*m_afEntry[3];

    nReal fDet =
        m_afEntry[0]*kInverse.m_afEntry[0] +
        m_afEntry[1]*kInverse.m_afEntry[3] +
        m_afEntry[2]*kInverse.m_afEntry[6];

    if (CMath<nReal>::FAbs(fDet) <= CMath<nReal>::ZERO_TOLERANCE)
    {
        return ZERO;
    }

    nReal fInvDet = ((nReal)1.0)/fDet;
    kInverse.m_afEntry[0] *= fInvDet;
    kInverse.m_afEntry[1] *= fInvDet;
    kInverse.m_afEntry[2] *= fInvDet;
    kInverse.m_afEntry[3] *= fInvDet;
    kInverse.m_afEntry[4] *= fInvDet;
    kInverse.m_afEntry[5] *= fInvDet;
    kInverse.m_afEntry[6] *= fInvDet;
    kInverse.m_afEntry[7] *= fInvDet;
    kInverse.m_afEntry[8] *= fInvDet;
    return kInverse;
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal> CMatrix3<nReal>::Adjoint () const
{
    return CMatrix3<nReal>(
        m_afEntry[4]*m_afEntry[8] - m_afEntry[5]*m_afEntry[7],
        m_afEntry[2]*m_afEntry[7] - m_afEntry[1]*m_afEntry[8],
        m_afEntry[1]*m_afEntry[5] - m_afEntry[2]*m_afEntry[4],
        m_afEntry[5]*m_afEntry[6] - m_afEntry[3]*m_afEntry[8],
        m_afEntry[0]*m_afEntry[8] - m_afEntry[2]*m_afEntry[6],
        m_afEntry[2]*m_afEntry[3] - m_afEntry[0]*m_afEntry[5],
        m_afEntry[3]*m_afEntry[7] - m_afEntry[4]*m_afEntry[6],
        m_afEntry[1]*m_afEntry[6] - m_afEntry[0]*m_afEntry[7],
        m_afEntry[0]*m_afEntry[4] - m_afEntry[1]*m_afEntry[3]);
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMatrix3<nReal>::Determinant () const
{
    nReal fCo00 = m_afEntry[4]*m_afEntry[8] - m_afEntry[5]*m_afEntry[7];
    nReal fCo10 = m_afEntry[5]*m_afEntry[6] - m_afEntry[3]*m_afEntry[8];
    nReal fCo20 = m_afEntry[3]*m_afEntry[7] - m_afEntry[4]*m_afEntry[6];
    nReal fDet = m_afEntry[0]*fCo00 + m_afEntry[1]*fCo10 + m_afEntry[2]*fCo20;
    return fDet;
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMatrix3<nReal>::QForm (const CVector3<nReal>& rkU, const CVector3<nReal>& rkV)
    const
{
    return rkU.Dot((*this)*rkV);
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal> CMatrix3<nReal>::TimesDiagonal (const CVector3<nReal>& rkDiag) const
{
    return CMatrix3<nReal>(
        m_afEntry[0]*rkDiag[0],m_afEntry[1]*rkDiag[1],m_afEntry[2]*rkDiag[2],
        m_afEntry[3]*rkDiag[0],m_afEntry[4]*rkDiag[1],m_afEntry[5]*rkDiag[2],
        m_afEntry[6]*rkDiag[0],m_afEntry[7]*rkDiag[1],m_afEntry[8]*rkDiag[2]);
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal> CMatrix3<nReal>::DiagonalTimes (const CVector3<nReal>& rkDiag) const
{
    return CMatrix3<nReal>(
        rkDiag[0]*m_afEntry[0],rkDiag[0]*m_afEntry[1],rkDiag[0]*m_afEntry[2],
        rkDiag[1]*m_afEntry[3],rkDiag[1]*m_afEntry[4],rkDiag[1]*m_afEntry[5],
        rkDiag[2]*m_afEntry[6],rkDiag[2]*m_afEntry[7],rkDiag[2]*m_afEntry[8]);
}
//----------------------------------------------------------------------------
template <class nReal>
void CMatrix3<nReal>::ToAxisAngle (CVector3<nReal>& rkAxis, nReal& rfAngle) const
{
    // Let (x,y,z) be the unit-length axis and let A be an angle of rotation.
    // The rotation matrix is R = I + sin(A)*P + (1-cos(A))*P^2 where
    // I is the identity and
    //
    //       +-        -+
    //   P = |  0 -z +y |
    //       | +z  0 -x |
    //       | -y +x  0 |
    //       +-        -+
    //
    // If A > 0, R represents a counterclockwise rotation about the axis in
    // the sense of looking from the tip of the axis vector towards the
    // origin.  Some algebra will show that
    //
    //   cos(A) = (trace(R)-1)/2  and  R - R^t = 2*sin(A)*P
    //
    // In the event that A = pi, R-R^t = 0 which prevents us from extracting
    // the axis through P.  Instead note that R = I+2*P^2 when A = pi, so
    // P^2 = (R-I)/2.  The diagonal entries of P^2 are x^2-1, y^2-1, and
    // z^2-1.  We can solve these for axis (x,y,z).  Because the angle is pi,
    // it does not matter which sign you choose on the square roots.

    nReal fTrace = m_afEntry[0] + m_afEntry[4] + m_afEntry[8];
    nReal fCos = ((nReal)0.5)*(fTrace-(nReal)1.0);
    rfAngle = CMath<nReal>::ACos(fCos);  // in [0,PI]

    if (rfAngle > (nReal)0.0)
    {
        if (rfAngle < CMath<nReal>::PI)
        {
            rkAxis[0] = m_afEntry[7]-m_afEntry[5];
            rkAxis[1] = m_afEntry[2]-m_afEntry[6];
            rkAxis[2] = m_afEntry[3]-m_afEntry[1];
            rkAxis.Normalize();
        }
        else
        {
            // angle is PI
            nReal fHalfInverse;
            if (m_afEntry[0] >= m_afEntry[4])
            {
                // r00 >= r11
                if (m_afEntry[0] >= m_afEntry[8])
                {
                    // r00 is maximum diagonal term
                    rkAxis[0] = ((nReal)0.5)*CMath<nReal>::Sqrt(m_afEntry[0] -
                        m_afEntry[4] - m_afEntry[8] + (nReal)1.0);
                    fHalfInverse = ((nReal)0.5)/rkAxis[0];
                    rkAxis[1] = fHalfInverse*m_afEntry[1];
                    rkAxis[2] = fHalfInverse*m_afEntry[2];
                }
                else
                {
                    // r22 is maximum diagonal term
                    rkAxis[2] = ((nReal)0.5)*CMath<nReal>::Sqrt(m_afEntry[8] -
                        m_afEntry[0] - m_afEntry[4] + (nReal)1.0);
                    fHalfInverse = ((nReal)0.5)/rkAxis[2];
                    rkAxis[0] = fHalfInverse*m_afEntry[2];
                    rkAxis[1] = fHalfInverse*m_afEntry[5];
                }
            }
            else
            {
                // r11 > r00
                if (m_afEntry[4] >= m_afEntry[8])
                {
                    // r11 is maximum diagonal term
                    rkAxis[1] = ((nReal)0.5)*CMath<nReal>::Sqrt(m_afEntry[4] -
                        m_afEntry[0] - m_afEntry[8] + (nReal)1.0);
                    fHalfInverse  = ((nReal)0.5)/rkAxis[1];
                    rkAxis[0] = fHalfInverse*m_afEntry[1];
                    rkAxis[2] = fHalfInverse*m_afEntry[5];
                }
                else
                {
                    // r22 is maximum diagonal term
                    rkAxis[2] = ((nReal)0.5)*CMath<nReal>::Sqrt(m_afEntry[8] -
                        m_afEntry[0] - m_afEntry[4] + (nReal)1.0);
                    fHalfInverse = ((nReal)0.5)/rkAxis[2];
                    rkAxis[0] = fHalfInverse*m_afEntry[2];
                    rkAxis[1] = fHalfInverse*m_afEntry[5];
                }
            }
        }
    }
    else
    {
        // The angle is 0 and the matrix is the identity.  Any axis will
        // work, so just use the x-axis.
        rkAxis[0] = (nReal)1.0;
        rkAxis[1] = (nReal)0.0;
        rkAxis[2] = (nReal)0.0;
    }
}
//----------------------------------------------------------------------------
template <class nReal>
void CMatrix3<nReal>::Orthonormalize ()
{
    // Algorithm uses Gram-Schmidt orthogonalization.  If 'this' matrix is
    // M = [m0|m1|m2], then orthonormal output matrix is Q = [q0|q1|q2],
    //
    //   q0 = m0/|m0|
    //   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
    //   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
    //
    // where |V| indicates length of vector V and A*B indicates dot
    // product of vectors A and B.

    // compute q0
    nReal fInvLength = CMath<nReal>::InvSqrt(m_afEntry[0]*m_afEntry[0] +
        m_afEntry[3]*m_afEntry[3] + m_afEntry[6]*m_afEntry[6]);

    m_afEntry[0] *= fInvLength;
    m_afEntry[3] *= fInvLength;
    m_afEntry[6] *= fInvLength;

    // compute q1
    nReal fDot0 = m_afEntry[0]*m_afEntry[1] + m_afEntry[3]*m_afEntry[4] +
        m_afEntry[6]*m_afEntry[7];

    m_afEntry[1] -= fDot0*m_afEntry[0];
    m_afEntry[4] -= fDot0*m_afEntry[3];
    m_afEntry[7] -= fDot0*m_afEntry[6];

    fInvLength = CMath<nReal>::InvSqrt(m_afEntry[1]*m_afEntry[1] +
        m_afEntry[4]*m_afEntry[4] + m_afEntry[7]*m_afEntry[7]);

    m_afEntry[1] *= fInvLength;
    m_afEntry[4] *= fInvLength;
    m_afEntry[7] *= fInvLength;

    // compute q2
    nReal fDot1 = m_afEntry[1]*m_afEntry[2] + m_afEntry[4]*m_afEntry[5] +
        m_afEntry[7]*m_afEntry[8];

    fDot0 = m_afEntry[0]*m_afEntry[2] + m_afEntry[3]*m_afEntry[5] +
        m_afEntry[6]*m_afEntry[8];

    m_afEntry[2] -= fDot0*m_afEntry[0] + fDot1*m_afEntry[1];
    m_afEntry[5] -= fDot0*m_afEntry[3] + fDot1*m_afEntry[4];
    m_afEntry[8] -= fDot0*m_afEntry[6] + fDot1*m_afEntry[7];

    fInvLength = CMath<nReal>::InvSqrt(m_afEntry[2]*m_afEntry[2] +
        m_afEntry[5]*m_afEntry[5] + m_afEntry[8]*m_afEntry[8]);

    m_afEntry[2] *= fInvLength;
    m_afEntry[5] *= fInvLength;
    m_afEntry[8] *= fInvLength;
}
//----------------------------------------------------------------------------
template <class nReal>
void CMatrix3<nReal>::EigenDecomposition (CMatrix3& rkRot, CMatrix3& rkDiag) const
{
    // Factor M = R*D*R^T.  The columns of R are the eigenvectors.  The
    // diagonal entries of D are the corresponding eigenvalues.
    nReal afDiag[3], afSubd[2];
    rkRot = *this;
    bool bReflection = rkRot.Tridiagonalize(afDiag,afSubd);
    bool bConverged = rkRot.QLAlgorithm(afDiag,afSubd);
    assert(bConverged);

    // (insertion) sort eigenvalues in increasing order, d0 <= d1 <= d2
    nInt32 i;
    nReal fSave;

    if (afDiag[1] < afDiag[0])
    {
        // swap d0 and d1
        fSave = afDiag[0];
        afDiag[0] = afDiag[1];
        afDiag[1] = fSave;

        // swap V0 and V1
        for (i = 0; i < 3; i++)
        {
            fSave = rkRot[i][0];
            rkRot[i][0] = rkRot[i][1];
            rkRot[i][1] = fSave;
        }
        bReflection = !bReflection;
    }

    if (afDiag[2] < afDiag[1])
    {
        // swap d1 and d2
        fSave = afDiag[1];
        afDiag[1] = afDiag[2];
        afDiag[2] = fSave;

        // swap V1 and V2
        for (i = 0; i < 3; i++)
        {
            fSave = rkRot[i][1];
            rkRot[i][1] = rkRot[i][2];
            rkRot[i][2] = fSave;
        }
        bReflection = !bReflection;
    }

    if (afDiag[1] < afDiag[0])
    {
        // swap d0 and d1
        fSave = afDiag[0];
        afDiag[0] = afDiag[1];
        afDiag[1] = fSave;

        // swap V0 and V1
        for (i = 0; i < 3; i++)
        {
            fSave = rkRot[i][0];
            rkRot[i][0] = rkRot[i][1];
            rkRot[i][1] = fSave;
        }
        bReflection = !bReflection;
    }

    rkDiag.MakeDiagonal(afDiag[0],afDiag[1],afDiag[2]);

    if (bReflection)
    {
        // The orthogonal transformation that diagonalizes M is a reflection.
        // Make the eigenvectors a right--handed system by changing sign on
        // the last column.
        rkRot[0][2] = -rkRot[0][2];
        rkRot[1][2] = -rkRot[1][2];
        rkRot[2][2] = -rkRot[2][2];
    }
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal>& CMatrix3<nReal>::FromEulerAnglesXYZ (nReal fXAngle, nReal fYAngle,
    nReal fZAngle)
{
    nReal fCos, fSin;

    fCos = CMath<nReal>::Cos(fXAngle);
    fSin = CMath<nReal>::Sin(fXAngle);
    CMatrix3 kXMat(
        (nReal)1.0,(nReal)0.0,(nReal)0.0,
        (nReal)0.0,fCos,-fSin,
        (nReal)0.0,fSin,fCos);

    fCos = CMath<nReal>::Cos(fYAngle);
    fSin = CMath<nReal>::Sin(fYAngle);
    CMatrix3 kYMat(
        fCos,(nReal)0.0,fSin,
        (nReal)0.0,(nReal)1.0,(nReal)0.0,
        -fSin,(nReal)0.0,fCos);

    fCos = CMath<nReal>::Cos(fZAngle);
    fSin = CMath<nReal>::Sin(fZAngle);
    CMatrix3 kZMat(
        fCos,-fSin,(nReal)0.0,
        fSin,fCos,(nReal)0.0,
        (nReal)0.0,(nReal)0.0,(nReal)1.0);

    *this = kXMat*(kYMat*kZMat);
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal>& CMatrix3<nReal>::FromEulerAnglesXZY (nReal fXAngle, nReal fZAngle,
    nReal fYAngle)
{
    nReal fCos, fSin;

    fCos = CMath<nReal>::Cos(fXAngle);
    fSin = CMath<nReal>::Sin(fXAngle);
    CMatrix3 kXMat(
        (nReal)1.0,(nReal)0.0,(nReal)0.0,
        (nReal)0.0,fCos,-fSin,
        (nReal)0.0,fSin,fCos);

    fCos = CMath<nReal>::Cos(fZAngle);
    fSin = CMath<nReal>::Sin(fZAngle);
    CMatrix3 kZMat(
        fCos,-fSin,(nReal)0.0,
        fSin,fCos,(nReal)0.0,
        (nReal)0.0,(nReal)0.0,(nReal)1.0);

    fCos = CMath<nReal>::Cos(fYAngle);
    fSin = CMath<nReal>::Sin(fYAngle);
    CMatrix3 kYMat(
        fCos,(nReal)0.0,fSin,
        (nReal)0.0,(nReal)1.0,(nReal)0.0,
        -fSin,(nReal)0.0,fCos);

    *this = kXMat*(kZMat*kYMat);
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal>& CMatrix3<nReal>::FromEulerAnglesYXZ (nReal fYAngle, nReal fXAngle,
    nReal fZAngle)
{
    nReal fCos, fSin;

    fCos = CMath<nReal>::Cos(fYAngle);
    fSin = CMath<nReal>::Sin(fYAngle);
    CMatrix3 kYMat(
        fCos,(nReal)0.0,fSin,
        (nReal)0.0,(nReal)1.0,(nReal)0.0,
        -fSin,(nReal)0.0,fCos);

    fCos = CMath<nReal>::Cos(fXAngle);
    fSin = CMath<nReal>::Sin(fXAngle);
    CMatrix3 kXMat(
        (nReal)1.0,(nReal)0.0,(nReal)0.0,
        (nReal)0.0,fCos,-fSin,
        (nReal)0.0,fSin,fCos);

    fCos = CMath<nReal>::Cos(fZAngle);
    fSin = CMath<nReal>::Sin(fZAngle);
    CMatrix3 kZMat(
        fCos,-fSin,(nReal)0.0,
        fSin,fCos,(nReal)0.0,
        (nReal)0.0,(nReal)0.0,(nReal)1.0);

    *this = kYMat*(kXMat*kZMat);
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal>& CMatrix3<nReal>::FromEulerAnglesYZX (nReal fYAngle, nReal fZAngle,
    nReal fXAngle)
{
    nReal fCos, fSin;

    fCos = CMath<nReal>::Cos(fYAngle);
    fSin = CMath<nReal>::Sin(fYAngle);
    CMatrix3 kYMat(
        fCos,(nReal)0.0,fSin,
        (nReal)0.0,(nReal)1.0,(nReal)0.0,
        -fSin,(nReal)0.0,fCos);

    fCos = CMath<nReal>::Cos(fZAngle);
    fSin = CMath<nReal>::Sin(fZAngle);
    CMatrix3 kZMat(
        fCos,-fSin,(nReal)0.0,
        fSin,fCos,(nReal)0.0,
        (nReal)0.0,(nReal)0.0,(nReal)1.0);

    fCos = CMath<nReal>::Cos(fXAngle);
    fSin = CMath<nReal>::Sin(fXAngle);
    CMatrix3 kXMat(
        (nReal)1.0,(nReal)0.0,(nReal)0.0,
        (nReal)0.0,fCos,-fSin,
        (nReal)0.0,fSin,fCos);

    *this = kYMat*(kZMat*kXMat);
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal>& CMatrix3<nReal>::FromEulerAnglesZXY (nReal fZAngle, nReal fXAngle,
    nReal fYAngle)
{
    nReal fCos, fSin;

    fCos = CMath<nReal>::Cos(fZAngle);
    fSin = CMath<nReal>::Sin(fZAngle);
    CMatrix3 kZMat(
        fCos,-fSin,(nReal)0.0,
        fSin,fCos,(nReal)0.0,
        (nReal)0.0,(nReal)0.0,(nReal)1.0);

    fCos = CMath<nReal>::Cos(fXAngle);
    fSin = CMath<nReal>::Sin(fXAngle);
    CMatrix3 kXMat(
        (nReal)1.0,(nReal)0.0,(nReal)0.0,
        (nReal)0.0,fCos,-fSin,
        (nReal)0.0,fSin,fCos);

    fCos = CMath<nReal>::Cos(fYAngle);
    fSin = CMath<nReal>::Sin(fYAngle);
    CMatrix3 kYMat(
        fCos,(nReal)0.0,fSin,
        (nReal)0.0,(nReal)1.0,(nReal)0.0,
        -fSin,(nReal)0.0,fCos);

    *this = kZMat*(kXMat*kYMat);
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal>& CMatrix3<nReal>::FromEulerAnglesZYX (nReal fZAngle, nReal fYAngle,
    nReal fXAngle)
{
    nReal fCos, fSin;

    fCos = CMath<nReal>::Cos(fZAngle);
    fSin = CMath<nReal>::Sin(fZAngle);
    CMatrix3 kZMat(
        fCos,-fSin,(nReal)0.0,
        fSin,fCos,(nReal)0.0,
        (nReal)0.0,(nReal)0.0,(nReal)1.0);

    fCos = CMath<nReal>::Cos(fYAngle);
    fSin = CMath<nReal>::Sin(fYAngle);
    CMatrix3 kYMat(
        fCos,(nReal)0.0,fSin,
        (nReal)0.0,(nReal)1.0,(nReal)0.0,
        -fSin,(nReal)0.0,fCos);

    fCos = CMath<nReal>::Cos(fXAngle);
    fSin = CMath<nReal>::Sin(fXAngle);
    CMatrix3 kXMat(
        (nReal)1.0,(nReal)0.0,(nReal)0.0,
        (nReal)0.0,fCos,-fSin,
        (nReal)0.0,fSin,fCos);

    *this = kZMat*(kYMat*kXMat);
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
typename CMatrix3<nReal>::EulerResult CMatrix3<nReal>::ToEulerAnglesXYZ (
    nReal& rfXAngle, nReal& rfYAngle, nReal& rfZAngle) const
{
    // +-           -+   +-                                        -+
    // | r00 r01 r02 |   |  cy*cz           -cy*sz            sy    |
    // | r10 r11 r12 | = |  cz*sx*sy+cx*sz   cx*cz-sx*sy*sz  -cy*sx |
    // | r20 r21 r22 |   | -cx*cz*sy+sx*sz   cz*sx+cx*sy*sz   cx*cy |
    // +-           -+   +-                                        -+

    if (m_afEntry[2] < (nReal)1.0)
    {
        if (m_afEntry[2] > -(nReal)1.0)
        {
            // y_angle = asin(r02)
            // x_angle = atan2(-r12,r22)
            // z_angle = atan2(-r01,r00)
            rfYAngle = (nReal)asin((double)m_afEntry[2]);
            rfXAngle = CMath<nReal>::ATan2(-m_afEntry[5],m_afEntry[8]);
            rfZAngle = CMath<nReal>::ATan2(-m_afEntry[1],m_afEntry[0]);
            return EA_UNIQUE;
        }
        else
        {
            // y_angle = -pi/2
            // z_angle - x_angle = atan2(r10,r11)
            // WARNING.  The solution is not unique.  Choosing z_angle = 0.
            rfYAngle = -CMath<nReal>::HALF_PI;
            rfXAngle = -CMath<nReal>::ATan2(m_afEntry[3],m_afEntry[4]);
            rfZAngle = (nReal)0.0;
            return EA_NOT_UNIQUE_DIF;
        }
    }
    else
    {
        // y_angle = +pi/2
        // z_angle + x_angle = atan2(r10,r11)
        // WARNING.  The solutions is not unique.  Choosing z_angle = 0.
        rfYAngle = CMath<nReal>::HALF_PI;
        rfXAngle = CMath<nReal>::ATan2(m_afEntry[3],m_afEntry[4]);
        rfZAngle = (nReal)0.0;
        return EA_NOT_UNIQUE_SUM;
    }
}
//----------------------------------------------------------------------------
template <class nReal>
typename CMatrix3<nReal>::EulerResult CMatrix3<nReal>::ToEulerAnglesXZY (
    nReal& rfXAngle, nReal& rfZAngle, nReal& rfYAngle) const
{
    // +-           -+   +-                                        -+
    // | r00 r01 r02 |   |  cy*cz           -sz      cz*sy          |
    // | r10 r11 r12 | = |  sx*sy+cx*cy*sz   cx*cz  -cy*sx+cx*sy*sz |
    // | r20 r21 r22 |   | -cx*sy+cy*sx*sz   cz*sx   cx*cy+sx*sy*sz |
    // +-           -+   +-                                        -+

    if (m_afEntry[1] < (nReal)1.0)
    {
        if (m_afEntry[1] > -(nReal)1.0)
        {
            // z_angle = asin(-r01)
            // x_angle = atan2(r21,r11)
            // y_angle = atan2(r02,r00)
            rfZAngle = (nReal)asin(-(double)m_afEntry[1]);
            rfXAngle = CMath<nReal>::ATan2(m_afEntry[7],m_afEntry[4]);
            rfYAngle = CMath<nReal>::ATan2(m_afEntry[2],m_afEntry[0]);
            return EA_UNIQUE;
        }
        else
        {
            // z_angle = +pi/2
            // y_angle - x_angle = atan2(-r20,r22)
            // WARNING.  The solution is not unique.  Choosing y_angle = 0.
            rfZAngle = CMath<nReal>::HALF_PI;
            rfXAngle = -CMath<nReal>::ATan2(-m_afEntry[6],m_afEntry[8]);
            rfYAngle = (nReal)0.0;
            return EA_NOT_UNIQUE_DIF;
        }
    }
    else
    {
        // z_angle = -pi/2
        // y_angle + x_angle = atan2(-r20,r22)
        // WARNING.  The solution is not unique.  Choosing y_angle = 0.
        rfZAngle = -CMath<nReal>::HALF_PI;
        rfXAngle = CMath<nReal>::ATan2(-m_afEntry[6],m_afEntry[8]);
        rfYAngle = (nReal)0.0;
        return EA_NOT_UNIQUE_SUM;
    }
}
//----------------------------------------------------------------------------
template <class nReal>
typename CMatrix3<nReal>::EulerResult CMatrix3<nReal>::ToEulerAnglesYXZ (
    nReal& rfYAngle, nReal& rfXAngle, nReal& rfZAngle) const
{
    // +-           -+   +-                                       -+
    // | r00 r01 r02 |   |  cy*cz+sx*sy*sz  cz*sx*sy-cy*sz   cx*sy |
    // | r10 r11 r12 | = |  cx*sz           cx*cz           -sx    |
    // | r20 r21 r22 |   | -cz*sy+cy*sx*sz  cy*cz*sx+sy*sz   cx*cy |
    // +-           -+   +-                                       -+

    if (m_afEntry[5] < (nReal)1.0)
    {
        if (m_afEntry[5] > -(nReal)1.0)
        {
            // x_angle = asin(-r12)
            // y_angle = atan2(r02,r22)
            // z_angle = atan2(r10,r11)
            rfXAngle = (nReal)asin(-(double)m_afEntry[5]);
            rfYAngle = CMath<nReal>::ATan2(m_afEntry[2],m_afEntry[8]);
            rfZAngle = CMath<nReal>::ATan2(m_afEntry[3],m_afEntry[4]);
            return EA_UNIQUE;
        }
        else
        {
            // x_angle = +pi/2
            // z_angle - y_angle = atan2(-r01,r00)
            // WARNING.  The solution is not unique.  Choosing z_angle = 0.
            rfXAngle = CMath<nReal>::HALF_PI;
            rfYAngle = -CMath<nReal>::ATan2(-m_afEntry[1],m_afEntry[0]);
            rfZAngle = (nReal)0.0;
            return EA_NOT_UNIQUE_DIF;
        }
    }
    else
    {
        // x_angle = -pi/2
        // z_angle + y_angle = atan2(-r01,r00)
        // WARNING.  The solution is not unique.  Choosing z_angle = 0.
        rfXAngle = -CMath<nReal>::HALF_PI;
        rfYAngle = CMath<nReal>::ATan2(-m_afEntry[1],m_afEntry[0]);
        rfZAngle = (nReal)0.0;
        return EA_NOT_UNIQUE_SUM;
    }
}
//----------------------------------------------------------------------------
template <class nReal>
typename CMatrix3<nReal>::EulerResult CMatrix3<nReal>::ToEulerAnglesYZX (
    nReal& rfYAngle, nReal& rfZAngle, nReal& rfXAngle) const
{
    // +-           -+   +-                                       -+
    // | r00 r01 r02 |   |  cy*cz  sx*sy-cx*cy*sz   cx*sy+cy*sx*sz |
    // | r10 r11 r12 | = |  sz     cx*cz           -cz*sx          |
    // | r20 r21 r22 |   | -cz*sy  cy*sx+cx*sy*sz   cx*cy-sx*sy*sz |
    // +-           -+   +-                                       -+

    if (m_afEntry[3] < (nReal)1.0)
    {
        if (m_afEntry[3] > -(nReal)1.0)
        {
            // z_angle = asin(r10)
            // y_angle = atan2(-r20,r00)
            // x_angle = atan2(-r12,r11)
            rfZAngle = (nReal)asin((double)m_afEntry[3]);
            rfYAngle = CMath<nReal>::ATan2(-m_afEntry[6],m_afEntry[0]);
            rfXAngle = CMath<nReal>::ATan2(-m_afEntry[5],m_afEntry[4]);
            return EA_UNIQUE;
        }
        else
        {
            // z_angle = -pi/2
            // x_angle - y_angle = atan2(r21,r22)
            // WARNING.  The solution is not unique.  Choosing x_angle = 0.
            rfZAngle = -CMath<nReal>::HALF_PI;
            rfYAngle = -CMath<nReal>::ATan2(m_afEntry[7],m_afEntry[8]);
            rfXAngle = (nReal)0.0;
            return EA_NOT_UNIQUE_DIF;
        }
    }
    else
    {
        // z_angle = +pi/2
        // x_angle + y_angle = atan2(r21,r22)
        // WARNING.  The solution is not unique.  Choosing x_angle = 0.
        rfZAngle = CMath<nReal>::HALF_PI;
        rfYAngle = CMath<nReal>::ATan2(m_afEntry[7],m_afEntry[8]);
        rfXAngle = (nReal)0.0;
        return EA_NOT_UNIQUE_SUM;
    }
}
//----------------------------------------------------------------------------
template <class nReal>
typename CMatrix3<nReal>::EulerResult CMatrix3<nReal>::ToEulerAnglesZXY (
    nReal& rfZAngle, nReal& rfXAngle, nReal& rfYAngle) const
{
    // +-           -+   +-                                        -+
    // | r00 r01 r02 |   |  cy*cz-sx*sy*sz  -cx*sz   cz*sy+cy*sx*sz |
    // | r10 r11 r12 | = |  cz*sx*sy+cy*sz   cx*cz  -cy*cz*sx+sy*sz |
    // | r20 r21 r22 |   | -cx*sy            sx      cx*cy          |
    // +-           -+   +-                                        -+

    if (m_afEntry[7] < (nReal)1.0)
    {
        if (m_afEntry[7] > -(nReal)1.0)
        {
            // x_angle = asin(r21)
            // z_angle = atan2(-r01,r11)
            // y_angle = atan2(-r20,r22)
            rfXAngle = (nReal)asin((double)m_afEntry[7]);
            rfZAngle = CMath<nReal>::ATan2(-m_afEntry[1],m_afEntry[4]);
            rfYAngle = CMath<nReal>::ATan2(-m_afEntry[6],m_afEntry[8]);
            return EA_UNIQUE;
        }
        else
        {
            // x_angle = -pi/2
            // y_angle - z_angle = atan2(r02,r00)
            // WARNING.  The solution is not unique.  Choosing y_angle = 0.
            rfXAngle = -CMath<nReal>::HALF_PI;
            rfZAngle = -CMath<nReal>::ATan2(m_afEntry[2],m_afEntry[0]);
            rfYAngle = (nReal)0.0;
            return EA_NOT_UNIQUE_DIF;
        }
    }
    else
    {
        // x_angle = +pi/2
        // y_angle + z_angle = atan2(r02,r00)
        // WARNING.  The solution is not unique.  Choosing y_angle = 0.
        rfXAngle = CMath<nReal>::HALF_PI;
        rfZAngle = CMath<nReal>::ATan2(m_afEntry[2],m_afEntry[0]);
        rfYAngle = (nReal)0.0;
        return EA_NOT_UNIQUE_SUM;
    }
}
//----------------------------------------------------------------------------
template <class nReal>
typename CMatrix3<nReal>::EulerResult CMatrix3<nReal>::ToEulerAnglesZYX (
    nReal& rfZAngle, nReal& rfYAngle, nReal& rfXAngle) const
{
    // +-           -+   +-                                      -+
    // | r00 r01 r02 |   |  cy*cz  cz*sx*sy-cx*sz  cx*cz*sy+sx*sz |
    // | r10 r11 r12 | = |  cy*sz  cx*cz+sx*sy*sz -cz*sx+cx*sy*sz |
    // | r20 r21 r22 |   | -sy     cy*sx           cx*cy          |
    // +-           -+   +-                                      -+

    if (m_afEntry[6] < (nReal)1.0)
    {
        if (m_afEntry[6] > -(nReal)1.0)
        {
            // y_angle = asin(-r20)
            // z_angle = atan2(r10,r00)
            // x_angle = atan2(r21,r22)
            rfYAngle = (nReal)asin(-(double)m_afEntry[6]);
            rfZAngle = CMath<nReal>::ATan2(m_afEntry[3],m_afEntry[0]);
            rfXAngle = CMath<nReal>::ATan2(m_afEntry[7],m_afEntry[8]);
            return EA_UNIQUE;
        }
        else
        {
            // y_angle = +pi/2
            // x_angle - z_angle = atan2(r01,r02)
            // WARNING.  The solution is not unique.  Choosing x_angle = 0.
            rfYAngle = CMath<nReal>::HALF_PI;
            rfZAngle = -CMath<nReal>::ATan2(m_afEntry[1],m_afEntry[2]);
            rfXAngle = (nReal)0.0;
            return EA_NOT_UNIQUE_DIF;
        }
    }
    else
    {
        // y_angle = -pi/2
        // x_angle + z_angle = atan2(-r01,-r02)
        // WARNING.  The solution is not unique.  Choosing x_angle = 0;
        rfYAngle = -CMath<nReal>::HALF_PI;
        rfZAngle = CMath<nReal>::ATan2(-m_afEntry[1],-m_afEntry[2]);
        rfXAngle = (nReal)0.0;
        return EA_NOT_UNIQUE_SUM;
    }
}
//----------------------------------------------------------------------------
template <class nReal>
typename CMatrix3<nReal>::EulerResult CMatrix3<nReal>::ToEulerAnglesXYX (
    nReal& rfX0Angle, nReal& rfYAngle, nReal& rfX1Angle) const
{
    // +-           -+   +-                                                -+
    // | r00 r01 r02 |   |  cy      sy*sx1               sy*cx1             |
    // | r10 r11 r12 | = |  sy*sx0  cx0*cx1-cy*sx0*sx1  -cy*cx1*sx0-cx0*sx1 |
    // | r20 r21 r22 |   | -sy*cx0  cx1*sx0+cy*cx0*sx1   cy*cx0*cx1-sx0*sx1 |
    // +-           -+   +-                                                -+

    if (m_afEntry[0] < (nReal)1.0)
    {
        if (m_afEntry[0] > -(nReal)1.0)
        {
            // y_angle  = acos(r00)
            // x0_angle = atan2(r10,-r20)
            // x1_angle = atan2(r01,r02)
            rfYAngle = (nReal)acos((double)m_afEntry[0]);
            rfX0Angle = CMath<nReal>::ATan2(m_afEntry[3],-m_afEntry[6]);
            rfX1Angle = CMath<nReal>::ATan2(m_afEntry[1],m_afEntry[2]);
            return EA_UNIQUE;
        }
        else
        {
            // Not a unique solution:  x1_angle - x0_angle = atan2(-r12,r11)
            rfYAngle = CMath<nReal>::PI;
            rfX0Angle = -CMath<nReal>::ATan2(-m_afEntry[5],m_afEntry[4]);
            rfX1Angle = (nReal)0.0;
            return EA_NOT_UNIQUE_DIF;
        }
    }
    else
    {
        // Not a unique solution:  x1_angle + x0_angle = atan2(-r12,r11)
        rfYAngle = (nReal)0.0;
        rfX0Angle = CMath<nReal>::ATan2(-m_afEntry[5],m_afEntry[4]);
        rfX1Angle = (nReal)0.0;
        return EA_NOT_UNIQUE_SUM;
    }
}
//----------------------------------------------------------------------------
template <class nReal>
typename CMatrix3<nReal>::EulerResult CMatrix3<nReal>::ToEulerAnglesXZX (
    nReal& rfX0Angle, nReal& rfZAngle, nReal& rfX1Angle) const
{
    // +-           -+   +-                                                -+
    // | r00 r01 r02 |   | cz      -sz*cx1               sz*sx1             |
    // | r10 r11 r12 | = | sz*cx0   cz*cx0*cx1-sx0*sx1  -cx1*sx0-cz*cx0*sx1 |
    // | r20 r21 r22 |   | sz*sx0   cz*cx1*sx0+cx0*sx1   cx0*cx1-cz*sx0*sx1 |
    // +-           -+   +-                                                -+

    if (m_afEntry[0] < (nReal)1.0)
    {
        if (m_afEntry[0] > -(nReal)1.0)
        {
            // z_angle  = acos(r00)
            // x0_angle = atan2(r20,r10)
            // x1_angle = atan2(r02,-r01)
            rfZAngle = (nReal)acos((double)m_afEntry[0]);
            rfX0Angle = CMath<nReal>::ATan2(m_afEntry[6],m_afEntry[3]);
            rfX1Angle = CMath<nReal>::ATan2(m_afEntry[2],-m_afEntry[1]);
            return EA_UNIQUE;
        }
        else
        {
            // Not a unique solution:  x1_angle - x0_angle = atan2(r21,r22)
            rfZAngle = CMath<nReal>::PI;
            rfX0Angle = -CMath<nReal>::ATan2(m_afEntry[7],m_afEntry[8]);
            rfX1Angle = (nReal)0.0;
            return EA_NOT_UNIQUE_DIF;
        }
    }
    else
    {
        // Not a unique solution:  x1_angle + x0_angle = atan2(r21,r22)
        rfZAngle = (nReal)0.0;
        rfX0Angle = CMath<nReal>::ATan2(m_afEntry[7],m_afEntry[8]);
        rfX1Angle = (nReal)0.0;
        return EA_NOT_UNIQUE_SUM;
    }
}
//----------------------------------------------------------------------------
template <class nReal>
typename CMatrix3<nReal>::EulerResult CMatrix3<nReal>::ToEulerAnglesYXY (
    nReal& rfY0Angle, nReal& rfXAngle, nReal& rfY1Angle) const
{
    // +-           -+   +-                                                -+
    // | r00 r01 r02 |   |  cy0*cy1-cx*sy0*sy1  sx*sy0   cx*cy1*sy0+cy0*sy1 |
    // | r10 r11 r12 | = |  sx*sy1              cx      -sx*cy1             |
    // | r20 r21 r22 |   | -cy1*sy0-cx*cy0*sy1  sx*cy0   cx*cy0*cy1-sy0*sy1 |
    // +-           -+   +-                                                -+

    if (m_afEntry[4] < (nReal)1.0)
    {
        if (m_afEntry[4] > -(nReal)1.0)
        {
            // x_angle  = acos(r11)
            // y0_angle = atan2(r01,r21)
            // y1_angle = atan2(r10,-r12)
            rfXAngle = (nReal)acos((double)m_afEntry[4]);
            rfY0Angle = CMath<nReal>::ATan2(m_afEntry[1],m_afEntry[7]);
            rfY1Angle = CMath<nReal>::ATan2(m_afEntry[3],-m_afEntry[5]);
            return EA_UNIQUE;
        }
        else
        {
            // Not a unique solution:  y1_angle - y0_angle = atan2(r02,r00)
            rfXAngle = CMath<nReal>::PI;
            rfY0Angle = -CMath<nReal>::ATan2(m_afEntry[2],m_afEntry[0]);
            rfY1Angle = (nReal)0.0;
            return EA_NOT_UNIQUE_DIF;
        }
    }
    else
    {
        // Not a unique solution:  y1_angle + y0_angle = atan2(r02,r00)
        rfXAngle = (nReal)0.0;
        rfY0Angle = CMath<nReal>::ATan2(m_afEntry[2],m_afEntry[0]);
        rfY1Angle = (nReal)0.0;
        return EA_NOT_UNIQUE_SUM;
    }
}
//----------------------------------------------------------------------------
template <class nReal>
typename CMatrix3<nReal>::EulerResult CMatrix3<nReal>::ToEulerAnglesYZY (
    nReal& rfY0Angle, nReal& rfZAngle, nReal& rfY1Angle) const
{
    // +-           -+   +-                                                -+
    // | r00 r01 r02 |   |  cz*cy0*cy1-sy0*sy1  -sz*cy0  cy1*sy0+cz*cy0*sy1 |
    // | r10 r11 r12 | = |  sz*cy1               cz      sz*sy1             |
    // | r20 r21 r22 |   | -cz*cy1*sy0-cy0*sy1   sz*sy0  cy0*cy1-cz*sy0*sy1 |
    // +-           -+   +-                                                -+

    if (m_afEntry[4] < (nReal)1.0)
    {
        if (m_afEntry[4] > -(nReal)1.0)
        {
            // z_angle  = acos(r11)
            // y0_angle = atan2(r21,-r01)
            // y1_angle = atan2(r12,r10)
            rfZAngle = (nReal)acos((double)m_afEntry[4]);
            rfY0Angle = CMath<nReal>::ATan2(m_afEntry[7],-m_afEntry[1]);
            rfY1Angle = CMath<nReal>::ATan2(m_afEntry[5],m_afEntry[3]);
            return EA_UNIQUE;
        }
        else
        {
            // Not a unique solution:  y1_angle - y0_angle = atan2(-r20,r22)
            rfZAngle = CMath<nReal>::PI;
            rfY0Angle = -CMath<nReal>::ATan2(-m_afEntry[6],m_afEntry[8]);
            rfY1Angle = (nReal)0.0;
            return EA_NOT_UNIQUE_DIF;
        }
    }
    else
    {
        // Not a unique solution:  y1_angle + y0_angle = atan2(-r20,r22)
        rfZAngle = (nReal)0.0;
        rfY0Angle = CMath<nReal>::ATan2(-m_afEntry[6],m_afEntry[8]);
        rfY1Angle = (nReal)0.0;
        return EA_NOT_UNIQUE_SUM;
    }
}
//----------------------------------------------------------------------------
template <class nReal>
typename CMatrix3<nReal>::EulerResult CMatrix3<nReal>::ToEulerAnglesZXZ (
    nReal& rfZ0Angle, nReal& rfXAngle, nReal& rfZ1Angle) const
{
    // +-           -+   +-                                                -+
    // | r00 r01 r02 |   | cz0*cz1-cx*sz0*sz1  -cx*cz1*sz0-cz0*sz1   sx*sz0 |
    // | r10 r11 r12 | = | cz1*sz0+cx*cz0*sz1   cx*cz0*cz1-sz0*sz1  -sz*cz0 |
    // | r20 r21 r22 |   | sx*sz1               sx*cz1               cx     |
    // +-           -+   +-                                                -+

    if (m_afEntry[8] < (nReal)1.0)
    {
        if (m_afEntry[8] > -(nReal)1.0)
        {
            // x_angle  = acos(r22)
            // z0_angle = atan2(r02,-r12)
            // z1_angle = atan2(r20,r21)
            rfXAngle = (nReal)acos((double)m_afEntry[8]);
            rfZ0Angle = CMath<nReal>::ATan2(m_afEntry[2],-m_afEntry[5]);
            rfZ1Angle = CMath<nReal>::ATan2(m_afEntry[6],m_afEntry[7]);
            return EA_UNIQUE;
        }
        else
        {
            // Not a unique solution:  z1_angle - z0_angle = atan2(-r01,r00)
            rfXAngle = CMath<nReal>::PI;
            rfZ0Angle = -CMath<nReal>::ATan2(-m_afEntry[1],m_afEntry[0]);
            rfZ1Angle = (nReal)0.0;
            return EA_NOT_UNIQUE_DIF;
        }
    }
    else
    {
        // Not a unique solution:  z1_angle + z0_angle = atan2(-r01,r00)
        rfXAngle = (nReal)0.0;
        rfZ0Angle = CMath<nReal>::ATan2(-m_afEntry[1],m_afEntry[0]);
        rfZ1Angle = (nReal)0.0;
        return EA_NOT_UNIQUE_SUM;
    }
}
//----------------------------------------------------------------------------
template <class nReal>
typename CMatrix3<nReal>::EulerResult CMatrix3<nReal>::ToEulerAnglesZYZ (
    nReal& rfZ0Angle, nReal& rfYAngle, nReal& rfZ1Angle) const
{
    // +-           -+   +-                                                -+
    // | r00 r01 r02 |   |  cy*cz0*cz1-sz0*sz1  -cz1*sz0-cy*cz0*sz1  sy*cz0 |
    // | r10 r11 r12 | = |  cy*cz1*sz0+cz0*sz1   cz0*cz1-cy*sz0*sz1  sy*sz0 |
    // | r20 r21 r22 |   | -sy*cz1               sy*sz1              cy     |
    // +-           -+   +-                                                -+

    if (m_afEntry[8] < (nReal)1.0)
    {
        if (m_afEntry[8] > -(nReal)1.0)
        {
            // y_angle  = acos(r22)
            // z0_angle = atan2(r12,r02)
            // z1_angle = atan2(r21,-r20)
            rfYAngle = (nReal)acos((double)m_afEntry[8]);
            rfZ0Angle = CMath<nReal>::ATan2(m_afEntry[5],m_afEntry[2]);
            rfZ1Angle = CMath<nReal>::ATan2(m_afEntry[7],-m_afEntry[6]);
            return EA_UNIQUE;
        }
        else // r22 = -1
        {
            // Not a unique solution:  z1_angle - z0_angle = atan2(r10,r11)
            rfYAngle = CMath<nReal>::PI;
            rfZ0Angle = -CMath<nReal>::ATan2(m_afEntry[3],m_afEntry[4]);
            rfZ1Angle = (nReal)0.0;
            return EA_NOT_UNIQUE_DIF;
        }
    }
    else // r22 = +1
    {
        // Not a unique solution:  z1_angle + z0_angle = atan2(r10,r11)
        rfYAngle = (nReal)0.0;
        rfZ0Angle = CMath<nReal>::ATan2(m_afEntry[3],m_afEntry[4]);
        rfZ1Angle = (nReal)0.0;
        return EA_NOT_UNIQUE_SUM;
    }
}
//----------------------------------------------------------------------------
template <class nReal>
CMatrix3<nReal>& CMatrix3<nReal>::Slerp (nReal fT, const CMatrix3& rkR0,
    const CMatrix3& rkR1)
{
    CVector3<nReal> kAxis;
    nReal fAngle;
    CMatrix3 kProd = rkR0.TransposeTimes(rkR1);
    kProd.ToAxisAngle(kAxis,fAngle);
    FromAxisAngle(kAxis,fT*fAngle);
    *this = rkR0*(*this);
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
bool CMatrix3<nReal>::Tridiagonalize (nReal afDiag[3], nReal afSubd[2])
{
    // Householder reduction T = Q^t M Q
    //   Input:   
    //     mat, symmetric 3x3 matrix M
    //   Output:  
    //     mat, orthogonal matrix Q (a reflection)
    //     diag, diagonal entries of T
    //     subd, subdiagonal entries of T (T is symmetric)

    nReal fM00 = m_afEntry[0];
    nReal fM01 = m_afEntry[1];
    nReal fM02 = m_afEntry[2];
    nReal fM11 = m_afEntry[4];
    nReal fM12 = m_afEntry[5];
    nReal fM22 = m_afEntry[8];

    afDiag[0] = fM00;
    if (CMath<nReal>::FAbs(fM02) >= CMath<nReal>::ZERO_TOLERANCE)
    {
        afSubd[0] = CMath<nReal>::Sqrt(fM01*fM01+fM02*fM02);
        nReal fInvLength = ((nReal)1.0)/afSubd[0];
        fM01 *= fInvLength;
        fM02 *= fInvLength;
        nReal fTmp = ((nReal)2.0)*fM01*fM12+fM02*(fM22-fM11);
        afDiag[1] = fM11+fM02*fTmp;
        afDiag[2] = fM22-fM02*fTmp;
        afSubd[1] = fM12-fM01*fTmp;

        m_afEntry[0] = (nReal)1.0;
        m_afEntry[1] = (nReal)0.0;
        m_afEntry[2] = (nReal)0.0;
        m_afEntry[3] = (nReal)0.0;
        m_afEntry[4] = fM01;
        m_afEntry[5] = fM02;
        m_afEntry[6] = (nReal)0.0;
        m_afEntry[7] = fM02;
        m_afEntry[8] = -fM01;
        return true;
    }
    else
    {
        afDiag[1] = fM11;
        afDiag[2] = fM22;
        afSubd[0] = fM01;
        afSubd[1] = fM12;

        m_afEntry[0] = (nReal)1.0;
        m_afEntry[1] = (nReal)0.0;
        m_afEntry[2] = (nReal)0.0;
        m_afEntry[3] = (nReal)0.0;
        m_afEntry[4] = (nReal)1.0;
        m_afEntry[5] = (nReal)0.0;
        m_afEntry[6] = (nReal)0.0;
        m_afEntry[7] = (nReal)0.0;
        m_afEntry[8] = (nReal)1.0;
        return false;
    }
}
//----------------------------------------------------------------------------
template <class nReal>
bool CMatrix3<nReal>::QLAlgorithm (nReal afDiag[3], nReal afSubd[2])
{
    // This is an implementation of the symmetric QR algorithm from the book
    // "Matrix Computations" by Gene H. Golub and Charles F. Van Loan, second
    // edition.  The algorithm is 8.2.3.  The implementation has a slight
    // variation to actually make it a QL algorithm, and it traps the case
    // when either of the subdiagonal terms s0 or s1 is zero and reduces the
    // 2-by-2 subblock directly.

    const nInt32 iMax = 32;
    for (nInt32 i = 0; i < iMax; i++)
    {
        nReal fSum, fDiff, fDiscr, fEValue0, fEValue1, fCos, fSin, fTmp;
        nInt32 iRow;

        fSum = CMath<nReal>::FAbs(afDiag[0]) + CMath<nReal>::FAbs(afDiag[1]);
        if (CMath<nReal>::FAbs(afSubd[0]) + fSum == fSum)
        {
            // The matrix is effectively
            //       +-        -+
            //   M = | d0  0  0 |
            //       | 0  d1 s1 |
            //       | 0  s1 d2 |
            //       +-        -+

            // Compute the eigenvalues as roots of a quadratic equation.
            fSum = afDiag[1] + afDiag[2];
            fDiff = afDiag[1] - afDiag[2];
            fDiscr = CMath<nReal>::Sqrt(fDiff*fDiff +
                ((nReal)4.0)*afSubd[1]*afSubd[1]);
            fEValue0 = ((nReal)0.5)*(fSum - fDiscr);
            fEValue1 = ((nReal)0.5)*(fSum + fDiscr);

            // Compute the Givens rotation.
            if (fDiff >= (nReal)0.0)
            {
                fCos = afSubd[1];
                fSin = afDiag[1] - fEValue0;
            }
            else
            {
                fCos = afDiag[2] - fEValue0;
                fSin = afSubd[1];
            }
            fTmp = CMath<nReal>::InvSqrt(fCos*fCos + fSin*fSin);
            fCos *= fTmp;
            fSin *= fTmp;

            // Postmultiply the current orthogonal matrix with the Givens
            // rotation.
            for (iRow = 0; iRow < 3; iRow++)
            {
                fTmp = m_afEntry[2+3*iRow];
                m_afEntry[2+3*iRow] = fSin*m_afEntry[1+3*iRow] + fCos*fTmp;
                m_afEntry[1+3*iRow] = fCos*m_afEntry[1+3*iRow] - fSin*fTmp;
            }

            // Update the tridiagonal matrix.
            afDiag[1] = fEValue0;
            afDiag[2] = fEValue1;
            afSubd[0] = (nReal)0.0;
            afSubd[1] = (nReal)0.0;
            return true;
        }

        fSum = CMath<nReal>::FAbs(afDiag[1]) + CMath<nReal>::FAbs(afDiag[2]);
        if (CMath<nReal>::FAbs(afSubd[1]) + fSum == fSum)
        {
            // The matrix is effectively
            //       +-         -+
            //   M = | d0  s0  0 |
            //       | s0  d1  0 |
            //       | 0   0  d2 |
            //       +-         -+

            // Compute the eigenvalues as roots of a quadratic equation.
            fSum = afDiag[0] + afDiag[1];
            fDiff = afDiag[0] - afDiag[1];
            fDiscr = CMath<nReal>::Sqrt(fDiff*fDiff +
                ((nReal)4.0)*afSubd[0]*afSubd[0]);
            fEValue0 = ((nReal)0.5)*(fSum - fDiscr);
            fEValue1 = ((nReal)0.5)*(fSum + fDiscr);

            // Compute the Givens rotation.
            if (fDiff >= (nReal)0.0)
            {
                fCos = afSubd[0];
                fSin = afDiag[0] - fEValue0;
            }
            else
            {
                fCos = afDiag[1] - fEValue0;
                fSin = afSubd[0];
            }
            fTmp = CMath<nReal>::InvSqrt(fCos*fCos + fSin*fSin);
            fCos *= fTmp;
            fSin *= fTmp;

            // Postmultiply the current orthogonal matrix with the Givens
            // rotation.
            for (iRow = 0; iRow < 3; iRow++)
            {
                fTmp = m_afEntry[1+3*iRow];
                m_afEntry[1+3*iRow] = fSin*m_afEntry[0+3*iRow] + fCos*fTmp;
                m_afEntry[0+3*iRow] = fCos*m_afEntry[0+3*iRow] - fSin*fTmp;
            }

            // Update the tridiagonal matrix.
            afDiag[0] = fEValue0;
            afDiag[1] = fEValue1;
            afSubd[0] = (nReal)0.0;
            afSubd[1] = (nReal)0.0;
            return true;
        }

        // The matrix is
        //       +-        -+
        //   M = | d0 s0  0 |
        //       | s0 d1 s1 |
        //       | 0  s1 d2 |
        //       +-        -+

        // Set up the parameters for the first pass of the QL step.  The
        // value for A is the difference between diagonal term D[2] and the
        // implicit shift suggested by Wilkinson.
        nReal fRatio = (afDiag[1]-afDiag[0])/(((nReal)2.0f)*afSubd[0]);
        nReal fRoot = CMath<nReal>::Sqrt((nReal)1.0 + fRatio*fRatio);
        nReal fB = afSubd[1];
        nReal fA = afDiag[2] - afDiag[0];
        if (fRatio >= (nReal)0.0)
        {
            fA += afSubd[0]/(fRatio + fRoot);
        }
        else
        {
            fA += afSubd[0]/(fRatio - fRoot);
        }

        // Compute the Givens rotation for the first pass.
        if (CMath<nReal>::FAbs(fB) >= CMath<nReal>::FAbs(fA))
        {
            fRatio = fA/fB;
            fSin = CMath<nReal>::InvSqrt((nReal)1.0 + fRatio*fRatio);
            fCos = fRatio*fSin;
        }
        else
        {
            fRatio = fB/fA;
            fCos = CMath<nReal>::InvSqrt((nReal)1.0 + fRatio*fRatio);
            fSin = fRatio*fCos;
        }

        // Postmultiply the current orthogonal matrix with the Givens
        // rotation.
        for (iRow = 0; iRow < 3; iRow++)
        {
            fTmp = m_afEntry[2+3*iRow];
            m_afEntry[2+3*iRow] = fSin*m_afEntry[1+3*iRow]+fCos*fTmp;
            m_afEntry[1+3*iRow] = fCos*m_afEntry[1+3*iRow]-fSin*fTmp;
        }

        // Set up the parameters for the second pass of the QL step.  The
        // values tmp0 and tmp1 are required to fully update the tridiagonal
        // matrix at the end of the second pass.
        nReal fTmp0 = (afDiag[1] - afDiag[2])*fSin +
            ((nReal)2.0)*afSubd[1]*fCos;
        nReal fTmp1 = fCos*afSubd[0];
        fB = fSin*afSubd[0];
        fA = fCos*fTmp0 - afSubd[1];
        fTmp0 *= fSin;

        // Compute the Givens rotation for the second pass.  The subdiagonal
        // term S[1] in the tridiagonal matrix is updated at this time.
        if (CMath<nReal>::FAbs(fB) >= CMath<nReal>::FAbs(fA))
        {
            fRatio = fA/fB;
            fRoot = CMath<nReal>::Sqrt((nReal)1.0 + fRatio*fRatio);
            afSubd[1] = fB*fRoot;
            fSin = ((nReal)1.0)/fRoot;
            fCos = fRatio*fSin;
        }
        else
        {
            fRatio = fB/fA;
            fRoot = CMath<nReal>::Sqrt((nReal)1.0 + fRatio*fRatio);
            afSubd[1] = fA*fRoot;
            fCos = ((nReal)1.0)/fRoot;
            fSin = fRatio*fCos;
        }

        // Postmultiply the current orthogonal matrix with the Givens
        // rotation.
        for (iRow = 0; iRow < 3; iRow++)
        {
            fTmp = m_afEntry[1+3*iRow];
            m_afEntry[1+3*iRow] = fSin*m_afEntry[0+3*iRow]+fCos*fTmp;
            m_afEntry[0+3*iRow] = fCos*m_afEntry[0+3*iRow]-fSin*fTmp;
        }

        // Complete the update of the tridiagonal matrix.
        nReal fTmp2 = afDiag[1] - fTmp0;
        afDiag[2] += fTmp0;
        fTmp0 = (afDiag[0] - fTmp2)*fSin + ((nReal)2.0)*fTmp1*fCos;
        afSubd[0] = fCos*fTmp0 - fTmp1;
        fTmp0 *= fSin;
        afDiag[1] = fTmp2 + fTmp0;
        afDiag[0] -= fTmp0;
    }
    return false;
}
//----------------------------------------------------------------------------
template <class nReal>
void CMatrix3<nReal>::Bidiagonalize (CMatrix3& rkA, CMatrix3& rkL, CMatrix3& rkR)
{
    nReal afV[3], afW[3];
    nReal fLength, fSign, fT1, fInvT1, fT2;
    bool bIdentity;

    // map first column to (*,0,0)
    fLength = CMath<nReal>::Sqrt(rkA[0][0]*rkA[0][0] + rkA[1][0]*rkA[1][0] +
        rkA[2][0]*rkA[2][0]);
    if (fLength > (nReal)0.0)
    {
        fSign = (rkA[0][0] > (nReal)0.0 ? (nReal)1.0 : -(nReal)1.0);
        fT1 = rkA[0][0] + fSign*fLength;
        fInvT1 = ((nReal)1.0)/fT1;
        afV[1] = rkA[1][0]*fInvT1;
        afV[2] = rkA[2][0]*fInvT1;

        fT2 = -((nReal)2.0)/(((nReal)1.0)+afV[1]*afV[1]+afV[2]*afV[2]);
        afW[0] = fT2*(rkA[0][0]+rkA[1][0]*afV[1]+rkA[2][0]*afV[2]);
        afW[1] = fT2*(rkA[0][1]+rkA[1][1]*afV[1]+rkA[2][1]*afV[2]);
        afW[2] = fT2*(rkA[0][2]+rkA[1][2]*afV[1]+rkA[2][2]*afV[2]);
        rkA[0][0] += afW[0];
        rkA[0][1] += afW[1];
        rkA[0][2] += afW[2];
        rkA[1][1] += afV[1]*afW[1];
        rkA[1][2] += afV[1]*afW[2];
        rkA[2][1] += afV[2]*afW[1];
        rkA[2][2] += afV[2]*afW[2];

        rkL[0][0] = ((nReal)1.0)+fT2;
        rkL[0][1] = fT2*afV[1];
        rkL[1][0] = rkL[0][1];
        rkL[0][2] = fT2*afV[2];
        rkL[2][0] = rkL[0][2];
        rkL[1][1] = ((nReal)1.0)+fT2*afV[1]*afV[1];
        rkL[1][2] = fT2*afV[1]*afV[2];
        rkL[2][1] = rkL[1][2];
        rkL[2][2] = ((nReal)1.0)+fT2*afV[2]*afV[2];
        bIdentity = false;
    }
    else
    {
        rkL = CMatrix3::IDENTITY;
        bIdentity = true;
    }

    // map first row to (*,*,0)
    fLength = CMath<nReal>::Sqrt(rkA[0][1]*rkA[0][1]+rkA[0][2]*rkA[0][2]);
    if (fLength > (nReal)0.0)
    {
        fSign = (rkA[0][1] > (nReal)0.0 ? (nReal)1.0 : -(nReal)1.0);
        fT1 = rkA[0][1] + fSign*fLength;
        afV[2] = rkA[0][2]/fT1;

        fT2 = -((nReal)2.0)/(((nReal)1.0)+afV[2]*afV[2]);
        afW[0] = fT2*(rkA[0][1]+rkA[0][2]*afV[2]);
        afW[1] = fT2*(rkA[1][1]+rkA[1][2]*afV[2]);
        afW[2] = fT2*(rkA[2][1]+rkA[2][2]*afV[2]);
        rkA[0][1] += afW[0];
        rkA[1][1] += afW[1];
        rkA[1][2] += afW[1]*afV[2];
        rkA[2][1] += afW[2];
        rkA[2][2] += afW[2]*afV[2];
        
        rkR[0][0] = (nReal)1.0;
        rkR[0][1] = (nReal)0.0;
        rkR[1][0] = (nReal)0.0;
        rkR[0][2] = (nReal)0.0;
        rkR[2][0] = (nReal)0.0;
        rkR[1][1] = ((nReal)1.0)+fT2;
        rkR[1][2] = fT2*afV[2];
        rkR[2][1] = rkR[1][2];
        rkR[2][2] = ((nReal)1.0)+fT2*afV[2]*afV[2];
    }
    else
    {
        rkR = CMatrix3::IDENTITY;
    }

    // map second column to (*,*,0)
    fLength = CMath<nReal>::Sqrt(rkA[1][1]*rkA[1][1]+rkA[2][1]*rkA[2][1]);
    if (fLength > (nReal)0.0)
    {
        fSign = (rkA[1][1] > (nReal)0.0 ? (nReal)1.0 : -(nReal)1.0);
        fT1 = rkA[1][1] + fSign*fLength;
        afV[2] = rkA[2][1]/fT1;

        fT2 = -((nReal)2.0)/(((nReal)1.0)+afV[2]*afV[2]);
        afW[1] = fT2*(rkA[1][1]+rkA[2][1]*afV[2]);
        afW[2] = fT2*(rkA[1][2]+rkA[2][2]*afV[2]);
        rkA[1][1] += afW[1];
        rkA[1][2] += afW[2];
        rkA[2][2] += afV[2]*afW[2];

        nReal fA = ((nReal)1.0)+fT2;
        nReal fB = fT2*afV[2];
        nReal fC = ((nReal)1.0)+fB*afV[2];

        if (bIdentity)
        {
            rkL[0][0] = (nReal)1.0;
            rkL[0][1] = (nReal)0.0;
            rkL[1][0] = (nReal)0.0;
            rkL[0][2] = (nReal)0.0;
            rkL[2][0] = (nReal)0.0;
            rkL[1][1] = fA;
            rkL[1][2] = fB;
            rkL[2][1] = fB;
            rkL[2][2] = fC;
        }
        else
        {
            for (nInt32 iRow = 0; iRow < 3; iRow++)
            {
                nReal fTmp0 = rkL[iRow][1];
                nReal fTmp1 = rkL[iRow][2];
                rkL[iRow][1] = fA*fTmp0+fB*fTmp1;
                rkL[iRow][2] = fB*fTmp0+fC*fTmp1;
            }
        }
    }
}
//----------------------------------------------------------------------------
template <class nReal>
void CMatrix3<nReal>::GolubKahanStep (CMatrix3& rkA, CMatrix3& rkL, CMatrix3& rkR)
{
    nReal fT11 = rkA[0][1]*rkA[0][1]+rkA[1][1]*rkA[1][1];
    nReal fT22 = rkA[1][2]*rkA[1][2]+rkA[2][2]*rkA[2][2];
    nReal fT12 = rkA[1][1]*rkA[1][2];
    nReal fTrace = fT11+fT22;
    nReal fDiff = fT11-fT22;
    nReal fDiscr = CMath<nReal>::Sqrt(fDiff*fDiff+((nReal)4.0)*fT12*fT12);
    nReal fRoot1 = ((nReal)0.5)*(fTrace+fDiscr);
    nReal fRoot2 = ((nReal)0.5)*(fTrace-fDiscr);

    // adjust right
    nReal fY = rkA[0][0] - (CMath<nReal>::FAbs(fRoot1-fT22) <=
        CMath<nReal>::FAbs(fRoot2-fT22) ? fRoot1 : fRoot2);
    nReal fZ = rkA[0][1];
    nReal fInvLength = CMath<nReal>::InvSqrt(fY*fY+fZ*fZ);
    nReal fSin = fZ*fInvLength;
    nReal fCos = -fY*fInvLength;

    nReal fTmp0 = rkA[0][0];
    nReal fTmp1 = rkA[0][1];
    rkA[0][0] = fCos*fTmp0-fSin*fTmp1;
    rkA[0][1] = fSin*fTmp0+fCos*fTmp1;
    rkA[1][0] = -fSin*rkA[1][1];
    rkA[1][1] *= fCos;

    nInt32 iRow;
    for (iRow = 0; iRow < 3; iRow++)
    {
        fTmp0 = rkR[0][iRow];
        fTmp1 = rkR[1][iRow];
        rkR[0][iRow] = fCos*fTmp0-fSin*fTmp1;
        rkR[1][iRow] = fSin*fTmp0+fCos*fTmp1;
    }

    // adjust left
    fY = rkA[0][0];
    fZ = rkA[1][0];
    fInvLength = CMath<nReal>::InvSqrt(fY*fY+fZ*fZ);
    fSin = fZ*fInvLength;
    fCos = -fY*fInvLength;

    rkA[0][0] = fCos*rkA[0][0]-fSin*rkA[1][0];
    fTmp0 = rkA[0][1];
    fTmp1 = rkA[1][1];
    rkA[0][1] = fCos*fTmp0-fSin*fTmp1;
    rkA[1][1] = fSin*fTmp0+fCos*fTmp1;
    rkA[0][2] = -fSin*rkA[1][2];
    rkA[1][2] *= fCos;

    nInt32 iCol;
    for (iCol = 0; iCol < 3; iCol++)
    {
        fTmp0 = rkL[iCol][0];
        fTmp1 = rkL[iCol][1];
        rkL[iCol][0] = fCos*fTmp0-fSin*fTmp1;
        rkL[iCol][1] = fSin*fTmp0+fCos*fTmp1;
    }

    // adjust right
    fY = rkA[0][1];
    fZ = rkA[0][2];
    fInvLength = CMath<nReal>::InvSqrt(fY*fY+fZ*fZ);
    fSin = fZ*fInvLength;
    fCos = -fY*fInvLength;

    rkA[0][1] = fCos*rkA[0][1]-fSin*rkA[0][2];
    fTmp0 = rkA[1][1];
    fTmp1 = rkA[1][2];
    rkA[1][1] = fCos*fTmp0-fSin*fTmp1;
    rkA[1][2] = fSin*fTmp0+fCos*fTmp1;
    rkA[2][1] = -fSin*rkA[2][2];
    rkA[2][2] *= fCos;

    for (iRow = 0; iRow < 3; iRow++)
    {
        fTmp0 = rkR[1][iRow];
        fTmp1 = rkR[2][iRow];
        rkR[1][iRow] = fCos*fTmp0-fSin*fTmp1;
        rkR[2][iRow] = fSin*fTmp0+fCos*fTmp1;
    }

    // adjust left
    fY = rkA[1][1];
    fZ = rkA[2][1];
    fInvLength = CMath<nReal>::InvSqrt(fY*fY+fZ*fZ);
    fSin = fZ*fInvLength;
    fCos = -fY*fInvLength;

    rkA[1][1] = fCos*rkA[1][1]-fSin*rkA[2][1];
    fTmp0 = rkA[1][2];
    fTmp1 = rkA[2][2];
    rkA[1][2] = fCos*fTmp0-fSin*fTmp1;
    rkA[2][2] = fSin*fTmp0+fCos*fTmp1;

    for (iCol = 0; iCol < 3; iCol++)
    {
        fTmp0 = rkL[iCol][1];
        fTmp1 = rkL[iCol][2];
        rkL[iCol][1] = fCos*fTmp0-fSin*fTmp1;
        rkL[iCol][2] = fSin*fTmp0+fCos*fTmp1;
    }
}
//----------------------------------------------------------------------------
template <class nReal>
void CMatrix3<nReal>::SingularValueDecomposition (CMatrix3& rkL, CMatrix3& rkD,
    CMatrix3& rkRTranspose) const
{
    nInt32 iRow, iCol;

    CMatrix3 kA = *this;
    Bidiagonalize(kA,rkL,rkRTranspose);
    rkD.MakeZero();

    const nInt32 iMax = 32;
    const nReal fEpsilon = (nReal)1e-04;
    for (nInt32 i = 0; i < iMax; i++)
    {
        nReal fTmp, fTmp0, fTmp1;
        nReal fSin0, fCos0, fTan0;
        nReal fSin1, fCos1, fTan1;

        bool bTest1 = (CMath<nReal>::FAbs(kA[0][1]) <=
            fEpsilon*(CMath<nReal>::FAbs(kA[0][0]) +
            CMath<nReal>::FAbs(kA[1][1])));
        bool bTest2 = (CMath<nReal>::FAbs(kA[1][2]) <=
            fEpsilon*(CMath<nReal>::FAbs(kA[1][1]) +
            CMath<nReal>::FAbs(kA[2][2])));
        if (bTest1)
        {
            if (bTest2)
            {
                rkD[0][0] = kA[0][0];
                rkD[1][1] = kA[1][1];
                rkD[2][2] = kA[2][2];
                break;
            }
            else
            {
                // 2x2 closed form factorization
                fTmp = (kA[1][1]*kA[1][1] - kA[2][2]*kA[2][2] +
                    kA[1][2]*kA[1][2])/(kA[1][2]*kA[2][2]);
                fTan0 = ((nReal)0.5)*(fTmp + CMath<nReal>::Sqrt(fTmp*fTmp +
                    ((nReal)4.0)));
                fCos0 = CMath<nReal>::InvSqrt(((nReal)1.0)+fTan0*fTan0);
                fSin0 = fTan0*fCos0;

                for (iCol = 0; iCol < 3; iCol++)
                {
                    fTmp0 = rkL[iCol][1];
                    fTmp1 = rkL[iCol][2];
                    rkL[iCol][1] = fCos0*fTmp0-fSin0*fTmp1;
                    rkL[iCol][2] = fSin0*fTmp0+fCos0*fTmp1;
                }
                
                fTan1 = (kA[1][2]-kA[2][2]*fTan0)/kA[1][1];
                fCos1 = CMath<nReal>::InvSqrt(((nReal)1.0)+fTan1*fTan1);
                fSin1 = -fTan1*fCos1;

                for (iRow = 0; iRow < 3; iRow++)
                {
                    fTmp0 = rkRTranspose[1][iRow];
                    fTmp1 = rkRTranspose[2][iRow];
                    rkRTranspose[1][iRow] = fCos1*fTmp0-fSin1*fTmp1;
                    rkRTranspose[2][iRow] = fSin1*fTmp0+fCos1*fTmp1;
                }

                rkD[0][0] = kA[0][0];
                rkD[1][1] = fCos0*fCos1*kA[1][1] -
                    fSin1*(fCos0*kA[1][2]-fSin0*kA[2][2]);
                rkD[2][2] = fSin0*fSin1*kA[1][1] +
                    fCos1*(fSin0*kA[1][2]+fCos0*kA[2][2]);
                break;
            }
        }
        else 
        {
            if (bTest2)
            {
                // 2x2 closed form factorization 
                fTmp = (kA[0][0]*kA[0][0] + kA[1][1]*kA[1][1] -
                    kA[0][1]*kA[0][1])/(kA[0][1]*kA[1][1]);
                fTan0 = ((nReal)0.5)*(-fTmp + CMath<nReal>::Sqrt(fTmp*fTmp +
                    ((nReal)4.0)));
                fCos0 = CMath<nReal>::InvSqrt(((nReal)1.0)+fTan0*fTan0);
                fSin0 = fTan0*fCos0;

                for (iCol = 0; iCol < 3; iCol++)
                {
                    fTmp0 = rkL[iCol][0];
                    fTmp1 = rkL[iCol][1];
                    rkL[iCol][0] = fCos0*fTmp0-fSin0*fTmp1;
                    rkL[iCol][1] = fSin0*fTmp0+fCos0*fTmp1;
                }
                
                fTan1 = (kA[0][1]-kA[1][1]*fTan0)/kA[0][0];
                fCos1 = CMath<nReal>::InvSqrt(((nReal)1.0)+fTan1*fTan1);
                fSin1 = -fTan1*fCos1;

                for (iRow = 0; iRow < 3; iRow++)
                {
                    fTmp0 = rkRTranspose[0][iRow];
                    fTmp1 = rkRTranspose[1][iRow];
                    rkRTranspose[0][iRow] = fCos1*fTmp0-fSin1*fTmp1;
                    rkRTranspose[1][iRow] = fSin1*fTmp0+fCos1*fTmp1;
                }

                rkD[0][0] = fCos0*fCos1*kA[0][0] -
                    fSin1*(fCos0*kA[0][1]-fSin0*kA[1][1]);
                rkD[1][1] = fSin0*fSin1*kA[0][0] +
                    fCos1*(fSin0*kA[0][1]+fCos0*kA[1][1]);
                rkD[2][2] = kA[2][2];
                break;
            }
            else
            {
                GolubKahanStep(kA,rkL,rkRTranspose);
            }
        }
    }

    // Make the diagonal entries positive.
    for (iRow = 0; iRow < 3; iRow++)
    {
        if (rkD[iRow][iRow] < (nReal)0.0)
        {
            rkD[iRow][iRow] = -rkD[iRow][iRow];
            for (iCol = 0; iCol < 3; iCol++)
            {
                rkRTranspose[iRow][iCol] = -rkRTranspose[iRow][iCol];
            }
        }
    }
}
//----------------------------------------------------------------------------
template <class nReal>
void CMatrix3<nReal>::SingularValueComposition (const CMatrix3& rkL,
    const CMatrix3& rkD, const CMatrix3& rkRTranspose)
{
    *this = rkL*(rkD*rkRTranspose);
}
//----------------------------------------------------------------------------
template <class nReal>
void CMatrix3<nReal>::PolarDecomposition (CMatrix3& rkQ, CMatrix3& rkS)
{
    // Decompose M = L*D*R^T.
    CMatrix3 kL, kD, kRTranspose;
    SingularValueDecomposition(kL,kD,kRTranspose);

    // Compute Q = L*R^T.
    rkQ = kL*kRTranspose;

    // Compute S = R*D*R^T.
    rkS = kRTranspose.TransposeTimes(kD*kRTranspose);

    // Numerical round-off errors can cause S not to be symmetric in the
    // sense that S[i][j] and S[j][i] are slightly different for i != j.
    // Correct this by averaging S and Transpose(S).
    rkS[0][1] = ((nReal)0.5)*(rkS[0][1] + rkS[1][0]);
    rkS[1][0] = rkS[0][1];
    rkS[0][2] = ((nReal)0.5)*(rkS[0][2] + rkS[2][0]);
    rkS[2][0] = rkS[0][2];
    rkS[1][2] = ((nReal)0.5)*(rkS[1][2] + rkS[2][1]);
    rkS[2][1] = rkS[1][2];
}
//----------------------------------------------------------------------------
template <class nReal>
void CMatrix3<nReal>::QDUDecomposition (CMatrix3& rkQ, CMatrix3& rkD,
    CMatrix3& rkU) const
{
    // Factor M = QR = QDU where Q is orthogonal (rotation), D is diagonal
    // (scaling),  and U is upper triangular with ones on its diagonal
    // (shear).  Algorithm uses Gram-Schmidt orthogonalization (the QR
    // algorithm).
    //
    // If M = [ m0 | m1 | m2 ] and Q = [ q0 | q1 | q2 ], then
    //
    //   q0 = m0/|m0|
    //   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
    //   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
    //
    // where |V| indicates length of vector V and A*B indicates dot
    // product of vectors A and B.  The matrix R has entries
    //
    //   r00 = q0*m0  r01 = q0*m1  r02 = q0*m2
    //   r10 = 0      r11 = q1*m1  r12 = q1*m2
    //   r20 = 0      r21 = 0      r22 = q2*m2
    //
    // so D = diag(r00,r11,r22) and U has entries u01 = r01/r00,
    // u02 = r02/r00, and u12 = r12/r11.

    // build orthogonal matrix Q
    nReal fInvLength = CMath<nReal>::InvSqrt(m_afEntry[0]*m_afEntry[0] +
        m_afEntry[3]*m_afEntry[3] + m_afEntry[6]*m_afEntry[6]);
    rkQ[0][0] = m_afEntry[0]*fInvLength;
    rkQ[1][0] = m_afEntry[3]*fInvLength;
    rkQ[2][0] = m_afEntry[6]*fInvLength;

    nReal fDot = rkQ[0][0]*m_afEntry[1] + rkQ[1][0]*m_afEntry[4] +
        rkQ[2][0]*m_afEntry[7];
    rkQ[0][1] = m_afEntry[1]-fDot*rkQ[0][0];
    rkQ[1][1] = m_afEntry[4]-fDot*rkQ[1][0];
    rkQ[2][1] = m_afEntry[7]-fDot*rkQ[2][0];
    fInvLength = CMath<nReal>::InvSqrt(rkQ[0][1]*rkQ[0][1] +
        rkQ[1][1]*rkQ[1][1] + rkQ[2][1]*rkQ[2][1]);
    rkQ[0][1] *= fInvLength;
    rkQ[1][1] *= fInvLength;
    rkQ[2][1] *= fInvLength;

    fDot = rkQ[0][0]*m_afEntry[2] + rkQ[1][0]*m_afEntry[5] +
        rkQ[2][0]*m_afEntry[8];
    rkQ[0][2] = m_afEntry[2]-fDot*rkQ[0][0];
    rkQ[1][2] = m_afEntry[5]-fDot*rkQ[1][0];
    rkQ[2][2] = m_afEntry[8]-fDot*rkQ[2][0];
    fDot = rkQ[0][1]*m_afEntry[2] + rkQ[1][1]*m_afEntry[5] +
        rkQ[2][1]*m_afEntry[8];
    rkQ[0][2] -= fDot*rkQ[0][1];
    rkQ[1][2] -= fDot*rkQ[1][1];
    rkQ[2][2] -= fDot*rkQ[2][1];
    fInvLength = CMath<nReal>::InvSqrt(rkQ[0][2]*rkQ[0][2] +
        rkQ[1][2]*rkQ[1][2] + rkQ[2][2]*rkQ[2][2]);
    rkQ[0][2] *= fInvLength;
    rkQ[1][2] *= fInvLength;
    rkQ[2][2] *= fInvLength;

    // guarantee that orthogonal matrix has determinant 1 (no reflections)
    nReal fDet = rkQ[0][0]*rkQ[1][1]*rkQ[2][2] + rkQ[0][1]*rkQ[1][2]*rkQ[2][0]
        +  rkQ[0][2]*rkQ[1][0]*rkQ[2][1] - rkQ[0][2]*rkQ[1][1]*rkQ[2][0]
        -  rkQ[0][1]*rkQ[1][0]*rkQ[2][2] - rkQ[0][0]*rkQ[1][2]*rkQ[2][1];

    if (fDet < (nReal)0.0)
    {
        for (nInt32 iRow = 0; iRow < 3; iRow++)
        {
            for (nInt32 iCol = 0; iCol < 3; iCol++)
            {
                rkQ[iRow][iCol] = -rkQ[iRow][iCol];
            }
        }
    }

    // build "right" matrix R
    CMatrix3 kR;
    kR[0][0] = rkQ[0][0]*m_afEntry[0] + rkQ[1][0]*m_afEntry[3] +
        rkQ[2][0]*m_afEntry[6];
    kR[0][1] = rkQ[0][0]*m_afEntry[1] + rkQ[1][0]*m_afEntry[4] +
        rkQ[2][0]*m_afEntry[7];
    kR[1][1] = rkQ[0][1]*m_afEntry[1] + rkQ[1][1]*m_afEntry[4] +
        rkQ[2][1]*m_afEntry[7];
    kR[0][2] = rkQ[0][0]*m_afEntry[2] + rkQ[1][0]*m_afEntry[5] +
        rkQ[2][0]*m_afEntry[8];
    kR[1][2] = rkQ[0][1]*m_afEntry[2] + rkQ[1][1]*m_afEntry[5] +
        rkQ[2][1]*m_afEntry[8];
    kR[2][2] = rkQ[0][2]*m_afEntry[2] + rkQ[1][2]*m_afEntry[5] +
        rkQ[2][2]*m_afEntry[8];

    // the scaling component
    rkD.MakeDiagonal(kR[0][0],kR[1][1],kR[2][2]);

    // the shear component
    nReal fInvD0 = ((nReal)1.0)/rkD[0][0];
    rkU[0][0] = (nReal)1.0;
    rkU[0][1] = kR[0][1]*fInvD0;
    rkU[0][2] = kR[0][2]*fInvD0;
    rkU[1][0] = (nReal)0.0;
    rkU[1][1] = (nReal)1.0;
    rkU[1][2] = kR[1][2]/rkD[1][1];
    rkU[2][0] = (nReal)0.0;
    rkU[2][1] = (nReal)0.0;
    rkU[2][2] = (nReal)1.0;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CMatrix3<nReal> operator* (nReal fScalar, const CMatrix3<nReal>& rkM)
{
    return rkM*fScalar;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CVector3<nReal> operator* (const CVector3<nReal>& rkV,
    const CMatrix3<nReal>& rkM)
{
    return CVector3<nReal>(
        rkV[0]*rkM[0][0] + rkV[1]*rkM[1][0] + rkV[2]*rkM[2][0],
        rkV[0]*rkM[0][1] + rkV[1]*rkM[1][1] + rkV[2]*rkM[2][1],
        rkV[0]*rkM[0][2] + rkV[1]*rkM[1][2] + rkV[2]*rkM[2][2]);
}



typedef CMatrix3<nReal> Matrix3f;


}