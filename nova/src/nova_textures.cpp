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
#include "nova_stable_precompiled_headers.h"

#include "nova_textures.h"
#include "nova_render_system.h"

namespace nova {


CTexture::CTexture(CResourceManager * rm, const nstring & name, const nstring & group, TAttach state) :
	mEnvType(EV_MODULATE), mTexture(0), mMipMap(CTextureSurfaceList::DO_NOT_USE_MIPMAPS),
	mType(CHardwarePixelBuffer::USE_TEXTURE_2D), mWrapTypeS(CLAMP_TO_EDGE), mWrapTypeT(CLAMP_TO_EDGE),
	mMagFilter(FL_MAG_LINEAR), mMinFilter(FL_LINEAR),
	CResource(rm, name, group, state)
{
	memset(&mTextureParam, 0, sizeof(TTextureContainer));
	mTextureParam.nUScale = 1;
	mTextureParam.nVScale = 1;
}

void CTexture::SetImageList(const stl<nstring>::vector &list)
{
	mImageList = list;
}

void CTexture::FreeResourceImpl()
{
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

	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CTextureListener * lis =
			dynamic_cast<CTextureListener *>(GetListener(i));
		lis->PreApplyTextureListener(this);
	}

	glMatrixMode(GL_TEXTURE);

	glLoadIdentity();
	glTranslatef(mTextureParam.nUOffset, mTextureParam.nVOffset, 0.0f);
	glScalef(mTextureParam.nUScale, mTextureParam.nVScale, 1.0f);
	glRotatef(mTextureParam.nRotation, 0, 0, 1);

	glEnable(CHardwarePixelBuffer::TextureTarget(mType));
	glBindTexture(CHardwarePixelBuffer::TextureTarget(mType), mTexture);

	//glTexParameteri(CHardwarePixelBuffer::TextureTarget(mType), GL_TEXTURE_MIN_FILTER, mMinFilter);
	//glTexParameteri(CHardwarePixelBuffer::TextureTarget(mType), GL_TEXTURE_MAG_FILTER, mMagFilter);
	glTexParameteri(CHardwarePixelBuffer::TextureTarget(mType), GL_TEXTURE_WRAP_S, mWrapTypeS);
	glTexParameteri(CHardwarePixelBuffer::TextureTarget(mType), GL_TEXTURE_WRAP_T, mWrapTypeT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mEnvType);

	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CTextureListener * lis =
			dynamic_cast<CTextureListener *>(GetListener(i));
		lis->PostApplyTextureListener(this);
	}

}


void CTexture::AddSubTexture(CImagePtr & image, nova::nUInt32 level, nInt32 face,
	nova::nUInt32 xoff, nova::nUInt32 yoff, nova::nUInt32 zoff)
{
	if(!isReady)
		throw NOVA_EXP("CTexture::CreateTexture - resource not created!", BAD_OPERATION);

	CTextureSurfaceListPtr plane = GetSurfaceList(face);
	CHardwarePixelBufferPtr surface = plane->GetSurface(level);

	CMemoryBuffer lock = surface->Lock(xoff, yoff, zoff,
		image->GetWidth(), image->GetHeight(), image->GetDepth(), HWB_DYNAMIC);

	image->GetBits().CopyTo(lock, image->GetBits().GetBufferSize(), 0);
	surface->Unlock(lock);
}

CTextureSurfaceListPtr CTexture::GetSurfaceList(nova::nUInt32 face)
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

void CTexture::LoadResourceImpl(void)
{
	for(nova::nUInt32 i = 0; i < mImageList.size(); i++)
	{
		// Trying to load images
		CResourcePtr resource = CResourceManager::GetResourceFromHash(mImageList[i]);
		if(!resource.IsNull())
			resource->LoadResource();
	}
}

