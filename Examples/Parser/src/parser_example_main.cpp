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
#include "nova_example_application.h"

using namespace nova;

class ParserExample : public CExampleApplication
{
protected:

public:

	ParserExample()
	{
	}

	~ParserExample()
	{
	}

	void MakeTree(nova::uint nodes, nova::CTree<nova::nstring> & mytree)
	{
		nstring datastr("Config_file_01"); 
		mytree.CreateRoot(datastr);

		CMathf::RandSeed((nova::uint)time(NULL));
		for(nova::uint i = 1; i < nodes; ++i)
		{
			std::ostringstream arg1;
			arg1 << "PARAM" << i << "_TEST";

			datastr.clear();
			datastr.append(arg1.str().c_str());
			int num = mytree.GetRootElement()->AddNone(datastr);
			
			nova::uint col = CMathf::Rand() % 5;
			nova::CTreeNode<nstring> * node = mytree.GetRootElement()->GetNode(num);
			for(nova::uint j = 0; j < col; ++j)
			{
				std::ostringstream arg2;
				arg2 << "  " << CMathf::Rand() % 200;

				datastr.clear();
				datastr.append(arg2.str().c_str());
				node->AddNone(datastr);
			}
		}
	}

	nova::CTreeNode<nstring> * MakeTimeWork(clock_t time1)
	{
		double res = 1000*((double)time1)/ CLOCKS_PER_SEC;
		std::ostringstream arg1;

		arg1 << "Table parset in: " << res << " ms";
		nstring str(arg1.str().c_str());
		return new nova::CTreeNode<nstring>(str);
	}

	void Start()
	{
		nova::CTree<nova::nstring> outtree;
		nova::CTree<nova::nstring> * intree;

		nstring filename(CONF_FILE);
		nova::CConfig config(filename, nova::CConfig::RECURSIVE);

		MakeTree(1500, outtree);
		config.SaveTree(outtree, filename);

		clock_t start = clock();
		config.MakeTree();
		clock_t timework = clock() - start;

		intree = config.GetTree();
		intree->GetRootElement()->AddCurentNone(MakeTimeWork(timework));

		nstring newfile = filename + "new";
		config.SaveTree(*intree, newfile);
	}
};


ENTRY_POINT	
{
	try
	{
		ParserExample example;
		example.Start();
	}
	catch(NovaExp & exp)
	{
		cerr << exp.Message() << endl;
		exp.FixError();
	}
	catch(exception & exp)
	{
		cerr << exp.what() << endl;
	}
	cout << "ShutDoun Parser Example... " << endl;

	return 0;
}
