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

#include "nova_error.h"

#ifdef _MSC_VER
#pragma warning (disable:4661)
#endif

namespace nova {

template<class T> class CSingelton
{
protected:
	static T * SingeltonObject;

public:
	CSingelton() throw(nova::NovaExp)
	{
		if(!SingeltonObject)
			SingeltonObject = static_cast<T *>(this);
		//else
			//throw NOVA_EXP("Singelton object already created!", BAD_OPERATION);
	}

	~CSingelton()
	{
		SingeltonObject = NULL;
	}

	static T & GetSingelton()
	{
		return *SingeltonObject;
	}

	static T * GetSingeltonPtr()
	{
		return SingeltonObject;
	}
};

}