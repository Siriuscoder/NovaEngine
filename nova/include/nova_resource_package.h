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
// Success doesn't come to you…you go to it.
#pragma once 

#include "nova_error.h"
#include "nova_gzstreams.h"
#include "nova_fstream.h"

#define NPK_VERSION(major,minor)	(nova::nUInt32)(((nova::nUInt16)major & 0xffff) | ((((nova::nUInt32)minor) & 0xffff) << 16))

#define NPK_VERSION_1_0				NPK_VERSION(1, 0)
#define NPK_VERSION_2_0				NPK_VERSION(2, 0)

namespace nova
{

class NOVA_EXPORT CFilesPackage : public CBase
{
private:

	nstring mPackageName;
	CDataStream *mFile;
	bool mIsOpened;
	bool mWritePackage;
	bool mSelfOpen;
	nova::nUInt32 mVersion;

#pragma pack(push, 1)
	typedef struct PackageHeader
	{
		nova::nUInt32 m_signature;
		nova::nUInt32 version;
		nova::nUInt32 v_signature;
		char package_name[50];
	} TPackageHeader;

	typedef struct FileHeaderV10
	{
		nova::nUInt32 number;
		size_t size;
		size_t pos;
		char file_name[150];
		char ext[10];
		char resource_group[50];
	} TFileHeaderV10;

#ifdef NPK_VERSION_2_0
	typedef struct FileHeaderV20
	{
		nova::nUInt32 number;
		size_t size;
		size_t pos;
		char file_name[150];
		char ext[10];
		char resource_group[50];
		char filePath[255];
	} TFileHeaderV20;
#endif

#pragma pack(pop)

protected:

	stl<nstring, TFileHeaderV20>::map mPackageMap;
	TPackageHeader mPackageHeader;

	bool InFileList(const nstring & name);

public:

	CFilesPackage();

	~CFilesPackage();

	void OpenPackage(const nstring & pkg, bool write = false);

	void OpenPackage(CDataStream *pkgstream, bool write = false);

	void ClosePackage(void);

	nova::stl<nstring>::vector GetFileList(void) const;

	nova::nstring GetFileExt(const nstring & name) const ;

	nova::nstring GetFilePath(const nstring & name) const ;

	CMemoryBuffer GetFile(const nstring & name) const ;

	bool IsOpened(void) const;

	void PutFile(const CMemoryBuffer &buf , const nstring & name, const nstring & ext, const nstring &grs, const nstring &inPath); 

	nstring GetPackageName(void) const;

};


}