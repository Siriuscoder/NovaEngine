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

#include "nova_win32_workspace.h"
#include "nova_resource.h"
#include "nova_render_system.h"
#include "nova_string_utils.h"

namespace nova
{

HINSTANCE CWin32Workspace::mModuleHandle = NULL;
bool CWin32Workspace::mWinClassRegistred = false;

CWin32Workspace::CWin32Workspace(nstring & name, nInt32 priority, CGLSupport * creator)
{
	mName = name;
	mPriority = priority;
	mSupport = dynamic_cast<CWGLSupport *>( creator );
	mBestPixelFormat = 0;
	mHwnd = NULL;
	mContext = NULL;
	mLoopFunc = CrossEventHandler::Win32MainLoopFunction;
	mBestFormatFinded = false;
}

void CWin32Workspace::SwitchNextContext()
{
	if(!mContext) return;

	if(mContext->IsReady())
	{
		mContext->SetCurrent();
	}
}

CWin32Workspace::~CWin32Workspace() 
{
	LOG_MESSAGE("Destroy basic window classes..");
	DestroyWinClass();

	LOG_MESSAGE("Window render target successfully destroyed");
}

GLuint CWin32Workspace::GetBestPixelFormat()
{
	return mBestPixelFormat;
}

void CWin32Workspace::PreUpdateAction()
{
	CRenderWindow::PreUpdateAction();
	//SwitchNextContext();

	//if(mMetrics.FSAA > 1)
	//	glEnable( GL_MULTISAMPLE_ARB ); 

	mEventHandler->OnRender();
}

void CWin32Workspace::PostUpdateAction()
{
	CRenderWindow::PostUpdateAction();

	//if(mMetrics.FSAA > 1)
	//	glDisable( GL_MULTISAMPLE_ARB );

/*	if(mWindowsCount > 1)
	{
		if(!mContext) return;
		if(mContext->IsReady())
		{
			if(mContext->IsCurrent())
				mContext->EndCurrent();
		}
	}
*/
}

void CWin32Workspace::CreateRenderWindow(TWindowInitialTarget & init)
{
	if(mContext)
		throw NOVA_EXP("CWin32Workspace::CreateRenderWindow - \
		Render context already created!", BAD_OPERATION);
	LOG_MESSAGE("Trying make up render window...");

	mMetrics.d_buff = init.d_buff;
	mMetrics.fullscreen = init.fullscreen;
	mMetrics.freq = init.freq;
	mMetrics.vsync = init.vsync;
	mMetrics.FSAA = init.FSAA;
	mMetrics.DepthBuffered = init.DepthBuffered;

	if(!mWinClassRegistred)
	{
		LOG_MESSAGE("Trying create basic window classes...");
		CreateWindowClass();
	}

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_SWAP_EXCHANGE;
	if(init.d_buff)
		pfd.dwFlags |= PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	if(init.color_bits > 16)
	{
		pfd.cColorBits = 24;
		mMetrics.color_bits = 24;
	}
	else
	{
		pfd.cColorBits = init.color_bits;
		mMetrics.color_bits = init.color_bits;
	}
	pfd.cAlphaBits = (init.color_bits > 16)? 8 : 0;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	mColourDepth = mMetrics.color_bits + pfd.cAlphaBits;
	mColourBits = mMetrics.color_bits;
	mIsDepthBuffered = init.DepthBuffered;

	if(mBestPixelFormat == 0)
	{
		LOG_MESSAGE("Trying create best pixel format...");
		CreateBestPixelFormat(pfd);
	}

	char szWindowClass[MAX_LOADSTRING];
	DWORD dwStyleEx = 0;
	DWORD dwStyle = WS_VISIBLE;
	LoadString(mModuleHandle, IDS_WINCLASS, szWindowClass, MAX_LOADSTRING);

	if(mMetrics.fullscreen)
	{
		//dwStyleEx |= WS_EX_TOPMOST;
		dwStyle |= WS_POPUP;
	}
	else
	{
		dwStyle |= WS_OVERLAPPED | WS_BORDER | WS_CAPTION |
						WS_SYSMENU | WS_MINIMIZEBOX;
	}

	nova::nUInt32 width, height;
	if(init.width == 0 || init.height == 0)
	{
		width = GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);
	}
	else
	{
		width = init.width;
		height = init.height;
	}
	
	mHwnd = CreateWindowEx(dwStyleEx, szWindowClass, this->mName.c_str(), 
		dwStyle, init.xpos, init.ypos, width, height, NULL, NULL,
		mModuleHandle, this);

