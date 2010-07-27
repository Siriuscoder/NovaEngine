/***************************************************************************
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

#include "nova_error.h"

namespace nova
{

class NOVA_EXPORT CStringUtils
{
public:

	static nova::stl<nstring>::vector Split(const nstring & str);

	static nova::stl<nstring>::vector Split(const nstring & str, char delim);

	static int StringToInt(const nstring & str);

	static int StringToInt(const char * str);

	static nstring IntToString(int num);

	static nstring IntTo16xString(int num);

	static float StringToFloat(const nstring & str);

	static double StringToDouble(const nstring & str);

	static float StringToFloat(const char * str);

	static double StringToDouble(const char * str);

	static nstring FloatToString(float real);

	static nstring DoubleToString(double real);

	static nstring ToUpperCase(const nstring &str);

	static nstring ToLowerCase(const nstring &str);

};

}