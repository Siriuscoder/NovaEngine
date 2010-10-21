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

/***************************** Render Function ************************************/
void CMyEventHandler::OnRender()
{
	//engine->GetRenderManager()->GetConsoleManager()->Render();
/*
glMatrixMode(GL_PROJECTION);// Выбор матрицы проекции
glLoadIdentity();		// Сброс матрицы проекции
gluPerspective(45.0f,(nReal)800/(nReal)600,0.1f,100.0f);
							// Вычислить соотношение геометрических размеров для окна
glMatrixMode(GL_MODELVIEW);// Выбор матрицы просмотра модели
glLoadIdentity();				// Сброс просмотра
glTranslatef(-1.5f,0.0f,-6.0f);			// Сдвиг влево и вглубь экрана
glRotatef(rtri,0.0f,1.0f,0.0f);			// Вращение пирамиды по оси Y
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
glLoadIdentity();
glTranslatef(1.5f,0.0f,-7.0f);		// Сдвинуть вправо и вглубь экрана
glRotatef(rquad,1.0f,1.0f,1.0f);	// Вращение куба по X, Y & Z
glBegin(GL_QUADS);			// Рисуем куб
	glColor3f(0.0f,1.0f,0.0f);		// Синий
	glVertex3f( 1.0f, 1.0f,-1.0f);		// Право верх квадрата (Верх)
	glVertex3f(-1.0f, 1.0f,-1.0f);		// Лево верх
	glVertex3f(-1.0f, 1.0f, 1.0f);		// Лево низ
	glVertex3f( 1.0f, 1.0f, 1.0f);		// Право низ
	glColor3f(1.0f,0.5f,0.0f);		// Оранжевый
	glVertex3f( 1.0f,-1.0f, 1.0f);		// Верх право квадрата (Низ)
	glVertex3f(-1.0f,-1.0f, 1.0f);		// Верх лево
	glVertex3f(-1.0f,-1.0f,-1.0f);		// Низ лево
	glVertex3f( 1.0f,-1.0f,-1.0f);		// Низ право
	glColor3f(1.0f,0.0f,0.0f);		// Красный
	glVertex3f( 1.0f, 1.0f, 1.0f);		// Верх право квадрата (Перед)
	glVertex3f(-1.0f, 1.0f, 1.0f);		// Верх лево
	glVertex3f(-1.0f,-1.0f, 1.0f);		// Низ лево
	glVertex3f( 1.0f,-1.0f, 1.0f);		// Низ право
	glColor3f(1.0f,1.0f,0.0f);		// Желтый
	glVertex3f( 1.0f,-1.0f,-1.0f);		// Верх право квадрата (Зад)
	glVertex3f(-1.0f,-1.0f,-1.0f);		// Верх лево
	glVertex3f(-1.0f, 1.0f,-1.0f);		// Низ лево
	glVertex3f( 1.0f, 1.0f,-1.0f);		// Низ право
	glColor3f(0.0f,0.0f,1.0f);		// Синий
	glVertex3f(-1.0f, 1.0f, 1.0f);		// Верх право квадрата (Лево)
	glVertex3f(-1.0f, 1.0f,-1.0f);		// Верх лево
	glVertex3f(-1.0f,-1.0f,-1.0f);		// Низ лево
	glVertex3f(-1.0f,-1.0f, 1.0f);		// Низ право
	glColor3f(1.0f,0.0f,1.0f);		// Фиолетовый
	glVertex3f( 1.0f, 1.0f,-1.0f);		// Верх право квадрата (Право)
	glVertex3f( 1.0f, 1.0f, 1.0f);		// Верх лево
	glVertex3f( 1.0f,-1.0f, 1.0f);		// Низ лево
	glVertex3f( 1.0f,-1.0f,-1.0f);		// Низ право
	glEnd();				// Закончили квадраты

	*/

}

void CMyEventHandler::OnTimer(int id)
{
	if(id == 100) rtri += 3.50f;
	if(id == 200) rquad += 3.50f;	
}


