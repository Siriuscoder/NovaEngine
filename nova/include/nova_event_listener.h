п»ї/***************************************************************************
 *   Copyright (C) 2009 by Sirius										   *
 *	 Vdov Nikita Sergeevich	(c)											   *
 *	 siriusnick@gmail.com												   *
 *																		   *
 *	 This source file is part of NovaEngine								   *
 *	 (Object-oriented Game Development Engine)							   *
 *																		   *
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

#include "nova_object_constructor.h"

namespace nova
{

class NOVA_EXPORT CEventListener : public CObjectConstructor
{
public:

	CEventListener() {}
	virtual ~CEventListener() {}

	virtual void CommonListener() {}
};

class NOVA_EXPORT CListenerInterface
{
public:

	typedef stl<CEventListener *>::vector TListeners;
protected:

	TListeners mListeners;

public:

	CListenerInterface();

	~CListenerInterface();

	void AddListener(CEventListener * listener);

	void RemoveListener(CEventListener * listener);

	void RemoveListener(nova::uint listener);

	void RemoveAllListeners();

	CEventListener * GetListener(nova::uint i);

	nova::uint GetListenersCount();
};

}