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

template <class nReal>
class CVector4 : public CMemoryManaged
{
public:
    // construction
    CVector4 ();  // uninitialized
    CVector4 (nReal fX, nReal fY, nReal fZ, nReal fW);
    CVector4 (const nReal* afTuple);
    CVector4 (const CVector4& rkV);

    // coordinate access
    inline operator const nReal* () const;
    inline operator nReal* ();
    inline nReal operator[] (nInt32 i) const;
    inline nReal& operator[] (nInt32 i);
    inline nReal X () const;
    inline nReal& X ();
    inline nReal Y () const;
    inline nReal& Y ();
    inline nReal Z () const;
    inline nReal& Z ();
    inline nReal W () const;
    inline nReal& W ();

    // assignment
    inline CVector4& operator= (const CVector4& rkV);

    // comparison
    bool operator== (const CVector4& rkV) const;
    bool operator!= (const CVector4& rkV) const;
    bool operator<  (const CVector4& rkV) const;
    bool operator<= (const CVector4& rkV) const;
    bool operator>  (const CVector4& rkV) const;
    bool operator>= (const CVector4& rkV) const;

    // arithmetic operations
    inline CVector4 operator+ (const CVector4& rkV) const;
    inline CVector4 operator- (const CVector4& rkV) const;
    inline CVector4 operator* (nReal fScalar) const;
    inline CVector4 operator/ (nReal fScalar) const;
    inline CVector4 operator- () const;

    // arithmetic updates
    inline CVector4& operator+= (const CVector4& rkV);
    inline CVector4& operator-= (const CVector4& rkV);
    inline CVector4& operator*= (nReal fScalar);
    inline CVector4& operator/= (nReal fScalar);

    // vector operations
    inline nReal Length () const;
    inline nReal SquaredLength () const;
    inline nReal Dot (const CVector4& rkV) const;
    inline nReal Normalize ();

    // special vectors
    NOVA_EXPORT static const CVector4 ZERO;
    NOVA_EXPORT static const CVector4 UNIT_X;  // (1,0,0,0)
    NOVA_EXPORT static const CVector4 UNIT_Y;  // (0,1,0,0)
    NOVA_EXPORT static const CVector4 UNIT_Z;  // (0,0,1,0)
    NOVA_EXPORT static const CVector4 UNIT_W;  // (0,0,0,1)
    NOVA_EXPORT static const CVector4 ONE;     // (1,1,1,1)

private:
    // support for comparisons
    nInt32 CompareArrays (const CVector4& rkV) const;

    nReal m_afTuple[4];
};

// arithmetic operations
template <class nReal>
CVector4<nReal> operator* (nReal fScalar, const CVector4<nReal>& rkV);

// debugging output
template <class nReal>
std::ostream& operator<< (std::ostream& rkOStr, const CVector4<nReal>& rkV);