void CTexture::BuildResourceImpl(void)
{
//	if(!stream.GetBeginPtr())
//		NOVA_EXP("CTexture::CreateTexture - imagebuff NULL ptr, can`t create texture!", BAD_OPERATION);
	mSize = 0;

	stl<nstring>::vector::iterator it;
	nInt32 face;

	if(mType == CHardwarePixelBuffer::USE_CUBEMAP_TEXTURE)
	{
		bool use = CRenderSystem::GetSingelton().CheckCapabilities(TEXTURE_CATEGORY, CAP_CUBEMAPPING);
		if(!use)
			throw NOVA_EXP("CTexture::BuildResourceImpl - Your graphic card do not \
								support cubemapping!", BAD_OPERATION);
	}

	if(mType == CHardwarePixelBuffer::USE_CUBEMAP_TEXTURE && mImageList.size() != 6)
		throw NOVA_EXP("CTexture::BuildResourceImpl - if you using \
			CUBEMAP_TEXTURE, you must send six texture faces!", BAD_OPERATION);

	if(mType != CHardwarePixelBuffer::USE_CUBEMAP_TEXTURE && mImageList.size() > 1)
		throw NOVA_EXP("CTexture::BuildResourceImpl - if don`t using \
			CUBEMAP_TEXTURE, you must send one texture face!", BAD_OPERATION);

	if(mImageList.size() == 0)
		throw NOVA_EXP("CTexture::CreateTexture - you must send one \
			or more texture faces!", BAD_OPERATION);
	GLuint cubeid = 0;

	for(it = mImageList.begin(), face = 0; it != mImageList.end(); ++it, ++face)
	{
		CImagePtr image = CResourceManager::GetResourceFromHash(*it);
		if(image.IsNull())
			throw NOVA_EXP(nstring("CTexture::BuildResourceImpl - source image not found: ") + (*it), BAD_OPERATION);

		CTextureSurfaceListPtr TexturePlane(new CTextureSurfaceList(
			mType, mMipMap, face, image->GetPixelFormat(), cubeid));

		TexturePlane->BuildSurfaceList(image->GetBits(), image->GetWidth(), image->GetHeight(), image->GetDepth());

		for(nInt32 i = 0; i < TexturePlane->GetMaxLevels(); ++i)
			mSize += TexturePlane->GetSurface(i)->GetHDSize();

		if(mType == CHardwarePixelBuffer::USE_CUBEMAP_TEXTURE && face == 0)
			cubeid = TexturePlane->GetTargetId();

		mSurfaceList.push_back(TexturePlane);
	}

	mTexture = mSurfaceList[0]->GetTargetId();
	mMagFilter = (TMagFilters)mSurfaceList[0]->GetRecomendedMagFilter();
	mMinFilter = (TMinFilters)mSurfaceList[0]->GetRecomendedMinFilter();
}

