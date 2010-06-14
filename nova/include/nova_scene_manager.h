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

#include "nova_render_system.h"
#include "nova_static_object.h"
#include "nova_movable_object.h"
#include "nova_mesh_box.h"
#include "nova_mesh_loader.h"
#include "nova_tree.h"
#include "nova_bounding_box.h"

namespace nova
{

enum NNodeType
{
	NT_MESH_NODE,
	NT_CAMERA_NODE,
	NT_LIGHT_NODE,
	NT_OCTREE_NODE
};

enum NSceneType
{
	NS_BASIC_SCENE,
	NS_OCTREE_SCENE
};

class CSceneNode;
class CSceneManager;

class NOVA_EXPORT CSceneNodeListener : public CEventListener
{
public:

	virtual void ValidateNodeListener(CSceneNode * object) {}

	virtual void InValidateNodeListener(CSceneNode * object) {}
};

class NOVA_EXPORT CSceneNode : public CListenerInterface
{
protected:

	NNodeType mNodeType;
	bool isValidated;
	CWorldObject *mChildObject;
	CBoundingBox mBoundingBox;

	virtual void ValidateNodeImpl(void) = 0;

	virtual void ReleaseNodeImpl(void) = 0;

public:

	CSceneNode(NNodeType type) : mNodeType(type), mChildObject(NULL) {}

	CSceneNode(NNodeType type, CWorldObject *obj) : mNodeType(type), mChildObject(obj) {}

	virtual CWorldObject* ConstractWorldObject(void) = 0;

	virtual void PrepareNode(void) = 0;

	void SetWorldObject(CWorldObject *obj);

	CWorldObject *GetWorldObject(void);

	NNodeType GetNodeType(void);

	void ValidateNode(void);

	void InValidateNode(void);

	void ReleaseNode(void);
};

class NOVA_EXPORT CSceneManagerListener : public CEventListener
{
public:

	virtual void SceneRenderBeginListener(CSceneManager * object) {}

	virtual void SceneRenderEndListener(CSceneManager * object) {}
};

class NOVA_EXPORT CSceneManager : public CListenerInterface
{
protected:

	int mRenderedBatches;
	int mRenderedFaces;
	int mSceneType;
	nstring mRegisterGroup;
	CCamera *mCurCamera; 
	CViewPort *mCurView;
	nstring mSceneName;
	CTree<CSceneNode*> mSceneTree;

	virtual void RenderSceneImpl(void) = 0;

	int RenderСompoundObjects(void);

public:

	CSceneManager(const nstring & scene_name, const nstring & group);

	virtual ~CSceneManager();

	void RenderScene(CCamera *camera, CViewPort *view);

	CTreeNode<CSceneNode*> *ConstactSpecifiedNode(NNodeType type);

	CTreeNode<CSceneNode*> *GetRootElement(void);

	void SetRootElement(CSceneNode *elem);

	CTree<CSceneNode*> *GetSceneTreePtr(void);

	virtual void PrepareScene(void) = 0;

	virtual void PrepareRenderQueue(void) = 0;

	virtual void PrepareSceneFrame(void) = 0;

	virtual void BuildScene(void) = 0;

	virtual void DestroyScene(void) = 0;

	int GetSceneType(void); 

	nstring GetSceneName(void);

	void ClearObjects(void);

	int GetRenderedBatches(void);

	int GetRenderedFaces(void);
};

}
