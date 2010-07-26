п»ї/***************************************************************************
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
#include "nova_resource_package.h"

namespace nova
{

class NOVA_EXPORT CScene : public CBase, public CSingelton<CScene>
{
friend CSceneManager;
friend CSceneScrap;
/* 
	РљР»Р°СЃСЃ СЃС†РµРЅС‹ СЏРІР»СЏРµС‚СЃСЏ РЅР° СЃР°РјРѕРј РґРµР»Рµ Р»РёС€СЊ РјРµРЅРµРґР¶РµСЂРѕРј
	СЃРєСЂР°РїРѕРІ. РЎРєСЂР°Рї СЌС‚Рѕ РєСѓСЃРѕРє СЃС†РµРЅС‹, (РЅР°Р±РѕСЂ РѕР±СЉРµРєС‚РѕРІ) РЅР° РєРѕС‚РѕСЂС‹Р№ 
	РјС‹ РґРµР»РёРј СЃС†РµРЅСѓ Р»РѕРіРёС‡РµСЃРєРё.. С‚РѕРµСЃС‚СЊ РІ РѕРґРёРЅ РјРѕРјРµРЅС‚ РІСЂРµРјРµРЅРё РјРѕР¶РµС‚ СЂРµРЅРґРµСЂРёС‚СЃСЏ
	Р»РёС€СЊ РѕРґРёРЅ СЃРєСЂР°Рї.. РјС‹ РјРѕР¶РµРј РІРєР»СЋС‡Р°С‚СЊ Рё РѕС‚РєР»СЋС‡Р°С‚СЊ СЃРєСЂР°РїС‹. РґРµР»Р°С‚СЊ РёС… Р°РєС‚С‹РІРЅС‹РјРё, 
	РјРµРЅСЏС‚СЊСЃСЏ РѕР±СЉРµРєС‚Р°РјРё РјРµР¶РґСѓ РЅРёРјРё. 
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