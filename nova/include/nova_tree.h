/***************************************************************************
 *   Copyright (C) 2009 by SIRIUS										   *
 *   SiriusStarNick@yandex.ru											   *
 *                                                                         *
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

/// \file nova_tree.h
/// \brief Р‘Р°Р·РѕРІРѕРµ РѕРїРёСЃР°РЅРёРµ СЃС‚СЂСѓРєС‚СѓСЂС‹ РґРµСЂРµРІР°
///
/// Р”СѓРјР°СЋ, С‡С‚Рѕ СЌС‚Р° РєРѕРЅСЃС‚СЂСѓРєС†РёСЏ РјРЅРµ РїРѕРЅР°РґРѕР±С‚СЃСЏ РІ РґР°Р»СЊРЅРµР№С€РµРј
/// Р”РµСЂРµРІРѕ СѓРЅРёРІРµСЂСЃР°Р»СЊРЅРѕРµ, РєР°Р¶РґР°СЏ РІРµСЂС€РёРЅР° РјРѕР¶РµС‚ СЃРѕРґРµСЂР¶Р°С‚СЊ РЅРµ
/// РѕРіСЂР°РЅРёС‡РµРЅРЅРѕРµ РєРѕР»РёС‡РµСЃС‚РІРѕ РїРѕС‚РѕРјРєРѕРІ.
/// Р РµР°Р»РёР·РѕРІР°РЅРЅРѕ РІ РІРёРґРµ С€Р°Р±Р»РѕРЅРѕРІ.
/// Р­С‚Рѕ С‚РѕР»СЊРєРѕ РѕСЃРЅРѕРІР°, РїРѕСЌС‚РѕРјСѓ РјРЅРѕРіРёРµ РґРµР№СЃС‚РІРёСЏ РЅСѓР¶РЅРѕ Р±СѓРґРµС‚ РґРѕРґРµР»С‹РІР°С‚СЊ
/// РІ РґР°Р»СЊРЅРµР№С€РµРј, РІ Р·Р°РІРёСЃРёРјРѕСЃС‚Рё РѕС‚ РєРѕРЅРєСЂРµС‚РЅРѕР№ Р·Р°РґР°С‡Рё.

#include "nova_error.h"

namespace nova {

/// \brief РљР»Р°СЃСЃ РІРµСЂС€РёРЅР°.
///
/// Р”РµСЂРµРІРѕ СЃРѕСЃС‚РѕРёС‚ РёР· С‚Р°РєРёР№ РІРµСЂС€РёРЅ, РїСЂРёС‡РµРј РІ РєР°Р¶РЅРѕР№ 
/// РІРµСЂС€РёРЅРµ С…СЂР°РЅРёС‚СЃСЏ Р»РёСЃС‚ СѓРєР°Р·Р°С‚РµР»РµР№ РЅР° РІСЃРµ РµРµ РїРѕС‚РѕРјРєРё.
template <class usertype> class CTreeNode : public CObjectConstructor
{
protected:

#if !defined(DOXYGEN_IGNORE)
/// \brief Р”Р°РЅРЅС‹Рµ РєРѕС‚РѕСЂС‹Рµ С…СЂР°РЅРёС‚ РІРµСЂС€РёРЅР°
	usertype data;

/// \brief РўРёРї СѓРєР°Р·Р°С‚РµР»СЊ РЅР° РІРµСЂС€РёРЅСѓ
	typedef CTreeNode<usertype> *tpnode;
/// \brief Р›РёСЃС‚ РІРµСЂС€РёРЅ
	typename nova::stl< tpnode >::list ch_nodes;
	//list<tpnode, CNovaAlloc<tpnode> > ch_nodes;
#endif


public:

/// \brief РљРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ
	CTreeNode() {};

/// \brief РљРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ
///
/// \param _data Р