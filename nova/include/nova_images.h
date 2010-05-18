﻿/***************************************************************************
 *   Copyright (C) 2009 by Sirius										   *
 *   siriusnick@gmail.com												   *
 *                                                                         *
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

#include "nova_streams.h"
#include "nova_devil_codec.h"
#include "nova_resource_manager.h"

namespace nova
{

class CImage;
class CImageManager;

class NOVA_EXPORT CImageListener : public CResourceListener
{
public:

	virtual void LoadImageListener(CImage * object) {}

};

class NOVA_EXPORT CImage : public CResource
{
friend CDevILCodec;
friend CImageManager;

private:
	CMemoryBuffer data;

	nova::uint mWidth;
	nova::uint mHeight;
	CImageFormats::NovaPixelFormats mPixelFormat;
	nova::uint mDepth;
	nova::uint mStride;
	nova::uint mhStride;
	nstring mFilename;
	CImageCodec * mCodec;

	// compress info
	bool mCompressedStream;
	ESaveFormats mCompressor;

protected:

	void SetParam(const nstring & file,
		CImageFormats::NovaPixelFormats format);

	void SetParam(const CMemoryBuffer & bits,
		nova::uint width,
		nova::uint height,
		nova::uint depth,
		CImageFormats::NovaPixelFormats format);

	void SetParam(const CMemoryBuffer & buffer,
		ESaveFormats compressor,
		CImageFormats::NovaPixelFormats format);

public:

	inline nova::uint GetWidth() const
	{
		return mWidth;
	}

	inline nova::uint GetHeight() const
	{
		return mHeight;
	}

	inline CImageFormats::NovaPixelFormats GetPixelFormat() const 
	{
		return mPixelFormat;
	}

	inline nova::uint GetDepth() const
	{
		return mDepth;
	}

	inline void SetPixelFormat(CImageFormats::NovaPixelFormats p)
	{
		mPixelFormat = p;
	}

	inline nova::uint GetStride() const 
	{
		return mStride;
	}

	inline nova::uint GetVertStride() const
	{
		return mhStride;
	}

	CImage(CResourceManager * rm, const nstring & name, const nstring & group, TAttach state);

	~CImage();

	virtual void BuildResource(void);

	virtual void PrepareResource(void);

#ifdef USING_DEVIL

	void Alienifying();

	void Blurring(int iter);

	void Contrast(real level);

	void Equalization();

	void Gamma(real correct);

	void Negativity();

	void Noise(real tolerance);

	void Pixelization(nova::uint pix_size);

	void Sharpering(real factor, int iter);
#endif

	void BackWidth();

	void BackHeigth();

	void* GetBitsPtr() const;

	CMemoryBuffer GetBits();

	virtual void FreeResource();

	virtual void PreAddingAction();
	virtual void PostAddindAction();

	virtual void PreUnloadingAction();
};

typedef CSmartPtr<CImage> CImagePtr;

class NOVA_EXPORT CImageManager : public CSingelton<CImageManager>, public CResourceManager
{
protected:


public:

	CImageManager();
	~CImageManager();

	virtual CResourcePtr CreateInstance(const nstring & name,
		const nstring & group, CResource::TAttach state);

	virtual void UnloadAllManagerResources();

	virtual CImagePtr CreateNewImage(const nstring & name, const nstring & group,
		const nstring & file,
		CImageFormats::NovaPixelFormats p = CImageFormats::NF_RGB,
		CResource::TAttach state = CResource::NV_ATTACHED);

	virtual CImagePtr CreateNewImage(const nstring & name, const nstring & group,
		const CMemoryBuffer & bits,
		nova::uint width,
		nova::uint height,
		nova::uint depth,
		CImageFormats::NovaPixelFormats p = CImageFormats::NF_RGB,
		CResource::TAttach state = CResource::NV_ATTACHED);

	virtual CImagePtr CreateNewImage(const nstring & name, const nstring & group,
		const CMemoryBuffer & bits,
		ESaveFormats compressor,
		CImageFormats::NovaPixelFormats p = CImageFormats::NF_RGB,
		CResource::TAttach state = CResource::NV_ATTACHED);

	virtual CImagePtr CreateNewImageAsync(const nstring & name, const nstring & group,
		const nstring & file,
		CImageFormats::NovaPixelFormats p = CImageFormats::NF_RGB,
		CResource::TAttach state = CResource::NV_ATTACHED);

	virtual CImagePtr CreateNewImageAsync(const nstring & name, const nstring & group,
		const CMemoryBuffer & bits,
		nova::uint width,
		nova::uint height,
		nova::uint depth,
		CImageFormats::NovaPixelFormats p = CImageFormats::NF_RGB,
		CResource::TAttach state = CResource::NV_ATTACHED);

	virtual CImagePtr CreateNewImageAsync(const nstring & name, const nstring & group,
		const CMemoryBuffer & bits,
		ESaveFormats compressor,
		CImageFormats::NovaPixelFormats p = CImageFormats::NF_RGB,
		CResource::TAttach state = CResource::NV_ATTACHED);
};

}
