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

#include "nova_content_loader.h"

namespace nova
{

class NOVA_EXPORT C3DSChunk : public CMemoryManaged
{
private:

	nova::nUInt16 mID;
	nova::nUInt32 mStart;
	nova::nUInt32 mEnd;

public:

	C3DSChunk();

	void SetID(nova::nUInt16 id);

	void SetStart(nova::nUInt32 start);

	void SetEnd(nova::nUInt32 end);

	nova::nUInt16 GetID(void) const;

	nova::nUInt32 GetStart(void) const;

	nova::nUInt32 GetEnd(void) const;
};

class NOVA_EXPORT C3DSLoader : public CSceneContentLoaderBase
{
private:

	C3DSChunk ReadChunk();

	bool FindChunk(C3DSChunk & target, C3DSChunk & parent);

	void SkipChunk(const C3DSChunk &chunk);

	void GotoChunk(const C3DSChunk &chunk);

// -- read data methods ---
	CColorRGB ReadColor(const C3DSChunk &chunk);

	float ReadPercentage(const C3DSChunk &chunk);

	CMeshBox::TMeshContainer *LoadSingleMesh(const C3DSChunk &chunk, nstring & obj_name);

	void ReadFaceList(const C3DSChunk &chunk, CMeshBox::TMeshContainer *mesh);

	TMaterialContainer *ReadMaterial(const C3DSChunk &parent);

	TTextureContainer *ReadMap(const C3DSChunk &chunk);

protected:

	void InitLoader(void);

	void CloseLoader(void);

	void LoadImpl(void);

	void LoadMeshList(void);

	void LoadMaterialList(void);

public:

	C3DSLoader();

	~C3DSLoader();
};



}