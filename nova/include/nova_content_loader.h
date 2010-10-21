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

class NOVA_EXPORT CSceneContentLoaderBase : public CObjectConstructor
{
public:

	typedef struct _MatGroupInfo
	{
		nstring nMatName;
		int nFace;
	} TMatGroupInfo;

	typedef nova::stl<TMatGroupInfo>::vector TMatGroups;

	typedef struct _MeshContainer
	{
		nstring nName;
		nova::CColorRGB mMeshColor;
		CMeshBox::TVertexes nVertexList;
		CMeshBox::TTexCoords nMappingFacesList;
		CMeshBox::TIndexes nIndexList;
		TMatGroups nMatGroupsList;
	} TMeshContainer;

	typedef struct _MaterialContainer
	{
		nstring nName;
		nova::CColorRGB nAmbientColor;
		nova::CColorRGB nDiffuseColor;
		nova::CColorRGB nSpecularColor;
		real nShininess;
		real nShinStrength;
		real nTransparency;
		real nFalloff;
		real nSelfIllum;
		real nBlur;
		real nShading;

		bool nSelfIllumFlag;
		bool nTwoSided;
		bool nMapDecal;
		bool nIsAdditive;
		bool nSoften;
/* Texture Maps */
		nstring nTexMap1;
		nstring nTexMap2;
		nstring nOpacMap;
		nstring nSpecMap;
		nstring nBumpMap;
		nstring nShinMap;
		nstring nSelfIlMap;
		nstring nReflectionMap;
/* Mask Maps */
		nstring nTexMap1Mask;
		nstring nTexMap2Mask;
		nstring nOpacMapMask;
		nstring nSpecMapMask;
		nstring nBumpMapMask;
		nstring nShinMapMask;
		nstring nSelfIlMapMask;
		nstring nReflectionMapMask;
/* Acubic opt */
		int nAutoreflMapAntiAlias;
		nova::uint nAutoreflMapFlags;
        int nAutoreflMapSize;
        int nAutoreflMapFrameStep;

	} TMaterialContainer;

	typedef struct _TextureContainer
	{
		nstring nName;
		nstring nBitMap;
		real nPercent;
		real nRotation;
		real nBlur;
		real nUScale;
		real nVScale;
		real nUOffset;
		real nVOffset;
		real nNoiseSize;
		real nNoiseLevel;

	} TTextureContainer;

protected:

	virtual void InitLoader(void) = 0;

	virtual void CloseLoader(void) = 0;

	virtual void LoadImpl(void) = 0;

public:

	CSceneContentLoaderBase();
	virtual ~CSceneContentLoaderBase();

	void LoadImmediately(void);

	void LoadAsync(int timeout);

	stl<string>::vector GetMeshList(void);

	stl<string>::vector GetMaterialList(void);

	stl<string>::vector GetTextureList(void);

protected:

	nova::stl<nstring, TMeshContainer>::map mMeshesMap;
	nova::stl<nstring, TMaterialContainer>::map mMaterialsMap;
	nova::stl<nstring, TTextureContainer>::map mTexturesMap;
};

class NOVA_EXPORT CMeshLoaderFactory : public CObjectConstructor
{
public:
	
	CMeshLoaderFactory() {}
	~CMeshLoaderFactory() {}

	virtual CSceneContentLoaderBase *CreateMeshFactory(CDataStream & stream, bool geom, bool mult_thread) = 0;
};

}
