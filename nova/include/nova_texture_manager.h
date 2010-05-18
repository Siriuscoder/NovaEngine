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

#include "nova_textures.h"
#include "nova_render_system.h"

namespace nova
{

class NOVA_EXPORT CTextureManager : public CSingelton<CTextureManager>, public CResourceManager
{
protected:

	nova::uint blending_type_sfactor;
	nova::uint blending_type_dfactor;
	int mAnisotropyLevel;
	CTextureSurfaceList::MipMapTagertType mAutoMipmap;

public:

	CTextureManager();
	~CTextureManager();

	virtual CResourcePtr CreateInstance(const nstring & name,
		const nstring & group, CResource::TAttach state);

	void SetDefaultAutoMipmap(CTextureSurfaceList::MipMapTagertType type);

	CTextureSurfaceList::MipMapTagertType GetDefaultAutoMipmap();

	CTexturePtr GetThisTexture(const nstring & name);

	CTexturePtr GetThisTexture(nova::uint handle);

	virtual CTexturePtr CreateNewTexture(const nstring & name, const nstring & group, const CImagePtr & image,
		CHardwarePixelBuffer::TargetType type = CHardwarePixelBuffer::USE_TEXTURE_2D,
		CTextureSurfaceList::MipMapTagertType mip = CTextureSurfaceList::DO_NOT_USE_MIPMAPS,
		CTexture::TWrap wrapS = CTexture::CLAMP_TO_EDGE, CTexture::TWrap wrapT = CTexture::CLAMP_TO_EDGE,
		CTexture::TEnv env = CTexture::EV_MODULATE, CTexture::TMagFilters magf = CTexture::FL_MAG_LINEAR,
		CTexture::TMinFilters minf = CTexture::FL_LINEAR, CResource::TAttach state = CResource::NV_ATTACHED);

	virtual CTexturePtr CreateNewTexturesCube(const nstring & name, const nstring & group, const TImageList & list,
		CTextureSurfaceList::MipMapTagertType mip = CTextureSurfaceList::DO_NOT_USE_MIPMAPS,
		CTexture::TEnv env = CTexture::EV_REPLACE,
		CTexture::TMagFilters magf = CTexture::FL_MAG_LINEAR,
		CTexture::TMinFilters minf = CTexture::FL_LINEAR,
		CResource::TAttach state = CResource::NV_ATTACHED);

	virtual CTexturePtr CreateNewTextureAsync(const nstring & name, const nstring & group, const CImagePtr & image,
		CHardwarePixelBuffer::TargetType type = CHardwarePixelBuffer::USE_TEXTURE_2D,
		CTextureSurfaceList::MipMapTagertType mip = CTextureSurfaceList::DO_NOT_USE_MIPMAPS,
		CTexture::TWrap wrapS = CTexture::CLAMP_TO_EDGE, CTexture::TWrap wrapT = CTexture::CLAMP_TO_EDGE,
		CTexture::TEnv env = CTexture::EV_MODULATE, CTexture::TMagFilters magf = CTexture::FL_MAG_LINEAR,
		CTexture::TMinFilters minf = CTexture::FL_LINEAR, CResource::TAttach state = CResource::NV_ATTACHED);

	virtual CTexturePtr CreateNewTexturesCubeAsync(const nstring & name, const nstring & group, const TImageList & list,
		CTextureSurfaceList::MipMapTagertType mip = CTextureSurfaceList::DO_NOT_USE_MIPMAPS,
		CTexture::TEnv env = CTexture::EV_REPLACE,
		CTexture::TMagFilters magf = CTexture::FL_MAG_LINEAR,
		CTexture::TMinFilters minf = CTexture::FL_LINEAR,
		CResource::TAttach state = CResource::NV_ATTACHED);

	virtual void UnloadAllManagerResources();
};



}
