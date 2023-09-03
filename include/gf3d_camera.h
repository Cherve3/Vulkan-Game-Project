#ifndef __GF3D_CAMERA_H__
#define __GF3D_CAMERA_H__

#include "matrix.h"
#include "vector.h"

typedef struct
{
	Matrix4D *proj;
	Matrix4D *model;
	Matrix4D *view;
	Vector3 position;
	Vector3 forward;
	Vector3 up;
	Vector3 right;
	Vector3 offset;
	Vector3 rotation;

	float yaw;
	float pitch;
	float speed;

	int zoom;
}Camera;

void gf3d_camera_init();

void camera_update(Vector3 Position, Vector3 rotation, const int x_rel, const int y_rel);
//void camera_update(Vector3 rotation, Vector3 playerPosition, Matrix4 playerMat);

Matrix4D *gf3d_get_camera();

/**
* @brief get the current camera view
* @param view output, the matrix provided will be populated with the current camera information
*/
void gf3d_camera_get_view(Matrix4D view);

/**
* @brief set the current camera based on the matrix provided
*/
void gf3d_camera_set_view(Matrix4D view);


Vector3 gf3d_camera_get_position();
/**
* @brief set the camera properties based on position and direction that the camera should be looking
* @param position the location for the camera
* @param target the point the camera should be looking at
* @param up the direction considered to be "up"
*/
void gf3d_camera_look_at(
	Vector3 position,
	Vector3 target,
	Vector3 up
	);

/**
* @brief explicitely set the camera positon, holding all other parameters the same
* @param position the new position for the camera
*/
void gf3d_camera_set_position(Vector3 position);

/**
* @brief move the camera relatively based on the vector provided
* @param move the ammount to move the camera
*/
void gf3d_camera_move(Vector3 move);

void gf3d_camera_update(Vector3 pos, Vector3 rotate);

#endif
