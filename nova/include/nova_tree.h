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

/// \file nova_tree.h
/// \brief Базовое описание структуры дерева
///
/// Думаю, что эта конструкция мне понадобтся в дальнейшем
/// Дерево универсальное, каждая вершина может содержать не
/// ограниченное количество потомков.
/// Реализованно в виде шаблонов.
/// Это только основа, поэтому многие действия нужно будет доделывать
/// в дальнейшем, в зависимости от конкретной задачи.

#include "nova_error.h"

namespace nova {

/// \brief Класс вершина.
///
/// Дерево состоит из такий вершин, причем в кажной 
/// вершине хранится лист указателей на все ее потомки.
template <class usertype> class CTreeNode : public CObjectConstructor
{
protected:

#if !defined(DOXYGEN_IGNORE)
/// \brief Данные которые хранит вершина
	usertype data;

/// \brief Тип указатель на вершину
	typedef CTreeNode<usertype> *tpnode;
/// \brief Лист вершин
	typename nova::stl< tpnode >::list ch_nodes;
	//list<tpnode, CNovaAlloc<tpnode> > ch_nodes;
#endif


public:

/// \brief Конструктор
	CTreeNode() {};

/// \brief Конструктор
///
/// \param _data Информация которую мы хотим воткнуть в вершину
/// практически любая структура данных.
	CTreeNode(usertype _data);

/// \brief Взять инфу вершины.
	usertype GetData();

/// \brief Взять количество потомков.
	nInt32 GetChildrenLen();

/// \brief Взять дочернюю вершину по номеру в листе.
///
/// \attention Будьте внимаетельны с номером!
	CTreeNode<usertype> * GetNode(nInt32 id);

/// \brief Добавить вершину в конец листа
///
/// \param _data данные вершины.
	nInt32 AddNone(usertype & _data);

/// \brief Добавить вершину в конец листа
	nInt32 AddCurrentNone(CTreeNode<usertype> * node);

/// \brief Удалить вершину и все ее дочерние элементы, по номеру листа. 
	void DeleteNode(nInt32 id);
/// \brief Удалить всю секцию дерева полностью.
	void DeleteAll();
/// \brief Деструктор
 	~CTreeNode();
/// \brief Оператор []
///
/// Тоже самое что и GetNode))
	inline CTreeNode<usertype> * operator[](nUInt32 id)
	{
		return GetNode(id);
	}

	typedef usertype iterator;
};

/// \brief Класс дерева.
/// 
/// Собственно все что он содержит это указатель на корневой элемент.
/// \attention Не используйте класс CTreeNode без класса CTree.
/// вы рискуеете потерять корень дерева! это очень важно, держите
/// его на виду всегда! Поэтому я и добавил этот класс, с ним не 
/// пропадете =)
/// \code 
/// CTree<nInt32> tree;
///
/// tree.CreateRoot(3); // Заносим в корень тройку.
/// tree.GetRootElement()->AddNone(2); // добавляем элемент
/// tree.GetRootElement()->AddNone(1);
/// tree.GetRootElement()->AddNone(4);
/// CTreeNode<nInt32> * node = tree.GetRootElement()->GetNode(1);
/// node->AddNone(5);
/// node->AddNone(6);
/// // у нас получилось вот такое дерево.
/// //            3
/// //         /  |  \
/// //        2   1   4
/// //           / \
/// //          5   6
/// \endcode
template <class usertype> class CTree : public CBase
{
	CTreeNode<usertype> * root;
public:
/// \brief Взять корневой элемент дерева.
	CTreeNode<usertype> * GetRootElement();
/// \brief Создать корневой элемент дерева.
///
/// \param _data ваши данные .
	void CreateRoot(usertype & _data);

/// \brief Конструктор
	CTree() : CBase((char*)"CTree"), root(NULL) {}

/// \brief Освободить дерево.
	void FreeAll();
/// \brief Деструктор.
	~CTree();
};


template <class usertype> usertype CTreeNode<usertype>::GetData()
{
	return data;
}

template <class usertype> CTreeNode<usertype> * CTreeNode<usertype>::GetNode(nInt32 id)
{
	if(id < 0)
		throw NovaExp("CTreeNode::GetNode - id < 0 !!", BAD_OPERATION);
	typename stl< CTreeNode<usertype> *>::list::iterator it;
	//list< void *, CNovaAlloc<void *> >::iterator it;
	it = ch_nodes.begin();
	for(nInt32 i = 0; i < id; ++i, it++);

	return *it;
}


template <class usertype> CTreeNode<usertype>::CTreeNode(usertype _data)
{
	data = _data;
	ch_nodes.clear();
}

template <class usertype> CTreeNode<usertype>::~CTreeNode()
{
	DeleteAll();
}

template <class usertype> nInt32 CTreeNode<usertype>::GetChildrenLen()
{
	return ch_nodes.size();
}

template <class usertype> nInt32 CTreeNode<usertype>::AddNone(usertype &_data)
{
	CTreeNode<usertype> * node = new CTreeNode<usertype>(_data);
	ch_nodes.push_back(node);

	return ch_nodes.size()-1;
}

template <class usertype> nInt32 CTreeNode<usertype>::AddCurrentNone(CTreeNode<usertype> * node)
{
	ch_nodes.push_back(node);

	return ch_nodes.size()-1;
}

template <class usertype> void CTreeNode<usertype>::DeleteNode(nInt32 id)
{
	if(id < 0)
		throw NovaExp("CTreeNode::DeleteNode - id < 0 !!", BAD_OPERATION);

	try 
	{
		typename stl< CTreeNode<usertype> *>::list::iterator it;
		it = ch_nodes.begin() + id;
		CTreeNode<usertype> * node = *it;
		
		node->DeleteAll();
		delete node;
		ch_nodes.erase(it);
	} 
	catch(exception & ex) 
	{
		throw NovaExp(ex.what(), MEM_ERROR);
	}
}

template <class usertype> void CTreeNode<usertype>::DeleteAll()
{
	try 
	{
		typename stl< CTreeNode<usertype> *>::list::iterator it;
		for(it = ch_nodes.begin(); it != ch_nodes.end(); it++)
		{
			CTreeNode<usertype> * node = *it;
			node->DeleteAll();

			delete node;
			node = NULL;
		}
		ch_nodes.clear();
	}
	catch(exception & ex) 
	{
		throw NovaExp(ex.what(), MEM_ERROR);
	}
}


template <class usertype> CTreeNode<usertype> * CTree<usertype>::GetRootElement()
{
	return root;
}

template <class usertype> void CTree<usertype>::CreateRoot(usertype &_data)
{
	root = new CTreeNode<usertype>(_data);
}

template <class usertype> void CTree<usertype>::FreeAll()
{
	if(root)
	{
		root->DeleteAll();
		delete root;
		root = NULL;
	}
}
	

template <class usertype> CTree<usertype>::~CTree()
{
	FreeAll();
}

}
