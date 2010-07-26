п»ї/***************************************************************************
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

nova::real rtri = 0;
nova::real rquad = 0;

CMyEventHandler::CMyEventHandler() : CEventConveyor()
{
	fQUIT = reinterpret_cast<nova::pfQUIT>(&CMyEventHandler::OnQuit);
	fKEYUP = reinterpret_cast<nova::pfKEYUP>(&CMyEventHandler::OnKeyUp);
	fON_RENDER = reinterpret_cast<nova::pfON_RENDER>(&CMyEventHandler::OnRender);
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

void CMyEventHandler::OnChar(nova::word s, int m)
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

/***************************** Render Function ************************************/
void CMyEventHandler::OnRender()
{
	//engine->GetRenderManager()->GetConsoleManager()->Render();
/*
glMatrixMode(GL_PROJECTION);// Р’С‹Р±РѕСЂ РјР°С‚СЂРёС†С‹ РїСЂРѕРµРєС†РёРё
glLoadIdentity();		// РЎР±СЂРѕСЃ РјР°С‚СЂРёС†С‹ РїСЂРѕРµРєС†РёРё
gluPerspective(45.0f,(real)800/(real)600,0.1f,100.0f);
							// Р’С‹С‡РёСЃР»РёС‚СЊ СЃРѕРѕС‚РЅРѕС€РµРЅРёРµ РіРµРѕРјРµС‚СЂРёС‡РµСЃРєРёС… СЂР°Р·РјРµСЂРѕРІ РґР»СЏ РѕРєРЅР°
glMatrixMode(GL_MODELVIEW);// Р’С‹Р±РѕСЂ РјР°С‚СЂРёС†С‹ РїСЂРѕСЃРјРѕС‚СЂР° РјРѕРґРµР»Рё
glLoadIdentity();				// РЎР±СЂРѕСЃ РїСЂРѕСЃРјРѕС‚СЂР°
glTranslatef(-1.5f,0.0f,-6.0f);			// РЎРґРІРёРі РІР»РµРІРѕ Рё РІРіР»СѓР±СЊ СЌРєСЂР°РЅР°
glRotatef(rtri,0.0f,1.0f,0.0f);			// Р’СЂР°С‰РµРЅРёРµ РїРёСЂР°РјРёРґС‹ РїРѕ РѕСЃРё Y
glBegin(GL_TRIANGLES);				// РќР°С‡Р°Р»Рѕ СЂРёСЃРѕРІР°РЅРёСЏ РїРёСЂР°РјРёРґС‹
	glColor3f(1.0f,0.0f,0.0f);			// РљСЂР°СЃРЅС‹Р№
	glVertex3f( 0.0f, 1.0f, 0.0f);			// Р’РµСЂС… С‚СЂРµСѓРіРѕР»СЊРЅРёРєР° (РџРµСЂРµРґРЅСЏСЏ)
	glColor3f(0.0f,1.0f,0.0f);			// Р—РµР»РµРЅРЅС‹Р№
	glVertex3f(-1.0f,-1.0f, 1.0f);			// Р›РµРІР°СЏ С‚РѕС‡РєР°
	glColor3f(0.0f,0.0f,1.0f);			// РЎРёРЅРёР№
	glVertex3f( 1.0f,-1.0f, 1.0f);			// РџСЂР°РІР°СЏ С‚РѕС‡РєР°
	glColor3f(1.0f,0.0f,0.0f);			// РљСЂР°СЃРЅР°СЏ
	glVertex3f( 0.0f, 1.0f, 0.0f);			// Р’РµСЂС… С‚СЂРµСѓРіРѕР»СЊРЅРёРєР° (РџСЂР°РІР°СЏ)
	glColor3f(0.0f,0.0f,1.0f);			// РЎРёРЅРёСЏ
	glVertex3f( 1.0f,-1.0f, 1.0f);			// Р›РµРІРѕ С‚СЂРµСѓРіРѕР»СЊРЅРёРєР° (РџСЂР°РІР°СЏ)
	glColor3f(0.0f,1.0f,0.0f);			// Р—РµР»РµРЅРЅР°СЏ
	glVertex3f( 1.0f,-1.0f, -1.0f);			// РџСЂР°РІРѕ С‚СЂРµСѓРіРѕР»СЊРЅРёРєР° (РџСЂР°РІР°СЏ)
	glColor3f(1.0f,0.0f,0.0f);			// РљСЂР°СЃРЅС‹Р№
	glVertex3f( 0.0f, 1.0f, 0.0f);			// РќРёР· С‚СЂРµСѓРіРѕР»СЊРЅРёРєР° (РЎР·Р°РґРё)
	glColor3f(0.0f,1.0f,0.0f);			// Р—РµР»РµРЅРЅС‹Р№
	glVertex3f( 1.0f,-1.0f, -1.0f);			// Р›РµРІРѕ С‚СЂРµСѓРіРѕР»СЊРЅРёРєР° (РЎР·Р°РґРё)
	glColor3f(0.0f,0.0f,1.0f);			// РЎРёРЅРёР№
	glVertex3f(-1.0f,-1.0f, -1.0f);			// РџСЂР°РІРѕ С‚СЂРµСѓРіРѕР»СЊРЅРёРєР° (РЎР·Р°РґРё)
	glColor3f(1.0f,0.0f,0.0f);			// РљСЂР°СЃРЅС‹Р№
	glVertex3f( 0.0f, 1.0f, 0.0f);			// Р’РµСЂС… С‚СЂРµСѓРіРѕР»СЊРЅРёРєР° (Р›РµРІРѕ)
	glColor3f(0.0f,0.0f,1.0f);			// РЎРёРЅРёР№
	glVertex3f(-1.0f,-1.0f,-1.0f);			// Р›РµРІРѕ С‚СЂРµСѓРіРѕР»СЊРЅРёРєР° (Р›РµРІРѕ)
	glColor3f(0.0f,1.0f,0.0f);			// Р—РµР»РµРЅРЅС‹Р№
	glVertex3f(-1.0f,-1.0f, 1.0f);			// РџСЂР°РІРѕ С‚СЂРµСѓРіРѕР»СЊРЅРёРєР° (Р›РµРІРѕ)
glEnd();
glLoadIdentity();
glTranslatef(1.5f,0.0f,-7.0f);		// РЎРґРІРёРЅСѓС‚СЊ РІРїСЂР°РІРѕ Рё РІРіР»СѓР±СЊ СЌРєСЂР°РЅР°
glRotatef(rquad,1.0f,1.0f,1.0f);	// Р’СЂР°С‰РµРЅРёРµ РєСѓР±Р° РїРѕ X, Y & Z
glBegin(GL_QUADS);			// Р РёСЃСѓРµРј РєСѓР±
	glColor3f(0.0f,1.0f,0.0f);		// РЎРёРЅРёР№
	glVertex3f( 1.0f, 1.0f,-1.0f);		// РџСЂР°РІРѕ РІРµСЂС… РєРІР°РґСЂР°С‚Р° (Р’РµСЂС…)
	glVertex3f(-1.0f, 1.0f,-1.0f);		// Р›РµРІРѕ РІРµСЂС…
	glVertex3f(-1.0f, 1.0f, 1.0f);		// Р›РµРІРѕ РЅРёР·
	glVertex3f( 1.0f, 1.0f, 1.0f);		// РџСЂР°РІРѕ РЅРёР·
	glColor3f(1.0f,0.5f,0.0f);		// РћСЂР°РЅР¶РµРІС‹Р№
	glVertex3f( 1.0f,-1.0f, 1.0f);		// Р’РµСЂС… РїСЂР°РІРѕ РєРІР°РґСЂР°С‚Р° (РќРёР·)
	glVertex3f(-1.0f,-1.0f, 1.0f);		// Р’РµСЂС… Р»РµРІРѕ
	glVertex3f(-1.0f,-1.0f,-1.0f);		// РќРёР· Р»РµРІРѕ
	glVertex3f( 1.0f,-1.0f,-1.0f);		// РќРёР· РїСЂР°РІРѕ
	glColor3f(1.0f,0.0f,0.0f);		// РљСЂР°СЃРЅС‹Р№
	glVertex3f( 1.0f, 1.0f, 1.0f);		// Р’РµСЂС… РїСЂР°РІРѕ РєРІР°РґСЂР°С‚Р° (РџРµСЂРµРґ)
	glVertex3f(-1.0f, 1.0f, 1.0f);		// Р’РµСЂС… Р»РµРІРѕ
	glVertex3f(-1.0f,-1.0f, 1.0f);		// РќРёР· Р»РµРІРѕ
	glVertex3f( 1.0f,-1.0f, 1.0f);		// РќРёР· РїСЂР°РІРѕ
	glColor3f(1.0f,1.0f,0.0f);		// Р–РµР»С‚С‹Р№
	glVertex3f( 1.0f,-1.0f,-1.0f);		// Р’РµСЂС… РїСЂР°РІРѕ РєРІР°РґСЂР°С‚Р° (Р—Р°Рґ)
	glVertex3f(-1.0f,-1.0f,-1.0f);		// Р’РµСЂС… Р»РµРІРѕ
	glVertex3f(-1.0f, 1.0f,-1.0f);		// РќРёР· Р»РµРІРѕ
	glVertex3f( 1.0f, 1.0f,-1.0f);		// РќРёР· РїСЂР°РІРѕ
	glColor3f(0.0f,0.0f,1.0f);		// РЎРёРЅРёР№
	glVertex3f(-1.0f, 1.0f, 1.0f);		// Р’РµСЂС… РїСЂР°РІРѕ РєРІР°РґСЂР°С‚Р° (Р›РµРІРѕ)
	glVertex3f(-1.0f, 1.0f,-1.0f);		// Р’РµСЂС… Р»РµРІРѕ
	glVertex3f(-1.0f,-1.0f,-1.0f);		// РќРёР· Р»РµРІРѕ
	glVertex3f(-1.0f,-1.0f, 1.0f);		// РќРёР· РїСЂР°РІРѕ
	glColor3f(1.0f,0.0f,1.0f);		// Р¤РёРѕР»РµС‚РѕРІС‹Р№
	glVertex3f( 1.0f, 1.0f,-1.0f);		// Р’РµСЂС… РїСЂР°РІРѕ РєРІР°РґСЂР°С‚Р° (РџСЂР°РІРѕ)
	glVertex3f( 1.0f, 1.0f, 1.0f);		// Р’РµСЂС… Р»РµРІРѕ
	glVertex3f( 1.0f,-1.0f, 1.0f);		// РќРёР· Р»РµРІРѕ
	glVertex3f( 1.0f,-1.0f,-1.0f);		// РќРёР· РїСЂР°РІРѕ
	glEnd();				// Р—Р°РєРѕРЅС‡РёР»Рё РєРІР°РґСЂР°С‚С‹

	*/

}

void CMyEventHandler::OnTimer(int id)
{
	if(id == 100) rtri += 3.50f;
	if(id == 200) rquad += 3.50f;	
}


