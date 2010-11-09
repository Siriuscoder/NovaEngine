/***************************************************************************
 *   Copyright (C) 2010 by Sirius										   *
 *	 Vdov Nikita Sergeevich	(c)											   *
 *	 siriusnick@gmail.com												   *
 *																		   *
 *	 This source file is part of NovaEngine	tool-box					   *
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
#include "nova_example_application.h"
#include "nova_string_utils.h"

using namespace nova;

nstring gPackage;
nstring gSourceFile;

class SceneBuilderTool : public CExampleApplication
{
protected:

	nova::CCPUTimer mTimer;

	nova::nstring mDest;
	nova::nstring mSource;
	nova::CASELoader mLoader;

public:

	SceneBuilderTool()
	{
		mEngine = new nova::CNovaEngine();
	}

	~SceneBuilderTool()
	{
		delete mEngine;
		mEngine = NULL;
	}

	void Launch(const nova::nstring &file, const nova::nstring &dest_path, bool cpy_image, bool pack)
	{
		nova::CFileStream ase_file;
		ase_file.Open(file, false, false);

		mTimer.Reset();
		mLoader.LoadImmediately(&ase_file);
		nova::nReal msec = static_cast<nova::nReal>(mTimer.GetKernelMicroseconds() * 0.001);
		ase_file.Close();

		std::cout << std::endl << "Ase File parsed with " + nova::CStringUtils::FloatToString(msec) << " ms" << std::endl << std::endl;

		PrintInfo();
	}

	void PrintInfo(void)
	{
		nova::stl<nstring>::vector meshes = mLoader.GetMeshList();
		std::cout << "Objects list: " << std::endl;
		for(nova::nUInt32 i = 0; i < meshes.size(); i++)
		{
			std::cout << meshes[i] << std::endl;
			std::cout << "Mesh info: " << std::endl;
			nova::CSceneContentLoaderBase::TMeshContainer geom = mLoader.GetMesh(meshes[i]);

			std::cout << "Vertex count: " << nova::CStringUtils::IntToString(geom.nVertexList.size()) << std::endl;
			std::cout << "Face count: " << nova::CStringUtils::IntToString(geom.nIndexList.size()) << std::endl;
			std::cout << "Mat ID: " << nova::CStringUtils::IntToString(geom.MatID) << std::endl << std::endl;
		}
	
		nova::stl<nstring>::vector materials = mLoader.GetMaterialList();
		std::cout << std::endl << "Materials list: " << std::endl;
		for(nova::nUInt32 i = 0; i < materials.size(); i++)
			std::cout << materials[i] << std::endl;

		nova::stl<nstring>::vector maps = mLoader.GetTextureList();
		std::cout << std::endl << "Texture list: " << std::endl;
		for(nova::nUInt32 i = 0; i < maps.size(); i++)
			std::cout << maps[i] << std::endl;
	}

	void PrintHelp(void)
	{
		cout << "Usage: [-i, -pack] Source.ase DestinationFolder/" << endl;
	}
};


ENTRY_POINT	
{
	SceneBuilderTool SceneBuilder;

	try
	{
/////// Parsing unput args //////////////
		CParser rParser;
		stl<nstring>::vector args;
		bool try_cpy_image = false, pack = false;
		cout << "Hello, this is Scene Builder tool for Nova engine.." << endl;
		cout << "Converting ASE file to html Nova engine scene files, and after packing using gz(zlib)" << endl;

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

		if(args.size() < 2)
		{
			SceneBuilder.PrintHelp();
			return -1;
		}
		if(args[0] == "-i")
		{
			try_cpy_image = true;
			args.erase(args.begin());
		}
		if(args[0] == "-pack")
		{
			pack = true;
			args.erase(args.begin());
		}
		if(args.size() < 2)
		{
			SceneBuilder.PrintHelp();
			return -1;
		}

		SceneBuilder.Launch(args[0], args[1], try_cpy_image, pack);
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
