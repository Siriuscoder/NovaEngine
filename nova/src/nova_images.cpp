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
#include "nova_stable_precompiled_headers.h"

#include "nova_images.h"

#include <IL/il.h>
#include <IL/ilu.h>

namespace nova
{

CImage::CImage(CResourceManager * rm, const nstring & name, const nstring & group, TAttach state) :
	mWidth(0), mHeight(0), mDepth(0), mCompressedStream(false), mCompressor(SF_BMP),
	CResource(rm, name, group, state)
{
	mCodec = NULL;
}

CImage::~CImage()
{
	nova::nstringstream str;
	str << "CImage::~CImage() " << "name: " << mName << " group: " << mGroup;
	LOG_MESSAGE(str.str().c_str());

	FreeResource();
}

void CImage::SetParam(const nstring & file,
		CImageFormats::NovaPixelFormats format)
{
	if(isReady)
		throw NOVA_EXP("CImage::LoadImage - Image already created.", BAD_OPERATION);

	mPixelFormat = format;
	mFilename = file;

	for(nova::uint i = 0; i < GetListenersCount(); i++)
	{
		CImageListener * lis =
			dynamic_cast<CImageListener *>(GetListener(i));
		lis->LoadImageListener(this);
	}
}

void CImage::SetParam(const CMemoryBuffer & bits,
		nova::uint width,
		nova::uint height,
		nova::uint depth,
		CImageFormats::NovaPixelFormats format)
{
	if(isReady)
		throw NOVA_EXP("CImage::LoadImage - Image already created.", BAD_OPERATION);

	mPixelFormat = format;

	this->mWidth = width;
	this->mHeight = height;
	this->mDepth = depth;

/*	if(bits.GetStreamSize() != mSize)
		throw NOVA_EXP("CImage::LoadImage - Image size value incorrect!", BAD_OPERATION);
*/

	data.AllocBuffer(bits.GetBufferSize());
	bits.CopyTo(data, data.GetBufferSize(), 0);

	for(nova::uint i = 0; i < GetListenersCount(); i++)
	{
		CImageListener * lis =
			dynamic_cast<CImageListener *>(GetListener(i));
		lis->LoadImageListener(this);
	}
}

void CImage::SetParam(const CMemoryBuffer & buffer,
		ESaveFormats compressor,
		CImageFormats::NovaPixelFormats format)
{
	if(isReady)
		throw NOVA_EXP("CImage::LoadImage - Image already created.", BAD_OPERATION);

	mPixelFormat = format;

/*	if(bits.GetStreamSize() != mSize)
		throw NOVA_EXP("CImage::LoadImage - Image size value incorrect!", BAD_OPERATION);
*/

	data.AllocBuffer(buffer.GetBufferSize());
	buffer.CopyTo(data, data.GetBufferSize(), 0);

	mCompressedStream = true;
	mCompressor = compressor;

	for(nova::uint i = 0; i < GetListenersCount(); i++)
	{
		CImageListener * lis =
			dynamic_cast<CImageListener *>(GetListener(i));
		lis->LoadImageListener(this);
	}
}

void CImage::BuildResource(void)
{
	if(Ready())
		return ;

	CResource::BuildResource();
}

void CImage::PrepareResource(void)
{
	if(mFilename.size() > 0)
	{
		// Грузим из файла //

		CImageCodec * codec = CImageCodec::GetCodecForExtention(mFilename);
		if(codec)
		{
			mCodec = codec;
			mCodec->DecodeFromFile(mFilename, this, mPixelFormat);
		}
		else
		{
			char str[256];
			sprintf(str, "CImage::PrepareResource(): Can not find image codec for %s file", mFilename.c_str());  
			throw NOVA_EXP(str, BAD_OPERATION);
		}
	}
	else
	{
		// Используем буфер памяти с сжатым растром//
		if(mCompressedStream)
		{
			CImageCodec * codec = CImageCodec::GetCodecForCompressor(mCompressor);
			if(codec)
			{
				mCodec = codec;
				mCodec->DecodeFromBuffer(data, this, mPixelFormat, mCompressor);
			}
			else
			{
				char str[256];
				sprintf(str, "CImage::PrepareResource(): Can not find image codec for %d compressor", mCompressor);  
				throw NOVA_EXP(str, BAD_OPERATION);
			}
		}
		else
		{
			// Используем буфер памяти с готовым растром//
			CImageFormats inf;
			inf.SetExFormat(mPixelFormat);

			mSize = mWidth * mHeight * mDepth * inf.GetInternalChannels();
			mStride = mWidth * inf.GetInternalChannels();
			mhStride = mHeight * inf.GetInternalChannels();
		}
	}

	CResource::PrepareResource();
}

void CImage::FreeResource()
{
	CResource::FreeResource();
	mWidth = 0;
	mHeight = 0;
	mDepth = 0;

	data.FreeBuffer();
}

void* CImage::GetBitsPtr() const
{
	return data.GetBegin();
}

CMemoryBuffer CImage::GetBits()
{
	return data;
}

#ifdef USING_DEVIL

void CImage::Alienifying()
{
	ILuint image;
	CDevILFormats informat;
	informat.SetExFormat(mPixelFormat);
	// Generate the main image name to use.
	ilGenImages(1, &image);

	// Bind this image name.
	ilBindImage(image);

	ilTexImage(mWidth, mHeight, mDepth, informat.GetInternalChannels(),
		informat.GetFormat(), IL_UNSIGNED_BYTE, data.GetBegin());

	iluAlienify();

	ilCopyPixels(0, 0, 0, mWidth, mHeight, mDepth, informat.GetFormat(),
		IL_UNSIGNED_BYTE, data.GetBegin());

	ilDeleteImages(1, &image);
}

void CImage::Blurring(int iter)
{
	ILuint image;
	CDevILFormats informat;
	informat.SetExFormat(mPixelFormat);
	// Generate the main image name to use.
	ilGenImages(1, &image);

	// Bind this image name.
	ilBindImage(image);

	ilTexImage(mWidth, mHeight, mDepth, informat.GetInternalChannels(),
		informat.GetFormat(), IL_UNSIGNED_BYTE, data.GetBegin());

	iluBlurGaussian(iter);

	ilCopyPixels(0, 0, 0, mWidth, mHeight, mDepth, informat.GetFormat(),
		IL_UNSIGNED_BYTE, data.GetBegin());

	ilDeleteImages(1, &image);
}

void CImage::Contrast(nReal level)
{
	ILuint image;
	CDevILFormats informat;
	informat.SetExFormat(mPixelFormat);
	// Generate the main image name to use.
	ilGenImages(1, &image);

	// Bind this image name.
	ilBindImage(image);

	ilTexImage(mWidth, mHeight, mDepth, informat.GetInternalChannels(),
		informat.GetFormat(), IL_UNSIGNED_BYTE, data.GetBegin());

	iluContrast(level);

	ilCopyPixels(0, 0, 0, mWidth, mHeight, mDepth, informat.GetFormat(),
		IL_UNSIGNED_BYTE, data.GetBegin());

	ilDeleteImages(1, &image);
}

void CImage::Equalization()
{
	ILuint image;
	CDevILFormats informat;
	informat.SetExFormat(mPixelFormat);
	// Generate the main image name to use.
	ilGenImages(1, &image);

	// Bind this image name.
	ilBindImage(image);

	ilTexImage(mWidth, mHeight, mDepth, informat.GetInternalChannels(),
		informat.GetFormat(), IL_UNSIGNED_BYTE, data.GetBegin());

	iluEqualize();

	ilCopyPixels(0, 0, 0, mWidth, mHeight, mDepth, informat.GetFormat(),
		IL_UNSIGNED_BYTE, data.GetBegin());

	ilDeleteImages(1, &image);
}

void CImage::Gamma(nReal correct)
{
	ILuint image;
	CDevILFormats informat;
	informat.SetExFormat(mPixelFormat);
	// Generate the main image name to use.
	ilGenImages(1, &image);

	// Bind this image name.
	ilBindImage(image);

	ilTexImage(mWidth, mHeight, mDepth, informat.GetInternalChannels(),
		informat.GetFormat(), IL_UNSIGNED_BYTE, data.GetBegin());

	iluGammaCorrect(correct);

	ilCopyPixels(0, 0, 0, mWidth, mHeight, mDepth, informat.GetFormat(),
		IL_UNSIGNED_BYTE, data.GetBegin());

	ilDeleteImages(1, &image);
}


void CImage::Negativity()
{
	ILuint image;
	CDevILFormats informat;
	informat.SetExFormat(mPixelFormat);
	// Generate the main image name to use.
	ilGenImages(1, &image);

	// Bind this image name.
	ilBindImage(image);

	ilTexImage(mWidth, mHeight, mDepth, informat.GetInternalChannels(),
		informat.GetFormat(), IL_UNSIGNED_BYTE, data.GetBegin());

	iluNegative();

	ilCopyPixels(0, 0, 0, mWidth, mHeight, mDepth, informat.GetFormat(),
		IL_UNSIGNED_BYTE, data.GetBegin());

	ilDeleteImages(1, &image);
}

void CImage::Noise(nReal tolerance)
{
	ILuint image;
	CDevILFormats informat;
	informat.SetExFormat(mPixelFormat);
	// Generate the main image name to use.
	ilGenImages(1, &image);

	// Bind this image name.
	ilBindImage(image);

	ilTexImage(mWidth, mHeight, mDepth, informat.GetInternalChannels(),
		informat.GetFormat(), IL_UNSIGNED_BYTE, data.GetBegin());

	iluNoisify(tolerance);

	ilCopyPixels(0, 0, 0, mWidth, mHeight, mDepth, informat.GetFormat(),
		IL_UNSIGNED_BYTE, data.GetBegin());

	ilDeleteImages(1, &image);
}

void CImage::Pixelization(nova::uint pix_size)
{
	ILuint image;
	CDevILFormats informat;
	informat.SetExFormat(mPixelFormat);
	// Generate the main image name to use.
	ilGenImages(1, &image);

	// Bind this image name.
	ilBindImage(image);

	ilTexImage(mWidth, mHeight, mDepth, informat.GetInternalChannels(),
		informat.GetFormat(), IL_UNSIGNED_BYTE, data.GetBegin());

	iluPixelize(pix_size);

	ilCopyPixels(0, 0, 0, mWidth, mHeight, mDepth, informat.GetFormat(),
		IL_UNSIGNED_BYTE, data.GetBegin());

	ilDeleteImages(1, &image);
}

void CImage::Sharpering(nReal factor, int iter)
{
	ILuint image;
	CDevILFormats informat;
	informat.SetExFormat(mPixelFormat);
	// Generate the main image name to use.
	ilGenImages(1, &image);

	// Bind this image name.
	ilBindImage(image);

	ilTexImage(mWidth, mHeight, mDepth, informat.GetInternalChannels(),
		informat.GetFormat(), IL_UNSIGNED_BYTE, data.GetBegin());

	iluSharpen(factor, iter);

	ilCopyPixels(0, 0, 0, mWidth, mHeight, mDepth, informat.GetFormat(),
		IL_UNSIGNED_BYTE, data.GetBegin());

	ilDeleteImages(1, &image);
}

#endif

void CImage::BackHeigth()
{
	CMemoryBuffer buf;
	buf.AllocBuffer(data.GetBufferSize());

	for(nova::uint i = 0; i < mHeight; i++)
	{
		memcpy((nova::byte *)buf.GetBegin() + i * mStride,
			(nova::byte *)data.GetBegin() + mStride * (mHeight - 1 - i), mStride);
	}

	buf.CopyTo(data, buf.GetBufferSize(), 0);
	buf.FreeBuffer();
}

void CImage::BackWidth()
{
	CMemoryBuffer buf;
	buf.AllocBuffer(data.GetBufferSize());

	for(nova::uint i = 0; i < mWidth; i++)
	{
		memcpy((nova::byte *)buf.GetBegin() + i * mhStride,
				(nova::byte *)data.GetBegin() + mhStride * (mWidth - 1 - i),  mhStride);
	}

	buf.CopyTo(data, buf.GetBufferSize(), 0);
	buf.FreeBuffer();
}

void CImage::PreAddingAction()
{
	CResource::PreAddingAction();
}

void CImage::PostAddindAction()
{
	CResource::PostAddingAction();
}

void CImage::PreUnloadingAction()
{
	CResource::PreUnloadingAction();
}


template<> CImageManager * CSingelton<CImageManager>::SingeltonObject = NULL;

CImageManager::CImageManager()
{
	LOG_MESSAGE("Image manager created..");
}

CImageManager::~CImageManager()
{
	UnloadAllManagerResources();
	LOG_MESSAGE("Image manager destroyed..");
}

CResourcePtr CImageManager::CreateInstance(const nstring & name,
	const nstring & group, CResource::TAttach state)
{
	CResourcePtr ptr(new CImage(this, name, group, state));
	return ptr;
}

void CImageManager::UnloadAllManagerResources()
{
	UnloadResourceFromHash(this);
}

CImagePtr CImageManager::CreateNewImage(const nstring & name,
	const nstring & group,
	const nstring & file,
	CImageFormats::NovaPixelFormats p,
	CResource::TAttach state)
{
	CImagePtr imgp = CResourceManager::AddNewResource(name, group, state);
	if(imgp.IsNull())
		throw NOVA_EXP("CImageManager::CreateNewImage - resource factory return \
							Null pointer...", MEM_ERROR);

	// Попробуем загрузить изображение
	imgp->SetParam(file, p);
	imgp->PrepareResource();
	CResourceManager::BuildNextResource(name);

	nova::nstringstream str;
	str << "Image Factory: image object name: " << name << " group: " << group << " created...";
	LOG_MESSAGE(str.str());

	return imgp;
}

CImagePtr CImageManager::CreateNewImage(const nstring & name,
	const nstring & group,
	const CMemoryBuffer & bits,
	nova::uint width,
	nova::uint height,
	nova::uint depth,
	CImageFormats::NovaPixelFormats p,
	CResource::TAttach state)
{
	CImagePtr imgp = CResourceManager::AddNewResource(name, group, state);
	if(imgp.IsNull())
		throw NOVA_EXP("CImageManager::CreateNewImage - resource factory return \
							Null pointer...", MEM_ERROR);

	// Попробуем загрузить изображение
	imgp->SetParam(bits, width, height, depth, p);
	imgp->PrepareResource();
	CResourceManager::BuildNextResource(name);

	nova::nstringstream str;
	str << "Image Factory: image object name: " << name << " group: " << group << " created...";
	LOG_MESSAGE(str.str());

	return imgp;
}

CImagePtr CImageManager::CreateNewImageAsync(const nstring & name,
	const nstring & group,
	const nstring & file,
	CImageFormats::NovaPixelFormats p,
	CResource::TAttach state)
{
	CImagePtr imgp = CResourceManager::AddNewResource(name, group, state);
	if(imgp.IsNull())
		throw NOVA_EXP("CImageManager::CreateNewImage - resource factory return \
							Null pointer...", MEM_ERROR);

	imgp->SetParam(file, p);
	imgp->PrepareResource();
	mResourceBuildQueue.AddToQueue(imgp.GetPtr());
	nova::nstringstream str;
	str << "Image Factory: image object name: " << name << " group: " << group << " async created...";
	LOG_MESSAGE(str.str());

	return imgp;
}

CImagePtr CImageManager::CreateNewImageAsync(const nstring & name,
	const nstring & group,
	const CMemoryBuffer & bits,
	nova::uint width,
	nova::uint height,
	nova::uint depth,
	CImageFormats::NovaPixelFormats p,
	CResource::TAttach state)
{
	CImagePtr imgp = CResourceManager::AddNewResource(name, group, state);
	if(imgp.IsNull())
		throw NOVA_EXP("CImageManager::CreateNewImage - resource factory return \
							Null pointer...", MEM_ERROR);

	imgp->SetParam(bits, width, height, depth, p);
	imgp->PrepareResource();
	mResourceBuildQueue.AddToQueue(imgp.GetPtr());
	nova::nstringstream str;
	str << "Image Factory: image object name: " << name << " group: " << group << " async created...";
	LOG_MESSAGE(str.str());

	return imgp;
}

CImagePtr CImageManager::CreateNewImage(const nstring & name, const nstring & group,
	const CMemoryBuffer & bits,
	ESaveFormats compressor,
	CImageFormats::NovaPixelFormats p,
	CResource::TAttach state)
{
	CImagePtr imgp = CResourceManager::AddNewResource(name, group, state);
	if(imgp.IsNull())
		throw NOVA_EXP("CImageManager::CreateNewImage - resource factory return \
							Null pointer...", MEM_ERROR);

	// Попробуем загрузить изображение
	imgp->SetParam(bits, compressor, p);
	imgp->PrepareResource();
	CResourceManager::BuildNextResource(name);

	nova::nstringstream str;
	str << "Image Factory: image object name: " << name << " group: " << group << " created...";
	LOG_MESSAGE(str.str());

	return imgp;
}

CImagePtr CImageManager::CreateNewImageAsync(const nstring & name, const nstring & group,
	const CMemoryBuffer & bits,
	ESaveFormats compressor,
	CImageFormats::NovaPixelFormats p,
	CResource::TAttach state)
{
	CImagePtr imgp = CResourceManager::AddNewResource(name, group, state);
	if(imgp.IsNull())
		throw NOVA_EXP("CImageManager::CreateNewImage - resource factory return \
							Null pointer...", MEM_ERROR);

	// Попробуем загрузить изображение
	imgp->SetParam(bits, compressor, p);
	imgp->PrepareResource();
	mResourceBuildQueue.AddToQueue(imgp.GetPtr());

	nova::nstringstream str;
	str << "Image Factory: image object name: " << name << " group: " << group << " async created...";
	LOG_MESSAGE(str.str());

	return imgp;
}

}
