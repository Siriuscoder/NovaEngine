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

#include "nova_error.h"

namespace nova
{

template <class nReal> class CMath
{
public:
    // Wrappers to hide implementations of functions.  The ACos and ASin
    // functions clamp the input argument to [-1,1] to avoid NaN issues
    // when the input is slightly larger than 1 or slightly smaller than -1.
    // Other functions have the potential for using a fast and approximate
    // algorithm rather than calling the standard CMath library functions.
    static nReal ACos (nReal fValue);
    static nReal ASin (nReal fValue);
    static nReal ATan (nReal fValue);
    static nReal ATan2 (nReal fY, nReal fX);
    static nReal Ceil (nReal fValue);
    static nReal Cos (nReal fValue);
    static nReal Exp (nReal fValue);
    static nReal FAbs (nReal fValue);
    static nReal Floor (nReal fValue);
    static nReal FMod (nReal fX, nReal fY);
    static nReal InvSqrt (nReal fValue);
    static nReal Log (nReal fValue);
    static nReal Log2 (nReal fValue);
    static nReal Log10 (nReal fValue);
    static nReal Pow (nReal fBase, nReal fExponent);
    static nReal Sin (nReal fValue);
    static nReal Sqr (nReal fValue);
    static nReal Sqrt (nReal fValue);
    static nReal Tan (nReal fValue);

    // Return -1 if the input is negative, 0 if the input is zero, and +1
    // if the input is positive.
    static nInt32 Sign (nInt32 iValue);
    static nReal Sign (nReal fValue);

    // Generate a random number in [0,1).  The random number generator may
    // be seeded by a first call to UnitRandom with a positive seed.
    static nReal UnitRandom (unsigned int uiSeed = 0);

    // Generate a random number in [-1,1).  The random number generator may
    // be seeded by a first call to SymmetricRandom with a positive seed.
    static nReal SymmetricRandom (unsigned int uiSeed = 0);

    // Generate a random number in [min,max).  The random number generator may
    // be seeded by a first call to IntervalRandom with a positive seed.
    static nReal IntervalRandom (nReal fMin, nReal fMax,
        unsigned int uiSeed = 0);

	static nInt32 Rand();
	static void RandSeed(unsigned int uiSeed = 0);

    // Fast evaluation of trigonometric and inverse trigonometric functions
    // using polynomial approximations.  The speed ups were measured on an
    // AMD 2800 (2.08 GHz) processor using Visual Studion .NET 2003 with a
    // release build.

    // The input must be in [0,pi/2].
    // max error sin0 = 1.7e-04, speed up = 4.0
    // max error sin1 = 1.9e-08, speed up = 2.8
    static nReal FastSin0 (nReal fAngle);
    static nReal FastSin1 (nReal fAngle);

    // The input must be in [0,pi/2]
    // max error cos0 = 1.2e-03, speed up = 4.5
    // max error cos1 = 6.5e-09, speed up = 2.8
    static nReal FastCos0 (nReal fAngle);
    static nReal FastCos1 (nReal fAngle);

    // The input must be in [0,pi/4].
    // max error tan0 = 8.1e-04, speed up = 5.6
    // max error tan1 = 1.9e-08, speed up = 3.4
    static nReal FastTan0 (nReal fAngle);
    static nReal FastTan1 (nReal fAngle);

    // The input must be in [0,1].
    // max error invsin0 = 6.8e-05, speed up = 7.5
    // max error invsin1 = 1.4e-07, speed up = 5.5
    static nReal FastInvSin0 (nReal fValue);
    static nReal FastInvSin1 (nReal fValue);

    // The input must be in [0,1].
    // max error invcos0 = 6.8e-05, speed up = 7.5
    // max error invcos1 = 1.4e-07, speed up = 5.7
    static nReal FastInvCos0 (nReal fValue);
    static nReal FastInvCos1 (nReal fValue);

    // The input must be in [-1,1].
    // max error invtan0 = 1.2e-05, speed up = 2.8
    // max error invtan1 = 2.3e-08, speed up = 1.8
    static nReal FastInvTan0 (nReal fValue);
    static nReal FastInvTan1 (nReal fValue);

    // A fast approximation to 1/sqrt.
    static nReal FastInvSqrt (nReal fValue);

    // Fast approximations to exp(-x).  The input x must be in [0,infinity).
    // max error negexp0 = 0.00024, speed up = 25.4
    // max error negexp1 = 0.000024, speed up = 25.4
    // max error negexp2 = 0.0000024, speed up = 20.5
    // max error negexp3 = 0.00000025, speed up = 17.3
    static nReal FastNegExp0 (nReal fValue);
    static nReal FastNegExp1 (nReal fValue);
    static nReal FastNegExp2 (nReal fValue);
    static nReal FastNegExp3 (nReal fValue);

    // common constants
    NOVA_EXPORT static const nReal EPSILON;
    NOVA_EXPORT static const nReal ZERO_TOLERANCE;
    NOVA_EXPORT static const nReal MAX_REAL;
    NOVA_EXPORT static const nReal PI;
    NOVA_EXPORT static const nReal TWO_PI;
    NOVA_EXPORT static const nReal HALF_PI;
    NOVA_EXPORT static const nReal INV_PI;
    NOVA_EXPORT static const nReal INV_TWO_PI;
    NOVA_EXPORT static const nReal DEG_TO_RAD;
    NOVA_EXPORT static const nReal RAD_TO_DEG;
    NOVA_EXPORT static const nReal LN_2;
    NOVA_EXPORT static const nReal LN_10;
    NOVA_EXPORT static const nReal INV_LN_2;
    NOVA_EXPORT static const nReal INV_LN_10;
};

template <class nReal>
nReal CMath<nReal>::ACos (nReal fValue)
{
    if (-(nReal)1.0 < fValue)
    {
        if (fValue < (nReal)1.0)
        {
            return (nReal)acos((double)fValue);
        }
        else
        {
            return (nReal)0.0;
        }
    }
    else
    {
        return PI;
    }
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::ASin (nReal fValue)
{
    if (-(nReal)1.0 < fValue)
    {
        if (fValue < (nReal)1.0)
        {
            return (nReal)asin((double)fValue);
        }
        else
        {
            return HALF_PI;
        }
    }
    else
    {
        return -HALF_PI;
    }
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::ATan (nReal fValue)
{
    return (nReal)atan((double)fValue);
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::ATan2 (nReal fY, nReal fX)
{
    return (nReal)atan2((double)fY,(double)fX);
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::Ceil (nReal fValue)
{
    return (nReal)ceil((double)fValue);
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::Cos (nReal fValue)
{
    return (nReal)cos((double)fValue);
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::Exp (nReal fValue)
{
    return (nReal)exp((double)fValue);
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::FAbs (nReal fValue)
{
    return (nReal)fabs((double)fValue);
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::Floor (nReal fValue)
{
    return (nReal)floor((double)fValue);
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::FMod (nReal fX, nReal fY)
{
    return (nReal)fmod((double)fX,(double)fY);
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::InvSqrt (nReal fValue)
{
    return (nReal)(1.0/sqrt((double)fValue));
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::Log (nReal fValue)
{
    return (nReal)log((double)fValue);
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::Log2 (nReal fValue)
{
    return CMath<nReal>::INV_LN_2 * (nReal)log((double)fValue);
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::Log10 (nReal fValue)
{
    return CMath<nReal>::INV_LN_10 * (nReal)log((double)fValue);
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::Pow (nReal fBase, nReal fExponent)
{
    return (nReal)pow((double)fBase,(double)fExponent);
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::Sin (nReal fValue)
{
    return (nReal)sin((double)fValue);
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::Sqr (nReal fValue)
{
    return fValue*fValue;
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::Sqrt (nReal fValue)
{
    return (nReal)sqrt((double)fValue);
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::Tan (nReal fValue)
{
    return (nReal)tan((double)fValue);
}
//----------------------------------------------------------------------------
template <class nReal>
nInt32 CMath<nReal>::Sign (nInt32 iValue)
{
    if (iValue > 0)
    {
        return 1;
    }

    if (iValue < 0)
    {
        return -1;
    }

    return 0;
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::Sign (nReal fValue)
{
    if (fValue > (nReal)0.0)
    {
        return (nReal)1.0;
    }

    if (fValue < (nReal)0.0)
    {
        return -(nReal)1.0;
    }

    return (nReal)0.0;
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::UnitRandom (unsigned int uiSeed )
{
    if (uiSeed > 0)
    {
        srand(uiSeed);
    }

    double dRatio = ((double)rand())/((double)(RAND_MAX));
    return (nReal)dRatio;
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::SymmetricRandom (unsigned int uiSeed)
{
    if (uiSeed > 0.0)
    {
        srand(uiSeed);
    }

    double dRatio = ((double)rand())/((double)(RAND_MAX));
    return (nReal)(2.0*dRatio - 1.0);
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::IntervalRandom (nReal fMin, nReal fMax, unsigned int uiSeed)
{
    if (uiSeed > 0)
    {
        srand(uiSeed);
    }

    double dRatio = ((double)rand())/((double)(RAND_MAX));
    return fMin+(fMax-fMin)*((nReal)dRatio);
}
//----------------------------------------------------------------------------
template <class nReal>
nInt32 CMath<nReal>::Rand()
{
	return rand();
}

template <class nReal>
void CMath<nReal>::RandSeed(unsigned int uiSeed)
{
	srand(uiSeed);
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::FastSin0 (nReal fAngle)
{
    nReal fASqr = fAngle*fAngle;
    nReal fResult = (nReal)7.61e-03;
    fResult *= fASqr;
    fResult -= (nReal)1.6605e-01;
    fResult *= fASqr;
    fResult += (nReal)1.0;
    fResult *= fAngle;
    return fResult;
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::FastSin1 (nReal fAngle)
{
    nReal fASqr = fAngle*fAngle;
    nReal fResult = -(nReal)2.39e-08;
    fResult *= fASqr;
    fResult += (nReal)2.7526e-06;
    fResult *= fASqr;
    fResult -= (nReal)1.98409e-04;
    fResult *= fASqr;
    fResult += (nReal)8.3333315e-03;
    fResult *= fASqr;
    fResult -= (nReal)1.666666664e-01;
    fResult *= fASqr;
    fResult += (nReal)1.0;
    fResult *= fAngle;
    return fResult;
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::FastCos0 (nReal fAngle)
{
    nReal fASqr = fAngle*fAngle;
    nReal fResult = (nReal)3.705e-02;
    fResult *= fASqr;
    fResult -= (nReal)4.967e-01;
    fResult *= fASqr;
    fResult += (nReal)1.0;
    return fResult;
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::FastCos1 (nReal fAngle)
{
    nReal fASqr = fAngle*fAngle;
    nReal fResult = -(nReal)2.605e-07;
    fResult *= fASqr;
    fResult += (nReal)2.47609e-05;
    fResult *= fASqr;
    fResult -= (nReal)1.3888397e-03;
    fResult *= fASqr;
    fResult += (nReal)4.16666418e-02;
    fResult *= fASqr;
    fResult -= (nReal)4.999999963e-01;
    fResult *= fASqr;
    fResult += (nReal)1.0;
    return fResult;
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::FastTan0 (nReal fAngle)
{
    nReal fASqr = fAngle*fAngle;
    nReal fResult = (nReal)2.033e-01;
    fResult *= fASqr;
    fResult += (nReal)3.1755e-01;
    fResult *= fASqr;
    fResult += (nReal)1.0;
    fResult *= fAngle;
    return fResult;
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::FastTan1 (nReal fAngle)
{
    nReal fASqr = fAngle*fAngle;
    nReal fResult = (nReal)9.5168091e-03;
    fResult *= fASqr;
    fResult += (nReal)2.900525e-03;
    fResult *= fASqr;
    fResult += (nReal)2.45650893e-02;
    fResult *= fASqr;
    fResult += (nReal)5.33740603e-02;
    fResult *= fASqr;
    fResult += (nReal)1.333923995e-01;
    fResult *= fASqr;
    fResult += (nReal)3.333314036e-01;
    fResult *= fASqr;
    fResult += (nReal)1.0;
    fResult *= fAngle;
    return fResult;
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::FastInvSin0 (nReal fValue)
{
    nReal fRoot = CMath<nReal>::Sqrt(((nReal)1.0)-fValue);
    nReal fResult = -(nReal)0.0187293;
    fResult *= fValue;
    fResult += (nReal)0.0742610;
    fResult *= fValue;
    fResult -= (nReal)0.2121144;
    fResult *= fValue;
    fResult += (nReal)1.5707288;
    fResult = HALF_PI - fRoot*fResult;
    return fResult;
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::FastInvSin1 (nReal fValue)
{
    nReal fRoot = CMath<nReal>::Sqrt(FAbs(((nReal)1.0)-fValue));
    nReal fResult = -(nReal)0.0012624911;
    fResult *= fValue;
    fResult += (nReal)0.0066700901;
    fResult *= fValue;
    fResult -= (nReal)0.0170881256;
    fResult *= fValue;
    fResult += (nReal)0.0308918810;
    fResult *= fValue;
    fResult -= (nReal)0.0501743046;
    fResult *= fValue;
    fResult += (nReal)0.0889789874;
    fResult *= fValue;
    fResult -= (nReal)0.2145988016;
    fResult *= fValue;
    fResult += (nReal)1.5707963050;
    fResult = HALF_PI - fRoot*fResult;
    return fResult;
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::FastInvCos0 (nReal fValue)
{
    nReal fRoot = CMath<nReal>::Sqrt(((nReal)1.0)-fValue);
    nReal fResult = -(nReal)0.0187293;
    fResult *= fValue;
    fResult += (nReal)0.0742610;
    fResult *= fValue;
    fResult -= (nReal)0.2121144;
    fResult *= fValue;
    fResult += (nReal)1.5707288;
    fResult *= fRoot;
    return fResult;
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::FastInvCos1 (nReal fValue)
{
    nReal fRoot = CMath<nReal>::Sqrt(FAbs(((nReal)1.0)-fValue));
    nReal fResult = -(nReal)0.0012624911;
    fResult *= fValue;
    fResult += (nReal)0.0066700901;
    fResult *= fValue;
    fResult -= (nReal)0.0170881256;
    fResult *= fValue;
    fResult += (nReal)0.0308918810;
    fResult *= fValue;
    fResult -= (nReal)0.0501743046;
    fResult *= fValue;
    fResult += (nReal)0.0889789874;
    fResult *= fValue;
    fResult -= (nReal)0.2145988016;
    fResult *= fValue;
    fResult += (nReal)1.5707963050;
    fResult *= fRoot;
    return fResult;
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::FastInvTan0 (nReal fValue)
{
    nReal fVSqr = fValue*fValue;
    nReal fResult = (nReal)0.0208351;
    fResult *= fVSqr;
    fResult -= (nReal)0.085133;
    fResult *= fVSqr;
    fResult += (nReal)0.180141;
    fResult *= fVSqr;
    fResult -= (nReal)0.3302995;
    fResult *= fVSqr;
    fResult += (nReal)0.999866;
    fResult *= fValue;
    return fResult;
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::FastInvTan1 (nReal fValue)
{
    nReal fVSqr = fValue*fValue;
    nReal fResult = (nReal)0.0028662257;
    fResult *= fVSqr;
    fResult -= (nReal)0.0161657367;
    fResult *= fVSqr;
    fResult += (nReal)0.0429096138;
    fResult *= fVSqr;
    fResult -= (nReal)0.0752896400;
    fResult *= fVSqr;
    fResult += (nReal)0.1065626393;
    fResult *= fVSqr;
    fResult -= (nReal)0.1420889944;
    fResult *= fVSqr;
    fResult += (nReal)0.1999355085;
    fResult *= fVSqr;
    fResult -= (nReal)0.3333314528;
    fResult *= fVSqr;
    fResult += (nReal)1.0;
    fResult *= fValue;
    return fResult;
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::FastNegExp0 (nReal fValue)
{
    nReal fResult = (nReal)0.0038278;
    fResult *= fValue;
    fResult += (nReal)0.0292732;
    fResult *= fValue;
    fResult += (nReal)0.2507213;
    fResult *= fValue;
    fResult += (nReal)1.0;
    fResult *= fResult;
    fResult *= fResult;
    fResult = ((nReal)1.0)/fResult;
    return fResult;
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::FastNegExp1 (nReal fValue)
{
    nReal fResult = (nReal)0.00026695;
    fResult *= fValue;
    fResult += (nReal)0.00227723;
    fResult *= fValue;
    fResult += (nReal)0.03158565;
    fResult *= fValue;
    fResult += (nReal)0.24991035;
    fResult *= fValue;
    fResult += (nReal)1.0;
    fResult *= fResult;
    fResult *= fResult;
    fResult = ((nReal)1.0)/fResult;
    return fResult;
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::FastNegExp2 (nReal fValue)
{
    nReal fResult = (nReal)0.000014876;
    fResult *= fValue;
    fResult += (nReal)0.000127992;
    fResult *= fValue;
    fResult += (nReal)0.002673255;
    fResult *= fValue;
    fResult += (nReal)0.031198056;
    fResult *= fValue;
    fResult += (nReal)0.250010936;
    fResult *= fValue;
    fResult += (nReal)1.0;
    fResult *= fResult;
    fResult *= fResult;
    fResult = ((nReal)1.0)/fResult;
    return fResult;
}
//----------------------------------------------------------------------------
template <class nReal>
nReal CMath<nReal>::FastNegExp3 (nReal fValue)
{
    nReal fResult = (nReal)0.0000006906;
    fResult *= fValue;
    fResult += (nReal)0.0000054302;
    fResult *= fValue;
    fResult += (nReal)0.0001715620;
    fResult *= fValue;
    fResult += (nReal)0.0025913712;
    fResult *= fValue;
    fResult += (nReal)0.0312575832;
    fResult *= fValue;
    fResult += (nReal)0.2499986842;
    fResult *= fValue;
    fResult += (nReal)1.0;
    fResult *= fResult;
    fResult *= fResult;
    fResult = ((nReal)1.0)/fResult;
    return fResult;
}

typedef CMath<nReal> CMathf;


}
