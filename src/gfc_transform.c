
#include "gfc_matrix.h"
#include "gfc_transform.h"
#include "gfc_quaternion.h"
#include "gfc_vector.h"

#include "gf3d_entity.h"


Transform update_transform(Transform transform)
{
	if (!vector3d_equal(transform.oldPos, vector3d(0, 0, 0)))
	{
		transform.oldPos = transform.position;
		transform.oldRot = transform.rotation;
		transform.oldScale = transform.scale;
	}
	else
	{
		vector3d_add(transform.oldPos, transform.position, vector3d(1,1,1));

		
		quaternion_vector_multiply(&transform.oldRot,
			quaternion(0,0,0,0), vector3d(0.5,0.5,0.5) );


		vector3d_add(transform.oldScale, transform.scale, vector3d(1, 1, 1));
	}
}

Quaternion rotate_transform(Quaternion rotation, Vector3D axis, float angle)
{
	Quaternion temp;
	quaternion_init(&temp, axis, angle);
	quaternion_multiply(&temp, temp, rotation);
	quaternion_normalize(&temp);

	return temp;
}

Quaternion get_look_rotation(Vector3D position, Vector3D point, Vector3D up)
{
	point.x -= position.x;
	point.y -= position.y;
	point.z -= position.z;

	vector3d_normalize(&point);

	return quaternion_from_matrix(gfc_matrix_rotation(point, up), up);
}

int has_changed(Transform transform)
{
	if (!vector3d_equal(transform.oldPos, transform.position))
		return true;

	if (!quaternion_equal(transform.oldRot, transform.rotation))
		return true;

	if (!vector3d_equal(transform.oldScale, transform.scale))
		return true;

	return false;
}

Matrix4 *get_transformation(Transform transform)
{
	Matrix4 temp, position, *rotation, scale;
	gfc_matrix_identity(position);
	gfc_matrix_identity(temp);
	gfc_matrix_identity(rotation);
	gfc_matrix_identity(scale);

	position[3][0] = transform.position.x;
	position[3][1] = transform.position.y;
	position[3][2] = transform.position.z;

	rotation = quaternion_to_matrix(transform.rotation);

	gfc_matrix_init_scale(scale, transform.scale);
	 
	gfc_matrix_multiply(temp, position, rotation);
	gfc_matrix_multiply(temp,temp, scale);

	return temp;

}
