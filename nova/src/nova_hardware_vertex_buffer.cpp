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

#include "nova_hardware_vertex_buffer.h"
#include "nova_render_system.h"

namespace nova
{

CHardwareGeometryDataBuffer::CHardwareGeometryDataBuffer() : mBufID(0)
{
	CreateBuffer();
}

CHardwareGeometryDataBuffer::~CHardwareGeometryDataBuffer()
{
	DestroyBuffer();
}

GLuint CHardwareGeometryDataBuffer::StateToUsage(THardwareBufferLock st)
{
	GLuint usage = 0;

	switch(st)
	{
	case HWB_STREAM:
		usage = GL_STREAM_DRAW_ARB;
		break;
	case HWB_STATIC:
		usage = GL_STATIC_DRAW_ARB;
		break;
	case HWB_DYNAMIC:
		usage = GL_DYNAMIC_DRAW_ARB;
		break;
	
	}

	return usage;
}

GLuint CHardwareGeometryDataBuffer::GetBufferID()
{
	return mBufID;
}

CHardwareVertexBuffer::CHardwareVertexBuffer(CMemoryBuffer &data, THardwareBufferLock opt)
{
	if(mBufID > 0)
	{
		if(data.GetBufferSize() == 0)
			throw NOVA_EXP("CHardwareVertexBuffer(): You trying to create v-buffer with zero length..", BAD_OPERATION);

		BindBuffer();

		GLuint usage = StateToUsage(opt);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, data.GetBufferSize(), data.GetBegin(), usage);

		mReady = true;
		glGetBufferParameterivARB(GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, (GLint*)&mSize);

		UnbindBuffer();
	}
	else
		throw NOVA_EXP("CHardwareVertexBuffer(): v-buffer id not created..", BAD_OPERATION);
}

CHardwareVertexBuffer::~CHardwareVertexBuffer()
{

}

void CHardwareGeometryDataBuffer::CreateBuffer(void)
{
	if(!CRenderSystem::GetSingelton().CheckCapabilities(BASIC_CATEGORY, CAP_VBO))
		throw NOVA_EXP("CHardwareVertexBuffer(): Current OpenGL version do not support Vertex Buffer Object Extention.. \
			Can not create vertex buffer..", BAD_OPERATION);

	glGenBuffersARB(1, &mBufID);
}

void CHardwareGeometryDataBuffer::DestroyBuffer(void)
{
	if(mBufID > 0)
	{
		glDeleteBuffersARB(1, &mBufID);
	}
}

void CHardwareVertexBuffer::BindBuffer()
{
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, mBufID);
}

void CHardwareVertexBuffer::UnbindBuffer()
{
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

CMemoryBuffer CHardwareVertexBuffer::LockSource(size_t offset, size_t length, THardwareBufferLock opt)
{
	nova::nByte *mapper = NULL;
	CMemoryBuffer result;

	if((offset + length) > mSize)
		throw NOVA_EXP("HardwareVertexBuffer::LockSource(): Calling length and size very large..", BAD_OPERATION);

	BindBuffer();
	
	if(opt == HWB_DYNAMIC)
	{
		if((mapper = static_cast<nova::nByte *>(glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_READ_WRITE_ARB))) != NULL)
		{
			mapper += offset;
			CMemoryBuffer res(mapper, length);

			result = res;
		}
		else
		{
			UnbindBuffer();
			throw NOVA_EXP("HardwareVertexBuffer::LockSource(): Can not map v-buffer memory..", BAD_OPERATION);
		}
	}
	else
	{
		result.AllocBuffer(length);
		glGetBufferSubDataARB(GL_ARRAY_BUFFER_ARB, offset, length, result.GetBegin());
	}

	//UnbindBuffer();

	return result;
}

void CHardwareVertexBuffer::UnlockSource(CMemoryBuffer & buf)
{
	BindBuffer();

	if(mLockState == HWB_DYNAMIC)
	{
		if(!glUnmapBufferARB(GL_ARRAY_BUFFER_ARB))
		{
			UnbindBuffer();
			throw NOVA_EXP("HardwareVertexBuffer::LockSource(): Can not map v-buffer memory..", BAD_OPERATION);
		}
	}
	else
	{
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, mLockStart, mLockSize, buf.GetBegin());
		buf.FreeBuffer();
	}

	UnbindBuffer();
}

////////////// Index buffer implementation ////////////////////////////////////
CHardwareIndexBuffer::CHardwareIndexBuffer(CMemoryBuffer &data, THardwareBufferLock opt)
{
	if(mBufID > 0)
	{
		if(data.GetBufferSize() == 0)
			throw NOVA_EXP("CHardwareIndexBuffer(): You trying to create v-buffer with zero length..", BAD_OPERATION);

		BindBuffer();

		GLuint usage = StateToUsage(opt);
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, data.GetBufferSize(), data.GetBegin(), usage);

		mReady = true;
		glGetBufferParameterivARB(GL_ELEMENT_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, (GLint*)&mSize);

		UnbindBuffer();
	}
	else
		throw NOVA_EXP("CHardwareIndexBuffer(): v-buffer id not created..", BAD_OPERATION);
}

CHardwareIndexBuffer::~CHardwareIndexBuffer()
{

}

void CHardwareIndexBuffer::BindBuffer()
{
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mBufID);
}

void CHardwareIndexBuffer::UnbindBuffer()
{
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
}

CMemoryBuffer CHardwareIndexBuffer::LockSource(size_t offset, size_t length, THardwareBufferLock opt)
{
	nova::nByte *mapper = NULL;
	CMemoryBuffer result;

	if((offset + length) > mSize)
		throw NOVA_EXP("HardwareVertexBuffer::LockSource(): Calling length and size very large..", BAD_OPERATION);

	BindBuffer();
	
	if(opt == HWB_DYNAMIC)
	{
		if((mapper = static_cast<nova::nByte *>(glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, GL_READ_WRITE_ARB))) != NULL)
		{
			mapper += offset;
			CMemoryBuffer res(mapper, length);

			result = res;
		}
		else
		{
			UnbindBuffer();
			throw NOVA_EXP("HardwareVertexBuffer::LockSource(): Can not map v-buffer memory..", BAD_OPERATION);
		}
	}
	else
	{
		result.AllocBuffer(length);
		glGetBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, offset, length, result.GetBegin());
	}

	UnbindBuffer();

	return result;
}

void CHardwareIndexBuffer::UnlockSource(CMemoryBuffer & buf)
{
	//BindBuffer();

	if(mLockState == HWB_DYNAMIC)
	{
		if(!glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB))
		{
			UnbindBuffer();
			throw NOVA_EXP("HardwareVertexBuffer::LockSource(): Can not map v-buffer memory..", BAD_OPERATION);
		}
	}
	else
	{
		glBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mLockStart, mLockSize, buf.GetBegin());
		buf.FreeBuffer();
	}

	UnbindBuffer();
}


}