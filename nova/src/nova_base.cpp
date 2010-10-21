/***************************************************************************
 *   Copyright (C) 2007 by SIRIUS										   *
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
#include "nova_stable_precompiled_headers.h"
// Базовый класс для всех классов игры

#include "nova_base.h"
#include "nova_threads.h"

namespace nova {

nUInt32 gClassGenerate = 0x0;

CBase::CBase(char * _name)
{
	if(mObjectName)
		strcpy(this->mObjectName, _name);
	
	OBJECT_MUTEX_SECTION_LOCK;
	gClassGenerate++;
	OBJECT_MUTEX_SECTION_UNLOCK;

	mIDObject = gClassGenerate;
	if(gClassGenerate > 0xEEEFFFFF)
		gClassGenerate = 0x0;
	mParent = NULL;
}

int CBase::GetObjectId()
{
	return mIDObject;
}

void CBase::GetClassName(char *buff)
{
	if(buff)
		strcpy(buff, mObjectName);
}

nUInt32 CBase::GetClassSizeO(CBase * pobject)
{
	if(pobject)
		return sizeof(pobject);
	else return 0;
}

nUInt32 CBase::GetClassSize()
{
	return sizeof(this);
}

}



