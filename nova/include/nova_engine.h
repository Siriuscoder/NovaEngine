/***************************************************************************
 *   Copyright (C) 2008 by SIRIUS										   *
 *   SiriusStarNick@yandex.ru											   *
 *	 SiriusNick@gmain.com												   *
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
/*
Изменил nova_object.h
nova_render_system.h
nova_camera.h
*/
#pragma once

/// \file nova_engine.h
/// \brief Главный заголовочный файл.
///
/// Нам нужно его подключить что бы использовать классы движка
/// \code
/// #include "nova_engine.h"
///
/// using namespace nova;
/// \endcode

#include "nova_configloader.h"
#include "nova_scene.h"

#include "nova_fonts.h"

#define DEF_PARENT					NULL

/// \namespace nova
/// \brief Глобальное пространство имен nova.
///
/// В этом пространстве находится весь проект,
/// поэтому чтобы обратится к лююому классу, функции, и тд. нужно его подключить,
/// или записать что то типа nova::**** .

#ifdef WIN_BUILD
#define SYSTEM_LOG_FILE "./Nova.log"
#else
#define SYSTEM_LOG_FILE "/tmp/nova.log"
#endif

namespace nova
{

#ifdef NOVA_MAKING_DLL
extern HINSTANCE ghInstance;
#endif

/// \struct nv_en_initst
/// \brief Сруктура инициализауии движка.
///
/// Думаю, что это еще не полный список полей
struct nv_en_initst
{
///	\brief Флаг консоли: 0 неиспользуем, > 0 используем
///
/// \attention если мы ставим 0, то менеджер консолей создаваться не будет!!
	nByte console;
/// \brief Флаг графической подложки
///
/// 2 варианта:
/// 1) Win32 значение CGraphicManager::use_win32
/// 2) SDL значение CGraphicManager::use_sdl
/// \attention Можно выбрать 2 варианта, выбор, как понимаете
/// нужно сделать в любом случае!!! или будет плохо :)
/// почему я выбрал скажем не просто SDL? Все просто, во первых
/// уж очень хотелось сделать все самому на win. во вторых дабы дать
/// выбор пользователю.. скажу, что если кодите под виндой
/// желательно выбирать именно win, хотя можно и SDL. Сам он
/// прикручен для переносимости на разные платформы. SDL реализован
/// на Linux, Windows, Windows CE, BeOS, MacOS, Mac OS X, FreeBSD,
/// NetBSD, OpenBSD, BSD/OS, Solaris, IRIX, and QNX. Во какой
/// выбор! наслаждайтесь)
	nByte draw_sys;
};

enum StartInit
{
	ST_INIT_AUTOMATICALY,
	ST_INIT_YOURSELF
};

/// \class CNovaEngine
/// \brief Основной класс движка!
///
/// Через него вы можете обратится к нужным вам конструкциям
/// и только через него!
/// \attention Даже и не пытайтесь создать 2 экземпляра
/// этого класса, он должен быть один в любом случае!
/// получите исключение!
/// \see NovaExp

class NOVA_EXPORT CNovaEngine : public CSingelton<CNovaEngine>, public CBase
{
public:

	typedef struct _trender_stats
	{
		nova::nReal best_rendered_frames;
		nova::nReal worst_rendered_frames;
		nova::nReal average_rendered_frames;
		nova::nReal last_rendered_frames;
		nova::nReal last_frame_time;
		nova::nReal best_frame_time;
		nova::nReal worst_frame_time;
		nova::nReal average_frame_time;
		nova::nUInt32 rendered_triangles;
		nova::nUInt32 rendered_batches;

		void PrintStats(void);

	} TRenderStats;

protected:

/// \brief Указатель на менеджер рендера.
	CFontManager * mFontManager;
//	CConsoleManager * ConsoleManager;
//	CResourceManager * ResourceManager;
	CTextureManager * mTextureManager;
	CImageManager * mImageManager;
	CMeshManager * mMeshManager;
	CMaterialManager *mMaterialManager;
	CRenderSystem * mRenderer;
	CScene * mScene;
	CLog * mSystemLog;


private:
	bool	mRenderLoop;

	TRenderStats mStats;
	nova::nUInt32 mLastSecond;
	nova::nUInt32 mLastTime;
	size_t 	mFrameCount;
	CCPUTimer mCPUTimer;
	CCPUTimer mFrameTimer;

	void UpdateStatistic();
	void ResetStats();

public:

/// \brief Конструктор
///
/// \param _inst Инициализирующяя структура
/// \see nv_en_initst
	CNovaEngine();

/// \brief Деструктор
	~CNovaEngine();

//	inline CConsoleManager * GetConsoleManager()
//	{
//		return ConsoleManager;
//	}

/*	inline CResourceManager * GetResourceManager()
	{
		return ResourceManager;
	}
*/

/// \brief Инициализация
///
/// Тут происходит создание Менеджера рендера, менеджера консолей
/// если мы указали использование консолей, ну и много еще чего)
	nInt32 Init(StartInit flag);

