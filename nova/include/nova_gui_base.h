п»ї/***************************************************************************
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
	nova::real x;
	nova::real y;
} *pPoint, Point;

class NOVA_EXPORT PanelBase : public nova::CBase
{
protected:
	Point location;

	real width;
	real height;

// РЈРєР°Р·Р°С‚РµР»СЊ РЅР° С‚РµРєСЃС‚СѓСЂРЅС‹Р№ РјРµРЅРµРґР¶РµСЂ
///
// РќРѕРјРµСЂ СЃРѕРѕС‚РІРµС‚СЃРІСѓСЋС‰РµР№ С‚РµРєСЃС‚СѓСЂС‹ РІ РјРµРЅРµРґР¶РµСЂРµ 
///
// РЈРєР°Р·Р°С‚РµР»СЊ РЅР° РєР°РјРµСЂСѓ
///
// РЈРєР°Р·Р°С‚РµР»СЊ РЅР° РјРµРЅРµРґР¶РµСЂ РєР°РјРµСЂ
	nova::uint transparency;

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

	void SetLocation(real x, real y);
	void SetLocation(Point p);
	Point GetLocation();

	void SetWidth(real w);
	void SetHeight(real h);
	real GetWidth();
	real GetHeight();

	void SetTransparency(nova::uint tr);

	void Move(Point p);
	void Move(real x, real y);
};



}