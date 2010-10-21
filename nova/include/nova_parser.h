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

/// \file nova_parser.h
/// \brief Текстовый парсер
///
/// Подумал вынести парсер в отдельный модуль
/// Он еще нам понадобится и не только для конфигов.

#include "nova_tree.h"

namespace nova {

#define LINE_LENGTH			300

/// \class CParser
/// \brief Парсер
///
/// Для использования функций парсера советую наследоваться от этого
/// класса.
class NOVA_EXPORT CParser
{
private:

	CTreeNode<nstring> * cnode;
	size_t found;
	nstring parstr;
	char value[LINE_LENGTH];

public:

/// \brief Вектор строк
	typedef stl<nstring>::vector values;

/// \brief Парсер входной строки
///
/// Строка поступает в виде ___command____arg1___arg2___arg3__....
/// в итоге мы получаем массив отдельных строк command,arg1,arg2.arg3.
/// \param input Входная строка
/// \return Выходная вершина дерева.
	CTreeNode<nstring> * ParseStringTree(nstring & input);

/// \brief Парсер входной строки
///
/// Строка поступает в виде ___command____arg1___arg2___arg3__....
/// в итоге мы получаем массив отдельных строк command,arg1,arg2.arg3.
/// \param input Входная строка
/// \return Выходной массив строк
	values ParseString(nstring input);

/// \brief Рекурсивный парсер входной строки
///
/// Строка поступает в виде ___command____arg1___arg2___arg3__....
/// в итоге мы получаем массив отдельных строк command,arg1,arg2.arg3.
/// \param input Входная строка
/// \param result Выходной массив строк
	nInt32 ParseStringRecurse(nstring input, values & result);

/// \brief Рекурсивный парсер входной строки
///
/// Строка поступает в виде ___command____arg1___arg2___arg3__....
/// в итоге мы получаем массив отдельных строк command,arg1,arg2.arg3.
/// \param input Входная строка
/// \return node Выходная вершина дерева.
	CTreeNode<nstring> * ParseStringRecurseTree(nstring & input);

/// \brief Преобразовать вершину дерева в массив строк
	values NodeToValues(CTreeNode<nstring> * node);
/// \brief Преобразовать массив строк в вершину дерева
	CTreeNode<nstring> * ValuesToNode(values & input);
};

}
