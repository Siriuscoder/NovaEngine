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

	nova::nReal mZFar;
	nova::nReal mZNear;
	nova::nReal mLeft; 
	nova::nReal mRight; 
	nova::nReal mBottom;
	nova::nReal mTop;

	nova::nReal mFOV;

	nova::nReal mAspect;

	CViewPort * mParentView;

	ProjectionType mType;
	PolygonMode mPolygonMode;
	FaceSide mSide;

public:

	CCamera();

	CCamera(nova::nReal Left, nova::nReal Right, nova::nReal Bottom, nova::nReal Top,
			nova::nReal ZFar, nova::nReal ZNear, CViewPort * mParentView);

	CCamera(nova::nReal FOV, nova::nReal ZFar, 
		nova::nReal ZNear, CViewPort * mParentView);

	~CCamera();

	void Validate(void);

	void InValidateModel(void);

	void InValidateProjection(void);

	void SetZFar(nova::nReal zfar);

	void SetZNear(nova::nReal znear);

	void SetFOV(nova::nReal fov);

	void SetAspect(nova::nReal aspect);

	void SetType(ProjectionType type);

	CViewPort * GetParentView(void);

	nova::nReal GetZFar(void);

	nova::nReal GetZNear(void);

	nova::nReal GetFOV(void);

	nova::nReal GetAspect(void);

	ProjectionType GetProjectionType(void);

	nova::nReal GetLeft(void); 

	nova::nReal GetRight(void); 

	nova::nReal GetBottom(void);

	nova::nReal GetTop(void);

	void SetLeft(nova::nReal left);

	void SetRight(nova::nReal right);

	void SetBottom(nova::nReal bottom);

	void SetTop(nova::nReal top);

	PolygonMode GetPolygonMode(void);

	void SetPolygonMode(PolygonMode mode, FaceSide side);

	Vector3f GetDirection(void);

	void SetDirection(const Vector3f & vec);

	void RotateTo(const Vector3f vec);

	void LookAt(const nReal x, const nReal y, const nReal z);

	void LookAt(const Vector3f & direction);

	Vector3f GetUpDirection(void);

	Vector3f GetRightDirection(void);

	void Roll(const nova::nReal radian);

	void Yaw(const nova::nReal radian);

	void Pitch(const nova::nReal radian);
};

typedef CSmartPtr<CCamera> CCameraPtr;

}