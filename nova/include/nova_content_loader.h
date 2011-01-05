﻿/***************************************************************************
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

#include "nova_mesh_box.h"
#include "nova_gzstreams.h"
#include "nova_color.h"
#include "nova_material.h"

namespace nova
{

class NOVA_EXPORT CSceneContentLoaderBase : public CMemoryManaged
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

	virtual void InitLoader(void) = 0;

	virtual void CloseLoader(void) = 0;

	virtual void LoadImpl(void) = 0;

public:

	CSceneContentLoaderBase();
	virtual ~CSceneContentLoaderBase();

	void LoadImmediately(CDataStream *stream);

	void LoadAsync(CDataStream *stream, nInt32 timeout);

	stl<nstring>::vector GetMeshList(void);

	stl<nstring>::vector GetMaterialList(void);

	stl<nstring>::vector GetTextureList(void);

	CMeshBox::TMeshContainer GetMesh(const nstring &name);

	TMaterialContainer GetMaterial(const nstring & name);

	CTexture::TTextureContainer GetTextureList(const nstring & name);

protected:

	nova::stl<nstring, CMeshBox::TMeshContainer>::map mMeshesMap;
	nova::stl<nstring, TMaterialContainer>::map mMaterialsMap;
	nova::stl<nstring, CTexture::TTextureContainer>::map mTexturesMap;
	nova::CDataStream *mpStream;
};

class NOVA_EXPORT CMeshLoaderFactory : public CMemoryManaged
{
public:
	
	CMeshLoaderFactory() {}
	~CMeshLoaderFactory() {}

	virtual CSceneContentLoaderBase *CreateMeshFactory(CDataStream & stream, bool geom, bool mult_thread) = 0;
};

}