template <class nReal>
CVector4<nReal>::CVector4 ()
{
    // uninitialized for performance in array construction
}
//----------------------------------------------------------------------------
template <class nReal>
CVector4<nReal>::CVector4 (nReal fX, nReal fY, nReal fZ, nReal fW)
{
    m_afTuple[0] = fX;
    m_afTuple[1] = fY;
    m_afTuple[2] = fZ;
    m_afTuple[3] = fW;
}
//----------------------------------------------------------------------------
template <class nReal>
CVector4<nReal>::CVector4 (const nReal* afTuple)
{
    m_afTuple[0] = afTuple[0];
    m_afTuple[1] = afTuple[1];
    m_afTuple[2] = afTuple[2];
    m_afTuple[3] = afTuple[3];
}
//----------------------------------------------------------------------------
template <class nReal>
CVector4<nReal>::CVector4 (const CVector4& rkV)
{
    m_afTuple[0] = rkV.m_afTuple[0];
    m_afTuple[1] = rkV.m_afTuple[1];
    m_afTuple[2] = rkV.m_afTuple[2];
    m_afTuple[3] = rkV.m_afTuple[3];
}
//----------------------------------------------------------------------------
template <class nReal>
inline CVector4<nReal>::operator const nReal* () const
{
    return m_afTuple;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CVector4<nReal>::operator nReal* ()
{
    return m_afTuple;
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal CVector4<nReal>::operator[] (nInt32 i) const
{
    return m_afTuple[i];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal& CVector4<nReal>::operator[] (nInt32 i)
{
    return m_afTuple[i];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal CVector4<nReal>::X () const
{
    return m_afTuple[0];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal& CVector4<nReal>::X ()
{
    return m_afTuple[0];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal CVector4<nReal>::Y () const
{
    return m_afTuple[1];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal& CVector4<nReal>::Y ()
{
    return m_afTuple[1];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal CVector4<nReal>::Z () const
{
    return m_afTuple[2];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal& CVector4<nReal>::Z ()
{
    return m_afTuple[2];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal CVector4<nReal>::W () const
{
    return m_afTuple[3];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal& CVector4<nReal>::W ()
{
    return m_afTuple[3];
}
//----------------------------------------------------------------------------
template <class nReal>
inline CVector4<nReal>& CVector4<nReal>::operator= (const CVector4& rkV)
{
    m_afTuple[0] = rkV.m_afTuple[0];
    m_afTuple[1] = rkV.m_afTuple[1];
    m_afTuple[2] = rkV.m_afTuple[2];
    m_afTuple[3] = rkV.m_afTuple[3];
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
nInt32 CVector4<nReal>::CompareArrays (const CVector4& rkV) const
{
    return memcmp(m_afTuple,rkV.m_afTuple,4*sizeof(nReal));
}
//----------------------------------------------------------------------------
template <class nReal>
bool CVector4<nReal>::operator== (const CVector4& rkV) const
{
    return CompareArrays(rkV) == 0;
}
//----------------------------------------------------------------------------
template <class nReal>
bool CVector4<nReal>::operator!= (const CVector4& rkV) const
{
    return CompareArrays(rkV) != 0;
}
//----------------------------------------------------------------------------
template <class nReal>
bool CVector4<nReal>::operator< (const CVector4& rkV) const
{
    return CompareArrays(rkV) < 0;
}
//----------------------------------------------------------------------------
template <class nReal>
bool CVector4<nReal>::operator<= (const CVector4& rkV) const
{
    return CompareArrays(rkV) <= 0;
}
//----------------------------------------------------------------------------
template <class nReal>
bool CVector4<nReal>::operator> (const CVector4& rkV) const
{
    return CompareArrays(rkV) > 0;
}
//----------------------------------------------------------------------------
template <class nReal>
bool CVector4<nReal>::operator>= (const CVector4& rkV) const
{
    return CompareArrays(rkV) >= 0;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CVector4<nReal> CVector4<nReal>::operator+ (const CVector4& rkV) const
{
    return CVector4(
        m_afTuple[0]+rkV.m_afTuple[0],
        m_afTuple[1]+rkV.m_afTuple[1],
        m_afTuple[2]+rkV.m_afTuple[2],
        m_afTuple[3]+rkV.m_afTuple[3]);
}
//----------------------------------------------------------------------------
template <class nReal>
inline CVector4<nReal> CVector4<nReal>::operator- (const CVector4& rkV) const
{
    return CVector4(
        m_afTuple[0]-rkV.m_afTuple[0],
        m_afTuple[1]-rkV.m_afTuple[1],
        m_afTuple[2]-rkV.m_afTuple[2],
        m_afTuple[3]-rkV.m_afTuple[3]);
}
//----------------------------------------------------------------------------
template <class nReal>
inline CVector4<nReal> CVector4<nReal>::operator* (nReal fScalar) const
{
    return CVector4(
        fScalar*m_afTuple[0],
        fScalar*m_afTuple[1],
        fScalar*m_afTuple[2],
        fScalar*m_afTuple[3]);
}
//----------------------------------------------------------------------------
template <class nReal>
inline CVector4<nReal> CVector4<nReal>::operator/ (nReal fScalar) const
{
    CVector4 kQuot;

    if (fScalar != (nReal)0.0)
    {
        nReal fInvScalar = ((nReal)1.0)/fScalar;
        kQuot.m_afTuple[0] = fInvScalar*m_afTuple[0];
        kQuot.m_afTuple[1] = fInvScalar*m_afTuple[1];
        kQuot.m_afTuple[2] = fInvScalar*m_afTuple[2];
        kQuot.m_afTuple[3] = fInvScalar*m_afTuple[3];
    }
    else
    {
        kQuot.m_afTuple[0] = CMath<nReal>::MAX_REAL;
        kQuot.m_afTuple[1] = CMath<nReal>::MAX_REAL;
        kQuot.m_afTuple[2] = CMath<nReal>::MAX_REAL;
        kQuot.m_afTuple[3] = CMath<nReal>::MAX_REAL;
    }

    return kQuot;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CVector4<nReal> CVector4<nReal>::operator- () const
{
    return CVector4(
        -m_afTuple[0],
        -m_afTuple[1],
        -m_afTuple[2],
        -m_afTuple[3]);
}
//----------------------------------------------------------------------------
template <class nReal>
inline CVector4<nReal> operator* (nReal fScalar, const CVector4<nReal>& rkV)
{
    return CVector4<nReal>(
        fScalar*rkV[0],
        fScalar*rkV[1],
        fScalar*rkV[2],
        fScalar*rkV[3]);
}
//----------------------------------------------------------------------------
template <class nReal>
inline CVector4<nReal>& CVector4<nReal>::operator+= (const CVector4& rkV)
{
    m_afTuple[0] += rkV.m_afTuple[0];
    m_afTuple[1] += rkV.m_afTuple[1];
    m_afTuple[2] += rkV.m_afTuple[2];
    m_afTuple[3] += rkV.m_afTuple[3];
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CVector4<nReal>& CVector4<nReal>::operator-= (const CVector4& rkV)
{
    m_afTuple[0] -= rkV.m_afTuple[0];
    m_afTuple[1] -= rkV.m_afTuple[1];
    m_afTuple[2] -= rkV.m_afTuple[2];
    m_afTuple[3] -= rkV.m_afTuple[3];
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CVector4<nReal>& CVector4<nReal>::operator*= (nReal fScalar)
{
    m_afTuple[0] *= fScalar;
    m_afTuple[1] *= fScalar;
    m_afTuple[2] *= fScalar;
    m_afTuple[3] *= fScalar;
    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
inline CVector4<nReal>& CVector4<nReal>::operator/= (nReal fScalar)
{
    if (fScalar != (nReal)0.0)
    {
        nReal fInvScalar = ((nReal)1.0)/fScalar;
        m_afTuple[0] *= fInvScalar;
        m_afTuple[1] *= fInvScalar;
        m_afTuple[2] *= fInvScalar;
        m_afTuple[3] *= fInvScalar;
    }
    else
    {
        m_afTuple[0] = CMath<nReal>::MAX_REAL;
        m_afTuple[1] = CMath<nReal>::MAX_REAL;
        m_afTuple[2] = CMath<nReal>::MAX_REAL;
        m_afTuple[3] = CMath<nReal>::MAX_REAL;
    }

    return *this;
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal CVector4<nReal>::Length () const
{
    return CMath<nReal>::Sqrt(
        m_afTuple[0]*m_afTuple[0] +
        m_afTuple[1]*m_afTuple[1] +
        m_afTuple[2]*m_afTuple[2] +
        m_afTuple[3]*m_afTuple[3]);
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal CVector4<nReal>::SquaredLength () const
{
    return
        m_afTuple[0]*m_afTuple[0] +
        m_afTuple[1]*m_afTuple[1] +
        m_afTuple[2]*m_afTuple[2] +
        m_afTuple[3]*m_afTuple[3];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal CVector4<nReal>::Dot (const CVector4& rkV) const
{
    return
        m_afTuple[0]*rkV.m_afTuple[0] +
        m_afTuple[1]*rkV.m_afTuple[1] +
        m_afTuple[2]*rkV.m_afTuple[2] +
        m_afTuple[3]*rkV.m_afTuple[3];
}
//----------------------------------------------------------------------------
template <class nReal>
inline nReal CVector4<nReal>::Normalize ()
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
std::ostream& operator<< (std::ostream& rkOStr, const CVector4<nReal>& rkV)
{
     return rkOStr << rkV.X() << ' ' << rkV.Y() << ' ' << rkV.Z()
         << ' ' << rkV.W();
}


typedef CVector4<nova::nReal> Vector4f;



}