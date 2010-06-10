/***************************************************************************
 *   Copyright (C) 2009 by Sirius										   *
 *	 Vdov Nikita Sergeevich	(c)											   *
 *	 siriusnick@gmail.com												   *
 *																		   *
 *	 This source file is part of NovaEngine								   *
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
#include "nova_stable_precompiled_headers.h"

#include "nova_lenear_scene_manager.h"

namespace nova
{

/*
CWorldObject *CTreeSceneManager::ConstructRenderableObject(const nstring & name)
{
	CMovableObject *myobject = NULL;
	myobject = new CMovableObject(name, mRegisterGroup);
	return dynamic_cast<CWorldObject *>(myobject);
}

CWorldObject *CTreeSceneManager::ConstructSingleObject(const nstring & name)
{
	CWorldObject *obj = ConstructRenderableObject(name);

	return obj;
}

void CTreeSceneManager::RenderSceneImpl()
{

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
}

*/

}