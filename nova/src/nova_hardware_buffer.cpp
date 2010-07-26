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
#include "nova_stable_precompiled_headers.h"

#include "nova_hardware_buffer.h"

namespace nova
{

CHardwareBuffer::CHardwareBuffer() : mIsLocked(false),
	mReady(false), mLockStart(0), mLockSize(0),
	mLockState(HWB_STATIC), mSize(0), CBase("CHardwareBuffer")
{

}

bool CHardwareBuffer::IsReady(void)
{
	return mReady;
}

bool CHardwareBuffer::IsLocked(void)
{
	return mIsLocked;
}

THardwareBufferLock CHardwareBuffer::GetLockState(void)
{
	return mLockState;
}

size_t CHardwareBuffer::GetLockStart(void)
{
	return mLockStart;
}

size_t CHardwareBuffer::GetLockSize(void)
{
	return mLockSize;
}

CMemoryBuffer CHardwareBuffer::Lock(size_t offset, size_t length, THardwareBufferLock opt)
{
	if(mIsLocked)
		throw NOVA_EXP("CHardwareBuffer::Lock() - Buffer is locked!", BAD_OPERATION);

	if(!mReady)
		throw NOVA_EXP("CHardwareBuffer::Lock() - Buffer is not ready!", BAD_OPERATION);

	CMemoryBuffer ptr = LockSource(offset, length, opt);
	mLockState = opt;
	mIsLocked = true;
	mLockStart = offset;
	mLockSize = length;

	return ptr;
}

void CHardwareBuffer::Unlock(CMemoryBuffer & buf)
{
	if(!mIsLocked)
		throw NOVA_EXP("CHardwareBuffer::Unlock() - Buffer is unlocked!", BAD_OPERATION);
	//if(mLockState == HWB_READ_ONLY || mLockState == HWB_NO_OVERWRITE)
	//	return ;
	if(!mReady)
		throw NOVA_EXP("CHardwareBuffer::Lock() - Buffer is not ready!", BAD_OPERATION);

	UnlockSource(buf);
	mIsLocked = false;
	mLockStart = 0;
	mLockSize = 0;
}

size_t CHardwareBuffer::GetHDSize()
{
	return mSize;
}

CMemoryBuffer CHardwareBuffer::Lock(THardwareBufferLock opt)
{
	return Lock(0, mSize, opt);
}

void CHardwareBuffer::CopyToBuffer(CHardwareBuffer & dest, size_t doffset,
	size_t offset, size_t length)
{
	//void * ptr = src.LockSource(soffset, slength, HWB_READ_ONLY);
	//if(mIsLocked)
	//	throw NOVA_EXP("CHardwareBuffer::CopyBuffer() - Buffer is locked!", BAD_OPERATION);

	CMemoryBuffer dlock = dest.Lock(doffset, length, dest.GetLockState());
	CMemoryBuffer slock = Lock(offset, length, GetLockState());

	slock.CopyTo(dlock, length, 0);

	dest.Unlock(dlock);
	Unlock(slock);
}

CMemoryBuffer CHardwareBuffer::ReadData(size_t offset, size_t length)
{
	CMemoryBuffer res;
	CMemoryBuffer sour;

	res.AllocBuffer(length);
	sour = Lock(offset, length, mLockState);
	sour.CopyTo(res, length, 0);

	Unlock(sour);

	return res;
}

void CHardwareBuffer::WriteData(CMemoryBuffer & buf, size_t offset, size_t length, THardwareBufferLock opt)
{	
	CMemoryBuffer res;
	CMemoryBuffer dest;

	dest = Lock(offset, length, opt);
	buf.CopyTo(dest, length, 0);
	Unlock(dest);
}

}
