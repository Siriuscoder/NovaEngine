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

#include "nova_devil_codec.h"
#include "nova_images.h"

namespace nova
{

#ifdef USING_DEVIL

template<> CDevILCodec * CSingelton<CDevILCodec>::SingeltonObject = NULL;

static void* STDCALL__ DevILAlloc(const ILsizei size)
{
	//return winnie_allocator::Alloc(size);
	nova::nByte *pblock = NULL;
	return getmem<nova::nByte>(pblock, size);
}

static void STDCALL__ DevILFree(const void *p)
{
	//return winnie_allocator::Free(const_cast<void*>(p));
	void *cp = const_cast<void *>(p);
	freemem<nova::nByte>(static_cast<nova::nByte*>(cp));
}

nova::nUInt32 CDevILFormats::GetFormat()
{
	switch(ex_format)
	{
	case NF_COLOR_INDEX: return IL_COLOR_INDEX;
	case NF_RGB: return IL_RGB;
	case NF_RGBA: return IL_RGBA;
	case NF_BGR: return IL_BGR;
	case NF_BGRA: return IL_BGRA;
	case NF_LUMINANCE: return IL_LUMINANCE; 

	default: 
		{
			ex_format = NF_RGB;
			return IL_RGB;
		}
	}	

	return 0;
}


void CDevILFormats::SetFormat(nova::nUInt32 format)
{
	switch(format)
	{
	case IL_COLOR_INDEX: ex_format = NF_COLOR_INDEX; break;
	case IL_RGB: ex_format = NF_RGB; break;
	case IL_RGBA: ex_format = NF_RGBA; break;
	case IL_BGR: ex_format = NF_BGR; break;
	case IL_BGRA: ex_format = NF_BGRA; break;
	case IL_LUMINANCE: ex_format = NF_LUMINANCE; break; 

	default: ex_format = NF_DEFAULT;
	}
}

void CDevILCodec::Initialize()
{
	if(mInit)
		return;

	nstring line("Initializing DevIL codec..\n \
		(Developer's Image Library (DevIL) is a programmer's \n \
		library to develop applications with very powerful image \n \
		loading capabilities, yet is easy for a developer to learn and use.\n\n");

	line.append("Registered image extentions: \n");
	const char *il_version = ilGetString ( IL_VERSION_NUM );
	line += "DevIL version: ";
	line += il_version;
	line += "\n";

	mExtentions.push_back(".bmp");
	mExtentions.push_back(".cut");
	mExtentions.push_back(".dds");
	mExtentions.push_back(".exr");
	mExtentions.push_back(".hdr");
	mExtentions.push_back(".ico");
	mExtentions.push_back(".jpg");
	mExtentions.push_back(".lbm");
	mExtentions.push_back(".mdl");
	mExtentions.push_back(".mng");
	mExtentions.push_back(".pcd");
	mExtentions.push_back(".pcx");
	mExtentions.push_back(".pic");
	mExtentions.push_back(".png");
	mExtentions.push_back(".psd");
	mExtentions.push_back(".tif");
	mExtentions.push_back(".tga");
	mExtentions.push_back(".raw");

	mCompressorList.push_back(SF_BMP);
	mCompressorList.push_back(SF_DDS);
	mCompressorList.push_back(SF_PCX);
	mCompressorList.push_back(SF_TGA);

	for(nova::nUInt32 i = 0; i < mExtentions.size(); ++i)
		line += "\t" + mExtentions[i] + "\n";

	LOG_MESSAGE(line);

	ilSetMemory(DevILAlloc, DevILFree);
	ilInit();
	iluInit();
	ilEnable( IL_FILE_OVERWRITE );

	ILenum Error = 0;
	if((Error = ilGetError()) != NULL)
	{
		nstring str("CDevILCodec::Initialize(): Can not init DevIL - ");
		str.append(iluErrorString(Error));
		throw NOVA_EXP(str.c_str(), BAD_OPERATION);
	}

	mInit = true;
}

void CDevILCodec::Shutdown()
{

}

CDevILCodec::CDevILCodec()
{
	//memset(&mEffect, NULL, sizeof(TDevILEffects));
}

CDevILCodec::~CDevILCodec()
{
	LOG_MESSAGE("CDevILCodec destroyed..");
}

void CDevILCodec::CodeToBuffer(CMemoryBuffer & out, const CImage &image,
							   ESaveFormats ext)
{
	ILuint imageid;
	CDevILFormats informat;
	informat.SetExFormat(image.GetPixelFormat());
	// Generate the main image name to use.
	ilGenImages(1, &imageid);

	// Bind this image name.
	ilBindImage(imageid);

	ilTexImage(image.GetWidth(), image.GetHeight(), image.GetDepth(), informat.GetInternalChannels(),
		informat.GetFormat(), IL_UNSIGNED_BYTE, image.GetBitsPtr());

	ILenum type = 0;
	switch(ext)
	{
	case SF_BMP:
		type = IL_BMP;
		break;
	case SF_ICO:
		type = IL_ICO;
		break;
	case SF_JPG:
		type = IL_JPG;
		break;
	case SF_PCX:
		type = IL_PCX;
		break;
	case SF_PIC:
		type = IL_PIC;
		break;
	case SF_PNG:
		type = IL_PNG;
		break;
	case SF_TGA:
		type = IL_TGA;
		break;
	case SF_TIF:
		type = IL_TIF;
		break;
	case SF_GIF:
		type = IL_GIF;
		break;
	case SF_DDS:
		type = IL_DDS;
		break;
	case SF_PIX:
		type = IL_PIX;
		break;
	case SF_HDR:
		type = IL_HDR;
		break;

	default:
		return;
	}

	out.AllocBuffer(image.GetSize()+0xff);
	ilSaveL(type, out.GetBegin(), out.GetBufferSize());

	ilDeleteImages(1, &imageid);

	ILenum Error = 0;
	if((Error = ilGetError()) != NULL)
	{
		nstring str("CDevILCodec::CodeToStream: ");
		str.append(iluErrorString(Error));
		throw NOVA_EXP(str.c_str(), BAD_OPERATION);
	}
}

void CDevILCodec::CodeToFile(const nstring & filename, const CImage &image)
{
	ILuint imageid;
	CDevILFormats informat;
	informat.SetExFormat(image.GetPixelFormat());
	// Generate the main image name to use.
	ilGenImages(1, &imageid);

	// Bind this image name.
	ilBindImage(imageid);

	ilTexImage(image.GetWidth(), image.GetHeight(), image.GetDepth(), informat.GetInternalChannels(),
		informat.GetFormat(), IL_UNSIGNED_BYTE, image.GetBitsPtr());

	ilSaveImage(filename.c_str());

	ilDeleteImages(1, &imageid);

	ILenum Error = 0;
	if((Error = ilGetError()) != NULL)
	{
		nstring str("CDevILCodec::CodeToFile: ");
		str.append(iluErrorString(Error));
		throw NOVA_EXP(str.c_str(), BAD_OPERATION);
	}
}

void CDevILCodec::DecodeFromFile(const nstring & filename, CImage *image,
		CImageFormats::NovaPixelFormats format)
{
	ILuint image_id;
	// Generate the main image name to use.
	ilGenImages(1, &image_id);

	// Bind this image name.
	ilBindImage(image_id);

	if(!ilLoadImage(filename.c_str()))
	{
		ILenum Error = 0;
		if((Error = ilGetError()) != NULL)
		{
			nstring str("CDevILCodec::DecodeFromFile(): Can not load image file - ");
			str.append(iluErrorString(Error));
			throw NOVA_EXP(str.c_str(), BAD_OPERATION);
		}
	}

	image->GetImageSource().mHeight = ilGetInteger(IL_IMAGE_HEIGHT);
	image->GetImageSource().mWidth = ilGetInteger(IL_IMAGE_WIDTH);
	image->GetImageSource().mDepth = ilGetInteger(IL_IMAGE_DEPTH);

//	this->size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
	if(format == CImageFormats::NF_DEFAULT)
	{
		CDevILFormats inf;
		inf.SetFormat(ilGetInteger(IL_IMAGE_FORMAT));
		format = inf.GetExFormat();
	}

	CDevILFormats informat;
	informat.SetExFormat(format);
	image->GetImageSource().mPixelFormat = format;


	image->GetBits().FreeBuffer();
	image->GetBits().AllocBuffer(image->GetSize());

	ilCopyPixels(0, 0, 0, image->GetWidth(), image->GetHeight(), 
		image->GetDepth(), informat.GetFormat(), IL_UNSIGNED_BYTE, image->GetBits().GetBegin());

	ilDeleteImages(1, &image_id);

	ILenum Error = 0;
	if((Error = ilGetError()) != NULL)
	{
		nstring str("CDevILCodec::DecodeFromFile(): Can not load image file - ");
		str.append(iluErrorString(Error));
		throw NOVA_EXP(str.c_str(), BAD_OPERATION);
	}
}

void CDevILCodec::DecodeFromBuffer(const CMemoryBuffer & input, CImage *image,
		CImageFormats::NovaPixelFormats format, ESaveFormats ext)
{
	ILuint image_id;
	// Generate the main image name to use.
	ilGenImages(1, &image_id);

	// Bind this image name.
	ilBindImage(image_id);

	ILenum type = 0;
	switch(ext)
	{
	case SF_BMP:
		type = IL_BMP;
		break;
	case SF_ICO:
		type = IL_ICO;
		break;
	case SF_JPG:
		type = IL_JPG;
		break;
	case SF_PCX:
		type = IL_PCX;
		break;
	case SF_PIC:
		type = IL_PIC;
		break;
	case SF_PNG:
		type = IL_PNG;
		break;
	case SF_TGA:
		type = IL_TGA;
		break;
	case SF_TIF:
		type = IL_TIF;
		break;
	case SF_GIF:
		type = IL_GIF;
		break;
	case SF_DDS:
		type = IL_DDS;
		break;
	case SF_PIX:
		type = IL_PIX;
		break;
	case SF_HDR:
		type = IL_HDR;
		break;

	default:
		return;
	}

	if(!ilLoadL(type, input.GetBegin(), input.GetBufferSize()))
	{
		ILenum Error = 0;
		if((Error = ilGetError()) != NULL)
		{
			nstring str("CDevILCodec::DecodeFormStream: Can not load image lump - ");
			str.append(iluErrorString(Error));
			throw NOVA_EXP(str.c_str(), BAD_OPERATION);
		}
	}

	image->GetImageSource().mHeight = ilGetInteger(IL_IMAGE_HEIGHT);
	image->GetImageSource().mWidth = ilGetInteger(IL_IMAGE_WIDTH);
	image->GetImageSource().mDepth = ilGetInteger(IL_IMAGE_DEPTH);

//	this->size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
	if(format == CImageFormats::NF_DEFAULT)
	{
		CDevILFormats inf;
		inf.SetFormat(ilGetInteger(IL_IMAGE_FORMAT));
		format = inf.GetExFormat();
	}

	CDevILFormats informat;
	informat.SetExFormat(format);
	image->GetImageSource().mPixelFormat = format;


	image->GetBits().FreeBuffer();
	image->GetBits().AllocBuffer(image->GetSize());

	ilCopyPixels(0, 0, 0, image->GetWidth(), image->GetHeight(), 
		image->GetDepth(), informat.GetFormat(), IL_UNSIGNED_BYTE, image->GetBits().GetBegin());

	ilDeleteImages(1, &image_id);

	ILenum Error = 0;
	if((Error = ilGetError()) != NULL)
	{
		nstring str("CDevILCodec::DecodeFormStream(): Can not load image file - ");
		str.append(iluErrorString(Error));
		throw NOVA_EXP(str.c_str(), BAD_OPERATION);
	}
}

/*void CDevILCodec::LoadImage(nstring & file, CImageFormats::NovaPixelFormats format)
{
	CImageCodec::LockCodec();
	// Generate the main image name to use.
	ilGenImages(1, &image_id);

	// Bind this image name.
	ilBindImage(image_id);

	if(!ilLoadImage(file.c_str()))
		throw NOVA_EXP("CDevILCodec::LoadImage::Can not load image!", BAD_OPERATION);

	COpenGLFormats informat;

	this->height = ilGetInteger(IL_IMAGE_HEIGHT);
	this->width = ilGetInteger(IL_IMAGE_WIDTH);
	this->depth = ilGetInteger(IL_IMAGE_DEPTH);
//	this->size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
	if(format == CImageFormats::NF_DEFAULT)
	{
		CDevILFormats inf;
		inf.SetFormat(ilGetInteger(IL_IMAGE_FORMAT));
		format = inf.GetExFormat();
	}

	informat.SetExFormat(format);
	pixel_format = format;
	this->size = informat.GetInternalChannels() * height * width * depth;

	//pixel_format = ilGetInteger(IL_IMAGE_FORMAT);

	if((Error = ilGetError()) != NULL)
		throw NOVA_EXP(iluErrorString(Error), BAD_OPERATION);
}

CMemoryBuffer CDevILCodec::UnlockBits()
{
	if(!IsLocked())
		return CMemoryBuffer();

	CDevILFormats informat;
	informat.SetExFormat(pixel_format);

	nByte * bits = NULL;
	bits = getmem(bits, size);

	ilBindImage(image_id);
	ilCopyPixels(0, 0, 0, width, height, depth, informat.GetFormat(), IL_UNSIGNED_BYTE, bits);

	nova::nUInt32 actsize = size;
	CImageCodec::UnlockCodec();

	return CMemoryBuffer(bits, actsize);
}

void CDevILCodec::ReleaseCodec()
{
	if(image_id)
		ilDeleteImages(1, &image_id);
	image_id = 0;
	width = 0;
	height = 0;
	depth = 0;
	size  = 0;
	locked = false;
}

*/

#endif

}
