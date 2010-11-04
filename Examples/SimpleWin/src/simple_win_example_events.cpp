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
	nova::CRenderWindowPtr window = nova::CRenderSystem::GetSingelton().GetRootWindow();
	nova::CViewPortPtr view = window->GetViewport(2);
	if(!view.IsNull())
	{
		nova::CCameraPtr camera = view->GetActiveCamera();
		if(!camera.IsNull())
		{
			if(code == nova::BT_LBUTTON)
			{
				camera->SetOrientation(nova::Quaternionf::IDENTITY);

				camera->Yaw(-((x - (view->GetActualWidth() >> 1)) * 0.008f));
				camera->Pitch(-((y - (view->GetActualHeight() >> 1)) * 0.008f));
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


