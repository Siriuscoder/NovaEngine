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

	virtual void CreateTextureListener(CTexture * object) {}

	virtual void AddSubTextureListener(CTexture * object) {}

	virtual void ApplyTextureListener(CTexture * object) {}
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

protected:

	TEnv mEnvType;
	TWrap mWrapTypeS;
	TWrap mWrapTypeT;
	TMagFilters mMagFilter;
	TMinFilters mMinFilter;

	CHardwarePixelBuffer::TargetType mType;
	GLuint mTexture;
	CTextureSurfaceList::MipMapTagertType mMipMap;

	nReal mSOffset;         //смещение по оси s
	nReal mTOffset;         //смещение по оси t
	nReal mSTiling;         //масштабирование по оси s
	nReal mTTiling;         //масштабирование по оси t
	nReal mAngle;            //угол поворота вокруг оси r (градусов)

	stl<CTextureSurfaceListPtr>::vector mSurfaceList;

	TImageList mList;

public:

	CTexture(CResourceManager * rm, const nstring & name, const nstring & group, TAttach state);

	virtual void FreeResource();

	~CTexture();

	void ApplyTexture();

	inline void SetType(CHardwarePixelBuffer::TargetType type)
	{
		mType = type;
	}

	inline CHardwarePixelBuffer::TargetType GetType()
	{
		return mType;
	}

	inline void SetSOffset(nReal s)
	{
		mSOffset = s;
	}

	inline void SetTOffset(nReal t)
	{
		mTOffset = t;
	}

	inline void SetSTiling(nReal s)
	{
		mSTiling = s;
	}

	inline void SetTTiling(nReal t)
	{
		mTTiling = t;
	}

	inline void SetAngle(nReal a)
	{
		mAngle = a;
	}

	inline void SetEnvType(TEnv a)
	{
		mEnvType = a;
	}

	inline nReal GetSOffset()
	{
		return mSOffset;
	}

	inline nReal GetTOffset()
	{
		return mTOffset;
	}

	inline nReal GetSTiling()
	{
		return mSTiling;
	}

	inline nReal GetTTiling()
	{
		return mTTiling;
	}

	inline nReal GetAngle()
	{
		return mAngle;
	}

	inline void UseMipMaps(CTextureSurfaceList::MipMapTagertType state)
	{
		mMipMap = state;
	}

	inline void SetWrapS(TWrap wrap)
	{
		mWrapTypeS = wrap;
	}

	inline void SetWrapT(TWrap wrap)
	{
		mWrapTypeT = wrap;
	}

	inline TWrap GetWrapS(void)
	{
		return mWrapTypeS;
	}

	inline TWrap GetWrapT(void)
	{
		return mWrapTypeT;
	}

	inline TMagFilters GetMagFilters(void)
	{
		return mMagFilter;
	}

	inline TMinFilters GetMinFilters(void)
	{
		return mMinFilter;
	}

	void SetImageList(const TImageList & list);
	void CreateTexture();

	virtual void BuildResource(void);

	virtual void PrepareResource(void);

	void AddSubTexture(CImagePtr & image, nova::nUInt32 level, int face,
		nova::nUInt32 xoff, nova::nUInt32 yoff = 0, nova::nUInt32 zoff = 0);

	CTextureSurfaceListPtr GetSurfaceList(nova::nUInt32 face);

	CImageFormats::NovaPixelFormats GetPixelFormat();

	virtual void PreAddingAction();
	virtual void PostAddindAction();

	virtual void PreUnloadingAction();
};

typedef CSmartPtr<CTexture> CTexturePtr;

}



