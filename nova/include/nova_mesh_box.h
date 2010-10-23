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
#include "nova_matrix3d.h"

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
	};

	nova::nReal v[2];
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
	nInt32 mat_id;
	nInt32 tri_id;
	TNormal3d normal;
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

	nova::Matrix3f mRotationMatrix;
	nova::Vector3f mRealPosition;

	static void QSortFaces(TIndexes &index, TFacesInfo &faces);

private:

	static nInt32 QComparer(const void * a, const void * b);

protected:

	stl<nstring>::vector mMatNames;

	TVertexes mVertexes;
	TNormals mNormals;
	TTexCoords mTexCoords;
	TIndexes mIndexes;
	TFacesInfo mInfo;
	TSubMats mSubMats;

public:

	CMeshBox(CResourceManager * rm, nstring & name, nstring & group, TAttach state);
	~CMeshBox();

	void CalculateNormals(void/* Simple method calc normals or not? 
							  i want to use smoothing groups in future*/);

	// loading data arrays
	void FillMeshBuffer(CMemoryBuffer & vertexes); 

	void FillNornalBuffer(CMemoryBuffer & normals); 

	void FillTexCoordBuffer(CMemoryBuffer & coords); 

	void FillIndexBuffer(CMemoryBuffer & indexes); 

	bool CheckValidLength();

	nova::nUInt32 GetVertexesLen(void);

	nova::nUInt32 GetTrianglesLen(void);

	nova::nUInt32 GetNormalsLen(void);

	size_t GetVertexesLenInBytes(void);

	size_t GetTrianglesLenInBytes(void);

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
	nInt32 GetMaterialID(nova::nUInt32 face);

	void SetMaterialID(nova::nUInt32 face, nInt32 id);

	nInt32 GetMaterialIDByName(nstring & name);

	nstring GetMeterialNameByID(nova::nUInt32 id);

	stl<nstring>::vector GetMaterials();

	nInt32 AddNewSubMaterial(nstring & resource_name);

	void * GetMesh(void);

	void * GetNormals(void);

	void * GetTexCoord(void);

	void * GetIndexes(void);

	TTriangleInfo GetFaceInfo(nova::nUInt32 face);

	void ToWorldCoord(void);

	void SetRotationMatrix(const nova::Matrix3f & mat);

	void SetPosition(const nova::Vector3f & vec);
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

	CMeshBoxPtr CreateMeshAsync(nstring & name, nstring & group, 
		CMemoryBuffer & vertexes, CMemoryBuffer & normals,
		CMemoryBuffer & coords, CMemoryBuffer & indexes,
		stl<nstring>::vector & sub_mats, CMeshBox::TFacesInfo & mat_indexes,
		nova::Matrix3f & trans_mat, nova::Vector3f & trans_vec,
		CResource::TAttach state = CResource::NV_ATTACHED);
};

}