	void StartRender();

	void StopRender();

	nstring GetEngineVersion();

	nstring GetEngineName();

	nstring GetPackageString();

    nova::nReal GetLastFPS() const;

    nova::nReal GetAverageFPS() const;

    nova::nReal GetBestFPS() const;

    nova::nReal GetWorstFPS() const;

	TRenderStats & GetStatsPtr();

	void GetStats(TRenderStats & stat);

	CRenderSystem * GetRenderSystem(void);

	void Release(void);

	void AttachEventHandlerToMainWindow(CEventConveyor * handler);

	void MakeRenderWindow(void);
};

/**
	\mainpage NovaEngine Документация

	\section getting_started Введение

	Всем привет! Думаю что пора начать документировать мою нибольшую либу.
	И так, что это такое? Общими словами, я просто захотел сделать новый анрилл :)
	Nova это открытая, кросплатформенная библиотека для работы разработки игр.
	В основе ее лежит две системы Win32 и SDL, можно использовать любую, а на линуксе только SDL.
	В качестве быстрой рисовалки используем OpenGL.
	Зачем я этим начал заниматься? Собственно сама библиотека ничего нового из себя не представляет,
	да и сделать что то конкурентно способное на современном рынке я бы не смог..
	Но тут я стараюсь объеденить некоторые технологии, сделать их более гибкими, легкими в использовании
	для каждого.

	\section we_may_can Возможности

	В конечнов варианте (в идеале) мой движок будет поддерживать физику, звук, относительно быстрый вывод графики,
	скелетную анимацию, набор шейдерных спецэффектов, гибкий и легкий интерфейс для программистов.


	\section we_have Cделанно

	Базовая система(менеджер памяти, потоки, сокеты),
	Консоль, две системы шрифтов Win GDI и FreeType (Freefont),
	Создание рабочего пространства двумя впособами через Win32 и SDL,
	Графический менеджер, Менеджер рендеринга, конвейер событий
	и набор утилит для работы с матрицами.

	\section licensing Лицензия

	NovaEngine поставляется в исходном коде, куски этого кода могут быть использованы как угодно)
	Лицензия GPL, текст лицензии поставляется вместе с пакетом.

	\section autor Автор

	Sirius (SiriusStarNick@yandex.ru, siriusnick@gmail.com)
	site: http://vkontakte.ru/id2422996
	home site: 	http://siriuscoder.ucoz.ru/

	Copyright (C) Sirius 2009 (Vdov Nikita Sergeevich)
*/

/**
	\page examplespage Примеры использования

	\section code1 Код инициализации

	\subsection winsec Win инициализация

	\code
	#include "nova_engine.h"

	using namespace nova;

..................................
	CNovaEngine * nv_engine;

	nv_en_initst initst;
	POINT pos = {0, 0};
	nv_gs gs;

	initst.console = 1; //использовать консоль или нет?
	initst.draw_sys = CGraphicManager::use_win32;

	nv_engine = new CNovaEngine(&initst);

	gs.color_bits = 24;
	gs.d_buff = 1;
	gs.depth_bits = 32;
	gs.freq = 100;
	gs.fullscreen = 0;

	CWorkSpace * ws = nv_engine->GetRenderManager()->GetGraphicManager()->GetSpace();

	ws->Resolution(800, 600);
	ws->Posinion(pos);
	ws->Init(&gs);
	ws->CreateSpace();
	ws->InitOpenGL();
	\endcode

	\subsection sdlsec SDL/UNIX инициализация

	\code
	#include "nova_engine.h"

	using namespace nova;

..................................
	CNovaEngine * nv_engine;

	nv_en_initst initst;
	POINT pos = {0, 0};
	nv_gs gs;

	initst.console = 1; // или 0, использовать консоль или нет?)
	initst.draw_sys = CGraphicManager::use_sdl;

	nv_engine = new CNovaEngine(&initst);

	gs.color_bits = 24;
	gs.d_buff = 1;
	gs.depth_bits = 32;
	gs.freq = 100;
	gs.fullscreen = 0;

	CWorkSpace * ws = nv_engine->GetRenderManager()->GetGraphicManager()->GetSpace();

	ws->Resolution(800, 600);
	ws->Posinion(pos);
	ws->Init(&gs);
	ws->CreateSpace();
	ws->InitOpenGL();
	\endcode
*/

/**
\page lisencepage Лицензия

		    GNU GENERAL PUBLIC LICENSE
		       Version 2, June 1991

 Copyright (C) 1989, 1991 Free Software Foundation, Inc.
                       59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.

*/

}
