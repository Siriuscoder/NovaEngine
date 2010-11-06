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

	void Load(void)
	{
		nova::CASELoader loader;
		nova::CFileStream ase_file;

		ase_file.Open("test_plane.ASE", false, false);

		mTimer.Reset();
		loader.LoadImmediately(&ase_file);
		nova::nReal msec = static_cast<nova::nReal>(mTimer.GetKernelMicroseconds() * 0.001);
		ase_file.Close();

		std::cout << std::endl << "Ase File parsed with " + nova::CStringUtils::FloatToString(msec) << " ms" << std::endl << std::endl;

		nova::stl<nstring>::vector meshes = loader.GetMeshList();
		std::cout << "Objects list: " << std::endl;
		for(nova::nUInt32 i = 0; i < meshes.size(); i++)
			std::cout << meshes[i] << std::endl;

		nova::stl<nstring>::vector materials = loader.GetMaterialList();
		std::cout << std::endl << "Materials list: " << std::endl;
		for(nova::nUInt32 i = 0; i < materials.size(); i++)
			std::cout << materials[i] << std::endl;

		nova::stl<nstring>::vector maps = loader.GetTextureList();
		std::cout << std::endl << "Texture list: " << std::endl;
		for(nova::nUInt32 i = 0; i < maps.size(); i++)
			std::cout << maps[i] << std::endl;
	}
};


ENTRY_POINT	
{
	try
	{
		SceneBuilderTool SceneBuilder;

		SceneBuilder.Load();
		cin.get();
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

	return 0;
}
