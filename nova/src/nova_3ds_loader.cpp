/***************************************************************************
 *   Copyright (C) 2009 by Sirius										   *
 *	 Vdov Nikita Sergeevich	(c)		ƀ									   *
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

#include "nova_3ds_loader.h"
#include "nova_images.h"
#include "nova_formats.h"
#include "nova_string_utils.h"

namespace nova
{
/*
	I am change this file

*/

enum T3DS_Generic_staff
{

T3DS_SEEK_START				=1900,
T3DS_SEEK_CURSOR			=1901,

// common chunks
// colors
T3DS_COLOR_F				=0x0010,
T3DS_COLOR_24				=0x0011,
T3DS_LIN_COLOR_24			=0x0012,
T3DS_LIN_COLOR_F			=0x0013,

// percentage
T3DS_INT_PERCENTAGE			=0x0030,
T3DS_FLOAT_PERCENTAGE		=0x0031,

// ambient light
T3DS_AMBIENT_LIGHT			=0x2100,

T3DS_MAIN3DS				=0x4D4D,
T3DS_EDIT3DS				=0x3D3D,	// this is the start of the editor config

// keyframer chunk ids
T3DS_KFDATA					=0xB000,	// the keyframer section
T3DS_KFHDR					=0xB00A,
T3DS_OBJECT_NODE_TAG		=0xB002,
T3DS_NODE_HDR				=0xB010,
T3DS_PIVOT					=0xB013,
T3DS_POS_TRACK_TAG			=0xB020,
T3DS_ROT_TRACK_TAG			=0xB021,
T3DS_SCL_TRACK_TAG			=0xB022,

// material entries
T3DS_MAT_ENTRY				=0xAFFF,
T3DS_MAT_NAME				=0xA000,
T3DS_MAT_AMBIENT			=0xA010,
T3DS_MAT_DIFFUSE			=0xA020,
T3DS_MAT_SPECULAR			=0xA030,
T3DS_MAT_SHININESS			=0xA040,
T3DS_MAT_SHIN2PCT			=0xA041,
T3DS_MAT_TRANSPARENCY		=0xA050,
T3DS_MAT_XPFALL				=0xA052,
T3DS_MAT_REFBLUR			=0xA053,
T3DS_MAT_SHADING			=0xA100,
T3DS_MAT_TWO_SIDE			=0xA081,
T3DS_MAT_ADDITIVE			=0xA083,
T3DS_MAT_WIRE				=0xA085,
T3DS_MAT_FACEMAP			=0xA088,
T3DS_MAT_WIRESIZE			=0xA087,
T3DS_MAT_DECAL				=0xA082,
T3DS_MAT_MAPNAME			=0xA300,
T3DS_MAT_MAP_TILING			=0xA351,
T3DS_MAT_MAP_USCALE			=0xA354,
T3DS_MAT_MAP_VSCALE			=0xA356,
T3DS_MAT_MAP_UOFFSET		=0xA358,
T3DS_MAT_MAP_VOFFSET		=0xA35A,
T3DS_MAT_MAP_ANG    		=0xA35C,

T3DS_EDIT_OBJECT    		=0x4000,
T3DS_OBJ_TRIMESH    		=0x4100,
T3DS_OBJ_LIGHT      		=0x4600,
T3DS_OBJ_CAMERA     		=0x4700,

T3DS_CAM_RANGES				=0x4720,

T3DS_LIT_OFF				=0x4620,
T3DS_LIT_SPOT				=0x4610,
T3DS_LIT_INRANGE			=0x4659,
T3DS_LIT_OUTRANGE			=0x465A,

T3DS_TRI_VERTEXLIST			=0x4110,
T3DS_TRI_VERTEXOPTIONS		=0x4111,
T3DS_TRI_FACELIST			=0x4120,
T3DS_TRI_MAT_GROUP			=0x4130,
T3DS_TRI_SMOOTH_GROUP		=0x4150,
T3DS_TRI_FACEMAPPING		=0x4140,
T3DS_TRI_MATRIX				=0x4160,

T3DS_SPOTLIGHT				=0x4610,

T3DS_MAT_TEXMAP             =0xA200,
T3DS_MAT_TEXMASK            =0xA33E,
T3DS_MAT_TEX2MAP            =0xA33A,
T3DS_MAT_TEX2MASK           =0xA340,
T3DS_MAT_OPACMAP            =0xA210,
T3DS_MAT_OPACMASK           =0xA342,
T3DS_MAT_BUMPMAP            =0xA230,
T3DS_MAT_BUMPMASK           =0xA344,
T3DS_MAT_SPECMAP            =0xA204,
T3DS_MAT_SPECMASK           =0xA348,
T3DS_MAT_SHINMAP            =0xA33C,
T3DS_MAT_SHINMASK           =0xA346,
T3DS_MAT_SELFIMAP           =0xA33D,
T3DS_MAT_SELFIMASK          =0xA34A,
T3DS_MAT_REFLMAP            =0xA220,
T3DS_MAT_REFLMASK           =0xA34C,
T3DS_MAT_ACUBIC             =0xA310
};


