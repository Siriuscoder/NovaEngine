/***************************************************************************
 *   Copyright (C) 2010 by Sirius										   *
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

#include "nova_fstream.h"

namespace nova
{

void CFileStream::Open(const nstring & file, bool wr, bool app)
{
/*
	mIOFile.open(file.c_str(), (wr ? fstream::out : fstream::in) | (app ? fstream::app : 0));

	if(mIOFile.good())
	{
		mWrite = wr;
		mIsOpened = true;

		if(!wr)
		{
			mIOFile.seekg(ios_base::end);
			mSize = mIOFile.tellg();
			mIOFile.seekg(ios_base::beg);
		}
		else
			mSize = 0;
	}
	else
		throw NOVA_EXP("CFileStream::Open - can not open the file!", BAD_OPERATION);
*/

	mIOFile = fopen(file.c_str(), (wr ? (app ? "ab" : "wb") : "rb"));
	if(mIOFile)
	{
		mWrite = wr;
		mIsOpened = true;

		if(!wr)
		{
			fseek(mIOFile, 0, SEEK_END);
			mSize = ftell(mIOFile);
			fseek(mIOFile, 0, SEEK_SET);
		}
		else
			mSize = 0;
	}
	else
		throw NOVA_EXP("CFileStream::Open - can not open the file!", BAD_OPERATION);
}

size_t CFileStream::Read (const CMemoryBuffer & dest)
{
	if(!mIsOpened)
		throw NOVA_EXP("CFileStream::Read - file still not opened!", BAD_OPERATION);
	if(mWrite)
		throw NOVA_EXP("CFileStream::Read - file opened for writing!", BAD_OPERATION);

	return fread(dest.GetBegin(), sizeof(nova::nByte), dest.GetBufferSize(), mIOFile);
}

size_t CFileStream::Read (void *dest, const size_t count)
{
	if(!mIsOpened)
		throw NOVA_EXP("CFileStream::Read - file still not opened!", BAD_OPERATION);
	if(mWrite)
		throw NOVA_EXP("CFileStream::Read - file opened for writing!", BAD_OPERATION);

	return fread(dest, sizeof(nova::nByte), count, mIOFile);
}

size_t CFileStream::Write (const CMemoryBuffer & source)
{
	if(!mIsOpened)
		throw NOVA_EXP("CFileStream::Write - file still not opened!", BAD_OPERATION);
	if(!mWrite)
		throw NOVA_EXP("CFileStream::Write - file opened for reading!", BAD_OPERATION);

	return fwrite(source.GetBegin(), sizeof(nova::nByte), source.GetBufferSize(), mIOFile);
}

size_t CFileStream::Write (void *source, const size_t count)
{
	if(!mIsOpened)
		throw NOVA_EXP("CFileStream::Write - file still not opened!", BAD_OPERATION);
	if(!mWrite)
		throw NOVA_EXP("CFileStream::Write - file opened for reading!", BAD_OPERATION);

	return fwrite(source, sizeof(nova::nByte), count, mIOFile);
}

nstring CFileStream::ReadLine (const size_t count)
{
	if(!mIsOpened)
		throw NOVA_EXP("CFileStream::ReadLine - stream is not opened!", BAD_OPERATION);

	if(mWrite)
		throw NOVA_EXP("CFileStream::ReadLine - stream opened for writing", BAD_OPERATION);

	char *input = NULL;
	nstring res;
	size_t cc = 0;
	input = getmem<char>(input, count);
	if ( fgets ( input, count, mIOFile ) != NULL )
	{
		cc = strlen(input);
		res.assign(input);
	}

	return res;
}

nstring CFileStream::ReadLine (const char delim)
{
	if(!mIsOpened)
		throw NOVA_EXP("CFileStream::ReadLine - stream is not opened!", BAD_OPERATION);

	if(mWrite)
		throw NOVA_EXP("CFileStream::ReadLine - stream opened for writing", BAD_OPERATION);

	size_t cc = 0;
	nstring res;

	register char bb = 0;
	for(; ; cc++)
	{
		if(Eof())
			break;
		bb = fgetc(mIOFile);

		if(bb == delim)
			break;
		if(bb == '\n')
			break;

		res.push_back(bb);
	}

	return res;
}

size_t CFileStream::WriteLine (const nstring & str, const size_t count)
{
	if(!mIsOpened)
		throw NOVA_EXP("CFileStream::WriteLine - stream is not opened!", BAD_OPERATION);

	if(!mWrite)
		throw NOVA_EXP("CFileStream::WriteLine - stream opened for reading", BAD_OPERATION);

	register size_t cc = 0;
	for(; cc < count; cc++)
	{
		fputc(str.c_str()[cc], mIOFile);
	}

	return cc;
}


size_t CFileStream::WriteLine (const nstring & str, const char delim)
{
	if(!mIsOpened)
		throw NOVA_EXP("CFileStream::WriteLine - stream is not opened!", BAD_OPERATION);

	if(!mWrite)
		throw NOVA_EXP("CFileStream::WriteLine - stream opened for reading", BAD_OPERATION);

	register size_t cc = 0;
	for(; ; cc++)
	{
		if(str.c_str()[cc] == delim)
			break;

		fputc(str.c_str()[cc], mIOFile);
	}

	return cc;
}

void CFileStream::Skip (long count)
{
	if(!mIsOpened)
		throw NOVA_EXP("CFileStream::Skip - stream is not opened!", BAD_OPERATION);

	size_t pos = Tell();

	pos += count;
	Seek(pos);
}

void CFileStream::Seek (size_t pos)
{
	if(!mIsOpened)
		throw NOVA_EXP("CFileStream::Seek - stream is not opened!", BAD_OPERATION);

	fseek(mIOFile, pos, SEEK_SET);
}

size_t CFileStream::Tell (void) const
{
	if(!mIsOpened)
		throw NOVA_EXP("CFileStream::Tell - stream is not opened!", BAD_OPERATION);

	return (size_t)ftell(mIOFile);
}

bool CFileStream::Eof (void) const
{
	if(!mIsOpened)
		throw NOVA_EXP("CFileStream::Eof - stream is not opened!", BAD_OPERATION);

	return feof(mIOFile) == 0 ? false : true;
}

void CFileStream::Flush(void)
{
	if(!mIsOpened)
		throw NOVA_EXP("CFileStream::Flush - stream is not opened!", BAD_OPERATION);

	fflush(mIOFile);
}

void CFileStream::Close (void)
{
	if(!mIsOpened)
		throw NOVA_EXP("CFileStream::Close - stream is not opened!", BAD_OPERATION);

	mIsOpened = false;
	mSize = 0;
	fclose(mIOFile);
}


}