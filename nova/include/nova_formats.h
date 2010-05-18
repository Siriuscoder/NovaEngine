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

#include "nova_error.h"

namespace nova
{

class NOVA_EXPORT CFormats : public CBase
{
public:

	CFormats() : CBase("CFormats") {}
	~CFormats() {}

};

class NOVA_EXPORT CImageFormats : public CFormats
{
public:

	typedef enum 
	{
		NF_DEFAULT,
		NF_COLOR_INDEX,
		NF_RED,
		NF_GREEN,
		NF_BLUE,
		NF_ALPHA,
		NF_RGB,
		NF_RGBA,
		NF_BGR,
		NF_BGRA,
		NF_LUMINANCE,
		NF_LUMINANCE_ALPHA
	} NovaPixelFormats;

protected:
	NovaPixelFormats ex_format;

public:

	int GetInternalChannels();

	inline NovaPixelFormats GetExFormat()
	{
		return ex_format;
	}

	virtual nova::uint GetFormat() 
	{
		return 0;
	}

	inline void SetExFormat(NovaPixelFormats exformat)
	{
		ex_format = exformat;
	}

	virtual void SetFormat(nova::uint format) {}


	CImageFormats() {}
	~CImageFormats() {}
};

class NOVA_EXPORT COpenGLFormats : public CImageFormats
{
public:

	COpenGLFormats() {}
	~COpenGLFormats() {}

	void SetFormat(nova::uint format);
	nova::uint GetFormat();

};


}