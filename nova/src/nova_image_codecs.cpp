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

#include "nova_image_codecs.h"
#include "nova_string_utils.h"

namespace nova
{

stl<StringList, CImageCodec*>::map CImageCodec::gCodecs;

CImageCodec::CImageCodec() : CBase("CImageCodec") 
{
	//Initialize();
	mInit = false;
}

CImageCodec::~CImageCodec()
{
	UnregisterCodec(this);
}

void CImageCodec::RegisterCodec(CImageCodec* codec)
{
	if(codec)
	{
		std::pair<StringList, CImageCodec*> codecpair;
		codecpair.first = codec->mExtentions;
		codecpair.second = codec;

		gCodecs.insert(codecpair);
	}
}

void CImageCodec::UnregisterCodec(CImageCodec* codec)
{
	if(codec)
	{
		stl<StringList, CImageCodec*>::map::iterator it;

		if((it = gCodecs.find(codec->mExtentions)) != gCodecs.end())
		{
			gCodecs.erase(it);
		}
	}
}

CImageCodec * CImageCodec::GetCodecForExtention(const nstring & filename)
{
	stl<StringList, CImageCodec*>::map::iterator it;
	it = gCodecs.begin();

	for(; it != gCodecs.end(); ++it)
	{
		for(nova::uint i = 0; i < (*it).first.size(); ++i)
		{
			const char *file = CStringUtils::ToLowerCase(filename).c_str();
			const char *ex = CStringUtils::ToLowerCase((*it).first[i]).c_str();

			if(strstr(file, ex))
			{
				return (*it).second;
			}
		}
	}

	return NULL;
}

CImageCodec * CImageCodec::GetCodecForCompressor(ESaveFormats ext)
{	
	stl<StringList, CImageCodec*>::map::iterator it;
	it = gCodecs.begin();

	for(; it != gCodecs.end(); ++it)
	{
		for(nova::uint i = 0; i < (*it).second->mCompressorList.size(); ++i)
		{
			if((*it).second->mCompressorList[i] == ext)
				return (*it).second;
		}
	}

	return NULL;
}

}