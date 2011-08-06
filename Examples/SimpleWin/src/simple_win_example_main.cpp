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

	CMyEventHandler mEventController;

	class SceneListener : public nova::CSceneManagerListener
	{
		void SceneRenderBeginListener(nova::CSceneManager * object)
		{

		}

	} mRenderListener;

public:

	SimpleWinExample()
	{

	}

	~SimpleWinExample()
	{

	}

	virtual void InitExample(void)
	{
		mEngine->AttachEventHandlerToMainWindow(&mEventController);
	}

	virtual void LoadResources()
	{
		nova::CBasicSceneManager *sceneManager = new nova::CBasicSceneManager("BasicSceneManager", "TestSceneGroup");

		sceneManager->AddListener(&mRenderListener);
		nova::CScene::GetSingelton().AddScrap(sceneManager);

		//nova::CResourcePtr meshResource = nova::CResourceManager::LoadResourceFromXml("test/Minigun.xml");
		//sceneManager->SetRootElement(sceneManager->CreateRenderableNodeFromResource(meshResource->GetResName()));
		//sceneManager->PrepareScene();

		nova::CScene::GetSingelton().LoadSceneForce("sniper/scene.xml");
		//nova::CImagePtr image = nova::CImageManager::GetResourceFromHash("iMinigunTX");
		//image->BackHeigth();
	}

	virtual void InitViewport(void)
	{
		nova::CRenderWindowPtr win = mEngine->GetRenderSystem()->GetRootWindow();
		nova::CViewPortPtr viewport1 = win->AddViewport(0, 0, win->GetWidth(), win->GetHeight(), 1);
		//nova::CViewPortPtr viewport2 = win->AddViewport(0, 100, 640, 480, 2);

		nova::CCameraPtr camera1 = viewport1->CreatePerspectiveCamera("cam1", 45.0f, 200.0f, 0.1f);
		//nova::CCameraPtr camera2 = viewport2->CreatePerspectiveCamera("cam2", 45.0f, 200.0f, 0.1f);
		viewport1->SetActiveCamera("cam1");
		//viewport2->SetActiveCamera("cam2");

		camera1->SetLocation(0, 0, -100.0f);
		//camera1->SetPolygonMode(nova::CCamera::PM_WIREFRAME, nova::CCamera::FS_FRONT_AND_BACK);

		//camera2->SetLocation(0, 0, -100.0f);
		//camera2->SetPolygonMode(nova::CCamera::PM_WIREFRAME, nova::CCamera::FS_FRONT_AND_BACK);
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