void CTexture::SerializeToXmlFileImpl(xmlTextWriterPtr writer)
{
	if(!writer)
		return;

	std::stringstream str;
	str << "Texture target info: "	
		<< "USE_TEXTURE_1D = " << (int)CHardwarePixelBuffer::USE_TEXTURE_1D 
		<< ", USE_TEXTURE_2D = " << (int)CHardwarePixelBuffer::USE_TEXTURE_2D
		<< ", USE_TEXTURE_3D = " << (int)CHardwarePixelBuffer::USE_TEXTURE_3D
		<< ", USE_CUBEMAP_TEXTURE = " << (int)CHardwarePixelBuffer::USE_CUBEMAP_TEXTURE;


	if(xmlTextWriterWriteComment(writer, BAD_CAST str.str().c_str()) < 0)
		NOVA_EXP("CTexture::SerializeToXmlFile: xmlTextWriterWriteComment fail", BAD_OPERATION);

	if(xmlTextWriterWriteElement(writer, BAD_CAST "SourceImage1", BAD_CAST mImageList[0].c_str()) < 0)
		NOVA_EXP("CTexture::SerializeToXmlFileImpl: xmlTextWriterWriteElement fail", BAD_OPERATION);
	if(xmlTextWriterWriteElement(writer, BAD_CAST "SourceImage2", BAD_CAST mImageList[1].c_str()) < 0)
		NOVA_EXP("CTexture::SerializeToXmlFileImpl: xmlTextWriterWriteElement fail", BAD_OPERATION);
	if(xmlTextWriterWriteElement(writer, BAD_CAST "SourceImage3", BAD_CAST mImageList[2].c_str()) < 0)
		NOVA_EXP("CTexture::SerializeToXmlFileImpl: xmlTextWriterWriteElement fail", BAD_OPERATION);
	if(xmlTextWriterWriteElement(writer, BAD_CAST "SourceImage4", BAD_CAST mImageList[3].c_str()) < 0)
		NOVA_EXP("CTexture::SerializeToXmlFileImpl: xmlTextWriterWriteElement fail", BAD_OPERATION);
	if(xmlTextWriterWriteElement(writer, BAD_CAST "SourceImage5", BAD_CAST mImageList[4].c_str()) < 0)
		NOVA_EXP("CTexture::SerializeToXmlFileImpl: xmlTextWriterWriteElement fail", BAD_OPERATION);
	if(xmlTextWriterWriteElement(writer, BAD_CAST "SourceImage6", BAD_CAST mImageList[5].c_str()) < 0)
		NOVA_EXP("CTexture::SerializeToXmlFileImpl: xmlTextWriterWriteElement fail", BAD_OPERATION);


	if(xmlTextWriterWriteFormatElement(writer, BAD_CAST "Percent", "%f", mTextureParam.nPercent) < 0)
		NOVA_EXP("CTexture::SerializeToXmlFileImpl: xmlTextWriterWriteElement fail", BAD_OPERATION);
	if(xmlTextWriterWriteFormatElement(writer, BAD_CAST "Rotation", "%f", mTextureParam.nRotation) < 0)
		NOVA_EXP("CTexture::SerializeToXmlFileImpl: xmlTextWriterWriteElement fail", BAD_OPERATION);
	if(xmlTextWriterWriteFormatElement(writer, BAD_CAST "Blur", "%f", mTextureParam.nBlur) < 0)
		NOVA_EXP("CTexture::SerializeToXmlFileImpl: xmlTextWriterWriteElement fail", BAD_OPERATION);
	if(xmlTextWriterWriteFormatElement(writer, BAD_CAST "UScale", "%f", mTextureParam.nUScale) < 0)
		NOVA_EXP("CTexture::SerializeToXmlFileImpl: xmlTextWriterWriteElement fail", BAD_OPERATION);
	if(xmlTextWriterWriteFormatElement(writer, BAD_CAST "VScale", "%f", mTextureParam.nVScale) < 0)
		NOVA_EXP("CTexture::SerializeToXmlFileImpl: xmlTextWriterWriteElement fail", BAD_OPERATION);
	if(xmlTextWriterWriteFormatElement(writer, BAD_CAST "UOffset", "%f", mTextureParam.nUOffset) < 0)
		NOVA_EXP("CTexture::SerializeToXmlFileImpl: xmlTextWriterWriteElement fail", BAD_OPERATION);
	if(xmlTextWriterWriteFormatElement(writer, BAD_CAST "VOffset", "%f", mTextureParam.nVOffset) < 0)
		NOVA_EXP("CTexture::SerializeToXmlFileImpl: xmlTextWriterWriteElement fail", BAD_OPERATION);
	if(xmlTextWriterWriteFormatElement(writer, BAD_CAST "NoiseSize", "%f", mTextureParam.nNoiseSize) < 0)
		NOVA_EXP("CTexture::SerializeToXmlFileImpl: xmlTextWriterWriteElement fail", BAD_OPERATION);
	if(xmlTextWriterWriteFormatElement(writer, BAD_CAST "NoiseLevel", "%f", mTextureParam.nNoiseLevel) < 0)
		NOVA_EXP("CTexture::SerializeToXmlFileImpl: xmlTextWriterWriteElement fail", BAD_OPERATION);


	str.clear();
	str << "Texture evironment param info: "	
		<< "EV_MODULATE = " << (int)EV_MODULATE 
		<< ", EV_DECAL = " << (int)EV_DECAL
		<< ", EV_BLEND = " << (int)EV_BLEND
		<< ", EV_REPLACE = " << (int)EV_REPLACE;

	if(xmlTextWriterWriteComment(writer, BAD_CAST str.str().c_str()) < 0)
		NOVA_EXP("CTexture::SerializeToXmlFile: xmlTextWriterWriteComment fail", BAD_OPERATION);

	str.clear();
	str << "Texture wrap type info: "	
		<< "CLAMP = " << (int)CLAMP 
		<< ", CLAMP_TO_EDGE = " << (int)CLAMP_TO_EDGE
		<< ", REPEAT = " << (int)REPEAT;

	if(xmlTextWriterWriteComment(writer, BAD_CAST str.str().c_str()) < 0)
		NOVA_EXP("CTexture::SerializeToXmlFile: xmlTextWriterWriteComment fail", BAD_OPERATION);

	if(xmlTextWriterWriteFormatElement(writer, BAD_CAST "WrapS", "%d", (int)mWrapTypeS) < 0)
		NOVA_EXP("CTexture::SerializeToXmlFileImpl: xmlTextWriterWriteElement fail", BAD_OPERATION);
	if(xmlTextWriterWriteFormatElement(writer, BAD_CAST "WrapT", "%d", (int)mWrapTypeT) < 0)
		NOVA_EXP("CTexture::SerializeToXmlFileImpl: xmlTextWriterWriteElement fail", BAD_OPERATION);


	if(xmlTextWriterWriteFormatElement(writer, BAD_CAST "TextureEnvironment", "%d", (int)mEnvType) < 0)
		NOVA_EXP("CTexture::SerializeToXmlFileImpl: xmlTextWriterWriteElement fail", BAD_OPERATION);
}


}




