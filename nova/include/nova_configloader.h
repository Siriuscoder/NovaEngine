/***************************************************************************
 *   Copyright (C) 2008 by SIRIUS										   *
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

/// \file nova_configloader.h
/// \brief Р¤Р°Р№Р» СЃ РѕРїРёСЃР°РЅРёРµРј РїР°СЂСЃРµСЂР° РєРѕРЅС„РёРіРѕРІ.
///
/// Р’С‹ РЅРµ РѕР±СЏР·Р°РЅРЅС‹ РїРѕР»СЊР·РѕРІР°С‚СЊСЃСЏ РёРјРµРЅРЅРѕ СЌС‚РёРј РєР»Р°СЃСЃРѕРј
/// Рё РёСЃРїРѕР»СЊР·РѕРІР°С‚СЊ РєРѕРЅС„РёРіРё РІРѕРѕР±С‰Рµ? СЌС‚Рѕ РІР°Рј СЂРµС€Р°С‚СЊ
/// СЏ РїРѕРґСѓРјР°Р», С‡С‚Рѕ СЌС‚Рѕ Р±СѓРґРµС‚ РЅРµ Р»РёС€РЅРёРј.


#include "nova_parser.h"

namespace nova {


/// \class CConfig
/// \brief РљР»Р°СЃСЃ СЂР°Р±РѕС‚С‹ СЃ РєРѕРЅС„РёРіСѓСЂР°С†РёРѕРЅРЅС‹РјРё С„Р°Р№Р»Р°РјРё
///
/// РљР»Р°СЃСЃ РїРѕСЃС‚СЂРѕРµРЅРЅ РЅР° С€Р°Р±Р»РѕРЅРµ РґРµСЂРµРІР°, РїРѕР»СЊР·РѕРІР°С‚РµР»СЊ РјРѕР¶РµС‚ 
/// РїРѕР»СѓС‡РёС‚СЊ РЅР°Р±РѕСЂ СЃС‚СЂРѕРє, РґР°Р»СЊС€Рµ РµРіРѕ СЂР°Р±РѕС‚Р°.
/// РќР°РїСЂРёРјРµСЂ С‚Р°РєРѕР№ РєРѕРЅС„РёРі.
/// \code
/// MYCONF		vers_0.1
/// RESOLUTION	800 600
/// 
/// BIT_RATE	24
/// \endcode
/// РџСЂРёРјРµСЂ
/// \code 
/// CConfig conf(nstring("./myconfig.conf"));
/// 
/// conf.MakeTree();
///
/// stl<nstring>::vector res = conf.GetNodeValues(nstring("RESOLUTION"));
/// // res ("800", "600")
/// \endcode
class NOVA_EXPORT CConfig : public CBase, public CParser
{
private:
	CTree<nstring> conftree;
	nstring filename;
	uint parseflag;

public:
/// \brief Р¤Р»Р°Рі РёСЃРїРѕР»СЊР·РѕРІР°РЅРёСЏ СЂРµРєСѓСЂСЃРёРІРЅРѕРіРѕ РїР°СЂСЃРµСЂР°
	static const uint RECURSIVE = 0x12;
/// \brief Р¤Р»Р°Рі С‚СЃРїРѕР»СЊР·РѕРІР°РЅРёСЏ РѕР±С‹С‡РЅРѕРіРѕ РїР°СЂСЃРµСЂР°.
	static const uint COMMON = 0x13;
/// \brief РљРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ
///
/// \param flag С„Р»Р°Рі РїР°СЂСЃРµСЂР°
	CConfig(uint flag = COMMON);

/// \brief РљРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ 
///
/// \param _filename РџСѓС‚СЊ Рє С„Р°Р№Р»Сѓ РєРѕРЅС„РёРіР°.
/// \param flag С„Р»Р°Рі РїР°СЂСЃРµСЂР°
	CConfig(nstring & _filename, uint flag = COMMON);
/// \brief Р”РµСЃС‚СЂСѓРєС‚РѕСЂ
	~CConfig();

/// \brief РџР°СЂСЃРµСЂ
///
/// РћС‚РєСЂС‹РІР°РµС‚ С„Р°Р№Р», РїР°СЂСЃРёС‚ РєРѕРЅС„РёРі, СЃС‚СЂРѕРёС‚ РґРµСЂРµРІРѕ.
	int MakeTree();
/// \brief Р‘РµСЂРµС‚ СѓРєР°Р·Р°С‚РµР»СЊ РЅР° РІРµСЂС€РёРЅСѓ РґРµСЂРµРІР° РїРѕ РёРјРµРЅРё.
	CTreeNode<nstring> * GetNodePointer(nstring & node);
/// \brief Р‘РµСЂРµС‚ РІРµРєС‚РѕСЂ РїР°СЂР°РјРµС‚СЂРѕРІ РёР· СѓРєР°Р·Р°РЅРЅРѕР№ РІРµСЂС€РёРЅС‹ РґРµСЂРµРІР°.
	values GetNodeValues(nstring & node);
/// \brief Р‘РµСЂРµС‚ СѓРєР°Р·Р°С‚РµР»СЊ РЅР° РґРµСЂРµРІРѕ СЂР°Р·Р±РѕСЂР°.
	CTree<nstring> * GetTree();

/// \brief РћС‡РёС‰СЏРµС‚ РґРµСЂРµРІРѕ.
	void FreeTree();

/// \brief РЎРѕС…СЂР°РЅСЏРµС‚ РІ РєРѕРЅС„РёРі С‚РµРєСѓС‰РµРµ РґРµСЂРµРІРѕ РїР°СЂР°РјРµС‚СЂРѕРІ.
///
/// Р•СЃР»Рё РµСЃС‚СЊ Р·Р°РіСЂСѓР·РєР° Рё РїРѕСЃС‚СЂРѕРµРЅРёРµ РґРµСЂРµРІР° РёР· С„Р°Р№Р»Р°, С‚Рѕ РїРѕС‡РµРјСѓ 
/// Р±С‹ Рё РЅРµ СЃРґРµР»Р°С‚СЊ СЃРѕС…СЂР°РЅРёРµ РґРµСЂРµРІР° РїР°СЂР°РјРµС‚СЂРѕРІ.
/// \param tree РІС…РѕРґРЅРѕРµ Р·Р°РїРѕР»РЅРµРЅРЅРѕРµ РґРµСЂРµРІРѕ.
/// \param _filename РёРјСЏ РІС‹С…РѕРґРЅРѕРіРѕ С„Р°Р№Р»Р°.
	int SaveTree(CTree<nstring> & tree, nstring & _filename);
};









}
