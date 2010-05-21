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

#include "nova_hardware_buffer.h"
#include "nova_smart_ptr.h"

namespace nova
{

class NOVA_EXPORT CHardwareGeometryDataBuffer : public CHardwareBuffer
{
protected:

	GLuint mBufID;

	GLuint StateToUsage(THardwareBufferLock st);

	virtual void CreateBuffer(void);

	virtual void DestroyBuffer(void);

public:

	CHardwareGeometryDataBuffer();

	~CHardwareGeometryDataBuffer();

	GLuint GetBufferID(void);

	virtual void BindBuffer(void) = 0;

	virtual void UnbindBuffer(void) = 0;
};

typedef CHardwareGeometryDataBuffer CHardwareTextureDataBuffer;

class NOVA_EXPORT CHardwareVertexBuffer : public CHardwareGeometryDataBuffer
{
protected:

	virtual CMemoryBuffer LockSource(size_t offset, size_t length, THardwareBufferLock opt);

	virtual void UnlockSource(CMemoryBuffer & buf);

public:

	CHardwareVertexBuffer(CMemoryBuffer &data, THardwareBufferLock opt);

	~CHardwareVertexBuffer();

	void BindBuffer(void);

	void UnbindBuffer(void);

};

typedef CSmartPtr<CHardwareVertexBuffer> CHardwareVertexBufferPtr;

class NOVA_EXPORT CHardwareIndexBuffer : public CHardwareGeometryDataBuffer
{
protected:

	virtual CMemoryBuffer LockSource(size_t offset, size_t length, THardwareBufferLock opt);

	virtual void UnlockSource(CMemoryBuffer & buf);

public:

	CHardwareIndexBuffer(CMemoryBuffer &data, THardwareBufferLock opt);

	~CHardwareIndexBuffer();

	void BindBuffer(void);

	void UnbindBuffer(void);

};

typedef CSmartPtr<CHardwareIndexBuffer> CHardwareIndexBufferPtr;

}