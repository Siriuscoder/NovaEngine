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

	void StartPack(const nstring &package, const stl<nstring>::vector & files)
	{
		mPack.OpenPackage(package, true);


		for(nova::uint i = 0; i < files.size(); i++)
		{
			nova::CFileStream packingfile;
			packingfile.Open(files[i], false, false);

			nova::CMemoryBuffer buf;
			buf.AllocBuffer(packingfile.Size());
			packingfile.Read(buf);

			mPack.PutFile(buf, files[i], nstring("ext"), nstring("res_pack"));

			cout << "=";
			buf.FreeBuffer();
			packingfile.Close();
		}

		mPack.ClosePackage();

		cout << endl << "Packing complete" << endl;
	}

	void StartUnpack(const nstring &package)
	{
		mPack.OpenPackage(package, false);
		stl<nstring>::vector vf = mPack.GetFileList();
		cout << endl << "Unpacking complete, file list: " << endl;

		for(nova::uint i = 0; i < vf.size(); i++)
			cout << vf[i] << endl;

		nova::CMemoryBuffer buf = mPack.GetFile(nstring("image.jpg"));

		nova::CFileStream stream;
		stream.Open("test_image.jpg", true, false);
		stream.Write(buf);

		buf.FreeBuffer();
		stream.Close();
		mPack.ClosePackage();
	}

	void Launch(const nstring &package, const stl<nstring>::vector & files, bool extract)
	{
		if(extract)
			StartUnpack(package);
		else
			StartPack(package, files);
	}
};

#if defined(__WIN32__)
void RestreamToAllocatedConsole(void)
{
	AllocConsole();

	FILE *hf = _fdopen(_open_osfhandle(
                (long)GetStdHandle(STD_OUTPUT_HANDLE), 2), "w");
	FILE *hfr = _fdopen(_open_osfhandle(
                (long)GetStdHandle(STD_INPUT_HANDLE), 2), "r");

	*stdout = *stderr = *hf;
	*stdin = *hfr;
}

void CloseConsole()
{
	FreeConsole();
}
#endif


ENTRY_POINT	
{
#if defined(__WIN32__)
	RestreamToAllocatedConsole();
#endif

	try
	{
		ResourcePackerTool packer;
/////// Parsing unput args //////////////
		CParser rParser;
		stl<nstring>::vector args;
		stl<nstring>::vector files;
		cout << "Hello, this is Resource Packing tool for Nova engine.." << endl;

		{
#if defined(__WIN32__)
			nstring input;
			input.append(cmdParam);

			rParser.ParseStringRecurse(input, args);
#else
			for(int i = 1; i < argc, i++)
				args.push_back(nstring(arg[i]));
#endif
		}

		if(args[0] == nstring("-pack"))
		{
			if(args.size() >= 3)
			{
				cout << "Trying to create pack " << args[1].c_str() << "..." << endl;
				for(nova::uint i = 2; i < args.size(); i++)
					files.push_back(args[i]);
					
				packer.Launch(args[1], files, false);
			}
			else
				throw NOVA_EXP("ResourcePacker: bad arguments", BAD_OPERATION);
		}
		else if(args[0] == nstring("-unpack"))
		{
			if(args.size() == 2)
			{
				cout << "Trying to unpack " << args[1].c_str() << "..." << endl;
				packer.Launch(args[1], files, true);
			}
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
		//exp.FixError();
	}
	catch(exception & exp)
	{
		cerr << exp.what() << endl;
	}


#if defined(__WIN32__)
	cin.get();
	CloseConsole();
#endif

	return 0;
}
