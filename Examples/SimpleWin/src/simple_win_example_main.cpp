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
#include "simple_win_example_events.h"
#include "nova_example_application.h"

class SimpleWinExample : public CExampleApplication
{
protected:

	CMyEventHandler * mEventController;

public:

	SimpleWinExample()
	{
		mEventController = new CMyEventHandler();
	}

	~SimpleWinExample()
	{
		delete mEventController;
	}

	virtual void InitExample(void)
	{
		mEngine->AttachEventHandlerToMainWindow(mEventController);
	}

	virtual void LoadResources()
	{
	/*
		nova::CTreeSceneManager *moution_manager = 
			new nova::CTreeSceneManager(nova::nstring("SceneOne"), nova::nstring("movable_factory"));

		nova::CSceneScrap *mainscrap = new nova::CSceneScrap(moution_manager, true);
		nova::CScene::GetSingelton().AddScrap(mainscrap);
	*/
	}

	virtual void InitViewport(void)
	{
		nova::CRenderWindowPtr win = mEngine->GetRenderSystem()->GetRootWindow();
		nova::CViewPortPtr viewport1 = win->AddViewport(0, 0, 800, 600, 2);
		nova::CViewPortPtr viewport2 = win->AddViewport(0, 0, 256, 192, 1);

        nova::nstring cam1("cam1"), cam2("cam2");
		nova::CCameraPtr camera1 = viewport1->CreatePerspectiveCamera(cam1, 45.0f, 100.0f, 0.1f);
		nova::CCameraPtr camera2 = viewport2->CreatePerspectiveCamera(cam2, 45.0f, 100.0f, 0.1f);
		viewport1->SetActiveCamera(cam1);
		viewport2->SetActiveCamera(cam2);

		//camera->OffsetPosition(0, 0, -6.0f);
		camera2->SetLocation(0.0, 0.0, -5.0f);
		camera2->LookAt(3, 0, 0);

		camera1->SetLocation(0, 0, -6.0f);
		camera1->SetPolygonMode(nova::CCamera::PM_WIREFRAME, nova::CCamera::FS_FRONT_AND_BACK);
		//camera1->SetLocalLocation(0, 10.0, -6.0f);

		//camera->Roll(0.1f);
		//camera1->LookAt(0, 0, 0);
	}
};


ENTRY_POINT
{
	try
	{
		SimpleWinExample example;
		example.Start(nova::ST_INIT_AUTOMATICALY);
	}
	catch(nova::NovaExp & exp)
	{
		std::cerr << exp.Message() << endl;
		exp.FixError();
	}
	catch(exception & exp)
	{
		cerr << exp.what() << endl;
	}
	cout << "ShutDoun Simple Win Example... " << endl;

	return 0;
}