C3DSChunk::C3DSChunk() : mID(0), mStart(0), mEnd(0)
{

}

void C3DSChunk::SetID(nova::nUInt16 id)
{
	mID = id;
}

void C3DSChunk::SetStart(nova::nUInt32 start)
{
	mStart = start;
}

void C3DSChunk::SetEnd(nova::nUInt32 end)
{
	mEnd = end;
}

nova::nUInt16 C3DSChunk::GetID(void) const
{
	return mID;
}

nova::nUInt32 C3DSChunk::GetStart(void) const
{
	return mStart;
}

nova::nUInt32 C3DSChunk::GetEnd(void) const
{
	return mEnd;
}

C3DSLoader::C3DSLoader()
{

}


C3DSLoader::~C3DSLoader()
{

}

C3DSChunk C3DSLoader::ReadChunk()
{
	C3DSChunk chunk;

	chunk.SetID(mpStream->ReadMemOfType<nova::nUInt16>());
	nova::nUInt32 len = mpStream->ReadMemOfType<nova::nUInt32>();
	chunk.SetStart(mpStream->Tell());
	chunk.SetEnd(chunk.GetStart() + (len-sizeof(nova::nUInt16)-sizeof(nova::nUInt32)));

	return chunk;
}

bool C3DSLoader::FindChunk(C3DSChunk & target, C3DSChunk & parent)
{
	if(parent.GetEnd() >= mpStream->Tell())
		return false;

	C3DSChunk chunk;
	chunk = ReadChunk();

	while((chunk.GetID() != target.GetID()) &&
		(chunk.GetEnd() <= parent.GetEnd()))
	{
		SkipChunk(chunk);
		if(chunk.GetEnd() >= parent.GetEnd())
			break;

		chunk = ReadChunk();
	}

	if(chunk.GetID() == target.GetID())
	{
		target.SetStart(chunk.GetStart());
		target.SetEnd(chunk.GetEnd());
		return true;
	}

	return false;
}

void C3DSLoader::SkipChunk(const C3DSChunk &chunk)
{
	mpStream->Seek(chunk.GetEnd());
}

void C3DSLoader::GotoChunk(const C3DSChunk &chunk)
{
	mpStream->Seek(chunk.GetStart());
}

CColorRGB C3DSLoader::ReadColor(const C3DSChunk &chunk)
{
	CColorRGB color = CColorRGB::BLACK;

	GotoChunk(chunk);
	switch(chunk.GetID())
	{
	case T3DS_COLOR_F:
		{
			color.R() = mpStream->ReadMemOfTypeInSize<float>(4);
			color.G() = mpStream->ReadMemOfTypeInSize<float>(4);
			color.B() = mpStream->ReadMemOfTypeInSize<float>(4);
		}
		break;
    case T3DS_COLOR_24:
		{
			color.R() = mpStream->ReadMemOfType<nova::nByte>() / 255.0f;
			color.G() = mpStream->ReadMemOfType<nova::nByte>() / 255.0f;
			color.B() = mpStream->ReadMemOfType<nova::nByte>() / 255.0f;
		}
        break;
    case T3DS_LIN_COLOR_F:
			color.R() = mpStream->ReadMemOfTypeInSize<float>(4);
			color.G() = mpStream->ReadMemOfTypeInSize<float>(4);
			color.B() = mpStream->ReadMemOfTypeInSize<float>(4);
        break;
    case T3DS_LIN_COLOR_24:
			color.R() = mpStream->ReadMemOfType<nova::nByte>() / 255.0f;
			color.G() = mpStream->ReadMemOfType<nova::nByte>() / 255.0f;
			color.B() = mpStream->ReadMemOfType<nova::nByte>() / 255.0f;
        break;
	default:
		throw NOVA_EXP("C3DSLoader::ReadColor - unknown color block type...", BAD_OPERATION);
	}

	return color;
}

