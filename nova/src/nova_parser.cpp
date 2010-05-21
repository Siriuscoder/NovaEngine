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

#include "nova_parser.h"

namespace nova {


int CParser::ParseStringRecurse(nstring input, CParser::values & result)
{
	if(input.empty())
		return 0;

	while(input[0] == ' ') {
		input.erase(input.begin());
		if(input.empty())
			return 0;
	}
	while(input[input.length() - 1] == ' ') {
		input.erase(input.end()-1);
		if(input.empty())
			return 0;
	}

	found = input.find(' ');
	parstr.clear();
	memset(value, 0, LINE_LENGTH);
	if(found != nstring::npos)
	{
		input.copy(value, found);
		input.erase(0, found+1);
		parstr.append(value);
		result.push_back(parstr);
	}
	else if(found == nstring::npos)
	{
		result.push_back(input);
		input.clear();
	}

	if(!input.empty())
		return ParseStringRecurse(input, result);

	return E_SUCCESS;
}

CTreeNode<nstring> * CParser::ParseStringRecurseTree(nstring & input)
{
	values result;

	ParseStringRecurse(input, result);
	return ValuesToNode(result);
}

CTreeNode<nstring> * CParser::ParseStringTree(nstring & input)
{
	values result = ParseString(input);
	return ValuesToNode(result);
}

CParser::values CParser::ParseString(nstring input)
{
	CParser::values result;
	while(true)
	{
		found = input.find("  ");
		if(found == nstring::npos)
			break;
		input.replace(found, 2, " ");
	}

	while(input[0] == ' ') 
		input.erase(input.begin());
	while(input[input.length() - 1] == ' ') 
		input.erase(input.end()-1);

	while(true)
	{
		found = input.find(' ');
		parstr.clear();
		memset(value, 0, LINE_LENGTH);
		if(found != nstring::npos)
		{
			input.copy(value, found);
			input.erase(0, found+1);
			parstr.append(value);
			result.push_back(parstr);
		}
		else if(found == nstring::npos)
		{
			result.push_back(input);
			break;
		}
	}


	return result;
}

CParser::values CParser::NodeToValues(CTreeNode<nstring> * node)
{
	values result;
	if(node)
	{
		result.push_back(node->GetData());

		for(int i = 0; i < node->GetChildrenLen(); ++i)
			result.push_back((*node)[i]->GetData());
	}

	return result;
}

CTreeNode<nstring> * CParser::ValuesToNode(CParser::values & input)
{
	if(input.size() == 0)
		return NULL;

	CTreeNode<nstring> * node = new CTreeNode<nstring>(input[0]);
	for(stl<nstring>::vector::iterator it = input.begin()+1; it != input.end(); it++)
		node->AddNone(*it);

	return node;
}

}