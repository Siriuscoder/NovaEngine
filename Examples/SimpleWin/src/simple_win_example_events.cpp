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

nova::nReal rtri = 0;
nova::nReal rquad = 0;

CMyEventHandler::CMyEventHandler() : CEventConveyor()
{
	fQUIT = reinterpret_cast<nova::pfQUIT>(&CMyEventHandler::OnQuit);
	fKEYUP = reinterpret_cast<nova::pfKEYUP>(&CMyEventHandler::OnKeyUp);
	fTIMER_TICK = reinterpret_cast<nova::pfTIMER_TICK>(&CMyEventHandler::OnTimer);
	fCHAR = reinterpret_cast<nova::pfCHAR>(&CMyEventHandler::OnChar);
	fMOUSEMOVE = reinterpret_cast<nova::pfMOUSEMOVE>(&CMyEventHandler::OnMouseMove);
}

CMyEventHandler::~CMyEventHandler() {}


void CMyEventHandler::OnKeyUp(int key, int mode)
{
	if(key == 27)
		nova::CNovaEngine::GetSingelton().StopRender();

/*	nova::CViewPortPtr view = nova::CRenderSystem::GetSingelton().GetRootWindow()->GetViewport(2);
	if(!view.IsNull())
	{
		nova::CCameraPtr camera = view->GetActiveCamera();
		if(!camera.IsNull())
		{
			if(key == 38)
				camera->Roll(0.1f);
			if(key == 39)
				camera->Pitch(0.1f);
			if(key == 40)
				camera->Yaw(0.1f);
			if(key == 37)
				camera->OffsetPosition(0.0f, 0.0, 1.0);
		}
	}
*/
}

void CMyEventHandler::OnChar(nova::nUInt16 s, int m)
{

}


void CMyEventHandler::OnMouseMove(int x, int y, int code)
{
	if(code == nova::BT_LBUTTON)
	{
		nova::CRenderWindowPtr window = nova::CRenderSystem::GetSingelton().GetRootWindow();
		nova::CViewPortPtr view1 = window->GetViewport(1);
		//nova::CViewPortPtr view2 = window->GetViewport(2);
		if(!view1.IsNull())
		{
			nova::CCameraPtr camera1 = view1->GetCameraByName("cam1");
			//nova::CCameraPtr camera2 = view2->GetCameraByName("cam2");
			if(!camera1.IsNull())
			{
				camera1->SetOrientation(nova::Quaternionf::IDENTITY);
				//camera2->SetOrientation(nova::Quaternionf::IDENTITY);

				camera1->Yaw(-((x - (view1->GetActualWidth() >> 1)) * 0.008f));
				camera1->Pitch(-((y - (view1->GetActualHeight() >> 1)) * 0.008f));
				//camera2->Yaw(-((x - (view1->GetActualWidth() >> 1)) * 0.008f));
				//camera2->Pitch(-((y - (view1->GetActualHeight() >> 1)) * 0.008f));
			}
		}
	}

	//SetCursorPos(window->GetWidth() >> 1, window->GetHeight() >> 1);
}

void CMyEventHandler::OnQuit()
{
	//CRenderManager::GetSingeltonPtr()->BreakRender();
}

void CMyEventHandler::OnTimer(int id)
{
	if(id == 100) rtri += 3.50f;
	if(id == 200) rquad += 3.50f;	
}