float C3DSLoader::ReadPercentage(const C3DSChunk &chunk)
{
    GotoChunk(chunk);
	switch (chunk.GetID())
    {
    case T3DS_INT_PERCENTAGE:
        return mpStream->ReadMemOfType<nova::nUInt16>() / 100.0f;
    case T3DS_FLOAT_PERCENTAGE:
        return mpStream->ReadMemOfTypeInSize<float>(4);
    }

    throw NOVA_EXP("C3DSLoader::ReadPercentage - unknown Percentage block type...", BAD_OPERATION);

    return 0;
}

CMesh::TMeshContainer * C3DSLoader::LoadSingleMesh(const C3DSChunk &chunk, nstring & obj_name)
{
	nUInt16 count = 0;
	CMesh::TMeshContainer tMesh;
	CMesh::TMeshContainer *pMesh = NULL;
	memset(&tMesh, 0, sizeof(CMesh::TMeshContainer));

	mMeshesMap.insert(std::pair<nstring, CMesh::TMeshContainer>(obj_name, tMesh));
	pMesh = &(mMeshesMap[obj_name]);
	if(!pMesh)
		return NULL;

	GotoChunk(chunk);

	C3DSChunk _ch = ReadChunk();
	while(_ch.GetEnd() <= chunk.GetEnd())
	{
		switch(_ch.GetID())
		{
		case T3DS_TRI_VERTEXLIST:
			{
				count = mpStream->ReadMemOfType<nUInt16>();
				for(nUInt16 ic = 0; ic < count; ic++)
				{
					TVertex3d vertex;
					vertex.x = mpStream->ReadMemOfType<float>();
					vertex.y = mpStream->ReadMemOfType<float>();
					vertex.z = mpStream->ReadMemOfType<float>();

					pMesh->nVertexList.push_back(vertex);
				}
			}
			break;
		case T3DS_TRI_FACEMAPPING:
			{
				count = mpStream->ReadMemOfType<nova::nUInt16>();
				for(nUInt16 ic = 0; ic < count; ic++)
				{
					TUVMapping uv;
					uv.s = mpStream->ReadMemOfType<float>();
					uv.t = mpStream->ReadMemOfType<float>();
					uv.w = 0;

					pMesh->nMappingFacesList.push_back(uv);
				}
			}
			break;
		case T3DS_TRI_FACELIST:
			{
				ReadFaceList(_ch, pMesh);
			}
			break;
		case T3DS_TRI_MATRIX:
			{
				pMesh->nTMatrix = Matrix4f::IDENTITY;
				for (nUInt16 ic = 0; ic < 4; ic++) 
				{
                    for (nUInt16 jc = 0; jc < 3; jc++) 
					{
                        pMesh->nTMatrix(ic, jc) = mpStream->ReadMemOfTypeInSize<float>(4);
                    }
                }
			}
			break;
		default:
			break;
		}
        SkipChunk(_ch);

        if (_ch.GetEnd() >= chunk.GetEnd())
            break;

        _ch = ReadChunk();
	}

	return pMesh;
}

