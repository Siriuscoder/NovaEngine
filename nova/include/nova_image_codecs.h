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

#include "nova_streams.h"
#include "nova_formats.h"
#include "nova_singelton.h"

namespace nova 
{

typedef stl<nstring>::vector StringList; 

enum ESaveFormats
{
SF_BMP = 0xCC,
SF_ICO,
SF_JPG,
SF_PCX,
SF_PIC,
SF_PNG,
SF_TGA,
SF_TIF,
SF_GIF,
SF_DDS,
SF_PIX,
SF_HDR
};
	


class NOVA_EXPORT CImage;


class NOVA_EXPORT CImageCodec : public CBase
{
protected:

	static stl<StringList, CImageCodec*>::map gCodecs;
	StringList mExtentions;
	stl<ESaveFormats>::vector mCompressorList;

	bool mInit;

public:

	static void RegisterCodec(CImageCodec* codec);

	static void UnregisterCodec(CImageCodec* codec);

	static CImageCodec * GetCodecForExtention(const nstring & filename);

	static CImageCodec * GetCodecForCompressor(ESaveFormats ext);

	CImageCodec();

	~CImageCodec();

	virtual void Initialize() = 0;

	virtual void CodeToBuffer(CMemoryBuffer & out, const CImage &image,
		ESaveFormats ext = SF_BMP) = 0;

	virtual void CodeToFile(const nstring & filename, const CImage &image) = 0;

	virtual void DecodeFromFile(const nstring & filename, CImage *image,
		CImageFormats::NovaPixelFormats format = CImageFormats::NF_DEFAULT) = 0;

	virtual void DecodeFromBuffer(const CMemoryBuffer & input, CImage *image,
		CImageFormats::NovaPixelFormats format = CImageFormats::NF_DEFAULT,
		ESaveFormats ext = SF_BMP) = 0;
};

}
