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

CMaterial::CMaterial(CResourceManager * rm, nstring & name, nstring & group, TAttach state) :
	CResource(rm, name, group, state)
{

}

nova::CColorRGB CMaterial::GetAmbientColor(void)
{
	return mAmbientColor;
}

nova::CColorRGB CMaterial::GetDiffuseColor(void)
{
	return mDiffuseColor;
}

nova::CColorRGB CMaterial::GetSpecularColor(void)
{
	return mSpecularColor;
}

nReal CMaterial::GetShininess(void)
{
	return mShininess;
}

nReal CMaterial::GetTransparency(void)
{
	return mTransparency;
}

nstring CMaterial::GetTexMap(void)
{
	return mTexMap;
}

nstring CMaterial::GetMultiTexMap(void)
{
	return mMultiTexMap;
}

nstring CMaterial::GetBumpMap(void)
{
	return mBumpMap;
}

nstring CMaterial::GetSpecMap(void)
{
	return mSpecMap;
}

nstring CMaterial::GetReflectionMap(void)
{
	return mReflectionMap;
}


void CMaterial::SetAmbientColor(CColorRGB & nmap)
{
	mAmbientColor = nmap;
}

void CMaterial::SetDiffuseColor(CColorRGB & nmap)
{
	mDiffuseColor = nmap;
}

void CMaterial::SetSpecularColor(CColorRGB & nmap)
{
	mSpecularColor = nmap;
}

void CMaterial::SetShininess(nReal val)
{
	mShininess = val;
}

void CMaterial::SetTransparency(nReal val)
{
	mTransparency = val;
}

void CMaterial::SetTexMap(nstring & nmap)
{
	mTexMap = nmap;
}

void CMaterial::SetMultiTexMap(nstring & nmap)
{
	mMultiTexMap = nmap;
}

void CMaterial::SetSpecMap(nstring & nmap)
{
	mSpecMap = nmap;
}

void CMaterial::SetBumpMap(nstring & nmap)
{
	mBumpMap = nmap;
}

void CMaterial::SetReflectionMap(nstring & nmap)
{
	mReflectionMap = nmap;
}

void CMaterial::FreeResource(void)
{
	CResource::FreeResource();
}

void CMaterial::BuildResource(void)
{
	if(isReady)
		return;

	if(mTexMap.size() != 0)
		mTexMapPtr = CTextureManager::GetSingelton().GetResourceFromHash(mTexMap);
	if(mMultiTexMap.size() != 0)
		mMultiTexMapPtr = CTextureManager::GetSingelton().GetResourceFromHash(mMultiTexMap);
	if(mSpecMap.size() != 0)
		mSpecMapPtr = CTextureManager::GetSingelton().GetResourceFromHash(mSpecMap);
	if(mBumpMap.size() != 0)
		mBumpMapPtr = CTextureManager::GetSingelton().GetResourceFromHash(mBumpMap);
	if(mReflectionMap.size() != 0)
		mReflectionMapPtr = CTextureManager::GetSingelton().GetResourceFromHash(mReflectionMap);

	CResource::BuildResource();
}

void CMaterial::PrepareResource(void)
{
	CResource::PrepareResource();
}

void CMaterial::ApplyMaterial(void)
{
	//glPushAttrib(GL_LIGHTING_BIT);
	glEnable(GL_COLOR_MATERIAL);

	if(mTransparency > 0.0f)
		CRenderSystem::GetSingelton().SetBlend(true);


	if(!mTexMapPtr.IsNull())
	{
		if(mTexMapPtr->GetPixelFormat() == CImageFormats::NF_ALPHA ||
			mTexMapPtr->GetPixelFormat() == CImageFormats::NF_BGRA ||
			mTexMapPtr->GetPixelFormat() == CImageFormats::NF_LUMINANCE_ALPHA ||
			mTexMapPtr->GetPixelFormat() == CImageFormats::NF_RGBA)
			CRenderSystem::GetSingelton().SetBlend(true);

		mTexMapPtr->ApplyTexture();
	}

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glColor4f(mDiffuseColor.R(), mDiffuseColor.G(), mDiffuseColor.B(), mTransparency);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
	glColor3fv(mAmbientColor);
	glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
	glColor3fv(mSpecularColor);

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mAmbientColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mSpecularColor);

	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CMaterialListener * lis =
			dynamic_cast<CMaterialListener *>(GetListener(i));
		lis->ApplyMaterialListener(this);
	}
}

template<> CMaterialManager * CSingelton<CMaterialManager>::SingeltonObject = NULL;

CMaterialManager::CMaterialManager()
{
	LOG_MESSAGE("Material manager started..");
}

CMaterialManager::~CMaterialManager()
{
	UnloadAllManagerResources();
	LOG_MESSAGE("Texture manager destroyed..");
}

CResourcePtr CMaterialManager::CreateInstance(nstring & name,
	nstring & group, CResource::TAttach state)
{
	CResourcePtr ptr(new CMaterial(this, name, group, state));
	return ptr;
}

void CMaterialManager::UnloadAllManagerResources()
{
	UnloadResourceFromHash(this);
}

CMaterialPtr CMaterialManager::CreateMaterial(nstring & name, nstring & group,
	nova::CColorRGB AmbientColor,
	nova::CColorRGB DiffuseColor,
	nova::CColorRGB SpecularColor,
	nReal Shininess,
	nReal Transparency,
	nstring TexMap,
	nstring MultiTexMap,
	nstring SpecMap,
	nstring BumpMap,
	nstring ReflectionMap,
	CResource::TAttach state)
{
	CMaterialPtr mat = CResourceManager::AddNewResource(name, group, state);
	if(mat.IsNull())
		throw NOVA_EXP("CMaterialManager::CreateMaterial - resource factory return \
							Null pointer...", MEM_ERROR);

	mat->SetAmbientColor(AmbientColor);
	mat->SetDiffuseColor(DiffuseColor);
	mat->SetSpecularColor(SpecularColor);
	mat->SetShininess(Shininess);
	mat->SetTransparency(Transparency);
	mat->SetTexMap(TexMap);
	mat->SetMultiTexMap(MultiTexMap);
	mat->SetSpecMap(SpecMap);
	mat->SetBumpMap(BumpMap);
	mat->SetReflectionMap(ReflectionMap);

	mat->PrepareResource();
	mat->BuildResource();


	nova::nstringstream str;
	str << "Material Factory: material name: " << name << " group: " << group << " created..";
	LOG_MESSAGE(str.str());

	return mat;
}


CResourcePtr CMaterialManager::LoadResourceFromXml(const nstring &filename, const CFilesPackage &package)
{
	return CResourcePtr();
}

CResourcePtr CMaterialManager::LoadResourceFromXml(const nstring &filename)
{
	return CResourcePtr();
}

}