void C3DSLoader::ReadFaceList(const C3DSChunk &chunk,  CMesh::TMeshContainer *mesh)
{
	nova::nUInt16 count;
	nova::nUInt16 mat_id = 0;
	GotoChunk(chunk);

	count = mpStream->ReadMemOfType<nova::nUInt16>();
	for(nUInt16 i = 0; i < count; ++i)
	{
		TFaceIndex index;
		index.a = mpStream->ReadMemOfType<nova::nUInt16>();
		index.b = mpStream->ReadMemOfType<nova::nUInt16>();
		index.c = mpStream->ReadMemOfType<nova::nUInt16>();
		mpStream->ReadMemOfType<nova::nUInt16>();

		mesh->nIndexList.push_back(index);
	}

	// now read the optional chunks
    C3DSChunk _ch = ReadChunk();

	while (_ch.GetEnd() <= chunk.GetEnd())
    {
		switch (_ch.GetID())
        {
        case T3DS_TRI_MAT_GROUP:
			{
				nInt32 mat_id = 0;
				char str[128] = "\0";
				mpStream->ReadASCIIZ(str, 128);
				mat_id++;

				count = mpStream->ReadMemOfType<nova::nUInt16>();
				for (nova::nUInt16 i = 0; i < count; i++)
				{
					TTriangleInfo info;

					info.nMatName = nstring(str);
					info.nFace = mpStream->ReadMemOfType<nova::nUInt16>();
					info.nMatSubID = mat_id;

					mesh->nMeshInfoList.push_back(info);
				}
			}
            break;
/*
        case TRI_SMOOTH_GROUP:
            for (i=0; i<mesh.GetTriangleCount(); i++)
                mesh.GetTri(i).smoothingGroups = (nUInt32) ReadInt();

            break;
        }
*/
		}

        SkipChunk(_ch);
        _ch = ReadChunk();
	}
}

void C3DSLoader::LoadMeshList(void)
{
	stl<CMeshBoxPtr>::vector result;

	mpStream->Seek(0);

	C3DSChunk mainchunk;
	C3DSChunk object_block;

	mainchunk = ReadChunk();
	if(mainchunk.GetID() != T3DS_MAIN3DS)
	{
		//mpStream->FreeBuffer();
		throw NOVA_EXP("C3DSLoader::LoadMeshList - memory image is not appear \
			in 3ds format..", BAD_OPERATION);
	}

	object_block.SetID(T3DS_EDIT3DS);
    if (!FindChunk(object_block, mainchunk))
        return;

	GotoChunk(object_block);

	C3DSChunk scene_object;
	scene_object.SetID(T3DS_EDIT_OBJECT);
	while(FindChunk(scene_object, object_block))
	{
		char str[255] = "\0";
		mpStream->ReadASCIIZ( str, 255);
		nstring ob_name(str);

		C3DSChunk _ch = ReadChunk();
		if(_ch.GetID() == T3DS_OBJ_TRIMESH)
			LoadSingleMesh(_ch, ob_name);

		SkipChunk(scene_object);
	}
}

