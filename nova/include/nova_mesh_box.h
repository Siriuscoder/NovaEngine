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

class CMesh;

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

	virtual void BuildMeshListener(CMesh * object) {}

	virtual void CalculateNormalsListener(CMesh * object) {}

	virtual void GenerateBoundingBoxListener(CMesh * object) {}
};



class NOVA_EXPORT CMesh : public CResource
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
// For mesh internal loading
		CDataStream *pMeshStream;
		bool nInternalLoading;
	} TMeshContainer;

	static void QSortFaces(TIndexes &index, TFacesInfo &faces);

private:

	static nInt32 QComparer(const void * a, const void * b);

protected:

	TMeshContainer mMeshDef;

	void LoadResourceImpl(void);

	void FreeResourceImpl(void);

	void BuildResourceImpl(void);

public:

	CMesh(CResourceManager * rm, const nstring & name, const nstring & group, TAttach state);
	~CMesh();

	TMeshContainer &GetMeshDefinition(void);

	void SetMeshDefinition(TMeshContainer *_mesh);

	void CalculateNormals(void/* Simple method calc normals or not? 
							  i want to use smoothing groups in future*/);

	bool CheckValidLength();

	CBoundingBox GenerateBoundingBox(void);

	void GenerateNormalsToFaces(void);

	void SortFaceIndexByMaterials(void);

	void * GetVertexPointer(size_t *count);

	void * GetNormalsPointer(size_t *count);

	void * GetUVPointer(size_t *count);

	void * GetIndexesPointer(size_t *count);

	void ToWorldCoord(void);
};

typedef CSmartPtr<CMesh> CMeshBoxPtr;

class NOVA_EXPORT CMeshManager : public CSingelton<CMeshManager>, public CResourceManager
{
protected:

	CResourcePtr LoadResourceFromXmlNodeImpl(xmlNodePtr node);

	CResourcePtr LoadResourceFromXmlNodeImpl(xmlNodePtr node, const CFilesPackage &package);

public:

	CMeshManager(const nstring & resourceFactoryName);
	~CMeshManager();

	virtual void UnloadAllManagerResources();

	virtual CResourcePtr CreateInstance(const nstring & name, 
		const nstring & group, CResource::TAttach state);

	CMeshBoxPtr CreateMesh(CMesh::TMeshContainer *def, const nstring &group,
		CResource::TAttach state = CResource::NV_ATTACHED);

};

}