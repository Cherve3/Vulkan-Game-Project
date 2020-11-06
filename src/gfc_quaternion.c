
#include "gfc_quaternion.h"
#include "gfc_matrix.h"

Quaternion quaternion(float x, float y, float z, float w)
{
	Quaternion Q;
	Q.x = x;
	Q.y = y;
	Q.z = z;
	Q.w = w;
	return Q;
}

void quaternion_init(Quaternion *out, Vector3D axis, float angle)
{
	float sinHalfAngle = (float)sin(angle * 0.5);
	float cosHalfAngle = (float)cos(angle * 0.5);

	out->x = axis.x * sinHalfAngle;
	out->y = axis.y * sinHalfAngle;
	out->z = axis.z * sinHalfAngle;
	out->w = cosHalfAngle;
}

float quaternion_magnitude(Quaternion Q)
{
	return sqrt(Q.x * Q.x + Q.y * Q.y + Q.z * Q.z + Q.w * Q.w);
}


void quaternion_normalize(Quaternion *Q)
{
	float M;
	if (!Q)return;
	M = quaternion_magnitude(*Q);
	if (M == 0.0f)
	{
		return;
	}
	M = 1 / M;
	Q->x *= M;
	Q->y *= M;
	Q->z *= M;
	Q->w *= M;
}

void quaternion_conjugate(Quaternion *out, Quaternion Q)
{
	if (!out) return;
	out->x = - (Q.x);
	out->y = - (Q.y);
	out->z = - (Q.z);
	out->w =    Q.w;

	return out;
}

Matrix4 *quaternion_rotation_matrix(Quaternion Q)
{
	Vector3D forward, up, right;
	forward = vector3d( 2.0f *(Q.x * Q.z - Q.w * Q.y), 2.0f * (Q.y * Q.z + Q.w * Q.x), 1.0f - 2.0f * (Q.x * Q.x + Q.y * Q.y));
	up      = vector3d(2.0f *(Q.x * Q.y + Q.w * Q.z), 1.0f - 2.0f * (Q.x * Q.x + Q.z * Q.z), 2.0f * (Q.y * Q.z - Q.w * Q.x));
	right   = vector3d(1.0f - 2.0f *(Q.y * Q.y + Q.z * Q.z), 2.0f * (Q.x * Q.y - Q.w * Q.z), 2.0f * (Q.x * Q.z + Q.w * Q.y));

	return rotate(forward, up, right);
}

void quaternion_multiply(Quaternion *out,Quaternion Q, Quaternion Q2)
{
	
	out->w = Q.w * Q2.w - Q.x * Q2.x - Q.y * Q2.y - Q.z * Q2.z;
	out->x = Q.x * Q2.w + Q.w * Q2.x + Q.y * Q2.z - Q.z * Q2.y;
	out->y = Q.y * Q2.w + Q.w * Q2.y + Q.z * Q2.x - Q.x * Q2.z;
	out->z = Q.z * Q2.w + Q.w * Q2.z + Q.x * Q2.y - Q.y * Q2.x;
}

void quaternion_vector_multiply(Quaternion *out, Quaternion Q, Vector3D V)
{
	out->w = -Q.x * V.x - Q.y * V.y - Q.z * V.z;
	out->x =  Q.w * V.x + Q.y * V.z - Q.z * V.y;
	out->y =  Q.w * V.y + Q.z * V.x - Q.x * V.z;
	out->z =  Q.w * V.z + Q.x * V.y - Q.y * V.x;
}

Matrix4 *quaternion_to_matrix(Quaternion Q)
{
	Vector3D forward, up, right;

	forward =	vector3d(2.0f * (Q.x * Q.z - Q.w * Q.y), 2.0f * (Q.y * Q.z + Q.w * Q.x), 1.0f - 2.0f * (Q.x * Q.x + Q.y * Q.y));
	up =		vector3d(2.0f * (Q.x * Q.y + Q.w * Q.z), 1.0f - 2.0f * (Q.x * Q.x + Q.z * Q.z), 2.0f * (Q.y * Q.z - Q.w * Q.x));
	right =		vector3d(1.0f - 2.0f * (Q.y * Q.y + Q.z * Q.z), 2.0f * (Q.x * Q.y - Q.w * Q.z), 2.0f * (Q.x * Q.z + Q.w * Q.y));

	//gfc_matrix_init_rotation(forward, up, right);
}

Quaternion quaternion_from_matrix(Matrix4 rot)
{
	Quaternion Q;
	float trace = rot[0][0] + rot[1][1] + rot[2][2];
	
	if (trace > 0)
	{
		float s = 0.5f / (float)sqrt(trace + 1.0f);
		Q.w = 0.25f / s;
		Q.x = (rot[1][2] - rot[2][1]) * s;
		Q.y = (rot[2][0] - rot[0][2]) * s;
		Q.z = (rot[0][1] - rot[1][0]) * s;
	}
	else
	{
		if (rot[0][0] > rot[1][1] && rot[0][0] > rot[2][2])
		{
			float s = 2.0f * (float)sqrt(1.0f + rot[0][0] - rot[1][1] - rot[2][2]);
			Q.w = (rot[1][2] - rot[2][1]) / s;
			Q.x = 0.25f * s;
			Q.y = (rot[1][0] + rot[0][1]) / s;
			Q.z = (rot[2][0] + rot[0][2]) / s;
		}
		else if (rot[1][1] > rot[2][2])
		{
			float s = 2.0f * (float)sqrt(1.0f + rot[1][1] - rot[0][0] - rot[2][2]);
			Q.w = (rot[2][0] - rot[0][2]) / s;
			Q.x = (rot[1][0] + rot[0][1]) / s;
			Q.y = 0.25f * s;
			Q.z = (rot[2][1] + rot[1][2]) / s;
		}
		else
		{
			float s = 2.0f * (float)sqrt(1.0f + rot[2][2] - rot[0][0] - rot[1][1]);
			Q.w = (rot[0][1] - rot[1][0]) / s;
			Q.x = (rot[2][0] + rot[0][2]) / s;
			Q.y = (rot[1][2] + rot[2][1]) / s;
			Q.z = 0.25f * s;
		}
	}

	float length = (float)sqrt(Q.x * Q.x + Q.y * Q.y + Q.z * Q.z + Q.w * Q.w);
	Q.x /= length;
	Q.y /= length;
	Q.z /= length;
	Q.w /= length;

	return Q;
}