CMaterial::TMaterialContainer * C3DSLoader::ReadMaterial(const C3DSChunk &parent)
{
	CMaterial::TMaterialContainer tMat;
	memset(&tMat, 0, sizeof(CMaterial::TMaterialContainer));
	CMaterial::TMaterialContainer *pMat = NULL;
	C3DSChunk chunk;
	C3DSChunk child;

	GotoChunk(parent);

    chunk = ReadChunk();
	while (chunk.GetEnd() <= parent.GetEnd())
    {
		switch (chunk.GetID())
        {
        case T3DS_MAT_NAME:
			{
				char str[255] = "\0";
				mpStream->ReadASCIIZ(str, 255);

				mMaterialsMap.insert(std::pair<nstring, CMaterial::TMaterialContainer>(nstring(str), tMat));
				pMat = &(mMaterialsMap[nstring(str)]);
			}
            break;
        case T3DS_MAT_AMBIENT:
            child = ReadChunk();
			if(pMat)
				pMat->nAmbientColor = ReadColor(child);
            break;
        case T3DS_MAT_DIFFUSE:
            child = ReadChunk();
			if(pMat)
				pMat->nDiffuseColor = ReadColor(child);
            break;
        case T3DS_MAT_SPECULAR:
            child = ReadChunk();
			if(pMat)
				pMat->nSpecularColor = ReadColor(child);
            break;
        case T3DS_MAT_SHININESS:
            child = ReadChunk();
			if(pMat)
				pMat->nShininess = ReadPercentage(child);
            break;
        case T3DS_MAT_TRANSPARENCY:
            child = ReadChunk();
			if(pMat)
				pMat->nTransparency = ReadPercentage(child);
            break;
        case T3DS_MAT_SHADING:
			if(pMat)
				pMat->nShading = (nReal)mpStream->ReadMemOfType<nova::nUInt16>();
			break;
		case T3DS_MAT_XPFALL:
			{
            child = ReadChunk();
			if(pMat)
				pMat->nFalloff = ReadPercentage(child);
			}
			break;
		case T3DS_MAT_REFBLUR:
			{
            child = ReadChunk();
			if(pMat)
				pMat->nBlur = ReadPercentage(child);
			}
			break;
        case T3DS_MAT_TEXMAP:
			{
				CTexture::TTextureContainer * texture = ReadMap(chunk);
				if(texture && pMat)
					pMat->nDiffuseMap1 = texture->nName;
			}
            break;
        case T3DS_MAT_TEX2MAP:
			{
				CTexture::TTextureContainer * texture = ReadMap(chunk);
				if(texture && pMat)
					pMat->nDiffuseMap2 = texture->nName;
			}
            break;
        case T3DS_MAT_BUMPMAP:
			{
				CTexture::TTextureContainer * texture = ReadMap(chunk);
				if(texture && pMat)
					pMat->nBumpMap = texture->nName;
			}
            break;
        case T3DS_MAT_SPECMAP:
			{
				CTexture::TTextureContainer * texture = ReadMap(chunk);
				if(texture && pMat)
					pMat->nSpecMap = texture->nName;
			}
            break;
		case T3DS_MAT_TEXMASK:
			{
				CTexture::TTextureContainer * texture = ReadMap(chunk);
				if(texture && pMat)
					pMat->nDiffuseMap1Mask = texture->nName;
			}
			break;
		case T3DS_MAT_TEX2MASK:
			{
				CTexture::TTextureContainer * texture = ReadMap(chunk);
				if(texture && pMat)
					pMat->nDiffuseMap2Mask = texture->nName;
			}
			break;
		case T3DS_MAT_OPACMAP:
			{
				CTexture::TTextureContainer * texture = ReadMap(chunk);
				if(texture && pMat)
					pMat->nOpacMap = texture->nName;
			}
			break;
		case T3DS_MAT_OPACMASK:
			{
				CTexture::TTextureContainer * texture = ReadMap(chunk);
				if(texture && pMat)
					pMat->nOpacMapMask = texture->nName;
			}
			break;
		case T3DS_MAT_BUMPMASK:
			{
				CTexture::TTextureContainer * texture = ReadMap(chunk);
				if(texture && pMat)
					pMat->nBumpMapMask = texture->nName;
			}
		case T3DS_MAT_SPECMASK:
			{
				CTexture::TTextureContainer * texture = ReadMap(chunk);
				if(texture && pMat)
					pMat->nSpecMapMask = texture->nName;
			}
			break;
		case T3DS_MAT_SHINMAP:
			{
				CTexture::TTextureContainer * texture = ReadMap(chunk);
				if(texture && pMat)
					pMat->nShinMap = texture->nName;
			}
			break;
		case T3DS_MAT_SHINMASK:
			{
				CTexture::TTextureContainer * texture = ReadMap(chunk);
				if(texture && pMat)
					pMat->nShinMapMask = texture->nName;
			}
			break;
		case T3DS_MAT_SELFIMAP:
			{
				CTexture::TTextureContainer * texture = ReadMap(chunk);
				if(texture && pMat)
					pMat->nSelfIlMap = texture->nName;
			}
			break;
		case T3DS_MAT_SELFIMASK:
			{
				CTexture::TTextureContainer * texture = ReadMap(chunk);
				if(texture && pMat)
					pMat->nSelfIlMapMask = texture->nName;
			}
			break;
		case T3DS_MAT_REFLMAP:
			{
				CTexture::TTextureContainer * texture = ReadMap(chunk);
				if(texture && pMat)
					pMat->nReflectionMap = texture->nName;
			}
			break;
		case T3DS_MAT_REFLMASK:
			{
				CTexture::TTextureContainer * texture = ReadMap(chunk);
				if(texture && pMat)
					pMat->nReflectionMapMask = texture->nName;
			}
			break;
		case T3DS_MAT_ACUBIC:
			{
				if(pMat)
				{
					pMat->nAutoreflMapAntiAlias = mpStream->ReadMemOfType<nByte>();
					pMat->nAutoreflMapFlags = mpStream->ReadMemOfType<nUInt16>();
					pMat->nAutoreflMapSize = mpStream->ReadMemOfType<nInt32>();
					pMat->nAutoreflMapFrameStep = mpStream->ReadMemOfType<nInt32>();
				}
			}
			break;
        }

        SkipChunk(chunk);
        chunk = ReadChunk();
    }



	return pMat;
}

