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
// РћР±С‰РёРµ С„СѓРЅРєС†РёРё РїСЂРѕРІРµСЂРєРё РѕС€РёР±РѕРє
#pragma once

/// \file nova_error.h
/// \brief РћР±С‰РёРµ С„СѓРЅРєС†РёРё РїСЂРѕРІРµСЂРєРё РѕС€РёР±РѕРє
///
/// РўСѓС‚ Сѓ РЅР°СЃ РѕРїРёСЃР°РЅРёРµ С‚СЂРѕР№РєРё С„СѓРЅРєС†РёР№ СѓРїСЂР°РІР»РµРЅРёСЏ РѕС€РёР±РєР°РјРё
/// РЅСѓ Рё РєР»Р°СЃСЃР° РёСЃРєР»СЋС‡РµРЅРёР№. РќР° СЃР°РјРѕРј РґРµР»Рµ РІР°Рј РїРѕРЅР°РґРѕР±РёС‚СЃСЏ 
/// С‚РѕР»СЊРєРѕ РѕРЅ, Р¤СѓРЅРєС†РёРё С‚Р°Рє, РІ РЅР°РіСЂСѓР·РєСѓ)
/// Р›СЋР±Р°СЏ С„СѓРЅРєС†РёСЏ РґРІРёР¶РєР° РјРѕР¶РµС‚ РєРёРґР°С‚СЊ РёРјРµРЅРЅРѕ СЌС‚Рѕ
/// РёСЃРєР»СЋС‡РµРЅРёРµ.

#include "nova_log.h"

