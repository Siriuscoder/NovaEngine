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
#include "nova_stable_precompiled_headers.h"

#include "nova_buffers.h"

namespace nova
{

size_t CBuffer::GetBufferSize() const
{
	return mbSize;
}

CMemoryBuffer::CMemoryBuffer(void *p_mem, size_t size) :
	mBegin(p_mem), CBuffer(size)
{
	mAlloc = false;
}

CMemoryBuffer::CMemoryBuffer() :
	mBegin(NULL), CBuffer(0)
{
	mAlloc = false;
}

CMemoryBuffer::~CMemoryBuffer()
{
	//FreeBuffer();
}

CMemoryBuffer::CMemoryBuffer(const CMemoryBuffer & other)
{
	mBegin = other.mBegin;
	mAlloc = other.mAlloc;
	mbSize = other.mbSize;
}

void CMemoryBuffer::FreeBuffer(void)
{
	if(mAlloc && mBegin)
	{
		gDefaultAllocator.FreeMemory(mBegin);
		mAlloc = false;
		mBegin = NULL;
	}
}

void CMemoryBuffer::ReallocBuffer(size_t newsize)
{
	if(mBegin && mAlloc)
	{
		mBegin = gDefaultAllocator.ReallocMemory(mBegin, newsize);
	}
}

void CMemoryBuffer::AllocBuffer(size_t size)
{
	FreeBuffer();
	
	mBegin = gDefaultAllocator.AllocMemory(size);
	mAlloc = true;
	mbSize = size;
}
	
void *CMemoryBuffer::GetBegin(void) const
{
	return mBegin;
}

void CMemoryBuffer::CopyTo(const CMemoryBuffer & buffer, size_t size, size_t off) const
{
	if(buffer.GetBufferSize() < size)
		NOVA_EXP("CMemoryBuffer::CopyTo: unput buffer too small...", MEM_ERROR);

	if((off + size) > mbSize)
		NOVA_EXP("CMemoryBuffer::CopyTo: offset param too large", MEM_ERROR);

	nova::byte *p = (nova::byte *)mBegin + off;
	memcpy(buffer.GetBegin(), p, size);
}

}