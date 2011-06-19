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
#include "nova_string_utils.h"

using namespace nova;

nstring gPackage;
nstring gSourceFile;

class ResourcePackerTool : public CExampleApplication
{
protected:

	nova::CFilesPackage mPack;
	nova::nstring mPackageName;

public:

	ResourcePackerTool()
	{
		mEngine = new nova::CNovaEngine();
	}

	~ResourcePackerTool()
	{
		delete mEngine;
		mEngine = NULL;
	}


	void ListDir(const nova::nstring sdir, int count)
	{
#ifdef NOVA_PLATFORM_WINDOWS
		nova::nstring str, strDir;
		struct _finddata_t c_file;
		long hFile;

		// Find first file in current directory
		str = sdir + "\\*";
		if( (hFile = _findfirst( str.c_str(), &c_file )) == -1L )
			return;

		do
		{
			//skip if find . and ..
			if ((strcmp(c_file.name, ".") == 0 ||  strcmp(c_file.name, "..") == 0)) 
				continue;

			if (c_file.attrib & _A_SUBDIR)
			{
				strDir = sdir + "\\" + c_file.name;
				ListDir(strDir.c_str(), count+1);
				continue;
			}

			OnFile(c_file.name, sdir);
		}
		while(_findnext( hFile, &c_file ) == 0);

		_findclose( hFile );
		return;
#endif
	}

	void OnFile(const nova::nstring &fileName, const nova::nstring &path)
	{
		nova::CFileStream packingfile;
		nova::CMemoryBuffer buf;
		nova::nstring fullName = path + DELIM + fileName;

		cout << "Packing file " << path << DELIM << fileName << "..."; 
		cout.flush();

		packingfile.Open(fullName);
		buf.AllocBuffer(packingfile.Size());
		packingfile.Read(buf);	

		stl<nstring>::vector vfex = CStringUtils::Split(fileName, '.');
		mPack.PutFile(buf, fullName, vfex[vfex.size()-1], mPackageName);

		cout << "done." << std::endl;
		buf.FreeBuffer();
		packingfile.Close();
	}

	void StartPack(const nstring &package, const nstring &folder)
	{
		mPack.OpenPackage(package, true);

		stl<nstring>::vector vfpk = CStringUtils::Split(package, DELIM);
		mPackageName = vfpk[vfpk.size()-1];

		ListDir(folder, 0);

		mPack.ClosePackage();
		cout << endl << endl << "Packing complete.." << endl;
	}

	void StartUnpack(const nstring &package, const nstring &folder)
	{
		mPack.OpenPackage(package, false);
		stl<nstring>::vector vf = mPack.GetFileList();
		cout << endl << "File list unpacked successfully, begin saving files -> " << endl;
		nova::CFileStream stream;
		nova::nstring fileName;

		for(nova::nUInt32 i = 0; i < vf.size(); i++)
		{
			fileName = mPack.GetFileName(vf[i]);
			cout << "Unpacking file: " << fileName << ", internal path " << vf[i] << endl;
			nova::CMemoryBuffer buf = mPack.GetFile(vf[i]);

			stream.Open(folder + DELIM + fileName, true, false);
			stream.Write(buf);

			buf.FreeBuffer();
			stream.Close();
		}

		mPack.ClosePackage();

		cout << endl << "Done.." << endl;
	}

	void Launch(const nstring &package, const nstring &folder, bool extract)
	{
		if(extract)
			StartUnpack(package, folder);
		else
			StartPack(package, folder);
	}

	void PrintHelp(void)
	{
		cout << "Usage: [-pack | -unpack] PackageName.pack | SourceFile1, SourceFile2..." << endl;
	}
};


ENTRY_POINT	
{
	ResourcePackerTool packer;

	try
	{
/////// Parsing unput args //////////////
		CParser rParser;
		stl<nstring>::vector args;
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

		if(!args.size())
		{
			packer.PrintHelp();
		}
		else if(args[0] == "-pack")
		{
			if(args.size() == 3)
			{
				cout << "Trying to create package " << args[1].c_str() << "..." << endl;
				packer.Launch(args[1], args[2], false);
			}
			else
				packer.PrintHelp();
		}
		else if(args[0] == "-unpack")
		{
			if(args.size() == 3)
			{
				cout << "Trying to unpack " << args[1].c_str() << "..." << endl;
				packer.Launch(args[1], args[2], true);
			}
			else
				packer.PrintHelp();
		}
		else
			packer.PrintHelp();
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

	cin.get();

	return 0;
}
