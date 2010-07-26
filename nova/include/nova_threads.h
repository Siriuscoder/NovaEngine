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
// Р‘Р°Р·РѕРІС‹Р№ РєР»Р°СЃСЃ РїРѕС‚РѕРєРѕРІ, РІСЃРµ РїРѕС‚РѕРєРё РІРЅСѓС‚СЂРё РґРІРёР¶РєР° 
// СЂРµР°Р»РёР·СѓСЋС‚СЃСЏ РЅР°СЃР»РµРґРѕРІР°РЅРёРµРј РѕС‚ СЌС‚РѕРіРѕ РєР»Р°СЃСЃР°
#include "nova_error.h"

/// \file nova_threads.h
/// \brief Р‘Р°Р·РѕРІС‹Р№ СЂРµР°Р»РёР·Р°С†РёСЏ РїРѕС‚РѕРєРѕРІ
///
/// Р¤Р°Р№Р» СЃРѕРґРµСЂР¶РёС‚ РѕРїРёСЃР°РЅРёРµ РєР»Р°СЃСЃР° CThread
/// РєРѕС‚РѕСЂС‹Р№ СЏРІР»СЏРµС‚СЃСЏ РїСЂРµРґРєРѕРј РІСЃРµС… РїРѕС‚РѕС‡РЅС‹С… РєР»Р°СЃСЃРѕРІ,
/// РІ РЅРµРј СЂРµР°Р»РёР·РѕРІР°РЅРЅ Р±Р°Р·РѕРІС‹Р№ С„СѓРЅРєС†РёРѕРЅР°Р».
/// \attention РџРѕС‚РѕРєРё РїРѕСЃС‚СЂРѕРµРЅРЅС‹ РЅР° SDL threads.

namespace nova {


/// \breaf РЎРѕСЃС‚РѕСЏРЅРёСЏ РЅРёС‚Рё
///
/// РљР°Р¶РґР°СЏ РЅРёС‚СЊ РјРѕР¶РµС‚ Р±С‹С‚СЊ РІ СЌС‚РёС… 2С… СЃРѕСЃС‚РѕСЏРЅРёСЏС…
/// РћСЃС‚Р°РЅРѕРІР»РµРЅРЅР°СЏ, РёР»Рё Р·Р°РїСѓС‰РµРЅРЅР° (РІСЃРµ РїСЂРѕСЃС‚Рѕ РґСѓРјР°СЋ)
enum ThState
{
	TH_KILLED,
	TH_PLAY	
};

#if !defined(DOXYGEN_IGNORE)
class NOVA_EXPORT CThread; 
#endif

///////// Mutex Sections ///////////////////////////////////
extern pthread_mutex_t ALLOCATOR_MUTEX_SECTION;
#define ALLOCATOR_MUTEX_SECTION_LOCK pthread_mutex_lock(&ALLOCATOR_MUTEX_SECTION)
#define ALLOCATOR_MUTEX_SECTION_UNLOCK pthread_mutex_unlock(&ALLOCATOR_MUTEX_SECTION)

extern pthread_mutex_t LOG_MUTEX_SECTION;
#define LOG_MUTEX_SECTION_LOCK pthread_mutex_lock(&LOG_MUTEX_SECTION)
#define LOG_MUTEX_SECTION_UNLOCK pthread_mutex_unlock(&LOG_MUTEX_SECTION)

extern pthread_mutex_t RESOURCE_MUTEX_SECTION;
#define RESOURCE_MUTEX_SECTION_LOCK pthread_mutex_lock(&RESOURCE_MUTEX_SECTION)
#define RESOURCE_MUTEX_SECTION_UNLOCK pthread_mutex_unlock(&RESOURCE_MUTEX_SECTION)

extern pthread_mutex_t OBJECT_MUTEX_SECTION;
#define OBJECT_MUTEX_SECTION_LOCK pthread_mutex_lock(&OBJECT_MUTEX_SECTION)
#define OBJECT_MUTEX_SECTION_UNLOCK pthread_mutex_unlock(&OBJECT_MUTEX_SECTION)
////////////////////////////////////////////////////////////

//РћР±СЉСЏРІР»СЏРµРј С‚РёРї РїРѕС‚РѕС‡РЅРѕР№ С„СѓРЅРєС†РёРё
/// \brief РўРёРї РїРѕС‚РѕС‡РЅРѕР№ С„СѓРЅРєС†РёРё
///
/// РџСЂРё РЅР°СЃР»РµРґРѕРІР°РЅРёРё РѕС‚ РєР»Р°СЃСЃР° CThread, РІ РїРѕР»СЊР·РѕРІР°С‚РµР»СЊСЃРєРѕРј РєР»Р°СЃСЃРµ
/// РїРѕС‚РѕРјРєРµ РІС‹ РґРѕР»Р¶РЅС‹ РѕР±СЉСЏРІРёС‚СЊ СЌС‚Сѓ С„СѓРЅРєС†РёСЋ.
typedef int (THISCALL__ CThread::*pfThread)(void *);


/// \brief РћСЃРЅРѕРІР° РїРѕС‚РѕРєРѕРІ
///
/// Р