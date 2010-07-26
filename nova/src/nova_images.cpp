п»ї/***************************************************************************
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
		// Р“СЂСѓР·РёРј РёР· С„Р°Р№Р»Р° //

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
		// Р