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

#include "nova_error.h"

namespace nova
{

class NOVA_EXPORT CBuffer : public CBase
{
protected:
//	void *mBegin;
	size_t mbSize;

public:

	CBuffer() : mbSize(0), CBase("CBuffer"){}

	CBuffer(size_t size) : mbSize(size), CBase("CBuffer") {}

	~CBuffer() {}

	//void * GetBeginPtr() const;

	size_t GetBufferSize() const;
};


class NOVA_EXPORT CMemoryBuffer : public CBuffer
{
protected:

	void *mBegin;
	bool mAlloc;

public:

	CMemoryBuffer(void *p_mem, size_t size);

	CMemoryBuffer(const CMemoryBuffer & other);

	CMemoryBuffer();

	~CMemoryBuffer();

	void *GetBegin(void) const;

	void AllocBuffer(size_t size);

	void ReallocBuffer(size_t newsize);

	void FreeBuffer(void);

	void CopyTo(const CMemoryBuffer & buffer, size_t size, size_t off) const;
};

}