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
#include "nova_formats.h"
#include "nova_smart_ptr.h"

namespace nova
{

class NOVA_EXPORT CHardwarePixelBuffer : public CHardwareBuffer
{
public:

	enum TargetType
	{
		USE_TEXTURE_1D = 0x5,
		USE_TEXTURE_2D,
		USE_TEXTURE_3D,
		USE_CUBEMAP_TEXTURE// wtf????
	};

protected:

	size_t 	mWidth;
	size_t 	mHeight;
	size_t 	mDepth;
	size_t 	mRowPitch;
	size_t 	mSlicePitch;
	CImageFormats::NovaPixelFormats mPixelFormat;
	GLuint  mTargetId;
	int mFace;
	int mLevel;

	TargetType mTarget;

	CMemoryBuffer LockSource(size_t offset, size_t length, THardwareBufferLock opt);

	void UnlockSource(CMemoryBuffer & buf);

	void SizeToWHD(size_t * Width, size_t * Height, size_t * Depth,
		size_t Start);

	void WHDToSize(size_t Width, size_t Height, size_t Depth,
		size_t * Start);

	size_t 	mLockWidth;
	size_t 	mLockHeight;
	size_t 	mLockDepth;
	size_t 	mLockActSize;

public:

	static GLenum TextureTarget(TargetType type);

	nova::uint GetWidth();

	nova::uint GetHeight();

	nova::uint GetDepth();

	nova::uint GetLockWidth();

	nova::uint GetLockHeight();

	nova::uint GetLockDepth();

	nova::uint GetLockActSize();

	GLuint GetBufferId();

	size_t GetRowPitch();

	size_t GetSlicePitch();

	CImageFormats::NovaPixelFormats GetPixelFormat();

	CHardwarePixelBuffer();

	CHardwarePixelBuffer(GLuint TargetId, TargetType tag, int level,
		int face, CImageFormats::NovaPixelFormats pixelformat);

	~CHardwarePixelBuffer();

	CMemoryBuffer Lock(size_t xoff, size_t yoff, size_t zoff,
		int width, int height, int depth, THardwareBufferLock opt);
};

typedef CSmartPtr<CHardwarePixelBuffer> CHardwarePixelBufferPtr;

class NOVA_EXPORT CTextureSurfaceList : public CBase
{
public:

	typedef nova::stl<CHardwarePixelBufferPtr>::vector TSurfaseList;

	enum MipMapTagertType
	{
		DO_NOT_USE_MIPMAPS,
		USE_SOFTWARE_MIPMAPS,
		USE_HARDWARE_MIPMAPS
	};

protected:

	TSurfaseList mSurfaceList;

	int mMaxMipmaps;
	int mFace;
	CHardwarePixelBuffer::TargetType mType;
	MipMapTagertType mMipState;
	GLuint  mTargetId;
	CImageFormats::NovaPixelFormats mPixelFormat;

public:

	CTextureSurfaceList(CHardwarePixelBuffer::TargetType type,
		MipMapTagertType mip, int face, CImageFormats::NovaPixelFormats pixelformat, GLuint cube);

	void BuildSurfaceList(const CMemoryBuffer & data, size_t width, size_t height, size_t depth);

	~CTextureSurfaceList();

	CHardwarePixelBufferPtr GetSurface(nova::uint level);

	size_t GetMaxMipmaps(size_t width, size_t height, size_t depth);

	CHardwarePixelBuffer::TargetType GetType();

	MipMapTagertType GetMipState();

	GLuint  GetTargetId();

	CImageFormats::NovaPixelFormats GetPixelFormat();

	CHardwarePixelBufferPtr operator[](int level);

	int GetMaxLevels(void);

};

typedef CSmartPtr<CTextureSurfaceList> CTextureSurfaceListPtr;

}
