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

#include "nova_resource_manager.h"
#include "nova_bounding_box.h"
#include "nova_streams.h"
#include "nova_singelton.h"
#include "nova_matrix4d.h"
#include "nova_matrix3d.h"
#include "nova_color.h"

namespace nova
{

class CMeshBox;

#pragma pack(push, 1)

union TVertex3d
{
	struct 
	{
		nova::nReal x;
		nova::nReal y;
		nova::nReal z;
	};

	nova::nReal v[3];
};

union TVertex4d
{
	struct 
	{
		nova::nReal x;
		nova::nReal y;
		nova::nReal z;
		nova::nReal t;
	};

	nova::nReal v[4];
};

union TUVMapping
{
	struct 
	{
		nova::nReal s;
		nova::nReal t;
		nova::nReal w;
	};

	nova::nReal v[3];
};

union TNormal3d
{
	struct
	{
		nova::nReal x;
		nova::nReal y;
		nova::nReal z;
	};

	nova::nReal v[3];
};	

union TNormal4d
{
	struct
	{
		nova::nReal x;
		nova::nReal y;
		nova::nReal z;
		nova::nReal t;
	};

	nova::nReal v[4];
};	

union TFaceIndex
{
	struct
	{
		nInt32 a;
		nInt32 b;
		nInt32 c;
	};

	nova::nInt32 v[3];
};

struct TTriangleInfo
{
	nstring nMatName;
	nUInt32 nMatSubID;
	nUInt32 nFace;
	TNormal3d nFaceNormal;
};

#pragma pack(pop)

class NOVA_EXPORT CMeshBoxListener : public CResourceListener
{
public:

	virtual void BuildMeshListener(CMeshBox * object) {}

	virtual void CalculateNormalsListener(CMeshBox * object) {}

	virtual void GenerateBoundingBoxListener(CMeshBox * object) {}
};



class NOVA_EXPORT CMeshBox : public CResource
{
public:

	typedef nova::stl<TVertex3d>::vector TVertexes;
	typedef nova::stl<TNormal3d>::vector TNormals;
	typedef nova::stl<TUVMapping>::vector TTexCoords;
	typedef nova::stl<TFaceIndex>::vector TIndexes;
	typedef nova::stl<TTriangleInfo>::vector TFacesInfo;
	typedef nova::stl<nInt32>::vector TSubMats;

	typedef struct _MeshContainer
	{
		nstring nName;
		nova::CColorRGB mMeshColor;
		TVertexes nVertexList;
		TTexCoords nMappingFacesList;
		TNormals nNormalList;
		TIndexes nIndexList;
		TFacesInfo nMeshInfoList;
		Matrix4f nTMatrix;
		nInt32 MatID;
// Reserved buffers
		TIndexes nTVIndexList;
		TTexCoords nTVMappingList;
	} TMeshContainer;

	static void QSortFaces(TIndexes &index, TFacesInfo &faces);

private:

	static nInt32 QComparer(const void * a, const void * b);

protected:

	TMeshContainer mMeshDef;

public:

	CMeshBox(CResourceManager * rm, nstring & name, nstring & group, TAttach state);
	~CMeshBox();

	TMeshContainer &GetMeshDefinition(void);

	void CalculateNormals(void/* Simple method calc normals or not? 
							  i want to use smoothing groups in future*/);

	bool CheckValidLength();

	CBoundingBox GenerateBoundingBox(void);

	void GenerateNormalsToFaces(void);

	void SortFaceIndexByMaterials(void);
//----------------------------------
	virtual void FreeResource(void);

	virtual void PreAddingAction(void);

	virtual void PostAddingAction(void);

	virtual void PreUnloadingAction(void);

	virtual void BuildResource(void);

	virtual void PrepareResource(void);
//-----------------------------------

	void * GetVertexPointer(size_t *count);

	void * GetNormalsPointer(size_t *count);

	void * GetUVPointer(size_t *count);

	void * GetIndexesPointer(size_t *count);

	void ToWorldCoord(void);
};

typedef CSmartPtr<CMeshBox> CMeshBoxPtr;

class NOVA_EXPORT CMeshManager : public CSingelton<CMeshManager>, public CResourceManager
{
protected:


public:

	CMeshManager();
	~CMeshManager();

	virtual void UnloadAllManagerResources();

	virtual CResourcePtr CreateInstance(nstring & name, 
		nstring & group, CResource::TAttach state);

	CMeshBoxPtr CreateMesh(nstring & name, nstring & group, 
		CMemoryBuffer & vertexes, CMemoryBuffer & normals,
		CMemoryBuffer & coords, CMemoryBuffer & indexes,
		stl<nstring>::vector & sub_mats, CMeshBox::TFacesInfo & mat_indexes,
		nova::Matrix3f & trans_mat, nova::Vector3f & trans_vec,
		CResource::TAttach state = CResource::NV_ATTACHED); 
};

}