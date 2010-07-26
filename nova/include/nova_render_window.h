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
#pragma once

#include "nova_render_target.h"
#include "nova_events.h"
#include "nova_glsupport.h"

namespace nova
{

struct WindowInitialTarget
{
	// Р‘РёС‚С‹ С†РІРµС‚Р°
	nova::byte color_bits;
	// Р”РІРѕР№РЅР°СЏ Р±СѓС„РµСЂРёР·Р°С†РёСЏ
	// 0 - РЅРµС‚, Р±РѕР»СЊС€Рµ 0 - РґР°
	bool d_buff;
	// РџРѕР»РЅРѕСЌРєСЂР°РЅРЅС‹Р№ СЂРµР¶РёРј
	// 0 - РЅРµС‚, Р±РѕР»СЊС€Рµ 0 - РґР°
	nova::byte fullscreen;
	// Р§Р°СЃС‚РѕС‚Р° СЌР»РµРєС‚СЂРѕРЅРЅРѕР№ С‚СЂСѓР±РєРё
	// РїСЂРёРёРµРЅСЏРµС‚СЃСЏ С‚РѕР»СЊРєРѕ РІ СЃР»СѓС‡Р°РµРµ РїРѕР»РЅРѕСЌРєСЂР°РЅРЅРѕРіРѕ СЂРµР¶РёРјР°
	// Р°РєРєСѓСЂР°С‚РЅРµРµ СЃ СЌС‚РёРј РїРѕР»РµРј СЃС‚Р°РІРёРј Р·РЅР°С‡РµРЅРёСЏ 60-100 РіРµСЂС†
	// Сѓ РєРѕРіРѕ Р–Рљ РјРѕРЅРёС‚РѕСЂС‹ СЃС‚Р°РІРёРј 60-80 РіРµСЂС†
	nova::byte freq;

	int xpos;

	int ypos;

	nova::uint height;

	nova::uint width;

	nova::uint FSAA;

	bool DepthBuffered;

	bool vsync;
};


class CRenderWindow;


typedef int (*MainLoopUtility)(CRenderWindow * window);

class NOVA_EXPORT CRenderWindow : public CRenderTarget
{
protected:

	int mXPosition;
	int mYPosition;

	bool mFullScreen;
	bool mClosed;

	WindowInitialTarget mMetrics;

	CEventConveyor * mEventHandler;

	CGLSupport * mSupport;
	CGLContext * mContext;

	MainLoopUtility mLoopFunc;

public:

	CRenderWindow() : mXPosition(0), mYPosition(0),
		mFullScreen(false), mClosed(false), mSupport(NULL), 
		mContext(NULL), mEventHandler(NULL)
	{
		memset(&mMetrics, 0, sizeof(WindowInitialTarget));
		mLoopFunc = NULL;
	}

	virtual ~CRenderWindow() {}

	void GetMetrics(int & xpos, int & ypos,
		nova::uint & height, nova::uint & width,
		nova::uint & color_depth, nova::uint & color_bits);

	WindowInitialTarget GetMetrics();

	virtual void CreateRenderWindow(WindowInitialTarget & init) = 0;

	void PrepareTarget() {}

	virtual void Destroy() = 0;

	virtual void SwapBuffers() {} 

	int GetXPosition();

	int GetYPosition();

	bool IsFullScreen();

	bool IsClosed();

	void SetEventHandler(CEventConveyor * handler);

	CEventConveyor * GetEventHandler();

	virtual void PreUpdateAction();

	virtual void PostUpdateAction();

	CGLContext * GetContext();

	CGLSupport * GetSupport();

	void Close();

	int MessagePump(void);

	virtual void SaveContents(nstring & file) {}
};

typedef CSmartPtr<CRenderWindow> CRenderWindowPtr;

}