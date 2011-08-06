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
#include "nova_hardware_vertex_buffer.h"

namespace nova
{

class CMesh;


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

#pragma pack(push, 1)
// vertex node full structure
	typedef struct
	{
		union
		{
			struct 
			{
				nova::nReal x;
				nova::nReal y;
				nova::nReal z;
			};

			nova::nReal v[3];
		};

		union
		{
			struct 
			{
				nova::nReal nx;
				nova::nReal ny;
				nova::nReal nz;
			};

			nova::nReal nv[3];
		};

		union 
		{
			struct 
			{
				nova::nReal s;
				nova::nReal t;
				nova::nReal w;
			};

			nova::nReal stw[3];
		};
	} stVertex3d3n3uv_t;

	typedef union
	{
		struct
		{
			nUInt32 a;
			nUInt32 b;
			nUInt32 c;
		};

		nova::nUInt32 v[3];
	} TFaceABC;

	typedef struct
	{
		TFaceABC faceIndex;
		nstring matName;
		nUInt32 matSubID;
		nUInt32 faceNo;
		nova::nReal faceNormal[3];
	} TFaceInfo;

	typedef struct FaceInfoToFaceABC
	{
		FaceInfoToFaceABC(size_t maxCount) : count(0)
		{
			mIndexes.resize(maxCount);
		}


		void operator()(TFaceInfo &face)
		{
			mIndexes[count] = face.faceIndex;
			count++;
		}
		
		stl<TFaceABC>::vector mIndexes;
	private:

		int count;
	} TFaceInfoToFaceABC;

#pragma pack(pop)

	typedef nova::stl<stVertex3d3n3uv_t>::vector TStVertexArray;
	typedef nova::stl<TFaceInfo>::vector TFacesInfoArray;
	typedef nova::stl<nInt32>::vector TSubMats, TMatChange;

	typedef struct _MeshContainer
	{
		nstring nName;
		nova::CColorRGB mMeshColor;
		TStVertexArray nVertexList;
		TFacesInfoArray nMeshInfoList;
		Matrix4f nTMatrix;
		nInt32 MatID;
// Reserved buffers
		//TIndexes nTVIndexList;
		//TTexCoords nTVMappingList;
		TMatChange nMatChangesGroups;
// For mesh internal loading
	} TMeshContainer;

	struct TIndexSortCmp
	{
		bool operator()(TFaceInfo &left, TFaceInfo &right);
	};

protected:

	void LoadResourceImpl(void);

	void FreeResourceImpl(void);

	void BuildResourceImpl(void);

	void SerializeToXmlFileImpl(xmlTextWriterPtr writer);

public:

	CMesh(CResourceManager * rm, const nstring & name, const nstring & group, TAttach state);
	~CMesh();

	TMeshContainer &GetMeshDefinition(void);

	void SetMeshDefinition(TMeshContainer *_mesh);

	void CalculateNormals(void/* Simple method calc normals or not? 
							  i want to use smoothing groups in future*/);

	CBoundingBox GenerateBoundingBox(void);

	void GenerateNormalsToFaces(void);

	void SortFaceIndexByMaterials(void);

	void GenerateMatChangesGroups(void);

	CHardwareVertexBufferPtr CreateVBO(void);

	CHardwareIndexBufferPtr CreateIBO(void);

	void SetSourcePackage(CFilesPackage *package);

	inline CFilesPackage *GetSourcePackage(void) const { return mPackage; }

	inline void SetMshFile(const nova::nstring &name) { mMeshfile = name; }

	inline nova::nstring GetMshFileName() const { return mMeshfile; }

private:

	CFilesPackage *mPackage;
	bool mPackageLoading;
	nstring mMeshfile;
	TMeshContainer mMeshDef;
};

typedef CSmartPtr<CMesh> CMeshBoxPtr;

class NOVA_EXPORT CMeshManager : public CSingelton<CMeshManager>, public CResourceManager
{
protected:

	CResourcePtr LoadResourceFromXmlNodeImpl(const nstring &name, const nstring &group, xmlNodePtr node);

	CResourcePtr LoadResourceFromXmlNodeImpl(const nstring &name, const nstring &group, xmlNodePtr node, const CFilesPackage &package);

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