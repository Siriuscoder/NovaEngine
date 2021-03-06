﻿/***************************************************************************
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

#include "nova_render_system.h"
#include "nova_static_object.h"
#include "nova_movable_object.h"
#include "nova_mesh_box.h"
#include "nova_content_loader.h"
#include "nova_tree.h"
#include "nova_bounding_box.h"
#include "nova_resource_manager.h"

namespace nova
{

class CSceneNode;
class CSceneManager;

class NOVA_EXPORT CSceneNodeListener : public CEventListener
{
public:

	virtual void ValidateNodeListener(CSceneNode *object) {}

	virtual void InValidateNodeListener(CSceneNode *object) {}

	virtual void BuildNodeListener(CSceneNode *object) {}

	virtual void ReleaseNodeListener(CSceneNode *object) {}

	virtual void RenderNodeListener(CSceneNode *object) {}

	virtual void PrepareNodeListener(CSceneNode *object) {}
};

class NOVA_EXPORT CSceneNode : public CListenerInterface
{
protected:

	bool isValidated;
	CBoundingBox mBoundingBox;
	CSceneManager *mParentSceneManager;
	nova::CSmartPtr<CRenderableObject> mpSceneObject;

	virtual void ValidateNodeImpl(void) = 0;

	virtual void InValidateNodeImpl(void) = 0;

	virtual void ReleaseNodeImpl(void) = 0;

	virtual void BuildNodeImpl(void) = 0;

	virtual void RenderNodeImpl(void) = 0;

	virtual void PrepareNodeImpl(void) = 0;

public:

	CSceneNode(CRenderableObject *pRenObj, CSceneManager *scene) : 
		mParentSceneManager(scene), mpSceneObject(pRenObj), isValidated(false), 
		mBatchesCount(0), mFacesCount(0) {}

	virtual ~CSceneNode() {}

	void BuildNode(void);

	void ValidateNode(void);

	void InValidateNode(void);

	void ReleaseNode(void);

	bool IsValidated(void);

	void RenderNode(void);

	void PrepareNode(void);

	CBoundingBox &GetBoundingBox(void);

	void SetBoundingBox(const CBoundingBox &box);

	inline nova::CSmartPtr<CRenderableObject> GetObjectInterface(void) { return mpSceneObject; }

	bool IsVisible(void);

	inline nova::nUInt32 GetRenderedBatchesCount(void) { return mBatchesCount; }

	inline nova::nUInt32 GetRenderedFacesCount(void) { return mFacesCount; }

protected:

	nova::nUInt32 mBatchesCount;
	nova::nUInt32 mFacesCount;
};

class NOVA_EXPORT CSceneManagerListener : public CEventListener
{
public:

	virtual void SceneRenderBeginListener(CSceneManager * object) {}

	virtual void SceneRenderEndListener(CSceneManager * object) {}

	virtual void PrepareSceneListener(CSceneManager *scene) {}

	virtual void PrepareSceneFrameListener(CSceneManager *scene) {}

	virtual void BuildSceneListener(CSceneManager *scene) {}

	virtual void DestroySceneListener(CSceneManager *scene) {}
};

class NOVA_EXPORT CSceneManager : public CListenerInterface
{
public:

	typedef nova::CSmartPtr<CSceneNode> TNodeType;
protected:

	nInt32 mRenderedBatches;
	nInt32 mRenderedFaces;
	nstring mRegisterGroup;
	CCamera *mCurCamera; 
	CViewPort *mCurView;
	nstring mSceneName;
	CTree<TNodeType> mSceneTree;

	bool isEnabled;
	bool mDepthTestEnabled;

	virtual nInt32 RenderSceneImpl(void) = 0;

	virtual nInt32 RenderСompoundObjects(void) {return 0;}

	virtual void PrepareSceneImpl(void) = 0;

	virtual nInt32 PrepareRenderQueueImpl(void) = 0;

	virtual void PrepareSceneFrameImpl(void) = 0;

	virtual void BuildSceneImpl(void) = 0;

	virtual void SerializeSceneToXmlImpl(xmlTextWriterPtr xmlWriter) = 0;

	virtual void DeSerializeSceneFromXmlImpl(xmlNodePtr node) = 0;

	void DestroySceneNode(CTreeNode<TNodeType> *node);

public:

	CSceneManager(const nstring & scene_name, const nstring & group);

	virtual ~CSceneManager();

	void RenderScene(CCamera *camera, CViewPort *view);

	virtual CSceneManager::TNodeType CreateRenderableNodeFromResource(const nstring &resource_name);

	CTreeNode<TNodeType> *GetRootElement(void);

	void SetRootElement(TNodeType elem);

	CTree<TNodeType> &GetSceneTree(void);

	void PrepareScene(void);

	nInt32 PrepareRenderQueue(void);

	void PrepareSceneFrame(void);

	void BuildScene(void);

	virtual void DestroyScene(void);

	nstring GetSceneName(void);

	nstring GetSceneSlavesGroup(void);

	nInt32 GetRenderedBatches(void);

	nInt32 GetRenderedFaces(void);

	void SerializeSceneToXml(xmlTextWriterPtr xmlWriter);

	void DeSerializeSceneFromXml(xmlNodePtr node);

	inline bool IsEnabled(void) { return isEnabled; }

	inline void Enable(void) { isEnabled = true; }

	inline void Disable(void) { isEnabled = false; }

	inline void DepthTest(bool flag) { mDepthTestEnabled = flag; }

	inline bool IsDepthTestEnabled() { return mDepthTestEnabled; }
};

}
