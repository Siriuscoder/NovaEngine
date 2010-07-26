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

/// \file nova_object_constructor.h
/// \brief Р—РґРµСЃСЊ РѕРїРёСЃР°РЅ Р±Р°Р·РѕРІС‹Р№ РєРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ РѕР±СЉРµРєС‚РѕРІ 
///
/// РќСѓ РєРѕРЅРµС‡РЅРѕ Р¶Рµ РѕРЅ РѕСЃРЅРѕРІР°РЅ РЅР° РІРёРЅРЅРё Р°Р»Р»РѕРєР°С‚РѕСЂРµ.

#include "nova_memory.h"

namespace nova {

/// \class CObjectConstructor
/// \brief РљРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ РѕР±СЉРµРєС‚РѕРІ
///
/// РќР°СЃР»РµРґСѓСЏСЃСЊ РѕС‚ РєР»Р°СЃСЃР° CBase РІС‹ Р°РІС‚РѕРјР°С‚РёС‡РµСЃРєРё РїРѕР»СѓС‡Р°РµС‚Рµ
/// РїРµСЂРµРіСЂСѓР¶РµРЅС‹Рµ new/delete РґР»СЏ РІР°С€РµРіРѕ РєР»Р°СЃСЃР° (РЅРѕ СЌС‚Рѕ РґРµР»Р°С‚СЊ СЃРѕРІСЃРµРј РЅРµ РѕР±СЏР·Р°С‚РµР»СЊРЅРѕ)
class NOVA_EXPORT CObjectConstructor
{
public:

/// \brief РљРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ
	CObjectConstructor() {};
/// \brief Р”РµСЃС‚СЂСѓРєС‚РѕСЂ
	~CObjectConstructor() {};

/// \brief РћРїРµСЂР°С‚РѕСЂ new
	void *operator new(size_t size);
/// \brief РћРїРµСЂР°С‚РѕСЂ delete
	void operator delete(void *p);

/// \brief РћРїРµСЂР°С‚РѕСЂ new РґР»СЏ РјР°СЃСЃРёРІРѕРІ
	void *operator new[](size_t size);
/// \brief РћРїРµСЂР°С‚РѕСЂ delete РґР»СЏ РјР°СЃСЃРёРІРѕРІ
	void operator delete[](void *p);

/// \brief РћРїРµСЂР°С‚РѕСЂ new
/// 
/// РќРµ РіРµРЅРµСЂРёСЂСѓРµС‚ РёСЃРєР»СЋС‡РёС‚РµР»СЊРЅС‹С… СЃРёС‚СѓР°С†РёР№
	void *operator new(size_t size, const std::nothrow_t & n);
/// \brief РћРїРµСЂР°С‚РѕСЂ delete
/// 
/// РќРµ РіРµРЅРµСЂРёСЂСѓРµС‚ РёСЃРєР»СЋС‡РёС‚РµР»СЊРЅС‹С… СЃРёС‚СѓР°С†РёР№
	void operator delete(void *p, const std::nothrow_t & n);

/// \brief РћРїРµСЂР°С‚РѕСЂ new РґР»СЏ РјР°СЃСЃРёРІРѕРІ
/// 
/// РќРµ РіРµРЅРµСЂРёСЂСѓРµС‚ РёСЃРєР»СЋС‡РёС‚РµР»СЊРЅС‹С… СЃРёС‚СѓР°С†РёР№
	void *operator new[](size_t size, const std::nothrow_t & n);
/// \brief РћРїРµСЂР°С‚РѕСЂ delete РґР»СЏ РјР°СЃСЃРёРІРѕРІ
/// 
/// РќРµ РіРµРЅРµСЂРёСЂСѓРµС‚ РёСЃРєР»СЋС‡РёС‚РµР»СЊРЅС‹С… СЃРёС‚СѓР°С†РёР№
	void operator delete[](void *p, const std::nothrow_t & n);
/// \brief РњР°РєСЃРёРјР°Р»СЊРЅС‹Р№ СЂР°Р·РјРµСЂ Р±Р»РѕРєР° РїР°РјСЏС‚Рё
	nova::ulonglong GetMaxBlockSize(void);
};



}
