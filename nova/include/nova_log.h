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
#pragma once

#include "nova_base.h"

namespace nova
{

#define LOG_MESSAGE(x) if(CLog::GetInstancePtr()) CLog::GetInstance().PrintMessage(x)

class NOVA_EXPORT CLog : public CBase
{
public:

	enum LogState
	{
		LG_ACTIVE,
		LG_SLEEP,
		LG_CLOSED,
		LG_REDIRECTED
	};

	enum LogFormat
	{
		LG_WITH_TIME,
		LG_NO_TIME,
		LG_WITH_TIME_WARNING,
		LG_WITH_TIME_TITLE,
		LG_WITH_TIME_ERROR,
		LG_NO_TIME_WARNING,
		LG_NO_TIME_TITLE,
		LG_NO_TIME_ERROR
	};

private:

	std::ofstream mLogFile;
	std::ostream * mOut;
	LogState mState;

	static CLog * mInstance;

public:

	CLog(nstring & file);
	~CLog();

	int PrintMessage(const nova::nstring & mes, LogFormat format = LG_WITH_TIME);
	int PrintMessage(const char * mes, LogFormat format = LG_WITH_TIME);
	int PrintStream(const nova::nstringstream & stream, LogFormat format = LG_WITH_TIME);

	void Flush();
	void Close();
	void SetState(LogState state);

	void ReDirectTo(std::ostream & out);

	inline static CLog & GetInstance()
	{
		return (*mInstance);
	}

	inline static CLog * GetInstancePtr()
	{
		return mInstance;
	}

	CLog & operator << (const nova::nstring & mes);
	CLog & operator << (const char * mes);
};


}
