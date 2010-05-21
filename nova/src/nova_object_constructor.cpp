/***************************************************************************
 *   Copyright (C) 2009 by SIRIUS										   *
 *   SiriusStarNick@yandex.ru											   *
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

#include "nova_object_constructor.h"

namespace nova {

void *CObjectConstructor::operator new(size_t size)
{
	//return winnie_allocator::Alloc(size);
	nova::byte *pblock = NULL;
	return getmem<nova::byte>(pblock, size);
}

void CObjectConstructor::operator delete(void *p)
{
	//winnie_allocator::Free(p);
	freemem<nova::byte>(static_cast<nova::byte*>(p));
}

void *CObjectConstructor::operator new[](size_t size)
{
	//return winnie_allocator::Alloc(size);
	nova::byte *pblock = NULL;
	return getmem<nova::byte>(pblock, size);
}

void CObjectConstructor::operator delete[](void *p)
{
	//winnie_allocator::Free(p);
	freemems<nova::byte>(static_cast<nova::byte*>(p));
}

void *CObjectConstructor::operator new(size_t size, const std::nothrow_t & n)
{
	//return winnie_allocator::Alloc(size);
	nova::byte *pblock = NULL;
	return getmem<nova::byte>(pblock, size);
}

void CObjectConstructor::operator delete(void *p, const std::nothrow_t & n)
{
	//winnie_allocator::Free(p);
	freemem<nova::byte>(static_cast<nova::byte*>(p));
}

void *CObjectConstructor::operator new[](size_t size, const std::nothrow_t & n)
{
	//return winnie_allocator::Alloc(size);
	nova::byte *pblock = NULL;
	return getmem<nova::byte>(pblock, size);
}

void CObjectConstructor::operator delete[](void *p, const std::nothrow_t & n)
{
	//winnie_allocator::Free(p);
	freemems<nova::byte>(static_cast<nova::byte*>(p));
}

nova::ulonglong CObjectConstructor::GetMaxBlockSize(void)
{
	return std::numeric_limits<size_t>::max();
}

}