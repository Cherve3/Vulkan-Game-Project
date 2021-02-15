#ifndef __GFC_TRANSFORM_H__
#define __GFC_TRANSFORM_H__

#include "gfc_quaternion.h"

typedef struct{

	Matrix4 parentMatrix;

	Vector3D position;
	Quaternion rotation;
	Vector3D scale;

	Vector3D oldPos;
	Quaternion oldRot;
	Vector3D oldScale;

}Transform;

Transform update_transform();

Quaternion rotate_transform(Quaternion rotation, Vector3D axis, float angle);

Quaternion get_look_rotation(Vector3D position, Vector3D point, Vector3D up);

int has_changed(Transform transform);

Matrix4 *get_transformation(Transform transform);

Matrix4 *get_parent_matrix();

void set_parent_matrix();

Vector3D get_transform_position();

Vector3D get_transform_rotation();

Vector3D get_position();

void set_position(Vector3D vec);

Quaternion get_rotation();

void set_rotation(Quaternion rot);

Vector3D get_scale();

void set_scale(Vector3D scale);

#endif
