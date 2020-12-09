#include <string.h>

#include "simple_logger.h"
#include "gfc_matrix.h"
#include "gfc_vector.h"

#include "gf3d_vgraphics.h"
#include "gf3d_camera.h"


static Camera camera;

void gf3d_camera_init()
{
	camera.proj = gf3d_vgraphics_get_ubo_proj();
	camera.model = gf3d_vgraphics_get_ubo_model();
	camera.view = gf3d_vgraphics_get_ubo_view();
	camera.position = vector3d(0, -10, 0);
	camera.forward = vector3d(0, 0, 1);
	camera.up = vector3d(0, 1, 0);
	camera.rotation = vector3d(0, 0, 0);
	camera.offset = vector3d(0, -10, -5);
	camera.speed = 0.5;

	vector3d_normalize(&camera.forward);
	vector3d_normalize(&camera.up);

		
}

void camera_update(Vector3D rotation, Vector3D playerPosition, Matrix4 playerMat, const int x_rel, const int y_rel)
{
	//gfc_matrix_new_translation(camera.view, camera.position);
	//gfc_matrix_rotate(camera.view, camera.view, -x_rel*GFC_DEGTORAD, camera.up);
	
	Vector3D negate;
	vector3d_negate(negate, camera.position);

	camera.yaw += x_rel;
	camera.pitch += y_rel;
	
	if (camera.pitch > 89.0f)
		camera.pitch = 89.0f;
	if (camera.pitch < -60.0f)
		camera.pitch = -60.0f;
		
	if (camera.yaw >= 360.0f)
		camera.yaw = 0;
	if (camera.yaw <= -360.0f)
		camera.yaw = 0;
	//slog("\nYAW: %f\nPitch: %f", camera.yaw, camera.pitch);
	
	gfc_matrix_new_translation(camera.view, vector3d(0,-8,0));
	//gf3d_camera_look_at(camera.position,playerPosition,camera.up);
	gfc_matrix_rotate(camera.view, camera.view, camera.pitch*GFC_DEGTORAD, vector_right());
	gfc_matrix_rotate(camera.view, camera.view, camera.yaw*GFC_DEGTORAD, camera.up);
	
	camera.position.x = -playerPosition.x;
	camera.position.y = -playerPosition.y - 10;
	camera.position.z = -playerPosition.z - 20;
	gfc_matrix_translate(camera.view,camera.position);

	

	/*
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_MOUSEWHEEL)
		{
			if (event.wheel.y > 0) // scroll up
			{
				gf3d_camera_move(vector3d(0, 0, event.wheel.y));
			}
			else if (event.wheel.y < 0) // scroll down
			{
				gf3d_camera_move(vector3d(0, 0, event.wheel.y));
			}
		}
	}*/
}

Matrix4 *gf3d_get_camera()
{
	return camera.view;
}

void gf3d_camera_look_at(Vector3D position, Vector3D target, Vector3D up)
{
	gfc_matrix_view(
		camera.view,
		position,
		target,
		up
		);
}

Vector3D gf3d_camera_get_position()
{
	return camera.position;
}

void gf3d_camera_set_position(Vector3D position)
{
	camera.position.x = position.x;
	camera.position.y = position.y;
	camera.position.z = position.z;
}

void gf3d_camera_move(Vector3D move)
{
	vector3d_add(camera.position, camera.position, move);
	gfc_matrix_translate(camera.view, move);
}

/*eol@eof*/