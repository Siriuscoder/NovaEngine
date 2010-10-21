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

#include "nova_render_window.h"

namespace nova
{

nInt32 CRenderWindow::GetXPosition()
{
	return this->mXPosition;
}

nInt32 CRenderWindow::GetYPosition()
{
	return this->mYPosition;
}

bool CRenderWindow::IsFullScreen()
{
	return this->mFullScreen;
}

bool CRenderWindow::IsClosed()
{
	return this->mClosed;
}

void CRenderWindow::GetMetrics(nInt32 & xpos, nInt32 & ypos,
		nova::nUInt32 & height, nova::nUInt32 & width,
		nova::nUInt32 & color_depth, nova::nUInt32 & color_bits)
{
	xpos = mXPosition;
	ypos = mYPosition;

	height = this->mHeight;
	width = this->mWidth;

	color_depth = this->mColourDepth;
	color_bits = this->mColourBits;
}

TWindowInitialTarget CRenderWindow::GetMetrics()
{
	return mMetrics;
}

void CRenderWindow::SetEventHandler(CEventConveyor * handler)
{
	if(mEventHandler)
		throw NOVA_EXP("CRenderWindow::SetEventHandler - This window already have event handler!", BAD_OPERATION);

	if(handler)
		mEventHandler = handler;
}

CEventConveyor * CRenderWindow::GetEventHandler()
{
	return mEventHandler;
}

CGLContext * CRenderWindow::GetContext()
{
	return mContext;
}

CGLSupport * CRenderWindow::GetSupport()
{
	return mSupport;
}

void CRenderWindow::Close()
{
	mClosed = true;
}

void CRenderWindow::PreUpdateAction()
{
	CRenderTarget::PreUpdateAction();
}

void CRenderWindow::PostUpdateAction()
{
	CRenderTarget::PostUpdateAction();
}

nInt32 CRenderWindow::MessagePump(void)
{
	if(mLoopFunc != NULL)
		return mLoopFunc(this);

	return 0;
}

}