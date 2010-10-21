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
#pragma once

/// \file nova_console.h
/// \brief Файл содержит реализацию игровой консоли.
///
/// Реализованны классы консоли и менеджер консолей.

#include "nova_fonts.h"
#include "nova_parser.h"

namespace nova {

#define COMMAND_LEN										100

/// \struct nv_console_pram
/// \brief Строчные параметры консоли
///
/// Общий набор параметров консоли для инициализации.
struct nv_console_pram
{
/// \brief Высота строчек (в пикселях)
	uint newline;
/// \brief Расстоние левой границы консоли (в пикселях)
	uint leftclear;
/// \brief Дистанция между буквами (в пикселях)
	uint distance;
/// \brief Расстояние вехней границы консоли (в пикселях)
	uint topclear;
/// \brief Расстояние нижней границы консоли (в пикселях)
	uint bottomclear;
/// \brief Расстояние правой границы консоли (в пикселях)
	uint rightclear;
/// \brief Ширина табуляции (в пикселях)
	uint tab;
};

/// \struct nv_console_init
/// \brief Общие консоли
struct nv_console_init
{
/// \brief Тип шрифта
///
/// Как я говорил, движок поддерживает два типа шрифтов
/// TrueType Font, Win GDI Font. Второй случай конечно только для винды,
/// поэтому на других системах его видно не будет, на винде же есть выбор.
/// \attention Если вы ставите подложку SDL, вы не можете использовать Win GDI Font.
/// Это связанно с тем, что для Win GDI Font нужен так называемый графический контекст окна
/// который из SDL не вытащить. поэтому для этого типа шрифтов можно использовать только подложку Win.
/// Для TrueType Font можно использовать любую.
	uint fonttype;
/// \brief Размер экрана
///
/// x ширина. у высота.
	POINT screen;
/// \brief Максимальное количество символов в консоли.
	uint maxchar;
/// \brief Структура строчных параметров.
///
/// \see nv_console_pram
	nv_console_pram param;
};

/// \struct nv_console_char
/// \brief Символ консоли.
///
/// Содержит два поля код символа и цвет символа
/// таким образом каждый символ может быть разного цвета)
struct nv_console_char
{
/// \brief Код символа (UTF-8)
	wchar_t unicode;
/// Цвет в формате RGB 
///
/// Для установки цвета пользуйтель макросом N_RGB (x, x, x)
	CColorRGB color;
};

/// \brief Пустой символ. (Завершающий)
const nv_console_char empty = {0};

/// \brief Вектор строк
typedef stl<nstring>::vector args;

/// \class CConsoleBase
/// \brief Базовая консоль
/// 
/// Тут содержится базовое описание консоли.
class NOVA_EXPORT CConsoleBase : public CBase
{
protected:
/// \brief Буфер строки
	stl<nv_console_char>::vector console_string;

/// \brief Параметры консоли
	nv_console_init			initdata;
	CColorRGB				color;

	/// \brief Идентификатор OGL текстуры, которая должна будет рисоваться на заднем фоне.
	CTexturePtr backtexture;

/// \brief Устанавливает параметры камеры для отрисовки консоли
///
/// Ортографическая проекция, координатная сетка полностью совпадает с пикселями экрана
/// тоесть координаты x = 10, y = 15 будут измеряться именно в пикселях.
	void SetProjection();
/// \brief Отрисовка фона косоли.
	void DrawBackGround();
/// \brief Очистка буфера строки.
	void ClearStr();

public:
/// \brief Отрисовка консоли.
	virtual int Paint() = 0;

/// \brief Конструктор.
	CConsoleBase(char * _name) : CBase(_name) 
	{
		color = CColorRGB(0.7f, 0.7f, 0.7f);
	}

	inline void SetDefaultTextColor(CColorRGB & color)
	{
		this->color = color;
	}

	inline void SetTexture(CTexturePtr back)
	{
		backtexture = back;
	}

