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

#include "nova_error.h"
#include "nova_smart_ptr.h"
#include "nova_event_listener.h"
#include "nova_resource_package.h"

namespace nova
{

class CResourceManager;
class CResource;

class NOVA_EXPORT CResourceListener : public CEventListener
{
public:

	virtual void PreAddingActionListener(CResource * object) {}

	virtual void PostAddindActionListener(CResource * object) {}

	virtual void PreUnloadingActionListener(CResource * object) {}

	virtual void FreeResourceListener(CResource * object) {}

	virtual void PreLoadResourceListener(CResource * object) {}

	virtual void PostLoadResourceListener(CResource * object) {}

	virtual void PreBuildResourceListener(CResource * object) {}

	virtual void PostBuildResourceListener(CResource * object) {}
};

class NOVA_EXPORT CResource : public CBase, public CListenerInterface
{
public:

	enum TAttach
	{
		NV_ATTACHED,
		NV_FREE
	};

protected:

	nova::nUInt32 mResourceHandle;
	nstring mName;
	nstring mGroup;
	TAttach mState;

	CResourceManager * mCreator;
	nova::nUInt32 mSize;
	bool isReady;
	bool isLoaded;
	bool isBuilded;

	virtual void LoadResourceImpl(void) = 0;

	virtual void FreeResourceImpl(void) = 0;

	virtual void BuildResourceImpl(void) = 0;

public:

	CResource(CResourceManager * rm, const nstring & name, const nstring & group, TAttach state);
	virtual ~CResource() {}

	inline nstring GetResName() const
	{
		return mName;
	}

	inline TAttach GetAttachState() const
	{
		return mState;
	}


	inline nstring GetResGroupName() const
	{
		return mGroup;
	}

	inline nova::nUInt32 GetSize() const
	{
		return mSize;
	}

	inline nova::nUInt32 GetHandle() const
	{
		return mResourceHandle;
	}

	inline bool IsReady() const
	{
		return isReady;
	}

	inline bool IsLoaded() const
	{
		return isLoaded;
	}

	inline bool IsBuilded() const
	{
		return isBuilded;
	}

	inline CResourceManager * GetCreator() const
	{
		return mCreator;
	}

	void FreeResource(void);

	void PreAddingAction(void);

	void PostAddingAction(void);

	void PreUnloadingAction(void);

	void LoadResource(void);

	void BuildResource(void);

	void RebuildResource(void);
};

typedef CSmartPtr<CResource> CResourcePtr;

class NOVA_EXPORT CAsyncLockingQueue
{
private:

	stl<CResource *>::vector mQueue;
	pthread_mutex_t mLockMutex;

public:

	CAsyncLockingQueue();
	~CAsyncLockingQueue();

	nInt32 ProccesQueue(void);
	void AddToQueue(CResource *resource);

	void ClearQueue(void);
};

class NOVA_EXPORT CResourceManager : public CBase
{
protected:
	static stl<nstring, CResourcePtr>::map mResourceHash;
	static CAsyncLockingQueue mResourceBuildQueue;
//	CImageCodecsManager * ImageCodecsManager;
//	CTextureManager * TextureManager;
//	CImageManager * ImageManager;
	bool CompareResourceCollision(const CResourcePtr & resource);

	CResourceListener * mpDefListener;
	nstring mResourceLocation;

public:

	CResourceManager() : mpDefListener(NULL), CBase("CResourceManager") {}

	~CResourceManager() {}

	void SetResourceLocation(const nstring & path);

	nstring GetResourceLocation();

	void AddDefaultListener(CResourceListener * listener);

	void LoadResourceIntoHash(const CResourcePtr & resource);

	void UnloadResourceFromHash(const CResourcePtr & resource);

	void UnloadResourceFromHash(const nstring & name);

	void UnloadResourceGroupFromHash(const nstring & group);

	void UnloadResourceFromHash(nova::nUInt32 handle);

	static size_t GetHashSize();

	void UnloadAllResources();

	void FreeAllResource();

	void UnloadResourceFromHash(CResourceManager * rm);

	CResourcePtr GetResourceFromHash(const nstring & name);

	CResourcePtr GetResourceFromHash(nova::nUInt32 handle);

	CResourcePtr AddNewResource(const nstring & name,
		const nstring & group, CResource::TAttach state);

	virtual CResourcePtr CreateInstance(const nstring & name,
		const nstring & group, CResource::TAttach state) = 0;

	virtual void UnloadAllManagerResources() = 0;

	/* for force loading form packages */
	virtual CResourcePtr LoadResourceFromXml(const nstring &filename, const CFilesPackage &package) = 0;

	virtual CResourcePtr LoadResourceFromXml(const nstring &filename) = 0;

	nInt32 LoadResourcesForce(const CFilesPackage &rPack);

	nInt32 LoadResourcesInBackgroundMode(const CFilesPackage &rPack);

};

}
