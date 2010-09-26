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

	virtual void PrepareResourceListener(CResource * object) {}

	virtual void BuildResourceListener(CResource * object) {}
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

	nova::uint mResourceHandle;
	nstring mName;
	nstring mGroup;
	TAttach mState;

	CResourceManager * mCreator;
	nova::uint mSize;
	bool isReady;

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

	inline nova::uint GetSize() const
	{
		return mSize;
	}

	inline nova::uint GetHandle() const
	{
		return mResourceHandle;
	}

	inline bool Ready() const
	{
		return isReady;
	}

	inline CResourceManager * GetCreator() const
	{
		return mCreator;
	}

	virtual void FreeResource(void);

	virtual void PreAddingAction(void);
	virtual void PostAddingAction(void);

	virtual void PreUnloadingAction(void);

	virtual void PrepareResource(void);

	virtual void BuildResource(void);
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

	int ProccesQueue(void);
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

	void UnloadResourceFromHash(nova::uint handle);

	static size_t GetHashSize();

	void UnloadAllResources();

	void FreeAllResource();

	void UnloadResourceFromHash(CResourceManager * rm);

	CResourcePtr GetResourceFromHash(const nstring & name);

	CResourcePtr GetResourceFromHash(nova::uint handle);

	CResourcePtr AddNewResource(const nstring & name,
		const nstring & group, CResource::TAttach state);

	virtual CResourcePtr CreateInstance(const nstring & name,
		const nstring & group, CResource::TAttach state) = 0;

	virtual void UnloadAllManagerResources() = 0;

	void BuildNextResource(const nstring & name);

	int LoadResourcesForce(const CFilesPackage &rPack);

	int LoadResourcesInBackgroundMode(const CFilesPackage &rPack);

};

}
