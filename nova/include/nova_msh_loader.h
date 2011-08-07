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

#include "nova_mesh_box.h"

namespace nova
{

#define MSH_VERTEX_BIT		(0x1)
#define MSH_INDEX_BIT		((0x1) << 1)
#define MSH_MAT_GROUP_BIT	((0x1) << 2)

class NOVA_EXPORT CGlobalMshLoader : public CMemoryManaged
{
private:

#pragma pack(push, 1)
	typedef struct _msh_header
	{
		nova::nUInt32 nSignature;
		nova::nUInt32 nVersion;
		nova::nUInt32 nVersionSgn;

// Data presence bits mask
		nova::nByte nDataMask;
		char nName[256];
	} TMSHHeader;

#pragma pack(pop)

private:

	static CMesh::TMeshContainer gMeshBuffer;

public:

	static CMesh::TMeshContainer &LoadMeshFromFile(const nstring &file);

	static CMesh::TMeshContainer &LoadMeshFromStream(CDataStream *stream);

	static void SaveMeshToStream(const CMesh::TMeshContainer &mesh, CDataStream *stream);

	static void SaveMeshToFile(const CMesh::TMeshContainer &mesh, const nstring &file);
};

}