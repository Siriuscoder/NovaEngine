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

#include "nova_hardware_texture_buffer.h"
#include "nova_render_system.h"

namespace nova
{

////////////// Index buffer implementation ////////////////////////////////////
CHardwareTextureBuffer::CHardwareTextureBuffer(size_t size, THardwareBufferLock opt)
{
	if(mBufID > 0)
	{
		if(!CRenderSystem::GetSingelton().CheckCapabilities(BASIC_CATEGORY, CAP_PBO))
			throw NOVA_EXP("CHardwareTextureBuffer: You card do not support pbo extention..", BAD_OPERATION);

		if(size == 0)
			throw NOVA_EXP("CHardwareTextureBuffer(): You trying to create v-buffer with zero length..", BAD_OPERATION);

		BindBuffer();

		GLuint usage;
		switch(opt)
		{
			case HWB_DYNAMIC:
				usage = GL_STREAM_READ_ARB;
				mUsage = GL_PIXEL_PACK_BUFFER_ARB;
				break;
			default:
				usage = GL_STREAM_DRAW_ARB;
				mUsage = GL_PIXEL_UNPACK_BUFFER_ARB;
				break;
		}

		// Preparing pixel buffer
		glBufferDataARB(mUsage, size, 0, usage);

		mReady = true;
		mSize = size;

		UnbindBuffer();
	}
	else
		throw NOVA_EXP("CHardwareTextureBuffer(): v-buffer id not created..", BAD_OPERATION);
}

CHardwareTextureBuffer::~CHardwareTextureBuffer()
{

}

void CHardwareTextureBuffer::BindBuffer()
{
	glBindBufferARB(mUsage, mBufID);
}

void CHardwareTextureBuffer::UnbindBuffer()
{
	glBindBufferARB(mUsage, 0);
}

CMemoryBuffer CHardwareTextureBuffer::LockSource(size_t offset, size_t length, THardwareBufferLock opt)
{
	nova::byte *mapper = NULL;
	CMemoryBuffer result;

	if((offset + length) > mSize)
		throw NOVA_EXP("HardwareVertexBuffer::LockSource(): Calling length and size very large..", BAD_OPERATION);

	BindBuffer();


	GLenum method;
	if(mUsage == GL_PIXEL_UNPACK_BUFFER_ARB)
	{
	// Note that glMapBufferARB() causes sync issue.
	// If GPU is working with this buffer, glMapBufferARB() will wait(stall)
	// until GPU to finish its job. To avoid waiting (idle), you can call
	// first glBufferDataARB() with NULL pointer before glMapBufferARB().
	// If you do that, the previous data in PBO will be discarded and
	// glMapBufferARB() returns a new allocated pointer immediately
	// even if GPU is still working with the previous data.
		glBufferDataARB(mUsage, mSize, 0, GL_STREAM_DRAW_ARB);
		method = GL_WRITE_ONLY_ARB;
	}
	else
		method = GL_READ_ONLY_ARB;

	
	if((mapper = static_cast<nova::byte *>(glMapBufferARB(mUsage, method))) != NULL)
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

	//UnbindBuffer();

	return result;
}

void CHardwareTextureBuffer::UnlockSource(CMemoryBuffer & buf)
{
	//BindBuffer();

	if(!glUnmapBufferARB(mUsage))
	{
		UnbindBuffer();
		throw NOVA_EXP("HardwareVertexBuffer::LockSource(): Can not map v-buffer memory..", BAD_OPERATION);
	}

	UnbindBuffer();
}

}