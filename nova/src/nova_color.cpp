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
#include "nova_stable_precompiled_headers.h"

#include "nova_color.h"

namespace nova
{

const CColorRGB CColorRGB::BLACK(0.0f,0.0f,0.0f);
const CColorRGB CColorRGB::WHITE(1.0f,1.0f,1.0f);
const CColorRGB CColorRGB::INVALID(-1.0f,-1.0f,-1.0f);

//----------------------------------------------------------------------------
CColorRGB::CColorRGB () : CBase("CColorRGB")
{
    m_afTuple[0] = 0.0f;
    m_afTuple[1] = 0.0f;
    m_afTuple[2] = 0.0f;
}
//----------------------------------------------------------------------------
CColorRGB::CColorRGB (nReal fR, nReal fG, nReal fB) : CBase("CColorRGB")
{
    m_afTuple[0] = fR;
    m_afTuple[1] = fG;
    m_afTuple[2] = fB;
}

CColorRGB::CColorRGB (nova::byte bR, nova::byte bG, nova::byte bB) : CBase("CColorRGB")
{
    m_afTuple[0] = (nReal)bR / 255;
    m_afTuple[1] = (nReal)bG / 255;
    m_afTuple[2] = (nReal)bB / 255;
}
//----------------------------------------------------------------------------
CColorRGB::CColorRGB (const nReal* afTuple) : CBase("CColorRGB")
{
    m_afTuple[0] = afTuple[0];
    m_afTuple[1] = afTuple[1];
    m_afTuple[2] = afTuple[2];
}
//----------------------------------------------------------------------------
CColorRGB::CColorRGB (const CColorRGB& rkC) : CBase("CColorRGB")
{
    m_afTuple[0] = rkC.m_afTuple[0];
    m_afTuple[1] = rkC.m_afTuple[1];
    m_afTuple[2] = rkC.m_afTuple[2];
}
//----------------------------------------------------------------------------
CColorRGB& CColorRGB::operator= (const CColorRGB& rkC)
{
    m_afTuple[0] = rkC.m_afTuple[0];
    m_afTuple[1] = rkC.m_afTuple[1];
    m_afTuple[2] = rkC.m_afTuple[2];
    return *this;
}
//----------------------------------------------------------------------------
bool CColorRGB::operator== (const CColorRGB& rkC) const
{
    return
        m_afTuple[0] == rkC.m_afTuple[0] &&
        m_afTuple[1] == rkC.m_afTuple[1] &&
        m_afTuple[2] == rkC.m_afTuple[2];
}
//----------------------------------------------------------------------------
bool CColorRGB::operator!= (const CColorRGB& rkC) const
{
    return
        m_afTuple[0] != rkC.m_afTuple[0] ||
        m_afTuple[1] != rkC.m_afTuple[1] ||
        m_afTuple[2] != rkC.m_afTuple[2];
}
//----------------------------------------------------------------------------
int CColorRGB::CompareArrays (const CColorRGB& rkC) const
{
    return memcmp(m_afTuple,rkC.m_afTuple,3*sizeof(nReal));
}
//----------------------------------------------------------------------------
bool CColorRGB::operator< (const CColorRGB& rkC) const
{
    return CompareArrays(rkC) < 0;
}
//----------------------------------------------------------------------------
bool CColorRGB::operator<= (const CColorRGB& rkC) const
{
    return CompareArrays(rkC) <= 0;
}
//----------------------------------------------------------------------------
bool CColorRGB::operator> (const CColorRGB& rkC) const
{
    return CompareArrays(rkC) > 0;
}
//----------------------------------------------------------------------------
bool CColorRGB::operator>= (const CColorRGB& rkC) const
{
    return CompareArrays(rkC) >= 0;
}
//----------------------------------------------------------------------------
CColorRGB CColorRGB::operator+ (const CColorRGB& rkC) const
{
    return CColorRGB(
        m_afTuple[0] + rkC.m_afTuple[0],
        m_afTuple[1] + rkC.m_afTuple[1],
        m_afTuple[2] + rkC.m_afTuple[2]);
}
//----------------------------------------------------------------------------
CColorRGB CColorRGB::operator- (const CColorRGB& rkC) const
{
    return CColorRGB(
        m_afTuple[0] - rkC.m_afTuple[0],
        m_afTuple[1] - rkC.m_afTuple[1],
        m_afTuple[2] - rkC.m_afTuple[2]);
}
//----------------------------------------------------------------------------
CColorRGB CColorRGB::operator* (const CColorRGB& rkC) const
{
    return CColorRGB(
        m_afTuple[0]*rkC.m_afTuple[0],
        m_afTuple[1]*rkC.m_afTuple[1],
        m_afTuple[2]*rkC.m_afTuple[2]);
}
//----------------------------------------------------------------------------
CColorRGB CColorRGB::operator* (nReal fScalar) const
{
    return CColorRGB(
        fScalar*m_afTuple[0],
        fScalar*m_afTuple[1],
        fScalar*m_afTuple[2]);
}
//----------------------------------------------------------------------------
CColorRGB operator* (nReal fScalar, const CColorRGB& rkC)
{
    return CColorRGB(fScalar*rkC[0],fScalar*rkC[1],fScalar*rkC[2]);
}
//----------------------------------------------------------------------------
CColorRGB& CColorRGB::operator+= (const CColorRGB& rkC)
{
    m_afTuple[0] += rkC.m_afTuple[0];
    m_afTuple[1] += rkC.m_afTuple[1];
    m_afTuple[2] += rkC.m_afTuple[2];
    return *this;
}
//----------------------------------------------------------------------------
CColorRGB& CColorRGB::operator-= (const CColorRGB& rkC)
{
    m_afTuple[0] -= rkC.m_afTuple[0];
    m_afTuple[1] -= rkC.m_afTuple[1];
    m_afTuple[2] -= rkC.m_afTuple[2];
    return *this;
}
//----------------------------------------------------------------------------
CColorRGB& CColorRGB::operator*= (const CColorRGB& rkC)
{
    m_afTuple[0] *= rkC.m_afTuple[0];
    m_afTuple[1] *= rkC.m_afTuple[1];
    m_afTuple[2] *= rkC.m_afTuple[2];
    return *this;
}
//----------------------------------------------------------------------------
CColorRGB& CColorRGB::operator*= (nReal fScalar)
{
    m_afTuple[0] *= fScalar;
    m_afTuple[1] *= fScalar;
    m_afTuple[2] *= fScalar;
    return *this;
}
//----------------------------------------------------------------------------
void CColorRGB::Clamp ()
{
    for (int i = 0; i < 3; i++)
    {
        if (m_afTuple[i] > 1.0f)
        {
            m_afTuple[i] = 1.0f;
        }
        else if (m_afTuple[i] < 0.0f)
        {
            m_afTuple[i] = 0.0f;
        }
    }
}
//----------------------------------------------------------------------------
void CColorRGB::ScaleByMax ()
{
    nReal fMax = m_afTuple[0];
    if (m_afTuple[1] > fMax)
    {
        fMax = m_afTuple[1];
    }
    if (m_afTuple[2] > fMax)
    {
        fMax = m_afTuple[2];
    }

    if (fMax > 1.0f)
    {
        nReal fInvMax = 1.0f/fMax;
        for (int i = 0; i < 3; i++)
        {
            m_afTuple[i] *= fInvMax;
        }
    }
}

const CColorRGBA CColorRGBA::BLACK(0.0f,0.0f,0.0f,1.0f);
const CColorRGBA CColorRGBA::WHITE(1.0f,1.0f,1.0f,1.0f);
const CColorRGBA CColorRGBA::INVALID(-1.0f,-1.0f,-1.0f,-1.0f);

//----------------------------------------------------------------------------
CColorRGBA::CColorRGBA () : CBase("CColorRGBA")
{
    m_afTuple[0] = 0.0f;
    m_afTuple[1] = 0.0f;
    m_afTuple[2] = 0.0f;
    m_afTuple[3] = 0.0f;
}
//----------------------------------------------------------------------------
CColorRGBA::CColorRGBA (nReal fR, nReal fG, nReal fB, nReal fA) : CBase("CColorRGBA")
{
    m_afTuple[0] = fR;
    m_afTuple[1] = fG;
    m_afTuple[2] = fB;
    m_afTuple[3] = fA;
}

CColorRGBA::CColorRGBA (nova::byte bR, nova::byte bG, nova::byte bB, nova::byte bA) : CBase("CColorRGB")
{
    m_afTuple[0] = (nReal)bR / 255;
    m_afTuple[1] = (nReal)bG / 255;
    m_afTuple[2] = (nReal)bB / 255;
	m_afTuple[3] = (nReal)bA / 255;
}
//----------------------------------------------------------------------------
CColorRGBA::CColorRGBA (const nReal* afTuple) : CBase("CColorRGBA")
{
    m_afTuple[0] = afTuple[0];
    m_afTuple[1] = afTuple[1];
    m_afTuple[2] = afTuple[2];
    m_afTuple[3] = afTuple[3];
}
//----------------------------------------------------------------------------
CColorRGBA::CColorRGBA (const CColorRGBA& rkC) : CBase("CColorRGBA")
{
    m_afTuple[0] = rkC.m_afTuple[0];
    m_afTuple[1] = rkC.m_afTuple[1];
    m_afTuple[2] = rkC.m_afTuple[2];
    m_afTuple[3] = rkC.m_afTuple[3];
}
//----------------------------------------------------------------------------
CColorRGBA& CColorRGBA::operator= (const CColorRGBA& rkC)
{
    m_afTuple[0] = rkC.m_afTuple[0];
    m_afTuple[1] = rkC.m_afTuple[1];
    m_afTuple[2] = rkC.m_afTuple[2];
    m_afTuple[3] = rkC.m_afTuple[3];
    return *this;
}
//----------------------------------------------------------------------------
bool CColorRGBA::operator== (const CColorRGBA& rkC) const
{
    return
        m_afTuple[0] == rkC.m_afTuple[0] &&
        m_afTuple[1] == rkC.m_afTuple[1] &&
        m_afTuple[2] == rkC.m_afTuple[2] &&
        m_afTuple[3] == rkC.m_afTuple[3];
}
//----------------------------------------------------------------------------
bool CColorRGBA::operator!= (const CColorRGBA& rkC) const
{
    return
        m_afTuple[0] != rkC.m_afTuple[0] ||
        m_afTuple[1] != rkC.m_afTuple[1] ||
        m_afTuple[2] != rkC.m_afTuple[2] ||
        m_afTuple[3] != rkC.m_afTuple[3];
}
//----------------------------------------------------------------------------
int CColorRGBA::CompareArrays (const CColorRGBA& rkC) const
{
    return memcmp(m_afTuple,rkC.m_afTuple,4*sizeof(nReal));
}
//----------------------------------------------------------------------------
bool CColorRGBA::operator< (const CColorRGBA& rkC) const
{
    return CompareArrays(rkC) < 0;
}
//----------------------------------------------------------------------------
bool CColorRGBA::operator<= (const CColorRGBA& rkC) const
{
    return CompareArrays(rkC) <= 0;
}
//----------------------------------------------------------------------------
bool CColorRGBA::operator> (const CColorRGBA& rkC) const
{
    return CompareArrays(rkC) > 0;
}
//----------------------------------------------------------------------------
bool CColorRGBA::operator>= (const CColorRGBA& rkC) const
{
    return CompareArrays(rkC) >= 0;
}
//----------------------------------------------------------------------------
CColorRGBA CColorRGBA::operator+ (const CColorRGBA& rkC) const
{
    return CColorRGBA(
        m_afTuple[0] + rkC.m_afTuple[0],
        m_afTuple[1] + rkC.m_afTuple[1],
        m_afTuple[2] + rkC.m_afTuple[2],
        m_afTuple[3] + rkC.m_afTuple[3]);
}
//----------------------------------------------------------------------------
CColorRGBA CColorRGBA::operator- (const CColorRGBA& rkC) const
{
    return CColorRGBA(
        m_afTuple[0] - rkC.m_afTuple[0],
        m_afTuple[1] - rkC.m_afTuple[1],
        m_afTuple[2] - rkC.m_afTuple[2],
        m_afTuple[3] - rkC.m_afTuple[3]);
}
//----------------------------------------------------------------------------
CColorRGBA CColorRGBA::operator* (const CColorRGBA& rkC) const
{
    return CColorRGBA(
        m_afTuple[0]*rkC.m_afTuple[0],
        m_afTuple[1]*rkC.m_afTuple[1],
        m_afTuple[2]*rkC.m_afTuple[2],
        m_afTuple[3]*rkC.m_afTuple[3]);
}
//----------------------------------------------------------------------------
CColorRGBA CColorRGBA::operator* (nReal fScalar) const
{
    return CColorRGBA(
        fScalar*m_afTuple[0],
        fScalar*m_afTuple[1],
        fScalar*m_afTuple[2],
        fScalar*m_afTuple[3]);
}
//----------------------------------------------------------------------------
CColorRGBA operator* (nReal fScalar, const CColorRGBA& rkC)
{
    return CColorRGBA(
        fScalar*rkC[0],
        fScalar*rkC[1],
        fScalar*rkC[2],
        fScalar*rkC[3]);
}
//----------------------------------------------------------------------------
CColorRGBA& CColorRGBA::operator+= (const CColorRGBA& rkC)
{
    m_afTuple[0] += rkC.m_afTuple[0];
    m_afTuple[1] += rkC.m_afTuple[1];
    m_afTuple[2] += rkC.m_afTuple[2];
    m_afTuple[3] += rkC.m_afTuple[3];
    return *this;
}
//----------------------------------------------------------------------------
CColorRGBA& CColorRGBA::operator-= (const CColorRGBA& rkC)
{
    m_afTuple[0] -= rkC.m_afTuple[0];
    m_afTuple[1] -= rkC.m_afTuple[1];
    m_afTuple[2] -= rkC.m_afTuple[2];
    m_afTuple[3] -= rkC.m_afTuple[3];
    return *this;
}
//----------------------------------------------------------------------------
CColorRGBA& CColorRGBA::operator*= (const CColorRGBA& rkC)
{
    m_afTuple[0] *= rkC.m_afTuple[0];
    m_afTuple[1] *= rkC.m_afTuple[1];
    m_afTuple[2] *= rkC.m_afTuple[2];
    m_afTuple[3] *= rkC.m_afTuple[3];
    return *this;
}
//----------------------------------------------------------------------------
CColorRGBA& CColorRGBA::operator*= (nReal fScalar)
{
    m_afTuple[0] *= fScalar;
    m_afTuple[1] *= fScalar;
    m_afTuple[2] *= fScalar;
    m_afTuple[3] *= fScalar;
    return *this;
}
//----------------------------------------------------------------------------
void CColorRGBA::Clamp ()
{
    for (int i = 0; i < 4; i++)
    {
        if (m_afTuple[i] > 1.0f)
        {
            m_afTuple[i] = 1.0f;
        }
        else if (m_afTuple[i] < 0.0f)
        {
            m_afTuple[i] = 0.0f;
        }
    }
}
//----------------------------------------------------------------------------
void CColorRGBA::ScaleByMax ()
{
    // Use max of color channels, not alpha channel.
    nReal fMax = m_afTuple[0];
    if (m_afTuple[1] > fMax)
    {
        fMax = m_afTuple[1];
    }
    if (m_afTuple[2] > fMax)
    {
        fMax = m_afTuple[2];
    }

    if (fMax > 1.0f)
    {
        nReal fInvMax = 1.0f/fMax;
        for (int i = 0; i < 3; i++)
        {
            m_afTuple[i] *= fInvMax;
        }
    }

    // clamp alpha to [0,1]
    if (m_afTuple[3] > 1.0f)
    {
        m_afTuple[3] = 1.0f;
    }
    else if (m_afTuple[3] < 0.0f)
    {
        m_afTuple[3] = 0.0f;
    }
}


}