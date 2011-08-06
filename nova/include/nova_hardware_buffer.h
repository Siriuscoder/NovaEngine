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
#include "nova_streams.h"


namespace nova
{

enum THardwareBufferLock
{
	HWB_STREAM,
	HWB_STATIC,
	HWB_DYNAMIC,
};

class NOVA_EXPORT CHardwareBuffer : public CBase
{
protected:

	bool mIsLocked;
	bool mReady;
	size_t mLockStart;
	size_t mLockSize;
	size_t	mSize;
	THardwareBufferLock mLockState;


	virtual CMemoryBuffer LockSource(size_t offset, size_t length, THardwareBufferLock opt) = 0;

	virtual void UnlockSource(CMemoryBuffer & buf) = 0;

public:

	CHardwareBuffer();

	virtual ~CHardwareBuffer() {}

	virtual void CopyToBuffer(CHardwareBuffer & dest, size_t doffset,
		size_t offset, size_t length);

	bool IsReady(void);

	bool IsLocked(void);

	CMemoryBuffer ReadData(size_t offset, size_t length);

	void WriteData(CMemoryBuffer & buf, size_t offset, size_t length, THardwareBufferLock opt);

	THardwareBufferLock GetLockState(void);

	size_t GetLockStart(void);

	size_t GetLockSize(void);

	size_t GetHDSize();

	CMemoryBuffer Lock(size_t offset, size_t length, THardwareBufferLock opt);

	CMemoryBuffer Lock(THardwareBufferLock opt);

	void Unlock(CMemoryBuffer & buf);
// Другие возможные типы блокировок //
};

}
