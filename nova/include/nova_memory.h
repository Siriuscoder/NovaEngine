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
/// \file nova_memory.h
/// \brief Р¤СѓРЅРєС†РёРѕРЅР°Р» РґР»СЏ СЂР°Р±РѕС‚С‹ СЃ РїР°РјСЏС‚СЊСЋ
///
/// РўСѓС‚ Сѓ РЅР°СЃ Р°Р»Р»РѕРєР°С‚РѕСЂ РґР»СЏ STL РЅСѓ Рё РЅРµ С‚РѕР»СЊРєРѕ, РЅРµСЃРєРѕР»СЊРєРѕ С€Р°Р±Р»РѕРЅРѕРІ
/// РґР»СЏ РІС‹РґРµР»РµРЅРёСЏ Рё РѕСЃРІРѕР±РѕР¶РґРµРЅРёСЏ РїР°РјСЏС‚Рё, РЅРµСЃРєРѕР»СЊРєРѕ РїРµСЂРµРѕРїСЂРµРґРµР»РµРЅРёР№,
/// РІСЃРµРіРѕ РїРѕРЅРµРјРЅРѕРіСѓ)

#pragma once


#include "nova_common_allocator.h"

// РЅРµР±РѕР»СЊС€РѕР№ РјРµРЅРµРґР¶РµСЂ РїР°РјСЏС‚Рё
// СЃР»РµРґРёС‚ Р·Р° РїР°РјСЏС‚СЊСЋ, РїСЂРё СЃР±РѕРµ РІРѕР·РІСЂР°С‰Р°РµС‚ РѕС€РёР±РєСѓ, РіРµРЅРµСЂРёСЂСѓРµС‚ РёСЃРєР»СЋС‡РµРЅРёРµ
#include <new>

#define GET_ONE					0x01
#define GET_MANY				0x02


