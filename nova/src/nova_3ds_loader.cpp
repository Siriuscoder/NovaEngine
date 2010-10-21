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
T3DS_MAT_SHADING			=0xA100,
T3DS_MAT_TWO_SIDE			=0xA081,
T3DS_MAT_ADDITIVE			=0xA083,
T3DS_MAT_WIRE				=0xA085,
T3DS_MAT_FACEMAP			=0xA088,
T3DS_MAT_WIRESIZE			=0xA087,
T3DS_MAT_DECAL				=0xA082,
T3DS_MAT_TEXMAP				=0xA200,
T3DS_MAT_MAPNAME			=0xA300,
T3DS_MAT_MAP_TILING			=0xA351,
T3DS_MAT_MAP_USCALE			=0xA354,
T3DS_MAT_MAP_VSCALE			=0xA356,
T3DS_MAT_MAP_UOFFSET		=0xA358,
T3DS_MAT_MAP_VOFFSET		=0xA35A,
T3DS_MAT_MAP_ANG    		=0xA35C,
T3DS_MAT_TEX2MAP    		=0xA33A,
T3DS_MAT_OPACMAP    		=0xA210,
T3DS_MAT_BUMPMAP    		=0xA230,
T3DS_MAT_SPECMAP    		=0xA204,
T3DS_MAT_SHINMAP    		=0xA33C,
T3DS_MAT_REFLMAP    		=0xA220,
T3DS_MAT_ACUBIC     		=0xA310,

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

T3DS_SPOTLIGHT				=0x4610
};


C3DSChunk::C3DSChunk() : mID(0), mStart(0), mEnd(0)
{

}

void C3DSChunk::SetID(nova::nUInt16 id)
{
	mID = id;
}

void C3DSChunk::SetStart(nova::uint start)
{
	mStart = start;
}

void C3DSChunk::SetEnd(nova::uint end)
{
	mEnd = end;
}

nova::nUInt16 C3DSChunk::GetID(void) const
{
	return mID;
}

nova::uint C3DSChunk::GetStart(void) const
{
	return mStart;
}

nova::uint C3DSChunk::GetEnd(void) const
{
	return mEnd;
}

C3DSLoader::C3DSLoader(CDataStream & stream, bool geom, bool mult_thread)
{
	mp3dsStream = &stream;
	mGeometry = geom;
	mOwerThread = mult_thread;
}


C3DSLoader::~C3DSLoader()
{

}

C3DSChunk C3DSLoader::ReadChunk()
{
	C3DSChunk chunk;

	chunk.SetID(mp3dsStream->ReadMemOfType<nova::nUInt16>());
	nova::uint len = mp3dsStream->ReadMemOfType<nova::uint>();
	chunk.SetStart(mp3dsStream->Tell());
	chunk.SetEnd(chunk.GetStart() + (len-sizeof(nova::nUInt16)-sizeof(nova::uint)));

	return chunk;
}

bool C3DSLoader::FindChunk(C3DSChunk & target, C3DSChunk & parent)
{
	if(parent.GetEnd() >= mp3dsStream->Tell())
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
	mp3dsStream->Seek(chunk.GetEnd());
}

void C3DSLoader::GotoChunk(const C3DSChunk &chunk)
{
	mp3dsStream->Seek(chunk.GetStart());
}

