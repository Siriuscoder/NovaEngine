/***************************************************************************
 *   Copyright (C) 2007 by SIRIUS										   *
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

/// \file nova_console.h
/// \brief Р¤Р°Р№Р» СЃРѕРґРµСЂР¶РёС‚ СЂРµР°Р»РёР·Р°С†РёСЋ РёРіСЂРѕРІРѕР№ РєРѕРЅСЃРѕР»Рё.
///
/// Р РµР°Р»РёР·РѕРІР°РЅРЅС‹ РєР»Р°СЃСЃС‹ РєРѕРЅСЃРѕР»Рё Рё РјРµРЅРµРґР¶РµСЂ РєРѕРЅСЃРѕР»РµР№.

#include "nova_fonts.h"
#include "nova_parser.h"

namespace nova {

#define COMMAND_LEN										100

/// \struct nv_console_pram
/// \brief РЎС‚СЂРѕС‡РЅС‹Рµ РїР°СЂР°РјРµС‚СЂС‹ РєРѕРЅСЃРѕР»Рё
///
/// РћР±С‰РёР№ РЅР°Р±РѕСЂ РїР°СЂР°РјРµС‚СЂРѕРІ РєРѕРЅСЃРѕР»Рё РґР»СЏ РёРЅРёС†РёР°Р»РёР·Р°С†РёРё.
struct nv_console_pram
{
/// \brief Р’С‹СЃРѕС‚Р° СЃС‚СЂРѕС‡РµРє (РІ РїРёРєСЃРµР»СЏС…)
	uint newline;
/// \brief Р Р°СЃСЃС‚РѕРЅРёРµ Р»РµРІРѕР№ РіСЂР°РЅРёС†С‹ РєРѕРЅСЃРѕР»Рё (РІ РїРёРєСЃРµР»СЏС…)
	uint leftclear;
/// \brief Р”РёСЃС‚Р°РЅС†РёСЏ РјРµР¶РґСѓ Р±СѓРєРІР°РјРё (РІ РїРёРєСЃРµР»СЏС…)
	uint distance;
/// \brief Р Р°СЃСЃС‚РѕСЏРЅРёРµ РІРµС…РЅРµР№ РіСЂР°РЅРёС†С‹ РєРѕРЅСЃРѕР»Рё (РІ РїРёРєСЃРµР»СЏС…)
	uint topclear;
/// \brief Р Р°СЃСЃС‚РѕСЏРЅРёРµ РЅРёР¶РЅРµР№ РіСЂР°РЅРёС†С‹ РєРѕРЅСЃРѕР»Рё (РІ РїРёРєСЃРµР»СЏС…)
	uint bottomclear;
/// \brief Р Р°СЃСЃС‚РѕСЏРЅРёРµ РїСЂР°РІРѕР№ РіСЂР°РЅРёС†С‹ РєРѕРЅСЃРѕР»Рё (РІ РїРёРєСЃРµР»СЏС…)
	uint rightclear;
/// \brief РЁРёСЂРёРЅР° С‚Р°Р±СѓР»СЏС†РёРё (РІ РїРёРєСЃРµР»СЏС…)
	uint tab;
};

/// \struct nv_console_init
/// \brief РћР±С‰РёРµ РєРѕРЅСЃРѕР»Рё
struct nv_console_init
{
/// \brief РўРёРї С€СЂРёС„С‚Р°
///
/// РљР°Рє СЏ РіРѕРІРѕСЂРёР», РґРІРёР¶РѕРє РїРѕРґРґРµСЂР¶РёРІР°РµС‚ РґРІР° С‚РёРїР° С€СЂРёС„С‚РѕРІ
/// TrueType Font, Win GDI Font. Р’С‚РѕСЂРѕР№ СЃР»СѓС‡Р°Р№ РєРѕРЅРµС‡РЅРѕ С‚РѕР»СЊРєРѕ РґР»СЏ РІРёРЅРґС‹,
/// РїРѕСЌС‚РѕРјСѓ РЅР° РґСЂСѓРіРёС… СЃРёСЃС‚РµРјР°С… РµРіРѕ РІРёРґРЅРѕ РЅРµ Р±СѓРґРµС‚, РЅР° РІРёРЅРґРµ Р¶Рµ РµСЃС‚СЊ РІС‹Р±РѕСЂ.
/// \attention Р•СЃР»Рё РІС‹ СЃС‚Р°РІРёС‚Рµ РїРѕРґР»РѕР¶РєСѓ SDL, РІС‹ РЅРµ РјРѕР¶РµС‚Рµ РёСЃРїРѕР»СЊР·РѕРІР°С‚СЊ Win GDI Font.
/// Р­С‚Рѕ СЃРІСЏР·Р°РЅРЅРѕ СЃ С‚РµРј, С‡С‚Рѕ РґР»СЏ Win GDI Font РЅСѓР¶РµРЅ С‚Р°Рє РЅР°Р·С‹РІР°РµРјС‹Р№ РіСЂР°С„РёС‡РµСЃРєРёР№ РєРѕРЅС‚РµРєСЃС‚ РѕРєРЅР°
/// РєРѕС‚РѕСЂС‹Р№ РёР· SDL РЅРµ РІС‹С‚Р°С‰РёС‚СЊ. РїРѕСЌС‚РѕРјСѓ РґР»СЏ СЌС‚РѕРіРѕ С‚РёРїР° С€СЂРёС„С‚РѕРІ РјРѕР¶РЅРѕ РёСЃРїРѕР»СЊР·РѕРІР°С‚СЊ С‚РѕР»СЊРєРѕ РїРѕРґР»РѕР¶РєСѓ Win.
/// Р”Р»СЏ TrueType Font РјРѕР¶РЅРѕ РёСЃРїРѕР»СЊР·РѕРІР°С‚СЊ Р»СЋР±СѓСЋ.
	uint fonttype;
/// \brief Р Р°Р·РјРµСЂ СЌРєСЂР°РЅР°
///
/// x С€РёСЂРёРЅР°. Сѓ РІС‹СЃРѕС‚Р°.
	POINT screen;
/// \brief РњР°РєСЃРёРјР°Р»СЊРЅРѕРµ РєРѕР»РёС‡РµСЃС‚РІРѕ СЃРёРјРІРѕР»РѕРІ РІ РєРѕРЅСЃРѕР»Рё.
	uint maxchar;
/// \brief РЎС‚СЂСѓРєС‚СѓСЂР° СЃС‚СЂРѕС‡РЅС‹С… РїР°СЂР°РјРµС‚СЂРѕРІ.
///
/// \see nv_console_pram
	nv_console_pram param;
};

/// \struct nv_console_char
/// \brief РЎРёРјРІРѕР» РєРѕРЅСЃРѕР»Рё.
///
/// РЎРѕРґРµСЂР¶РёС‚ РґРІР° РїРѕР»СЏ РєРѕРґ СЃРёРјРІРѕР»Р° Рё С†РІРµС‚ СЃРёРјРІРѕР»Р°
/// С‚Р°РєРёРј РѕР±СЂР°Р·РѕРј РєР°Р¶РґС‹Р№ СЃРёРјРІРѕР» РјРѕР¶РµС‚ Р±С‹С‚СЊ СЂР°Р·РЅРѕРіРѕ С†РІРµС‚Р°)
struct nv_console_char
{
/// \brief РљРѕРґ СЃРёРјРІРѕР»Р° (UTF-8)
	wchar_t unicode;
/// Р¦РІРµС‚ РІ С„РѕСЂРјР°С‚Рµ RGB 
///
/// Р”Р»СЏ СѓСЃС‚Р°РЅРѕРІРєРё С†РІРµС‚Р° РїРѕР»СЊР·СѓР№С‚РµР»СЊ РјР°РєСЂРѕСЃРѕРј N_RGB (x, x, x)
	CColorRGB color;
};

/// \brief РџСѓСЃС‚РѕР№ СЃРёРјРІРѕР». (Р—Р°РІРµСЂС€Р°СЋС‰РёР№)
const nv_console_char empty = {0};

/// \brief Р’РµРєС‚РѕСЂ СЃС‚СЂРѕРє
typedef stl<nstring>::vector args;

/// \class CConsoleBase
/// \brief Р‘Р°Р·РѕРІР°СЏ РєРѕРЅСЃРѕР»СЊ
/// 
/// РўСѓС‚ СЃРѕРґРµСЂР¶РёС‚СЃСЏ Р±Р°Р·РѕРІРѕРµ РѕРїРёСЃР°РЅРёРµ РєРѕРЅСЃРѕР»Рё.
class NOVA_EXPORT CConsoleBase : public CBase
{
protected:
/// \brief Р‘СѓС„РµСЂ СЃС‚СЂРѕРєРё
	stl<nv_console_char>::vector console_string;

/// \brief РџР°СЂР°РјРµС‚СЂС‹ РєРѕРЅСЃРѕР»Рё
	nv_console_init			initdata;
	CColorRGB				color;

	/// \brief Р