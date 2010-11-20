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

#include "nova_texture_manager.h"
#include "nova_color.h"

namespace nova
{

class CMaterial;

class NOVA_EXPORT CMaterialListener : public CResourceListener
{
public:

	virtual void ApplyMaterialListener(CMaterial * material) {}
};

class NOVA_EXPORT CMaterial : public CResource
{
protected:

	nova::CColorRGB mAmbientColor;
	nova::CColorRGB mDiffuseColor;
	nova::CColorRGB mSpecularColor;
	nReal mShininess;
	nReal mTransparency;
	nstring mTexMap;
	nstring mMultiTexMap;
	nstring mSpecMap;
	nstring mBumpMap;
	nstring mReflectionMap;

	CTexturePtr mTexMapPtr;
	CTexturePtr mMultiTexMapPtr;
	CTexturePtr mSpecMapPtr;
	CTexturePtr mBumpMapPtr;
	CTexturePtr mReflectionMapPtr;

public:

	CMaterial(CResourceManager * rm, nstring & name, nstring & group, TAttach state);

	~CMaterial();

	virtual void FreeResource(void);

	virtual void BuildResource(void);

	virtual void PrepareResource(void);

	nova::CColorRGB GetAmbientColor(void);

	nova::CColorRGB GetDiffuseColor(void);

	nova::CColorRGB GetSpecularColor(void);

	nReal GetShininess(void);

	nReal GetTransparency(void);

	nstring GetTexMap(void);

	nstring GetMultiTexMap(void);

	nstring GetSpecMap(void);

	nstring GetBumpMap(void);

	nstring GetReflectionMap(void);


	void SetAmbientColor(CColorRGB & nmap);

	void SetDiffuseColor(CColorRGB & nmap);
	
	void SetSpecularColor(CColorRGB & nmap);

	void SetShininess(nReal val);

	void SetTransparency(nReal val);

	void SetTexMap(nstring & nmap);

	void SetMultiTexMap(nstring & nmap);

	void SetSpecMap(nstring & nmap);

	void SetBumpMap(nstring & nmap);

	void SetReflectionMap(nstring & nmap);

	void ApplyMaterial(void);
};

typedef CSmartPtr<CMaterial> CMaterialPtr;

class NOVA_EXPORT CMaterialManager : public CSingelton<CMaterialManager>, public CResourceManager
{
protected:

public:
	CMaterialManager();
	~CMaterialManager();

	virtual CResourcePtr CreateInstance(nstring & name, 
		nstring & group, CResource::TAttach state);

	virtual void UnloadAllManagerResources();

	virtual CMaterialPtr CreateMaterial(nstring & name, nstring & group, 
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
		CResource::TAttach state = CResource::NV_ATTACHED);


	/* for force loading form packages */
	CResourcePtr LoadResourceFromXml(const nstring &filename, const CFilesPackage &package);

	CResourcePtr LoadResourceFromXml(const nstring &filename);
};

}