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

#include "nova_content_loader.h"

namespace nova
{

CSceneContentLoaderBase::CSceneContentLoaderBase() : mpStream(NULL)
{

}

CSceneContentLoaderBase::~CSceneContentLoaderBase()
{
	mpStream = NULL;
}

void CSceneContentLoaderBase::LoadImmediately(CDataStream *stream)
{
	mpStream = stream;
	InitLoader();
	LoadImpl();
	CloseLoader();
}

void CSceneContentLoaderBase::LoadAsync(CDataStream *stream, nInt32 timeout)
{

}

stl<nstring>::vector CSceneContentLoaderBase::GetMeshList(void)
{
	stl<nstring>::vector result;
	nova::stl<nstring, CMeshBox::TMeshContainer>::map::const_iterator it;

	for(it = mMeshesMap.begin(); it != mMeshesMap.end(); it++)
		result.push_back((*it).first);

	return result;
}

stl<nstring>::vector CSceneContentLoaderBase::GetMaterialList(void)
{
	stl<nstring>::vector result;
	nova::stl<nstring, CMaterial::TMaterialContainer>::map::const_iterator it;

	for(it = mMaterialsMap.begin(); it != mMaterialsMap.end(); it++)
		result.push_back((*it).first);

	return result;
}

stl<nstring>::vector CSceneContentLoaderBase::GetTextureList(void)
{
	stl<nstring>::vector result;
	nova::stl<nstring, CTexture::TTextureContainer>::map::const_iterator it;

	for(it = mTexturesMap.begin(); it != mTexturesMap.end(); it++)
		result.push_back((*it).first);

	return result;
}

CMeshBox::TMeshContainer CSceneContentLoaderBase::GetMesh(const nstring & name)
{
	nova::stl<nstring, CMeshBox::TMeshContainer>::map::const_iterator it;
	if((it = mMeshesMap.find(name)) != mMeshesMap.end())
		return (*it).second;

	return CMeshBox::TMeshContainer();
}

CMaterial::TMaterialContainer CSceneContentLoaderBase::GetMaterial(const nstring & name)
{
	nova::stl<nstring, CMaterial::TMaterialContainer>::map::const_iterator it;
	if((it = mMaterialsMap.find(name)) != mMaterialsMap.end())
		return (*it).second;

	return CMaterial::TMaterialContainer();
}

CTexture::TTextureContainer CSceneContentLoaderBase::GetTextureList(const nstring & name)
{
	nova::stl<nstring, CTexture::TTextureContainer>::map::const_iterator it;
	if((it = mTexturesMap.find(name)) != mTexturesMap.end())
		return (*it).second;

	return CTexture::TTextureContainer();
}



}
