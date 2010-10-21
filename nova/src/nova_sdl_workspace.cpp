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
#include "nova_stable_precompiled_headers.h"

#include "nova_sdl_workspace.h"
#include "nova_render_system.h"
#include "nova_string_utils.h"

namespace nova
{

CSDLWorkspace::CSDLWorkspace(nstring & name, nInt32 priority, CGLSupport *creator) :
	mScreen(NULL), mVideo(NULL)
{
	mSupport = dynamic_cast<CSDLSupport*>(creator);
	mName = name;
	mPriority = priority;
	mLoopFunc = CrossEventHandler::SDLMainLoopFunction;
}

CSDLWorkspace::~CSDLWorkspace()
{
	LOG_MESSAGE("SDL: window render target successfully destroyed...");
}

void CSDLWorkspace::Destroy()
{
	LOG_MESSAGE("SDL: trying to destroying render window... (without call SDL_FreeSurface())");
    //SDL_FreeSurface(mScreen);
	mScreen = NULL;
	mReady = false;
	mClosed = true;
}

void CSDLWorkspace::PreUpdateAction()
{
	CRenderWindow::PreUpdateAction();
	mEventHandler->OnRender();
}

void CSDLWorkspace::PostUpdateAction()
{
	CRenderWindow::PostUpdateAction();
}

void CSDLWorkspace::SwapBuffers()
{
	// wait sync on linux ???

	SDL_GL_SwapBuffers();
}

void CSDLWorkspace::CreateRenderWindow(nova::TWindowInitialTarget &init)
{
	LOG_MESSAGE("SDL(!): trying to create render window...");

	mMetrics.d_buff = init.d_buff;
	mMetrics.fullscreen = init.fullscreen;
	mMetrics.freq = init.freq;
	mMetrics.vsync = init.vsync;
	mMetrics.FSAA = init.FSAA;
	mMetrics.DepthBuffered = init.DepthBuffered;

	nInt32 color_depth = init.color_bits;
	nInt32 alpha_bits = (init.color_bits > 16)? 8 : 0;
	nInt32 color_bits = (init.color_bits > 16)? 24 : init.color_bits;

	mMetrics.color_bits = color_bits;

// Берем видео инфу для начала
	mVideo = const_cast<SDL_VideoInfo *>(SDL_GetVideoInfo());

    if(!mVideo)
		throw NOVA_EXP((nstring("CSDLWorkspace::CreateRenderWindow - Couldn't get video information: ")+
			nstring(SDL_GetError())).c_str(), BAD_OPERATION);

	nova::nUInt32 flags = SDL_OPENGL | SDL_HWSURFACE | SDL_ANYFORMAT;
	if(mMetrics.fullscreen > 0)
	{
		mFullScreen = true;
		flags |= SDL_FULLSCREEN;
	}

	if(mMetrics.color_bits > 16)
	{
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	}

	if(alpha_bits > 0)
	{
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, alpha_bits);
	}

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	if(mMetrics.FSAA > 1)
	{
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, mMetrics.FSAA);
	}

	if(mMetrics.vsync)
		SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
	else
		SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 0);

	if(mMetrics.d_buff)
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	else
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);


// Создаем окошко, ставим разрешение и видео мод
	if((mScreen = SDL_SetVideoMode(init.width, init.height, mVideo->vfmt->BitsPerPixel, flags)) == NULL)
		throw NOVA_EXP((nstring("CSDLWorkspace::CreateRenderWindow - Couldn't set video mode: ")+
			nstring(SDL_GetError())).c_str(), BAD_OPERATION);
	LOG_MESSAGE("SDL(!): Render window successfully created!");

	{
		char driver[100];
		nstring dr(SDL_VideoDriverName(driver, 100));
		dr = "SDL(!): current driver name: " + dr;
		LOG_MESSAGE(dr);
	}

	if (!mMetrics.fullscreen)
		SDL_WM_SetCaption(mName.c_str(), 0);

	//SDL_Rect rect;
	//SDL_GetClipRect(mScreen, &rect);

	if(mMetrics.FSAA > 1)
	{
		glEnable( GL_MULTISAMPLE_ARB ); 
	}

	mMetrics.xpos = 0;
	mMetrics.ypos = 0;
	mMetrics.width = init.width;
	mMetrics.height = init.height;

	mSupport->InitGLExtentions();
#ifdef WIN_BUILD
	mSupport->InitWGLExtensions();
#endif

	mReady = true;
}

void CSDLWorkspace::SaveContents(nstring & file)
{
	if(!mReady)
		return;

	SDL_Surface * dib = NULL;
	Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

	if((dib = SDL_CreateRGBSurface(SDL_SWSURFACE, mMetrics.width, mMetrics.height, 
		mVideo->vfmt->BitsPerPixel, rmask, gmask, bmask, amask)) == NULL)
		throw NOVA_EXP((nstring("CSDLWorkspace::SaveContents - Couldn't allocate dib surface: ")+
			nstring(SDL_GetError())).c_str(), BAD_OPERATION);

	SDL_Rect dsrect, screct;
	SDL_GetClipRect(dib, &dsrect);
	screct.x =  screct.y = 0;
	screct.w = mMetrics.width;
	screct.h = mMetrics.height;

	SDL_BlitSurface(mScreen, &screct, dib, &dsrect);
	SDL_SaveBMP(dib, file.c_str());
	SDL_FreeSurface(dib);

	dib = NULL;
}

}