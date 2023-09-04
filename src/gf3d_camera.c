#include <string.h>

#include "simple_logger.h"
#include "vector.h"
#include "matrix.h"

#include "gf3d_vgraphics.h"
#include "gf3d_camera.h"


static Camera camera;

static float camera_height = 10;
static float camera_distance = 10;

void gf3d_camera_init()
{
	camera.proj = gf3d_vgraphics_get_ubo_proj();
	camera.model = gf3d_vgraphics_get_ubo_model();
	camera.view = gf3d_vgraphics_get_ubo_view();
	camera.position = vector3d_create(0, -10, 0);
	camera.forward = vector3d_create(0, 0, 1);
	camera.up = vector3d_create(0, 1, 0);
	camera.rotation = vector3d_create(0, 0, 0);
	camera.offset = vector3d_create(0, -10, -5);
	camera.speed = 0.5;
	camera.zoom = 20;

	vector3d_normal(&camera.forward);
	vector3d_normal(&camera.up);
}

void camera_update(Vector3 position, Vector3 rotate, const int x_rel, const int y_rel)
{	//Take 1
	//gfc_matrix_new_translation(camera.view, camera.position);
	//gfc_matrix_rotate(camera.view, camera.view, -x_rel*GFC_DEGTORAD, camera.up);

//	camera.yaw += x_rel;
//	camera.pitch += y_rel;
	/*
	if (camera.pitch > 89.0f)
		camera.pitch = 89.0f;
	if (camera.pitch < -60.0f)
		camera.pitch = -60.0f;

	if (camera.yaw >= 360.0f)
		camera.yaw = 0;
	if (camera.yaw <= -360.0f)
		camera.yaw = 0;
*/
//	rotate.z += camera.yaw * GFC_DEGTORAD;

//	gfc_matrix_rotate(gf3d_get_camera(), gf3d_get_camera(), -camera.yaw*GFC_DEGTORAD, vector3d(0, 1, 0));
	
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_MOUSEWHEEL)
		{
			if (event.wheel.y > 0) // scroll up
			{
				camera.zoom += event.wheel.y;
				if (camera.zoom == 0)
					camera.zoom = 1;

			}
			else if (event.wheel.y < 0) // scroll down
			{
				camera.zoom += event.wheel.y;
				if (camera.zoom == 0)
					camera.zoom = -1;
			}
		}
	}

	if (camera.zoom > 20)
		camera.zoom = 20;
	if (camera.zoom < -20)
		camera.zoom = -20;

	camera.yaw += x_rel*GFC_DEGTORAD;
	camera.pitch += y_rel *GFC_DEGTORAD;

	float s = SDL_sinf(camera.yaw - 2.35619);
	float c = SDL_cosf(camera.yaw - 2.35619);

	Vector2 cpos;
	cpos.x = ((-camera.zoom) * c) - ((-camera.zoom) * s);
	cpos.y = ((-camera.zoom) * s) + ((-camera.zoom) * c);
	
	matrix4d_look_at(
		vector3d_create(cpos.x + position.x, 15 + position.y, cpos.y + position.z),
		vector3d_create(position.x,position.y,position.z),
		vector3d_create(0, 1, 0), 
		camera.view);

	matrix4d_print(camera.view);
}

Matrix4D *gf3d_get_camera()
{
	return camera.view;
}

void gf3d_camera_look_at(Vector3 position, Vector3 target, Vector3 up)
{
	matrix4d_look_at(
		position,
		target,
		up,
		camera.view);
}

Vector3 gf3d_camera_get_position()
{
	return camera.position;
}

void gf3d_camera_set_position(Vector3 position)
{
	camera.position.x = position.x;
	camera.position.y = position.y;
	camera.position.z = position.z;
}

void gf3d_camera_move(Vector3 move)
{
	vector3d_addition(camera.position, move, &camera.position);
 	matrix4d_translate(move, camera.view);
}

/*eol@eof*/