	inline CTexturePtr GetTexture()
	{
		return backtexture;
	}
	
};

/// \class CConsole
/// \brief Консоль
class NOVA_EXPORT CConsole : public CConsoleBase, public CParser
{
	friend class CConsoleManager;
private:
	uint		position;
	nReal		dy;
	int			ypos;
	CFontPtr	font;


public:

/// \brief Конструктор.
	CConsole(nv_console_init * _init);
/// \brief Деструктор
	~CConsole();

/// \brief Вывести строку в консоль.
///
/// \param message строка с завершающим нулем!!
/// \param pc цвет строки
/// \code
/// console->writelen("Test!!!", N_RGB(150, 40, 130));
/// \endcode
    void WriteLen(const wchar_t *message, const CColorRGB & pc);
	void WriteLen(const wchar_t *message);
	void WriteLen(const char *message, const CColorRGB & pc);
	void WriteLen(const char *message);

    void WriteLen(const nstring & message, const CColorRGB & pc);
	void WriteLen(const nwstring & message, const CColorRGB & pc);
	void WriteLen(const nstring & message);
	void WriteLen(const nwstring & message);
/// \brief Вывести символ в консоль.
///
/// \param bb символ
/// \param pc цвет символа
/// \code
/// console->writelen('c', N_RGB(150, 40, 130));
/// \endcode
    int PutChar(const wchar_t bb, const CColorRGB & pc);
	int PutChar(const char bb, const CColorRGB & pc);
/// \brief Вывести строку в консоль.
///
/// Цвет ставится как стандартный белый.
/// \param message строка с завершающим нулем!!
/// \code
/// console->writelen(L"Test!!!");
/// \endcode
	void HoreLine(const wchar_t symbol);

/// \brief Отрисовка консоли 
    int  Paint();
/// \brief Очистка консоли.
	void Clear();

/// \brief Удаление последних х символов консоли.
	int Delete(int x);
  
/// \brief Оператор << 
///
/// Предусмотрен для удобства, чтобы не вызывать функцию writelen
/// можно просто сделать так
/// \code
/// CConsole con;
/// con << "Test!"
/// \endcode
	inline void operator << (const wchar_t *mes)
	{
		this->WriteLen(mes);
	}

};

/// \class CConsoleManager
/// \brief Менеджер консолей
///
/// Позволяет управлять консолями. Вообще говоря, количество консолей
/// не огранмченно. Есть активная консоль, которая отрисовывается с данный момент времени
/// всегда можно переключить на другую консоль.
class NOVA_EXPORT CConsoleManager : public CSingelton<CConsoleManager>, public CBase
{
private:
	stl<CConsole *>::vector consoles;
	int							active;
	int							iter;
	CFontPtr					font;

public:
	
/// \brief Конструктор.
	CConsoleManager();
/// \brief Деструктор
	~CConsoleManager();

/// \brief Создает новую консоль
///
/// \param param Структура параметров новой консоли
/// \see nv_console_init
	int NewConsole(nv_console_init * param);
/// \brief Устанавливает в консоль шрифт.
///
/// Для всех консолей будет использоваться именно этот шрифт.
///
/// \param _font Объект шрифта
	void SetFont(CFontPtr _font);
/// \brief Удаляет выбранную консоль
///
/// \param n Номер консоли (0,1,2..)
	void DeleteConsole(uint n);

/// \brief Удалить все консоли.
	void Clear();
/// \brief Отрисовка кадра активной консоли.
	void Render();

/// \brief Установить активную консоль
///
/// \param n Номер консоли (0,1,2..) 
	void SetActive(uint n);

/// \brief Взять номер активной консоли.
	const int GetActive();
/// \brief Взять адрес активной консоли
	CConsole * GetActivePtr();
/// \brief Взять количество консолей
	const int Count();

/// \brief Оператор []
///
/// Использовать можно так
/// \code
/// CConsoleManager manager;
///	*(manager[2]) << "Test";
/// \endcode
/// \param ind номер консоли которую мы хотим получить.
	inline CConsole * operator[](uint ind)
	{
		if(ind < consoles.size())
			return consoles[ind];

		return NULL;
	}
};

}

