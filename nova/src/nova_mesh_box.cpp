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

namespace nova
{

CMeshBox::CMeshBox(CResourceManager * rm, nstring & name, nstring & group, TAttach state) :
	CResource(rm, name, group, state)
{
	mVertexes.clear();
	mIndexes.clear();
	mNormals.clear();
	mTexCoords.clear();
	mInfo.clear();
	mSubMats.clear();
	mRotationMatrix = Matrix3f::IDENTITY;
	mRealPosition = Vector3f::ZERO;
}

CMeshBox::~CMeshBox()
{
	this->FreeResource();

	nova::nstringstream str;
	str << "CMeshBox::~CMeshBox() " << "name: " << mName << " group: " << mGroup;
	LOG_MESSAGE(str.str());
}

void CMeshBox::CalculateNormals(void/* Simple method */)
{
	if(mInfo.size() == 0)
		return ;

	typedef stl< stl<int>::vector >::vector t_expl;
	t_expl expl;
	TNormal3d normal;
	mNormals.clear();
	mNormals.resize(mVertexes.size());

	expl.resize(mVertexes.size());
	for(nova::uint i = 0; i < mInfo.size(); ++i)
	{
		expl[mIndexes[i].a].push_back(i);
		expl[mIndexes[i].b].push_back(i);
		expl[mIndexes[i].c].push_back(i);
	}

    for (nova::uint i = 0; i < mVertexes.size(); ++i)
    {
		memset(&normal, 0, sizeof(TNormal3d));
		nova::uint t = expl[i].size();

        for (nova::uint k = 0; k < t; ++k)
        {
			normal.x += mInfo[expl[i][k]].normal.x;
			normal.y += mInfo[expl[i][k]].normal.y;
			normal.z += mInfo[expl[i][k]].normal.z;
        }

		Vector3f vec(&(normal.x));
		vec.Normalize();

		normal.x = vec.X();
		normal.y = vec.Y();
		normal.z = vec.Z();
		mNormals.push_back(normal);
    }

	for(nova::uint i = 0; i < expl.size(); ++i)
		expl[i].clear();
	expl.clear();

	for(nova::uint i = 0; i < GetListenersCount(); i++)
	{
		CMeshBoxListener * lis =
			dynamic_cast<CMeshBoxListener *>(GetListener(i));
		lis->CalculateNormalsListener(this);
	}
}

nova::uint CMeshBox::GetVertexesLen(void)
{
	return mVertexes.size();
}

nova::uint CMeshBox::GetTrianglesLen(void)
{
	return mIndexes.size();
}

size_t CMeshBox::GetVertexesLenInBytes(void)
{
	return mVertexes.size() * sizeof(TVertex4d);
}

size_t CMeshBox::GetTrianglesLenInBytes(void)
{
	return mIndexes.size() * sizeof(TTriIndex);
}

CBoundingBox CMeshBox::GenerateBoundingBox(void)
{
	CBoundingBox testbox;

	nova::real xmax = mVertexes[0].x,
		xmin = mVertexes[0].x,
		ymax = mVertexes[0].y,
		ymin = mVertexes[0].y,
		zmax = mVertexes[0].z,
		zmin = mVertexes[0].z;

	TVertexes::iterator it = mVertexes.begin();
	for(; it != mVertexes.end(); ++it)
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


	for(nova::uint i = 0; i < GetListenersCount(); i++)
	{
		CMeshBoxListener * lis =
			dynamic_cast<CMeshBoxListener *>(GetListener(i));
		lis->GenerateBoundingBoxListener(this);
	}

	return testbox;
}

void CMeshBox::FreeResource()
{
	CResource::FreeResource();
	mVertexes.clear();
	mIndexes.clear();
	mNormals.clear();
	mTexCoords.clear();
	mInfo.clear();
	mSubMats.clear();
}

void CMeshBox::PreAddingAction()
{
	CResource::PreAddingAction();
}

void CMeshBox::PostAddingAction()
{
	CResource::PostAddingAction();
}

void CMeshBox::PreUnloadingAction()
{
	CResource::PreUnloadingAction();
}

void CMeshBox::ToWorldCoord()
{
	for(unsigned int i = 0; i < mVertexes.size(); i++)
	{
		nova::Vector3f vec(&(mVertexes[i].x));
		vec = vec * mRotationMatrix;
		vec = vec - Vector3f(mRealPosition.X(), mRealPosition.Y(), mRealPosition.Z());

		memcpy(&(mVertexes[i]), vec, sizeof(4 * sizeof(nova::real)));
	}
}

void CMeshBox::PrepareResource(void)
{
	if(!CheckValidLength())
		throw NOVA_EXP("CMeshBox::BuildResource - detected incorrect \
		mesh size.. do not resolved the conflict((", BAD_OPERATION);

	// Вычисляем нормальное положение объекта в мировой системе координат
	//ToWorldCoord();
	// Вычисляем индексы и нормали к граням
	//CreateInfo();
	// Вычисляем нормали к вершинам
	//CalculateNormals();
	// Сортируем вершины по материалам

	CResource::PrepareResource();
}

void CMeshBox::BuildResource(void)
{
	if(isReady)
		return;

	mSize += mVertexes.size() * sizeof(TVertex4d);
	mSize += mNormals.size() * sizeof(TNormal3d);
	mSize += mTexCoords.size() * sizeof(TTexCoord2d);
	mSize += mIndexes.size() * sizeof(TTriIndex);
	mSize += mInfo.size() * sizeof(TTriangleInfo);

	CResource::BuildResource();
}

void CMeshBox::FillMeshBuffer(CMemoryBuffer & vertexes)
{
	int chunksize = sizeof(TVertex4d);
	mVertexes.clear();

/*	while(vertexes.GetReadPos() < vertexes.GetStreamSize())
	{
		TVertex4d vertex;
		memcpy(&vertex, vertexes.GetReadPtr(), chunksize);

		vertexes.SetReadPos(vertexes.GetReadPos() + chunksize);
		mVertexes.push_back(vertex);
	}
*/
	size_t len = (size_t)(vertexes.GetBufferSize() / chunksize);
	mVertexes.resize(len);

	memcpy(&(mVertexes[0]), vertexes.GetBegin(), vertexes.GetBufferSize());
}

void CMeshBox::FillNornalBuffer(CMemoryBuffer & normals)
{
	int chunksize = sizeof(TNormal3d);
	mNormals.clear();

/*	while(normals.GetReadPos() < normals.GetStreamSize())
	{
		TNormal3d normal;
		memcpy(&normal, normals.GetReadPtr(), chunksize);

		normals.SetReadPos(normals.GetReadPos() + chunksize);
		mNormals.push_back(normal);
	}
*/
	size_t len = (size_t)(normals.GetBufferSize() / chunksize);
	mNormals.resize(len);

	memcpy(&(mNormals[0]), normals.GetBegin(), normals.GetBufferSize());
}

void CMeshBox::FillTexCoordBuffer(CMemoryBuffer & coords)
{
	int chunksize = sizeof(TTexCoord2d);
	mTexCoords.clear();

/*	while(coords.GetReadPos() < coords.GetStreamSize())
	{
		TTexCoord2d coord;
		memcpy(&coord, coords.GetReadPtr(), chunksize);

		coords.SetReadPos(coords.GetReadPos() + chunksize);
		mTexCoords.push_back(coord);
	}
*/
	size_t len = (size_t)(coords.GetBufferSize() / chunksize);
	mTexCoords.resize(len);

	memcpy(&(mTexCoords[0]), coords.GetBegin(), coords.GetBufferSize());
}

void CMeshBox::FillIndexBuffer(CMemoryBuffer & indexes)
{
	int chunksize = sizeof(TTriIndex);
	mIndexes.clear();

/*	while(indexes.GetReadPos() < indexes.GetStreamSize())
	{
		TTriIndex triangle;
		memcpy(&triangle, indexes.GetReadPtr(), chunksize);

		indexes.SetReadPos(indexes.GetReadPos() + chunksize);
		mIndexes.push_back(triangle);
	}
*/

	size_t len = (size_t)(indexes.GetBufferSize() / chunksize);
	mIndexes.resize(len);

	memcpy(&(mIndexes[0]), indexes.GetBegin(), indexes.GetBufferSize());

	mSubMats.resize(mIndexes.size());
}

bool CMeshBox::CheckValidLength()
{
	if(mVertexes.size() == mNormals.size() == mTexCoords.size())
		return true;

	return false;
}

int CMeshBox::GetMaterialIDByName(nstring & name)
{
	for(nova::uint i = 0; i < mMatIndexes.size(); ++i)
		if(mMatIndexes[i] == name)
			return i;

	return -1;
}

int CMeshBox::AddNewSubMaterial(nstring & resource_name)
{
	if(GetMaterialIDByName(resource_name) < 0)
	{
		mMatIndexes.push_back(resource_name);
		return mMatIndexes.size() -1;
	}

	return -1;
}

int CMeshBox::GetMaterialID(nova::uint face)
{
	if(face >= mSubMats.size())
		return -1;

	return mSubMats[face];
}

void CMeshBox::SetMaterialID(nova::uint face, int id)
{
	if(face >= mSubMats.size())
		return;

	mSubMats[face] = id;
}

void * CMeshBox::GetMesh(void)
{
	if(mVertexes.size() > 0)
		return &(mVertexes[0]);

	return NULL;
}

void * CMeshBox::GetNormals(void)
{
	if(mNormals.size() > 0)
		return &(mNormals[0]);

	return NULL;
}

void * CMeshBox::GetTexCoord(void)
{
	if(mTexCoords.size() > 0)
		return &(mTexCoords[0]);

	return NULL;
}

void * CMeshBox::GetIndexes(void)
{
	if(mIndexes.size() > 0)
		return &(mIndexes[0]);

	return NULL;
}

TTriangleInfo CMeshBox::GetFaceInfo(nova::uint face)
{
	if(face >= mInfo.size())
		return TTriangleInfo();

	return mInfo[face];
}

void CMeshBox::GenarateNormalsToFaces(void)
{
	TIndexes::iterator it;
	it = mIndexes.begin();

	try
	{
		for(int i = 0; it != mIndexes.end(); ++it, ++i)
		{
			TTriangleInfo info;

			info.mat_id = mSubMats[i];
			info.tri_id = i;

			Vector3f a, b, normal;

			a = Vector3f(mVertexes[(*it).b].x, mVertexes[(*it).b].y, mVertexes[(*it).b].z) -
				Vector3f(mVertexes[(*it).a].x, mVertexes[(*it).a].y, mVertexes[(*it).a].z);
			b = Vector3f(mVertexes[(*it).b].x, mVertexes[(*it).b].y, mVertexes[(*it).b].z) -
				Vector3f(mVertexes[(*it).c].x, mVertexes[(*it).c].y, mVertexes[(*it).c].z);

			normal = a.Cross(b);
			normal.Normalize();

			info.normal.x = normal.X();
			info.normal.y = normal.Y();
			info.normal.z = normal.Z();

			mInfo.push_back(info);
		}
	}
	catch(std::exception & ex)
	{
		nstring str("CMeshBox::CreateInfo - rased std::exception: ");
		str = str + ex.what();
		throw NOVA_EXP(str.c_str(), BAD_OPERATION);
	}
}

void CMeshBox::SetRotationMatrix(const nova::Matrix3f & mat)
{
	mRotationMatrix = mat;
}

void CMeshBox::SetPosition(const nova::Vector3f & vec)
{
	mRealPosition = vec;
}

template<> CMeshManager * CSingelton<CMeshManager>::SingeltonObject = NULL;

CMeshManager::CMeshManager()
{
	LOG_MESSAGE("Mesh manager created..");
}

CMeshManager::~CMeshManager()
{
	UnloadAllManagerResources();
	LOG_MESSAGE("Mesh manager destroyed..");
}

void CMeshManager::UnloadAllManagerResources()
{
	UnloadResourceFromHash(this);
}

CResourcePtr CMeshManager::CreateInstance(nstring & name,
		nstring & group, CResource::TAttach state)
{
	CResourcePtr ptr(new CMeshBox(this, name, group, state));
	return ptr;
}

CMeshBoxPtr CMeshManager::CreateMesh(nstring & name, nstring & group,
		CMemoryBuffer & vertexes, CMemoryBuffer & normals,
		CMemoryBuffer & coords, CMemoryBuffer & indexes,
		stl<nstring>::vector & sub_mats, CMeshBox::TFacesInfo & mat_indexes,
		nova::Matrix3f & trans_mat, nova::Vector3f & trans_vec,
		CResource::TAttach state)
{
	CMeshBoxPtr mesh = CResourceManager::AddNewResource(name, group, state);
	if(mesh.IsNull())
		throw NOVA_EXP("CMeshManager::CreateMesh - resource factory return \
							Null pointer...", MEM_ERROR);

	mesh->FillMeshBuffer(vertexes);
	mesh->FillTexCoordBuffer(coords);
	mesh->FillIndexBuffer(indexes);
	if(normals.GetBufferSize() > 0)
		mesh->FillNornalBuffer(normals);

	for(nova::uint i = 0; i < sub_mats.size(); ++i)
		mesh->AddNewSubMaterial(sub_mats[i]);
	for(nova::uint i = 0; i < mat_indexes.size(); ++i)
		mesh->SetMaterialID(mat_indexes[i].tri_id, mat_indexes[i].mat_id);

	mesh->SetPosition(trans_vec);
	mesh->SetRotationMatrix(trans_mat);

	mesh->PrepareResource();
	CResourceManager::BuildNextResource(mesh->GetResName());

	nova::nstringstream str;
	str << "Mesh Factory: mesh object name: " << name << " group: " << group << " created...";
	LOG_MESSAGE(str.str());

	return mesh;
}

CMeshBoxPtr CMeshManager::CreateMeshAsync(nstring & name, nstring & group,
		CMemoryBuffer & vertexes, CMemoryBuffer & normals,
		CMemoryBuffer & coords, CMemoryBuffer & indexes,
		stl<nstring>::vector & sub_mats, CMeshBox::TFacesInfo & mat_indexes,
		nova::Matrix3f & trans_mat, nova::Vector3f & trans_vec,
		CResource::TAttach state)
{
	CMeshBoxPtr mesh = CResourceManager::AddNewResource(name, group, state);
	if(mesh.IsNull())
		throw NOVA_EXP("CMeshManager::CreateMesh - resource factory return \
							Null pointer...", MEM_ERROR);

	mesh->FillMeshBuffer(vertexes);
	mesh->FillTexCoordBuffer(coords);
	mesh->FillIndexBuffer(indexes);
	if(normals.GetBufferSize() > 0)
		mesh->FillNornalBuffer(normals);

	for(nova::uint i = 0; i < sub_mats.size(); ++i)
		mesh->AddNewSubMaterial(sub_mats[i]);
	for(nova::uint i = 0; i < mat_indexes.size(); ++i)
		mesh->SetMaterialID(mat_indexes[i].tri_id, mat_indexes[i].mat_id);

	mesh->SetPosition(trans_vec);
	mesh->SetRotationMatrix(trans_mat);

	mesh->PrepareResource();
	mResourceBuildQueue.AddToQueue(mesh.GetPtr());
	nova::nstringstream str;
	str << "Mesh Factory: mesh object name: " << name << " group: " << group << " async created...";
	LOG_MESSAGE(str.str());

	return mesh;
}

}
