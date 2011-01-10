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

	virtual void PreApplyMaterialListener(CMaterial * material) {}

	virtual void PostApplyMaterialListener(CMaterial * material) {}
};

class NOVA_EXPORT CMaterial : public CResource
{
public:

	typedef struct _MaterialContainer
	{
		nstring nName;
		nInt32 nID;
		nova::CColorRGB nAmbientColor;
		nova::CColorRGB nDiffuseColor;
		nova::CColorRGB nSpecularColor;
		nReal nShininess;
		nReal nShinStrength;
		nReal nTransparency;
		nReal nFalloff;
		nReal nSelfIllum;
		nReal nBlur;
		nReal nShading;

		bool nSelfIllumFlag;
		bool nTwoSided;
		bool nMapDecal;
		bool nIsAdditive;
		bool nSoften;
/* Texture Maps */
		nstring nDiffuseMap1;
		nstring nDiffuseMap2;
		nstring nAmbientMap;
		nstring nOpacMap;
		nstring nSpecMap;
		nstring nBumpMap;
		nstring nShinMap;
		nstring nSelfIlMap;
		nstring nReflectionMap;
/* Mask Maps */
		nstring nDiffuseMap1Mask;
		nstring nDiffuseMap2Mask;
		nstring nAmbientMapMask;
		nstring nOpacMapMask;
		nstring nSpecMapMask;
		nstring nBumpMapMask;
		nstring nShinMapMask;
		nstring nSelfIlMapMask;
		nstring nReflectionMapMask;
/* Acubic opt */
		nInt32 nAutoreflMapAntiAlias;
		nova::nUInt32 nAutoreflMapFlags;
        nInt32 nAutoreflMapSize;
        nInt32 nAutoreflMapFrameStep;

		nova::stl<nstring>::vector nSubMats;

	} TMaterialContainer;

protected:

	TMaterialContainer mMaterialSource;

	CTexturePtr mDiffuseMap1;
	CTexturePtr mDiffuseMap2;
	CTexturePtr mAmbientMap;
	CTexturePtr mOpacMap;
	CTexturePtr mSpecMap;
	CTexturePtr mBumpMap;
	CTexturePtr mShinMap;
	CTexturePtr mSelfIlMap;
	CTexturePtr mReflectionMap;
	CTexturePtr mDiffuseMap1Mask;
	CTexturePtr mDiffuseMap2Mask;
	CTexturePtr mAmbientMapMask;
	CTexturePtr mOpacMapMask;
	CTexturePtr mSpecMapMask;
	CTexturePtr mBumpMapMask;
	CTexturePtr mShinMapMask;
	CTexturePtr mSelfIlMapMask;
	CTexturePtr mReflectionMapMask; 

	void LoadResourceImpl(void);

	void FreeResourceImpl(void);

	void BuildResourceImpl(void); 

	void SerializeToXmlFileImpl(xmlTextWriterPtr writer);

public:

	CMaterial(CResourceManager * rm, const nstring & name, const nstring & group, TAttach state);

	~CMaterial();

	TMaterialContainer &GetMaterialSource(void);

	void SetMaterialSource(const TMaterialContainer &source); 

	void ApplyMaterial(void);
};

typedef CSmartPtr<CMaterial> CMaterialPtr;

class NOVA_EXPORT CMaterialManager : public CSingelton<CMaterialManager>, public CResourceManager
{
protected:

	CResourcePtr LoadResourceFromXmlNodeImpl(const nstring &name, const nstring &group, xmlNodePtr node);

	CResourcePtr LoadResourceFromXmlNodeImpl(const nstring &name, const nstring &group, xmlNodePtr node, const CFilesPackage &package);

public:
	CMaterialManager(const nstring & resourceFactoryName);
	~CMaterialManager();

	virtual CResourcePtr CreateInstance(const nstring & name, 
		const nstring & group, CResource::TAttach state);

	virtual void UnloadAllManagerResources();

	virtual CMaterialPtr CreateMaterial(const nstring & name, const nstring & group, 
		const CMaterial::TMaterialContainer &params,
		CResource::TAttach state = CResource::NV_ATTACHED);



};

}