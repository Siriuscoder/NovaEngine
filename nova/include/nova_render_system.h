﻿/***************************************************************************
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

#ifdef WIN_BUILD
#include "nova_win32_workspace.h"
#endif

#include "nova_sdl_workspace.h"

namespace nova
{

#ifdef WIN_BUILD
#define DEFAULT_CONFIG_PATH "./config.xml"
#else
#define DEFAULT_CONFIG_PATH "/usr/local/share/nova/nova.config"
#endif

enum CapCategory
{
	BASIC_CATEGORY,
	SHADING_CATEGORY,
	TEXTURE_CATEGORY,
	NUM_CATEGORYES
};

enum StartUpFlags
{
	SF_START_AUTO,
	SF_START_AUTO_XML_CONFIG,
	SF_START_MANUAL
};

enum BasicCapabilities
{
	CAP_VBO,
	CAP_FBO,
	CAP_PBO,
	CAP_POINTSPRITE,
	CAP_DRAWBUFFERS_ATI,
	CAP_DRAWBUFFERS_ARB,
	CAP_OCCLUSION_QUERY
};

enum ShadingCapabilities
{


};

enum TextureCapabilities
{
	CAP_AUTOMIPMAP,
	CAP_CUBEMAPPING,
	CAP_ANISOTROPY,
	CAP_BLENDING

};

enum GPUVendor
{
	GPU_UNKNOWN,
	GPU_NVIDIA,
	GPU_ATI,
	GPU_INTEL,
	GPU_S3,
	GPU_MATROX,
	GPU_3DLABS,
	/// placeholder
	GPU_VENDOR_COUNT
};

enum BlendingFinctions
{
	BF_ZERO,
	BF_ONE,
	BF_SRC_COLOR,
	BF_ONE_MINUS_SRC_COLOR,
	BF_DST_COLOR,
	BF_ONE_MINUS_DST_COLOR,
	BF_SRC_ALPHA,
	BF_ONE_MINUS_SRC_ALPHA,
	BF_DST_ALPHA,
	BF_ONE_MINUS_DST_ALPHA,
	BF_SRC_ALPHA_SATURATE,
	BF_CONSTANT_COLOR,
	BF_ONE_MINUS_CONSTANT_COLOR,
	BF_CONSTANT_ALPHA,
	BF_ONE_MINUS_CONSTANT_ALPHA
};

class NOVA_EXPORT CRenderCapabilities : public CBase
{
private:

	nInt32 mCap[NUM_CATEGORYES];

	GPUVendor mVendor;

	nova::nUInt32 mMaxDrawBuffers;

public:

	CRenderCapabilities();
	~CRenderCapabilities() {}

	bool IsSupport(CapCategory cat, nInt32 cap);

	void SetCap(CapCategory cat, nInt32 cap);

	void SetGPUVendor(GPUVendor vendor);

	GPUVendor GetGPUVendor();

	void SetMaxBuffers(nova::nUInt32 num);

	nova::nUInt32 GetMaxBuffers();
};

class NOVA_EXPORT CRenderSystem : public CSingelton<CRenderSystem>, public CBase
{
public:

	typedef nova::stl<nInt32, CRenderTargetPtr>::map TRenderTargetsMap;
protected:

	CGLSupport * mSupport;

	CRenderTarget *mScreenShotTarget;

	TRenderTargetsMap mRenderTextures;
	CRenderWindowPtr mRootWindow;

	CRenderCapabilities mCapabilities;

	TWindowInitialTarget mVideoSettings;

	void SwitchToRootContext();

	void InitializeFromConfigFile();

private:

	nstring mPrivateConfigFile;

	nova::nUInt32 mGlobalFSAA;

	nInt32 mTargetsBuilded;

	StartUpFlags mFlags;
	bool mConfigIsReady;
	nstring workname;
	stl<nstring, CGLSupport*>::map mGlSupports;

	void _DoScreenShot(void);

public:

	CRenderSystem();
	~CRenderSystem();

	void RegisterGlSubSystem(CGLSupport *glsup);

	void UnregisterGlSubSystem(CGLSupport *glsup);

	void UnregisterAndDestroyAllSubSystems(void);

	CGLSupport *GetGlSubSystem(const nstring &name);

	void SetConfigPath(nstring & config);

	nInt32 MessageLoopPump();

	void StartUp(StartUpFlags flags);

	void ShutDown();

	void InitializeCapabilities();

	CRenderTargetPtr AttachRenderTarget(CRenderTarget * target);

	void DetachRenderTarget(CRenderTarget * target);

	void DetachRenderTarget(nstring & name);

	CRenderWindowPtr CreateRenderWindow(nstring & name, nInt32 priority);

	void ReleaseRenderTarget(CRenderTargetPtr & target);

	void ReleaseAllTargets();

	void PrepareAllRenderTargets();

	bool CheckCapabilities(CapCategory cat, nInt32 cap);

	CRenderCapabilities & GetCapabilities();

	nInt32 BuildAllRenderTargets();

	CRenderWindowPtr GetRootWindow();

	void MakeRenderWindowTarget(void);

	void SetPerspectiveProjection(nova::nReal fovy, nova::nReal aspect, nova::nReal zNear, nova::nReal zFar);

	void SetOrthoProjection(nova::nReal left, nova::nReal right,
		nova::nReal bottom, nova::nReal top, nova::nReal znear, nova::nReal zfar);

	void SetProjection(nova::Matrix4f & matrix);

	nova::Matrix4f GetProjectionMatrix();

	void SetPolygonMode(CCamera::PolygonMode mode, CCamera::FaceSide side);

	void SetLocalMatrix(nova::Matrix4f & matrix);

	nova::Matrix4f GetLocalMatrix(void);

	void SetBlend(bool flag);

	void SetBlendFunc(BlendingFinctions blend_sfactor, BlendingFinctions blend_dfactor);

	void SetVeiwport(nInt32 x, nInt32 y, nInt32 width, nInt32 height);

	void ClearBackBuffers(GLbitfield mask, CColorRGBA & clcolor);

	void DoScreenshot(CRenderTarget *target);
};


}
