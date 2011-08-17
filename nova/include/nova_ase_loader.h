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
#pragma once

#include "nova_content_loader.h"

namespace nova
{

class NOVA_EXPORT CASELoader : public CSceneContentLoaderBase
{
public:

	CASELoader() {}

	~CASELoader() {}

	typedef struct 
	{
		nova::nReal s;
		nova::nReal t;
		nova::nReal w;
	} stCoords;

	typedef struct 
	{
		stl<CMesh::TFaceABC>::vector tvIndexList;
		stl<stCoords>::vector tvMappingList;
	} exInternalAseArrays;

protected:

	void InitLoader(void);

	void CloseLoader(void);

	void LoadImpl(void);

	CMaterial::TMaterialContainer FindMatByID(nInt32 id);

private:

	std::map<nova::nstring, exInternalAseArrays> mExdenedArrays;
	nInt32 LoadAseInternal(void);

};

}