	if(!mHwnd)
		throw NOVA_EXP("CWin32Workspace::CreateRenderWindow - \
		Can not create render window.", BAD_OPERATION);

	//SetWindowLongPtr(mHwnd, GWLP_USERDATA, (LONG_PTR)this);

	HDC hdc = GetDC(mHwnd);
	if(!hdc)
		throw NOVA_EXP("CWin32Workspace::CreateRenderWindow - \
		Can not create compatible device context!", BAD_OPERATION);

	mContext = new CWGLContext(hdc);
////////////////////////
	//mBestPixelFormat = ChoosePixelFormat(hdc, &pfd);
////////////////////////
	dynamic_cast<CWGLContext *>(mContext)->CreateRenderContext(&pfd, mBestPixelFormat);

	{
		nstring str("Render context on pixel format created: 0x");
		str.append(CStringUtils::IntTo16xString((nInt32) dynamic_cast<CWGLContext *>(mContext)->GetRenderContext()));
		LOG_MESSAGE(str);
	}

	if(mBestFormatFinded)
		glEnable( GL_MULTISAMPLE_ARB ); 

/*	if(mWindowsCount > 1)
	{
		HGLRC oldrc = wglGetCurrentContext();
		if(!wglShareLists(oldrc, dynamic_cast<CWGLContext *>(mContext)->GetRenderContext()))
			throw NOVA_EXP("CWin32Workspace::CreateRenderWindow - \
				Can not share old and new contexts.", BAD_OPERATION);
	}
*/

	mContext->SetCurrent();

	nstring swapext("WGL_EXT_swap_control");
	if(mSupport->CheckExtention(swapext))
	{
		if(mMetrics.vsync)
			wglSwapIntervalEXT(1);
		else
			wglSwapIntervalEXT(0);
	}

	RECT rc;
	// top and left represent outer window position
	GetWindowRect(mHwnd, &rc);
	mMetrics.xpos = rc.top;
	mMetrics.ypos = rc.left;
	mXPosition = rc.top;
	mYPosition = rc.left;

	GetClientRect(mHwnd, &rc);
	mWidth = rc.right;
	mHeight = rc.bottom;
	mMetrics.width = mWidth;
	mMetrics.height = mHeight;

	if(mMetrics.fullscreen)
		SetScreenSettings();

	ShowWindow(mHwnd, SW_SHOWNORMAL);  
	UpdateWindow(mHwnd);
	SetCursorPos(mMetrics.width >> 1, mMetrics.height >> 1);

	LOG_MESSAGE("Render window successfully created!");
	mReady = true;
}

void CWin32Workspace::DestroyWinClass(void)
{
	char szWindowClass[MAX_LOADSTRING];
	char szDummyClass[MAX_LOADSTRING];
	LoadString(mModuleHandle, IDS_WINCLASS, szWindowClass, MAX_LOADSTRING);
	LoadString(mModuleHandle, IDS_DUMMYCLASS, szDummyClass, MAX_LOADSTRING);

	if(!UnregisterClass(szWindowClass, mModuleHandle))
		throw NOVA_EXP("CWin32Workspace::DestroyWinClass - Can not unregister \
							render window class!", BAD_OPERATION);
	
	if(!UnregisterClass(szDummyClass, mModuleHandle))
		throw NOVA_EXP("CWin32Workspace::DestroyWinClass - Can not unregister \
							render window class!", BAD_OPERATION);

	mWinClassRegistred = false;
}

void CWin32Workspace::Destroy()
{
	if(mContext)
	{
		if(mContext->IsReady())
		{
			if(mContext->IsCurrent())
				mContext->EndCurrent();

			dynamic_cast<CWGLContext *>(mContext)->DestroyThisContext();

			if(!ReleaseDC(mHwnd, dynamic_cast<CWGLContext *>(mContext)->GetDeviceContext()))
				throw NOVA_EXP("CWin32Workspace::Destroy() - Can not release dc!", BAD_OPERATION);
		}

		delete mContext;
		mContext = NULL;
	}
	LOG_MESSAGE("Render context successfully destroyed!");

	DestroyWindow(mHwnd);
	mHwnd = NULL;

	if(mMetrics.fullscreen)
		ChangeDisplaySettings(NULL,0);							// Switch Back To Desktop Resolution
	ShowCursor(TRUE);  

	mReady = false;
	mClosed = true;
}

