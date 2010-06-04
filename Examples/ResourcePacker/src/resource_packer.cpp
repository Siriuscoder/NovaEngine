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

nstring gPackage;
nstring gSourceFile;

class ResourcePackerTool : public CExampleApplication
{
protected:

	nova::CFilesPackage mPack;


public:

	ResourcePackerTool()
	{
	}

	~ResourcePackerTool()
	{
	}

	void StartPack(const nstring &package, const nstring &filelist)
	{

	}

	void StartUnpack(const nstring &package)
	{

	}

	void Launch(const nstring &package, const nstring &filelist, bool extract)
	{
		if(extract)
			StartUnpack(package);
		else
			StartPack(package, filelist);
	}
};


ENTRY_POINT	
{
	try
	{
		ResourcePackerTool packer;
/////// Parsing unput args //////////////
		CParser rParser;
		stl<nstring>::vector args;

		{
#if defined(__WIN32__)
			nstring input;
			input.append(cmdParam);

			rParser.ParseStringRecurse(input, args);
#else
			for(int i = 0; i < argc, i++)
				args.push_back(nstring(arg[i]));
#endif
		}

		if(args[1] == nstring("-pack"))
		{
			if(args.size() > 3)
				packer.Launch(args[2], args[3], false);
			else
				throw NOVA_EXP("ResourcePacker: bad arguments", BAD_OPERATION);
		}
		else if(args[1] == nstring("-unpack"))
		{
			if(args.size() > 2)
				packer.Launch(args[2], nstring(), true);
			else
				throw NOVA_EXP("ResourcePacker: bad arguments", BAD_OPERATION);
		}
		else
			throw NOVA_EXP("ResourcePacker: bad arguments", BAD_OPERATION);
/////////////////////////////////////////
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

	return 0;
}
