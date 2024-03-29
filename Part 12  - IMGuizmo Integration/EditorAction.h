#pragma once
#include <iostream>
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "AGameObject.h"

//an action performed by the user in the editor. Stores a snapshot for undoing and redoing
class EditorAction
{
public:
	EditorAction(AGameObject* gameObject);
	~EditorAction();

	AGameObject::String getOwnerName();
	Vector3D getStorePos();
	Vector3D getStoredScale();
	Vector3D getStoredOrientation();
	Matrix4x4 getStoredMatrix();

private:
	AGameObject::String objectName;
	Vector3D localPosition;
	Vector3D localScale;
	Vector3D localRotation;
	Matrix4x4 localMatrix;
};

