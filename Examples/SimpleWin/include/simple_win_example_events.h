п»ї/***************************************************************************
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

#include "nova_common_include.h"


class CMyEventHandler : public nova::CEventConveyor
{

public:
	CMyEventHandler();
	~CMyEventHandler();

	void OnKeyUp(int key, int mode);
	void OnQuit(void);
	void OnRender(void);
	void OnTimer(int);
	void OnChar(nova::word s, int m);
	void OnMouseMove(int x, int y, int code);

};
