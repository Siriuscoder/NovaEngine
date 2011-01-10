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

#include "nova_material.h"
#include "nova_render_system.h"

namespace nova
{

CMaterial::~CMaterial()
{
	nova::nstringstream str;
	str << "CMaterial::~CMaterial() " << "name: " << mName << " group: " << mGroup;
	LOG_MESSAGE(str.str());

	FreeResource();
}

CMaterial::CMaterial(CResourceManager * rm, const nstring & name, const nstring & group, TAttach state) :
	CResource(rm, name, group, state)
{

}

CMaterial::TMaterialContainer &CMaterial::GetMaterialSource(void)
{
	return mMaterialSource;
}

void CMaterial::SetMaterialSource(const CMaterial::TMaterialContainer &source)
{
	mMaterialSource = source;
}

void CMaterial::FreeResourceImpl(void)
{

}

void CMaterial::BuildResourceImpl(void)
{
	if(mMaterialSource.nDiffuseMap1.size() != 0)
		mDiffuseMap1 = CTextureManager::GetSingelton().GetResourceFromHash(mMaterialSource.nDiffuseMap1);
	if(mMaterialSource.nDiffuseMap2.size() != 0)
		mDiffuseMap2 = CTextureManager::GetSingelton().GetResourceFromHash(mMaterialSource.nDiffuseMap2);
	if(mMaterialSource.nAmbientMap.size() != 0)
		mAmbientMap = CTextureManager::GetSingelton().GetResourceFromHash(mMaterialSource.nAmbientMap);
	if(mMaterialSource.nOpacMap.size() != 0)
		mOpacMap = CTextureManager::GetSingelton().GetResourceFromHash(mMaterialSource.nOpacMap);
	if(mMaterialSource.nSpecMap.size() != 0)
		mSpecMap = CTextureManager::GetSingelton().GetResourceFromHash(mMaterialSource.nSpecMap);
	if(mMaterialSource.nBumpMap.size() != 0)
		mBumpMap = CTextureManager::GetSingelton().GetResourceFromHash(mMaterialSource.nBumpMap);
	if(mMaterialSource.nShinMap.size() != 0)
		mShinMap = CTextureManager::GetSingelton().GetResourceFromHash(mMaterialSource.nShinMap);
	if(mMaterialSource.nSelfIlMap.size() != 0)
		mSelfIlMap = CTextureManager::GetSingelton().GetResourceFromHash(mMaterialSource.nSelfIlMap);
	if(mMaterialSource.nReflectionMap.size() != 0)
		mReflectionMap = CTextureManager::GetSingelton().GetResourceFromHash(mMaterialSource.nReflectionMap);

	if(mMaterialSource.nDiffuseMap1Mask.size() != 0)
		mDiffuseMap1Mask = CTextureManager::GetSingelton().GetResourceFromHash(mMaterialSource.nDiffuseMap1Mask);
	if(mMaterialSource.nDiffuseMap2Mask.size() != 0)
		mDiffuseMap2Mask = CTextureManager::GetSingelton().GetResourceFromHash(mMaterialSource.nDiffuseMap2Mask);
	if(mMaterialSource.nAmbientMapMask.size() != 0)
		mAmbientMapMask = CTextureManager::GetSingelton().GetResourceFromHash(mMaterialSource.nAmbientMapMask);
	if(mMaterialSource.nOpacMapMask.size() != 0)
		mOpacMapMask = CTextureManager::GetSingelton().GetResourceFromHash(mMaterialSource.nOpacMapMask);
	if(mMaterialSource.nSpecMapMask.size() != 0)
		mSpecMapMask = CTextureManager::GetSingelton().GetResourceFromHash(mMaterialSource.nSpecMapMask);
	if(mMaterialSource.nBumpMapMask.size() != 0)
		mBumpMapMask = CTextureManager::GetSingelton().GetResourceFromHash(mMaterialSource.nBumpMapMask);
	if(mMaterialSource.nShinMapMask.size() != 0)
		mShinMapMask = CTextureManager::GetSingelton().GetResourceFromHash(mMaterialSource.nShinMapMask);
	if(mMaterialSource.nSelfIlMapMask.size() != 0)
		mSelfIlMapMask = CTextureManager::GetSingelton().GetResourceFromHash(mMaterialSource.nSelfIlMapMask);
	if(mMaterialSource.nReflectionMapMask.size() != 0)
		mReflectionMapMask = CTextureManager::GetSingelton().GetResourceFromHash(mMaterialSource.nReflectionMapMask);
}

void CMaterial::LoadResourceImpl(void)
{

}

void CMaterial::ApplyMaterial(void)
{
	//glPushAttrib(GL_LIGHTING_BIT);
	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CMaterialListener * lis =
			dynamic_cast<CMaterialListener *>(GetListener(i));
		lis->PreApplyMaterialListener(this);
	}