CColorRGB C3DSLoader::ReadColor(const C3DSChunk &chunk)
{
	CColorRGB color = CColorRGB::BLACK;

	GotoChunk(chunk);
	switch(chunk.GetID())
	{
	case T3DS_COLOR_F:
		{
			color.R() = mp3dsStream->ReadMemOfTypeInSize<float>(4);
			color.G() = mp3dsStream->ReadMemOfTypeInSize<float>(4);
			color.B() = mp3dsStream->ReadMemOfTypeInSize<float>(4);
		}
		break;
    case T3DS_COLOR_24:
		{
			color.R() = mp3dsStream->ReadMemOfType<nova::byte>() / 255.0f;
			color.G() = mp3dsStream->ReadMemOfType<nova::byte>() / 255.0f;
			color.B() = mp3dsStream->ReadMemOfType<nova::byte>() / 255.0f;
		}
        break;
    case T3DS_LIN_COLOR_F:
			color.R() = mp3dsStream->ReadMemOfTypeInSize<float>(4);
			color.G() = mp3dsStream->ReadMemOfTypeInSize<float>(4);
			color.B() = mp3dsStream->ReadMemOfTypeInSize<float>(4);
        break;
    case T3DS_LIN_COLOR_24:
			color.R() = mp3dsStream->ReadMemOfType<nova::byte>() / 255.0f;
			color.G() = mp3dsStream->ReadMemOfType<nova::byte>() / 255.0f;
			color.B() = mp3dsStream->ReadMemOfType<nova::byte>() / 255.0f;
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
        return mp3dsStream->ReadMemOfType<nova::nUInt16>() / 100.0f;
    case T3DS_FLOAT_PERCENTAGE:
        return mp3dsStream->ReadMemOfTypeInSize<float>(4);
    }

    throw NOVA_EXP("C3DSLoader::ReadPercentage - unknown Percentage block type...", BAD_OPERATION);

    return 0;
}

