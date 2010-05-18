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

#include "nova_memory.h"

namespace nova
{

template<class T> class CSmartPtr
{
protected:
	int * mUseCount;
	T * mPtr;

	void swap(CSmartPtr<T> & other) 
	{
		std::swap(mPtr, other.mPtr);
		std::swap(mUseCount, other.mUseCount);
	}

public:

	CSmartPtr() : mUseCount(NULL), mPtr(NULL) {}


	CSmartPtr(T * ptr) : mUseCount(NULL), mPtr(NULL)
	{
		mPtr = ptr;
		mUseCount = getmem<int>(mUseCount);
		*mUseCount = 1;
	}

	CSmartPtr(const CSmartPtr & smart_ptr) : mUseCount(NULL), mPtr(NULL)
	{
		if(smart_ptr.mPtr)
		{
			mUseCount = smart_ptr.mUseCount;
			mPtr = smart_ptr.mPtr;
			if(smart_ptr.mUseCount)
				(*mUseCount)++;

		}
	}

	template<class Y>
	CSmartPtr(const CSmartPtr<Y> & smart_ptr) : mUseCount(NULL), mPtr(NULL)
	{
		if(smart_ptr.GetPtr())
		{
			mUseCount = smart_ptr.GetUseCountPtr();
			mPtr = dynamic_cast<T *>(smart_ptr.GetPtr());
			if(mUseCount)
				(*mUseCount)++;

		}
	}

	void Bind(T * ptr)
	{
		if(!mPtr && !mUseCount)
		{
			mPtr = ptr;
			mUseCount = getmem<int>(mUseCount);
			*mUseCount = 1;
		}
	}

	T * GetPtr() const
	{
		return mPtr;
	}

	int GetUseCount() const
	{
		return *mUseCount;
	}

	int * GetUseCountPtr() const
	{
		return mUseCount;
	}


	bool IsNull()
	{
		return !mPtr;
	}

	void SetNull()
	{
		mUseCount = NULL;
		mPtr = NULL;
	}

	void Free()
	{
		if(mPtr)
			delete mPtr;
		if(mUseCount)
			freemem(mUseCount);

		mPtr = NULL;
		mUseCount = NULL;
	}

	void Release()
	{
		if(mUseCount && mPtr)
		{
			(*mUseCount)--;

			if((*mUseCount) <= 0)
				Free();
		}
	}

	inline T & operator*() const 
	{
		return *mPtr;
	}

	inline T * operator->() const 
	{ 
		return mPtr;
	}

	CSmartPtr & operator=(const CSmartPtr & other) 
	{
		if (mPtr == other.mPtr)
			return *this;
		// Swap current data into a local copy
		// this ensures we deal with rhs and this being dependent
		CSmartPtr<T> tmp(other);
		this->swap(tmp);
		return *this;
	}

	template<class Y>
	CSmartPtr & operator=(const CSmartPtr<Y> & other) 
	{
		if (mPtr == other.GetPtr())
			return *this;
		// Swap current data into a local copy
		// this ensures we deal with rhs and this being dependent
		CSmartPtr<T> tmp(other);
		this->swap(tmp);
		return *this;
	}

	virtual ~CSmartPtr()
	{
		Release();
	}
};


	template<class T, class U> inline bool operator==(CSmartPtr<T> const& a, CSmartPtr<U> const& b)
	{
		return a.GetPtr() == b.GetPtr();
	}

	template<class T, class U> inline bool operator!=(CSmartPtr<T> const& a, CSmartPtr<U> const& b)
	{
		return a.GetPtr() != b.GetPtr();
	}

	template<class T, class U> inline bool operator<(CSmartPtr<T> const& a, CSmartPtr<U> const& b)
	{
		return std::less<const void*>()(a.GetPtr(), b.GetPtr());
	}

}