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

#include "nova_hardware_pixel_buffer.h"
#include "nova_render_system.h"

namespace nova
{

nova::uint CHardwarePixelBuffer::GetWidth()
{
	return mWidth;
}

nova::uint CHardwarePixelBuffer::GetHeight()
{
	return mHeight;
}

nova::uint CHardwarePixelBuffer::GetDepth()
{
	return mDepth;
}

CImageFormats::NovaPixelFormats CHardwarePixelBuffer::GetPixelFormat()
{
	return mPixelFormat;
}

size_t CHardwarePixelBuffer::GetRowPitch()
{
	return mRowPitch;
}

size_t CHardwarePixelBuffer::GetSlicePitch()
{
	return mSlicePitch;
}

nova::uint CHardwarePixelBuffer::GetLockWidth()
{
	return mLockWidth;
}

nova::uint CHardwarePixelBuffer::GetLockHeight()
{
	return mLockHeight;
}

nova::uint CHardwarePixelBuffer::GetLockDepth()
{
	return mLockDepth;
}

nova::uint CHardwarePixelBuffer::GetLockActSize()
{
	return mLockActSize;
}

GLenum CHardwarePixelBuffer::TextureTarget(CHardwarePixelBuffer::TargetType type)
{
    switch(type)
    {
        case USE_TEXTURE_1D:
            return GL_TEXTURE_1D;
        case USE_TEXTURE_2D:
            return GL_TEXTURE_2D;
        case USE_TEXTURE_3D:
            return GL_TEXTURE_3D;
        case USE_CUBEMAP_TEXTURE:
            return GL_TEXTURE_CUBE_MAP;
        default:
            return 0;
    };
}

GLuint CHardwarePixelBuffer::GetBufferId()
{
	return mTargetId;
}

CHardwarePixelBuffer::CHardwarePixelBuffer() : mWidth(0), mHeight(0),
	mDepth(0), mRowPitch(0), mPixelFormat(CImageFormats::NF_RGB),
	mTargetId(0), mLevel(0), mTarget(USE_TEXTURE_2D), mFace(0),
	mLockWidth(0), mLockHeight(0), mLockDepth(0), mLockActSize(0)
{


}

CHardwarePixelBuffer::CHardwarePixelBuffer(GLuint TargetId, TargetType tag, int level,
	int face, CImageFormats::NovaPixelFormats pixelformat) : mPixelFormat(pixelformat),
	mTargetId(TargetId), mLevel(level), mTarget(tag), mFace(face),
	mLockWidth(0), mLockHeight(0), mLockDepth(0), mLockActSize(0)
{
	GLint value = 0;
	GLuint face_target = 0;

	GLenum type = TextureTarget(tag);

	if(tag == USE_CUBEMAP_TEXTURE)
		face_target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + face;
	else
		face_target = type;

	glBindTexture(type, TargetId);

	glGetTexLevelParameteriv(face_target, mLevel, GL_TEXTURE_WIDTH, &value);
	mWidth = value;

	if(tag == USE_TEXTURE_1D)
		value = 1;	// Height always 1 for 1D textures
	else
		glGetTexLevelParameteriv(face_target, mLevel, GL_TEXTURE_HEIGHT, &value);
	mHeight = value;

	if(tag != USE_TEXTURE_3D)
		value = 1; // Depth always 1 for non-3D textures
	else
		glGetTexLevelParameteriv(face_target, mLevel, GL_TEXTURE_DEPTH, &value);
	mDepth = value;

	COpenGLFormats informat;
	informat.SetExFormat(mPixelFormat);

	mRowPitch = mWidth;
	mSlicePitch = mWidth * mHeight;
	mSize = mWidth * mHeight * mDepth * informat.GetInternalChannels();
}

CHardwarePixelBuffer::~CHardwarePixelBuffer()
{
	nova::nstringstream str;
	str << "CHardwarePixelBuffer::~CHardwarePixelBuffer() free level "
		<< mLevel << " width: " << mWidth << ", heigth: " << mHeight;
	LOG_MESSAGE(str.str().c_str());
}

void CHardwarePixelBuffer::SizeToWHD(size_t * Width, size_t * Height, size_t * Depth,
		size_t Start)
{
	COpenGLFormats informat;
	informat.SetExFormat(mPixelFormat);
	unsigned int line = mRowPitch;
	unsigned int sec = mSlicePitch;
	Start /= informat.GetInternalChannels();

	if(Start < line)
	{
		*Width = Start;
		*Height = 0;
		*Depth = 0;
	}
	else if(Start >= line && Start < sec)
	{
		*Height = Start / line;
		*Width = Start % line;
		*Depth = 0;
	}
	else if(Start >= sec)
	{
		*Depth = Start / sec;
		unsigned int slice = Start % sec;

		if(slice < line)
		{
			*Width = slice;
			*Height = 0;
		}
		else if(slice >= line)
		{
			*Height = slice / line;
			*Width = slice % line;
		}
	}
}

void CHardwarePixelBuffer::WHDToSize(size_t Width, size_t Height, size_t Depth,
		size_t * Start)
{
	COpenGLFormats informat;
	informat.SetExFormat(mPixelFormat);
	unsigned int line = mRowPitch * informat.GetInternalChannels();
	unsigned int sec = mSlicePitch * informat.GetInternalChannels();

	*Start = (Width * informat.GetInternalChannels()) +
		(line * Height) + (sec * Depth);
}

CMemoryBuffer CHardwarePixelBuffer::LockSource(size_t offset, size_t length, THardwareBufferLock opt)
{
	if(mSize == 0)
		throw NOVA_EXP("CHardwarePixelBuffer::LockSource - \
		mSize == 0, null pixel box..", BAD_OPERATION);
	CMemoryBuffer data;
	GLuint face_target = 0;

	data.AllocBuffer(mSize);
	COpenGLFormats informat;
	informat.SetExFormat(mPixelFormat);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	GLenum type = TextureTarget(mTarget);

	if(mTarget == USE_CUBEMAP_TEXTURE)
		face_target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + mFace;
	else
		face_target = type;

	glBindTexture(type, mTargetId);

	glGetTexImage(face_target, mLevel, informat.GetFormat(),
		GL_UNSIGNED_BYTE, data.GetBegin());

	glPixelStorei(GL_PACK_ALIGNMENT, 4);

	int error = glGetError();
	if(error != GL_NO_ERROR)
	{
		nstringstream str;
		str << "CHardwarePixelBuffer::LockSource - \
		detecting OpenGL error with code " << error;
		throw NOVA_EXP(str.str().c_str(), BAD_OPERATION);
	}

	CMemoryBuffer result;
	result.AllocBuffer(mLockActSize);
	size_t line = mLockWidth * informat.GetInternalChannels();

	nova::byte * source = (nova::byte *)data.GetBegin() + offset;
	nova::byte * dest = (nova::byte *)result.GetBegin();
	for(nova::uint i = 0; i < mLockDepth; ++i)
	{
		for(nova::uint j = 0; j < mLockHeight; ++j)
		{
			memcpy(dest, source, line);
			source += mRowPitch * informat.GetInternalChannels();
			dest += line;
		}
		source += mSlicePitch * informat.GetInternalChannels();
	}

	data.FreeBuffer();

	return result;
}

CMemoryBuffer CHardwarePixelBuffer::Lock(size_t xoff,
	size_t yoff, size_t zoff, int width, int height, int depth, THardwareBufferLock opt)
{
	size_t start, end;

	WHDToSize(xoff, yoff, zoff, &start);
	WHDToSize(xoff + width-1, yoff + height-1, zoff + depth-1, &end);
	COpenGLFormats informat;
	informat.SetExFormat(mPixelFormat);
	mLockWidth = width;
	mLockHeight = height;
	mLockDepth = depth;
	mLockActSize = mLockWidth * mLockHeight * mLockDepth * informat.GetInternalChannels();

	return CHardwareBuffer::Lock(start, end - start, opt);
}

void CHardwarePixelBuffer::UnlockSource(CMemoryBuffer & buf)
{
	if(mSize == 0)
		throw NOVA_EXP("CHardwarePixelBuffer::UnlockSource - \
		mSize == 0, null pixel box..", BAD_OPERATION);
	GLuint face_target = 0;
	void *ptr = buf.GetBegin();

	if(!ptr)
		throw NOVA_EXP("CHardwarePixelBuffer::LockSource - \
			ptr == NULL, bad ptr..", BAD_OPERATION);

	COpenGLFormats informat;
	informat.SetExFormat(mPixelFormat);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	GLenum type = TextureTarget(mTarget);

	if(mTarget == USE_CUBEMAP_TEXTURE)
		face_target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + mFace;
	else
		face_target = type;

	glBindTexture(type, mTargetId);

	size_t xoff, yoff, zoff;
	SizeToWHD(&xoff, &yoff, &zoff, mLockStart);

	nova::uint _width;
	nova::uint _height;
	nova::uint _depth;
	SizeToWHD(&_width, &_height, &_depth, mLockStart + mLockSize);

	_width -= (xoff-1);
	_height -= (yoff-1);
	_depth -= (zoff-1);

	switch(mTarget)
	{
	case USE_TEXTURE_1D:
		{
			glTexSubImage1D(GL_TEXTURE_1D, mLevel, xoff, _width, informat.GetFormat(),
				GL_UNSIGNED_BYTE, ptr);
		}
		break;
	case USE_TEXTURE_2D:
	case USE_CUBEMAP_TEXTURE:
		{
			glTexSubImage2D(face_target, mLevel, xoff, yoff, _width,
				_height, informat.GetFormat(), GL_UNSIGNED_BYTE, ptr);
		}
		break;
	case USE_TEXTURE_3D:
		{
			glTexSubImage3D(GL_TEXTURE_3D, mLevel, xoff, yoff, zoff, _width,
				_height, _depth, informat.GetFormat(), GL_UNSIGNED_BYTE, ptr);
		}
		break;
	}

	mLockWidth = 0;
	mLockHeight = 0;
	mLockDepth = 0;
	mLockActSize = 0;

	glPixelStorei(GL_PACK_ALIGNMENT, 4);

	buf.FreeBuffer();
}

CTextureSurfaceList::CTextureSurfaceList(CHardwarePixelBuffer::TargetType type,
	MipMapTagertType mip, int face, CImageFormats::NovaPixelFormats pixelformat, GLuint cube) : CBase("CTextureSurfaceList")
{
	mType = type;
	mMipState = mip;
	mPixelFormat = pixelformat;
	mFace = face;
	mTargetId = cube;

	//BuildSurfaceList(data, width, height, depth);
}

CHardwarePixelBuffer::TargetType CTextureSurfaceList::GetType()
{
	return mType;
}

CTextureSurfaceList::MipMapTagertType CTextureSurfaceList::GetMipState()
{
	return mMipState;
}

GLuint  CTextureSurfaceList::GetTargetId()
{
	return mTargetId;
}

size_t CTextureSurfaceList::GetMaxMipmaps(size_t width, size_t height, size_t depth)
{
	size_t count = 0;
	do {
		if(width>1)		width = width/2;
		if(height>1)	height = height/2;
		if(depth>1)		depth = depth/2;
		/*
		NOT needed, compressed formats will have mipmaps up to 1x1
		if(PixelUtil::isValidExtent(width, height, depth, format))
			count ++;
		else
			break;
		*/

		count ++;
	} while(!(width == 1 && height == 1 && depth == 1));

	return count;
}

void CTextureSurfaceList::BuildSurfaceList(const CMemoryBuffer & data, size_t width, size_t height, size_t depth)
{
	if(!width || !height || !depth)
		throw NOVA_EXP("CTextureSurfaceList::BuildSurfaseList - incorrect input size param.", BAD_OPERATION);

	GLenum type = CHardwarePixelBuffer::TextureTarget(mType);
	bool mipmap_hardware_gen = CRenderSystem::GetSingelton().CheckCapabilities(TEXTURE_CATEGORY, CAP_AUTOMIPMAP);
	COpenGLFormats informat;
	informat.SetExFormat(mPixelFormat);
	mMaxMipmaps = GetMaxMipmaps(width, height, depth);

	glEnable(type);

	if(mType == CHardwarePixelBuffer::USE_CUBEMAP_TEXTURE && mFace > 0) {}
	else
		glGenTextures(1, &mTargetId);

	glBindTexture(type, mTargetId);

	if(mMipState == USE_HARDWARE_MIPMAPS)
	{
		if(!mipmap_hardware_gen)
			mMipState = USE_SOFTWARE_MIPMAPS;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	if(mMipState == DO_NOT_USE_MIPMAPS)
	{
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		mMaxMipmaps = 1;

		switch(mType)
		{
		case CHardwarePixelBuffer::USE_TEXTURE_1D:
			glTexImage1D(GL_TEXTURE_1D, 0, informat.GetInternalChannels(),
				width, 0, informat.GetFormat(), GL_UNSIGNED_BYTE, data.GetBegin());
			break;
		case CHardwarePixelBuffer::USE_TEXTURE_2D:
			glTexImage2D(GL_TEXTURE_2D, 0, informat.GetInternalChannels(),
				width, height, 0, informat.GetFormat(), GL_UNSIGNED_BYTE, data.GetBegin());
			break;
		case CHardwarePixelBuffer::USE_TEXTURE_3D:
			glTexImage3D(GL_TEXTURE_3D, 0, informat.GetInternalChannels(),
				width, height, depth, 0, informat.GetFormat(), GL_UNSIGNED_BYTE, data.GetBegin());
			break;
		case CHardwarePixelBuffer::USE_CUBEMAP_TEXTURE:
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + mFace, 0, informat.GetInternalChannels(),
				width, height, 0, informat.GetFormat(), GL_UNSIGNED_BYTE, data.GetBegin());
			break;
		}
	}
	else if(mMipState == USE_SOFTWARE_MIPMAPS)
	{
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		switch(mType)
		{
		case CHardwarePixelBuffer::USE_TEXTURE_1D:
			gluBuild1DMipmaps(GL_TEXTURE_1D, informat.GetInternalChannels(),
				width, informat.GetFormat(), GL_UNSIGNED_BYTE, data.GetBegin());
			break;
		case CHardwarePixelBuffer::USE_TEXTURE_2D:
			gluBuild2DMipmaps(GL_TEXTURE_2D, informat.GetInternalChannels(),
				width, height, informat.GetFormat(), GL_UNSIGNED_BYTE, data.GetBegin());
			break;
		case CHardwarePixelBuffer::USE_CUBEMAP_TEXTURE:
			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X + mFace, informat.GetInternalChannels(),
				width, height, informat.GetFormat(), GL_UNSIGNED_BYTE, data.GetBegin());
			break;
		case CHardwarePixelBuffer::USE_TEXTURE_3D:
			throw NOVA_EXP("CTextureSurfaceList::BuildSurfaseList - \
				OpenGL can not support software 3D mipmaps", BAD_OPERATION);
			break;
		}
	}
	else if(mMipState == USE_HARDWARE_MIPMAPS)
	{
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexParameteri(type, GL_TEXTURE_MAX_LEVEL_SGIS, mMaxMipmaps);
		glTexParameteri(type, GL_GENERATE_MIPMAP_SGIS, GL_TRUE );

		switch(mType)
		{
		case CHardwarePixelBuffer::USE_TEXTURE_1D:
			glTexImage1D(GL_TEXTURE_1D, 0, informat.GetInternalChannels(),
				width, 0, informat.GetFormat(), GL_UNSIGNED_BYTE, data.GetBegin());
			break;
		case CHardwarePixelBuffer::USE_TEXTURE_2D:
			glTexImage2D(GL_TEXTURE_2D, 0, informat.GetInternalChannels(),
				width, height, 0, informat.GetFormat(), GL_UNSIGNED_BYTE, data.GetBegin());
			break;
		case CHardwarePixelBuffer::USE_TEXTURE_3D:
			glTexImage3D(GL_TEXTURE_3D, 0, informat.GetInternalChannels(),
				width, height, depth, 0, informat.GetFormat(), GL_UNSIGNED_BYTE, data.GetBegin());
			break;
		case CHardwarePixelBuffer::USE_CUBEMAP_TEXTURE:
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + mFace, 0, informat.GetInternalChannels(),
				width, height, 0, informat.GetFormat(), GL_UNSIGNED_BYTE, data.GetBegin());
			break;
		}

