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

#include "nova_fonts.h"
#include "nova_gui_renderer.h"

namespace nova
{

typedef struct _Point
{
	nova::nReal x;
	nova::nReal y;
} *pPoint, Point;

class NOVA_EXPORT PanelBase : public nova::CBase
{
protected:
	Point location;

	nReal width;
	nReal height;

// Указатель на текстурный менеджер
///
// Номер соответсвующей текстуры в менеджере 
///
// Указатель на камеру
///
// Указатель на менеджер камер
	nova::nUInt32 transparency;

public:

	PanelBase() : width(0), height(0),
		transparency(0), CBase("PanelBase") 
	{
		location.x = 0.0f;
		location.y = 0.0f;
	}

	~PanelBase() {}

	virtual void Draw( void ) = 0;
	virtual void Init( void ) = 0;
	virtual void Release( void ) = 0;

	void SetLocation(nReal x, nReal y);
	void SetLocation(Point p);
	Point GetLocation();

	void SetWidth(nReal w);
	void SetHeight(nReal h);
	nReal GetWidth();
	nReal GetHeight();

	void SetTransparency(nova::nUInt32 tr);

	void Move(Point p);
	void Move(nReal x, nReal y);
};



}