п»ї/***************************************************************************
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

#include "nova_frustum.h"

namespace nova
{

CFrustum::CFrustum()
{
	ClearFrustumPlanes();
	mProjection = Matrix4f::IDENTITY;
}

void CFrustum::ExtractFrustum()
{
	if(!mActive)
		return;

	Matrix4f clip = Matrix4f::IDENTITY;
	nova::real len;

	clip = mProjection * mLocalMatrix;

   /* РќР°С…РѕРґРёРј A, B, C, D РґР»СЏ РџР РђР’РћР™ РїР»РѕСЃРєРѕСЃС‚Рё */
	mFrustumPlanes[0].Normal[0] = clip(0, 3) - clip(0, 0);
	mFrustumPlanes[0].Normal[1] = clip(1, 3) - clip(1, 0);
	mFrustumPlanes[0].Normal[2] = clip(2, 3) - clip(2, 0);
	mFrustumPlanes[0].Constant =  clip(3, 3) - clip(3, 0);

	/* РџСЂРёРІРѕРґРёРј СѓСЂР°РІРЅРµРЅРёРµ РїР»РѕСЃРєРѕСЃС‚Рё Рє РЅРѕСЂРјР°Р»СЊРЅРѕРјСѓ РІРёРґСѓ */
	len = mFrustumPlanes[0].Normal.Normalize();
	mFrustumPlanes[0].Constant /= len;

	/* РќР°С…РѕРґРёРј A, B, C, D РґР»СЏ Р›Р•Р’РћР™ РїР»РѕСЃРєРѕСЃС‚Рё */
	mFrustumPlanes[1].Normal[0] = clip(0, 3) + clip(0, 0);
	mFrustumPlanes[1].Normal[1] = clip(1, 3) + clip(1, 0);
	mFrustumPlanes[1].Normal[2] = clip(2, 3) + clip(2, 0);
	mFrustumPlanes[1].Constant  = clip(3, 3) + clip(3, 0);

	/* РџСЂРёРІРѕРґРёРј СѓСЂР°РІРЅРµРЅРёРµ РїР»РѕСЃРєРѕСЃС‚Рё Рє РЅРѕСЂРјР°Р»СЊРЅРѕРјСѓ РІРёРґСѓ */
	len = mFrustumPlanes[1].Normal.Normalize();
	mFrustumPlanes[1].Constant /= len;

	/* РќР°С…РѕРґРёРј A, B, C, D РґР»СЏ РќР