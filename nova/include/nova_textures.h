/***************************************************************************
 *   Copyright (C) 2009 by SIRIUS										   *
 *   SiriusStarNick@yandex.ru											   *
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
#pragma once

#include "nova_images.h"
#include "nova_hardware_pixel_buffer.h"

namespace nova {

class CTexture;

class NOVA_EXPORT CTextureListener : public CResourceListener
{
public:

	virtual void PreApplyTextureListener(CTexture * object) {}

	virtual void PostApplyTextureListener(CTexture * object) {}
};

typedef stl<CImagePtr>::vector TImageList;

class NOVA_EXPORT CTexture : public CResource
{
public:

	enum TMinFilters
	{
		FL_NEAREST = GL_NEAREST,
		FL_LINEAR = GL_LINEAR,
		FL_NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
		FL_NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
		FL_LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
		FL_LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
//		FL_FILTER4_SGIS
	};

	enum TMagFilters
	{
		FL_MAG_NEAREST = GL_NEAREST,
		FL_MAG_LINEAR = GL_LINEAR
/*
		FL_LINEAR_DETAIL_SGIS,
		FL_LINEAR_DETAIL_ALPHA_SGIS,
		FL_LINEAR_DETAIL_COLOR_SGIS,
		FL_LINEAR_SHARPEN_SGIS,
		FL_LINEAR_SHARPEN_ALPHA_SGIS,
		FL_LINEAR_SHARPEN_COLOR_SGIS
*/
	};

	enum TWrap
	{
		CLAMP = GL_CLAMP,
		CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
		REPEAT = GL_REPEAT
	};

	enum TEnv
	{
		EV_MODULATE = GL_MODULATE,
		EV_DECAL = GL_DECAL,
		EV_BLEND = GL_BLEND,
		EV_REPLACE = GL_REPLACE
	};

	typedef struct _TextureContainer
	{
		nstring nName;
		nstring nBitMap;
		nReal nPercent;
		nReal nRotation;
		nReal nBlur;
		nReal nUScale;
		nReal nVScale;
		nReal nUOffset;
		nReal nVOffset;
		nReal nNoiseSize;
		nReal nNoiseLevel;

	} TTextureContainer;

protected:

	TEnv mEnvType;
	TWrap mWrapTypeS;
	TWrap mWrapTypeT;
	TMagFilters mMagFilter;
	TMinFilters mMinFilter;
	CHardwarePixelBuffer::TargetType mType;
	GLuint mTexture;
	CTextureSurfaceList::MipMapTagertType mMipMap;
	TTextureContainer mTextureParam;
	stl<CTextureSurfaceListPtr>::vector mSurfaceList;
	//TImageList mList;
	stl<nstring>::vector mImageList;


	void LoadResourceImpl(void);

	void FreeResourceImpl(void);

	void BuildResourceImpl(void);

public:

	CTexture(CResourceManager * rm, const nstring & name, const nstring & group, TAttach state);

	~CTexture();

	inline TTextureContainer & GetTextureParams(void)
	{
		return mTextureParam;
	}

	inline void SetTextureParams(const TTextureContainer &param)
	{
		mTextureParam = param;
	}

	void ApplyTexture();

	inline void SetType(CHardwarePixelBuffer::TargetType type)
	{
		mType = type;
	}

	inline CHardwarePixelBuffer::TargetType GetType() const
	{
		return mType;
	}

	inline void SetEnvType(TEnv a)
	{
		mEnvType = a;
	}

	inline void UseMipMaps(CTextureSurfaceList::MipMapTagertType state)
	{
		mMipMap = state;
	}

	inline TMagFilters GetMagFilters(void) const
	{
		return mMagFilter;
	}

	inline TMinFilters GetMinFilters(void) const
	{
		return mMinFilter;
	}

	inline TWrap & WrapS(void)
	{
		return mWrapTypeS;
	}

	inline TWrap & WrapT(void)
	{
		return mWrapTypeT;
	} 

	void SetImageList(const stl<nstring>::vector &list);

	void AddSubTexture(CImagePtr & image, nova::nUInt32 level, nInt32 face,
		nova::nUInt32 xoff, nova::nUInt32 yoff = 0, nova::nUInt32 zoff = 0);

	CTextureSurfaceListPtr GetSurfaceList(nova::nUInt32 face);

	CImageFormats::NovaPixelFormats GetPixelFormat();
};

typedef CSmartPtr<CTexture> CTexturePtr;

}



