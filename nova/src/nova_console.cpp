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
//---------------------------------------------------------------------------
#include "nova_stable_precompiled_headers.h"


#include "nova_console.h"

namespace nova {

#pragma warning(disable:4996)


void CConsoleBase::SetProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, initdata.screen.x, initdata.screen.y, 0, 10, 15);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0.0, 0.0, -11);
}

void CConsoleBase::DrawBackGround()
{
	if(!backtexture.IsNull())
		backtexture->ApplyTexture();
	glDisable(GL_LIGHTING);

	glBegin(GL_QUADS);
		glTexCoord2f(1, 0);
		glVertex3d(initdata.screen.x, 0, 0);
		glTexCoord2f(0, 0);
		glVertex3d(0, 0, 0);
		glTexCoord2f(0, 1);
		glVertex3d(0, initdata.screen.y, 0);
		glTexCoord2f(1, 1);
		glVertex3d(initdata.screen.x, initdata.screen.y, 0);
	glEnd();
}

void CConsoleBase::ClearStr()
{
	console_string.clear();
}
//---------------------- CConsole ----------------------------------------

CConsole::CConsole(nv_console_init * _init) : CConsoleBase("CConsole")
{
	console_string.clear();
	console_string.push_back(empty);

	position = 0;
	dy = 0;
	ypos = _init->screen.y;

	//font->InitWGLFont(hdc, NEWLINE, DISTANCE, "Lucida Console");
	//font->InitFreeFont("E:\\source\\Game\\GE\\BUILD\\CHICAGO.TTF", NEWLINE);
	memcpy(&initdata, _init, sizeof(struct nv_console_init));
}

int CConsole::Paint()
{
	glDepthMask(false);

	glDisable(GL_LIGHTING);

	SetProjection();
	DrawBackGround();

	glTranslated(0.0, 0.0, 0.5);
	glTranslated(0.0, dy, 0.0);

	int i = 0;
	register POINT TempPos;

	TempPos.x = TempPos.y = initdata.param.topclear;

	while(this->console_string[i].unicode)
	{
		if(console_string[i].unicode == L'\n')
		{
			TempPos.x = initdata.param.leftclear;
			TempPos.y += initdata.param.newline;// + static_cast<long>(font->FTop);
		}
		if(console_string[i].unicode == L'\t')
			TempPos.x += initdata.param.tab;

		if(TempPos.x > initdata.screen.x - initdata.param.rightclear)
		{
			TempPos.x = initdata.param.leftclear;
			TempPos.y += initdata.param.newline;// + static_cast<long>(font->FTop);
		}
		if(TempPos.y > ypos - initdata.param.bottomclear)
		{
			ypos += initdata.param.newline ;//+ static_cast<long>(font->FTop);
			dy -= initdata.param.newline ;//+ font->FTop;
		}
		if(console_string[i].unicode != L'\n' && console_string[i].unicode != L'\t')
		{
			font->Draw(TempPos, console_string[i].color, console_string[i].unicode);
			TempPos.x += initdata.param.distance;
		}
		i++;
	}
//	font->PrintChar(124, TempPos, N_RGB(200, 30, 200));

	glDepthMask(true);

	return EXIT_OK;
}

CConsole::~CConsole()
{
	console_string.clear();
}

void CConsole::WriteLen(const wchar_t *message, const CColorRGB & pc)
{
	register wchar_t *p = const_cast<wchar_t*>(message);
  /*if((position + MesLen) > this->Maxchar)
    MesLen = this->Maxchar - position - 1;
  strncat(this->console_string, message, MesLen);
  position += MesLen;
  if(position >= Maxchar - 1)
    ClearConsole();
  *(console_string + position + 1) = '\0';
  */

	while(*p)
	{
		PutChar(*p, pc);
		p++;
	}
}

void CConsole::WriteLen(const char *message, const CColorRGB & pc)
{
	register char *p = const_cast<char *>(message);

	while(*p)
	{
		PutChar(*p, pc);
		p++;
	}
}