CMeshBoxPtr C3DSLoader::LoadSingleMesh(const C3DSChunk &chunk, nstring & obj_name)
{
	unsigned short count = 0;
	CMemoryBuffer	vertexes,
					mat_coords,
					indexes,
					mat_groups,
					normals;

	CMeshBox::TFacesInfo mat_indexes;

	nova::Matrix3f matrix = Matrix3f::IDENTITY;
	nova::Vector3f pos = Vector3f::ZERO;
	stl<nstring>::vector mat_names;

	GotoChunk(chunk);

	C3DSChunk _ch = ReadChunk();
	while(_ch.GetEnd() <= chunk.GetEnd())
	{
		switch(_ch.GetID())
		{
		case T3DS_TRI_VERTEXLIST:
			{
				count = mp3dsStream->ReadMemOfType<nova::nUInt16>();
				vertexes.AllocBuffer(count * sizeof(TVertex4d));

				mp3dsStream->Read(vertexes);
			}
			break;
		case T3DS_TRI_FACEMAPPING:
			{
				count = mp3dsStream->ReadMemOfType<nova::nUInt16>();
				mat_coords.AllocBuffer(count * sizeof(TTexCoord2d));

				mp3dsStream->Read(mat_coords);
			}
			break;
		case T3DS_TRI_FACELIST:
			{
				ReadFaceList(_ch, indexes, mat_indexes, mat_names);
			}
			break;
		case T3DS_TRI_MATRIX:
			{
				matrix(0, 0) = mp3dsStream->ReadMemOfTypeInSize<float>(4);
				matrix(0, 1) = mp3dsStream->ReadMemOfTypeInSize<float>(4);
				matrix(0, 2) = mp3dsStream->ReadMemOfTypeInSize<float>(4);

				matrix(1, 0) = mp3dsStream->ReadMemOfTypeInSize<float>(4);
				matrix(1, 1) = mp3dsStream->ReadMemOfTypeInSize<float>(4);
				matrix(1, 2) = mp3dsStream->ReadMemOfTypeInSize<float>(4);

				matrix(2, 0) = mp3dsStream->ReadMemOfTypeInSize<float>(4);
				matrix(2, 1) = mp3dsStream->ReadMemOfTypeInSize<float>(4);
				matrix(2, 2) = mp3dsStream->ReadMemOfTypeInSize<float>(4);

				pos.X() = mp3dsStream->ReadMemOfTypeInSize<float>(4);
				pos.Y() = mp3dsStream->ReadMemOfTypeInSize<float>(4);
				pos.Z() = mp3dsStream->ReadMemOfTypeInSize<float>(4);
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

	nstring group("Mesh3dsSection");
	CMeshBoxPtr mesh;
	if(	mOwerThread )
		mesh = CMeshManager::GetSingelton().CreateMeshAsync(obj_name,
			group, vertexes, normals, mat_coords, indexes, mat_names, mat_indexes, matrix, pos);
	else
		mesh = CMeshManager::GetSingelton().CreateMesh(obj_name,
			group, vertexes, normals, mat_coords, indexes, mat_names, mat_indexes, matrix, pos);

	vertexes.FreeBuffer();
	mat_coords.FreeBuffer();
	indexes.FreeBuffer();
	mat_names.clear();
	mat_indexes.clear();

	return mesh;
}

void C3DSLoader::ReadFaceList(const C3DSChunk &chunk, CMemoryBuffer & indexes,
	CMeshBox::TFacesInfo & mat_groups, stl<nstring>::vector & mat_names)
{
	nova::nUInt16 count;
	GotoChunk(chunk);

	count = mp3dsStream->ReadMemOfType<nova::nUInt16>();

	indexes.AllocBuffer(count * sizeof(TTriIndex));
	TTriIndex *tri = static_cast<TTriIndex *>(indexes.GetBegin());
	for(int i = 0; i < count; ++i)
	{
		tri->a = mp3dsStream->ReadMemOfType<nova::nUInt16>();
		tri->b = mp3dsStream->ReadMemOfType<nova::nUInt16>();
		tri->c = mp3dsStream->ReadMemOfType<nova::nUInt16>();

		//memcpy(indexes.GetWritePtr(), &tri, sizeof(TTriIndex));
		//indexes.SetWritePos(indexes.GetWritePos() + sizeof(TTriIndex));
		tri++;
	}

	// now read the optional chunks
    C3DSChunk _ch = ReadChunk();

	while (_ch.GetEnd() <= chunk.GetEnd())
    {
		switch (_ch.GetID())
        {
        case T3DS_TRI_MAT_GROUP:
			{
				int mat_id = 0;
				char str[30] = "\0";
				mp3dsStream->ReadASCIIZ(str, 30);

				mat_names.push_back(nstring(str));
				mat_id = mat_names.size() -1;

				count = mp3dsStream->ReadMemOfType<nova::nUInt16>();
				for (nova::nUInt16 i = 0; i < count; i++)
				{
					TTriangleInfo info;
					nova::nUInt16 index = mp3dsStream->ReadMemOfType<nova::nUInt16>();

					info.mat_id = mat_id;
					info.tri_id = index;

					mat_groups.push_back(info);
				}
			}
            break;
/*
        case TRI_SMOOTH_GROUP:
            for (i=0; i<mesh.GetTriangleCount(); i++)
                mesh.GetTri(i).smoothingGroups = (ulong) ReadInt();

            break;
        }
*/
		}

        SkipChunk(_ch);
        _ch = ReadChunk();
	}
}

stl<CMeshBoxPtr>::vector C3DSLoader::LoadMeshList(void)
{
	stl<CMeshBoxPtr>::vector result;

	mp3dsStream->Seek(0);

	C3DSChunk mainchunk;
	C3DSChunk object_block;

	mainchunk = ReadChunk();
	if(mainchunk.GetID() != T3DS_MAIN3DS)
	{
		//mp3dsStream->FreeBuffer();
		throw NOVA_EXP("C3DSLoader::LoadMeshList - memory image is not appear \
			in 3ds format..", BAD_OPERATION);
	}

	object_block.SetID(T3DS_EDIT3DS);
    if (!FindChunk(object_block, mainchunk))
        return result;

	GotoChunk(object_block);

	C3DSChunk scene_object;
	scene_object.SetID(T3DS_EDIT_OBJECT);
	while(FindChunk(scene_object, object_block))
	{
		char str[255] = "\0";
		mp3dsStream->ReadASCIIZ( str, 255);
		nstring ob_name(str);

		C3DSChunk _ch = ReadChunk();
		if(_ch.GetID() == T3DS_OBJ_TRIMESH)
			result.push_back(LoadSingleMesh(_ch, ob_name));

		SkipChunk(scene_object);
	}

	return result;
}

CMaterialPtr C3DSLoader::ReadMaterial(const C3DSChunk &parent)
{
	nova::CColorRGB AmbientColor = CColorRGB::BLACK;
	nova::CColorRGB DiffuseColor = CColorRGB::BLACK;
	nova::CColorRGB SpecularColor = CColorRGB::BLACK;
	nReal Shininess;
	nReal Transparency;
	nstring TexMap;
	nstring MultiTexMap;
	nstring SpecMap;
	nstring BumpMap;
	nstring ReflectionMap;
	nstring mat_name;
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
				mp3dsStream->ReadASCIIZ(str, 255);

				mat_name.append(str);
			}
            break;
        case T3DS_MAT_AMBIENT:
            child = ReadChunk();
            AmbientColor = ReadColor(child);
            break;
        case T3DS_MAT_DIFFUSE:
            child = ReadChunk();
            DiffuseColor = ReadColor(child);
            break;
        case T3DS_MAT_SPECULAR:
            child = ReadChunk();
            SpecularColor = ReadColor(child);
            break;
        case T3DS_MAT_SHININESS:
            child = ReadChunk();
            Shininess = ReadPercentage(child);
            break;
        case T3DS_MAT_TRANSPARENCY:
            child = ReadChunk();
            Transparency = ReadPercentage(child);
            break;
/*        case T3DS_MAT_SHADING:
            sh = ReadShort();
            switch (sh)
            {
            case 0:
                mat.SetShadingType(sWireframe);
                break;
            case 1:
                mat.SetShadingType(sFlat);
                break;
            case 2:
                mat.SetShadingType(sGouraud);
                break;
            case 3:
                mat.SetShadingType(sPhong);
                break;
            case 4:
                mat.SetShadingType(sMetal);
                break;
            }
            break;
        case T3DS_MAT_WIRE:
            mat.SetShadingType(sWireframe);
            break;

*/
        case T3DS_MAT_TEXMAP:
			{
				CTexturePtr texture = ReadMap(chunk);
				if(!texture.IsNull())
					TexMap = texture->GetResName();
			}
            break;
        case T3DS_MAT_TEX2MAP:
			{
				CTexturePtr texture = ReadMap(chunk);
				if(!texture.IsNull())
					MultiTexMap = texture->GetResName();
			}
            break;
/*        case T3DS_MAT_OPACMAP:
			CTexturePtr texture = ReadMap(chunk);
			if(!texture.IsNull())
				TexMap = texture->GetResName();
            break;
*/
        case T3DS_MAT_BUMPMAP:
			{
				CTexturePtr texture = ReadMap(chunk);
				if(!texture.IsNull())
					BumpMap = texture->GetResName();
			}
            break;
        case T3DS_MAT_SPECMAP:
			{
				CTexturePtr texture = ReadMap(chunk);
				if(!texture.IsNull())
					SpecMap = texture->GetResName();
			}
            break;
/*        case T3DS_MAT_REFLMAP:
            child = ReadChunk();
            mat.GetReflectionMap().strength = ReadPercentage(child);
            SkipChunk(child);
            child = ReadChunk();
            if (child.id != MAT_MAPNAME)
            {
                ErrorMsg("L3DS::ReadMaterial - error, expected chunk not found");
                return;
            }
            ReadASCIIZ(str, 30);
            if (strcmp(str, "") == 0)
                strcpy(mat.GetReflectionMap().mapName, "auto");
            break;
*/
        }

        SkipChunk(chunk);
        chunk = ReadChunk();
    }

	nstring group("Material3dsSection");
	CMaterialPtr material;
	if(	mOwerThread )
		material = CMaterialManager::GetSingelton().CreateMaterialAsync(mat_name,
			group, AmbientColor, DiffuseColor, SpecularColor, Shininess, Transparency,
			TexMap, MultiTexMap, SpecMap, BumpMap, ReflectionMap);
	else
		material = CMaterialManager::GetSingelton().CreateMaterial(mat_name,
			group, AmbientColor, DiffuseColor, SpecularColor, Shininess, Transparency,
			TexMap, MultiTexMap, SpecMap, BumpMap, ReflectionMap);

	return material;
}