namespace nova {


#define E_SUCCESS										0

#define EXIT_OK											0
///////////ERRORS////////////////////

#define MEM_ERROR										-1
#define BAD_OPERATION									-2
#define BAD_OPERATION_WITH_STREAM						-3
#define FATAL_OPERATION									-4

//------ Console Errors ---------------
#define CONSOLE_ENPTY									-10
#define BAD_CHAR										-11
#define ERROR_RESOLVE_ARG								-12
#define UNKNOW_COMMAND									-13

#define NOVA_EXP(x,y)	nova::NovaExp(__FILE__, __LINE__, x, y)

// РїСЂРѕРІРµСЂРєР° РѕС€РёР±РѕРє

/// \brief РћР±СЂР°Р±РѕС‚РєР° РѕС€РёР±РєРё.
///
/// Р§С‚Рѕ РґРµР»Р°РµС‚ СЌС‚Р° С„СѓРЅРєС†РёСЏ? Р’СЃРµ РїСЂРѕСЃС‚Рѕ, РѕРїСЂРµРґРµР»СЏРµС‚ С‚РёРї РѕС€РёР±РєРё
/// РІРїРёСЃС‹РІР°РµС‚ РёРјСЏ РѕР±СЉРµРєС‚Р° Рё СЃРѕРѕР±С‰РµРЅРёРµ РІ С„Р°Р№Р».
/// Р•СЃР»Рё РµСЃС‚СЊ РЅСѓР¶РґР° С‚Рѕ РјРѕР¶РЅРѕ РёСЃРїРѕР»СЊР·РѕРІР°С‚СЊ СЌС‚Сѓ С„СѓРЅРєС†РёСЋ РІ РїСЂРѕРіСЂР°РјРјРµ,
/// РЅРѕ СЌС‚Рѕ РїРѕ СѓСЃРјРѕС‚СЂРµРЅРёСЋ, РјРµС‚РѕРґ FixError() РєР»Р°СЃСЃР° NovaExp
/// РєР°Рє СЂР°Р· С‚Р°РєРё РµРµ Рё РёСЃРїРѕР»СЊР·СѓРµС‚, РїРѕСЌС‚РѕРјСѓ РЅР°РґРѕР±РЅРѕСЃС‚СЊ РІ 
/// РІ РІС‹Р·РѕРІРµ РµРµ РЅР°РїСЂСЏРјСѓСЋ РѕС‚РїР°РґР°РµС‚))

/// \param code РљРѕРґ РѕС€РёР±РєРё.
/// \param mes РЎРѕРѕР±С‰РµРЅРёРµ СЃ С‚РµРєСЃС‚РѕРј РѕС€РёР±РєРё
/// \param pp РЈРєР°Р·Р°С‚РµР»СЊ РЅР° РѕР±СЉРµРєС‚ РіРґРµ РІРѕР·РЅРёРєР»Р° РѕС€РёР±РєР° (РёСЃРїРѕР»СЊР·СѓРµС‚СЃСЏ СЂРµРґРєРѕ)
extern "C" NOVA_EXPORT int STDCALL__ error(int code, nstring & mes, CBase *pp);

/// \brief Р’Р·СЏС‚СЊ РєРѕРґ РїСЂРѕС€Р»РѕР№ РѕС€РёР±РєРё
///
/// РџСЂРѕСЃС‚Рѕ РІРѕР·РІСЂР°С‰СЏРµС‚ РєРѕРґ РїСЂРѕС€Р»РѕР№ РѕС€РёР±РєРё
/// РЅСѓ РІРѕС‚ РЅР°РїСЂРёРјРµСЂ РєР°Рє СЌС‚Рѕ РјРѕР¶РЅРѕ СЋР·Р°С‚СЊ
/// \code
///	#include "error.h"
///
/// ...................
/// nova::error(nova::last_error(), nstring("Some error"), NULL);
/// \endcode
///
extern "C" NOVA_EXPORT int last_error();

/// \brief РћР±РЅСѓР»СЏРµС‚ РїСЂРѕС€Р»СѓСЋ РѕС€РёР±РєСѓ
///
/// Р•СЃР»Рё РЅР°РґРѕР±РЅРѕСЃС‚Рё РІ РєРѕРґРµ РѕС€РёР±РєРё РЅРµС‚, С‚Рѕ РјРѕР¶РЅРѕ РѕР±РЅСѓР»РёС‚СЊ!
/// \code
///	#include "error.h"
///
/// ...................
/// nova::error(nova::last_error(), nstring("Some error"), NULL);
/// nova::drop_last_error();
/// \endcode
extern "C" NOVA_EXPORT void drop_last_error();

/// \class NovaExp
/// \brief РљР»Р°СЃСЃ РёСЃРєР»СЋС‡РµРЅРёР№
///
/// РЎРѕР±СЃС‚РІРµРЅРЅРѕ СЌС‚РѕС‚ РєР»Р°СЃСЃ РёСЃРїРѕР»СЊР·СѓРµС‚СЃСЏ РІСЃРµРјРё С„СѓРЅРєС†РёСЏРјРё РґРІРёР¶РєР°
/// С…РѕС‚РёС‚Рµ РїРѕР№РјР°С‚СЊ РёСЃРєР»СЋС‡РµРЅРёРµ - Р»РѕРІРёС‚Рµ РЅР° РЅРµРіРѕ)))

class NOVA_EXPORT NovaExp : public std::exception, public CBase
{
/// \brief РЎРѕРѕР±С‰РµРЅРёРµ РѕР± РѕС€РёР±РєРµ
	nstring e_mes;
/// \brief РљРѕРґ РѕС€РёР±РєРё
	int code;
public:
/// \brief Р”РµС„РѕР»С‚РЅС‹Р№ РєРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ
	NovaExp();
/// \brief РљРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ
///
/// \param mes РЎРѕРѕР±С‰РµРЅРёРµ РѕР± РѕС€РёР±РєРµ
/// \param _code РљРѕРґ РѕС€РёР±РєРё
/// \param file РёСЃС…РѕРґРЅС‹Р№ С„Р°Р№Р»
/// \param line РЅРѕРјРµСЂ СЃС‚СЂРѕРєРё РІ РёСЃС…РѕРґРЅРѕРј С„Р°Р№Р»Рµ
	NovaExp(cchar file[], int line, cchar * mes, int _code);

/// \param mes РЎРѕРѕР±С‰РµРЅРёРµ РѕР± РѕС€РёР±РєРµ
/// \param _code РљРѕРґ РѕС€РёР±РєРё
	NovaExp(cchar * mes, int _code);

/// \brief РљРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ
///
/// \param _code РљРѕРґ РѕС€РёР±РєРё
	NovaExp(int _code);

/// \brief Р’РѕР·РІСЂР°С‰СЏРµС‚ СЃРѕРѕР±С‰РµРЅРёРµ РѕР± РѕС€РёР±РєРµ
	cchar * what() const ;
/// \brief Р’РѕР·РІСЂР°С‰СЏРµС‚ СЃРѕРѕР±С‰РµРЅРёРµ РѕР± РѕС€РёР±РєРµ РІ РІРёРґРµ nstring
	nstring Message();

/// \brief Р’РѕР·РІСЂР°С‰СЏРµС‚ РєРѕРґ РѕС€РёР±РєРё
	int GetCode();

/// \brief РћР±СЂР°Р±РѕС‚С‡РёРє РѕС€РёР±РєРё
///
/// Р’С‹Р·С‹РІР°РµС‚ С„СѓРЅРєС†РёСЋ error() РєРѕС‚РѕСЂР°СЏ РѕРїРёСЃР°РЅРЅР° РІС‹С€Рµ!
/// \param pobject РЈРєР°Р·Р°С‚РµР»СЊ РЅР° РѕР±СЉРµРєС‚ РіРґРµ СЃРіРµРЅРµСЂРµРЅРЅРѕ РёСЃРєР»СЋС‡РµРЅРёРµ.
/// \code
/// try {
/// .....................
/// }
///	catch(NovaExp & exp)
///	{
///		cerr << exp.Message() << endl;
///		exp.FixError();
/// }
/// \endcode
	inline void FixError(CBase *pobject = NULL)
	{
		error(code, e_mes, pobject);
	}
	/// \brief Р”РµСЃС‚СЂСѓРєС‚РѕСЂ
	~NovaExp() throw() {}
};

}
