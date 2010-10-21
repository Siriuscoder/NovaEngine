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
/// \brief Здесь описан базовый конструктор объектов 
///
/// Ну конечно же он основан на винни аллокаторе.

#include "nova_memory.h"

namespace nova {

/// \class CObjectConstructor
/// \brief Конструктор объектов
///
/// Наследуясь от класса CBase вы автоматически получаете
/// перегруженые new/delete для вашего класса (но это делать совсем не обязательно)
class NOVA_EXPORT CObjectConstructor
{
public:

/// \brief Конструктор
	CObjectConstructor() {};
/// \brief Деструктор
	~CObjectConstructor() {};

/// \brief Оператор new
	void *operator new(size_t size);
/// \brief Оператор delete
	void operator delete(void *p);

/// \brief Оператор new для массивов
	void *operator new[](size_t size);
/// \brief Оператор delete для массивов
	void operator delete[](void *p);

/// \brief Оператор new
/// 
/// Не генерирует исключительных ситуаций
	void *operator new(size_t size, const std::nothrow_t & n);
/// \brief Оператор delete
/// 
/// Не генерирует исключительных ситуаций
	void operator delete(void *p, const std::nothrow_t & n);

/// \brief Оператор new для массивов
/// 
/// Не генерирует исключительных ситуаций
	void *operator new[](size_t size, const std::nothrow_t & n);
/// \brief Оператор delete для массивов
/// 
/// Не генерирует исключительных ситуаций
	void operator delete[](void *p, const std::nothrow_t & n);
/// \brief Максимальный размер блока памяти
	nova::nUInt64 GetMaxBlockSize(void);
};



}
