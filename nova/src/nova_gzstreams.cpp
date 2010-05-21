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

#include "nova_gzstreams.h"
#include "nova_string_utils.h"

namespace nova
{

CGZFileStream::CGZFileStream() : mGZFile(NULL), mOpt(GZ_READ)
{

}

CGZFileStream::~CGZFileStream()
{
//	if(IsOpened())
//		Close();
}

CGZFileStream::CGZFileStream(const CGZFileStream & other)
{
	mFileName = other.mFileName;
	mOpt = other.mOpt;
	mGZFile = other.mGZFile;
	mWrite = other.mWrite;
	mIsOpened = other.mIsOpened;
	mSize = other.mSize;
}
	
void CGZFileStream::Open(const nova::nstring & file, gzFileOpt opt)
{
	if(mIsOpened)
		throw NOVA_EXP("CGZFileStream::Open() - gz stream already opened!", BAD_OPERATION);

	nstring OpenString, Suffix;
	mFileName = file;
	mOpt = opt;

	if(mOpt != GZ_READ)
	{
		if(mOpt >= GZ_WRITE_WITH_COMPRESS_1 && 
			mOpt <= GZ_WRITE_WITH_COMPRESS_9)
		{
			Suffix = CStringUtils::IntToString(mOpt);
		}
		else if(mOpt == GZ_WRITE)
		{
			Suffix = CStringUtils::IntToString(5);
		}
		
		OpenString.append("wb");
		OpenString += Suffix;
		mWrite = true;
	}
	else
	{
		OpenString.append("rb");
		mWrite = false;
	}

	mGZFile = gzopen(mFileName.c_str(), OpenString.c_str());
	if(!mGZFile)
		throw NOVA_EXP("CGZFileStream::OpenGZFile - can not open gz file!", BAD_OPERATION);

	mIsOpened = true;
}


void CGZFileStream::Close()
{
	if(mIsOpened && mGZFile)
	{
		gzclose(mGZFile);
		mIsOpened = false;
	}
	else
		throw NOVA_EXP("CGZFileStream::CloseGZFile - file is not opened!", BAD_OPERATION);
}

size_t CGZFileStream::Tell() const
{
	if(mIsOpened && mGZFile)
	{
//		if(mOpt != GZ_READ)
//			NOVA_EXP("CGZFileStream::GetReadPos - file opened for write!", BAD_OPERATION);

		return gztell(mGZFile);
	}
	
	throw NOVA_EXP("CGZFileStream::GetReadPos - file is not opened!", BAD_OPERATION);
	return 0;
}

void CGZFileStream::Seek(nova::uint pos)
{
	if(mIsOpened && mGZFile)
	{
//		if(mOpt != GZ_READ)
//			NOVA_EXP("CGZFileStream::SetReadPos - file opened for write!", BAD_OPERATION);

		gzseek(mGZFile, pos, 0x0);
	}
	else
		throw NOVA_EXP("CGZFileStream::SetReadPos - file is not opened!", BAD_OPERATION);
}

bool CGZFileStream::Eof() const
{
	if(mIsOpened && mGZFile)
	{
		return (gzeof(mGZFile) == 1) ? true : false;
	}

	throw NOVA_EXP("CGZFileStream::IsEOF - file is not opened!", BAD_OPERATION);
	return true;
}

void CGZFileStream::Skip (long count)
{
	size_t pos = Tell();
	Seek(pos + count);
}

bool CGZFileStream::CompressData(CMemoryBuffer & dest, const CMemoryBuffer & source, int level)
{
	nova::uint len = source.GetBufferSize() + 
		static_cast<nova::uint>(source.GetBufferSize() * 0.01) + 0xC;

	CMemoryBuffer buffer;
	buffer.AllocBuffer(len);

	nova::ulong destlen = 0;
	int res = compress2((nova::byte *)buffer.GetBegin(), (uLongf *)&destlen, 
		(nova::byte *)source.GetBegin(), source.GetBufferSize(), level);

	if(res != Z_OK)
		NOVA_EXP("CGZFileStream::CompressData - error compressing data stream!", BAD_OPERATION);

	dest.AllocBuffer(destlen);
	buffer.CopyTo(dest, buffer.GetBufferSize(), 0);
	buffer.FreeBuffer();

	return true;
}


bool CGZFileStream::UnCompressData(CMemoryBuffer & dest, const CMemoryBuffer & source, int reallen)
{
	nova::ulong destlen = 0;
	dest.AllocBuffer(reallen);

	int res = uncompress((nova::byte *)dest.GetBegin(), (uLongf *)&destlen, 
		(nova::byte *)source.GetBegin(), source.GetBufferSize());

	if(res != Z_OK)
		NOVA_EXP("CGZFileStream::UnCompressData - error uncompressing data stream!", BAD_OPERATION);

	if(destlen != reallen)
		NOVA_EXP("CGZFileStream::UnCompressData - destlen != reallen - this very very interesting!", BAD_OPERATION);

	return true;
}


size_t CGZFileStream::Read (const CMemoryBuffer & dest)
{
	if(!mIsOpened)
		throw NOVA_EXP("CGZFileStream::ReadGZFile - file is not opened!", BAD_OPERATION);

	if(mOpt != GZ_READ)
		NOVA_EXP("CGZFileStream::Read - file opened for write!", BAD_OPERATION);

	int block = 0xFFFF;
	nova::uint len = dest.GetBufferSize();
	nova::uint slen = 0;

	while(slen < len)
	{
		nova::uint ilen = 0;
		if((slen + block) > len)
			ilen = len - slen;
		else
			ilen = block;

		void * pchunk = ((nova::byte *)dest.GetBegin()) + slen;
		slen += gzread(mGZFile, pchunk, ilen);
	}

	return slen;
}

size_t CGZFileStream::Read(void *dest, const size_t count)
{
	CMemoryBuffer stream(dest, count);
	return Read(stream);
}


size_t CGZFileStream::Write(const CMemoryBuffer & source)
{
	if(!mIsOpened)
		throw NOVA_EXP("CGZFileStream::WriteGZFile - file is not opened!", BAD_OPERATION);

	if(mOpt == GZ_READ)
		NOVA_EXP("CGZFileStream::Write - file opened for read!", BAD_OPERATION);

	int block = 16384;
	nova::uint len = source.GetBufferSize();
	nova::uint slen = 0;

	while(slen < len)
	{
		nova::uint ilen = 0;
		if((slen + block) > len)
			ilen = len - slen;
		else
			ilen = block;

		void * pchunk = ((nova::byte *)source.GetBegin()) + slen;
		slen += gzwrite(mGZFile, pchunk, ilen);
	}

	return slen;
}

size_t CGZFileStream::Write(void *source, const size_t count)
{
	CMemoryBuffer stream(source, count);
	return Write(stream);
}


void CGZFileStream::Flush()
{
	if(!mIsOpened)
		throw NOVA_EXP("CGZFileStream::Flush() - file is not opened!", BAD_OPERATION);

	gzflush(mGZFile, 0);
}

size_t CGZFileStream::ReadLine (nstring & str, const size_t count)
{
	if(!mIsOpened || !mGZFile)
		throw NOVA_EXP("CGZFileStream::ReadLine - gzfile is not opened!", BAD_OPERATION);

	if(mOpt != GZ_READ)
		NOVA_EXP("CGZFileStream::ReadLine - file opened for read!", BAD_OPERATION);

	char *buf = NULL;
	buf = getmem(buf, count);
	buf = gzgets(mGZFile, buf, count);

	str.append(buf);
	freemem(buf);

	return strlen(buf);
}


size_t CGZFileStream::ReadLine (nstring & str, const char delim)
{
	if(!mIsOpened || !mGZFile)
		throw NOVA_EXP("CGZFileStream::ReadLine - gzfile is not opened!", BAD_OPERATION);

	if(mOpt != GZ_READ)
		NOVA_EXP("CGZFileStream::ReadLine - file opened for read!", BAD_OPERATION);

	size_t cc = 0;

	register char bb;
	for(cc = 0; ; cc++)
	{
		if(Eof())
			break;

		bb = gzgetc(mGZFile);
		str.push_back(bb);

		cc++;
		if(bb == delim)
			break;
	}

	return cc;
}

size_t CGZFileStream::WriteLine (const nstring & str, const size_t count)
{
	if(!mIsOpened || !mGZFile)
		throw NOVA_EXP("CGZFileStream::WriteLine - gzfile is not opened!", BAD_OPERATION);

	if(mOpt == GZ_READ)
		NOVA_EXP("CGZFileStream::WriteLine - file opened for read!", BAD_OPERATION);

	size_t cc = 0, ci = 0;
	const char *pstr = str.c_str();
	if(count > 0)
		cc = count;
	else
		cc = str.length();

	for(ci = 0; ci < cc; ci++)
		gzputc(mGZFile, pstr[ci]);

	return ci;
}

size_t CGZFileStream::WriteLine (const nstring & str, const char delim)
{
	if(!mIsOpened || !mGZFile)
		throw NOVA_EXP("CGZFileStream::WriteLine - gzfile is not opened!", BAD_OPERATION);

	if(mOpt == GZ_READ)
		NOVA_EXP("CGZFileStream::WriteLine - file opened for read!", BAD_OPERATION);

	size_t cc = 0, ci = 0;
	const char *pstr = str.c_str();
	cc = str.length();

	for(ci = 0; ci < cc; ci++)
	{
		if(pstr[ci] == delim)
			break;

		gzputc(mGZFile, pstr[ci]);
	}


	return ci;
}

/*

void CGZFileStream::Puts(nstring & buf)
{
	if(!mOpen)
		throw NOVA_EXP("CGZFileStream::Puts - file is not opened!", BAD_OPERATION);

	gzputs(mGZFile, buf.c_str());
}

*/



}