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

#include "nova_hardware_vertex_buffer.h"
#include "nova_smart_ptr.h"

namespace nova
{

class NOVA_EXPORT CHardwareTextureBuffer : public CHardwareTextureDataBuffer
{
protected:

	GLuint mUsage;

	virtual CMemoryBuffer LockSource(size_t offset, size_t length, THardwareBufferLock opt);

	virtual void UnlockSource(CMemoryBuffer & buf);

public:

	CHardwareTextureBuffer(size_t size, THardwareBufferLock opt);

	~CHardwareTextureBuffer();

	virtual void BindBuffer(void);

	virtual void UnbindBuffer(void);
};

}