void CWin32Workspace::SwapBuffers()
{
	if(mContext)
	{
		glFinish();
		::SwapBuffers(dynamic_cast<CWGLContext *>(mContext)->GetDeviceContext());
	}
}

void CWin32Workspace::CreateWindowClass(void)
{
	char szWindowClass[MAX_LOADSTRING];
	char szDummyClass[MAX_LOADSTRING];
	WNDCLASSEX mWinClass;
	WNDCLASSEX mDummyClass;

	if(!mModuleHandle)
	{
		mModuleHandle = GetModuleHandle("Nova.dll");
		if(!mModuleHandle)
			throw NOVA_EXP("CWin32Workspace::CreateWindowClass() - Can not get module instance!", BAD_OPERATION);
	}

	LoadString(mModuleHandle, IDS_WINCLASS, szWindowClass, MAX_LOADSTRING);
	LoadString(mModuleHandle, IDS_DUMMYCLASS, szDummyClass, MAX_LOADSTRING);

	if(CWin32Workspace::mWinClassRegistred)
		return ;

	memset(&mWinClass, 0, sizeof(WNDCLASSEX));

	mWinClass.cbSize = sizeof(WNDCLASSEX);
	mWinClass.style = CS_VREDRAW|CS_HREDRAW|CS_OWNDC|CS_DBLCLKS;
	mWinClass.lpfnWndProc = CrossEventHandler::WinProcFunction;
	mWinClass.cbClsExtra = 0;
	mWinClass.cbWndExtra = 0;
	mWinClass.hInstance = mModuleHandle;
	mWinClass.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	mWinClass.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	mWinClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	mWinClass.lpszMenuName = NULL;
	mWinClass.lpszClassName = szWindowClass;
	mWinClass.hIconSm = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	//outlog << "winclass created! Address: " << &winclass << endl << endl;
	
	if (!RegisterClassEx(&mWinClass))
		throw NOVA_EXP("CWin32Workspace::CreateWindowClass() - \
		Can not register render window class!", BAD_OPERATION);

	memset(&mDummyClass, 0, sizeof(WNDCLASSEX));

	mDummyClass.cbSize = sizeof(WNDCLASSEX);
	mDummyClass.style = CS_OWNDC;
	mDummyClass.lpfnWndProc = DefWindowProc;//NULL;//CrossEventHandler::WinProc;
	mDummyClass.cbClsExtra = 0;
	mDummyClass.cbWndExtra = 0;
	mDummyClass.hInstance = mModuleHandle;
	mDummyClass.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	mDummyClass.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	mDummyClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	mDummyClass.lpszMenuName = NULL;
	mDummyClass.lpszClassName = szDummyClass;
	mDummyClass.hIconSm = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	//outlog << "winclass created! Address: " << &winclass << endl << endl;
	
	if (!RegisterClassEx(&mDummyClass))
		throw NOVA_EXP("CWin32Workspace::CreateWindowClass() - \
		Can not register dummy window class!", BAD_OPERATION);

	CWin32Workspace::mWinClassRegistred = true;
}

