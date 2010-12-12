/***************************************************************************
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

// For multiple image formats data store
class NOVA_EXPORT CImageBox : public CBase
{
public:

	nova::nUInt32 mWidth;
	nova::nUInt32 mHeight;
	CImageFormats::NovaPixelFormats mPixelFormat;
	nova::nUInt32 mDepth;


	nstring mFilename;
	nstring mCodecName;

	// compress info
	bool mCompressedStream;
	ESaveFormats mCompressor;

	CMemoryBuffer mSourceImageBits;

public:

	CImageBox();

	void CleanData(void);

	void SetData(const CMemoryBuffer & bits,
		nova::nUInt32 width,
		nova::nUInt32 height,
		nova::nUInt32 depth,
		CImageFormats::NovaPixelFormats format);

	void SetData(const nstring & file,
		CImageFormats::NovaPixelFormats format,
		const nstring & codec);

	void SetData(const CMemoryBuffer & buffer,
		ESaveFormats compressor,
		CImageFormats::NovaPixelFormats format,
		const nstring & codec);
};

class NOVA_EXPORT CImage : public CResource
{
friend CImageManager;

private:
	CMemoryBuffer mImage;

	nova::nUInt32 mStride;
	nova::nUInt32 mhStride;

	CImageBox mImageSource;

protected:

	void LoadResourceImpl(void);

	void FreeResourceImpl(void);

	void BuildResourceImpl(void);

public:

	void SetImageSource(const CImageBox &source);

	CImageBox & GetImageSource(void);

	inline nova::nUInt32 GetWidth() const
	{
		return mImageSource.mWidth;
	}

	inline nova::nUInt32 GetHeight() const
	{
		return mImageSource.mHeight;
	}

	inline CImageFormats::NovaPixelFormats GetPixelFormat() const 
	{
		return mImageSource.mPixelFormat;
	}

	inline nova::nUInt32 GetDepth() const
	{
		return mImageSource.mDepth;
	}

	inline nova::nUInt32 GetStride() const 
	{
		return mStride;
	}

	inline nova::nUInt32 GetVertStride() const
	{
		return mhStride;
	}

	CImage(CResourceManager * rm, const nstring & name, const nstring & group, TAttach state);

	~CImage();

#ifdef USING_DEVIL

	void Alienifying();

	void Blurring(nInt32 iter);

	void Contrast(nReal level);

	void Equalization();

	void Gamma(nReal correct);

	void Negativity();

	void Noise(nReal tolerance);

	void Pixelization(nova::nUInt32 pix_size);

	void Sharpering(nReal factor, nInt32 iter);
#endif

	void BackWidth();

	void BackHeigth();

	void* GetBitsPtr() const;

	CMemoryBuffer &GetBits();
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
		const nstring & codec,
		CImageFormats::NovaPixelFormats p = CImageFormats::NF_RGB,
		CResource::TAttach state = CResource::NV_ATTACHED);

	virtual CImagePtr CreateNewImage(const nstring & name, const nstring & group,
		const CMemoryBuffer & bits,
		nova::nUInt32 width,
		nova::nUInt32 height,
		nova::nUInt32 depth,
		CImageFormats::NovaPixelFormats p = CImageFormats::NF_RGB,
		CResource::TAttach state = CResource::NV_ATTACHED);

	virtual CImagePtr CreateNewImage(const nstring & name, const nstring & group,
		const CMemoryBuffer & buffer,
		ESaveFormats compressor,
		const nstring & codec,
		CImageFormats::NovaPixelFormats p = CImageFormats::NF_RGB,
		CResource::TAttach state = CResource::NV_ATTACHED);

	/* for force loading form packages */
	CResourcePtr LoadResourceFromXml(const nstring &filename, const CFilesPackage &package);

	CResourcePtr LoadResourceFromXml(const nstring &filename);
};


}
