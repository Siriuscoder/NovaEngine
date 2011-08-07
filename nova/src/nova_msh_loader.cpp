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

#include "nova_msh_loader.h"

#define MSH_MAIN_SIGNATURE			0x000005AF
#define MSH_VERSION(major,minor)	(nova::nUInt32)(((nova::nUInt16)major & 0xffff) | ((((nova::nUInt32)minor) & 0xffff) << 16))
#define MSH_VERSION_1_0				MSH_VERSION(1, 0)
#define MSH_VERSION_1_1				MSH_VERSION(1, 1)
#define MSH_VERSION_SIGN(x)			(nova::nUInt32)((nova::nUInt32)x | MSH_MAIN_SIGNATURE)

namespace nova
{

CMesh::TMeshContainer CGlobalMshLoader::gMeshBuffer;

CMesh::TMeshContainer &CGlobalMshLoader::LoadMeshFromFile(const nstring &file)
{
	CFileStream sfile;
	
	sfile.Open(file);
	LoadMeshFromStream(&sfile);

	sfile.Close();

	return gMeshBuffer;
}

CMesh::TMeshContainer &CGlobalMshLoader::LoadMeshFromStream(CDataStream *stream)
{
	TMSHHeader header;
	memset(&header, 0, sizeof(TMSHHeader));

	if(!stream)
		throw NOVA_EXP("CGlobalMshLoader::LoadMeshFromStream: stream is null ptr ", MEM_ERROR);

	stream->Read(&header, sizeof(TMSHHeader));
	if(header.nSignature != MSH_MAIN_SIGNATURE)
		throw NOVA_EXP("CGlobalMshLoader::LoadMeshFromStream: disparity of signatures, unknown format ", BAD_OPERATION);

	gMeshBuffer.nName.append(header.nName);
	if(header.nVersion == MSH_VERSION_1_1)
	{
		if(header.nVersionSgn != MSH_VERSION_SIGN(MSH_VERSION_1_1))
			throw NOVA_EXP("CGlobalMshLoader::LoadMeshFromStream: disparity of version signatures, unknown format version ", BAD_OPERATION);

		size_t size;
		if((header.nDataMask & MSH_VERTEX_BIT) == MSH_VERTEX_BIT)
		{
			stream->Read(&size, sizeof(size_t));
			gMeshBuffer.nVertexList.resize(size);
			stream->Read((void *)&(gMeshBuffer.nVertexList[0]), size * sizeof(CMesh::stVertex3d3n3uv_t));
		}

		if((header.nDataMask & MSH_INDEX_BIT) == MSH_INDEX_BIT)
		{
			stream->Read(&size, sizeof(size_t));
			gMeshBuffer.nMeshInfoList.resize(size);

			for(size_t i = 0; i < size; i++)
			{
				stream->Read((void *)&(gMeshBuffer.nMeshInfoList[i].faceIndex), size * sizeof(CMesh::TFaceABC));
				stream->Read((void *)&(gMeshBuffer.nMeshInfoList[i].matSubID), sizeof(nova::nUInt32));
				gMeshBuffer.nMeshInfoList[i].faceNo = i;
			}
		}
	}
	else
		throw NOVA_EXP("CGlobalMshLoader::LoadMeshFromStream: unknown format version.. ", BAD_OPERATION);

	return gMeshBuffer;
}

void CGlobalMshLoader::SaveMeshToStream(const CMesh::TMeshContainer &mesh, CDataStream *stream)
{
	TMSHHeader header;
	memset(&header, 0, sizeof(TMSHHeader));

	header.nSignature = MSH_MAIN_SIGNATURE;
	header.nVersion = MSH_VERSION_1_1;
	header.nVersionSgn = MSH_VERSION_SIGN(MSH_VERSION_1_1);

	header.nDataMask |= mesh.nVertexList.size() ? MSH_VERTEX_BIT : 0;
	header.nDataMask |= mesh.nMeshInfoList.size() ? MSH_INDEX_BIT : 0;
	strcpy(header.nName, mesh.nName.c_str());

	if(!stream)
		throw NOVA_EXP("CGlobalMshLoader::SaveMeshToStream: stream is null ptr ", MEM_ERROR);

	stream->Write(&header, sizeof(TMSHHeader));

	size_t block;
	if(mesh.nVertexList.size())
	{
		block = mesh.nVertexList.size();
		stream->Write(&block, sizeof(size_t));
		stream->Write((void *)&(mesh.nVertexList[0]), block * sizeof(CMesh::stVertex3d3n3uv_t));
	}

	if(mesh.nMeshInfoList.size())
	{
		block = mesh.nMeshInfoList.size();
		stream->Write(&block, sizeof(size_t));

		for(size_t i = 0; i < block; i++)
		{
			stream->Write((void *)&(mesh.nMeshInfoList[i].faceIndex), block * sizeof(CMesh::TFaceABC));
			stream->Write((void *)&(mesh.nMeshInfoList[i].matSubID), sizeof(nova::nUInt32));
		}
	}
}

void CGlobalMshLoader::SaveMeshToFile(const CMesh::TMeshContainer &mesh, const nstring &file)
{
	CFileStream sfile;

	sfile.Open(file, true, false);
	SaveMeshToStream(mesh, &sfile);

	sfile.Close();
}

}