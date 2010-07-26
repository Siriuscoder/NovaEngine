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

/// \file nova_parser.h
/// \brief РўРµРєСЃС‚РѕРІС‹Р№ РїР°СЂСЃРµСЂ
///
/// РџРѕРґСѓРјР°Р» РІС‹РЅРµСЃС‚Рё РїР°СЂСЃРµСЂ РІ РѕС‚РґРµР»СЊРЅС‹Р№ РјРѕРґСѓР»СЊ
/// РћРЅ РµС‰Рµ РЅР°Рј РїРѕРЅР°РґРѕР±РёС‚СЃСЏ Рё РЅРµ С‚РѕР»СЊРєРѕ РґР»СЏ РєРѕРЅС„РёРіРѕРІ.

#include "nova_tree.h"

namespace nova {

#define LINE_LENGTH			300

/// \class CParser
/// \brief РџР°СЂСЃРµСЂ
///
/// Р”Р»СЏ РёСЃРїРѕР»СЊР·РѕРІР°РЅРёСЏ С„СѓРЅРєС†РёР№ РїР°СЂСЃРµСЂР° СЃРѕРІРµС‚СѓСЋ РЅР°СЃР»РµРґРѕРІР°С‚СЊСЃСЏ РѕС‚ СЌС‚РѕРіРѕ
/// РєР»Р°СЃСЃР°.
class NOVA_EXPORT CParser
{
private:

	CTreeNode<nstring> * cnode;
	size_t found;
	nstring parstr;
	char value[LINE_LENGTH];

public:

/// \brief Р’РµРєС‚РѕСЂ СЃС‚СЂРѕРє
	typedef stl<nstring>::vector values;

/// \brief РџР°СЂСЃРµСЂ РІС…РѕРґРЅРѕР№ СЃС‚СЂРѕРєРё
///
/// РЎС‚СЂРѕРєР° РїРѕСЃС‚СѓРїР°РµС‚ РІ РІРёРґРµ ___command____arg1___arg2___arg3__....
/// РІ РёС‚РѕРіРµ РјС‹ РїРѕР»СѓС‡Р°РµРј РјР°СЃСЃРёРІ РѕС‚РґРµР»СЊРЅС‹С… СЃС‚СЂРѕРє command,arg1,arg2.arg3.
/// \param input Р’С…РѕРґРЅР°СЏ СЃС‚СЂРѕРєР°
/// \return Р’С‹С…РѕРґРЅР°СЏ РІРµСЂС€РёРЅР° РґРµСЂРµРІР°.
	CTreeNode<nstring> * ParseStringTree(nstring & input);

/// \brief РџР°СЂСЃРµСЂ РІС…РѕРґРЅРѕР№ СЃС‚СЂРѕРєРё
///
/// РЎС‚СЂРѕРєР° РїРѕСЃС‚СѓРїР°РµС‚ РІ РІРёРґРµ ___command____arg1___arg2___arg3__....
/// РІ РёС‚РѕРіРµ РјС‹ РїРѕР»СѓС‡Р°РµРј РјР°СЃСЃРёРІ РѕС‚РґРµР»СЊРЅС‹С… СЃС‚СЂРѕРє command,arg1,arg2.arg3.
/// \param input Р’С…РѕРґРЅР°СЏ СЃС‚СЂРѕРєР°
/// \return Р’С‹С…РѕРґРЅРѕР№ РјР°СЃСЃРёРІ СЃС‚СЂРѕРє
	values ParseString(nstring input);

/// \brief Р РµРєСѓСЂСЃРёРІРЅС‹Р№ РїР°СЂСЃРµСЂ РІС…РѕРґРЅРѕР№ СЃС‚СЂРѕРєРё
///
/// РЎС‚СЂРѕРєР° РїРѕСЃС‚СѓРїР°РµС‚ РІ РІРёРґРµ ___command____arg1___arg2___arg3__....
/// РІ РёС‚РѕРіРµ РјС‹ РїРѕР»СѓС‡Р°РµРј РјР°СЃСЃРёРІ РѕС‚РґРµР»СЊРЅС‹С… СЃС‚СЂРѕРє command,arg1,arg2.arg3.
/// \param input Р’С…РѕРґРЅР°СЏ СЃС‚СЂРѕРєР°
/// \param result Р’С‹С…РѕРґРЅРѕР№ РјР°СЃСЃРёРІ СЃС‚СЂРѕРє
	int ParseStringRecurse(nstring input, values & result);

/// \brief Р РµРєСѓСЂСЃРёРІРЅС‹Р№ РїР°СЂСЃРµСЂ РІС…РѕРґРЅРѕР№ СЃС‚СЂРѕРєРё
///
/// РЎС‚СЂРѕРєР° РїРѕСЃС‚СѓРїР°РµС‚ РІ РІРёРґРµ ___command____arg1___arg2___arg3__....
/// РІ РёС‚РѕРіРµ РјС‹ РїРѕР»СѓС‡Р°РµРј РјР°СЃСЃРёРІ РѕС‚РґРµР»СЊРЅС‹С… СЃС‚СЂРѕРє command,arg1,arg2.arg3.
/// \param input Р’С…РѕРґРЅР°СЏ СЃС‚СЂРѕРєР°
/// \return node Р’С‹С…РѕРґРЅР°СЏ РІРµСЂС€РёРЅР° РґРµСЂРµРІР°.
	CTreeNode<nstring> * ParseStringRecurseTree(nstring & input);

/// \brief РџСЂРµРѕР±СЂР°Р·РѕРІР°С‚СЊ РІРµСЂС€РёРЅСѓ РґРµСЂРµРІР° РІ РјР°СЃСЃРёРІ СЃС‚СЂРѕРє
	values NodeToValues(CTreeNode<nstring> * node);
/// \brief РџСЂРµРѕР±СЂР°Р·РѕРІР°С‚СЊ РјР°СЃСЃРёРІ СЃС‚СЂРѕРє РІ РІРµСЂС€РёРЅСѓ РґРµСЂРµРІР°
	CTreeNode<nstring> * ValuesToNode(values & input);
};

}
