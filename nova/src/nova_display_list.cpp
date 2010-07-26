п»ї/***************************************************************************
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

#include "nova_display_list.h"

namespace nova
{

CDisplayList::CDisplayList() : CBase("CDisplayList")
{

}

CDisplayList::~CDisplayList()
{

}

void CDisplayList::CreateList()
{
	listname = glGenLists(1);
	if(!listname)
		throw NOVA_EXP("CDisplayList::CDisplayList() - Can not create display list...", BAD_OPERATION);
}

void CDisplayList::DestroyList()
{
	glDeleteLists(listname, 1);
}

void CDisplayList::BeginList()
{
	glNewList(listname, GL_COMPILE);
	glPushAttrib(GL_ENABLE_BIT | GL_TRANSFORM_BIT);
}

void CDisplayList::EndList()
{
	glPopAttrib();
	glEndList();
}

void CDisplayList::SetList()
{
	glCallList(listname);
}

bool CDisplayList::IsList()
{
	return (glIsList(listname) == GL_TRUE) ? true : false;
}


}