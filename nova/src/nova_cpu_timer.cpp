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
#include "nova_stable_precompiled_headers.h"

#include "nova_cpu_timer.h"

namespace nova
{

CCPUTimer::CCPUTimer() : CBase("CCPUTimer")
{
#ifdef WIN_BUILD
	QueryPerformanceFrequency(&mFrequency);
	memset(&mStartCount, 0, sizeof(LARGE_INTEGER));
#else
	memset(&mStartCount, 0, sizeof(timeval));
#endif

	Reset();
}

CCPUTimer::~CCPUTimer()
{
	Reset();
}

void CCPUTimer::Reset()
{
	mZeroClock = clock();

#ifdef WIN_BUILD
    QueryPerformanceCounter(&mStartCount);
#else
    gettimeofday(&mStartCount, NULL);
#endif
}

nova::ulong CCPUTimer::GetMilliseconds()
{
	register nova::ulong cpu;
	register long cputicks;
	mUpdateClock = clock();

	cputicks = mUpdateClock - mZeroClock;
	cpu = static_cast<nova::ulong>(static_cast<float>(cputicks) /
		( static_cast<float>(CLOCKS_PER_SEC) / 1000.0f ));

	return cpu;
}

nova::ulong CCPUTimer::GetMicroseconds()
{
	register nova::ulong cpu;
	register long cputicks;
	mUpdateClock = clock();

	cputicks = mUpdateClock - mZeroClock;
	cpu = static_cast<nova::ulong>(static_cast<float>(cputicks) /
		( static_cast<float>(CLOCKS_PER_SEC) / 1000000.0f ));

	return cpu;
}

#ifdef WIN_BUILD

nova::ulong CCPUTimer::GetKernelMilliseconds()
{
	LARGE_INTEGER endcount;
    QueryPerformanceCounter(&endcount);

    double startTimeInMicroSec = mStartCount.QuadPart * (1000000.0 / mFrequency.QuadPart);
    double endTimeInMicroSec = endcount.QuadPart * (1000000.0 / mFrequency.QuadPart);

	return (nova::ulong)((endTimeInMicroSec - startTimeInMicroSec) * 0.001);
}

nova::ulong CCPUTimer::GetKernelMicroseconds()
{
	LARGE_INTEGER endcount;
    QueryPerformanceCounter(&endcount);

    double startTimeInMicroSec = mStartCount.QuadPart * (1000000.0 / mFrequency.QuadPart);
    double endTimeInMicroSec = endcount.QuadPart * (1000000.0 / mFrequency.QuadPart);

	return (nova::ulong)((endTimeInMicroSec - startTimeInMicroSec));
}

#else

nova::ulong CCPUTimer::GetKernelMilliseconds()
{
	timeval endcount;
    gettimeofday(&endcount, NULL);

    double startTimeInMicroSec = (mStartCount.tv_sec * 1000000.0) + mStartCount.tv_usec;
    double endTimeInMicroSec = (endcount.tv_sec * 1000000.0) + endcount.tv_usec;

	return (nova::ulong)((endTimeInMicroSec - startTimeInMicroSec) * 0.001);
}

nova::ulong CCPUTimer::GetKernelMicroseconds()
{
	timeval endcount;
    gettimeofday(&endcount, NULL);

    double startTimeInMicroSec = (mStartCount.tv_sec * 1000000.0) + mStartCount.tv_usec;
    double endTimeInMicroSec = (endcount.tv_sec * 1000000.0) + endcount.tv_usec;

	return (nova::ulong)((endTimeInMicroSec - startTimeInMicroSec));
}

#endif

}
