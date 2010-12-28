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

#include "nova_texture_manager.h"

namespace nova
{

template<> CTextureManager * CSingelton<CTextureManager>::SingeltonObject = NULL;

CTextureManager::CTextureManager(nstring resourceFactoryName) : CResourceManager(resourceFactoryName)
{
	mAutoMipmap = CTextureSurfaceList::USE_HARDWARE_MIPMAPS;
}


CTextureManager::~CTextureManager()
{
	UnloadAllManagerResources();
}

CResourcePtr CTextureManager::CreateInstance(const nstring & name,
	const nstring & group, CResource::TAttach state)
{
	CResourcePtr ptr(new CTexture(this, name, group, state));
	return ptr;
}

CTexturePtr CTextureManager::CreateNewTexture
(
	const nstring & name,
	const nstring & group,
	const CImagePtr & image,
	CHardwarePixelBuffer::TargetType type,
	CTextureSurfaceList::MipMapTagertType mip,
	CTexture::TWrap wrapS,
	CTexture::TWrap wrapT,
	CTexture::TEnv env,
	CResource::TAttach state
)
{
	CTexturePtr texp = CResourceManager::AddNewResource(name, group, state);
	if(texp.IsNull())
		throw NOVA_EXP("CTextureManager::CreateNewTexture - resource factory return \
							Null pointer...", MEM_ERROR);

	texp->SetType(type);
	texp->UseMipMaps(mip);
	texp->SetEnvType(env);
	texp->SetWrapS(wrapS);
	texp->SetWrapT(wrapT);

	// Выделяем место в видео памяти под текстуру...
	TImageList templ;
	templ.push_back(image);

	texp->SetImageList(templ);
	//texp->PrepareResource();

	nova::nstringstream str;
	str << "Texture Factory: texture object name: " << name << " group: " << group << " created...";
	LOG_MESSAGE(str.str());

	return texp;
}

CTexturePtr CTextureManager::CreateNewTexturesCube(
	const nstring & name,
	const nstring & group,
	const TImageList & list,
	CTextureSurfaceList::MipMapTagertType mip,
	CTexture::TEnv env,
	CResource::TAttach state)
{
	CTexturePtr texp = CResourceManager::AddNewResource(name, group, state);
	if(texp.IsNull())
		throw NOVA_EXP("CTextureManager::CreateNewTexture - resource factory return \
							Null pointer...", MEM_ERROR);

	texp->SetType(CHardwarePixelBuffer::USE_CUBEMAP_TEXTURE);
	texp->UseMipMaps(mip);
	texp->SetEnvType(env);
	texp->SetWrapS(CTexture::CLAMP);
	texp->SetWrapT(CTexture::CLAMP);

	// Выделяем место в видео памяти под текстуру...

	texp->SetImageList(list);
	//texp->PrepareResource();

	nova::nstringstream str;
	str << "Texture Factory: texture cube object name: " << name << " group: " << group << " created...";
	LOG_MESSAGE(str.str());


	return texp;
}

CResourcePtr CTextureManager::LoadResourceFromXmlNodeImpl(xmlNodePtr node)
{
	return CResourcePtr();
}

CResourcePtr CTextureManager::LoadResourceFromXmlNodeImpl(xmlNodePtr node, const CFilesPackage &package)
{
	return CResourcePtr();
}

//--------------------------------------------------------------------------

void CTextureManager::UnloadAllManagerResources()
{
	UnloadResourceFromHash(this);
}

CTexturePtr CTextureManager::GetThisTexture(const nstring & name)
{
	return GetResourceFromHash(name);
}

CTexturePtr CTextureManager::GetThisTexture(nova::nUInt32 handle)
{
	return GetResourceFromHash(handle);
}

void CTextureManager::SetDefaultAutoMipmap(CTextureSurfaceList::MipMapTagertType type)
{
	mAutoMipmap = type;
}

CTextureSurfaceList::MipMapTagertType CTextureManager::GetDefaultAutoMipmap()
{
	return mAutoMipmap;
}

}
