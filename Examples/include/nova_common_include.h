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

#if defined(__WIN32__)

#define ENTRY_POINT	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hIns, LPSTR cmdParam, int cmdShow)

#define FONT_PATH ".\\Courier10b95.ttf"
#define CONF_FILE ".\\test.conf"
#define TEXTURE_PATH ".\\back.jpg"

#else

#ifdef HAVE_CONFIG_H
#	include "config/config.h"
#endif

#define ENTRY_POINT	int main(int argc, char * arg[])

#define FONT_PATH "/usr/local/share/CHICAGO.TTF"
#define CONF_FILE "./test.conf"

#endif

#include "nova_engine.h"

//using namespace nova;
//using namespace nova::utils;