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
/// \file nova_memory.h
/// \brief Функционал для работы с памятью
///
/// Тут у нас аллокатор для STL ну и не только, несколько шаблонов
/// для выделения и освобождения памяти, несколько переопределений,
/// всего понемногу)

#pragma once


#include "nova_common_allocator.h"

// небольшой менеджер памяти
// следит за памятью, при сбое возвращает ошибку, генерирует исключение
#include <new>

#define GET_ONE					0x01
#define GET_MANY				0x02


namespace nova {



/// \brief Аллокатор STL
///
/// Мне встала неоходимость заменить аллокатор памяти в stl на аллокатор винни.
/// тут есть 2 варианта или мы перегружаем new/delete или пишем stl аллокатор.
/// так как глобально перешружать new/delete не очень красиво (Эту проблему я решил
/// используя корневой аллокатор CObjectConstructor), то я всетаки решил наследовать std::allocator
template<class usertype> class NOVA_EXPORT CNovaAlloc : public std::allocator<usertype>
{
public:
/// \brief Указатель на тип
	typedef typename std::allocator<usertype>::pointer npointer;
	typedef typename std::allocator<usertype>::size_type nsize_type;

/// \brief Это нам не понадобится
	template<class _Other> struct rebind
	{
        typedef CNovaAlloc<_Other> other;
	};

/// \brief Конструктор копирования
	template<class _Other> CNovaAlloc(const CNovaAlloc<_Other> & p) throw() { }

/// \brief Оператор присваивания (копирование)
    template<class _Other> CNovaAlloc<usertype> & operator=(const CNovaAlloc<_Other> & p)
	{
        return (*this);
    }

/// \brief Конструктор копирования
	CNovaAlloc(const CNovaAlloc<usertype> & p) throw() {}

/// \brief Конструктор
	CNovaAlloc() throw() {}

/// \brief Деструктор
	~CNovaAlloc() throw() {}

/// \brief Выделяет память
///
/// Тут уже интересней, использует функционал аллокотора винни (вообщем
/// это и есть аллокатор винни, только заключен в класс)
/// \param n количество элементов данного типа
/// \return указатель на блок выделенной памяти или NULL;
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

/// \brief Освобождает выделенный блок
///
///	\param p указатель на блок памяти
/// \param n количество элементов(его можно не использовать и ставить смело 0, для удаления
/// блока памяти аллокатору винни не нужно количество элементов.
/// \see allocate
    void deallocate(npointer p, nsize_type n)
	{
		//winnie_allocator::Free(p);
		gDefaultAllocator.FreeMemory(p);
    }
};


/// \brief Класс с переопределениями основных классов stl.
///
/// Новые типы ориентированны на аллокатор CNovaAlloc
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

/// \brief Переопределение класса string для работы с аллокатором памяти CNovaAlloc
///
/// (простая одно байтовая строчка char)
typedef basic_string<char, char_traits<char>, CNovaAlloc<char> > nstring;
/// \brief Переопределение класса wstring для работы с аллокатором памяти CNovaAlloc
///
/// (двухбайтовая строчка юникод UTF-16)
typedef basic_string<wchar_t, char_traits<wchar_t>, CNovaAlloc<wchar_t> > nwstring;

typedef std::basic_stringstream< char, char_traits<char>, nova::CNovaAlloc<char> > nstringstream;
typedef std::basic_stringstream< wchar_t, char_traits<wchar_t>, nova::CNovaAlloc<wchar_t> > nwstringstream;

/// \brief Выделяет память
///
/// Тоже самое что и CNovaAlloc только в виде функций.
/// \see CNovaAlloc
template <class usertype> inline usertype * getmem(usertype * pointer) throw()
{
	CNovaAlloc<usertype> allocator;
	pointer = allocator.allocate(1);

	return pointer;
}

/// \brief Освобождает выделенный блок
///
/// Тоже самое что и CNovaAlloc только в виде функций.
/// \see CNovaAlloc
template <class usertype> inline void freemem(usertype * pointer) throw()
{
	CNovaAlloc<usertype> allocator;
	allocator.deallocate(pointer, 0);
}

/// \brief Выделяет память
///
/// Тоже самое что и CNovaAlloc только в виде функций.
/// \see CNovaAlloc
template <class usertype> inline usertype * getmem(usertype * pointer, const size_t size) throw()
{
	CNovaAlloc<usertype> allocator;
	pointer = allocator.allocate(size);

	return pointer;
}

/// \brief Освобождает выделенный блок
///
/// Тоже самое что и CNovaAlloc только в виде функций.
/// \see CNovaAlloc
template <class usertype> inline void freemems(usertype * pointer) throw()
{
	CNovaAlloc<usertype> allocator;
	allocator.deallocate(pointer, 0);
}

}


