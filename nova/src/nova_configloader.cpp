/***************************************************************************
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
#include "nova_stable_precompiled_headers.h"

#include "nova_configloader.h"

//#include "iconv/iconv.h"
//#include "iconv/localcharset.h"

namespace nova {

CConfig::CConfig(uint flag) : CBase((char *)"CConfig")
{
	filename.clear();
	FreeTree();
	parseflag = flag;
}

CConfig::CConfig(nstring & _filename, uint flag) : CBase((char *)"CConfig")
{
	filename.clear();
	FreeTree();
	parseflag = flag;

	filename.append(_filename);
}

CConfig::~CConfig()
{
	filename.clear();
}

int CConfig::MakeTree()
{
	int count = 0;

	if(filename.size() <= 0)
		throw NovaExp("CConfig::MakeTree - filename is empty.", BAD_OPERATION);

	ifstream in;
	in.open(filename.c_str(), ifstream::in);
	if(in)
	{
		/*char strinutf_8[LINE_LENGTH];
		char strinlocale[LINE_LENGTH];
		size_t insize, outsize = LINE_LENGTH;

		cchar *inb = strinutf_8;
		char *outb = strinlocale;
		*/
		{
			char linebuf[256];
			in.getline(linebuf, 256);
			nstring parstr(linebuf);
			conftree.CreateRoot(parstr);
		}

		while(!in.eof())
		{
			/*memset(strinutf_8, 0, LINE_LENGTH);
			memset(strinlocale, 0, LINE_LENGTH);
			in >> strinutf_8;

			insize = strlen(strinutf_8);
			iconv_t cd = iconv_open(locale_charset(), "utf-8");
			iconv(cd, &inb, &insize, &outb, &outsize);
			iconv_close(cd);
			*/

			char linebuf[512];
			in.getline(linebuf, 512);
			nstring line(linebuf);

			if(line.size() > 0)
			{
				if(parseflag == CConfig::RECURSIVE)
					conftree.GetRootElement()->AddCurentNone(CParser::ParseStringRecurseTree(line));
				else if(parseflag == CConfig::COMMON)
					conftree.GetRootElement()->AddCurentNone(CParser::ParseStringTree(line));
			}	
		}

		in.close();
	}
	else
		throw NovaExp("CConfig::MakeTree() - Can not open config file!", BAD_OPERATION);

	return count;
}

void CConfig::FreeTree()
{
	conftree.FreeAll();
}

CTreeNode<nstring> * CConfig::GetNodePointer(nstring & node)
{
	for(int i = 0; i < conftree.GetRootElement()->GetChildrenLen(); ++i)
		if(node.compare((*(conftree.GetRootElement()))[i]->GetData()) == 0)
			return conftree.GetRootElement()->GetNode(i);

	return NULL;
}

CParser::values CConfig::GetNodeValues(nstring & node)
{
	return NodeToValues(GetNodePointer(node));
}

CTree<nstring> * CConfig::GetTree()
{
	return &conftree;
}

int CConfig::SaveTree(CTree<nstring> & tree, nstring & _filename)
{
	ofstream out;
	int count = 0;

	out.open(_filename.c_str());
	if(out)
	{
		out << tree.GetRootElement()->GetData() << endl;
		for(int i = 0; i < tree.GetRootElement()->GetChildrenLen(); ++i)
		{
			CTreeNode<nstring> * node = tree.GetRootElement()->GetNode(i);
			nstring line;

			line.clear();
			line.append(node->GetData());

			for(int j = 0; j < node->GetChildrenLen(); ++j)
				line.append(" " + node->GetNode(j)->GetData());
			out << line << endl;

			count++;
		}

		out.close();
	}
	else
		throw NovaExp("CConfig::SaveTree - Can not create config file!", BAD_OPERATION);
	return count;
}
			

}
