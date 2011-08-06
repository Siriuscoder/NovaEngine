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

#include "nova_mesh_box.h"
#include "nova_msh_loader.h"

namespace nova
{

CMesh::CMesh(CResourceManager * rm, const nstring & name, const nstring & group, TAttach state) :
	CResource(rm, name, group, state), mPackage(NULL)
{
	memset(&mMeshDef, 0, sizeof(TMeshContainer));
	mMeshDef.nTMatrix = Matrix4f::IDENTITY;
}

CMesh::~CMesh()
{
	//this->FreeResource();

	nova::nstringstream str;
	str << "CMesh::~CMesh() " << "name: " << mName << " group: " << mGroup;
	LOG_MESSAGE(str.str());
}

void CMesh::CalculateNormals(void/* Simple method */)
{
	if(mMeshDef.nMeshInfoList.size() == 0)
		return ;

	typedef stl< stl<nInt32>::vector >::vector t_expl;
	t_expl expl;

	expl.resize(mMeshDef.nVertexList.size());
	for(nova::nUInt32 i = 0; i < mMeshDef.nMeshInfoList.size(); ++i)
	{
		expl[mMeshDef.nMeshInfoList[i].faceIndex.a].push_back(i);
		expl[mMeshDef.nMeshInfoList[i].faceIndex.b].push_back(i);
		expl[mMeshDef.nMeshInfoList[i].faceIndex.c].push_back(i);
	}

    for (nova::nUInt32 i = 0; i < mMeshDef.nVertexList.size(); ++i)
    {
		//memset(&normal, 0, sizeof(TNormal3d));
		Vector3f vec;
		nova::nUInt32 t = expl[i].size();

        for (nova::nUInt32 k = 0; k < t; ++k)
        {
			vec += Vector3f(mMeshDef.nMeshInfoList[expl[i][k]].faceNormal);
        }

		vec.Normalize();

		mMeshDef.nVertexList[i].nx = vec.X();
		mMeshDef.nVertexList[i].ny = vec.Y();
		mMeshDef.nVertexList[i].nz = vec.Z();
    }


	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CMeshBoxListener * lis =
			dynamic_cast<CMeshBoxListener *>(GetListener(i));
		lis->CalculateNormalsListener(this);
	}
}

void CMesh::SetSourcePackage(CFilesPackage *package)
{
	if(package)
	{
		mPackage = package;
		mPackageLoading = true;
	}
}

CMesh::TMeshContainer &CMesh::GetMeshDefinition(void)
{
	return mMeshDef;
}

void CMesh::SetMeshDefinition(TMeshContainer *_mesh)
{
	if(_mesh)
		mMeshDef = *_mesh;
}

bool CMesh::TIndexSortCmp::operator()(TFaceInfo &left, TFaceInfo &right)
{
	return (left.matSubID < right.matSubID);
}

void CMesh::SortFaceIndexByMaterials(void)
{
	TIndexSortCmp cmp;
	//QSortFaces(mMeshDef.nIndexList, mMeshDef.nMeshInfoList);
	std::sort(mMeshDef.nMeshInfoList.begin(), mMeshDef.nMeshInfoList.end(), cmp);
}

CBoundingBox CMesh::GenerateBoundingBox(void)
{
	CBoundingBox testBox;

	nova::nReal xmax = mMeshDef.nVertexList[0].x,
		xmin = mMeshDef.nVertexList[0].x,
		ymax = mMeshDef.nVertexList[0].y,
		ymin = mMeshDef.nVertexList[0].y,
		zmax = mMeshDef.nVertexList[0].z,
		zmin = mMeshDef.nVertexList[0].z;

	TStVertexArray::iterator it = mMeshDef.nVertexList.begin()+1;
	for(; it != mMeshDef.nVertexList.end(); ++it)
	{
		xmax = std::max((*it).x, xmax);
		xmin = std::min((*it).x, xmin);
		ymax = std::max((*it).y, ymax);
		ymin = std::min((*it).y, ymin);
		zmax = std::max((*it).z, zmax);
		zmin = std::min((*it).z, zmin);
	}

	nova::Vector3f maxPoint(xmax, ymax, zmax);
	nova::Vector3f minPoint(xmin, ymin, zmin);

	testBox.CalcSides(minPoint, maxPoint);


	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CMeshBoxListener * lis =
			dynamic_cast<CMeshBoxListener *>(GetListener(i));
		lis->GenerateBoundingBoxListener(this);
	}

	return testBox;
}