CTexturePtr C3DSLoader::ReadMap(const C3DSChunk &chunk)
{
	C3DSChunk child;
	nstring tex_name("Texture_");
	nstring image_name;

	GotoChunk(chunk);
    child = ReadChunk();

	nova::nReal uScale;
	nova::nReal vScale;
	nova::nReal uOffset;
	nova::nReal vOffset;
	nova::nReal Angle;

	while (child.GetEnd() <= chunk.GetEnd())
    {
		switch (child.GetID())
        {
/*        case T3DS_INT_PERCENTAGE:
            map.strength = ReadPercentage(child);
            break;
*/
        case T3DS_MAT_MAPNAME:
			{
				char str[255] = "\0";
				mp3dsStream->ReadASCIIZ(str, 255);

				image_name.append(str);
			}
            break;
        case T3DS_MAT_MAP_USCALE:
            uScale = mp3dsStream->ReadMemOfTypeInSize<float>(4);
            break;
        case T3DS_MAT_MAP_VSCALE:
            vScale = mp3dsStream->ReadMemOfTypeInSize<float>(4);
            break;
        case T3DS_MAT_MAP_UOFFSET:
            uOffset = mp3dsStream->ReadMemOfTypeInSize<float>(4);
            break;
        case T3DS_MAT_MAP_VOFFSET:
            vOffset = mp3dsStream->ReadMemOfTypeInSize<float>(4);
            break;
        case T3DS_MAT_MAP_ANG:
            Angle = mp3dsStream->ReadMemOfTypeInSize<float>(4);
            break;
        }
        SkipChunk(child);
        child = ReadChunk();
    }

	nstring im_group("Image3dsSection");
	nstring path = CImageManager::GetSingelton().GetResourceLocation();
	CImagePtr image;
	nstring image_file = path + image_name;
	if(	mOwerThread )
		image = CImageManager::GetSingelton().CreateNewImageAsync(image_name, im_group,
			image_file, CImageFormats::NF_DEFAULT);
	else
		image = CImageManager::GetSingelton().CreateNewImage(image_name, im_group,
			image_file, CImageFormats::NF_DEFAULT);

	nstring tex_group("Texture3dsSection");
	tex_name.append(image_name);

	bool mip = true;
	CTextureSurfaceList::MipMapTagertType miptype = CTextureManager::GetSingelton().GetDefaultAutoMipmap();
	if(miptype == CTextureSurfaceList::DO_NOT_USE_MIPMAPS)
		mip = false;

	CTexturePtr texture;
	if(	mOwerThread )
		texture = CTextureManager::GetSingelton().CreateNewTextureAsync(tex_name, tex_name,
			image, CHardwarePixelBuffer::USE_TEXTURE_2D, CTextureManager::GetSingelton().GetDefaultAutoMipmap(),
			CTexture::CLAMP_TO_EDGE, CTexture::CLAMP_TO_EDGE, CTexture::EV_MODULATE);
	else
		texture = CTextureManager::GetSingelton().CreateNewTexture(tex_name, tex_name,
			image, CHardwarePixelBuffer::USE_TEXTURE_2D, CTextureManager::GetSingelton().GetDefaultAutoMipmap(),
			CTexture::CLAMP_TO_EDGE, CTexture::CLAMP_TO_EDGE, CTexture::EV_MODULATE);

	CImageManager::GetSingelton().UnloadResourceFromHash(image_name);

	texture->SetAngle(Angle);
	texture->SetSOffset(uOffset);
	texture->SetTOffset(vOffset);
	texture->SetSTiling(uScale);
	texture->SetTTiling(vScale);

	return texture;
}

stl<CMaterialPtr>::vector C3DSLoader::LoadMaterialList()
{
	stl<CMaterialPtr>::vector result;

	mp3dsStream->Seek(0);

	C3DSChunk mainchunk;
	C3DSChunk object_block;
	C3DSChunk mat;

	mainchunk = ReadChunk();
	if(mainchunk.GetID() != T3DS_MAIN3DS)
	{
		//mp3dsStream->FreeBuffer();
		throw NOVA_EXP("C3DSLoader::LoadMaterialList - memory image is not appear \
			in 3ds format..", BAD_OPERATION);
	}

	object_block.SetID(T3DS_EDIT3DS);
    if (!FindChunk(object_block, mainchunk))
        return result;

	GotoChunk(object_block);
	mat.SetID(T3DS_MAT_ENTRY);
    while (FindChunk(mat, object_block))
    {
        CMaterialPtr material = ReadMaterial(mat);
        SkipChunk(mat);

		if(!material.IsNull())
			result.push_back(material);
    }

	return result;
}


}
