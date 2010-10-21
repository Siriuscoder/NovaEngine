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

#include "nova_resource_manager.h"
#include "nova_string_utils.h"
#include "nova_threads.h"

namespace nova
{

CResource::CResource(CResourceManager * rm, const nstring & name, const nstring & group, TAttach state) :
	mState(state), CBase("CResource")
{
	mName = name;
	mGroup = group;
	isReady = false;
	mCreator = rm;
	mResourceHandle = this->GetObjectId();
}

void CResource::FreeResource()
{
	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CResourceListener * lis =
			dynamic_cast<CResourceListener *>(GetListener(i));
		lis->FreeResourceListener(this);
	}

	isReady = false;
}

void CResource::PreAddingAction()
{
	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CResourceListener * lis =
			dynamic_cast<CResourceListener *>(GetListener(i));
		lis->PreAddingActionListener(this);
	}
}

void CResource::PostAddingAction()
{
	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CResourceListener * lis =
			dynamic_cast<CResourceListener *>(GetListener(i));
		lis->PostAddindActionListener(this);
	}
}

void CResource::PreUnloadingAction()
{
	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CResourceListener * lis =
			dynamic_cast<CResourceListener *>(GetListener(i));
		lis->PreUnloadingActionListener(this);
	}
}

void CResource::PrepareResource(void)
{
	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CResourceListener * lis =
			dynamic_cast<CResourceListener *>(GetListener(i));
		lis->PrepareResourceListener(this);
	}
}

void CResource::BuildResource(void)
{
	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CResourceListener * lis =
			dynamic_cast<CResourceListener *>(GetListener(i));
		lis->BuildResourceListener(this);
	}

	isReady = true;
}

////////////////// Resource global hash //////////////////////////
stl<nstring, CResourcePtr>::map CResourceManager::mResourceHash;
////////////////// Resource global building queue ////////////////
CAsyncLockingQueue CResourceManager::mResourceBuildQueue;
/*CImageCodecsManager * CResourceManager::GetImageCodecsManager()
{
	return ImageCodecsManager;
}

*/
void CResourceManager::AddDefaultListener(CResourceListener * listener)
{
	mpDefListener = listener;
}

void CResourceManager::LoadResourceIntoHash(const CResourcePtr & resource)
{
	resource->PreAddingAction();

	RESOURCE_MUTEX_SECTION_LOCK;
	mResourceHash.insert(pair<nstring, CResourcePtr>(resource->GetResName(), resource));
	RESOURCE_MUTEX_SECTION_UNLOCK;

	resource->PostAddingAction();
}

void CResourceManager::UnloadResourceFromHash(const CResourcePtr & resource)
{
	stl<nstring, CResourcePtr>::map::iterator it;
	CResourcePtr res;

	RESOURCE_MUTEX_SECTION_LOCK;

	it = mResourceHash.find(resource->GetResName());
	if(it != mResourceHash.end())
	{
		res = (*it).second;
		mResourceHash.erase(it);
	}

	RESOURCE_MUTEX_SECTION_UNLOCK;

	if(!res.IsNull())
		res->PreUnloadingAction();
}

void CResourceManager::UnloadResourceFromHash(const nstring & name)
{
	stl<nstring, CResourcePtr>::map::iterator it;
	CResourcePtr res;

	RESOURCE_MUTEX_SECTION_LOCK;

	it = mResourceHash.find(name);
	if(it != mResourceHash.end())
	{
		res = (*it).second;
		mResourceHash.erase(it);
	}

	RESOURCE_MUTEX_SECTION_UNLOCK;

	if(!res.IsNull())
		res->PreUnloadingAction();
}

void CResourceManager::UnloadResourceGroupFromHash(const nstring & group)
{
	stl<nstring, CResourcePtr>::map::iterator it;
	stl<CResourcePtr>::vector res;

	RESOURCE_MUTEX_SECTION_LOCK;

	it = mResourceHash.begin();
	while(it != mResourceHash.end())
	{
		if( (*it).second->GetResGroupName() == group  )
		{
			//(*it).second->PreUnloadingAction();
			res.push_back((*it).second);
			mResourceHash.erase(it);
			it = mResourceHash.begin();
		}
		else
			it++;
	}

	RESOURCE_MUTEX_SECTION_UNLOCK;

	for(nova::nUInt32 i = 0; i < res.size(); ++i)
		if(!res[i].IsNull())
			res[i]->PreUnloadingAction();
	res.clear();
}