CHardwareVertexBufferPtr CMesh::CreateVBO(void)
{
	CMemoryBuffer buffer(&(mMeshDef.nVertexList[0]), mMeshDef.nVertexList.size() * sizeof(stVertex3d3n3uv_t));
	return CHardwareVertexBufferPtr(new CHardwareVertexBuffer(buffer, HWB_STATIC));
}

CHardwareIndexBufferPtr CMesh::CreateIBO(void)
{
	TFaceInfoToFaceABC funct(mMeshDef.nMeshInfoList.size());
	std::for_each(mMeshDef.nMeshInfoList.begin(), mMeshDef.nMeshInfoList.end(), funct);

	CMemoryBuffer buffer(&(funct.mIndexes[0]), funct.mIndexes.size() * sizeof(TFaceABC));
	return CHardwareIndexBufferPtr(new CHardwareIndexBuffer(buffer, HWB_STATIC));
}

void CMesh::FreeResourceImpl()
{
	mMeshDef.nVertexList.clear();
	mMeshDef.nMeshInfoList.clear();
}

void CMesh::LoadResourceImpl(void)
{
/*
	if(!CheckValidLength())
		throw NOVA_EXP("CMesh::BuildResource - detected incorrect \
		mesh size.. do not resolved the conflict((", BAD_OPERATION);
*/

	// Вычисляем нормальное положение объекта в мировой системе координат
	//ToWorldCoord();
	// Вычисляем индексы и нормали к граням
	//CreateInfo();
	// Вычисляем нормали к вершинам
	//CalculateNormals();
	// Сортируем вершины по материалам

	if(mPackageLoading && mPackage)
	{
// use to load mesh with internal format 
		CMemoryBuffer fileBuf = mPackage->GetFile(mMeshfile);
		if(fileBuf.GetBegin())
		{
			CMemoryStream fileStream;
			fileStream.Open(fileBuf);
			mMeshDef = CGlobalMshLoader::LoadMeshFromStream(&fileStream);

			fileStream.Close();
		}

		fileBuf.FreeBuffer();
	}
	else
	{
		if(!mMeshfile.empty())
		{
			mMeshDef = CGlobalMshLoader::LoadMeshFromFile(mMeshfile);
		}
	}
}

void CMesh::BuildResourceImpl(void)
{
	mSize += mMeshDef.nVertexList.size() * sizeof(stVertex3d3n3uv_t);
	mSize += mMeshDef.nMeshInfoList.size() * sizeof(TFaceInfo);

}

void CMesh::GenerateMatChangesGroups(void)
{
	TFacesInfoArray::iterator it;
	nova::nInt32 group = -1;
	int count = 0;

	mMeshDef.nMatChangesGroups.clear();
	for(it = mMeshDef.nMeshInfoList.begin(); it != mMeshDef.nMeshInfoList.end(); it++, count++)
	{
		if(group != it->matSubID)
		{
			mMeshDef.nMatChangesGroups.push_back(count);
			group = it->matSubID;
		}
	}
}

void CMesh::GenerateNormalsToFaces(void)
{
	TFacesInfoArray::iterator it;
	it = mMeshDef.nMeshInfoList.begin();

	try
	{
		for(; it != mMeshDef.nMeshInfoList.end(); ++it)
		{
			Vector3f a, b, normal;

			a = Vector3f(mMeshDef.nVertexList[(*it).faceIndex.b].x, 
							mMeshDef.nVertexList[(*it).faceIndex.b].y, 
							mMeshDef.nVertexList[(*it).faceIndex.b].z) -
				Vector3f(mMeshDef.nVertexList[(*it).faceIndex.a].x, 
							mMeshDef.nVertexList[(*it).faceIndex.a].y, 
							mMeshDef.nVertexList[(*it).faceIndex.a].z);

			b = Vector3f(mMeshDef.nVertexList[(*it).faceIndex.b].x,
							mMeshDef.nVertexList[(*it).faceIndex.b].y,
							mMeshDef.nVertexList[(*it).faceIndex.b].z) -
				Vector3f(mMeshDef.nVertexList[(*it).faceIndex.c].x,
							mMeshDef.nVertexList[(*it).faceIndex.c].y,
							mMeshDef.nVertexList[(*it).faceIndex.c].z);

			normal = a.Cross(b);
			normal.Normalize();

			Vector3f(it->faceNormal) = normal;
		}
	}
	catch(std::exception & ex)
	{
		nstring str("CMesh::CreateInfo - rased std::exception: ");
		str = str + ex.what();
		throw NOVA_EXP(str.c_str(), BAD_OPERATION);
	}
}

