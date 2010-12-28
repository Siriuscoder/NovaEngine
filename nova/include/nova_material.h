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


	void LoadResourceImpl(void);

	void FreeResourceImpl(void);

	void BuildResourceImpl(void); 

public:

	CMaterial(CResourceManager * rm, const nstring & name, const nstring & group, TAttach state);

	~CMaterial();

	nova::CColorRGB GetAmbientColor(void) const ;

	nova::CColorRGB GetDiffuseColor(void) const ;

	nova::CColorRGB GetSpecularColor(void) const ;

	nReal GetShininess(void) const ;

	nReal GetTransparency(void) const ;

	nstring GetTexMap(void) const ;

	nstring GetMultiTexMap(void) const ;

	nstring GetSpecMap(void) const ;

	nstring GetBumpMap(void) const ;

	nstring GetReflectionMap(void) const ;


	void SetAmbientColor(CColorRGB & nmap);

	void SetDiffuseColor(CColorRGB & nmap);
	
	void SetSpecularColor(CColorRGB & nmap);

	void SetShininess(nReal val);

	void SetTransparency(nReal val);

	void SetTexMap(const nstring & nmap);

	void SetMultiTexMap(const nstring & nmap);

	void SetSpecMap(const nstring & nmap);

	void SetBumpMap(const nstring & nmap);

	void SetReflectionMap(const nstring & nmap);

	void ApplyMaterial(void);
};

typedef CSmartPtr<CMaterial> CMaterialPtr;

class NOVA_EXPORT CMaterialManager : public CSingelton<CMaterialManager>, public CResourceManager
{
protected:

	CResourcePtr LoadResourceFromXmlNodeImpl(xmlNodePtr node);

	CResourcePtr LoadResourceFromXmlNodeImpl(xmlNodePtr node, const CFilesPackage &package);

public:
	CMaterialManager(nstring resourceFactoryName);
	~CMaterialManager();

	virtual CResourcePtr CreateInstance(const nstring & name, 
		const nstring & group, CResource::TAttach state);

	virtual void UnloadAllManagerResources();

	virtual CMaterialPtr CreateMaterial(const nstring & name, const nstring & group, 
		nova::CColorRGB AmbientColor,
		nova::CColorRGB DiffuseColor,
		nova::CColorRGB SpecularColor,
		nReal Shininess,
		nReal Transparency,
		const nstring TexMap,
		const nstring MultiTexMap,
		const nstring SpecMap,
		const nstring BumpMap,
		const nstring ReflectionMap,
		CResource::TAttach state = CResource::NV_ATTACHED);



};

}