CTexture::TTextureContainer * C3DSLoader::ReadMap(const C3DSChunk &chunk)
{
	CTexture::TTextureContainer tMap;
	memset(&tMap, 0, sizeof(CTexture::TTextureContainer));
	CTexture::TTextureContainer *pMap = NULL;
	C3DSChunk child;
	GotoChunk(chunk);
    child = ReadChunk();

	while (child.GetEnd() <= chunk.GetEnd())
    {
		switch (child.GetID())
        {
        case T3DS_MAT_MAPNAME:
			{
				char str[255] = "\0";
				mpStream->ReadASCIIZ(str, 255);

				stl<nstring, CTexture::TTextureContainer>::map::iterator it = mTexturesMap.find(nstring(str));
				if(it != mTexturesMap.end())
					pMap = &(it->second);
				else
				{
					mTexturesMap.insert(std::pair<nstring, CTexture::TTextureContainer>(nstring(str), tMap));
					pMap = &(mTexturesMap[nstring(str)]);
				}
			}
            break;
        case T3DS_MAT_MAP_USCALE:
			if(pMap)
				pMap->nUScale = mpStream->ReadMemOfTypeInSize<float>(4);
            break;
        case T3DS_MAT_MAP_VSCALE:
			if(pMap)
				pMap->nVScale = mpStream->ReadMemOfTypeInSize<float>(4);
            break;
        case T3DS_MAT_MAP_UOFFSET:
			if(pMap)
				pMap->nUOffset = mpStream->ReadMemOfTypeInSize<float>(4);
            break;
        case T3DS_MAT_MAP_VOFFSET:
			if(pMap)
				pMap->nVOffset = mpStream->ReadMemOfTypeInSize<float>(4);
            break;
        case T3DS_MAT_MAP_ANG:
			if(pMap)
				pMap->nRotation = mpStream->ReadMemOfTypeInSize<float>(4);
            break;
        }
        SkipChunk(child);
        child = ReadChunk();
    }


	return pMap;
}

void C3DSLoader::LoadMaterialList()
{
	stl<CMaterialPtr>::vector result;

	mpStream->Seek(0);

	C3DSChunk mainchunk;
	C3DSChunk object_block;
	C3DSChunk mat;

	mainchunk = ReadChunk();
	if(mainchunk.GetID() != T3DS_MAIN3DS)
	{
		//mpStream->FreeBuffer();
		throw NOVA_EXP("C3DSLoader::LoadMaterialList - memory image is not appear \
			in 3ds format..", BAD_OPERATION);
	}

	object_block.SetID(T3DS_EDIT3DS);
    if (!FindChunk(object_block, mainchunk))
        return;

	GotoChunk(object_block);
	mat.SetID(T3DS_MAT_ENTRY);
    while (FindChunk(mat, object_block))
    {
        ReadMaterial(mat);
        SkipChunk(mat);
    }
}

void C3DSLoader::LoadImpl(void)
{
	LoadMeshList();
	LoadMaterialList();
}

void C3DSLoader::InitLoader(void)
{
	if(!mpStream)
		throw NOVA_EXP("void CASELoader::LoadAseInternal(void): mpStream is bad ptr..", MEM_ERROR);

	if(!mpStream->IsOpened())
		throw NOVA_EXP("void CASELoader::LoadAseInternal(void): mpStream is not opened..", MEM_ERROR);
}

void C3DSLoader::CloseLoader(void)
{

}


}