void CConsole::WriteLen(const char *message)
{
	register char *p = const_cast<char *>(message);

	while(*p)
	{
		PutChar(*p, color);
		p++;
	}
}

void CConsole::WriteLen(const wchar_t *message)
{
	register wchar_t *p = const_cast<wchar_t*>(message);

	while(*p)
	{
		PutChar(*p, color);
		p++;
	}
}

void CConsole::WriteLen(const nstring & message, const CColorRGB & pc)
{
	WriteLen(message.c_str(), pc);
}

void CConsole::WriteLen(const nwstring & message, const CColorRGB & pc)
{
	WriteLen(message.c_str(), pc);
}

void CConsole::WriteLen(const nstring & message)
{
	WriteLen(message.c_str());
}

void CConsole::WriteLen(const nwstring & message)
{
	WriteLen(message.c_str());
}

void CConsole::Clear()
{
	this->ClearStr();
	console_string.push_back(empty);
	position = 0;
	dy = 0;
	ypos = initdata.screen.y;
}

int CConsole::PutChar(const wchar_t bb, const CColorRGB & pc)
{
	if(console_string.size() >= initdata.maxchar)
		console_string.erase(console_string.begin());
	nv_console_char sym = {bb, pc};

	console_string[console_string.size()-1] = sym;
	console_string.push_back(empty);
  //*(this->console_string + position-1) = bb;
  //*(this->console_string + position) = '\0';

	return EXIT_OK;
}

int CConsole::PutChar(const char bb, const CColorRGB & pc)
{
	if(console_string.size() >= initdata.maxchar)
		console_string.erase(console_string.begin());
	nv_console_char sym = {(wchar_t)bb, pc};

	console_string[console_string.size()-1] = sym;
	console_string.push_back(empty);
  //*(this->console_string + position-1) = bb;
  //*(this->console_string + position) = '\0';

	return EXIT_OK;
}

void CConsole::HoreLine(const wchar_t symbol)
{
	//int len = int(initdata.screen.x / (initdata.param.distance));

	for(int i = 0; i < 30; ++i)
		this->PutChar(symbol, color);

	PutChar('\n', color);
}

int CConsole::Delete(int x)
{
	int len = console_string.size() - x -1;
	if(len < 0)
		return -1;

	console_string.erase(console_string.end() - x, console_string.end());
	console_string[console_string.size()-1] = empty ;
	return EXIT_OK;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
template<> CConsoleManager * CSingelton<CConsoleManager>::SingeltonObject = NULL;

CConsoleManager::CConsoleManager() : CBase("CConsoleManager")
{
	active = -1;
	font = NULL;
}

void CConsoleManager::SetFont(CFontPtr _font)
{
	font = _font;
}


CConsoleManager::~CConsoleManager()
{
	Clear();
}

int CConsoleManager::NewConsole(nv_console_init * param)
{
	if(!param) return MEM_ERROR;

	CConsole *cc;


	cc = new CConsole(param);
	consoles.push_back(cc);
	cc->font = font;

	return EXIT_OK;
}

void CConsoleManager::DeleteConsole(uint n)
{
	delete consoles[n];
	if(consoles.size() > 0)
		active = 0;
	else
		active = -1;
}

const int CConsoleManager::Count()
{
	return consoles.size();
}

void CConsoleManager::Clear()
{
	for(uint i = 0; i < consoles.size(); i++)
		delete consoles[i];
	consoles.clear();
}

void CConsoleManager::Render()
{
	if(active < 0) return;
	consoles[active]->Paint();
}

void  CConsoleManager::SetActive(uint n)
{
	active = n;
}

CConsole * CConsoleManager::GetActivePtr()
{
	if(active < 0) return NULL;
	return consoles[active];
}

const int CConsoleManager::GetActive()
{
	return active;
}





}
