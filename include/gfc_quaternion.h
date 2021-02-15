#ifndef __GFC_QUATERNION_H__
#define __GFC_QUATERNION_H__

#include "gfc_matrix.h"
#include "gfc_vector.h"

typedef struct
{
	float x;
	float y;
	float z;
	float w;
}Quaternion;

#define quaternion_equal(a,b) ((a.x == b.x)&&(a.y == b.y)&&(a.z == b.z)&&(a.w == b.w))

Quaternion quaternion(float x, float y, float z, float w);

void quaternion_init(Quaternion *out, Vector3D axis, float angle);

float quaternion_magnitude(Quaternion Q);

void quaternion_normalize(Quaternion *Q);

void quaternion_conjugate(Quaternion *out, Quaternion Q);

Matrix4 *quaternion_rotation_matrix(Quaternion Q);

void quaternion_multiply(Quaternion *out, Quaternion Q, Quaternion Q2);

void quaternion_vector_multiply(Quaternion *out, Quaternion Q, Vector3D V);

Matrix4 *quaternion_to_matrix(Quaternion Q);

Quaternion quaternion_from_matrix(Matrix4 rot);



#endif