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
	mState(state), isLoaded(false), isBuilded(false), mSize(0), CBase("CResource")
{
	mName = name;
	mGroup = group;
	isReady = false;
	mCreator = rm;
	mResourceHandle = this->GetObjectId();
}

void CResource::FreeResource()
{
	if(isReady || isLoaded || isBuilded)
	{
		for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
		{
			CResourceListener * lis =
				dynamic_cast<CResourceListener *>(GetListener(i));
			lis->FreeResourceListener(this);
		}

		FreeResourceImpl();
		isReady = false;
		isLoaded = false;
		isBuilded = false;
	}
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

void CResource::LoadResource(void)
{
	if(isLoaded)
		return;

	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CResourceListener * lis =
			dynamic_cast<CResourceListener *>(GetListener(i));
		lis->PreLoadResourceListener(this);
	}

	LoadResourceImpl();
	isLoaded = true;

	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CResourceListener * lis =
			dynamic_cast<CResourceListener *>(GetListener(i));
		lis->PostLoadResourceListener(this);
	}
}

void CResource::BuildResource(void)
{
	if(isBuilded || isReady)
		return;

	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CResourceListener * lis =
			dynamic_cast<CResourceListener *>(GetListener(i));
		lis->PreBuildResourceListener(this);
	}

	BuildResourceImpl();
	isReady = true;
	isBuilded = true;

	for(nova::nUInt32 i = 0; i < GetListenersCount(); i++)
	{
		CResourceListener * lis =
			dynamic_cast<CResourceListener *>(GetListener(i));
		lis->PostBuildResourceListener(this);
	}
}

void CResource::RebuildResource(void)
{
	FreeResource();
	LoadResource();
	BuildResource();
}

void CResource::SerializeToXmlFile(const nstring &file)
{
    xmlTextWriterPtr writer = NULL;

    /* Create a new XmlWriter for uri, with no compression. */
    if((writer = xmlNewTextWriterFilename(file.c_str(), 0)) == NULL)
		NOVA_EXP("CResource::SerializeToXmlFile: Error creating the xml writer", BAD_OPERATION);

	// automatic indentation for readability
	xmlTextWriterSetIndent(writer, 1);

	if(xmlTextWriterStartDocument(writer, NULL, "UTF-8", NULL) < 0)
		NOVA_EXP("CResource::SerializeToXmlFile: xmlTextWriterStartDocument fail", BAD_OPERATION);

	if(xmlTextWriterStartElement(writer, BAD_CAST "NovaResource") < 0)
		NOVA_EXP("CResource::SerializeToXmlFile: xmlTextWriterStartElement fail", BAD_OPERATION);

    if(xmlTextWriterWriteComment(writer, BAD_CAST "Common resource header") < 0)
		NOVA_EXP("CResource::SerializeToXmlFile: xmlTextWriterWriteComment fail", BAD_OPERATION);

	if(xmlTextWriterStartElement(writer, BAD_CAST "ResourceHeader") < 0)
		NOVA_EXP("CResource::SerializeToXmlFile: xmlTextWriterStartElement fail", BAD_OPERATION);

	if(xmlTextWriterWriteElement(writer, BAD_CAST "ResourceName", BAD_CAST mName.c_str()) < 0)
		NOVA_EXP("CResource::SerializeToXmlFile: xmlTextWriterWriteElement fail", BAD_OPERATION);

	if(xmlTextWriterWriteElement(writer, BAD_CAST "ResourceGroup", BAD_CAST mGroup.c_str()) < 0)
		NOVA_EXP("CResource::SerializeToXmlFile: xmlTextWriterWriteElement fail", BAD_OPERATION);

	if(xmlTextWriterStartElement(writer, BAD_CAST "ResourceData") < 0)
		NOVA_EXP("CResource::SerializeToXmlFile: xmlTextWriterStartElement fail", BAD_OPERATION);

// Resource manager attribute
	if(xmlTextWriterWriteAttribute(writer, BAD_CAST "ResourceFactory", BAD_CAST GetCreator()->GetResourceFactoryName().c_str()) < 0)
		NOVA_EXP("CImage::SerializeToXmlFileImpl: xmlTextWriterWriteAttribute fail", BAD_OPERATION);

	SerializeToXmlFileImpl(writer);

    xmlTextWriterEndDocument(writer);
	xmlFreeTextWriter(writer);
}

////////////////// Resource global hash //////////////////////////
stl<nstring, CResourcePtr>::map CResourceManager::mResourceHash;
stl<nstring, CResourceManager *>::map CResourceManager::mResourceFactoryHash;
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

void CResourceManager::SetResourceLocation(const nstring & path)
{
	nstring mes("Set resource location: ");
	LOG_MESSAGE(mes + path);
	mResourceLocation = path;
}

nstring CResourceManager::GetResourceLocation() const
{
	return mResourceLocation;
}

