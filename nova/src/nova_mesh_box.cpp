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
	CResource(rm, name, group, state)
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
	TNormal3d normal;
	mMeshDef.nNormalList.clear();

	expl.resize(mMeshDef.nVertexList.size());
	for(nova::nUInt32 i = 0; i < mMeshDef.nMeshInfoList.size(); ++i)
	{
		expl[mMeshDef.nIndexList[i].a].push_back(i);
		expl[mMeshDef.nIndexList[i].b].push_back(i);
		expl[mMeshDef.nIndexList[i].c].push_back(i);
	}

    for (nova::nUInt32 i = 0; i < mMeshDef.nVertexList.size(); ++i)
    {
		memset(&normal, 0, sizeof(TNormal3d));
		nova::nUInt32 t = expl[i].size();

        for (nova::nUInt32 k = 0; k < t; ++k)
        {
			normal.x += mMeshDef.nMeshInfoList[expl[i][k]].nFaceNormal.x;
			normal.y += mMeshDef.nMeshInfoList[expl[i][k]].nFaceNormal.y;
			normal.z += mMeshDef.nMeshInfoList[expl[i][k]].nFaceNormal.z;
        }

		Vector3f vec(&(normal.x));
		vec.Normalize();

		normal.x = vec.X();
		normal.y = vec.Y();
		normal.z = vec.Z();
		mMeshDef.nNormalList.push_back(normal);
    }

	for(nova::nUInt32 i = 0; i < expl.size(); ++i)
		expl[i].clear();
	expl.clear();

	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CMeshBoxListener * lis =
			dynamic_cast<CMeshBoxListener *>(GetListener(i));
		lis->CalculateNormalsListener(this);
	}
}

nInt32 CMesh::QComparer(const void * a, const void * b)
{
	TTriangleInfo *right = (TTriangleInfo *)b;
	TTriangleInfo *left = (TTriangleInfo *)a;

	return left->nMatSubID - right->nMatSubID;
}

void CMesh::QSortFaces(TIndexes &index, TFacesInfo &faces)
{
	qsort(&(faces[0]), faces.size(), sizeof(TTriangleInfo), QComparer);

	TIndexes tmp;
	tmp.resize(index.size());
	std::copy(index.begin(), index.end(), tmp.begin());
// переставляем элементы в массиве индексов
	for(nova::nUInt32 i = 0; i < faces.size(); i++)
	{
		index[i] = tmp[faces[i].nFace];
		faces[i].nFace = i;
	}

	tmp.clear();
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

void CMesh::SortFaceIndexByMaterials(void)
{
	QSortFaces(mMeshDef.nIndexList, mMeshDef.nMeshInfoList);
}

CBoundingBox CMesh::GenerateBoundingBox(void)
{
	CBoundingBox testbox;

	nova::nReal xmax = mMeshDef.nVertexList[0].x,
		xmin = mMeshDef.nVertexList[0].x,
		ymax = mMeshDef.nVertexList[0].y,
		ymin = mMeshDef.nVertexList[0].y,
		zmax = mMeshDef.nVertexList[0].z,
		zmin = mMeshDef.nVertexList[0].z;

	TVertexes::iterator it = mMeshDef.nVertexList.begin()+1;
	for(; it != mMeshDef.nVertexList.end(); ++it)
	{
		xmax = std::max((*it).x, xmax);
		xmin = std::min((*it).x, xmin);
		ymax = std::max((*it).y, ymax);
		ymin = std::min((*it).y, ymin);
		zmax = std::max((*it).z, zmax);
		zmin = std::min((*it).z, zmin);
	}

	nova::Vector3f maxpoint(xmax, ymax, zmax);
	nova::Vector3f minpoint(xmin, ymin, zmin);

	testbox.CalcSides(minpoint, maxpoint);


	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CMeshBoxListener * lis =
			dynamic_cast<CMeshBoxListener *>(GetListener(i));
		lis->GenerateBoundingBoxListener(this);
	}

	return testbox;
}

void CMesh::FreeResourceImpl()
{

}



void CMesh::ToWorldCoord()
{
	for(nUInt32 i = 0; i < mMeshDef.nVertexList.size(); i++)
	{
		nova::Vector4f vec(mMeshDef.nVertexList[i].x, mMeshDef.nVertexList[i].y, mMeshDef.nVertexList[i].z, 0);
		vec = vec * mMeshDef.nTMatrix;

		mMeshDef.nVertexList[i].x = vec.X();
		mMeshDef.nVertexList[i].y = vec.Y();
		mMeshDef.nVertexList[i].z = vec.Z();
	}
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

	if(mMeshDef.nPackageLoading && mMeshDef.pPackage)
	{
// use to load mesh with internal format 
		CMemoryBuffer fileBuf = mMeshDef.pPackage->GetFile(mMeshDef.nMeshfile);
		if(fileBuf.GetBegin())
		{
			CMemoryStream fileStream;
			fileStream.Open(fileBuf);
			mMeshDef = CGlobalMshLoader::LoadMeshFromStream(&fileStream);

			fileStream.Close();
		}

		mMeshDef.nPackageLoading = true;
	}
	else
	{
		mMeshDef = CGlobalMshLoader::LoadMeshFromFile(mMeshDef.nMeshfile);
		mMeshDef.nPackageLoading = false;
	}
}

