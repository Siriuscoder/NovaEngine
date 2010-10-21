/***************************************************************************
 *   Copyright (C) 2008 by SIRIUS										   *
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

#include "nova_threads.h"
#include "nova_string_utils.h"

namespace nova {

pthread_mutex_t ALLOCATOR_MUTEX_SECTION = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t LOG_MUTEX_SECTION = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t RESOURCE_MUTEX_SECTION = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t OBJECT_MUTEX_SECTION = PTHREAD_MUTEX_INITIALIZER;

struct _in_data : public CObjectConstructor
{
	CThread * pth;
	void * buffer;
};

void * __in__thread(void *data)
{
	if(data)
	{
		// отключаем кансел
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

		_in_data * pdata = static_cast<_in_data *>(data);

		_in_data threadset;
		memcpy(&threadset, pdata, sizeof(_in_data));
		// Освобождаем поточную структуру
		delete pdata;

		if(threadset.pth->threadfunc)
		{
			pfThread func = threadset.pth->threadfunc;

			try 
			{	
				threadset.pth->LockStateMutex();
				threadset.pth->mState = TH_PLAY;
				threadset.pth->UnLockStateMutex();

				// Включаем кансел
				pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

				// Запускаем поточную функцию
				(threadset.pth->*func)(threadset.buffer);
			} 
			catch(nova::NovaExp & exp)
			{
				exp.FixError(threadset.pth);
			}

			// переводим состояние в режим завершено ))
			threadset.pth->LockStateMutex();
			threadset.pth->mState = TH_KILLED;
			threadset.pth->UnLockStateMutex();
		}
	}
	// отсоединяем поток
	pthread_detach(pthread_self());

	return NULL;
}

void CThread::BeginInit(void)
{
	threadfunc = NULL;
	mState = TH_KILLED;
	pthread_mutex_init(&mStateMutex, NULL);
}

CThread::CThread() : CBase("CThread")
{
	BeginInit();
}

CThread::CThread(char * _name) : CBase(_name)
{
	BeginInit();
}

nInt32 CThread::Exec(void *_buf) throw(nova::NovaExp)
{
//	pth = this;
//	if(!(pthread = SDL_CreateThread(__in__thread, _buf)))
//		throw NOVA_EXP("Can not create thread!", BAD_OPERATION);
//	state = TH_PLAY;
	pthread_attr_t attr;
	nInt32 rc = 0;
	_in_data * threadset = NULL;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	threadset = new _in_data;
	threadset->pth = this;
	threadset->buffer = _buf;

	rc = pthread_create(&pthread, &attr, __in__thread, (void *)threadset);
	pthread_attr_destroy(&attr);

	if(rc)
	{
		nstring codemes("CThread::Exec - do not posible create thread, error code: ");
		codemes.append(CStringUtils::IntToString(rc));

		throw NOVA_EXP(codemes.c_str(), BAD_OPERATION);
	}

	return rc;
}

CThread::~CThread()
{
	Destroy();
	pthread_mutex_destroy(&mStateMutex);
}

nInt32 CThread::Destroy()
{
//	if(pthread)
//		SDL_KillThread(pthread);
	if(mState == TH_PLAY)
		pthread_cancel(pthread);

	LockStateMutex();
	mState = TH_KILLED;
	UnLockStateMutex();
	return 0;
}


void CThread::WaitThread()
{
//	if(pthread)
//		SDL_WaitThread(pthread, NULL);
	void * pcancelstate;
	if(mState == TH_PLAY)
		pthread_join(pthread, &pcancelstate);
}

void CThread::LockStateMutex(void)
{
	pthread_mutex_lock(&mStateMutex);
}

void CThread::UnLockStateMutex(void)
{
	pthread_mutex_unlock(&mStateMutex);
}

ThState CThread::GetThreadState(void)
{
	ThState state = TH_KILLED;

	LockStateMutex();
	state = mState;
	UnLockStateMutex();

	return state;
}

}