п»ї/***************************************************************************
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
#include "nova_stable_precompiled_headers.h"

#include "nova_common_allocator.h"

namespace nova
{

#ifdef USE_WINNI_FAST_ALLOCATOR

template<> CAllocObject<winnie_allocator::CWinnieAlloc>::TUserAllocatorContaner CAllocObject<winnie_allocator::CWinnieAlloc>::gUserAllocator;
CAllocObject<winnie_allocator::CWinnieAlloc> gDefaultAllocator;

#elif defined(USE_NED_OGRE_ALLOCATOR)

template<> CAllocObject<nedalloc::CNedAlloc>::TUserAllocatorContaner CAllocObject<nedalloc::CNedAlloc>::gUserAllocator;
CAllocObject<nedalloc::CNedAlloc> gDefaultAllocator;

#elif defined(USE_MALLOC_FREE_SYSTEM_ALLOCATOR)

template<> CAllocObject<CSysAlloc>::TUserAllocatorContaner CAllocObject<CSysAlloc>::gUserAllocator;
CAllocObject<CSysAlloc> gDefaultAllocator;

#endif



}