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
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
// Р‘Р°Р·РѕРІС‹Р№ РєР»Р°СЃСЃ
#pragma once

/// \file nova_base.h
/// \brief Р‘Р°Р·РѕРІС‹Р№ Р·Р°РіРѕР»РѕРІРѕРє.
///
/// РўСѓС‚ Сѓ РЅР°СЃ СЂР°СЃРїРѕР»РѕР¶РµРЅ Р±Р°Р·РѕРІС‹Р№ РєР»Р°СЃСЃ
/// РєРѕРјРµРЅС‡Сѓ, С‚Р°Рє, РґР»СЏ РѕР±С‰РµРіРѕ СЂР°Р·РІРёС‚РёСЏ))

#include "nova_object_constructor.h"

namespace nova {

#define CLASS_NAME_LEN							30

#define _BASE_CLASS_

/// \class CBase
/// \brief РљРѕСЂРЅРµРІРѕР№ РєР»Р°СЃСЃ РґРІРёР¶РєР°
///
/// Р’РѕРѕР±С‰РµРјС‚Рѕ РїРѕС‡С‚Рё Р»СЋР±РѕР№ РєР»Р°СЃСЃ РІ РїСЂРѕСЃС‚СЂР°РЅСЃС‚РІРµ nova РЅР°СЃР»РµРґСѓРµС‚СЃСЏ 
/// РѕС‚ СЌС‚РѕРіРѕ РєР»Р°СЃСЃР°. Р’ СЃРІРѕРµР№ РїСЂРѕРіСЂР°РјРјРµ РІС‹ С‚Р°Рє Р¶Рµ РјРѕР¶РµС‚Рµ 
/// РЅР°СЃР»РµРґРѕРІР°С‚СЃСЏ РѕС‚ РЅРµРіРѕ (РµСЃР»Рё РµСЃС‚СЊ Р¶РµР»Р°РЅРёРµ), Р° РµСЃС‚СЊ СЃР»СѓС‡Р°Рё
/// РєРѕРіРґР° СЌС‚Рѕ РЅРµРѕР±С…РѕРґРёРјРѕ!
/// \attention РќР°СЃР»РµРґСѓСЏСЃСЊ РѕС‚ СЌС‚РѕРіРѕ РєР»Р°СЃСЃР° РІС‹ РїРµСЂРµРЅРёРјР°РµС‚Рµ РЅР°СЃР»РµРґРѕРІР°РЅРёРµ РѕС‚ 
/// РєР»Р°СЃСЃР° CObjectConstructor, РІ РєРѕС‚РѕСЂРѕРј РїРµСЂРµРіСЂСѓР¶РµРЅРЅС‹ РѕРїРµСЂР°С‚РѕСЂС‹ new/delete
/// С‚РµРј СЃР°РјС‹Рј РІС‹ РёСЃРїРѕР»СЊР·СѓРµС‚Рµ РґР»СЏ РєРѕРЅСЃС‚СЂСѓРёСЂРѕРІР°РЅРёСЏ РїР°РјСЏС‚Рё Р’РёРЅРЅРё Р°Р»Р»РѕРєР°С‚РѕСЂ
/// \see nova_allocator.h

class NOVA_EXPORT CBase : public nova::CObjectConstructor
{
protected:
/// \brief Р