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
#include "nova_stable_precompiled_headers.h"

#include "nova_formats.h"

#include <IL/il.h>
#include <IL/ilu.h>


namespace nova
{

int CImageFormats::GetInternalChannels()
{
	switch(ex_format)
	{
	case NF_COLOR_INDEX: return 1;
	case NF_RED: return 1;
	case NF_GREEN: return 1;
	case NF_BLUE: return 1;
	case NF_ALPHA: return 1;
	case NF_RGB: return 3;
	case NF_RGBA: return 4;
	case NF_BGR: return 3;
	case NF_BGRA: return 4;
	case NF_LUMINANCE: return 4; 
	case NF_LUMINANCE_ALPHA: return 4;

	default: return 3;
	}

	return 0;
}

nova::uint COpenGLFormats::GetFormat()
{
	switch(ex_format)
	{
	case NF_COLOR_INDEX: return GL_COLOR_INDEX;
	case NF_RED: return GL_RED;
	case NF_GREEN: return GL_GREEN;
	case NF_BLUE: return GL_BLUE;
	case NF_ALPHA: return GL_ALPHA;
	case NF_RGB: return GL_RGB;
	case NF_RGBA: return GL_RGBA;
	case NF_BGR: return GL_BGR;
	case NF_BGRA: return GL_BGRA;
	case NF_LUMINANCE: return GL_LUMINANCE; 
	case NF_LUMINANCE_ALPHA: return GL_LUMINANCE_ALPHA;

	default: 
		{
			ex_format = NF_RGB;
			return GL_RGB;
		}
	}

	return 0;
}

void COpenGLFormats::SetFormat(nova::uint format)
{
	switch(format)
	{
	case GL_COLOR_INDEX: ex_format = NF_COLOR_INDEX; break;
	case GL_RED: ex_format = NF_RED; break;
	case GL_GREEN: ex_format = NF_GREEN; break;
	case GL_BLUE: ex_format = NF_BLUE; break;
	case GL_ALPHA:  ex_format = NF_ALPHA; break;
	case GL_RGB: ex_format = NF_RGB; break;
	case GL_RGBA: ex_format = NF_RGBA; break;
	case GL_BGR: ex_format = NF_BGR; break;
	case GL_BGRA: ex_format = NF_BGRA; break;
	case GL_LUMINANCE: ex_format = NF_LUMINANCE; break; 
	case GL_LUMINANCE_ALPHA: ex_format = NF_LUMINANCE_ALPHA; break; 


	default: ex_format = NF_DEFAULT;
	}
}



}