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
#include "nova_stable_precompiled_headers.h"

#include "nova_streams.h"

namespace nova
{

size_t CDataStream::Size() const
{
	return mSize;
}

char* CDataStream::ReadASCIIZ(char * outbuf, int _max)
{
	int _count = 0;
	char c = ReadMemOfType<char>();
	while(c != 0 && (_count < _max-1))
	{
		outbuf[_count] = c;
		c = ReadMemOfType<char>();

		_count++;
	}

	outbuf[_count] = 0;
	return outbuf;
}

// ------------------------------ MemoryStream ----------------------------
CMemoryStream::CMemoryStream(const CMemoryStream & s)
{
	this->mStreamBuffer = s.mStreamBuffer;
	mWrite = s.mWrite;
	mSize = s.mSize;
	mIsOpened = s.mIsOpened;
	mPos = s.mPos;
}

CMemoryStream::CMemoryStream()
{
	
}

CMemoryStream::~CMemoryStream() {}


size_t CMemoryStream::Read (const CMemoryBuffer & dest)
{
	if(!mIsOpened)
		throw NOVA_EXP("CMemoryStream::ReadLine - stream is not opened!", BAD_OPERATION);

	if(mWrite)
		throw NOVA_EXP("CMemoryStream::ReadLine - stream opened for writing!", BAD_OPERATION);

	if(Eof())
		return 0;

	int total;

	if((mPos + dest.GetBufferSize()) > mSize)
		total = mSize - mPos;
	else
		total = dest.GetBufferSize();

	mStreamBuffer.CopyTo(dest, total, mPos);
	mPos += total;

	return total;
}

size_t CMemoryStream::Read (void *dest, const size_t count)
{
	CMemoryBuffer buf(dest, count);

	return Read(buf);
}

void CMemoryStream::Skip (long count)
{
	if(!mIsOpened)
		throw NOVA_EXP("CMemoryStream::ReadLine - stream is not opened!", BAD_OPERATION);

	if((count + mPos) > mSize)
		mPos = mSize;
	else
		mPos += count;
}

void CMemoryStream::Seek (size_t pos)
{
	if(!mIsOpened)
		throw NOVA_EXP("CMemoryStream::ReadLine - stream is not opened!", BAD_OPERATION);

	if(pos <= mSize)
		mPos = pos;
}

size_t CMemoryStream::Tell (void) const
{
	if(!mIsOpened)
		throw NOVA_EXP("CMemoryStream::ReadLine - stream is not opened!", BAD_OPERATION);

	return mPos;
}

bool CMemoryStream::Eof (void) const
{
	if(!mIsOpened)
		throw NOVA_EXP("CMemoryStream::ReadLine - stream is not opened!", BAD_OPERATION);

	if(mPos >= mSize)
		return true;

	return false;
}

void CMemoryStream::Close (void)
{
	if(!mIsOpened)
		throw NOVA_EXP("CMemoryStream::ReadLine - stream is not opened!", BAD_OPERATION);

	mPos = 0;
	mStreamBuffer.FreeBuffer();
	mIsOpened = false;
}

void CMemoryStream::Open (const size_t maxsize, bool write) 
{ 
	if(!mIsOpened)
		throw NOVA_EXP("CMemoryStream::Open - stream already opened!", BAD_OPERATION);

	mPos = 0;
	mSize = maxsize;
	mWrite = write;
	mStreamBuffer.AllocBuffer(mSize);
	mIsOpened = true; 
}

void CMemoryStream::Open (void *buf, const size_t maxsize, bool write) 
{ 
	if(mIsOpened)
		throw NOVA_EXP("CMemoryStream::ReadLine - stream is already opened!", BAD_OPERATION);

	mPos = 0;
	mSize = maxsize;
	mWrite = write;
	mStreamBuffer = CMemoryBuffer(buf, maxsize);
	mIsOpened = true; 
}

size_t CMemoryStream::ReadLine (nstring & str, const size_t count)
{
	if(!mIsOpened)
		throw NOVA_EXP("CMemoryStream::ReadLine - stream is not opened!", BAD_OPERATION);

	if(mWrite)
		throw NOVA_EXP("CMemoryStream::ReadLine - stream opened for writing", BAD_OPERATION);

	size_t cc = 0;

	register char bb;
	for( cc = 0; cc < count; cc++)
	{
		if(Eof())
			break;

		bb = *((char *)mStreamBuffer.GetBegin() + mPos);
		str.push_back(bb);

		mPos++;
	}

	return cc;
}
	
size_t CMemoryStream::ReadLine (nstring & str, const char delim)
{
	if(!mIsOpened)
		throw NOVA_EXP("CMemoryStream::ReadLine - stream is not opened!", BAD_OPERATION);

	if(mWrite)
		throw NOVA_EXP("CMemoryStream::ReadLine - stream opened for writing", BAD_OPERATION);

	size_t cc = 0;

	register char bb;
	for(cc = 0; ; cc++)
	{
		if(Eof())
			break;

		bb = *((char *)mStreamBuffer.GetBegin() + mPos);
		str.push_back(bb);

		mPos++;

		if(bb == delim)
			break;
	}

	return cc;
}

size_t CMemoryStream::Write (const CMemoryBuffer & source)
{
	if(!mIsOpened)
		throw NOVA_EXP("CMemoryStream::Write - stream is not opened!", BAD_OPERATION);

	if(!mWrite)
		throw NOVA_EXP("CMemoryStream::ReadLine - stream opened for reading", BAD_OPERATION);

	size_t length = mStreamBuffer.GetBufferSize();
	if(source.GetBufferSize() > (length - mPos))
	{
		length = (length - mPos) + source.GetBufferSize() + 0xFF;
		mStreamBuffer.ReallocBuffer(length);
	}

	void *ss = source.GetBegin();
	void *des = (nova::byte *)mStreamBuffer.GetBegin() + mPos;

	memcpy(des, ss, source.GetBufferSize());
	return source.GetBufferSize();
}

size_t CMemoryStream::Write (void *source, const size_t count)
{
	CMemoryBuffer tmp(source, count);
	return Write(tmp);
}

void CMemoryStream::Flush(void)
{


}


size_t CMemoryStream::WriteLine (const nstring & str, const size_t count)
{
	return 0;
}

size_t CMemoryStream::WriteLine (const nstring & str, const char delim)
{
	return 0;
}

}
