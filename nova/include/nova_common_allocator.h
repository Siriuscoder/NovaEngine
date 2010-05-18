/***************************************************************************
 *   Copyright (C) 2010 by Sirius										   *
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

#include "nova_winnie_allocator.h"
#include "nova_nedmalloc.h"
#include "nova_system_allocator.h"

namespace nova
{

template<class talloc>
class NOVA_EXPORT CAllocObject
{
public:

	typedef void* (_cdecl *TAllocFunc)(const size_t);
	typedef void (_cdecl *TFreeFunc)(void *);
	typedef void* (_cdecl *TReallocFunc)(void *, const size_t);

private:

	static struct TUserAllocatorContaner
	{
		TAllocFunc gAlloc;
		TFreeFunc gFree;
		TReallocFunc gRealloc;
	} gUserAllocator;

public:

	CAllocObject()
	{
		memset(&gUserAllocator, 0, sizeof(TUserAllocatorContaner));
	}

	static void SetUserAllocator(TAllocFunc alloc, TFreeFunc free, TReallocFunc realloc)
	{
		if(alloc && free && realloc)
		{
			gUserAllocator.gAlloc = alloc;
			gUserAllocator.gFree = free;
			gUserAllocator.gRealloc = realloc;
		}
	}

	inline void *AllocMemory(const size_t size)
	{
		if(gUserAllocator.gAlloc &&
			gUserAllocator.gFree &&
			gUserAllocator.gRealloc)
		{
			return gUserAllocator.gAlloc(size);
		}
		
		return talloc::AllocateMemory(size);
	}

	inline void FreeMemory(void *pmem)
	{
		if(gUserAllocator.gAlloc &&
			gUserAllocator.gFree &&
			gUserAllocator.gRealloc)
		{
			gUserAllocator.gFree(pmem);
		}
		else
		{	
			talloc::FreeMemory(pmem);
		}
	}

	inline void *ReallocMemory(void *pmem, const size_t size)
	{
		if(gUserAllocator.gAlloc &&
			gUserAllocator.gFree &&
			gUserAllocator.gRealloc)
		{
			return gUserAllocator.gRealloc(pmem, size);
		}

		return talloc::Reallocate(pmem, size);
	}
};

#ifdef USE_WINNI_FAST_ALLOCATOR
extern CAllocObject<winnie_allocator::CWinnieAlloc> gDefaultAllocator;
#elif defined(USE_NED_OGRE_ALLOCATOR)
extern CAllocObject<nedalloc::CNedAlloc> gDefaultAllocator;
#elif
#	ifndef USE_MALLOC_FREE_SYSTEM_ALLOCATOR
#	define USE_MALLOC_FREE_SYSTEM_ALLOCATOR
#	endif
extern CAllocObject<CSysAlloc> gDefaultAllocator;
#endif

}