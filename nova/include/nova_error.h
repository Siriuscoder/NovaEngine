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
// Общие функции проверки ошибок
#pragma once

/// \file nova_error.h
/// \brief Общие функции проверки ошибок
///
/// Тут у нас описание тройки функций управления ошибками
/// ну и класса исключений. На самом деле вам понадобится 
/// только он, Функции так, в нагрузку)
/// Любая функция движка может кидать именно это
/// исключение.

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

// проверка ошибок

/// \brief Обработка ошибки.
///
/// Что делает эта функция? Все просто, определяет тип ошибки
/// вписывает имя объекта и сообщение в файл.
/// Если есть нужда то можно использовать эту функцию в программе,
/// но это по усмотрению, метод FixError() класса NovaExp
/// как раз таки ее и использует, поэтому надобность в 
/// в вызове ее напрямую отпадает))

/// \param code Код ошибки.
/// \param mes Сообщение с текстом ошибки
/// \param pp Указатель на объект где возникла ошибка (используется редко)
extern "C" NOVA_EXPORT nInt32 STDCALL__ error(nInt32 code, nstring & mes, CBase *pp);

/// \brief Взять код прошлой ошибки
///
/// Просто возвращяет код прошлой ошибки
/// ну вот например как это можно юзать
/// \code
///	#include "error.h"
///
/// ...................
/// nova::error(nova::last_error(), nstring("Some error"), NULL);
/// \endcode
///
extern "C" NOVA_EXPORT nInt32 last_error();

/// \brief Обнуляет прошлую ошибку
///
/// Если надобности в коде ошибки нет, то можно обнулить!
/// \code
///	#include "error.h"
///
/// ...................
/// nova::error(nova::last_error(), nstring("Some error"), NULL);
/// nova::drop_last_error();
/// \endcode
extern "C" NOVA_EXPORT void drop_last_error();

/// \class NovaExp
/// \brief Класс исключений
///
/// Собственно этот класс используется всеми функциями движка
/// хотите поймать исключение - ловите на него)))

class NOVA_EXPORT NovaExp : public std::exception, public CBase
{
/// \brief Сообщение об ошибке
	nstring e_mes;
/// \brief Код ошибки
	nInt32 code;
public:
/// \brief Дефолтный конструктор
	NovaExp();
/// \brief Конструктор
///
/// \param mes Сообщение об ошибке
/// \param _code Код ошибки
/// \param file исходный файл
/// \param line номер строки в исходном файле
	NovaExp(nCChar file[], nInt32 line, nCChar * mes, nInt32 _code);

/// \brief Конструктор
///
/// \param mes Сообщение об ошибке
/// \param _code Код ошибки
/// \param file исходный файл
/// \param line номер строки в исходном файле
	NovaExp(nCChar file[], nInt32 line, const nstring & mes, nInt32 _code);

/// \param mes Сообщение об ошибке
/// \param _code Код ошибки
	NovaExp(nCChar * mes, nInt32 _code);

/// \param mes Сообщение об ошибке
/// \param _code Код ошибки
	NovaExp(const nstring & mes, nInt32 _code);

/// \brief Конструктор
///
/// \param _code Код ошибки
	NovaExp(nInt32 _code);

/// \brief Возвращяет сообщение об ошибке
	nCChar * what() const ;
/// \brief Возвращяет сообщение об ошибке в виде nstring
	nstring Message();

/// \brief Возвращяет код ошибки
	nInt32 GetCode();

/// \brief Обработчик ошибки
///
/// Вызывает функцию error() которая описанна выше!
/// \param pobject Указатель на объект где сгенеренно исключение.
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
	/// \brief Деструктор
	~NovaExp() throw() {}
};

}
