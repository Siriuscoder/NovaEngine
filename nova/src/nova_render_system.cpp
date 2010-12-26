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

#include "nova_render_system.h"
#include "nova_engine.h"
#include "nova_string_utils.h"

namespace nova
{

template<> CRenderSystem * CSingelton<CRenderSystem>::SingeltonObject = NULL;

CRenderCapabilities::CRenderCapabilities() : mMaxDrawBuffers(0),
	mVendor(GPU_UNKNOWN), CBase("CRenderCapabilities")
{
	memset(mCap, 0, sizeof(nInt32) * NUM_CATEGORYES);
}

bool CRenderCapabilities::IsSupport(nova::CapCategory cat, nInt32 cap)
{
	return ((mCap[cat] & (1 << cap)) != 0) ? true : false;
}

void CRenderCapabilities::SetCap(nova::CapCategory cat, nInt32 cap)
{
	mCap[cat] |= ( 1 << cap);
}

void CRenderCapabilities::SetGPUVendor(GPUVendor vendor)
{
	mVendor = vendor;
}

GPUVendor CRenderCapabilities::GetGPUVendor()
{
	return mVendor;
}

void CRenderCapabilities::SetMaxBuffers(nova::nUInt32 num)
{
	mMaxDrawBuffers = num;
}

nova::nUInt32 CRenderCapabilities::GetMaxBuffers()
{
	return mMaxDrawBuffers;
}
//-------------------------------------------------------------------------

CRenderSystem::CRenderSystem() : mTargetsBuilded(0), mRootWindow(NULL),
	mConfigIsReady(false), mSupport(NULL), mScreenShotTarget(NULL), CBase("CRenderSystem")
{
	mPrivateConfigFile = nstring(DEFAULT_CONFIG_PATH);
	LOG_MESSAGE("OpenGL RenderSystem created...");
}

void CRenderSystem::SetConfigPath(nstring & config)
{
	mPrivateConfigFile = config;
}

void CRenderSystem::SetVideoSettings(RenderSysType type, TWindowInitialTarget & config)
{
	mType = type;
	memcpy(&mVideoSettings, &config, sizeof(TWindowInitialTarget));
	mConfigIsReady = true;
}

void CRenderSystem::InitializeFromConfigFile()
{
#ifdef USE_XML
	if(mFlags == SF_START_AUTO_XML_CONFIG)
	{
		xmlDocPtr doc;
		doc = xmlParseFile(mPrivateConfigFile.c_str());

		if(doc == NULL)
			throw NOVA_EXP("CRenderSystem::InitializeFromConfigFile - can not load xml tree.", BAD_OPERATION);

		xmlNodePtr cur = xmlDocGetRootElement(doc);
		if(cur == NULL)
		{
			xmlFreeDoc(doc);
			throw NOVA_EXP("CRenderSystem::InitializeFromConfigFile - can not get xml root element.", BAD_OPERATION);
		}

		const xmlChar * caption = cur->name;
		if(xmlStrcmp(caption, (xmlChar *)"Configuration"))
		{
			xmlFreeDoc(doc);
			throw NOVA_EXP("CRenderSystem::InitializeFromConfigFile - \
				Config file have wrong format!", BAD_OPERATION);
		}

		cur = cur->children;

		bool ren = false, vid = false;

		while(cur != NULL)
		{
			if(xmlIsBlankNode(cur))
			{
				cur = cur->next;
				continue;
			}

			if(!xmlStrcmp(cur->name, (xmlChar *) "RenderSubSystem"))
			{
				ren = true;
				xmlChar * temp = xmlGetProp(cur, (xmlChar *) "SubSystem");
				if(!xmlStrcmp(temp, (xmlChar *) "WIN32"))
					mType = RS_WIN32;
				else if(!xmlStrcmp(temp, (xmlChar *) "SDL"))
					mType = RS_SDL;
				else if(!xmlStrcmp(temp, (xmlChar *) "XGL"))
					mType = RS_XGL;
				else if(!xmlStrcmp(temp, (xmlChar *) "RS_GLUT"))
				{
					// wtf ??? нету этого у меня ))
					mType = RS_GLUT;
				}
				else ren = false;
			}

			if(!xmlStrcmp(cur->name, (xmlChar *) "Video"))
			{
				xmlChar * temp = xmlGetProp(cur, (xmlChar *) "ColorBits");
				mVideoSettings.color_bits =
					CStringUtils::StringToInt(reinterpret_cast<char *>(temp)); //atoi((char *)temp);

				temp = xmlGetProp(cur, (xmlChar *) "DoubleBuffering");
				if(!xmlStrcmp(temp, (xmlChar *) "yes"))
					mVideoSettings.d_buff = true;
				else
					mVideoSettings.d_buff = false;

				temp = xmlGetProp(cur, (xmlChar *) "VSync");
				if(!xmlStrcmp(temp, (xmlChar *) "yes"))
					mVideoSettings.vsync = true;
				else
					mVideoSettings.vsync = false;

				temp = xmlGetProp(cur, (xmlChar *) "Freq");
				mVideoSettings.freq =
					CStringUtils::StringToInt(reinterpret_cast<char *>(temp));

				temp = xmlGetProp(cur, (xmlChar *) "FSAA");
				mVideoSettings.FSAA =
					CStringUtils::StringToInt(reinterpret_cast<char *>(temp));

				temp = xmlGetProp(cur, (xmlChar *) "Height");
				mVideoSettings.height =
					CStringUtils::StringToInt(reinterpret_cast<char *>(temp));

				temp = xmlGetProp(cur, (xmlChar *) "Width");
				mVideoSettings.width =
					CStringUtils::StringToInt(reinterpret_cast<char *>(temp));


				temp = xmlGetProp(cur, (xmlChar *) "Fullscreen");
				if(!xmlStrcmp(temp, (xmlChar *) "yes"))
					mVideoSettings.fullscreen = true;
				else
					mVideoSettings.fullscreen = false;

				workname = nstring((char *)xmlGetProp(cur, (xmlChar *) "Title"));
				mVideoSettings.DepthBuffered = true;
				mVideoSettings.xpos = 0;
				mVideoSettings.ypos = 0;

				vid = true;
			}

			cur = cur->next;
		}

		xmlFreeDoc(doc);

		if(ren && vid)
			mConfigIsReady = true;
	}
#endif
	if(mFlags == SF_START_AUTO)
	{

	}
}

nInt32 CRenderSystem::MessageLoopPump()
{
	if(!mRootWindow.IsNull())
		return mRootWindow->MessagePump();

	return 0;
}

void CRenderSystem::StartUp(StartUpFlags flags)
{
	CLog::GetInstance().PrintMessage(
	"\n\n\
	|===========================================|\n \
	|===========================================|\n \
	|                                           |\n \
	|	Starting OpenGL rendering system    |\n \
	|                                           |\n \
	|===========================================|\n \
	|===========================================|\n\n");
	mFlags = flags;
	InitializeFromConfigFile();
// Creating gl workspace
	if(!mConfigIsReady)
		return;

	switch(mType)
	{
#ifdef WIN_BUILD
	case RS_WIN32:
		{
			mSupport = new CWGLSupport();
		}
		break;
#else
	case RS_XGL:
		{

		}
		break;
#endif
	case RS_SDL:
		{
			mSupport = new CSDLSupport();
		}
		break;
	case RS_GLUT:
		{
			//Шутка
			throw NOVA_EXP("CRenderSystem::StartUp - Can not \
				create GLUT SubSystem: not supported!", BAD_OPERATION);
		}
		break;
	default:
		{
			mSupport = new CSDLSupport();
		}
	}
	mSupport->Start();
// Creating render window
	CreateRenderWindow(workname, 1);
}

void CRenderSystem::ShutDown()
{
	if(mSupport)
	{
		mSupport->Stop();
		CLog::GetInstance().PrintMessage(
		"\n\n\
		|===========================================|\n \
		|===========================================|\n \
		|                                           |\n \
		|	Stoping OpenGL rendering system     |\n \
		|                                           |\n \
		|===========================================|\n \
		|===========================================|\n\n");

		ReleaseAllTargets();
		delete mSupport;
	}
}

void CRenderSystem::InitializeCapabilities()
{
	LOG_MESSAGE("Finding necessary extentions..");
	nstring Vendor = mSupport->GetGPUVendor();

	if (Vendor.find("NVIDIA") != nstring::npos)
		mCapabilities.SetGPUVendor(GPU_NVIDIA);
	else if (Vendor.find("ATI") != nstring::npos)
		mCapabilities.SetGPUVendor(GPU_ATI);
	else if (Vendor.find("Intel") != nstring::npos)
		mCapabilities.SetGPUVendor(GPU_INTEL);
	else if (Vendor.find("S3") != nstring::npos)
		mCapabilities.SetGPUVendor(GPU_S3);
	else if (Vendor.find("Matrox") != nstring::npos)
		mCapabilities.SetGPUVendor(GPU_MATROX);
	else if (Vendor.find("3DLabs") != nstring::npos)
		mCapabilities.SetGPUVendor(GPU_3DLABS);
	else
		mCapabilities.SetGPUVendor(GPU_UNKNOWN);

    if(GLEW_VERSION_1_4 || GLEW_SGIS_generate_mipmap)
    {
		mCapabilities.SetCap(TEXTURE_CATEGORY, CAP_AUTOMIPMAP);
		LOG_MESSAGE("CAP_AUTOMIPMAP finded...");
    }

	if(GLEW_VERSION_1_3 ||
		GLEW_ARB_texture_env_combine ||
		GLEW_EXT_texture_env_combine)
	{
		mCapabilities.SetCap(TEXTURE_CATEGORY, CAP_BLENDING);
		LOG_MESSAGE("CAP_BLENDING finded...");
	}

	if(GLEW_EXT_texture_filter_anisotropic)
	{
		mCapabilities.SetCap(TEXTURE_CATEGORY, CAP_ANISOTROPY);
		LOG_MESSAGE("CAP_ANISOTROPY finded...");
	}

	if(GLEW_VERSION_1_3 ||
		GLEW_ARB_texture_cube_map ||
		GLEW_EXT_texture_cube_map)
	{
		mCapabilities.SetCap(TEXTURE_CATEGORY, CAP_CUBEMAPPING);
		LOG_MESSAGE("CAP_CUBEMAPPING finded...");
	}

	if (GLEW_VERSION_2_0 ||	GLEW_ARB_point_sprite)
	{
		mCapabilities.SetCap(BASIC_CATEGORY, CAP_POINTSPRITE);
		LOG_MESSAGE("CAP_POINTSPRITE finded...");
	}

	if(GLEW_VERSION_1_5 || GLEW_ARB_vertex_buffer_object)
	{
		mCapabilities.SetCap(BASIC_CATEGORY, CAP_VBO);
		LOG_MESSAGE("CAP_VBO (vertex buffering) finded...");
	}

	if(GLEW_EXT_framebuffer_object)
	{
		if(GLEW_VERSION_2_0 ||
			GLEW_ARB_draw_buffers ||
			GLEW_ATI_draw_buffers)
		{
			GLint buffers;
			glGetIntegerv(GL_MAX_DRAW_BUFFERS_ARB, &buffers);
			mCapabilities.SetMaxBuffers(buffers);

			if(GLEW_ARB_draw_buffers)
			{
				mCapabilities.SetCap(BASIC_CATEGORY, CAP_DRAWBUFFERS_ARB);
				LOG_MESSAGE("CAP_DRAWBUFFERS_ARB (frame buffering) finded...");
			}
			if(GLEW_ATI_draw_buffers)
			{
				mCapabilities.SetCap(BASIC_CATEGORY, CAP_DRAWBUFFERS_ATI);
				LOG_MESSAGE("CAP_DRAWBUFFERS_ATI (frame buffering) finded...");
			}
		}

		mCapabilities.SetCap(BASIC_CATEGORY, CAP_FBO);
		LOG_MESSAGE("CAP_FBO (frame buffering) finded...");
	}

	if(GLEW_ARB_pixel_buffer_object)
	{
		mCapabilities.SetCap(BASIC_CATEGORY, CAP_PBO);
		LOG_MESSAGE("CAP_PBO (ARB_pixel_buffer_object) finded...");
	}

	if(GLEW_ARB_occlusion_query)
	{
		mCapabilities.SetCap(BASIC_CATEGORY, CAP_OCCLUSION_QUERY);
		LOG_MESSAGE("CAP_OCCLUSION_QUERY finded...");
	}
}

CRenderTargetPtr CRenderSystem::AttachRenderTarget(CRenderTarget * target)
{
	TRenderTargetsMap::iterator it;
	if(mRenderTextures.find(target->GetPriority()) != mRenderTextures.end())
		throw NOVA_EXP("CRenderSystem::AttachWindowRenderTarget - can not attach this window, \
		this priority already exist!", BAD_OPERATION);

	std::pair<nInt32, CRenderTargetPtr> Tagert;
	Tagert.first = target->GetPriority();
	Tagert.second = CRenderTargetPtr(target);

	mRenderTextures.insert(Tagert);
	return Tagert.second;
}

void CRenderSystem::DetachRenderTarget(CRenderTarget * target)
{
	TRenderTargetsMap::iterator it = mRenderTextures.find(target->GetPriority());
	if(it == mRenderTextures.end())
		throw NOVA_EXP("CRenderSystem::AttachWindowRenderTarget - can not find this target!", BAD_OPERATION);

	mRenderTextures.erase(it);
}

void CRenderSystem::DetachRenderTarget(nstring & name)
{
	TRenderTargetsMap::iterator it = mRenderTextures.begin();
	bool finding = false;

	for(; it != mRenderTextures.end(); ++it)
		if((*it).second->GetTagertName() == name)
		{
			mRenderTextures.erase(it);
			finding = true;
		}

	if(!finding)
		throw NOVA_EXP("CRenderSystem::AttachWindowRenderTarget - can not find this target!", BAD_OPERATION);
}

CRenderWindowPtr CRenderSystem::CreateRenderWindow(nstring & name, nInt32 priority)
{
	CRenderWindow * target = mSupport->CreateRenderWindowTarget(name, priority);

	if(target)
	{
//		target->CreateRenderWindow(init);

		mRootWindow = CRenderWindowPtr(target);
	}

	return mRootWindow;
}

void CRenderSystem::MakeRenderWindowTarget(void)
{
	if(!mRootWindow.IsNull())
	{
		mRootWindow->CreateRenderWindow(mVideoSettings);
	}

	InitializeCapabilities();
}

void CRenderSystem::ReleaseRenderTarget(CRenderTargetPtr & target)
{
/*	target->Destroy();

*/

	DetachRenderTarget(target.GetPtr());
	//target.Free();
}

void CRenderSystem::ReleaseAllTargets()
{
	TRenderTargetsMap::iterator it;

//	PrepareAllRenderTargets();
	it = mRenderTextures.begin();

//	for(; it != mRenderTextures.end(); ++it)
//		(*it).second.Free();
	mRenderTextures.clear();


	mSupport->DestroyRootWindow();
}

CRenderSystem::~CRenderSystem()
{
	LOG_MESSAGE("Render system destroyed..");
}

void CRenderSystem::SwitchToRootContext()
{
	if(mSupport)
	{
		mSupport->GetRootWindow()->GetContext()->SetCurrent();
	}

}

void CRenderSystem::PrepareAllRenderTargets()
{
	//SwitchToRootContext();

	// what else ??
	TRenderTargetsMap::iterator it;
	it = mRenderTextures.begin();

	for(; it != mRenderTextures.end(); ++it)
	{
		CRenderTargetPtr RTT = (*it).second;

		if(RTT->IsReady())
			if(RTT->IsAutoUpdate())
			{
				RTT->Update();
				mTargetsBuilded++;
			}
	}

}

bool CRenderSystem::CheckCapabilities(CapCategory cat, nInt32 cap)
{
	return mCapabilities.IsSupport(cat, cap);
}

nInt32 CRenderSystem::BuildAllRenderTargets()
{
	// building RTT targets
	TRenderTargetsMap::iterator it;
	mTargetsBuilded = 0;

	it = mRenderTextures.begin();

	PrepareAllRenderTargets();

	for(; it != mRenderTextures.end(); ++it)
	{
		CRenderTargetPtr RTT = (*it).second;

		if(RTT->IsReady())
			if(RTT->IsAutoUpdate())
			{
				RTT->Update();
				mTargetsBuilded++;
			}
	}

	// building windows targets

	if(mRootWindow->IsReady())
		if(mRootWindow->IsAutoUpdate())
		{
			if(mRootWindow->IsClosed())
				return -1;

			mRootWindow->Update();
			mTargetsBuilded++;
		}

	_DoScreenShot();

	return mTargetsBuilded;
}

CRenderWindowPtr CRenderSystem::GetRootWindow()
{
	return mRootWindow;
}

CRenderCapabilities & CRenderSystem::GetCapabilities()
{
	return mCapabilities;
}

void CRenderSystem::SetPerspectiveProjection(nova::nReal fovy, nova::nReal aspect, nova::nReal zNear, nova::nReal zFar)
{
	glPushAttrib(GL_TRANSFORM_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(fovy, aspect, zNear, zFar);

	glPopAttrib();
}

nova::Matrix4f CRenderSystem::GetProjectionMatrix()
{
	nova::Matrix4f proj = nova::Matrix4f::IDENTITY;
	glGetFloatv(GL_PROJECTION_MATRIX, proj);

	return proj;
}

void CRenderSystem::SetPolygonMode(CCamera::PolygonMode mode, CCamera::FaceSide side)
{
	GLenum glmode = 0;
	GLenum glside = 0;

	switch(mode)
	{
	case CCamera::PM_POINTS:
		glmode = GL_POINT;
		break;
	case CCamera::PM_WIREFRAME:
		glmode = GL_LINE;
		break;
	case CCamera::PM_SOLID:
		glmode = GL_FILL;
		break;
	default:
		return;
	}

	switch(side)
	{
	case CCamera::FS_FRONT:
		glside = GL_FRONT;
		break;
	case CCamera::FS_BACK:
		glside = GL_BACK;
		break;
	case CCamera::FS_FRONT_AND_BACK:
		glside = GL_FRONT_AND_BACK;
		break;
	default:
		return;
	}

	glPolygonMode(glside, glmode);
}

void CRenderSystem::SetLocalMatrix(nova::Matrix4f & matrix)
{
	glPushAttrib(GL_TRANSFORM_BIT);

	glMatrixMode(GL_MODELVIEW);

	glLoadMatrixf(matrix);

	glPopAttrib();
}

nova::Matrix4f CRenderSystem::GetLocalMatrix(void)
{
	nova::Matrix4f mod = nova::Matrix4f::IDENTITY;
	glGetFloatv(GL_MODELVIEW_MATRIX, mod);

	return mod;
}

void CRenderSystem::SetProjection(nova::Matrix4f & matrix)
{
	glPushAttrib(GL_TRANSFORM_BIT);

	glMatrixMode(GL_PROJECTION);

	glLoadMatrixf(matrix);

	glPopAttrib();
}

void CRenderSystem::SetOrthoProjection(nova::nReal left, nova::nReal right,
		nova::nReal bottom, nova::nReal top, nova::nReal znear, nova::nReal zfar)
{
	glPushAttrib(GL_TRANSFORM_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(left, right, bottom, top, znear, zfar);

	glPopAttrib();
}

void CRenderSystem::SetBlend(bool flag)
{
	if(flag)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
}

void CRenderSystem::SetBlendFunc(BlendingFinctions blend_sfactor, BlendingFinctions blend_dfactor)
{
	GLenum sfactor = 0x0, dfactor = 0x0;

	for(nInt32 i = 0; i < 2; i++)
	{
		GLenum * target1 = NULL;
		GLenum * target2 = NULL;
		if(i == 0)
		{
			target1 = (GLenum*)&blend_sfactor;
			target2 = (GLenum*)&sfactor;
		}
		else
		{
			target1 = (GLenum*)&blend_dfactor;
			target2 = (GLenum*)&dfactor;
		}

		switch(*target1)
		{
		case BF_ZERO:
			*target2 = GL_ZERO;
			break;

		case BF_ONE:
			*target2 = GL_ONE;
			break;

		case BF_SRC_COLOR:
			*target2 = GL_SRC_COLOR;
			break;

		case BF_ONE_MINUS_SRC_COLOR:
			*target2 = GL_ONE_MINUS_SRC_COLOR;
			break;

		case BF_DST_COLOR:
			*target2 = GL_DST_COLOR;
			break;

		case BF_ONE_MINUS_DST_COLOR:
			*target2 = GL_ONE_MINUS_DST_COLOR;
			break;

		case BF_SRC_ALPHA:
			*target2 = GL_SRC_ALPHA;
			break;

		case BF_ONE_MINUS_SRC_ALPHA:
			*target2 = GL_ONE_MINUS_SRC_ALPHA;
			break;

		case BF_DST_ALPHA:
			*target2 = GL_DST_ALPHA;
			break;

		case BF_ONE_MINUS_DST_ALPHA:
			*target2 = GL_ONE_MINUS_DST_ALPHA;
			break;

		case BF_SRC_ALPHA_SATURATE:
			*target2 = GL_SRC_ALPHA_SATURATE;
			break;

		case BF_CONSTANT_COLOR:
			*target2 = GL_CONSTANT_COLOR;
			break;

		case BF_ONE_MINUS_CONSTANT_COLOR:
			*target2 = GL_ONE_MINUS_CONSTANT_COLOR;
			break;

		case BF_CONSTANT_ALPHA:
			*target2 = GL_CONSTANT_ALPHA;
			break;

		case BF_ONE_MINUS_CONSTANT_ALPHA:
			*target2 = GL_ONE_MINUS_CONSTANT_ALPHA;
			break;

		default:
			return ;
		}
	}

	glBlendFunc(sfactor, dfactor);
}

void CRenderSystem::SetVeiwport(nInt32 x, nInt32 y, nInt32 width, nInt32 height)
{
	glViewport(x, y, width, height);
}

void CRenderSystem::ClearBackBuffers(GLbitfield mask, CColorRGBA & clcolor)
{
	glClearColor(clcolor.R(), clcolor.G(), clcolor.B(), clcolor.A());

	glClear(mask);
}

void CRenderSystem::DoScreenshot(CRenderTarget *target)
{
	mScreenShotTarget =	target;
}

void CRenderSystem::_DoScreenShot(void)
{
	if(mScreenShotTarget)
	{
		// Write something code here

	}
}

}