	glEnable(GL_COLOR_MATERIAL);

	if(mMaterialSource.nTransparency > 0.0f)
		CRenderSystem::GetSingelton().SetBlend(true);


	if(!mDiffuseMap1.IsNull())
	{
		if(mDiffuseMap1->GetPixelFormat() == CImageFormats::NF_ALPHA ||
			mDiffuseMap1->GetPixelFormat() == CImageFormats::NF_BGRA ||
			mDiffuseMap1->GetPixelFormat() == CImageFormats::NF_LUMINANCE_ALPHA ||
			mDiffuseMap1->GetPixelFormat() == CImageFormats::NF_RGBA)
			CRenderSystem::GetSingelton().SetBlend(true);

		mDiffuseMap1->ApplyTexture();
	}

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glColor4f(mMaterialSource.nDiffuseColor.R(), mMaterialSource.nDiffuseColor.G(), mMaterialSource.nDiffuseColor.B(), mMaterialSource.nTransparency);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
	glColor3fv(mMaterialSource.nAmbientColor);
	glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
	glColor3fv(mMaterialSource.nSpecularColor);

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mMaterialSource.nShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mMaterialSource.nAmbientColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mMaterialSource.nSpecularColor);

	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CMaterialListener * lis =
			dynamic_cast<CMaterialListener *>(GetListener(i));
		lis->PostApplyMaterialListener(this);
	}
}

void CMaterial::SerializeToXmlFileImpl(xmlTextWriterPtr writer)
{


}

template<> CMaterialManager * CSingelton<CMaterialManager>::SingeltonObject = NULL;

CMaterialManager::CMaterialManager(const nstring & resourceFactoryName) : CResourceManager(resourceFactoryName)
{

}

CMaterialManager::~CMaterialManager()
{
	UnloadAllManagerResources();
}

CResourcePtr CMaterialManager::CreateInstance(const nstring & name,
	const nstring & group, CResource::TAttach state)
{
	CResourcePtr ptr(new CMaterial(this, name, group, state));
	return ptr;
}

void CMaterialManager::UnloadAllManagerResources()
{
	UnloadResourceFromHash(this);
}

CMaterialPtr CMaterialManager::CreateMaterial(const nstring & name, const nstring & group,
	const CMaterial::TMaterialContainer &params,
	CResource::TAttach state)
{
	CMaterialPtr mat = CResourceManager::AddNewResource(name, group, state);
	if(mat.IsNull())
		throw NOVA_EXP("CMaterialManager::CreateMaterial - resource factory return \
							Null pointer...", MEM_ERROR);

	
	mat->SetMaterialSource(params);

	nova::nstringstream str;
	str << "Material Factory: material name: " << name << " group: " << group << " created..";
	LOG_MESSAGE(str.str());

	return mat;
}


CResourcePtr CMaterialManager::LoadResourceFromXmlNodeImpl(const nstring &name, const nstring &group, xmlNodePtr node)
{
	return CResourcePtr();
}

CResourcePtr CMaterialManager::LoadResourceFromXmlNodeImpl(const nstring &name, const nstring &group, xmlNodePtr node, const CFilesPackage &package)
{
	return CResourcePtr();
}

}
