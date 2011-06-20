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

#include "nova_scene.h"
#include "nova_resource_manager.h"

namespace nova
{

template<> CScene * CSingelton<CScene>::SingeltonObject = NULL;

CScene::CScene() : CBase("CScene")
{
	mScraps.clear();
}

CScene::~CScene() 
{
	DeleteAllScraps();
}

void CScene::DeleteAllScraps(void)
{
	stl<nova::CSmartPtr<CSceneManager>>::list::iterator it = mScraps.begin();

	for(; it != mScraps.end(); it++)
	{
		(*it)->DestroyScene();
	}

	mScraps.clear();
}

void CScene::BuildAllScraps(void)
{
	stl<nova::CSmartPtr<CSceneManager>>::list::iterator it = mScraps.begin();

	for(; it != mScraps.end(); it++)
	{
		(*it)->BuildScene();
	}
}

nInt32 CScene::GetRenderedBatches(void)
{
	return mRenderedBatches;
}

nInt32 CScene::GetRenderedFaces(void)
{
	return mRenderedFaces;
}

void CScene::RenderAllScene(CCamera * camera, CViewPort * view)
{
	if(!camera || !view)
		return;

	stl<nova::CSmartPtr<CSceneManager>>::list::iterator it = mScraps.begin();
	mRenderedBatches = 0;
	mRenderedFaces = 0;

	CRenderSystem * Renderer = NULL;

	Renderer = CRenderSystem::GetSingeltonPtr();
	Renderer->SetVeiwport((nInt32)view->GetActualLeft(), 
		(nInt32)view->GetActualTop(), 
		(nInt32)view->GetActualWidth(), 
		(nInt32)view->GetActualHeight());

	camera->Validate();

	for(; it != mScraps.end(); ++it)
	{
		if((*it)->IsEnabled())
		{
			(*it)->PrepareSceneFrame();
			(*it)->RenderScene(camera, view);
			mRenderedFaces += (*it)->GetRenderedFaces();
			mRenderedBatches += (*it)->GetRenderedBatches();
		}
	}
}

nova::CSmartPtr<CSceneManager> CScene::FindScene(const nstring &name)
{
	stl<nova::CSmartPtr<CSceneManager>>::list::iterator it = mScraps.begin();

	for(; it != mScraps.end(); it++)
	{
		if((*it)->GetSceneName() == name)
			return (*it);
	}

	return NULL;
}

nInt32 CScene::AddScrap(CSceneManager *manager)
{
	if(manager)
	{
		mScraps.push_back(manager);
		return mScraps.size()-1;
	}

	throw NOVA_EXP("CScene::AddScrap: manager pointer refer to null ptr..", MEM_ERROR);
}

nInt32 CScene::LoadSceneForce(const CFilesPackage &rPack, const nstring &pckFile, bool withResorces)
{

	return 0;
}

nInt32 CScene::LoadSceneForce(const nstring &pckFile, bool withResorces)
{
	xmlDocPtr doc = NULL;
	doc = xmlParseFile(pckFile.c_str());

	if(doc == NULL)
		throw NOVA_EXP("CScene::LoadSceneForce - Parse error of xml file from file", BAD_OPERATION);

	xmlNodePtr cur = xmlDocGetRootElement(doc);
	if(cur == NULL)
	{
		xmlFreeDoc(doc);
		throw NOVA_EXP("CScene::LoadSceneForce - Parse error xml root element.", BAD_OPERATION);
	}

	const xmlChar * caption = cur->name;
	if(xmlStrcmp(caption, (xmlChar *)"NovaScene"))
	{
		xmlFreeDoc(doc);
		throw NOVA_EXP("CScene::LoadSceneForce - this xml file is not a nova scene file.", BAD_OPERATION);
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

		if(withResorces)
		{
			if(!xmlStrcmp(cur->name, (xmlChar *)"SceneResources"))
			{
				data = cur->children;
				
				while(data != NULL)
				{
					if(xmlIsBlankNode(cur))
					{
						data = data->next;
						continue;
					}

					if(!xmlStrcmp(data->name, (xmlChar *) "ResourceFile"))
					{
						nova::CResourceManager::LoadResourceFromXml(reinterpret_cast<char *>(data->children->content));
					}

					data = data->next;
				}
			}
		}

		if(!xmlStrcmp(cur->name, (xmlChar *)"SceneContent"))
		{
			xmlChar * managerName = xmlGetProp(cur, (xmlChar *) "SceneManager");
			nova::CSmartPtr<CSceneManager> scenePtr = FindScene(reinterpret_cast<char *>(managerName));

			if(!scenePtr.IsNull())
			{
				scenePtr->DeSerializeSceneFromXml(cur->children);
				scenePtr->PrepareScene();
			}
			else
			{
				xmlFreeDoc(doc);
				throw NOVA_EXP(nstring("CScene::LoadSceneForce - can not find scene manager ") + reinterpret_cast<char *>(managerName), BAD_OPERATION);
			}
		}

		cur = cur->next;
	}

	xmlFreeDoc(doc);
	return 0;
}

nInt32 CScene::LoadSceneInBackgroundMode(const CFilesPackage &rPack, const nstring &pckFile, bool withResorces)
{
	return 0;
}

nInt32 CScene::LoadSceneInBackgroundMode(const nstring &pckFile, bool withResorces)
{
	return 0;
}

}