п»ї/***************************************************************************
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

//(c) Winnie  woowoowoow 'at' bk.ru
//fast memory allocation routines.
/// \file nova_allocator.h
/// \brief РђР»Р»РѕРєР°С‚РѕСЂ РїР°РјСЏС‚Рё РёСЃРїРѕР»СЊР·СѓРµРјС‹Р№ РґРІРёР¶РєРѕРј.
///
/// РЇ РїРѕР·Р°РёРјСЃС‚РІРѕРІР°Р» Р°Р»Р»РѕРєР°С‚РѕСЂ РїР°РјСЏС‚Рё Р’РёРЅРЅРё (gamedev.ru)
/// РѕРЅ РІРїРѕР»РЅРµ Р±С‹СЃС‚СЂС‹Р№ Рё РїСЂРѕСЃС‚РѕР№ РґР»СЏ РёСЃРїРѕР»СЊР·РѕРІР°РЅРёСЏ.
/// Р—Р« РђРІС‚РѕСЂСѓ СЃРїР°СЃРёР±Рѕ ))
///
/// (c) Winnie  woowoowoow 'at' bk.ru
/// fast memory allocation routines.


#include "sys/nova_system.h"

#define WINNIE_ALLOC_THROW_STD_BADALLOC 1
#define WINNIE_ALLOC_USE_MALLOC_THRESHOLD 0
#define WINNIE_ALLOC_STATISTIC_ENABLED 1


#if !defined(DOXYGEN_IGNORE)
namespace nova {
#endif

/// \namespace winnie_allocator
/// \brief РђР»Р»РѕРєР°С‚РѕСЂ РЅР°С…РѕРґРёС‚СЃСЏ РІ РїСЂРѕСЃС‚РѕР°РЅСЃС‚СЂРµ РёРјРµРЅ winnie_allocator 
///
/// Р’С‹РЅРµСЃ РІСЃРµ РІ РѕС‚РґРµР»СЊРЅРѕРµ РїСЂРѕСЃС‚СЂР°РЅСЃС‚РІРѕ РёРјРµРЅ, РґР»СЏ РЅР°РіР»СЏРґРЅРѕСЃС‚Рё.
/// Р”Р»СЏ РїРѕРґРєР»СѓС‡РµРЅРёСЏ Р°Р»Р»РѕРєР°С‚РѕСЂР° РІ РІР°С€Сѓ РїСЂРѕРіСЂР°РјРјСѓ РёСЃРїРѕР»СЊР·СѓР№С‚Рµ
/// \code
/// using namespace nova::winnie_allocator;
/// \endcode
namespace winnie_allocator
{

const size_t malloc_threshold = 1024;
const unsigned first_os_block_size_log = 20; //1 MB.
const unsigned max_lists_num = 31;

/// \struct Statistic
/// \brief РЎС‚Р°С‚РёСЃС‚РёРєР° РёСЃРїРѕР»СЊР·РѕРІР°РЅРёСЏ РїР°РјСЏС‚Рё
///
/// Р