size_t CResourceManager::GetHashSize()
{
	size_t result = 0;
	RESOURCE_MUTEX_SECTION_LOCK;

	stl<nstring, CResourcePtr>::map::iterator it;
	for(it = mResourceHash.begin(); it !=  mResourceHash.end(); it++)
		result += (*it).second->GetSize();

	RESOURCE_MUTEX_SECTION_UNLOCK;

	return result;
}

void CResourceManager::UnloadAllResources()
{
	stl<nstring, CResourcePtr>::map::iterator it;
	stl<CResourcePtr>::vector snapshot;

	RESOURCE_MUTEX_SECTION_LOCK;

	for(it = mResourceHash.begin(); it !=  mResourceHash.end(); it++)
	{
		//(*it).second->PreUnloadingAction();
		//(*it).second->FreeResource();

		snapshot.push_back((*it).second);
	}
	mResourceHash.clear();

	RESOURCE_MUTEX_SECTION_UNLOCK;

	for(nova::nUInt32 i = 0; i < snapshot.size(); ++i)
		if(!snapshot[i].IsNull())
		{
			snapshot[i]->PreUnloadingAction();
		}

	snapshot.clear();
}

void CResourceManager::FreeAllResource()
{
	stl<nstring, CResourcePtr>::map::iterator it;
	stl<CResourcePtr>::vector snapshot;

	RESOURCE_MUTEX_SECTION_LOCK;

	for(it = mResourceHash.begin(); it !=  mResourceHash.end(); it++)
	{
		//(*it).second->PreUnloadingAction();
		//(*it).second->FreeResource();

		snapshot.push_back((*it).second);
	}

	mResourceHash.clear();

	RESOURCE_MUTEX_SECTION_UNLOCK;

	//UnloadAllResources();
	for(nova::nUInt32 i = 0; i < snapshot.size(); ++i)
		if(!snapshot[i].IsNull())
		{
			snapshot[i]->PreUnloadingAction();
			snapshot[i]->FreeResource();
		}

	snapshot.clear();
}

void CResourceManager::UnloadResourceFromHash(CResourceManager * rm)
{
	stl<nstring, CResourcePtr>::map::iterator it;
	stl<CResourcePtr>::vector res;

	RESOURCE_MUTEX_SECTION_LOCK;

	it = mResourceHash.begin();
	while(it != mResourceHash.end())
	{
		if( (*it).second->GetCreator() == rm  )
		{
			//(*it).second->PreUnloadingAction();
			res.push_back((*it).second);
			mResourceHash.erase(it);
			it = mResourceHash.begin();
		}
		else
			it++;
	}

	RESOURCE_MUTEX_SECTION_UNLOCK;

	for(nova::nUInt32 i = 0; i < res.size(); ++i)
		if(!res[i].IsNull())
			res[i]->PreUnloadingAction();

	res.clear();
}

void CResourceManager::UnloadResourceFromHash(nova::nUInt32 handle)
{
	stl<nstring, CResourcePtr>::map::iterator it;
	stl<CResourcePtr>::vector res;

	RESOURCE_MUTEX_SECTION_LOCK;

	it = mResourceHash.begin();
	while(it != mResourceHash.end())
	{
		if( (*it).second->GetHandle() == handle  )
		{
			//(*it).second->PreUnloadingAction();
			res.push_back((*it).second);
			mResourceHash.erase(it);
			break;
		}
		else
			it++;
	}

	RESOURCE_MUTEX_SECTION_UNLOCK;

	for(nova::nUInt32 i = 0; i < res.size(); ++i)
		if(!res[i].IsNull())
			res[i]->PreUnloadingAction();

	res.clear();
}