namespace nova {



/// \brief РђР»Р»РѕРєР°С‚РѕСЂ STL
///
/// РњРЅРµ РІСЃС‚Р°Р»Р° РЅРµРѕС…РѕРґРёРјРѕСЃС‚СЊ Р·Р°РјРµРЅРёС‚СЊ Р°Р»Р»РѕРєР°С‚РѕСЂ РїР°РјСЏС‚Рё РІ stl РЅР° Р°Р»Р»РѕРєР°С‚РѕСЂ РІРёРЅРЅРё.
/// С‚СѓС‚ РµСЃС‚СЊ 2 РІР°СЂРёР°РЅС‚Р° РёР»Рё РјС‹ РїРµСЂРµРіСЂСѓР¶Р°РµРј new/delete РёР»Рё РїРёС€РµРј stl Р°Р»Р»РѕРєР°С‚РѕСЂ.
/// С‚Р°Рє РєР°Рє РіР»РѕР±Р°Р»СЊРЅРѕ РїРµСЂРµС€СЂСѓР¶Р°С‚СЊ new/delete РЅРµ РѕС‡РµРЅСЊ РєСЂР°СЃРёРІРѕ (Р­С‚Сѓ РїСЂРѕР±Р»РµРјСѓ СЏ СЂРµС€РёР»
/// РёСЃРїРѕР»СЊР·СѓСЏ РєРѕСЂРЅРµРІРѕР№ Р°Р»Р»РѕРєР°С‚РѕСЂ CObjectConstructor), С‚Рѕ СЏ РІСЃРµС‚Р°РєРё СЂРµС€РёР» РЅР°СЃР»РµРґРѕРІР°С‚СЊ std::allocator
template<class usertype> class NOVA_EXPORT CNovaAlloc : public std::allocator<usertype>
{
public:
/// \brief РЈРєР°Р·Р°С‚РµР»СЊ РЅР° С‚РёРї
	typedef typename std::allocator<usertype>::pointer npointer;
	typedef typename std::allocator<usertype>::size_type nsize_type;

/// \brief Р­С‚Рѕ РЅР°Рј РЅРµ РїРѕРЅР°РґРѕР±РёС‚СЃСЏ
	template<class _Other> struct rebind
	{
        typedef CNovaAlloc<_Other> other;
	};

/// \brief РљРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ РєРѕРїРёСЂРѕРІР°РЅРёСЏ
	template<class _Other> CNovaAlloc(const CNovaAlloc<_Other> & p) throw() { }

/// \brief РћРїРµСЂР°С‚РѕСЂ РїСЂРёСЃРІР°РёРІР°РЅРёСЏ (РєРѕРїРёСЂРѕРІР°РЅРёРµ)
    template<class _Other> CNovaAlloc<usertype> & operator=(const CNovaAlloc<_Other> & p)
	{
        return (*this);
    }

/// \brief РљРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ РєРѕРїРёСЂРѕРІР°РЅРёСЏ
	CNovaAlloc(const CNovaAlloc<usertype> & p) throw() {}

/// \brief РљРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ
	CNovaAlloc() throw() {}

/// \brief Р”РµСЃС‚СЂСѓРєС‚РѕСЂ
	~CNovaAlloc() throw() {}

/// \brief Р’С‹РґРµР»СЏРµС‚ РїР°РјСЏС‚СЊ
///
/// РўСѓС‚ СѓР¶Рµ РёРЅС‚РµСЂРµСЃРЅРµР№, РёСЃРїРѕР»СЊР·СѓРµС‚ С„СѓРЅРєС†РёРѕРЅР°Р» Р°Р»Р»РѕРєРѕС‚РѕСЂР° РІРёРЅРЅРё (РІРѕРѕР±С‰РµРј
/// СЌС‚Рѕ Рё РµСЃС‚СЊ Р°Р»Р»РѕРєР°С‚РѕСЂ РІРёРЅРЅРё, С‚РѕР»СЊРєРѕ Р·Р°РєР»СЋС‡РµРЅ РІ РєР»Р°СЃСЃ)
/// \param n РєРѕР»РёС‡РµСЃС‚РІРѕ СЌР»РµРјРµРЅС‚РѕРІ РґР°РЅРЅРѕРіРѕ С‚РёРїР°
/// \return СѓРєР°Р·Р°С‚РµР»СЊ РЅР° Р±Р»РѕРє РІС‹РґРµР»РµРЅРЅРѕР№ РїР°РјСЏС‚Рё РёР»Рё NULL;
/// \code
///	CNovaAlloc<int> allocator;
/// int * pointer = allocator.allocate(10);
///
/// for(int * i = pointer; i < pointer+10; ++i)
///		*i = 5;
/// // 5 5 5 5 5 5 5 5 5 5
/// pointer.deallocate(p, 10);
/// \endcode

	npointer allocate(nsize_type n)
	{
		//void *p = winnie_allocator::Alloc(n * sizeof(usertype));
		void *p = gDefaultAllocator.AllocMemory(n * sizeof(usertype));

        return reinterpret_cast<npointer>(p);
    }

/// \brief РћСЃРІРѕР±РѕР¶РґР°РµС‚ РІС‹РґРµР»РµРЅРЅС‹Р№ Р±Р»РѕРє
///
///	\param p СѓРєР°Р·Р°С‚РµР»СЊ РЅР° Р±Р»РѕРє РїР°РјСЏС‚Рё
/// \param n РєРѕР»РёС‡РµСЃС‚РІРѕ СЌР»РµРјРµРЅС‚РѕРІ(РµРіРѕ РјРѕР¶РЅРѕ РЅРµ РёСЃРїРѕР»СЊР·РѕРІР°С‚СЊ Рё СЃС‚Р°РІРёС‚СЊ СЃРјРµР»Рѕ 0, РґР»СЏ СѓРґР°Р»РµРЅРёСЏ
/// Р±Р»РѕРєР° РїР°РјСЏС‚Рё Р°Р»Р»РѕРєР°С‚РѕСЂСѓ РІРёРЅРЅРё РЅРµ РЅСѓР¶РЅРѕ РєРѕР»РёС‡РµСЃС‚РІРѕ СЌР»РµРјРµРЅС‚РѕРІ.
/// \see allocate
    void deallocate(npointer p, nsize_type n)
	{
		//winnie_allocator::Free(p);
		gDefaultAllocator.FreeMemory(p);
    }
};


/// \brief РљР»Р°СЃСЃ СЃ РїРµСЂРµРѕРїСЂРµРґРµР»РµРЅРёСЏРјРё РѕСЃРЅРѕРІРЅС‹С… РєР»Р°СЃСЃРѕРІ stl.
///
/// РќРѕРІС‹Рµ С‚РёРїС‹ РѕСЂРёРµРЅС‚РёСЂРѕРІР°РЅРЅС‹ РЅР° Р°Р»Р»РѕРєР°С‚РѕСЂ CNovaAlloc
template<class T, class T1 = void>
class stl
{
public:

/// \brief Vector
	typedef std::vector<T, CNovaAlloc<T> > vector;

/// \brief Map
	typedef std::map<T, T1, less<T>, CNovaAlloc<pair<const T, T1> > > map;

/// \brief List
	typedef std::list<T, CNovaAlloc<T> > list;
};

/// \brief РџРµСЂРµРѕРїСЂРµРґРµР»РµРЅРёРµ РєР»Р°СЃСЃР° string РґР»СЏ СЂР°Р±РѕС‚С‹ СЃ Р°Р»Р»РѕРєР°С‚РѕСЂРѕРј РїР°РјСЏС‚Рё CNovaAlloc
///
/// (РїСЂРѕСЃС‚Р°СЏ РѕРґРЅРѕ Р±Р°Р№С‚РѕРІР°СЏ СЃС‚СЂРѕС‡РєР° char)
typedef basic_string<char, char_traits<char>, CNovaAlloc<char> > nstring;
/// \brief РџРµСЂРµРѕРїСЂРµРґРµР»РµРЅРёРµ РєР»Р°СЃСЃР° wstring РґР»СЏ СЂР°Р±РѕС‚С‹ СЃ Р°Р»Р»РѕРєР°С‚РѕСЂРѕРј РїР°РјСЏС‚Рё CNovaAlloc
///
/// (РґРІСѓС…Р±Р°Р№С‚РѕРІР°СЏ СЃС‚СЂРѕС‡РєР° СЋРЅРёРєРѕРґ UTF-16)
typedef basic_string<wchar_t, char_traits<wchar_t>, CNovaAlloc<wchar_t> > nwstring;

typedef std::basic_stringstream< char, char_traits<char>, nova::CNovaAlloc<char> > nstringstream;
typedef std::basic_stringstream< wchar_t, char_traits<wchar_t>, nova::CNovaAlloc<wchar_t> > nwstringstream;

/// \brief Р’С‹РґРµР»СЏРµС‚ РїР°РјСЏС‚СЊ
///
/// РўРѕР¶Рµ СЃР°РјРѕРµ С‡С‚Рѕ Рё CNovaAlloc С‚РѕР»СЊРєРѕ РІ РІРёРґРµ С„СѓРЅРєС†РёР№.
/// \see CNovaAlloc
template <class usertype> inline usertype * getmem(usertype * pointer) throw()
{
	CNovaAlloc<usertype> allocator;
	pointer = allocator.allocate(1);

	return pointer;
}

/// \brief РћСЃРІРѕР±РѕР¶РґР°РµС‚ РІС‹РґРµР»РµРЅРЅС‹Р№ Р±Р»РѕРє
///
/// РўРѕР¶Рµ СЃР°РјРѕРµ С‡С‚Рѕ Рё CNovaAlloc С‚РѕР»СЊРєРѕ РІ РІРёРґРµ С„СѓРЅРєС†РёР№.
/// \see CNovaAlloc
template <class usertype> inline void freemem(usertype * pointer) throw()
{
	CNovaAlloc<usertype> allocator;
	allocator.deallocate(pointer, 0);
}

/// \brief Р’С‹РґРµР»СЏРµС‚ РїР°РјСЏС‚СЊ
///
/// РўРѕР¶Рµ СЃР°РјРѕРµ С‡С‚Рѕ Рё CNovaAlloc С‚РѕР»СЊРєРѕ РІ РІРёРґРµ С„СѓРЅРєС†РёР№.
/// \see CNovaAlloc
template <class usertype> inline usertype * getmem(usertype * pointer, const size_t size) throw()
{
	CNovaAlloc<usertype> allocator;
	pointer = allocator.allocate(size);

	return pointer;
}

/// \brief РћСЃРІРѕР±РѕР¶РґР°РµС‚ РІС‹РґРµР»РµРЅРЅС‹Р№ Р±Р»РѕРє
///
/// РўРѕР¶Рµ СЃР°РјРѕРµ С‡С‚Рѕ Рё CNovaAlloc С‚РѕР»СЊРєРѕ РІ РІРёРґРµ С„СѓРЅРєС†РёР№.
/// \see CNovaAlloc
template <class usertype> inline void freemems(usertype * pointer) throw()
{
	CNovaAlloc<usertype> allocator;
	allocator.deallocate(pointer, 0);
}

}


