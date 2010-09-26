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
#pragma once
// Базовый класс потоков, все потоки внутри движка 
// реализуются наследованием от этого класса
#include "nova_error.h"

/// \file nova_threads.h
/// \brief Базовый реализация потоков
///
/// Файл содержит описание класса CThread
/// который является предком всех поточных классов,
/// в нем реализованн базовый функционал.
/// \attention Потоки построенны на SDL threads.

namespace nova {


/// \breaf Состояния нити
///
/// Каждая нить может быть в этих 2х состояниях
/// Остановленная, или запущенна (все просто думаю)
enum ThState
{
	TH_KILLED,
	TH_PLAY	
};

#if !defined(DOXYGEN_IGNORE)
class NOVA_EXPORT CThread; 
#endif

///////// Mutex Sections ///////////////////////////////////
extern pthread_mutex_t ALLOCATOR_MUTEX_SECTION; /* for winnie allocator */
#define ALLOCATOR_MUTEX_SECTION_LOCK pthread_mutex_lock(&ALLOCATOR_MUTEX_SECTION)
#define ALLOCATOR_MUTEX_SECTION_UNLOCK pthread_mutex_unlock(&ALLOCATOR_MUTEX_SECTION)

extern pthread_mutex_t LOG_MUTEX_SECTION;
#define LOG_MUTEX_SECTION_LOCK pthread_mutex_lock(&LOG_MUTEX_SECTION)
#define LOG_MUTEX_SECTION_UNLOCK pthread_mutex_unlock(&LOG_MUTEX_SECTION)

extern pthread_mutex_t RESOURCE_MUTEX_SECTION;
#define RESOURCE_MUTEX_SECTION_LOCK pthread_mutex_lock(&RESOURCE_MUTEX_SECTION)
#define RESOURCE_MUTEX_SECTION_UNLOCK pthread_mutex_unlock(&RESOURCE_MUTEX_SECTION)

extern pthread_mutex_t OBJECT_MUTEX_SECTION;
#define OBJECT_MUTEX_SECTION_LOCK pthread_mutex_lock(&OBJECT_MUTEX_SECTION)
#define OBJECT_MUTEX_SECTION_UNLOCK pthread_mutex_unlock(&OBJECT_MUTEX_SECTION)
////////////////////////////////////////////////////////////

//Объявляем тип поточной функции
/// \brief Тип поточной функции
///
/// При наследовании от класса CThread, в пользовательском классе
/// потомке вы должны объявить эту функцию.
typedef int (THISCALL__ CThread::*pfThread)(void *);


/// \brief Основа потоков
///
/// Использовать класс можно так
/// \code 
/// class CEventPumpThread : public CThread
/// {
/// private:
///		bool state;
/// // Наша поточная функция
///		int LoopFunc(void * data); 
///
/// public:
///		CEventPumpThread();
///		~CEventPumpThread();
///
///		int StartPump();
///		void StopPump();
///
///		inline bool IsProcess()
///		{
///			return state;
///		}
/// };
///
/// CEventPumpThread::CEventPumpThread() : CThread("CEventPumpThread"), state(false)
/// {
///		// в конструкторе присваиваем адрес поточной функции к threadfunc
/// 	threadfunc = reinterpret_cast<pfThread>(&CEventPumpThread::LoopFunc);
/// }
/// \endcode
/// \attention Не забудьте присвоить адрес поточной функции!
class NOVA_EXPORT CThread : public CBase
{
private:
	ThState mState;
	pthread_t pthread;
	pthread_mutex_t mStateMutex;

	friend void * __in__thread(void *_buf);

	void LockStateMutex(void);
	void UnLockStateMutex(void);

	void BeginInit(void);

protected:

	// указатель на функцию потока.
/// \brief указатель на потоковую функцию 
	pfThread threadfunc;
	
public:
/// \brief Коструктор
	CThread();
/// \brief Коструктор
///
/// \param _name Имя наследника (необязательно)
	CThread(char * _name);
/// \brief Деструктор
	~CThread();

/// \brief Создает нить и посылает в поточную функцию указатель на буфер
/// данных _buf.
///
/// \param _buf буффер данных
	int Exec(void *_buf) throw(nova::NovaExp);

/// \brief Останов нити.
	int Destroy();

/// \brief Ждать нить.
///
/// Заставляет вызывающий поток ждать до тех пор, пока текущая нить не завершится.
	void WaitThread();

/// \brief Берем состояние нити
///
/// \see ThState
	ThState GetThreadState(void);

};

}
