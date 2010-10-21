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
#pragma once

#include "nova_error.h"

namespace nova
{


class NOVA_EXPORT CCPUTimer : public CBase
{
private:

	nova::nUInt32 mZeroClock;
	nova::nUInt32 mUpdateClock;

#ifdef WIN_BUILD
    LARGE_INTEGER mFrequency;                    // ticks per second
    LARGE_INTEGER mStartCount;                   //
#else
    timeval mStartCount;                         //
#endif

public:

	CCPUTimer();
	~CCPUTimer();

	void Reset();

	nova::nUInt32 GetMilliseconds();

	nova::nUInt32 GetMicroseconds();

	nova::nUInt32 GetKernelMilliseconds();

	nova::nUInt32 GetKernelMicroseconds();
};

}

