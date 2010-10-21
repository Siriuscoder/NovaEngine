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

#include "nova_streams.h"

namespace nova
{

enum gzFileOpt
{
	GZ_WRITE,
	GZ_WRITE_WITH_COMPRESS_1,
	GZ_WRITE_WITH_COMPRESS_2,
	GZ_WRITE_WITH_COMPRESS_3,
	GZ_WRITE_WITH_COMPRESS_4,
	GZ_WRITE_WITH_COMPRESS_5,
	GZ_WRITE_WITH_COMPRESS_6,
	GZ_WRITE_WITH_COMPRESS_7,
	GZ_WRITE_WITH_COMPRESS_8,
	GZ_WRITE_WITH_COMPRESS_9,
	GZ_READ
};

class NOVA_EXPORT CGZFileStream : public CDataStream
{
protected:

	nova::nstring mFileName;
	gzFile mGZFile;

	gzFileOpt mOpt;

public:

	CGZFileStream();

	CGZFileStream(const CGZFileStream & other);

	~CGZFileStream();

	static bool CompressData(CMemoryBuffer & dest, const CMemoryBuffer & source, nInt32 level);

	static bool UnCompressData(CMemoryBuffer & dest, const CMemoryBuffer & sourcem, nInt32 reallen);

	virtual size_t Read (const CMemoryBuffer & dest);

	virtual size_t Read (void *dest, const size_t count);

	virtual size_t ReadLine (nstring & str, const size_t count);

	virtual size_t ReadLine (nstring & str, const char delim);

	virtual size_t Write (const CMemoryBuffer & source);

	virtual size_t Write (void *source, const size_t count);

	virtual size_t WriteLine (const nstring & str, const size_t count);

	virtual size_t WriteLine (const nstring & str, const char delim);

	virtual void Skip (long count);

	virtual void Seek (size_t pos);

	virtual size_t Tell (void) const;

	virtual bool Eof (void) const;

	void Flush(void);
 
	virtual void Close (void);

	void Open (const nova::nstring & file, gzFileOpt opt);
};


}
