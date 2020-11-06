#ifndef __GF3D_CAMERA_H__
#define __GF3D_CAMERA_H__

#include "gfc_matrix.h"

typedef struct
{
	Matrix4 *view;
	Vector3D position;
	Vector3D forward;
	Vector3D up;
	Vector3D offset;
	Vector3D rotation;
	
}Camera;

void gf3d_camera_init();

void camera_update();

Matrix4 *gf3d_get_camera();

/**
* @brief get the current camera view
* @param view output, the matrix provided will be populated with the current camera information
*/
void gf3d_camera_get_view(Matrix4 view);

/**
* @brief set the current camera based on the matrix provided
*/
void gf3d_camera_set_view(Matrix4 view);


Vector3D gf3d_camera_get_position();
/**
* @brief set the camera properties based on position and direction that the camera should be looking
* @param position the location for the camera
* @param target the point the camera should be looking at
* @param up the direction considered to be "up"
*/
void gf3d_camera_look_at(
	Vector3D position,
	Vector3D target,
	Vector3D up
	);

/**
* @brief explicitely set the camera positon, holding all other parameters the same
* @param position the new position for the camera
*/
void gf3d_camera_set_position(Vector3D position);

/**
* @brief move the camera relatively based on the vector provided
* @param move the ammount to move the camera
*/
void gf3d_camera_move(Vector3D move);

Vector3D *getLeft();
Vector3D *getRight();

void gf3d_camera_rotate_x(Camera *camera, float angle);
void gf3d_camera_rotate_y(Camera *camera, float angle);


void camera_rotate(Vector3D axis, float angle);

#endif
