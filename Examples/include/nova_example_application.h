п»ї/***************************************************************************
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
#pragma once

#include "nova_common_include.h"

class CExampleApplication : public nova::CBase
{
protected:
	nova::CNovaEngine * mEngine;

//CMyEventHandler * myevent;

public:
	CExampleApplication() : CBase("CExampleApplication") {}
	~CExampleApplication() {}

	void StartUpEngine(nova::StartInit init)
	{
		try
		{
			mEngine = new nova::CNovaEngine();

			mEngine->Init(init);
		}
		catch(nova::NovaExp & ex)
		{
			std::cerr << "CExampleApplication::StartUpEngine - Detected internal exception: " 
				<< ex.Message() << endl;
			ex.FixError(this);
		}
	}

	void ShutDown()
	{
		delete mEngine;
	}

//	virtual void AddTimers() {}
//	virtual void RemoveTimers() {}
	virtual void InitExample() {}

	virtual void LoadResources() {}

	virtual void InitViewport() {}

	virtual void Start(nova::StartInit init)
	{
		StartUpEngine(init);

		InitExample();

		mEngine->MakeRenderWindow();
		LoadResources();
		InitViewport();

		mEngine->StartRender();

		ShutDown();
	}
};


