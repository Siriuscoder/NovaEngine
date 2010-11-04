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

	class SceneListener : public nova::CSceneManagerListener
	{
		void SceneRenderBeginListener(nova::CSceneManager * object)
		{
			glBegin(GL_TRIANGLES);				// Начало рисования пирамиды
			glColor3f(1.0f,0.0f,0.0f);			// Красный
			glVertex3f( 0.0f, 1.0f, 0.0f);			// Верх треугольника (Передняя)
			glColor3f(0.0f,1.0f,0.0f);			// Зеленный
			glVertex3f(-1.0f,-1.0f, 1.0f);			// Левая точка
			glColor3f(0.0f,0.0f,1.0f);			// Синий
			glVertex3f( 1.0f,-1.0f, 1.0f);			// Правая точка
			glColor3f(1.0f,0.0f,0.0f);			// Красная
			glVertex3f( 0.0f, 1.0f, 0.0f);			// Верх треугольника (Правая)
			glColor3f(0.0f,0.0f,1.0f);			// Синия
			glVertex3f( 1.0f,-1.0f, 1.0f);			// Лево треугольника (Правая)
			glColor3f(0.0f,1.0f,0.0f);			// Зеленная
			glVertex3f( 1.0f,-1.0f, -1.0f);			// Право треугольника (Правая)
			glColor3f(1.0f,0.0f,0.0f);			// Красный
			glVertex3f( 0.0f, 1.0f, 0.0f);			// Низ треугольника (Сзади)
			glColor3f(0.0f,1.0f,0.0f);			// Зеленный
			glVertex3f( 1.0f,-1.0f, -1.0f);			// Лево треугольника (Сзади)
			glColor3f(0.0f,0.0f,1.0f);			// Синий
			glVertex3f(-1.0f,-1.0f, -1.0f);			// Право треугольника (Сзади)
			glColor3f(1.0f,0.0f,0.0f);			// Красный
			glVertex3f( 0.0f, 1.0f, 0.0f);			// Верх треугольника (Лево)
			glColor3f(0.0f,0.0f,1.0f);			// Синий
			glVertex3f(-1.0f,-1.0f,-1.0f);			// Лево треугольника (Лево)
			glColor3f(0.0f,1.0f,0.0f);			// Зеленный
			glVertex3f(-1.0f,-1.0f, 1.0f);			// Право треугольника (Лево)
			glEnd();
		}
	} mRenderListener;

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

		nova::COctreeSceneManager *nSceneMan = new nova::COctreeSceneManager("TestScene", "Manual");
		nSceneMan->AddListener(&mRenderListener);
		nova::CScene::GetSingelton().AddScrap(nSceneMan);
	}

	virtual void InitViewport(void)
	{
		nova::CRenderWindowPtr win = mEngine->GetRenderSystem()->GetRootWindow();
		nova::CViewPortPtr viewport1 = win->AddViewport(0, 0, 800, 600, 2);
		//nova::CViewPortPtr viewport2 = win->AddViewport(0, 0, 256, 192, 1);

		nova::CCameraPtr camera1 = viewport1->CreatePerspectiveCamera("cam1", 45.0f, 100.0f, 0.1f);
		viewport1->SetActiveCamera("cam1");

		camera1->SetLocation(0, 0, -6.0f);
		camera1->SetPolygonMode(nova::CCamera::PM_WIREFRAME, nova::CCamera::FS_FRONT);
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
