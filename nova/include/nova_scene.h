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

#include "nova_scene_scrap.h"

namespace nova
{

class NOVA_EXPORT CScene : public CBase, public CSingelton<CScene>
{
friend CSceneManager;
friend CSceneScrap;
/* 
	Класс сцены является на самом деле лишь менеджером
	скрапов. Скрап это кусок сцены, (набор объектов) на который 
	мы делим сцену логически.. тоесть в один момент времени может рендерится
	лишь один скрап.. мы можем включать и отключать скрапы. делать их актывными, 
	меняться объектами между ними. 
*/
protected:

	stl<CSceneScrap *>::vector mScraps;
	int mRenderedFaces;
	int mRenderedBatches;

public:

	CScene();

	~CScene();

	void DeleteAllScraps(void);

	int AddScrap(CSceneScrap *scrap);
/*
	void AddObjectToScrap(nova::uint id, CMovableObject *obj);

	void AddObjectToScrap(nova::uint id, CStaticObject *obj);
*/
	int AddScrap(CSceneManager *manager);

	CSceneManager *FindScene(const nstring &name);

	void DeleteScrap(nova::uint id);

	void RenderAllScene(CCamera * camera, CViewPort * view);

	int GetRenderedBatches(void);

	int GetRenderedFaces(void);

	CSceneScrap & GetScrap(nova::uint id);

	CSceneScrap * LoadSceneScrap(const StringList & models, CSceneManager * scene, CMeshLoader *loader, bool attach);

	void LoadSceneScrapAsync(const StringList & models, CSceneManager * scene, CMeshLoader *loader);
};

}