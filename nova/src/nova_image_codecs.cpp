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

stl<nstring, CImageCodec*>::map CImageCodec::gCodecs;

CImageCodec::CImageCodec() : CBase("CImageCodec") 
{
	//Initialize();
	mInit = false;
}

CImageCodec::~CImageCodec()
{

}

void CImageCodec::RegisterCodec(CImageCodec* codec, const nstring &name)
{
	if(codec)
	{
		std::pair<nstring, CImageCodec*> codecpair;
		codecpair.first = name;
		codecpair.second = codec;

		stl<nstring, CImageCodec*>::map::iterator it;
		if((it = gCodecs.find(name)) != gCodecs.end())
			gCodecs.insert(codecpair);
		else
			throw NOVA_EXP(nstring("CImageCodec::RegisterCodec - codec ") + name + " already registered..", BAD_OPERATION);

		codec->Initialize();
		LOG_MESSAGE(nstring("Codec ") + name + " successfully registered.");
	}
}

void CImageCodec::UnRegisterCodec(const nstring &name)
{
	stl<nstring, CImageCodec*>::map::iterator it;
	if((it = gCodecs.find(name)) != gCodecs.end())
	{
		it->second->Shutdown();
		gCodecs.erase(it);
		LOG_MESSAGE(nstring("Codec ") + name + " successfully unregistered.");
	}
}

CImageCodec * CImageCodec::GetCodec(const nstring & name)
{
	stl<nstring, CImageCodec*>::map::iterator it;
	if((it = gCodecs.find(name)) != gCodecs.end())
	{
		return it->second;
	}

	return NULL;
}

}