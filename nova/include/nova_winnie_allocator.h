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

//(c) Winnie  woowoowoow 'at' bk.ru
//fast memory allocation routines.
/// \file nova_allocator.h
/// \brief Аллокатор памяти используемый движком.
///
/// Я позаимствовал аллокатор памяти Винни (gamedev.ru)
/// он вполне быстрый и простой для использования.
/// ЗЫ Автору спасибо ))
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
/// \brief Аллокатор находится в простоанстре имен winnie_allocator 
///
/// Вынес все в отдельное пространство имен, для наглядности.
/// Для подклучения аллокатора в вашу программу используйте
/// \code
/// using namespace nova::winnie_allocator;
/// \endcode
namespace winnie_allocator
{

const size_t malloc_threshold = 1024;
const unsigned first_os_block_size_log = 20; //1 MB.
const unsigned max_lists_num = 31;

/// \struct Statistic
/// \brief Статистика использования памяти
///
/// Используется для распечатки дампа памяти
struct Statistic
{
/// \brief Страницы выделенных блоков
  size_t num_allocated_blocks[max_lists_num];
/// \brief Страницы освобожденных блоков
  size_t num_free_blocks[max_lists_num];
/// \brief Всего забранных блоков по страницам.
  size_t num_block_allocations[max_lists_num];
};

#if !defined(DOXYGEN_IGNORE)
extern "C" {
#endif

/// \brief Получить статистику
///
/// \param statistic структура со статистикой.
NOVA_EXPORT void GetAllocInfo(Statistic &statistic);

/// \brief Выделить память.
///
/// \param size размер нужной памяти в байтах
/// \return указатель на свободный блок памяти или NULL
/// \attention То что творится в этой функции называется мистика,
/// Вы можете спользовать этот аллокатор у себя в программе, он очень
/// хорош для малых блоков памяти и очень быстр, если вас не интересуют
/// малые блоки, то используйте malloc/free тут они надежнее, хоть и медленнее
/// В коде движка перегруженны операторы new/delete на этот аллокатор, поэтому тут я использую только его,
/// вы же можете тоже перегрузить эти операторы, или использовать allocator.
/// \see class CNovaAlloc, getmem/freemem
NOVA_EXPORT void *Alloc(const size_t size);

/// \brief Освобождает блок памяти.
///
/// Тоесть переводит занятый блок памяти в лист пустых.
/// \param p указатель на блок памяти.
NOVA_EXPORT void Free(void *p);

/// \brief Перевыделение памяти
///
/// Текущий блок памяти освобождается, захватывается новый с теми же данными
/// \param p указатель на старый блок памяти
/// \param size размер нужной памяти в байтах
/// \return указатель на новый блок памяти или NULL.
NOVA_EXPORT void *Realloc(void *p, const size_t size);

/// \brief Возвращяет примерный размер блока памяти.
///
/// \attention Именно примерный объем!!!! Он равен размеру данной страницы памяти
/// но это не означает, что забранный блок по адресу p именно такого размера.
/// \param p указаетль на выделенный блок памяти.
/// \return Размер.
NOVA_EXPORT size_t MSize(void *p);

/// \brief Полное обнуление всех страниц
///
/// Думаю, что этой штукой лучше не пользоваться, или пользоваться в самом конце программы
/// для очистки утечек памяти.
NOVA_EXPORT void DesroyHeap();

}

class NOVA_EXPORT CWinnieAlloc
{
private:

	CWinnieAlloc() {}
public:

	static void *AllocateMemory(const size_t size);

	static void FreeMemory(void *pmem);

	static void *Reallocate(void *pmem, const size_t size);
};

}

}


