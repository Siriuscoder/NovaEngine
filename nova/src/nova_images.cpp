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
#include "nova_string_utils.h"

#include <IL/il.h>
#include <IL/ilu.h>

namespace nova
{

CImageBox::CImageBox() : CBase("CImageBox")
{
	CleanData();
}

void CImageBox::CleanData(void)
{
	mWidth = 0;
	mHeight = 0;
	mDepth = 0;
	mPixelFormat = CImageFormats::NF_RGB;
	mFilename.clear();
	mCodecName.clear();

	// compress info
	mCompressedStream = false;
	mCompressor = SF_BMP;
}

void CImageBox::SetData(const CMemoryBuffer & bits,
		nova::nUInt32 width,
		nova::nUInt32 height,
		nova::nUInt32 depth,
		CImageFormats::NovaPixelFormats format)
{
	CleanData();
	mSourceImageBits = bits;
	mWidth = width;
	mHeight = height;
	mDepth = depth;

	mPixelFormat = format;
}

void CImageBox::SetData(const nstring & file,
		CImageFormats::NovaPixelFormats format,
		const nstring & codec)
{
	CleanData();
	mPixelFormat = format;
	mFilename = file;
	mCodecName = codec;
}

void CImageBox::SetData(const CMemoryBuffer & buffer,
		ESaveFormats compressor,
		CImageFormats::NovaPixelFormats format,
		const nstring & codec)
{
	CleanData();
	mSourceImageBits = buffer;
	mCodecName = codec;
	mPixelFormat = format;
	mCompressedStream = true;
}

CImage::CImage(CResourceManager * rm, const nstring & name, const nstring & group, TAttach state) :
	CResource(rm, name, group, state)
{

}

CImage::~CImage()
{
	nova::nstringstream str;
	str << "CImage::~CImage() " << "name: " << mName << " group: " << mGroup;
	LOG_MESSAGE(str.str().c_str());

	FreeResource();
}
/*
void CImage::SetBits(const CMemoryBuffer & bits,
		nova::nUInt32 width,
		nova::nUInt32 height,
		nova::nUInt32 depth,
		CImageFormats::NovaPixelFormats format)
{
	if(isReady)
		throw NOVA_EXP("CImage::LoadImage - Image already created.", BAD_OPERATION);

	mPixelFormat = format;

	this->mWidth = width;
	this->mHeight = height;
	this->mDepth = depth;


	mImage.AllocBuffer(bits.GetBufferSize());
	bits.CopyTo(mImage, mImage.GetBufferSize(), 0);
}

*/

void CImage::SetImageSource(const CImageBox &source)
{
	mImageSource = source;
}

void CImage::BuildResourceImpl(void)
{


}

void CImage::LoadResourceImpl(void)
{
	if(mImageSource.mFilename.size() > 0)
	{
		// Грузим из файла //

		CImageCodec * codec = CImageCodec::GetCodec(mImageSource.mCodecName);
		if(codec)
		{
			codec->DecodeFromFile(mImageSource.mFilename, this, mImageSource.mPixelFormat);
		}
		else
		{
			char str[256];
			sprintf(str, "CImage::LoadResourceImpl(): Can not load file %s (for image %s), reason: codec name %s not registered..", 
				mImageSource.mFilename.c_str(), this->GetResName(), mImageSource.mCodecName.c_str());  
			throw NOVA_EXP(str, BAD_OPERATION);
		}
	}
	else
	{
		// Используем буфер памяти с сжатым растром//
		if(mImageSource.mCompressedStream)
		{
			CImageCodec * codec = CImageCodec::GetCodec(mImageSource.mCodecName);
			if(codec)
			{
				codec->DecodeFromBuffer(mImage, this, mImageSource.mPixelFormat, mImageSource.mCompressor);
			}
			else
			{
				char str[256];
				sprintf(str, "CImage::LoadResourceImpl(): Can not load image %s from compressed stream, reason: codec name %s not registered..", 
					this->GetResName(), mImageSource.mCodecName);  
				throw NOVA_EXP(str, BAD_OPERATION);
			}
		}
		else
		{
			// Используем буфер памяти с готовым растром//
			mImage.AllocBuffer(mImageSource.mSourceImageBits.GetBufferSize());
			mImageSource.mSourceImageBits.CopyTo(mImage, mImage.GetBufferSize(), 0);
		}
	}

	CImageFormats inf;
	inf.SetExFormat(mImageSource.mPixelFormat);

	mSize =  mImageSource.mWidth *  mImageSource.mHeight *  mImageSource.mDepth * inf.GetInternalChannels();
	mStride =  mImageSource.mWidth * inf.GetInternalChannels();
	mhStride =  mImageSource.mHeight * inf.GetInternalChannels();
}

void CImage::FreeResourceImpl()
{
	mSize = 0;
	mStride = 0;
	mhStride = 0;

	mImage.FreeBuffer();
}

void* CImage::GetBitsPtr() const
{
	return mImage.GetBegin();
}

CMemoryBuffer &CImage::GetBits()
{
	return mImage;
}

CImageBox & CImage::GetImageSource(void)
{
	return mImageSource;
}

#ifdef USING_DEVIL

void CImage::Alienifying()
{
	ILuint image;
	CDevILFormats informat;
	informat.SetExFormat(mImageSource.mPixelFormat);
	// Generate the main image name to use.
	ilGenImages(1, &image);

	// Bind this image name.
	ilBindImage(image);

	ilTexImage(mImageSource.mWidth, mImageSource.mHeight, mImageSource.mDepth, informat.GetInternalChannels(),
		informat.GetFormat(), IL_UNSIGNED_BYTE, mImage.GetBegin());

	iluAlienify();

	ilCopyPixels(0, 0, 0, mImageSource.mWidth, mImageSource.mHeight, mImageSource.mDepth, informat.GetFormat(),
		IL_UNSIGNED_BYTE, mImage.GetBegin());

	ilDeleteImages(1, &image);
}

void CImage::Blurring(nInt32 iter)
{
	ILuint image;
	CDevILFormats informat;
	informat.SetExFormat(mImageSource.mPixelFormat);
	// Generate the main image name to use.
	ilGenImages(1, &image);

	// Bind this image name.
	ilBindImage(image);

	ilTexImage(mImageSource.mWidth, mImageSource.mHeight, mImageSource.mDepth, informat.GetInternalChannels(),
		informat.GetFormat(), IL_UNSIGNED_BYTE, mImage.GetBegin());

	iluBlurGaussian(iter);

	ilCopyPixels(0, 0, 0, mImageSource.mWidth, mImageSource.mHeight, mImageSource.mDepth, informat.GetFormat(),
		IL_UNSIGNED_BYTE, mImage.GetBegin());

	ilDeleteImages(1, &image);
}

void CImage::Contrast(nReal level)
{
	ILuint image;
	CDevILFormats informat;
	informat.SetExFormat(mImageSource.mPixelFormat);
	// Generate the main image name to use.
	ilGenImages(1, &image);

	// Bind this image name.
	ilBindImage(image);

	ilTexImage(mImageSource.mWidth, mImageSource.mHeight, mImageSource.mDepth, informat.GetInternalChannels(),
		informat.GetFormat(), IL_UNSIGNED_BYTE, mImage.GetBegin());

	iluContrast(level);

	ilCopyPixels(0, 0, 0, mImageSource.mWidth, mImageSource.mHeight, mImageSource.mDepth, informat.GetFormat(),
		IL_UNSIGNED_BYTE, mImage.GetBegin());

	ilDeleteImages(1, &image);
}

void CImage::Equalization()
{
	ILuint image;
	CDevILFormats informat;
	informat.SetExFormat(mImageSource.mPixelFormat);
	// Generate the main image name to use.
	ilGenImages(1, &image);

	// Bind this image name.
	ilBindImage(image);

	ilTexImage(mImageSource.mWidth, mImageSource.mHeight, mImageSource.mDepth, informat.GetInternalChannels(),
		informat.GetFormat(), IL_UNSIGNED_BYTE, mImage.GetBegin());

	iluEqualize();

	ilCopyPixels(0, 0, 0, mImageSource.mWidth, mImageSource.mHeight, mImageSource.mDepth, informat.GetFormat(),
		IL_UNSIGNED_BYTE, mImage.GetBegin());

	ilDeleteImages(1, &image);
}

void CImage::Gamma(nReal correct)
{
	ILuint image;
	CDevILFormats informat;
	informat.SetExFormat(mImageSource.mPixelFormat);
	// Generate the main image name to use.
	ilGenImages(1, &image);

	// Bind this image name.
	ilBindImage(image);

	ilTexImage(mImageSource.mWidth, mImageSource.mHeight, mImageSource.mDepth, informat.GetInternalChannels(),
		informat.GetFormat(), IL_UNSIGNED_BYTE, mImage.GetBegin());

	iluGammaCorrect(correct);

	ilCopyPixels(0, 0, 0, mImageSource.mWidth, mImageSource.mHeight, mImageSource.mDepth, informat.GetFormat(),
		IL_UNSIGNED_BYTE, mImage.GetBegin());

	ilDeleteImages(1, &image);
}


void CImage::Negativity()
{
	ILuint image;
	CDevILFormats informat;
	informat.SetExFormat(mImageSource.mPixelFormat);
	// Generate the main image name to use.
	ilGenImages(1, &image);

	// Bind this image name.
	ilBindImage(image);

	ilTexImage(mImageSource.mWidth, mImageSource.mHeight, mImageSource.mDepth, informat.GetInternalChannels(),
		informat.GetFormat(), IL_UNSIGNED_BYTE, mImage.GetBegin());

	iluNegative();

	ilCopyPixels(0, 0, 0, mImageSource.mWidth, mImageSource.mHeight, mImageSource.mDepth, informat.GetFormat(),
		IL_UNSIGNED_BYTE, mImage.GetBegin());

	ilDeleteImages(1, &image);
}

void CImage::Noise(nReal tolerance)
{
	ILuint image;
	CDevILFormats informat;
	informat.SetExFormat(mImageSource.mPixelFormat);
	// Generate the main image name to use.
	ilGenImages(1, &image);

	// Bind this image name.
	ilBindImage(image);

	ilTexImage(mImageSource.mWidth, mImageSource.mHeight, mImageSource.mDepth, informat.GetInternalChannels(),
		informat.GetFormat(), IL_UNSIGNED_BYTE, mImage.GetBegin());

	iluNoisify(tolerance);

	ilCopyPixels(0, 0, 0, mImageSource.mWidth, mImageSource.mHeight, mImageSource.mDepth, informat.GetFormat(),
		IL_UNSIGNED_BYTE, mImage.GetBegin());

	ilDeleteImages(1, &image);
}

void CImage::Pixelization(nova::nUInt32 pix_size)
{
	ILuint image;
	CDevILFormats informat;
	informat.SetExFormat(mImageSource.mPixelFormat);
	// Generate the main image name to use.
	ilGenImages(1, &image);

	// Bind this image name.
	ilBindImage(image);

	ilTexImage(mImageSource.mWidth, mImageSource.mHeight, mImageSource.mDepth, informat.GetInternalChannels(),
		informat.GetFormat(), IL_UNSIGNED_BYTE, mImage.GetBegin());

	iluPixelize(pix_size);

	ilCopyPixels(0, 0, 0, mImageSource.mWidth, mImageSource.mHeight, mImageSource.mDepth, informat.GetFormat(),
		IL_UNSIGNED_BYTE, mImage.GetBegin());

	ilDeleteImages(1, &image);
}

void CImage::Sharpering(nReal factor, nInt32 iter)
{
	ILuint image;
	CDevILFormats informat;
	informat.SetExFormat(mImageSource.mPixelFormat);
	// Generate the main image name to use.
	ilGenImages(1, &image);

	// Bind this image name.
	ilBindImage(image);

	ilTexImage(mImageSource.mWidth, mImageSource.mHeight, mImageSource.mDepth, informat.GetInternalChannels(),
		informat.GetFormat(), IL_UNSIGNED_BYTE, mImage.GetBegin());

	iluSharpen(factor, iter);

	ilCopyPixels(0, 0, 0, mImageSource.mWidth, mImageSource.mHeight, mImageSource.mDepth, informat.GetFormat(),
		IL_UNSIGNED_BYTE, mImage.GetBegin());

	ilDeleteImages(1, &image);
}

#endif

void CImage::BackHeigth()
{
	CMemoryBuffer buf;
	buf.AllocBuffer(mImage.GetBufferSize());

	for(nova::nUInt32 i = 0; i < mImageSource.mHeight; i++)
	{
		memcpy((nova::nByte *)buf.GetBegin() + i * mStride,
			(nova::nByte *)mImage.GetBegin() + mStride * (mImageSource.mHeight - 1 - i), mStride);
	}

	buf.CopyTo(mImage, buf.GetBufferSize(), 0);
	buf.FreeBuffer();
}

void CImage::BackWidth()
{
	if(!IsLoaded())
		return;

	CMemoryBuffer buf;
	buf.AllocBuffer(mImage.GetBufferSize());

	for(nova::nUInt32 i = 0; i < mImageSource.mWidth; i++)
	{
		memcpy((nova::nByte *)buf.GetBegin() + i * mhStride,
				(nova::nByte *)mImage.GetBegin() + mhStride * (mImageSource.mWidth - 1 - i),  mhStride);
	}

	buf.CopyTo(mImage, buf.GetBufferSize(), 0);
	buf.FreeBuffer();
}

void CImage::SerializeToXmlFileImpl(xmlTextWriterPtr writer)
{
	if(!writer)
		return;

	if(xmlTextWriterWriteElement(writer, BAD_CAST "ImageFile", BAD_CAST mImageSource.mFilename.c_str()) < 0)
		NOVA_EXP("CImage::SerializeToXmlFileImpl: xmlTextWriterWriteElement fail", BAD_OPERATION);
	if(xmlTextWriterWriteElement(writer, BAD_CAST "ImageCodec", BAD_CAST mImageSource.mCodecName.c_str()) < 0)
		NOVA_EXP("CImage::SerializeToXmlFileImpl: xmlTextWriterWriteElement fail", BAD_OPERATION);

	std::stringstream str;
	str << "Compressor info: "	
		<< "SF_BMP = " << (int)SF_BMP 
		<< ", SF_ICO = " << (int)SF_ICO
		<< ", SF_JPG = " << (int)SF_JPG
		<< ", SF_PCX = " << (int)SF_PCX
		<< ", SF_PIC = " << (int)SF_PIC
		<< ", SF_PNG = " << (int)SF_PNG
		<< ", SF_TGA = " << (int)SF_TGA
		<< ", SF_TIF = " << (int)SF_TIF
		<< ", SF_GIF = " << (int)SF_GIF
		<< ", SF_DDS = " << (int)SF_DDS
		<< ", SF_PIX = " << (int)SF_PIX
		<< ", SF_HDR = " << (int)SF_HDR;

	if(xmlTextWriterWriteComment(writer, BAD_CAST str.str().c_str()) < 0)
		NOVA_EXP("CResource::SerializeToXmlFile: xmlTextWriterWriteComment fail", BAD_OPERATION);

	if(xmlTextWriterWriteFormatElement(writer, BAD_CAST "Compressor", "%d", (int)mImageSource.mCompressor) < 0)
		NOVA_EXP("CImage::SerializeToXmlFileImpl: xmlTextWriterWriteElement fail", BAD_OPERATION);

	str.clear();
	str << "Optional inmage format (default RGB): "
		<< "NF_RED = " << (int)CImageFormats::NF_RED
		<< ", NF_GREEN = " << (int)CImageFormats::NF_GREEN
		<< ", NF_BLUE = " << (int)CImageFormats::NF_BLUE
		<< ", NF_ALPHA = " << (int)CImageFormats::NF_ALPHA
		<< ", NF_RGB = " << (int)CImageFormats::NF_RGB
		<< ", NF_RGBA = " << (int)CImageFormats::NF_RGBA
		<< ", NF_BGR = " << (int)CImageFormats::NF_BGR
		<< ", NF_BGRA = " << (int)CImageFormats::NF_BGRA
		<< ", NF_LUMINANCE = " << (int)CImageFormats::NF_LUMINANCE
		<< ", NF_LUMINANCE_ALPHA = " << (int)CImageFormats::NF_LUMINANCE_ALPHA;

	if(xmlTextWriterWriteComment(writer, BAD_CAST str.str().c_str()) < 0)
		NOVA_EXP("CResource::SerializeToXmlFile: xmlTextWriterWriteComment fail", BAD_OPERATION);

	if(xmlTextWriterWriteFormatElement(writer, BAD_CAST "ImageFormat", "%d", (int)mImageSource.mPixelFormat) < 0)
		NOVA_EXP("CImage::SerializeToXmlFileImpl: xmlTextWriterWriteElement fail", BAD_OPERATION);
}


template<> CImageManager * CSingelton<CImageManager>::SingeltonObject = NULL;

CImageManager::CImageManager(nstring resourceFactoryName) : CResourceManager(resourceFactoryName)
{

}

CImageManager::~CImageManager()
{
	UnloadAllManagerResources();
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
	const nstring & codec,
	CImageFormats::NovaPixelFormats p,
	CResource::TAttach state)
{
	CImagePtr imgp = CResourceManager::AddNewResource(name, group, state);
	if(imgp.IsNull())
		throw NOVA_EXP("CImageManager::CreateNewImage - resource factory return \
							Null pointer...", MEM_ERROR);

	// Попробуем загрузить изображение
	//imgp->SetParam(file, p);
	CImageBox storage;
	storage.SetData(file, p, codec);
	imgp->SetImageSource(storage);

	nova::nstringstream str;
	str << "Image Factory: image object name: " << name << " group: " << group << " created...";
	LOG_MESSAGE(str.str());

	return imgp;
}

CImagePtr CImageManager::CreateNewImage(const nstring & name,
	const nstring & group,
	const CMemoryBuffer & bits,
	nova::nUInt32 width,
	nova::nUInt32 height,
	nova::nUInt32 depth,
	CImageFormats::NovaPixelFormats p,
	CResource::TAttach state)
{
	CImagePtr imgp = CResourceManager::AddNewResource(name, group, state);
	if(imgp.IsNull())
		throw NOVA_EXP("CImageManager::CreateNewImage - resource factory return \
							Null pointer...", MEM_ERROR);

	// Попробуем загрузить изображение
//	imgp->SetBits(bits, width, height, depth, p);
	CImageBox storage;
	storage.SetData(bits, width, height, depth, p);
	imgp->SetImageSource(storage);

	nova::nstringstream str;
	str << "Image Factory: image object name: " << name << " group: " << group << " created...";
	LOG_MESSAGE(str.str());

	return imgp;
}

CImagePtr CImageManager::CreateNewImage(const nstring & name, const nstring & group,
	const CMemoryBuffer & buffer,
	ESaveFormats compressor,
	const nstring & codec,
	CImageFormats::NovaPixelFormats p,
	CResource::TAttach state)
{
	CImagePtr imgp = CResourceManager::AddNewResource(name, group, state);
	if(imgp.IsNull())
		throw NOVA_EXP("CImageManager::CreateNewImage - resource factory return \
							Null pointer...", MEM_ERROR);

	// Попробуем загрузить изображение
	//imgp->SetParam(bits, compressor, p);
	CImageBox storage;
	storage.SetData(buffer, compressor, p, codec);
	imgp->SetImageSource(storage);

	nova::nstringstream str;
	str << "Image Factory: image object name: " << name << " group: " << group << " created...";
	LOG_MESSAGE(str.str());

	return imgp;
}

CResourcePtr CImageManager::LoadResourceFromXmlNodeImpl(const nstring &name, const nstring &group, xmlNodePtr node)
{
	if(!node)
		return CResourcePtr();

	nstring nfile, ncodec;
	ESaveFormats comp;
	CImageFormats::NovaPixelFormats format = CImageFormats::NF_RGB;
	CFileStream ifile;
	CMemoryBuffer image_buf;


	while(node != NULL)
	{
		if(xmlIsBlankNode(node))
		{
			node = node->next;
			continue;
		}

		if(!xmlStrcmp(node->name, (xmlChar *) "ImageFile"))
			nfile.append(reinterpret_cast<char *>(node->content));
		if(!xmlStrcmp(node->name, (xmlChar *) "ImageCodec"))
			ncodec.append(reinterpret_cast<char *>(node->content));

		if(!xmlStrcmp(node->name, (xmlChar *) "Compressor"))
			comp = (ESaveFormats)CStringUtils::StringToInt(reinterpret_cast<char *>(node->content));
		if(!xmlStrcmp(node->name, (xmlChar *) "ImageFormat"))
			format = (CImageFormats::NovaPixelFormats)CStringUtils::StringToInt(reinterpret_cast<char *>(node->content));

		node = node->next;
	}

// Open image file
	ifile.Open(nfile);
	image_buf.AllocBuffer(ifile.Size());

// Read all file to memory buffer
	ifile.Read(image_buf);
	ifile.Close();

	CResourcePtr image = CreateNewImage(name, group, image_buf, comp, ncodec, format);
	image->LoadResource();

// Dealloc image buffer
	image_buf.FreeBuffer();

	return image;
}

CResourcePtr CImageManager::LoadResourceFromXmlNodeImpl(const nstring &name, const nstring &group, xmlNodePtr node, const CFilesPackage &package)
{
	if(!node)
		return CResourcePtr();

	nstring nfile, ncodec;
	ESaveFormats comp;
	CImageFormats::NovaPixelFormats format = CImageFormats::NF_RGB;
	CMemoryBuffer image_buf;


	while(node != NULL)
	{
		if(xmlIsBlankNode(node))
		{
			node = node->next;
			continue;
		}

		if(!xmlStrcmp(node->name, (xmlChar *) "ImageFile"))
			nfile.append(reinterpret_cast<char *>(node->content));
		if(!xmlStrcmp(node->name, (xmlChar *) "ImageCodec"))
			ncodec.append(reinterpret_cast<char *>(node->content));

		if(!xmlStrcmp(node->name, (xmlChar *) "Compressor"))
			comp = (ESaveFormats)CStringUtils::StringToInt(reinterpret_cast<char *>(node->content));
		if(!xmlStrcmp(node->name, (xmlChar *) "ImageFormat"))
			format = (CImageFormats::NovaPixelFormats)CStringUtils::StringToInt(reinterpret_cast<char *>(node->content));

		node = node->next;
	}

// Copy image file to memory buffer
	image_buf = package.GetFile(nfile);
	if(!image_buf.GetBufferSize())
		throw NOVA_EXP(nstring("CImageManager::LoadResourceFromXmlNodeImpl - Can not find image file ") + nfile + 
			" in package " + package.GetPackageName(), BAD_OPERATION);

	CResourcePtr image = CreateNewImage(name, group, image_buf, comp, ncodec, format);
	image->LoadResource();

// Dealloc image buffer
	image_buf.FreeBuffer();

	return image;
}

}
