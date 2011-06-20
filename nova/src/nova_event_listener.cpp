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

#include "nova_event_listener.h"

namespace nova
{

CListenerInterface::CListenerInterface()
{
	mListeners.clear();
}

CListenerInterface::~CListenerInterface()
{
	RemoveAllListeners();
}

void CListenerInterface::AddListener(CEventListener * listener)
{
	if(listener)
	{
		mListeners.push_back(listener);
	}
}

void CListenerInterface::RemoveListener(CEventListener * listener)
{
	TListeners::iterator it;

	it = mListeners.begin();
	for(; it != mListeners.end(); it++)
		if((*it) == listener)
		{
			//delete listener;
			mListeners.erase(it);
			break;
		}
}

void CListenerInterface::RemoveListener(nova::nUInt32 listener)
{
	if(listener < mListeners.size())
	{
		mListeners.erase(mListeners.begin() + listener);
	}
}

void CListenerInterface::RemoveAllListeners()
{
	TListeners::iterator it;

	it = mListeners.begin();
	//for(; it != mListeners.end(); it++)
	//	delete (*it);

	mListeners.clear();
}

nova::nUInt32 CListenerInterface::GetListenersCount()
{
	return mListeners.size();
}

CEventListener * CListenerInterface::GetListener(nova::nUInt32 i)
{
	if(i < mListeners.size())
		return mListeners[i];

	return NULL;
}

}