		glTexParameteri(type, GL_GENERATE_MIPMAP_SGIS, GL_FALSE );
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	int error = glGetError();
	if(error != GL_NO_ERROR)
	{
		nstringstream str;
		str << "CTextureSurfaceList::BuildSurfaseList - \
		detecting OpenGL error with code " << error;
		throw NOVA_EXP(str.str().c_str(), BAD_OPERATION);
	}

	for(int level = 0; level < mMaxMipmaps; ++level)
	{
		CHardwarePixelBufferPtr hpb(new CHardwarePixelBuffer(mTargetId, mType,
			level, mFace, mPixelFormat));
		mSurfaceList.push_back(hpb);
	}
}

CTextureSurfaceList::~CTextureSurfaceList()
{
	nova::nstringstream str;
	str << "CTextureSurfaceList::~CTextureSurfaceList() free texture) ";
	LOG_MESSAGE(str.str().c_str());

	mSurfaceList.clear();
	glDeleteTextures(1, &mTargetId);
}

CHardwarePixelBufferPtr CTextureSurfaceList::GetSurface(nova::uint level)
{
	if(level >= mSurfaceList.size())
		throw NOVA_EXP("CTextureSurfaceList::GetSurface - level out of range..", BAD_OPERATION);

	return mSurfaceList[level];
}

CHardwarePixelBufferPtr CTextureSurfaceList::operator[](int level)
{
	return GetSurface(level);
}

CImageFormats::NovaPixelFormats CTextureSurfaceList::GetPixelFormat()
{
	return mPixelFormat;
}

int CTextureSurfaceList::GetMaxLevels(void)
{
	return mSurfaceList.size();
}


}
