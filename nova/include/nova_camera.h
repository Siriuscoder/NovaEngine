п»ї/***************************************************************************
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
#pragma once

#include "nova_color.h"
#include "nova_smart_ptr.h"
#include "nova_quaternion.h"
#include "nova_frustum.h"

namespace nova
{

class CViewPort;

class CCamera;

class NOVA_EXPORT CCameraListener : public CFrustumListener
{
public:

	virtual void ModelChangedListener(CCamera * object) {}

	virtual void ProjectionChangedListener(CCamera * object) {}

	virtual void ValidateListener(CCamera * object) {}

};

class NOVA_EXPORT CCamera : public CFrustum
{
public:

enum PolygonMode
{
	PM_POINTS, 	
	PM_WIREFRAME,
	PM_SOLID 
};

enum FaceSide
{
	FS_FRONT,
	FS_BACK,
	FS_FRONT_AND_BACK
};

	enum ProjectionType
	{
		PT_PERSPECTIVE,
		PT_ORTHO
	};

protected:

	bool mProjectionChanged;

	nova::Quaternionf mOrientation;

	nova::Vector3f mDirection;

	nova::real mZFar;
	nova::real mZNear;
	nova::real mLeft; 
	nova::real mRight; 
	nova::real mBottom;
	nova::real mTop;

	nova::real mFOV;

	nova::real mAspect;

	CViewPort * mParentView;

	ProjectionType mType;
	PolygonMode mPolygonMode;
	FaceSide mSide;

public:

	CCamera();

	CCamera(nova::real Left, nova::real Right, nova::real Bottom, nova::real Top,
			nova::real ZFar, nova::real ZNear, CViewPort * mParentView);

	CCamera(nova::real FOV, nova::real ZFar, 
		nova::real ZNear, CViewPort * mParentView);

	~CCamera();

	void Validate(void);

	void InValidateModel(void);

	void InValidateProjection(void);

	void SetZFar(nova::real zfar);

	void SetZNear(nova::real znear);

	void SetFOV(nova::real fov);

	void SetAspect(nova::real aspect);

	void SetType(ProjectionType type);

	CViewPort * GetParentView(void);

	nova::real GetZFar(void);

	nova::real GetZNear(void);

	nova::real GetFOV(void);

	nova::real GetAspect(void);

	ProjectionType GetProjectionType(void);

	nova::real GetLeft(void); 

	nova::real GetRight(void); 

	nova::real GetBottom(void);

	nova::real GetTop(void);

	void SetLeft(nova::real left);

	void SetRight(nova::real right);

	void SetBottom(nova::real bottom);

	void SetTop(nova::real top);

	PolygonMode GetPolygonMode(void);

	void SetPolygonMode(PolygonMode mode, FaceSide side);

	Vector3f GetDirection(void);

	void SetDirection(const Vector3f & vec);

	void RotateTo(const Vector3f vec);

	void LookAt(const real x, const real y, const real z);

	void LookAt(const Vector3f & direction);

	Vector3f GetUpDirection(void);

	Vector3f GetRightDirection(void);

	void Roll(const nova::real radian);

	void Yaw(const nova::real radian);

	void Pitch(const nova::real radian);
};

typedef CSmartPtr<CCamera> CCameraPtr;

}