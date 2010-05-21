/***************************************************************************
 *   Copyright (C) 2007 by SIRIUS										   *
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

#include "nova_engine.h"
#include "nova_string_utils.h"

namespace nova
{

template<> CNovaEngine * CSingelton<CNovaEngine>::SingeltonObject = NULL;
#ifdef NOVA_MAKING_DLL
HINSTANCE ghInstance = NULL;
#endif

CNovaEngine::CNovaEngine() : CBase("CNovaEngine")
{
	nstring str(SYSTEM_LOG_FILE);
	mSystemLog = new CLog(str);

	nstring mes;
	mes.append("Starting... Engine kernel instance is 0x");
	mes += CStringUtils::IntTo16xString((int)this);

	mSystemLog->PrintMessage(mes, CLog::LG_WITH_TIME_TITLE);

	mes.clear();
	mes.append("Hello! Engine version: ");
	mes += GetEngineVersion();
	mes += " (c) sirius 2009\n\n";

	LOG_MESSAGE(mes);

	mFontManager = NULL;
	mTextureManager = NULL;
	mImageManager = NULL;
	mDevILCodec = NULL;
	mScene = NULL;
	mRenderer = NULL;
}

CNovaEngine::~CNovaEngine()
{
	Release();

	LOG_MESSAGE("Engine kernel instance destroyed..\n\n");
	LOG_MESSAGE("Thank you for using NovaEngine, good bye ))");

	delete mSystemLog;
}

void CNovaEngine::Release(void)
{
	mRenderer->ShutDown();
//	if(RenderManager)
//		delete RenderManager;
	if(mFontManager)
		delete mFontManager;
//	if(ConsoleManager)
//		delete ConsoleManager;
	if(mTextureManager)
		delete mTextureManager;
	if(mImageManager)
		delete mImageManager;
//	if(ResourceManager)
//		delete ResourceManager;
#ifdef USING_DEVIL
	if(mDevILCodec)
		delete mDevILCodec;
#endif
	if(mRenderer)
		delete mRenderer;
	if(mScene)
		delete mScene;

	mFontManager = NULL;
	mTextureManager = NULL;
	mImageManager = NULL;
	mDevILCodec = NULL;
	mScene = NULL;
	mRenderer = NULL;

	ResetStats();
}

int CNovaEngine::Init(StartInit flag)
{
	//RenderManager->SetGraphicManager(new CGraphicManager(initst.draw_sys));
	//ConsoleManager = new CConsoleManager();
	mFontManager = new CFontManager();
	//ResourceManager = new CResourceManager();
	mTextureManager = new CTextureManager();

	mImageManager = new CImageManager();

	mDevILCodec = new CDevILCodec();
	mDevILCodec->Initialize();

	mRenderer = new CRenderSystem();

	mScene = new CScene();

	ResetStats();

	if(flag == ST_INIT_AUTOMATICALY)
	{
		mRenderer->StartUp(SF_START_AUTO_XML_CONFIG);
	}

	return EXIT_OK;
}

void CNovaEngine::StartRender()
{
	mRenderLoop = true;
	ResetStats();

	while(mRenderLoop)
	{
		mStats.rendered_triangles = 0;
		mStats.rendered_batches = 0;
		(mRenderer->MainLoopPump());

		if(mRenderer->BuildAllRenderTargets() < 0)
			break;
		UpdateStatistic();
	}

	mRenderLoop = false;
	mStats.PrintStats();
}

void CNovaEngine::StopRender()
{
	mRenderLoop = false;
}

nstring CNovaEngine::GetEngineVersion()
{
	return nstring(PACKAGE_VERSION);
}

nstring CNovaEngine::GetPackageString()
{
	nstringstream result;
	result << PACKAGE_STRING << " Compiled on " << __DATE__ << " " << __TIME__;

	return result.str();
}

nstring CNovaEngine::GetEngineName()
{
	return nstring(PACKAGE_NAME);
}

void CNovaEngine::_trender_stats::PrintStats(void)
{
	nstringstream str;
	nstring ms(" ms");

	str << "    <<< Renderable statistic >>>" << std::endl;
	str << "Best frame time: " << best_frame_time << ms << endl;
	str << "Best rendered frames: " << best_rendered_frames << endl;
	str << "Worst frame time: " << worst_frame_time << ms << endl;
	str << "Worst rendered frames: " << worst_rendered_frames << endl;
	str << "Last frame time: " << last_frame_time << ms << endl;
	str << "Last rendered frames: " << last_rendered_frames << endl;
	str << "Average frame time: " << average_frame_time << ms << endl;
	str << "Average rendered frames: " << average_rendered_frames << endl;

	LOG_MESSAGE(str.str());
}


void CNovaEngine::ResetStats()
{
	//memset(&mStats, 0, sizeof(TRenderStats));
	mLastSecond = 0;
	mLastTime = 0;
	mFrameCount = 0;

	mStats.average_rendered_frames = 0.0f;
	mStats.average_frame_time = 0.0f;
	mStats.best_frame_time = std::numeric_limits<nova::real>::max();
	mStats.best_rendered_frames = 0.0f;
	mStats.last_rendered_frames = 0.0f;
	mStats.rendered_triangles = 0;
	mStats.worst_frame_time = 0.0f;
	mStats.worst_rendered_frames = std::numeric_limits<nova::real>::max();
	mStats.last_frame_time = 0;

	mCPUTimer.Reset();
	mFrameTimer.Reset();
}

void CNovaEngine::UpdateStatistic()
{
	mFrameCount++;
	nova::real ps = (nova::real)mFrameTimer.GetKernelMicroseconds() * 0.001f;
	nova::ulong sec = mCPUTimer.GetMilliseconds();

	mStats.best_frame_time = std::min(mStats.best_frame_time, ps);
	mStats.worst_frame_time = std::max(mStats.worst_frame_time, ps);

	if(sec > 1000)
	{
		mStats.last_rendered_frames	= (nova::real)mFrameCount * ((nova::real)sec / 1000.0f);
		mStats.last_frame_time = (nova::real)sec / (nova::real)mFrameCount;
		mFrameCount = 0;

		mStats.best_rendered_frames = std::max(mStats.best_rendered_frames, mStats.last_rendered_frames);
		mStats.worst_rendered_frames = std::min(mStats.worst_rendered_frames, mStats.last_rendered_frames);

		if(mStats.average_rendered_frames == 0)
			mStats.average_rendered_frames = mStats.last_rendered_frames;
		else
			mStats.average_rendered_frames =
			(mStats.average_rendered_frames + mStats.last_rendered_frames) / 2;

		if(mStats.average_frame_time == 0)
			mStats.average_frame_time = mStats.last_frame_time;
		else
			mStats.average_frame_time =
			(mStats.average_frame_time + mStats.last_frame_time) / 2;

		mCPUTimer.Reset();
	}

	mFrameTimer.Reset();
}

nova::real CNovaEngine::GetLastFPS() const
{
	return mStats.last_rendered_frames;
}

nova::real CNovaEngine::GetAverageFPS() const
{
	return mStats.average_rendered_frames;
}

nova::real CNovaEngine::GetBestFPS() const
{
	return mStats.best_rendered_frames;
}

nova::real CNovaEngine::GetWorstFPS() const
{
	return mStats.worst_rendered_frames;
}

void CNovaEngine::GetStats(TRenderStats & stat)
{
	memcpy(&stat, &mStats, sizeof(TRenderStats));
}

CTextureManager * CNovaEngine::GetTextureManager()
{
	return mTextureManager;
}

CImageManager * CNovaEngine::GetImageManager()
{
	return mImageManager;
}

CFontManager * CNovaEngine::GetFontManager()
{
	return mFontManager;
}

CRenderSystem * CNovaEngine::GetRenderSystem(void)
{
	return mRenderer;
}

CNovaEngine::TRenderStats & CNovaEngine::GetStatsPtr()
{
	return mStats;
}

void CNovaEngine::AttachEventHandlerToMainWindow(CEventConveyor * handler)
{
	if(mRenderer)
	{
		CRenderWindowPtr window = mRenderer->GetRootWindow();
		if(!window.IsNull())
			window->SetEventHandler(handler);
		else
			throw NOVA_EXP("CNovaEngine::AttachEventHandlerToMainWindow \
			Can not attach Event Controller.. RootWindow not created!", BAD_OPERATION);
	}
	else
		throw NOVA_EXP("CNovaEngine::AttachEventHandlerToMainWindow \
			Can not attach Event Controller.. Render system not created!", BAD_OPERATION);
}

void CNovaEngine::MakeRenderWindow(void)
{
	if(mRenderer)
	{
		mRenderer->MakeRenderWindowTarget();
	}
	else
		throw NOVA_EXP("CNovaEngine::MakeRenderWindow \
			Can not make render target.. Render system not created!", BAD_OPERATION);
}


//-------------------------------------------------------------
}

#ifdef NOVA_MAKING_DLL

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			nova::ghInstance = hinstDLL;
		break;
	}
	return TRUE;
}

#endif