void CMesh::SerializeToXmlFileImpl(xmlTextWriterPtr writer)
{
	if(xmlTextWriterWriteElement(writer, BAD_CAST "Meshfile", BAD_CAST mMeshfile.c_str()) < 0)
		NOVA_EXP("CMesh::SerializeToXmlFileImpl: xmlTextWriterWriteElement fail", BAD_OPERATION);
}

template<> CMeshManager * CSingelton<CMeshManager>::SingeltonObject = NULL;

CMeshManager::CMeshManager(const nstring & resourceFactoryName) : CResourceManager(resourceFactoryName)
{

}

CMeshManager::~CMeshManager()
{
	UnloadAllManagerResources();

}

void CMeshManager::UnloadAllManagerResources()
{
	UnloadResourceFromHash(this);
}

CResourcePtr CMeshManager::CreateInstance(const nstring & name,
		const nstring & group, CResource::TAttach state)
{
	CResourcePtr ptr(new CMesh(this, name, group, state));
	return ptr;
}

CMeshBoxPtr CMeshManager::CreateMesh(CMesh::TMeshContainer *def, const nstring &group,
		CResource::TAttach state)
{
	CMeshBoxPtr mesh = CResourceManager::AddNewResource(def->nName, group, state);
	if(mesh.IsNull())
		throw NOVA_EXP("CMeshManager::CreateMesh - resource factory return \
							Null pointer...", MEM_ERROR);

	mesh->SetMeshDefinition(def);
	//mesh->PrepareResource();

	nova::nstringstream str;
	str << "Mesh Factory: " << def->nName << " (group: " << group << ") created";
	LOG_MESSAGE(str.str());

	return mesh;
}

CResourcePtr CMeshManager::LoadResourceFromXmlNodeImpl(const nstring &name, const nstring &group, xmlNodePtr node)
{
	if(!node)
		return CResourcePtr();

	CMesh::TMeshContainer meshStruct;
	nova::nstring meshFile;
	while(node != NULL)
	{
		if(xmlIsBlankNode(node))
		{
			node = node->next;
			continue;
		}

		if(!xmlStrcmp(node->name, (xmlChar *) "Meshfile"))
			meshFile.append(reinterpret_cast<char *>(node->children->content));

		node = node->next;
	}

	meshStruct.nName = name;
	CMeshBoxPtr meshPtr = CreateMesh(&meshStruct, group);
	meshPtr->SetMshFile(meshFile);

	return meshPtr;
}

CResourcePtr CMeshManager::LoadResourceFromXmlNodeImpl(const nstring &name, const nstring &group, xmlNodePtr node, const CFilesPackage &package)
{
	if(!node)
		return CResourcePtr();

	CMesh::TMeshContainer meshStruct;
	nova::nstring meshFile;
	while(node != NULL)
	{
		if(xmlIsBlankNode(node))
		{
			node = node->next;
			continue;
		}

		if(!xmlStrcmp(node->name, (xmlChar *) "Meshfile"))
			meshFile.append(reinterpret_cast<char *>(node->children->content));

		node = node->next;
	}

	meshStruct.nName = name;
	CMeshBoxPtr meshPtr = CreateMesh(&meshStruct, group);
	meshPtr->SetSourcePackage(const_cast<CFilesPackage *>(&package));
	meshPtr->SetMshFile(meshFile);

	return meshPtr;
}

}
