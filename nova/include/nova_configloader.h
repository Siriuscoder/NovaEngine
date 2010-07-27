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

/// \file nova_configloader.h
/// \brief Файл с описанием парсера конфигов.
///
/// Вы не обязанны пользоваться именно этим классом
/// и использовать конфиги вообще? это вам решать
/// я подумал, что это будет не лишним.


#include "nova_parser.h"

namespace nova {


/// \class CConfig
/// \brief Класс работы с конфигурационными файлами
///
/// Класс построенн на шаблоне дерева, пользователь может 
/// получить набор строк, дальше его работа.
/// Например такой конфиг.
/// \code
/// MYCONF		vers_0.1
/// RESOLUTION	800 600
/// 
/// BIT_RATE	24
/// \endcode
/// Пример
/// \code 
/// CConfig conf(nstring("./myconfig.conf"));
/// 
/// conf.MakeTree();
///
/// stl<nstring>::vector res = conf.GetNodeValues(nstring("RESOLUTION"));
/// // res ("800", "600")
/// \endcode
class NOVA_EXPORT CConfig : public CBase, public CParser
{
private:
	CTree<nstring> conftree;
	nstring filename;
	uint parseflag;

public:
/// \brief Флаг использования рекурсивного парсера
	static const uint RECURSIVE = 0x12;
/// \brief Флаг тспользования обычного парсера.
	static const uint COMMON = 0x13;
/// \brief Конструктор
///
/// \param flag флаг парсера
	CConfig(uint flag = COMMON);

/// \brief Конструктор 
///
/// \param _filename Путь к файлу конфига.
/// \param flag флаг парсера
	CConfig(nstring & _filename, uint flag = COMMON);
/// \brief Деструктор
	~CConfig();

/// \brief Парсер
///
/// Открывает файл, парсит конфиг, строит дерево.
	int MakeTree();
/// \brief Берет указатель на вершину дерева по имени.
	CTreeNode<nstring> * GetNodePointer(nstring & node);
/// \brief Берет вектор параметров из указанной вершины дерева.
	values GetNodeValues(nstring & node);
/// \brief Берет указатель на дерево разбора.
	CTree<nstring> * GetTree();

/// \brief Очищяет дерево.
	void FreeTree();

/// \brief Сохраняет в конфиг текущее дерево параметров.
///
/// Если есть загрузка и построение дерева из файла, то почему 
/// бы и не сделать сохрание дерева параметров.
/// \param tree входное заполненное дерево.
/// \param _filename имя выходного файла.
	int SaveTree(CTree<nstring> & tree, nstring & _filename);
};









}
