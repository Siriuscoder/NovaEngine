/***************************************************************************
 *   Copyright (C) 2009 by Sirius										   *
 *   siriusnick@gmail.com												   *
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

#include "nova_buffers.h"

namespace nova
{

class NOVA_EXPORT CDataStream : public CBase
{
protected:

	nova::nUInt32 mSize;
	bool mIsOpened;
	bool mWrite;

public:

	CDataStream() : mSize(0), CBase("CDataStream"), mIsOpened(false), mWrite(false) {}

	~CDataStream() {}

	template<typename usertype>
	inline usertype ReadMemOfType()
	{
		usertype nb;
		size_t size = sizeof(usertype);

		Read((void *)&nb, size);

		return nb;
	}

	template<typename usertype>
	inline usertype ReadMemOfTypeInSize(size_t m_size)
	{
		usertype nb;
		Read((void *)&nb, m_size);

		return nb;
	}

	template<typename usertype>
	inline void WriteMemOfType(usertype &nb)
	{
		size_t size = sizeof(usertype);
		Write((void *)&nb, size);
	}

	template<typename usertype>
	inline void WriteMemOfTypeInSize(usertype &nb, size_t m_size)
	{
		Write((void *)&nb, m_size);
	}

	char *ReadASCIIZ(char * outbuf, nInt32 _max);

	virtual size_t Read (const CMemoryBuffer & dest) = 0;

	virtual size_t Read (void *dest, const size_t count) = 0;

	virtual size_t Write (const CMemoryBuffer & source) = 0;

	virtual size_t Write (void *source, const size_t count) = 0;

	virtual size_t ReadLine (nstring & str, const size_t count) = 0;

	virtual size_t ReadLine (nstring & str, const char delim) = 0;

	virtual size_t WriteLine (const nstring & str, const size_t count) = 0;

	virtual size_t WriteLine (const nstring & str, const char delim) = 0;

	virtual void Skip (long count) = 0;

	virtual void Seek (size_t pos) = 0;

	virtual size_t Tell (void) const = 0;

	virtual bool Eof (void) const = 0;

	virtual void Flush(void) = 0;

	size_t Size (void) const;
 
	virtual void Close (void) = 0;

	bool IsOpened() { return mIsOpened; }

};

class NOVA_EXPORT CMemoryStream : public CDataStream
{
protected:

	CMemoryBuffer mStreamBuffer;
	size_t mPos;

public:

	CMemoryStream();

	CMemoryStream(const CMemoryStream & s);

	~CMemoryStream();

	size_t Read (const CMemoryBuffer & dest);

	size_t Read (void *dest, const size_t count);

	size_t ReadLine (nstring & str, const size_t count);

	size_t ReadLine (nstring & str, const char delim);

	size_t Write (const CMemoryBuffer & source);

	size_t Write (void *source, const size_t count);

	size_t WriteLine (const nstring & str, const size_t count);

	size_t WriteLine (const nstring & str, const char delim);

	void Skip (long count);

	void Seek (size_t pos);

	size_t Tell (void) const;

	bool Eof (void) const;

	void Flush(void);
 
	void Close (void);

	void Open (const size_t maxsize, bool write);

	void Open (void *buf, const size_t maxsize, bool write);
};


}
