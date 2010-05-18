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
#include "nova_stable_precompiled_headers.h"

#include "nova_events.h"
#include "nova_render_system.h"
#include "nova_fonts.h"

namespace nova {

template<> CEventConveyor * CSingelton<CEventConveyor>::SingeltonObject = NULL;

CEventConveyor::CEventConveyor() : CBase("CEventConveyor")
{
	fCHAR = NULL;
	fREPAINT = NULL;
	fBEGIN = NULL;
	fTIMER_TICK = NULL;
	fEND = NULL;
	fMOUSEMOVE = NULL;
	fRBUTTONUP = NULL;
	fRBUTTONDBLCLK = NULL;
	fLBUTTONDBLCLK = NULL;
	fMBUTTONDBLCLK = NULL;
	fLBUTTONDOWN = NULL;
	fLBUTTONUP = NULL;
	fMBUTTONDOWN = NULL;
	fON_RENDER = NULL;
	fKEYDOWN = NULL;
	fMBUTTONUP = NULL;
	fRBUTTONDOWN = NULL;
	fKEYUP = NULL;
	fQUIT = NULL;

//	conv.clear();
}

CEventConveyor::~CEventConveyor()
{
//	ClearEvents();

	fCHAR = NULL;
	fREPAINT = NULL;
	fBEGIN = NULL;
	fTIMER_TICK = NULL;
	fEND = NULL;
	fMOUSEMOVE = NULL;
	fRBUTTONUP = NULL;
	fRBUTTONDBLCLK = NULL;
	fLBUTTONDBLCLK = NULL;
	fMBUTTONDBLCLK = NULL;
	fLBUTTONDOWN = NULL;
	fLBUTTONUP = NULL;
	fMBUTTONDOWN = NULL;
	fON_RENDER = NULL;
	fKEYDOWN = NULL;
	fMBUTTONUP = NULL;
	fRBUTTONDOWN = NULL;
	fKEYUP = NULL;
	fQUIT = NULL;
}
/*

void CEventConveyor::ClearEvents()
{
	if(GetConveyorSize() <= 0)
		return;

	stl<nv_ev_msg *>::list::iterator Iter;

	for(Iter = conv.begin(); Iter != conv.end(); Iter++)
	{
		nv_ev_msg * pevent = *Iter;
		freemem(pevent);
	}

	conv.clear();
}

void CEventConveyor::PushEvent(nv_ev_msg * _event)
{
	if(_event)
		conv.push_back(_event);
}

uint CEventConveyor::GetConveyorSize()
{
	return conv.size();
}

int CEventConveyor::GetEvent(nv_ev_msg * _event)
{
	if(GetConveyorSize() <= 0)
		return -1;

	if(!_event)
		throw NOVA_EXP("_event is NULL - can not write the event!", MEM_ERROR);

	stl<nv_ev_msg *>::list::iterator Iter;
	Iter = conv.begin();
	nv_ev_msg * mes = *Iter;

	memcpy(_event, mes, sizeof(nv_ev_msg));

	freemem(mes);
	conv.erase(Iter);

	//delete mes;
	return 1;
}

*/

int CEventConveyor::Generate(nv_ev_msg * _event)
{
	switch(_event->eventid)
	{
	case EV_CHAR:
			if(fCHAR)
				(this->*fCHAR)(_event->chCode.chCharCode, _event->iKeyData);
	break;
	case EV_REPAINT:
		if(this->fREPAINT)
			(this->*fREPAINT)();
	break;
	case EV_BEGIN:
		if(this->fBEGIN)
			(this->*fBEGIN)();
	break;
	case EV_TIMER_TICK:
		if(this->fTIMER_TICK)
			(this->*fTIMER_TICK)(_event->timerid);
	break;
	case EV_END:
		if(this->fEND)
			(this->*fEND)();
	break;
	case EV_MOUSEMOVE:
		if(this->fMOUSEMOVE)
			(this->*fMOUSEMOVE)(_event->Xpos, _event->Ypos, _event->fwKeys);
	break;
	case EV_RBUTTONUP:
		if(this->fRBUTTONUP)
			(this->*fRBUTTONUP)(_event->Xpos, _event->Ypos, _event->fwKeys);
	break;
	case EV_RBUTTONDOWN:
		if(this->fRBUTTONDOWN)
			(this->*fRBUTTONDOWN)(_event->Xpos, _event->Ypos, _event->fwKeys);
	break;
	case EV_RBUTTONDBLCLK:
		if(this->fRBUTTONDBLCLK)
			(this->*fRBUTTONDBLCLK)(_event->Xpos, _event->Ypos);
	break;
	case EV_LBUTTONDBLCLK:
		if(this->fLBUTTONDBLCLK)
			(this->*fLBUTTONDBLCLK)(_event->Xpos, _event->Ypos);
	break;
	case EV_LBUTTONDOWN:
		if(this->fLBUTTONDOWN)
			(this->*fLBUTTONDOWN)(_event->Xpos, _event->Ypos, _event->fwKeys);
	break;
	case EV_LBUTTONUP:
		if(this->fLBUTTONUP)
			(this->*fLBUTTONUP)(_event->Xpos, _event->Ypos, _event->fwKeys);
	break;
	case EV_MBUTTONDOWN:
		if(this->fMBUTTONDOWN)
			(this->*fMBUTTONDOWN)(_event->Xpos, _event->Ypos, _event->fwKeys);
	break;
	case EV_MBUTTONUP:
		if(this->fMBUTTONUP)
			(this->*fMBUTTONUP)(_event->Xpos, _event->Ypos, _event->fwKeys);
	break;
	case EV_MBUTTONDBLCLK:
		if(this->fMBUTTONDBLCLK)
			(this->*fMBUTTONDBLCLK)(_event->Xpos, _event->Ypos);
	break;
	case EV_KEYDOWN:
		if(this->fKEYDOWN)
			(this->*fKEYDOWN)(_event->chCode.nVirtKey, _event->iKeyData);
	break;
	case EV_ON_RENDER:
		if(this->fON_RENDER)
			(this->*fON_RENDER)();
	break;
	case EV_KEYUP:
		if(this->fKEYUP)
			(this->*fKEYUP)(_event->chCode.nVirtKey, _event->iKeyData);
	break;
	case EV_QUIT:
		if(this->fQUIT)
			(this->*fQUIT)();
	break;
	}
	return 0;
}



void CEventConveyor::OnRender()
{
	nv_ev_msg  sevent;
	sevent.eventid = EV_ON_RENDER;

	Generate(&sevent);
}

/*

int CEventConveyor::Execute()
{
	nv_ev_msg message;
	int col = 0;

	while(GetEvent(&message) > 0)
	{
		Generate(&message);
		col++;
	}

	return col;
}

*/

#ifdef WIN_BUILD

int CrossEventHandler::Win32MainLoopFunction(CRenderWindow * window)
{
	MSG msg;
	int DispMess = 0;

	while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
	{
		if (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			DispMess++;
		}
	}

	return DispMess;
}

LRESULT CrossEventHandler::WinProcFunction(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	nv_ev_msg message;
	memset(&message, 0, sizeof(nv_ev_msg));
	//nv_ev_msg message;
	if(msg == WM_CREATE)
	{
		CREATESTRUCT * cStr = (CREATESTRUCT *)lParam;

		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)cStr->lpCreateParams);
	}

	CRenderWindow * window = (CRenderWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if(window == NULL)
		return DefWindowProc(hwnd, msg, wParam, lParam);

	CEventConveyor * Generator = window->GetEventHandler();

	switch(msg)
	{
// перересовка
	case WM_PAINT:
		{
			message.eventid = EV_REPAINT;
			Generator->Generate(&message);

			RECT rect;
			GetClientRect(hwnd, &rect);
			ValidateRect(hwnd, &rect);
		}
	break;
// создание окна
	case WM_CREATE:
		{
			message.eventid = EV_BEGIN;
			Generator->Generate(&message);
		}
	break;
// уничтожение окна
	case WM_DESTROY:
		{
			message.eventid = EV_END;
			Generator->Generate(&message);
		}
	break;
// таймеры
	case WM_TIMER:
		{
			message.eventid = EV_TIMER_TICK;
			message.timerid = wParam;
			Generator->Generate(&message);
		}
	break;
// движение мыши
    case WM_MOUSEMOVE:
		{
			message.eventid = EV_MOUSEMOVE;
			message.Xpos = LOWORD(lParam);
			message.Ypos = HIWORD(lParam);
			switch(wParam)
			{
			case MK_CONTROL:
				message.fwKeys = BT_CONTROL;
			break;
			case MK_LBUTTON:
				message.fwKeys = BT_LBUTTON;
			break;
			case MK_SHIFT:
				message.fwKeys = BT_SHIFT;
			break;
			}
			Generator->Generate(&message);
		}
	break;
// права€ кнопка мыши
    case WM_RBUTTONUP:
		{
			message.eventid = EV_RBUTTONUP;
			message.Xpos = LOWORD(lParam);
			message.Ypos = HIWORD(lParam);
			switch(wParam)
			{
			case MK_CONTROL:
				message.fwKeys = BT_CONTROL;
			break;
			case MK_LBUTTON:
				message.fwKeys = BT_LBUTTON;
			break;
			case MK_SHIFT:
				message.fwKeys = BT_SHIFT;
			break;
			case MK_MBUTTON:
				message.fwKeys = BT_MBUTTON;
			break;
			case MK_RBUTTON:
				message.fwKeys = BT_RBUTTON;
			break;
			}
			Generator->Generate(&message);
		}
	break;
    case WM_RBUTTONDOWN:
		{
			message.eventid = EV_RBUTTONDOWN;
			message.Xpos = LOWORD(lParam);
			message.Ypos = HIWORD(lParam);
			switch(wParam)
			{
			case MK_CONTROL:
				message.fwKeys = BT_CONTROL;
			break;
			case MK_LBUTTON:
				message.fwKeys = BT_LBUTTON;
			break;
			case MK_SHIFT:
				message.fwKeys = BT_SHIFT;
			break;
			case MK_MBUTTON:
				message.fwKeys = BT_MBUTTON;
			break;
			case MK_RBUTTON:
				message.fwKeys = BT_RBUTTON;
			break;
			}
			Generator->Generate(&message);
		}
	break;
	case WM_RBUTTONDBLCLK:
		{
			message.eventid = EV_RBUTTONDBLCLK;
			message.Xpos = LOWORD(lParam);
			message.Ypos = HIWORD(lParam);
			Generator->Generate(&message);
		}
	break;
// лева€ кнопка мыши
	case WM_LBUTTONDBLCLK:
		{
			message.eventid = EV_LBUTTONDBLCLK;
			message.Xpos = LOWORD(lParam);
			message.Ypos = HIWORD(lParam);
			Generator->Generate(&message);
		}
	break;
	case WM_LBUTTONDOWN:
		{
			message.eventid = EV_LBUTTONDOWN;
			message.Xpos = LOWORD(lParam);
			message.Ypos = HIWORD(lParam);
			switch(wParam)
			{
			case MK_CONTROL:
				message.fwKeys = BT_CONTROL;
			break;
			case MK_LBUTTON:
				message.fwKeys = BT_LBUTTON;
			break;
			case MK_SHIFT:
				message.fwKeys = BT_SHIFT;
			break;
			case MK_MBUTTON:
				message.fwKeys = BT_MBUTTON;
			break;
			case MK_RBUTTON:
				message.fwKeys = BT_RBUTTON;
			break;
			}
			Generator->Generate(&message);
		}
	break;
    case WM_LBUTTONUP:
		{
			message.eventid = EV_LBUTTONUP;
			message.Xpos = LOWORD(lParam);
			message.Ypos = HIWORD(lParam);
			switch(wParam)
			{
			case MK_CONTROL:
				message.fwKeys = BT_CONTROL;
			break;
			case MK_LBUTTON:
				message.fwKeys = BT_LBUTTON;
			break;
			case MK_SHIFT:
				message.fwKeys = BT_SHIFT;
			break;
			case MK_MBUTTON:
				message.fwKeys = BT_MBUTTON;
			break;
			case MK_RBUTTON:
				message.fwKeys = BT_RBUTTON;
			break;
			}
			Generator->Generate(&message);
		}
	break;
// средн€€ кнопка мыши
	case WM_MBUTTONDOWN:
		{
			message.eventid = EV_MBUTTONDOWN;
			message.Xpos = LOWORD(lParam);
			message.Ypos = HIWORD(lParam);
			switch(wParam)
			{
			case MK_CONTROL:
				message.fwKeys = BT_CONTROL;
			break;
			case MK_LBUTTON:
				message.fwKeys = BT_LBUTTON;
			break;
			case MK_SHIFT:
				message.fwKeys = BT_SHIFT;
			break;
			case MK_MBUTTON:
				message.fwKeys = BT_MBUTTON;
			break;
			case MK_RBUTTON:
				message.fwKeys = BT_RBUTTON;
			break;
			}
			Generator->Generate(&message);
		}
	break;
	case WM_MBUTTONUP:
		{
			message.eventid = EV_MBUTTONUP;
			message.Xpos = LOWORD(lParam);
			message.Ypos = HIWORD(lParam);
			switch(wParam)
			{
			case MK_CONTROL:
				message.fwKeys = BT_CONTROL;
			break;
			case MK_LBUTTON:
				message.fwKeys = BT_LBUTTON;
			break;
			case MK_SHIFT:
				message.fwKeys = BT_SHIFT;
			break;
			case MK_MBUTTON:
				message.fwKeys = BT_MBUTTON;
			break;
			case MK_RBUTTON:
				message.fwKeys = BT_RBUTTON;
			break;
			}
			Generator->Generate(&message);
		}
	break;
	case WM_MBUTTONDBLCLK:
		{
			message.eventid = EV_MBUTTONDBLCLK;
			message.Xpos = LOWORD(lParam);
			message.Ypos = HIWORD(lParam);
			Generator->Generate(&message);
		}
	break;
// нажатие клавиш
    case WM_KEYDOWN:
		{
			message.eventid = EV_KEYDOWN;
			message.chCode.nVirtKey = static_cast<int>(wParam);
			message.iKeyData = lParam;
			Generator->Generate(&message);
		}
	break;
	case WM_KEYUP:
		{
			message.eventid = EV_KEYUP;
			message.chCode.nVirtKey = static_cast<int>(wParam);
			message.iKeyData = lParam;
			Generator->Generate(&message);
		}
	break;
    case WM_CHAR:
		{
			//wchar_t ecode[3] = L"";
			//char in[2] = "";
			message.eventid = EV_CHAR;
			//CCodeConvertion encode("cp1251", "utf-16");
			//byte key = wParam;
			//in[0] = (char)wParam;
			//encode.Convert(in, (char *)ecode, 2);
			message.chCode.chCharCode = CNormalUnicode::Convert((char)wParam);
			message.iKeyData = lParam;
			Generator->Generate(&message);
		}
	break;
	case WM_CLOSE:
		{
			window->Close();
		}
	break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

#endif

int CrossEventHandler::SDLMainLoopFunction(CRenderWindow * window)
{
    SDL_Event sys_event;
	int DispMess = 0;

		// process pending events
    while(SDL_PollEvent(&sys_event))
	{
		SDLProc(sys_event, window);
		DispMess++;
	}

	return DispMess;
}

int CrossEventHandler::SDLProc(SDL_Event & s_event, CRenderWindow * window)
{
	nv_ev_msg message;
	memset(&message, 0, sizeof(nv_ev_msg));

	CEventConveyor * Generator = window->GetEventHandler();

	switch(s_event.type)
	{
	case SDL_VIDEORESIZE:
		{
			message.eventid = EV_REPAINT;
			Generator->Generate(&message);
		}
		break;
	case SDL_KEYDOWN:
		{
			message.eventid = EV_CHAR;
			message.chCode.chCharCode = s_event.key.keysym.unicode;
			message.iKeyData = s_event.key.keysym.mod;
			Generator->Generate(&message);

			nv_ev_msg message1;
			memset(&message1, 0, sizeof(nv_ev_msg));

			message1.eventid = EV_KEYDOWN;
			message1.chCode.nVirtKey = s_event.key.keysym.sym;
			message1.iKeyData = s_event.key.keysym.mod;
			Generator->Generate(&message1);
		}
		break;
	case SDL_KEYUP:
		{
			message.eventid = EV_KEYUP;
			message.chCode.nVirtKey = s_event.key.keysym.sym;
			message.iKeyData = s_event.key.keysym.mod;
			Generator->Generate(&message);
		}
		break;
	case SDL_MOUSEMOTION:
		{
			message.eventid = EV_MOUSEMOVE;
			message.Xpos = s_event.motion.x;
			message.Ypos = s_event.motion.y;

			if(s_event.motion.state & SDL_BUTTON(SDL_BUTTON_LEFT))
				message.fwKeys = BT_LBUTTON;
			if(s_event.motion.state & SDL_BUTTON(SDL_BUTTON_MIDDLE))
				message.fwKeys = BT_MBUTTON;
			if(s_event.motion.state & SDL_BUTTON(SDL_BUTTON_RIGHT))
				message.fwKeys = BT_RBUTTON;
			Generator->Generate(&message);
		}
		break;
	case SDL_MOUSEBUTTONDOWN:
		{
			message.Xpos = s_event.button.x;
			message.Ypos = s_event.button.y;

			switch(s_event.button.button)
			{
			case SDL_BUTTON_LEFT:
				message.eventid = EV_LBUTTONDOWN;
				break;
			case SDL_BUTTON_MIDDLE:
				message.eventid = EV_LBUTTONDOWN;
				break;
			case SDL_BUTTON_RIGHT:
				message.eventid = EV_LBUTTONDOWN;
				break;
			}
			Generator->Generate(&message);
		}
		break;
	case SDL_MOUSEBUTTONUP:
		{
			message.Xpos = s_event.button.x;
			message.Ypos = s_event.button.y;

			switch(s_event.button.button)
			{
			case SDL_BUTTON_LEFT:
				message.eventid = EV_LBUTTONUP;
				break;
			case SDL_BUTTON_MIDDLE:
				message.eventid = EV_LBUTTONUP;
				break;
			case SDL_BUTTON_RIGHT:
				message.eventid = EV_LBUTTONUP;
				break;
			}
			Generator->Generate(&message);
		}
		break;
	case SDL_QUIT:
		{
			message.eventid = EV_QUIT;
			Generator->Generate(&message);
			window->Close();
		}
	}

	return 0;
}

}
