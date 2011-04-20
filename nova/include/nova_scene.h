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

#include "nova_basic_scene_manager.h"
#include "nova_resource_package.h"
#include "nova_ase_loader.h"
#include "nova_msh_loader.h"

namespace nova
{

class NOVA_EXPORT CScene : public CBase, public CSingelton<CScene>
{
friend CSceneManager;
/* 
	Класс сцены является на самом деле лишь менеджером
	скрапов. Скрап это кусок сцены, (набор объектов) на который 
	мы делим сцену логически.. тоесть в один момент времени может рендерится
	лишь один скрап.. мы можем включать и отключать скрапы. делать их актывными, 
	меняться объектами между ними. 
*/
protected:

	stl<nova::CSmartPtr<CSceneManager>>::list mScraps;
	nInt32 mRenderedFaces;
	nInt32 mRenderedBatches;

public:

	CScene();

	~CScene();

	void DeleteAllScraps(void);

	nInt32 AddScrap(CSceneManager *manager);

	nova::CSmartPtr<CSceneManager> FindScene(const nstring &name);

	void RenderAllScene(CCamera * camera, CViewPort * view);

	nInt32 GetRenderedBatches(void);

	nInt32 GetRenderedFaces(void);

	nInt32 LoadSceneForce(const CFilesPackage &rPack, bool withResorces);

	nInt32 LoadSceneForce(const nstring &pckFile, bool withResorces);

	nInt32 LoadSceneInBackgroundMode(const CFilesPackage &rPack, bool withResorces);

	nInt32 LoadSceneInBackgroundMode(const nstring &pckFile, bool withResorces);
};

}