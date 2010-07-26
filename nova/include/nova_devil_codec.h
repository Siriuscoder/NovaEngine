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
#pragma once 

#include "nova_image_codecs.h"

namespace nova
{

#ifdef USING_DEVIL

enum EDevILEffects
{
	DE_ALIENIFYING = 0x19,
	DE_BLURRING,
	DE_CONTRAST,
	DE_EQUALIZATION,
	DE_GAMMA,
	DE_NEGATIVITY,
	DE_NOISE,
	DE_PIXELIZATION,
	DE_SHARPERING
};


typedef struct TDevILEffects
{
	EDevILEffects effect;
	union TVal
	{
		nova::real r;
		nova::uint ui;
	};

	TVal par1;
	TVal par2;
	TVal par3;
} *pTDevILEffects;

class NOVA_EXPORT CDevILFormats : public CImageFormats
{
public:

	CDevILFormats() {}
	~CDevILFormats() {}

	nova::uint GetFormat();
	void SetFormat(nova::uint format);

};

class NOVA_EXPORT CDevILCodec : public CSingelton<CDevILCodec>, public CImageCodec
{
private:

	void ApplyEffect(void);
	TDevILEffects mEffect;

public:

	CDevILCodec();
	~CDevILCodec();

	virtual void Initialize();

	void SetEffect(const TDevILEffects &effect);

	virtual void CodeToBuffer(CMemoryBuffer & out, const CImage &image,
		ESaveFormats ext = SF_BMP);

	virtual void CodeToFile(const nstring & filename, const CImage &image);

	virtual void DecodeFromFile(const nstring & filename, CImage *image,
		CImageFormats::NovaPixelFormats format = CImageFormats::NF_DEFAULT);

	virtual void DecodeFromBuffer(const CMemoryBuffer & input, CImage *image,
		CImageFormats::NovaPixelFormats format = CImageFormats::NF_DEFAULT,
		ESaveFormats ext = SF_BMP);

};

#endif


}