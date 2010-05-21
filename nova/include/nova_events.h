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
#pragma once

#include "nova_error.h"
#include "nova_singelton.h"

namespace nova {

class NOVA_EXPORT CEventConveyor;

typedef void(THISCALL__ CEventConveyor::*pfCHAR)(word, int);
typedef void(THISCALL__ CEventConveyor::*pfREPAINT)(void);
typedef void(THISCALL__ CEventConveyor::*pfBEGIN)(void);
typedef void(THISCALL__ CEventConveyor::*pfTIMER_TICK)(int);
typedef void(THISCALL__ CEventConveyor::*pfEND)(void);
typedef void(THISCALL__ CEventConveyor::*pfMOUSEMOVE)(int, int, int);
typedef void(THISCALL__ CEventConveyor::*pfRBUTTONUP)(int, int, int);
typedef void(THISCALL__ CEventConveyor::*pfRBUTTONDBLCLK)(int, int);
typedef void(THISCALL__ CEventConveyor::*pfLBUTTONDBLCLK)(int, int);
typedef void(THISCALL__ CEventConveyor::*pfMBUTTONDBLCLK)(int, int);
typedef void(THISCALL__ CEventConveyor::*pfLBUTTONDOWN)(int, int, int);
typedef void(THISCALL__ CEventConveyor::*pfLBUTTONUP)(int, int, int);
typedef void(THISCALL__ CEventConveyor::*pfMBUTTONDOWN)(int, int, int);
typedef void(THISCALL__ CEventConveyor::*pfON_RENDER)(void);
typedef void(THISCALL__ CEventConveyor::*pfMBUTTONUP)(int, int, int);
typedef void(THISCALL__ CEventConveyor::*pfRBUTTONDOWN)(int, int, int);
typedef void(THISCALL__ CEventConveyor::*pfQUIT)(void);

typedef void(THISCALL__ CEventConveyor::*pfKEYDOWN)(int, int);
typedef void(THISCALL__ CEventConveyor::*pfKEYUP)(int, int);

enum TEvents
{
	EV_CHAR	=			0xf0,
	EV_REPAINT =		0xf1,//
	EV_BEGIN =			0xf2,//
	EV_TIMER_TICK =		0xf3,//
	EV_END =			0xf4,//
	EV_MOUSEMOVE =		0xf5,//
	EV_RBUTTONUP =		0xf6,//
	EV_RBUTTONDOWN =	0xf7,//
	EV_RBUTTONDBLCLK =	0xf8,//
	EV_LBUTTONDBLCLK =	0xf9,//
	EV_LBUTTONDOWN =	0xfa,//
	EV_LBUTTONUP =		0xfb,//
	EV_MBUTTONDOWN =	0xfc,//
	EV_MBUTTONUP =		0xfd,//
	EV_MBUTTONDBLCLK =	0xfe,//
	EV_KEYDOWN =		0xff,//
	EV_KEYUP =			0xef,//
	EV_QUIT =			0xcc,//

	EV_ON_RENDER =		0xA1//
};

enum TControls
{
	BT_CONTROL = 		0xB1,
	BT_LBUTTON =		0xB2,
	BT_MBUTTON =		0xB3,
	BT_RBUTTON =		0xB4,
	BT_SHIFT =			0xB5
};

typedef struct __msg
{
	TEvents eventid;

	int Xpos;
	int Ypos;
	int fwKeys;

	int iKeyData;
	int timerid;

	union {
		nova::word chCharCode;
		int nVirtKey;
	} chCode;
} nv_ev_msg;

class NOVA_EXPORT CEventConveyor : public CSingelton<CEventConveyor>, public CBase
{
private:
//	stl<nv_ev_msg *>::list conv;

protected:
	pfCHAR				fCHAR;		
	pfREPAINT			fREPAINT;
	pfBEGIN				fBEGIN;
	pfTIMER_TICK		fTIMER_TICK;
	pfEND				fEND;
	pfMOUSEMOVE			fMOUSEMOVE;
	pfRBUTTONUP			fRBUTTONUP;
	pfRBUTTONDBLCLK		fRBUTTONDBLCLK;
	pfLBUTTONDBLCLK		fLBUTTONDBLCLK;
	pfMBUTTONDBLCLK		fMBUTTONDBLCLK;
	pfLBUTTONDOWN		fLBUTTONDOWN;
	pfLBUTTONUP			fLBUTTONUP;
	pfMBUTTONDOWN		fMBUTTONDOWN;
	pfON_RENDER			fON_RENDER;
	pfKEYDOWN			fKEYDOWN;
	pfMBUTTONUP			fMBUTTONUP;
	pfRBUTTONDOWN		fRBUTTONDOWN;
	pfKEYUP				fKEYUP;
	pfQUIT				fQUIT;

public:
	CEventConveyor();
	~CEventConveyor();

//	void PushEvent(nv_ev_msg * _event);
//	uint GetConveyorSize();

//	int GetEvent(nv_ev_msg * _event);
//	void ClearEvents();

	int Generate(nv_ev_msg * _event);

	void OnRender();
//	int Execute();
};

class CRenderWindow;


class NOVA_EXPORT CrossEventHandler
{
public:

#ifdef WIN_BUILD
	static int Win32MainLoopFunction(CRenderWindow * window);

	static LRESULT CALLBACK WinProcFunction(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

	static int SDLMainLoopFunction(CRenderWindow * window);
	static int SDLProc(SDL_Event & s_event, CRenderWindow * window);

};


}
