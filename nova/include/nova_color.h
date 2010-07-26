п»ї/***************************************************************************
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

#include "nova_error.h"


namespace nova
{

class NOVA_EXPORT CColorRGB : public CBase
{
public:
    // Construction.  The components are intended to be in the interval [0,1].
    // For accessing of colors by array index, the map is 0 = red, 1 = green,
    // and 2 = blue.
    CColorRGB ();  // initial values (0,0,0)
    CColorRGB (real fR, real fG, real fB);
	CColorRGB (nova::byte bR, nova::byte bG, nova::byte bB);
    CColorRGB (const real* afTuple);
    CColorRGB (const CColorRGB& rkC);

    // member access
    operator const real* () const;
    operator real* ();
    real operator[] (int i) const;
    real& operator[] (int i);
    real R () const;
    real& R ();
    real G () const;
    real& G ();
    real B () const;
    real& B ();

    // assignment
    CColorRGB& operator= (const CColorRGB& rkC);

    // comparison
    bool operator== (const CColorRGB& rkC) const;
    bool operator!= (const CColorRGB& rkC) const;
    bool operator<  (const CColorRGB& rkC) const;
    bool operator<= (const CColorRGB& rkC) const;
    bool operator>  (const CColorRGB& rkC) const;
    bool operator>= (const CColorRGB& rkC) const;

    // arithmetic operations
    CColorRGB operator+ (const CColorRGB& rkC) const;
    CColorRGB operator- (const CColorRGB& rkC) const;
    CColorRGB operator* (const CColorRGB& rkC) const;
    CColorRGB operator* (real fScalar) const;

    // arithmetic updates
    CColorRGB& operator+= (const CColorRGB& rkC);
    CColorRGB& operator-= (const CColorRGB& rkC);
    CColorRGB& operator*= (const CColorRGB& rkC);
    CColorRGB& operator*= (real fScalar);

    // Transform the color channels to [0,1].  Clamp sets negative values to
    // zero and values larger than one to one.  ScaleByMax assumes the color
    // channels are nonnegative, finds the maximum color channel, and divides
    // all channels by that value.
    void Clamp ();
    void ScaleByMax ();

    static const CColorRGB BLACK;    // = (0,0,0)
    static const CColorRGB WHITE;    // = (1,1,1)
    static const CColorRGB INVALID;  // = (-1,-1,-1)

private:
    // support for comparisons
    int CompareArrays (const CColorRGB& rkC) const;

    real m_afTuple[3];
};

NOVA_EXPORT CColorRGB operator* (real fScalar, const CColorRGB& rkC);

class NOVA_EXPORT CColorRGBA : public CBase
{
public:
    // Construction.  The components are intended to be in the interval [0,1].
    // For accessing of colors by array index, the map is 0 = red, 1 = green,
    // 2 = blue, and 3 = alpha.
    CColorRGBA ();  // initial values (0,0,0,0)
    CColorRGBA (real fR, real fG, real fB, real fA);
	CColorRGBA (nova::byte bR, nova::byte bG, nova::byte bB, nova::byte bA);
    CColorRGBA (const real* afTuple);
    CColorRGBA (const CColorRGBA& rkC);

    // member access
    operator const real* () const;
    operator real* ();
    real operator[] (int i) const;
    real& operator[] (int i);
    real R () const;
    real& R ();
    real G () const;
    real& G ();
    real B () const;
    real& B ();
    real A () const;
    real& A ();

    // assignment
    CColorRGBA& operator= (const CColorRGBA& rkC);

    // comparison
    bool operator== (const CColorRGBA& rkC) const;
    bool operator!= (const CColorRGBA& rkC) const;
    bool operator<  (const CColorRGBA& rkC) const;
    bool operator<= (const CColorRGBA& rkC) const;
    bool operator>  (const CColorRGBA& rkC) const;
    bool operator>= (const CColorRGBA& rkC) const;

    // arithmetic operations
    CColorRGBA operator+ (const CColorRGBA& rkC) const;
    CColorRGBA operator- (const CColorRGBA& rkC) const;
    CColorRGBA operator* (const CColorRGBA& rkC) const;
    CColorRGBA operator* (real fScalar) const;

    // arithmetic updates
    CColorRGBA& operator+= (const CColorRGBA& rkC);
    CColorRGBA& operator-= (const CColorRGBA& rkC);
    CColorRGBA& operator*= (const CColorRGBA& rkC);
    CColorRGBA& operator*= (real fScalar);

    // Transform the color channels to [0,1].  Clamp sets negative values to
    // zero and values larger than one to one.  ScaleByMax assumes the color
    // channels are nonnegative, finds the maximum RGB channel, and divides
    // all RGB channels by that value.  The alpha channel is clamped to [0,1].
    void Clamp ();
    void ScaleByMax ();

    static const CColorRGBA BLACK;   // = (0,0,0,1)
    static const CColorRGBA WHITE;   // = (1,1,1,1)
    static const CColorRGBA INVALID; // = (-1,-1,-1,-1)

private:
    // support for comparisons
    int CompareArrays (const CColorRGBA& rkC) const;

    real m_afTuple[4];
};

NOVA_EXPORT CColorRGBA operator* (real fScalar, const CColorRGBA& rkC);

inline CColorRGB::operator const real* () const
{
    return m_afTuple;
}
//----------------------------------------------------------------------------
inline CColorRGB::operator real* ()
{
    return m_afTuple;
}
//----------------------------------------------------------------------------
inline real CColorRGB::operator[] (int i) const
{
    assert(0 <= i && i <= 2);
    if (i < 0)
    {
        i = 0;
    }
    else if (i > 2)
    {
        i = 2;
    }

    return m_afTuple[i];
}
//----------------------------------------------------------------------------
inline real& CColorRGB::operator[] (int i)
{
    assert(0 <= i && i <= 2);
    if (i < 0)
    {
        i = 0;
    }
    else if (i > 2)
    {
        i = 2;
    }

    return m_afTuple[i];
}
//----------------------------------------------------------------------------
inline real CColorRGB::R () const
{
    return m_afTuple[0];
}
//----------------------------------------------------------------------------
inline real& CColorRGB::R ()
{
    return m_afTuple[0];
}
//----------------------------------------------------------------------------
inline real CColorRGB::G () const
{
    return m_afTuple[1];
}
//----------------------------------------------------------------------------
inline real& CColorRGB::G ()
{
    return m_afTuple[1];
}
//----------------------------------------------------------------------------
inline real CColorRGB::B () const
{
    return m_afTuple[2];
}
//----------------------------------------------------------------------------
inline real& CColorRGB::B ()
{
    return m_afTuple[2];
}

inline CColorRGBA::operator const real* () const
{
    return m_afTuple;
}
//----------------------------------------------------------------------------
inline CColorRGBA::operator real* ()
{
    return m_afTuple;
}
//----------------------------------------------------------------------------
inline real CColorRGBA::operator[] (int i) const
{
    assert(0 <= i && i <= 3);
    if (i < 0)
    {
        i = 0;
    }
    else if ( i > 3 )
    {
        i = 3;
    }

    return m_afTuple[i];
}
//----------------------------------------------------------------------------
inline real& CColorRGBA::operator[] (int i)
{
    assert(0 <= i && i <= 3);
    if (i < 0)
    {
        i = 0;
    }
    else if ( i > 3 )
    {
        i = 3;
    }

    return m_afTuple[i];
}
//----------------------------------------------------------------------------
inline real CColorRGBA::R () const
{
    return m_afTuple[0];
}
//----------------------------------------------------------------------------
inline real& CColorRGBA::R ()
{
    return m_afTuple[0];
}
//----------------------------------------------------------------------------
inline real CColorRGBA::G () const
{
    return m_afTuple[1];
}
//----------------------------------------------------------------------------
inline real& CColorRGBA::G ()
{
    return m_afTuple[1];
}
//----------------------------------------------------------------------------
inline real CColorRGBA::B () const
{
    return m_afTuple[2];
}
//----------------------------------------------------------------------------
inline real& CColorRGBA::B ()
{
    return m_afTuple[2];
}
//----------------------------------------------------------------------------
inline real CColorRGBA::A () const
{
    return m_afTuple[3];
}
//----------------------------------------------------------------------------
inline real& CColorRGBA::A ()
{
    return m_afTuple[3];
}
//----------------------------------------------------------------------------


}

