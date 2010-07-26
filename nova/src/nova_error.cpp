/***************************************************************************
 *   Copyright (C) 2007 by SIRIUS										   *
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

#include "nova_error.h"
#include "nova_string_utils.h"
#include "nova_math.h"

namespace nova {

int er_code = 0;

/*
uint inline Stepin(uint i, int c)
{
	uint p = i;
	if(c == 0)
		return 1;

	while(c > 0)
	{
		c--;
		i *= p;
	}
	return i;
}
*/

void prError(nova::nstringstream & mes)
{
	CLog & log = CLog::GetInstance();

	log.PrintMessage(mes.str().c_str(), CLog::LG_WITH_TIME_ERROR);
//-------- РўР°Р±Р»РёС†Р° СЂР°СЃРїСЂРµРґРµР»РµРЅРёСЏ РїР°РјСЏС‚Рё -----------
#ifdef USE_WINNI_FAST_ALLOCATOR
	winnie_allocator::Statistic stat;
	winnie_allocator::GetAllocInfo(stat);

	nova::nstringstream table;

	table << "------------------ Memory Table ----------------------\n"
		<< "address\tallocated blocks\tfree_blocks\tblocks_allocations\n";


	for(int i = 0; i < winnie_allocator::max_lists_num; ++i)
	{
		int level = (int)CMathf::Pow(0x02, i);

		table << "0x" << CStringUtils::IntTo16xString(level) << "\t         ";

		table << CStringUtils::IntToString(stat.num_allocated_blocks[i]) << "\t         ";

		table << CStringUtils::IntToString(stat.num_free_blocks[i]) << "\t         ";

		table << CStringUtils::IntToString(stat.num_block_allocations[i]) << "\n";
	}

	log.PrintStream(table, CLog::LG_NO_TIME);
#endif
}

int STDCALL__ error(int code, nstring & mes, CBase *pp)
{
	char class_name[CLASS_NAME_LEN]= "Unknown class";
	nova::nstringstream err;

	switch(code)
	{
	case EXIT_OK:
		{
			return code;
		}
		break;
	case MEM_ERROR:
		{
			if(pp) pp->GetClassName(class_name);
			err << "Detecting error of memory allocation:: " <<
				class_name << "::" << mes << endl;
			er_code = code;

		}
		break;
	case BAD_OPERATION:
		{
			if(pp) pp->GetClassName(class_name);
			err << "Detecting inadmissible procedure:: " <<
				class_name << "::" << mes << endl;
			er_code = code;
		}
		break;
	case BAD_OPERATION_WITH_STREAM:
		{
			if(pp) pp->GetClassName(class_name);
			err << "Detecting inadmissible procedure with I/O stream:: " <<
				class_name << "::" << mes << endl;
			er_code = code;
		}
		break;
	case FATAL_OPERATION:
		{
			if(pp) pp->GetClassName(class_name);
			err << "Detecting inadmissible fatal operation:: " <<
				class_name << "::" << mes << endl;
			prError(err);
			er_code = code;
			abort();
		}
		break;
	default:
		{
			if(pp) pp->GetClassName(class_name);
			err << "Error: bad result value!" <<
				class_name << "::" << mes << endl;
			er_code = code;
		}
	}
	cerr << endl << err << endl;
	prError(err);
	return er_code;
}

int last_error()
{
	return er_code;
}

void drop_last_error()
{
	er_code = NULL;
}

NovaExp::NovaExp() : CBase("NovaExp")
{
	e_mes.clear();
	code = 0;
}

NovaExp::NovaExp(int _code) : CBase("NovaExp")
{
	e_mes.clear();
	code = _code;
}

NovaExp::NovaExp(nova::cchar *mes, int _code) : CBase("NovaExp")
{
	e_mes.clear();
	e_mes.append(mes);
	code = _code;
}

NovaExp::NovaExp(nova::cchar file[], int line, cchar * mes, int _code) : CBase("NovaExp")
{
	e_mes.clear();
	e_mes.append(mes);
	e_mes.append(" file: ");
	e_mes.append(file);
	e_mes.append(" line: ");
	e_mes.append(CStringUtils::IntToString(line));
	code = _code;
}

int NovaExp::GetCode()
{
	return code;
}

nstring NovaExp::Message()
{
	return e_mes;
}

cchar * NovaExp::what() const
{
	return e_mes.c_str();
}




}