CAsyncLockingQueue::CAsyncLockingQueue()
{
	mQueue.clear();
	nInt32 res = pthread_mutex_init(&mLockMutex, NULL);
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

nInt32 CAsyncLockingQueue::ProccesQueue(void)
{
	nInt32 res = 0;
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

/* for force loading form packages */
CResourcePtr CResourceManager::LoadResourceFromXml(const nstring &filename, const CFilesPackage &package)
{
	CResourcePtr res;
	nstring rname, rgroup;
	CResourceManager *manager = NULL;

	if(!package.IsOpened())
		return res;

	nova::CMemoryBuffer xmlfile = package.GetFile(filename);
	if(xmlfile.GetBegin() && ( package.GetFileExt(filename) == "xml"))
	{
		xmlDocPtr doc = NULL;
		doc = xmlParseMemory((char *)xmlfile.GetBegin(), xmlfile.GetBufferSize());

		if(doc == NULL)
			throw NOVA_EXP("CResourceManager::LoadResourceFromXml - Parse error of xml file from memory", BAD_OPERATION);

		xmlNodePtr cur = xmlDocGetRootElement(doc);
		if(cur == NULL)
		{
			xmlFreeDoc(doc);
			throw NOVA_EXP("CResourceManager::LoadResourceFromXml - Parse error xml root element.", BAD_OPERATION);
		}

		const xmlChar * caption = cur->name;
		if(xmlStrcmp(caption, (xmlChar *)"NovaResource"))
		{
			xmlFreeDoc(doc);
			throw NOVA_EXP("CResourceManager::LoadResourceFromXml - this xml file is not a nova resource file.", BAD_OPERATION);
		}

		cur = cur->children;
		xmlNodePtr dataNode = NULL;
		while(cur != NULL)
		{
			if(xmlIsBlankNode(cur))
			{
				cur = cur->next;
				continue;
			}

			if(!xmlStrcmp(cur->name, (xmlChar *)"ResourceHeader"))
			{
				cur = cur->children;
				continue;
			}

			if(!xmlStrcmp(cur->name, (xmlChar *) "ResourceName"))
				rname.append(reinterpret_cast<char *>(cur->children->content));
			if(!xmlStrcmp(cur->name, (xmlChar *) "ResourceGroup"))
				rgroup.append(reinterpret_cast<char *>(cur->children->content));

			if(!xmlStrcmp(cur->name, (xmlChar *) "ResourceData"))
			{
				xmlChar * managerName = xmlGetProp(cur, (xmlChar *) "ResourceFactory");
				dataNode = cur->children;

				if(!managerName)
				{
					xmlFreeDoc(doc);
					throw NOVA_EXP("CResourceManager::LoadResourceFromXml - \
						Resource manager is not set..", BAD_OPERATION);
				}

				manager = GetResourceFactory(nstring(reinterpret_cast<char *>(managerName)));
				if(!manager)
				{
					xmlFreeDoc(doc);
					throw NOVA_EXP(nstring("CResourceManager::LoadResourceFromXml - \
						Resource manager ") + reinterpret_cast<char *>(managerName)
						+ " is not found..", BAD_OPERATION);
				}
			}

			cur = cur->next;
		}

		if(manager)
		{
			res = manager->LoadResourceFromXmlNodeImpl(rname, rgroup, dataNode, package);
			res->LoadResource();
			//res->RemoveAllListeners();
		}

		xmlFreeDoc(doc);
	}

	xmlfile.FreeBuffer();

	return res;
}

/* for force loading from file system */
CResourcePtr CResourceManager::LoadResourceFromXml(const nstring &filename)
{
	CResourcePtr res;
	nstring rname, rgroup;
	CResourceManager *manager = NULL;

	xmlDocPtr doc = NULL;
	doc = xmlParseFile(filename.c_str());

	if(doc == NULL)
		throw NOVA_EXP("CResourceManager::LoadResourceFromXml - Parse error of xml file from memory", BAD_OPERATION);

	xmlNodePtr cur = xmlDocGetRootElement(doc);
	if(cur == NULL)
	{
		xmlFreeDoc(doc);
		throw NOVA_EXP("CResourceManager::LoadResourceFromXml - Parse error xml root element.", BAD_OPERATION);
	}

	const xmlChar * caption = cur->name;
	if(xmlStrcmp(caption, (xmlChar *)"NovaResource"))
	{
		xmlFreeDoc(doc);
		throw NOVA_EXP("CResourceManager::LoadResourceFromXml - this xml file is not a nova resource file.", BAD_OPERATION);
	}

	cur = cur->children;
	xmlNodePtr data = NULL;
	while(cur != NULL)
	{
		if(xmlIsBlankNode(cur))
		{
			cur = cur->next;
			continue;
		}

		if(!xmlStrcmp(cur->name, (xmlChar *)"ResourceHeader"))
		{
			cur = cur->children;
			continue;
		}

		if(!xmlStrcmp(cur->name, (xmlChar *) "ResourceName"))
			rname.append(reinterpret_cast<char *>(cur->children->content));
		if(!xmlStrcmp(cur->name, (xmlChar *) "ResourceGroup"))
			rgroup.append(reinterpret_cast<char *>(cur->children->content));

		if(!xmlStrcmp(cur->name, (xmlChar *) "ResourceData"))
		{
			xmlChar * managerName = xmlGetProp(cur, (xmlChar *) "ResourceFactory");
			data = cur->children;

			if(!managerName)
			{
				xmlFreeDoc(doc);
				throw NOVA_EXP("CResourceManager::LoadResourceFromXml - \
					Resource manager is not set..", BAD_OPERATION);
			}

			manager = GetResourceFactory(nstring(reinterpret_cast<char *>(managerName)));
			if(!manager)
			{
				xmlFreeDoc(doc);
				throw NOVA_EXP(nstring("CResourceManager::LoadResourceFromXml - \
					Resource manager ") + reinterpret_cast<char *>(managerName)
					+ " is not found..", BAD_OPERATION);
			}
		}

		cur = cur->next;
	}

	if(manager)
	{
		res = manager->LoadResourceFromXmlNodeImpl(rname, rgroup, data);
		res->LoadResource();
		//res->RemoveAllListeners();
	}

	LOG_MESSAGE(nstring("Resource file ") + filename + " loaded successfully");

	xmlFreeDoc(doc);

	return res;
}

void CResourceManager::BuildAllManagedResources(void)
{
	stl<nstring, CResourcePtr>::map::iterator it;
	stl<nstring, CResourcePtr>::map copyHash;

	RESOURCE_MUTEX_SECTION_LOCK;

	copyHash = mResourceHash;

	RESOURCE_MUTEX_SECTION_UNLOCK;

	for(it = copyHash.begin(); it !=  copyHash.end(); it++)
	{
		it->second->BuildResource();
	}
}

void CResourceManager::ReLoadAllManagedResources(void)
{
	stl<nstring, CResourcePtr>::map::iterator it;
	stl<nstring, CResourcePtr>::map copyHash;

	RESOURCE_MUTEX_SECTION_LOCK;

	copyHash = mResourceHash;

	RESOURCE_MUTEX_SECTION_UNLOCK;

	for(it = copyHash.begin(); it !=  copyHash.end(); it++)
	{
		it->second->FreeResource();
		it->second->LoadResource();
	}
}

void CResourceManager::ReBuildAllManagedResources(void)
{
	stl<nstring, CResourcePtr>::map::iterator it;
	stl<nstring, CResourcePtr>::map copyHash;

	RESOURCE_MUTEX_SECTION_LOCK;

	copyHash = mResourceHash;

	RESOURCE_MUTEX_SECTION_UNLOCK;

	for(it = copyHash.begin(); it !=  copyHash.end(); it++)
	{
		it->second->RebuildResource();
	}
}

void CResourceManager::LoadAllManagedResources(void)
{
	stl<nstring, CResourcePtr>::map::iterator it;
	stl<nstring, CResourcePtr>::map copyHash;

	RESOURCE_MUTEX_SECTION_LOCK;

	copyHash = mResourceHash;

	RESOURCE_MUTEX_SECTION_UNLOCK;

	for(it = copyHash.begin(); it !=  copyHash.end(); it++)
	{
		it->second->LoadResource();
	}
}

void CResourceManager::RegisterResourceFactory(CResourceManager *factory)
{
	stl<nstring, CResourceManager *>::map::iterator it;

	if(factory)
	{
		if((it = mResourceFactoryHash.find(factory->GetResourceFactoryName())) == mResourceFactoryHash.end())
		{
			mResourceFactoryHash.insert(std::pair<nstring, CResourceManager *>(factory->GetResourceFactoryName(), factory));
			LOG_MESSAGE(nstring("CResourceManager::RegisterResourceFactory: ") + 
				factory->GetResourceFactoryName() + " resource factory registered.");
		}
	}
}	

void CResourceManager::UnRegisterResourceFactory(CResourceManager *factory)
{
	stl<nstring, CResourceManager *>::map::iterator it; 

	if(factory)
	{
		if((it = mResourceFactoryHash.find(factory->GetResourceFactoryName())) != mResourceFactoryHash.end())
		{
			mResourceFactoryHash.erase(it);
			LOG_MESSAGE(nstring("CResourceManager::UnRegisterResourceFactory: ") + 
				factory->GetResourceFactoryName() + " resource factory unregistered.");
		}
	}
}

CResourceManager * CResourceManager::GetResourceFactory(const nstring &name)
{
	stl<nstring, CResourceManager *>::map::iterator it; 
	if((it = mResourceFactoryHash.find(name)) != mResourceFactoryHash.end())
	{
		return it->second;
	}

	return NULL;
}

}
