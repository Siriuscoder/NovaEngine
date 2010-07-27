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

#include "nova_system_allocator.h"
#include "nova_error.h"

namespace nova
{


void *CSysAlloc::AllocateMemory(const size_t size)
{
	void *p = malloc(size);

	if(!p)
		throw NOVA_EXP("malloc: Can not allocate memory, return NULL pointer..", MEM_ERROR);

	return p;
}

void CSysAlloc::FreeMemory(void *pmem)
{
	free(pmem);
}

void *CSysAlloc::Reallocate(void *pmem, const size_t size)
{
	void *p = realloc(pmem, size);

	if(!p)
		throw NOVA_EXP("realloc: Can not reallocate memory, return NULL pointer..", MEM_ERROR);

	return p;
}


}