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

class NOVA_EXPORT C3DSChunk : public CObjectConstructor
{
private:

	nova::nUInt16 mID;
	nova::uint mStart;
	nova::uint mEnd;

public:

	C3DSChunk();

	void SetID(nova::nUInt16 id);

	void SetStart(nova::uint start);

	void SetEnd(nova::uint end);

	nova::nUInt16 GetID(void) const;

	nova::uint GetStart(void) const;

	nova::uint GetEnd(void) const;
};

class NOVA_EXPORT C3DSLoader : public CSceneContentLoaderBase
{
private:

	//std::ifstream m3dsStream;
	nova::CDataStream *mp3dsStream;

	bool mGeometry;

	bool mOwerThread;

	C3DSChunk ReadChunk();

	bool FindChunk(C3DSChunk & target, C3DSChunk & parent);

	void SkipChunk(const C3DSChunk &chunk);

	void GotoChunk(const C3DSChunk &chunk);

// -- read data methods ---
	CColorRGB ReadColor(const C3DSChunk &chunk);

	float ReadPercentage(const C3DSChunk &chunk);

	CMeshBoxPtr LoadSingleMesh(const C3DSChunk &chunk, nstring & obj_name);

	void ReadFaceList(const C3DSChunk &chunk, CMemoryBuffer & indexes, 
		CMeshBox::TFacesInfo & mat_groups, stl<nstring>::vector & mat_names);

	CMaterialPtr ReadMaterial(const C3DSChunk &parent);

	CTexturePtr ReadMap(const C3DSChunk &chunk);

public:

	C3DSLoader(CDataStream & stream, bool geom, bool mult_thread);

	~C3DSLoader();

	stl<CMeshBoxPtr>::vector LoadMeshList(void);

	stl<CMaterialPtr>::vector LoadMaterialList(void);
};



}