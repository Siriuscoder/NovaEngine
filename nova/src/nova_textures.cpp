п»ї/***************************************************************************
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
#include "nova_stable_precompiled_headers.h"

#include "nova_textures.h"
#include "nova_render_system.h"

namespace nova {


CTexture::CTexture(CResourceManager * rm, const nstring & name, const nstring & group, TAttach state) :
	mEnvType(EV_MODULATE), mTexture(0), mSOffset(0), mTOffset(0), mSTiling(1),
	mTTiling(1), mAngle(0), mMipMap(CTextureSurfaceList::DO_NOT_USE_MIPMAPS),
	mType(CHardwarePixelBuffer::USE_TEXTURE_2D), mWrapTypeS(CLAMP_TO_EDGE), mWrapTypeT(CLAMP_TO_EDGE),
	mMagFilter(FL_MAG_LINEAR), mMinFilter(FL_LINEAR),
	CResource(rm, name, group, state)
{

}

void CTexture::CreateTexture(void)
{
//	if(!stream.GetBeginPtr())
//		NOVA_EXP("CTexture::CreateTexture - imagebuff NULL ptr, can`t create texture!", BAD_OPERATION);
	if(isReady)
		throw NOVA_EXP("CTexture::CreateTexture - resource already created!", BAD_OPERATION);
	mSize = 0;

	TImageList::iterator it;
	int face;

	if(mType == CHardwarePixelBuffer::USE_CUBEMAP_TEXTURE)
	{
		bool use = CRenderSystem::GetSingelton().CheckCapabilities(TEXTURE_CATEGORY, CAP_CUBEMAPPING);
		if(!use)
			throw NOVA_EXP("CTexture::CreateTexture - Your graphic card do not \
								support cubemapping!", BAD_OPERATION);
	}

	if(mType == CHardwarePixelBuffer::USE_CUBEMAP_TEXTURE && mList.size() != 6)
		throw NOVA_EXP("CTexture::CreateTexture - if you using \
			CUBEMAP_TEXTURE, you must send six texture faces!", BAD_OPERATION);

	if(mType != CHardwarePixelBuffer::USE_CUBEMAP_TEXTURE && mList.size() > 1)
		throw NOVA_EXP("CTexture::CreateTexture - if don`t using \
			CUBEMAP_TEXTURE, you must send one texture face!", BAD_OPERATION);

	if(mList.size() == 0)
		throw NOVA_EXP("CTexture::CreateTexture - you must send one \
			or more texture faces!", BAD_OPERATION);
	GLuint cubeid = 0;

	for(it = mList.begin(), face = 0; it != mList.end(); ++it, ++face)
	{
		if((*it).IsNull())
			throw NOVA_EXP("CTexture::CreateTexture - detected null face..", BAD_OPERATION);

		CTextureSurfaceListPtr TexturePlane(new CTextureSurfaceList(
			mType, mMipMap, face, (*it)->GetPixelFormat(), cubeid));

		TexturePlane->BuildSurfaceList((*it)->GetBits(), (*it)->GetWidth(), (*it)->GetHeight(), (*it)->GetDepth());

		for(int i = 0; i < TexturePlane->GetMaxLevels(); ++i)
			mSize += TexturePlane->GetSurface(i)->GetHDSize();

		if(mType == CHardwarePixelBuffer::USE_CUBEMAP_TEXTURE && face == 0)
			cubeid = TexturePlane->GetTargetId();

		mSurfaceList.push_back(TexturePlane);
	}

	mTexture = mSurfaceList[0]->GetTargetId();
}

void CTexture::SetImageList(const TImageList & list)
{
	mList = list;

	for(nova::uint i = 0; i < GetListenersCount(); i++)
	{
		CTextureListener * lis =
			dynamic_cast<CTextureListener *>(GetListener(i));
		lis->CreateTextureListener(this);
	}
}

void CTexture::FreeResource()
{
	CResource::FreeResource();
//	glDeleteTextures(1, &mTexture);
	stl<CTextureSurfaceListPtr>::vector::iterator it;
	for(it = mSurfaceList.begin(); it != mSurfaceList.end(); ++it)
		(*it).Free();

	mSurfaceList.clear();
}

void CTexture::ApplyTexture()
{
	if(!isReady)
		return;

	glMatrixMode(GL_TEXTURE);

	glLoadIdentity();
	glTranslatef(mSOffset, mTOffset, 0.0f);
	glScalef(mSTiling, mTTiling, 1.0f);
	glRotatef(mAngle, 0, 0, 1);

	glEnable(CHardwarePixelBuffer::TextureTarget(mType));
	glBindTexture(CHardwarePixelBuffer::TextureTarget(mType), mTexture);

	glTexParameteri(CHardwarePixelBuffer::TextureTarget(mType), GL_TEXTURE_MIN_FILTER, mMinFilter);
	glTexParameteri(CHardwarePixelBuffer::TextureTarget(mType), GL_TEXTURE_MAG_FILTER, mMagFilter);
	glTexParameteri(CHardwarePixelBuffer::TextureTarget(mType), GL_TEXTURE_WRAP_S, mWrapTypeS);
	glTexParameteri(CHardwarePixelBuffer::TextureTarget(mType), GL_TEXTURE_WRAP_T, mWrapTypeT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mEnvType);

	for(nova::uint i = 0; i < GetListenersCount(); i++)
	{
		CTextureListener * lis =
			dynamic_cast<CTextureListener *>(GetListener(i));
		lis->ApplyTextureListener(this);
	}

}


void CTexture::AddSubTexture(CImagePtr & image, nova::uint level, int face,
	nova::uint xoff, nova::uint yoff, nova::uint zoff)
{
	if(!isReady)
		throw NOVA_EXP("CTexture::CreateTexture - resource not created!", BAD_OPERATION);

	CTextureSurfaceListPtr plane = GetSurfaceList(face);
	CHardwarePixelBufferPtr surface = plane->GetSurface(level);

	CMemoryBuffer lock = surface->Lock(xoff, yoff, zoff,
		image->GetWidth(), image->GetHeight(), image->GetDepth(), HWB_DYNAMIC);

	image->GetBits().CopyTo(lock, image->GetBits().GetBufferSize(), 0);
	surface->Unlock(lock);


	for(nova::uint i = 0; i < GetListenersCount(); i++)
	{
		CTextureListener * lis =
			dynamic_cast<CTextureListener *>(GetListener(i));
		lis->AddSubTextureListener(this);
	}
}

CTextureSurfaceListPtr CTexture::GetSurfaceList(nova::uint face)
{
	if(face >= mSurfaceList.size())
		throw NOVA_EXP("CTexture::GetSurfaceList - this face not exist!", BAD_OPERATION);

	return mSurfaceList[face];
}

CTexture::~CTexture()
{
	nova::nstringstream str;
	str << "CTexture()::~CTexture() " << "name: " << mName << " group: " << mGroup;
	LOG_MESSAGE(str.str());

	FreeResource();
}

CImageFormats::NovaPixelFormats CTexture::GetPixelFormat()
{
	if(mSurfaceList.size() > 0)
		return mSurfaceList[0]->GetPixelFormat();

	return static_cast<CImageFormats::NovaPixelFormats>(0);
}

void CTexture::PreAddingAction()
{
	CResource::PreAddingAction();
}

void CTexture::PostAddindAction()
{
	CResource::PostAddingAction();
}

void CTexture::PreUnloadingAction()
{
	CResource::PreUnloadingAction();
}

void CTexture::PrepareResource(void)
{
	CResource::PrepareResource();
}

void CTexture::BuildResource(void)
{
	if(isReady)
		return;

	CreateTexture( );

	CResource::BuildResource();
}


}