bool CWin32Workspace::CreateBestPixelFormat(PIXELFORMATDESCRIPTOR & pfd)
{
// Creating the Dummy window (test FSAA)
	HWND DummyHwnd = NULL;
	HDC DummyHdc = NULL;
	GLuint TestFormat = 0;
	char szDummyClass[MAX_LOADSTRING];
	LoadString(mModuleHandle, IDS_DUMMYCLASS, szDummyClass, MAX_LOADSTRING);

	DummyHwnd = CreateWindowEx(NULL, szDummyClass, "Dummy", WS_OVERLAPPEDWINDOW,
		0, 0, 100, 100, NULL, NULL, mModuleHandle, NULL);
	if(!DummyHwnd)
		NOVA_EXP("CWin32Workspace::CreateBestPixelFormat - Can not create dummy window(", BAD_OPERATION);

	DummyHdc = GetDC(DummyHwnd);
	if(!DummyHdc)
		NOVA_EXP("CWin32Workspace::CreateBestPixelFormat - Can not get HDC from dummy window!", BAD_OPERATION);

	CWGLContext DummyContext(DummyHdc);

	TestFormat = ChoosePixelFormat(DummyHdc, &pfd);
	DummyContext.CreateRenderContext(&pfd, TestFormat);

	DummyContext.SetCurrent();

// Starting wgl
	if(!mSupport->GetState())
	{
		dynamic_cast<CWGLSupport *>(mSupport)->InitGLExtentions();
		dynamic_cast<CWGLSupport *>(mSupport)->InitWGLExtensions(DummyContext.GetDeviceContext());
	}

// Setting multisumple options 
	if(mMetrics.FSAA > 1)
	{
		nstring e1("WGL_ARB_multisample"), e2("WGL_ARB_pixel_format"), e3("WGL_EXT_framebuffer_sRGB");
		if(mSupport->CheckExtention(e1) && mSupport->CheckExtention(e2))
		{
			bool useHwGamma = false;
			// Use WGL to test extended caps (multisample, sRGB)
			stl<nInt32>::vector attribList;
			attribList.push_back(WGL_DRAW_TO_WINDOW_ARB); attribList.push_back(GL_TRUE);
			attribList.push_back(WGL_SUPPORT_OPENGL_ARB); attribList.push_back(GL_TRUE);
			attribList.push_back(WGL_DOUBLE_BUFFER_ARB); attribList.push_back(GL_TRUE);
			attribList.push_back(WGL_SAMPLE_BUFFERS_ARB); attribList.push_back(GL_TRUE);
			attribList.push_back(WGL_ACCELERATION_ARB); attribList.push_back(WGL_FULL_ACCELERATION_ARB);
			attribList.push_back(WGL_COLOR_BITS_ARB); attribList.push_back(pfd.cColorBits);
			attribList.push_back(WGL_ALPHA_BITS_ARB); attribList.push_back(pfd.cAlphaBits);
			attribList.push_back(WGL_DEPTH_BITS_ARB); attribList.push_back(24);
			attribList.push_back(WGL_STENCIL_BITS_ARB); attribList.push_back(8);
			attribList.push_back(WGL_SAMPLES_ARB); attribList.push_back(mMetrics.FSAA);
			if (useHwGamma && mSupport->CheckExtention(e3))
			{
				attribList.push_back(WGL_FRAMEBUFFER_SRGB_CAPABLE_EXT); attribList.push_back(GL_TRUE);
			}
			// terminator
			attribList.push_back(0);

			GLuint numformats;
			GLint format;
			nInt32 valid = wglChoosePixelFormatARB(DummyContext.GetDeviceContext(), &(attribList[0]),
				NULL, 1, &format, &numformats);

			if(valid && numformats > 0)
			{
				mBestPixelFormat = format;
				mBestFormatFinded = true;

				nstring str("Find compatible best pixel format: 0x");
				str.append(CStringUtils::IntTo16xString(mBestPixelFormat));
				LOG_MESSAGE(str);
			}
			else
			{
				mBestPixelFormat = TestFormat;
				mBestFormatFinded = false;
				
				nstring str("CWin32Workspace::CreateBestPixelFormat: Can not find compatible best pixel format,\n using simple format.. 0x");
				str.append(CStringUtils::IntTo16xString(mBestPixelFormat));
				LOG_MESSAGE(str);
			}
		}
		else
		{
			mBestPixelFormat = TestFormat;
			mBestFormatFinded = false;

			nstring str("CWin32Workspace::CreateBestPixelFormat: Can not find compatible best pixel format\n (extention WGL_ARB_multisample not supported) using simple format.. 0x");
			str.append(CStringUtils::IntTo16xString(mBestPixelFormat));
			LOG_MESSAGE(str);
		}

	}
	else
		mBestPixelFormat = TestFormat;

	DummyContext.EndCurrent();
	DummyContext.DestroyThisContext();

	ReleaseDC(DummyHwnd, DummyContext.GetDeviceContext());
	DestroyWindow(DummyHwnd);
	DummyHwnd = NULL;

	return true;
}


void CWin32Workspace::SetScreenSettings(void)
{
	DEVMODE dmScreenSettings;

	ZeroMemory(&dmScreenSettings, sizeof(dmScreenSettings));
	dmScreenSettings.dmSize = sizeof(DEVMODE);              
	dmScreenSettings.dmBitsPerPel = (mMetrics.color_bits > 16) ? 32 : mMetrics.color_bits;
	dmScreenSettings.dmPelsWidth = mMetrics.width;
	dmScreenSettings.dmPelsHeight = mMetrics.height;
	dmScreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY | DM_BITSPERPEL;  
	dmScreenSettings.dmDisplayFrequency = mMetrics.freq;
	  //outlog << "Apply Screen Settinnv_gs..." << endl << endl;
	ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);  
	mFullScreen = true;
}



}