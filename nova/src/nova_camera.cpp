/***************************************************************************
 *   Copyright (C) 2009 by SIRIUS										   *
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

#include "nova_camera.h"
#include "nova_render_system.h" 

namespace nova
{

CCamera::CCamera() : mProjectionChanged(false),
	mParentView(NULL), mAspect(1.3333333f), mFOV(90), mZFar(-1), mZNear(1),
	mLeft(0), mRight(0), mBottom(0), mTop(0), mPolygonMode(PM_SOLID), mSide(FS_FRONT_AND_BACK)

{
	mOrientation = nova::Quaternionf::IDENTITY;
	mDirection = nova::Vector3f::ZERO;
	mType = PT_PERSPECTIVE;
	mProjectionChanged = true;

	LOG_MESSAGE("Creating perspective camera...");
}

CCamera::CCamera(nova::nReal Left, nova::nReal Right, nova::nReal Bottom, nova::nReal Top,
				 nova::nReal ZFar, nova::nReal ZNear, CViewPort * mParentView) : 
	mProjectionChanged(false), mParentView(mParentView), 
	mAspect(1.3333333f), mFOV(90), mZFar(ZFar), mZNear(ZNear),
	mLeft(Left), mRight(Right), mBottom(Bottom), mTop(Top), mPolygonMode(PM_SOLID),
	mSide(FS_FRONT_AND_BACK)
{
	mOrientation = nova::Quaternionf::IDENTITY;
	mDirection = nova::Vector3f::ZERO;
	mType = PT_ORTHO;
	mProjectionChanged = true;

	LOG_MESSAGE("Creating ORTHO camera...");
}

CCamera::CCamera(nova::nReal FOV, nova::nReal ZFar, 
				 nova::nReal ZNear, CViewPort * mParentView) :
	mProjectionChanged(false), mParentView(mParentView), 
	mFOV(FOV), mZFar(ZFar), mZNear(ZNear),
	mLeft(0), mRight(0), mBottom(0), mTop(0), mPolygonMode(PM_SOLID),
	mSide(FS_FRONT_AND_BACK)
{
	mOrientation = nova::Quaternionf::IDENTITY;
	mType = PT_PERSPECTIVE;
	mAspect = (nova::nReal)mParentView->GetActualWidth() / (nova::nReal)mParentView->GetActualHeight();
	mProjectionChanged = true;

	LOG_MESSAGE("Creating perspective camera...");
}

void CCamera::Validate(void)
{
	bool change = false;
	if(mLocationChanged || mOrientationChanged || mScaleChanged)
		change = true;

	ComputeLocalMatrix();

	if(mProjectionChanged)
	{
		switch(mType)
		{
		case PT_PERSPECTIVE:
			{
				//SetPerspectiveProjection(
				CRenderSystem::GetSingelton().SetPerspectiveProjection(mFOV,
					mAspect, mZNear, mZFar);
				CFrustum::mProjection = CRenderSystem::GetSingelton().GetProjectionMatrix();
				change = true;
			}
			break;
		case PT_ORTHO:
			{
				CRenderSystem::GetSingelton().SetOrthoProjection(mLeft, mRight, mBottom, mTop, mZNear, mZFar);
				CFrustum::mProjection = CRenderSystem::GetSingelton().GetProjectionMatrix();
				change = true;
			}
			break;
		default:
			{
			}
		}

		mProjectionChanged = false;
	}

	CRenderSystem::GetSingelton().SetPolygonMode(mPolygonMode, mSide);
	CRenderSystem::GetSingelton().SetProjection(CFrustum::mProjection);
	CRenderSystem::GetSingelton().SetLocalMatrix(CWorldObject::mLocalMatrix);

	if(change)
		CFrustum::ExtractFrustum();

	for(nova::uint i = 0; i < GetListenersCount(); i++)
	{
		CCameraListener * lis = 
			dynamic_cast<CCameraListener *>(GetListener(i));
		lis->ValidateListener(this);
	}
}


void CCamera::InValidateModel(void)
{
	for(nova::uint i = 0; i < GetListenersCount(); i++)
	{
		CCameraListener * lis = 
			dynamic_cast<CCameraListener *>(GetListener(i));
		lis->ModelChangedListener(this);
	}

	LocationChanged();
	OrientationChanged();
	ScaleChanged();
}

void CCamera::InValidateProjection(void)
{
	for(nova::uint i = 0; i < GetListenersCount(); i++)
	{
		CCameraListener * lis = 
			dynamic_cast<CCameraListener *>(GetListener(i));
		lis->ProjectionChangedListener(this);
	}

	mProjectionChanged = true;
}

void CCamera::SetZFar(nova::nReal zfar)
{
	mZFar = zfar;
	InValidateProjection();
}

void CCamera::SetZNear(nova::nReal znear)
{
	mZNear = znear;
	InValidateProjection();
}

void CCamera::SetFOV(nova::nReal fov)
{
	mFOV = fov;
	InValidateProjection();
}

void CCamera::SetAspect(nova::nReal aspect)
{
	mAspect = aspect;
	InValidateProjection();
}

void CCamera::SetType(ProjectionType type)
{
	mType = type;
	InValidateProjection();
}

CViewPort * CCamera::GetParentView(void)
{
	return mParentView;
}

nova::nReal CCamera::GetZFar(void)
{
	return mZFar;
}

nova::nReal CCamera::GetZNear(void)
{
	return mZNear;
}

nova::nReal CCamera::GetFOV(void)
{
	return mFOV;
}

nova::nReal CCamera::GetAspect(void)
{
	return mAspect;
}

CCamera::ProjectionType CCamera::GetProjectionType(void)
{
	return mType;
}

nova::nReal CCamera::GetLeft(void)
{
	return mLeft;
}

nova::nReal CCamera::GetRight(void)
{
	return mRight;
}

nova::nReal CCamera::GetBottom(void)
{
	return mBottom;
}

nova::nReal CCamera::GetTop(void)
{
	return mTop;
}

void CCamera::SetLeft(nova::nReal left)
{
	mLeft = left;
	InValidateProjection();
}

void CCamera::SetRight(nova::nReal right)
{
	mRight = right;
	InValidateProjection();
}

void CCamera::SetBottom(nova::nReal bottom)
{
	mBottom = bottom;
	InValidateProjection();
}

void CCamera::SetTop(nova::nReal top)
{
	mTop = top;
	InValidateProjection();
}

CCamera::PolygonMode CCamera::GetPolygonMode(void)
{
	return mPolygonMode;
}

void CCamera::SetPolygonMode(PolygonMode mode, FaceSide side)
{
	mPolygonMode = mode;
	mSide = side;
}

Vector3f CCamera::GetDirection(void)
{
	return mOrientation.Rotate(Vector3f::UNIT_Z);
}

Vector3f CCamera::GetUpDirection(void)
{
	return mOrientation.Rotate(Vector3f::UNIT_Y);
}

Vector3f CCamera::GetRightDirection(void)
{
	return mOrientation.Rotate(Vector3f::UNIT_X);
}

void CCamera::SetDirection(const Vector3f & vec)
{
	//Vector3f vec1 = GetDirection();

	RotateTo(vec);
}

void CCamera::RotateTo(const Vector3f vec)
{
	Quaternionf temp;
	//Vector3f direct;


	//direct.X() = vec.X() - mLocalPosition.X();
	//direct.Y() = vec.Y() - mLocalPosition.Y();
	//direct.Z() = vec.Z() - mLocalPosition.Z();
	//direct.Normalize();

	temp.Align(GetDirection(), vec);

	Rotate(temp);
}

void CCamera::LookAt(const nReal x, const nReal y, const nReal z)
{
	LookAt(Vector3f(x, y, z));
}

void CCamera::LookAt(const Vector3f & direction)
{
	SetDirection(direction - mPosition);
}

void CCamera::Roll(const nova::nReal radian)
{
	Vector3f axis = mOrientation.Rotate(Vector3f::UNIT_Z);
	Quaternionf zrot;

	zrot.FromAxisAngle(axis, radian);
	Rotate(zrot);
}

void CCamera::Yaw(const nova::nReal radian)
{
	Vector3f axis = mOrientation.Rotate(Vector3f::UNIT_Y);
	Quaternionf yrot;

	yrot.FromAxisAngle(axis, radian);
	Rotate(yrot);
}

void CCamera::Pitch(const nova::nReal radian)
{
	Vector3f axis = mOrientation.Rotate(Vector3f::UNIT_X);
	Quaternionf xrot;

	xrot.FromAxisAngle(axis, radian);
	Rotate(xrot);
}

CCamera::~CCamera()
{
	nstring str("Camera destoyed name: ");
	str.append(mName);
	LOG_MESSAGE(str);
}

}