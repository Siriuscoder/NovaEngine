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

namespace nova
{

class NOVA_EXPORT CSceneManager : public CBase, public CListenerInterface
{
protected:

	int mRenderedBatches;
	int mRenderedFaces;
	int mSceneType;
	typedef stl<nstring, stl<CWorldObject *>::vector>::map TObjectsMap;

	TObjectsMap mObjectsMap;
	nstring mRegisterGroup;
	CCamera *mCurCamera; 
	CViewPort *mCurView;
	nstring mSceneName;

	virtual CWorldObject *ConstructRenderableObject(const nstring & name) = 0;

	virtual void RenderSceneImpl(void) = 0;

	int RenderСompoundObjects(void);

public:

	CSceneManager(const nstring & scene_name, const nstring & group);

	virtual ~CSceneManager();

	void RenderScene(CCamera *camera, CViewPort *view);

	CWorldObject *ConstructSingleObjectFromResourceMesh(const nstring & name, const nstring & meshname,
		bool attach);

	void AttachSingleObject(CWorldObject *obj, const nstring & mesh);

	virtual void PrepareScene(void) = 0;

	virtual void BuildScene(void) = 0;

	virtual void DestroyScene(void) = 0;

	int GetSceneType(void); 

	nstring GetSceneName(void);

	void ClearObjects(void);

	int GetRenderedBatches(void);

	int GetRenderedFaces(void);
};

}
