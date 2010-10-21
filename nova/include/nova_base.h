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
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
// Базовый класс
#pragma once

/// \file nova_base.h
/// \brief Базовый заголовок.
///
/// Тут у нас расположен базовый класс
/// коменчу, так, для общего развития))

#include "nova_object_constructor.h"

namespace nova {

#define CLASS_NAME_LEN							30

#define _BASE_CLASS_

/// \class CBase
/// \brief Корневой класс движка
///
/// Вообщемто почти любой класс в пространстве nova наследуется 
/// от этого класса. В своей программе вы так же можете 
/// наследоватся от него (если есть желание), а есть случаи
/// когда это необходимо!
/// \attention Наследуясь от этого класса вы перенимаете наследование от 
/// класса CObjectConstructor, в котором перегруженны операторы new/delete
/// тем самым вы используете для конструирования памяти Винни аллокатор
/// \see nova_allocator.h

class NOVA_EXPORT CBase : public nova::CObjectConstructor
{
protected:
/// \brief Идентификатор объекта
///
/// \attention Присваевается автоматически
	nInt32			mIDObject;

/// \brief Служебное поле
	nInt32			mReserved;

/// \brief Имя класса
///
/// \attention Присваевается автоматически
	char		mObjectName[CLASS_NAME_LEN];

/// \brief указатель на базовый класс
///
/// Можно использовать, можно и нет. как хотите!
	CBase		*mParent;

/// \brief Буффер флагов
///
/// По вашему усморению!
	nByte		mFlags[20];

public:
/// \brief Конструктор
///
/// \param _name имя класса
	CBase(char * _name);

/// \brief Берет имя класса
///
/// \param buff буффер для строки
	void GetClassName(char *buff);

/// \brief Берет идентификатор класса
	nInt32 GetObjectId();

/// \brief Берет размер объекта
///
/// \attention Особо не увлекайтесь этим полем
/// я не знаю точно работает ли оно или нет))
	nUInt32 GetClassSize();

/// \brief Берет размер объекта
///
/// \param pobject указатель на объект
/// \attention Особо не увлекайтесь этим полем
/// я не знаю точно работает ли оно или нет))
	nUInt32 GetClassSizeO(CBase * pobject);

/*	virtual bool operator ==(CBASE & pobject){};
	virtual CBASE operator +(CBASE & pobject){};
	virtual CBASE operator -(CBASE & pobject){};

	virtual void operator +(CBASE & pobject){};
	virtual void operator -(CBASE & pobject){};

	virtual CBASE * operator [](nInt32 index){};
	virtual void operator <<(CBASE & pobject){};
	virtual void operator >>(CBASE & pobject){}; 
*/
};

}