bool CResourceManager::CompareResourceCollision(const CResourcePtr & resource)
{
	bool res = false;
	RESOURCE_MUTEX_SECTION_LOCK;

	stl<nstring, CResourcePtr>::map::iterator it;
	it = mResourceHash.find(resource->GetResName());
	if(it != mResourceHash.end())
		res = true;

/*	for(it = mResourceHash.begin(); it !=  mResourceHash.end(); it++)
		if( (*it).second->GetHandle() == resource->GetHandle() )
			return true;
*/
	RESOURCE_MUTEX_SECTION_UNLOCK;

	return res;
}

CResourcePtr CResourceManager::AddNewResource(const nstring & name,
	const nstring & group, CResource::TAttach state)
{
	CResourcePtr ptr = CreateInstance(name, group, state);

	if(mpDefListener != NULL)
		ptr->AddListener(mpDefListener);

	if(CompareResourceCollision(ptr))
		throw NOVA_EXP("This resource already exist in hash...", BAD_OPERATION);

	if(state == CResource::NV_ATTACHED)
	{
		LoadResourceIntoHash(ptr);
		return ptr;
	}
	else if(state == CResource::NV_FREE)
		return ptr;

	return CResourcePtr();
}

CResourcePtr CResourceManager::GetResourceFromHash(const nstring & name)
{
	stl<nstring, CResourcePtr>::map::iterator it;
	CResourcePtr res;
	RESOURCE_MUTEX_SECTION_LOCK;

	it = mResourceHash.find(name);
	if(it != mResourceHash.end())
		res = (*it).second;

	RESOURCE_MUTEX_SECTION_UNLOCK;

	return res;
}

CResourcePtr CResourceManager::GetResourceFromHash(nova::nUInt32 handle)
{
	stl<nstring, CResourcePtr>::map::iterator it;
	CResourcePtr res;
	RESOURCE_MUTEX_SECTION_LOCK;

	for(it = mResourceHash.begin(); it != mResourceHash.end(); it++)
		if( (*it).second->GetHandle() == handle  )
			res = (*it).second;

	RESOURCE_MUTEX_SECTION_UNLOCK;

	return res;
}

void CResourceManager::BuildNextResource(const nstring & name)
{
	CResourcePtr res = GetResourceFromHash(name);
	if(!res.IsNull())
		res->BuildResource();
}

void CResourceManager::SetResourceLocation(const nstring & path)
{
	nstring mes("Set resource location: ");
	LOG_MESSAGE(mes + path);
	mResourceLocation = path;
}

nstring CResourceManager::GetResourceLocation()
{
	return mResourceLocation;
}

CAsyncLockingQueue::CAsyncLockingQueue()
{
	mQueue.clear();
	int res = pthread_mutex_init(&mLockMutex, NULL);
	if(res != 0)
	{
		nstring er("CAsyncLockingQueue::CAsyncLockingQueue() - can not create mutex section code: ");
		er.append(CStringUtils::IntTo16xString(res));
		throw NOVA_EXP(er.c_str(), BAD_OPERATION);
	}
}

CAsyncLockingQueue::~CAsyncLockingQueue()
{
	ClearQueue();
	pthread_mutex_destroy(&mLockMutex);
}

int CAsyncLockingQueue::ProccesQueue(void)
{
	int res = 0;
	pthread_mutex_lock(&mLockMutex);

	for(nova::nUInt32 i = 0; i < mQueue.size(); ++i)
	{
		mQueue[i]->BuildResource();
		res++;
	}
	mQueue.clear();

	pthread_mutex_unlock(&mLockMutex);

	return res;
}

void CAsyncLockingQueue::AddToQueue(CResource *resource)
{
	pthread_mutex_lock(&mLockMutex);
	mQueue.push_back(resource);
	pthread_mutex_unlock(&mLockMutex);
}

void CAsyncLockingQueue::ClearQueue(void)
{
	pthread_mutex_lock(&mLockMutex);
	mQueue.clear();
	pthread_mutex_unlock(&mLockMutex);
}

int CResourceManager::LoadResourcesForce(const CFilesPackage &rPack)
{
	return 0;
}

int CResourceManager::LoadResourcesInBackgroundMode(const CFilesPackage &rPack)
{
	return 0;
}

}
