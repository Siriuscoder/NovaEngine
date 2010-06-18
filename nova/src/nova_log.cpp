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
#include "nova_log.h"
#include "nova_console.h"
#include "nova_threads.h"


namespace nova
{


//CLog * CLog::SingeltonObject = NULL;
CLog * CLog::mInstance = NULL;

CLog::CLog(nstring & file) : mState(LG_ACTIVE), CBase("CLog")
{
	mLogFile.open(file.c_str(), ios_base::out);
	mOut = &std::cout;

	if(!mInstance)
		mInstance = this;
	else throw NOVA_EXP("CLog::CLog - log stream already opened!", BAD_OPERATION);

#if defined(__WIN32__)
	AllocConsole();

	FILE *hf = _fdopen(_open_osfhandle(
                (long)GetStdHandle(STD_OUTPUT_HANDLE), 2), "w");

	if(hf)
		*stdout = *stderr = *hf;
#endif
}

CLog::~CLog()
{
	Close();
	mInstance = NULL;

#if defined(__WIN32__)
	FreeConsole();
#endif
}

void CLog::Flush()
{
	mLogFile.flush();
}

void CLog::Close()
{
	mLogFile.close();
	mState = LG_CLOSED;
}

void CLog::SetState(LogState state)
{
	mState = state;
}

void CLog::ReDirectTo(std::ostream & out)
{
	mOut = &out;
	mState = LG_REDIRECTED;
}

int CLog::PrintMessage(const char * mes, LogFormat format)
{
	nova::nstring smes(mes);
	return PrintMessage(smes, format);
}


int CLog::PrintMessage(const nova::nstring & mes, LogFormat format)
{
	nstringstream res;
	time_t now;
	struct tm * nowst;

	switch(format)
	{
	case LG_WITH_TIME:
		{
			time(&now);
			nowst = localtime(&now);
			res << std::setw(2) << std::setfill('0') << nowst->tm_hour
				<< ":" << std::setw(2) << std::setfill('0') << nowst->tm_min
				<< ":" << std::setw(2) << std::setfill('0') << nowst->tm_sec
				<< ": " << mes << std::endl;
		} break;
	case LG_NO_TIME:
		{
			res << mes << endl;
		} break;
	case LG_WITH_TIME_WARNING:
		{
			time(&now);
			nowst = localtime(&now);
			res << std::setw(2) << std::setfill('0') << nowst->tm_hour
				<< ":" << std::setw(2) << std::setfill('0') << nowst->tm_min
				<< ":" << std::setw(2) << std::setfill('0') << nowst->tm_sec
				<< ": <- WARNING -> : " << mes << std::endl;
		} break;
	case LG_WITH_TIME_TITLE:
		{
			time(&now);
			nowst = localtime(&now);
			res << std::setw(2) << std::setfill('0') << nowst->tm_hour
				<< ":" << std::setw(2) << std::setfill('0') << nowst->tm_min
				<< ":" << std::setw(2) << std::setfill('0') << nowst->tm_sec
				<< ": " << std::setw(10) << std::setfill('=') <<
				"==" << mes << std::setw(10) << std::setfill('=') <<
				"==" << std::endl;
		} break;
	case LG_WITH_TIME_ERROR:
		{
			time(&now);
			nowst = localtime(&now);
			res << std::setw(2) << std::setfill('0') << nowst->tm_hour
				<< ":" << std::setw(2) << std::setfill('0') << nowst->tm_min
				<< ":" << std::setw(2) << std::setfill('0') << nowst->tm_sec
				<< ": <-= ERROR =-> : " << mes << std::endl;
		} break;
	case LG_NO_TIME_WARNING:
		{
			res << "<- WARNING -> : " << mes << std::endl;
		} break;
	case LG_NO_TIME_TITLE:
		{
			res << std::setw(10) << std::setfill('=') <<
				"==" << mes << std::setw(10) << std::setfill('=') <<
				"==" << std::endl;
		} break;
	case LG_NO_TIME_ERROR:
		{
			res << "<-= ERROR =-> : " << mes << std::endl;
		} break;
	}

	// for cross-threads io operations
	LOG_MUTEX_SECTION_LOCK;

	switch(mState)
	{
	case LG_ACTIVE:
		{
			mLogFile << res.str();
			Flush();
		}
		break;
	case LG_REDIRECTED:
		{
			*mOut << res.str();
			mOut->flush();
		}
		break;
	}

	cout << res.str();

	// unlocking section
	LOG_MUTEX_SECTION_UNLOCK;

	return mes.size();
}

int CLog::PrintStream(const nova::nstringstream & stream, LogFormat format)
{
	return PrintMessage(stream.str(), format);
}

CLog & CLog::operator << (const nova::nstring & mes)
{
	PrintMessage(mes);
	return (*this);
}

CLog & CLog::operator << (const char * mes)
{
	PrintMessage(mes);
	return (*this);
}


}