void CMesh::BuildResourceImpl(void)
{
	mSize += mMeshDef.nVertexList.size() * sizeof(TVertex3d);
	mSize += mMeshDef.nNormalList.size() * sizeof(TNormal3d);
	mSize += mMeshDef.nMappingFacesList.size() * sizeof(TUVMapping);
	mSize += mMeshDef.nIndexList.size() * sizeof(TFaceIndex);
	mSize += mMeshDef.nMeshInfoList.size() * sizeof(TTriangleInfo);

}

bool CMesh::CheckValidLength()
{
	if(mMeshDef.nVertexList.size() == mMeshDef.nNormalList.size() == mMeshDef.nMappingFacesList.size())
		return true;

	return false;
}

void * CMesh::GetVertexPointer(size_t *count)
{
	*count = mMeshDef.nVertexList.size();
	if(*count > 0)
	{
		return &(mMeshDef.nVertexList[0]);
	}

	return NULL;
}

void * CMesh::GetNormalsPointer(size_t *count)
{
	*count = mMeshDef.nNormalList.size();
	if(*count > 0)
	{
		return &(mMeshDef.nNormalList[0]);
	}

	return NULL;
}

void * CMesh::GetUVPointer(size_t *count)
{
	*count = mMeshDef.nMappingFacesList.size();
	if(*count > 0)
	{
		return &(mMeshDef.nMappingFacesList[0]);
	}

	return NULL;
}

void * CMesh::GetIndexesPointer(size_t *count)
{
	*count = mMeshDef.nIndexList.size();
	if(*count > 0)
	{
		return &(mMeshDef.nIndexList[0]);
	}

	return NULL;
}

void CMesh::GenerateNormalsToFaces(void)
{
	TFacesInfo::iterator it;
	it = mMeshDef.nMeshInfoList.begin();

	try
	{
		for(nInt32 i = 0; it != mMeshDef.nMeshInfoList.end(); ++it, ++i)
		{
			Vector3f a, b, normal;

			a = Vector3f(mMeshDef.nVertexList[mMeshDef.nIndexList[(*it).nFace].b].x, 
							mMeshDef.nVertexList[mMeshDef.nIndexList[(*it).nFace].b].y, 
							mMeshDef.nVertexList[mMeshDef.nIndexList[(*it).nFace].b].z) -
				Vector3f(mMeshDef.nVertexList[mMeshDef.nIndexList[(*it).nFace].a].x, 
							mMeshDef.nVertexList[mMeshDef.nIndexList[(*it).nFace].a].y, 
							mMeshDef.nVertexList[mMeshDef.nIndexList[(*it).nFace].a].z);

			b = Vector3f(mMeshDef.nVertexList[mMeshDef.nIndexList[(*it).nFace].b].x, 
							mMeshDef.nVertexList[mMeshDef.nIndexList[(*it).nFace].b].y, 
							mMeshDef.nVertexList[mMeshDef.nIndexList[(*it).nFace].b].z) -
				Vector3f(mMeshDef.nVertexList[mMeshDef.nIndexList[(*it).nFace].c].x, 
							mMeshDef.nVertexList[mMeshDef.nIndexList[(*it).nFace].c].y, 
							mMeshDef.nVertexList[mMeshDef.nIndexList[(*it).nFace].c].z);

			normal = a.Cross(b);
			normal.Normalize();

			mMeshDef.nMeshInfoList[i].nFaceNormal.x = normal.X();
			mMeshDef.nMeshInfoList[i].nFaceNormal.y = normal.Y();
			mMeshDef.nMeshInfoList[i].nFaceNormal.z = normal.Z();
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
	if(xmlTextWriterWriteElement(writer, BAD_CAST "Meshfile", BAD_CAST mMeshDef.nMeshfile.c_str()) < 0)
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
	while(node != NULL)
	{
		if(xmlIsBlankNode(node))
		{
			node = node->next;
			continue;
		}

		if(!xmlStrcmp(node->name, (xmlChar *) "Meshfile"))
			meshStruct.nMeshfile.append(reinterpret_cast<char *>(node->children->content));

		node = node->next;
	}

	meshStruct.pPackage = NULL;
	meshStruct.nPackageLoading = false;

	CMeshBoxPtr meshPtr = CreateMesh(&meshStruct, "default");

	return meshPtr;
}

CResourcePtr CMeshManager::LoadResourceFromXmlNodeImpl(const nstring &name, const nstring &group, xmlNodePtr node, const CFilesPackage &package)
{
	if(!node)
		return CResourcePtr();

	CMesh::TMeshContainer meshStruct;
	while(node != NULL)
	{
		if(xmlIsBlankNode(node))
		{
			node = node->next;
			continue;
		}

		if(!xmlStrcmp(node->name, (xmlChar *) "Meshfile"))
			meshStruct.nMeshfile.append(reinterpret_cast<char *>(node->children->content));

		node = node->next;
	}

	meshStruct.pPackage = const_cast<CFilesPackage *>(&package);
	meshStruct.nPackageLoading = true;

	CMeshBoxPtr meshPtr = CreateMesh(&meshStruct, "default");

	return meshPtr;
}

}
