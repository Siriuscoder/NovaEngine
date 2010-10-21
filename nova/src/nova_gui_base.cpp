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
#include "nova_stable_precompiled_headers.h"

#include "nova_gui_base.h"

namespace nova
{

void PanelBase::SetLocation(nReal x, nReal y)
{
	location.x = x;
	location.y = y;
}

void PanelBase::SetLocation(Point p)
{
	location = p;
}

Point PanelBase::GetLocation()
{
	return location;
}

void PanelBase::SetWidth(nReal w)
{
	this->width = w;
}

void PanelBase::SetHeight(nReal h)
{
	this->height = h;
}

nReal PanelBase::GetWidth()
{
	return width;
}

nReal PanelBase::GetHeight()
{
	return height;
}

void PanelBase::SetTransparency(nova::uint tr)
{
	if(tr <= 255)
		transparency = tr;
}

void PanelBase::Move(Point p)
{
	location.x += p.x;
	location.y += p.y;
}

void PanelBase::Move(nReal x, nReal y)
{
	location.x += x;
	location.y += y;
}
	

}
