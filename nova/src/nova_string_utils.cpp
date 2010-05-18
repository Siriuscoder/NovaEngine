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
#include "nova_stable_precompiled_headers.h"

#include "nova_string_utils.h"

namespace nova
{

nova::stl<nstring>::vector CStringUtils::Split(const nstring & str)
{
	nstringstream stream(str);
	nova::stl<nstring>::vector temp;

	if(!stream.good())
		return temp;

	while(!stream.eof())
	{
		nstring current;
		stream >> current;

		temp.push_back(current);
	}

	return temp;
}

int CStringUtils::StringToInt(const nstring & str)
{
	return StringToInt(str.c_str());
}

nstring CStringUtils::IntToString(int num)
{
	char temp[30] = "\0";

	sprintf(temp, "%d", num);
	return nstring(temp);
	//return nstring(itoa(num, temp, 10));
}

nstring CStringUtils::IntTo16xString(int num)
{
	char temp[30] = "\0";

	sprintf(temp, "%X", num);
	return nstring(temp);
	//return nstring(itoa(num, temp, 16));
}

float CStringUtils::StringToFloat(const nstring & str)
{
	return StringToFloat(str.c_str());
}

double CStringUtils::StringToDouble(const nstring & str)
{
	return StringToDouble(str.c_str());
}

nstring CStringUtils::FloatToString(float real)
{
	nova::nstringstream stream;

	stream << real;
	return stream.str();
}

nstring CStringUtils::DoubleToString(double real)
{
	nova::nstringstream stream;

	stream << real;
	return stream.str();
}

int CStringUtils::StringToInt(const char * str)
{
	int res = 0;

	sscanf(str, "%d", &res);
	return res;
	//return atoi(str);
}

float CStringUtils::StringToFloat(const char * str)
{
	nova::nstringstream stream(str);

	float result = 0.0;
	stream >> result;

	return result;
}

double CStringUtils::StringToDouble(const char * str)
{
	nova::nstringstream stream(str);

	double result = 0.0;
	stream >> result;

	return result;
}

nstring CStringUtils::ToUpperCase(const nstring &str)
{
	nova::nstringstream upper;
	for(nova::uint i = 0; i < str.size(); ++i)
		upper << (char)toupper(str[i]);

	return upper.str();
}

nstring CStringUtils::ToLowerCase(const nstring &str)
{
	nova::nstringstream lower;
	for(nova::uint i = 0; i < str.size(); ++i)
		lower << (char)tolower(str[i]);

	return lower.str();
}

}