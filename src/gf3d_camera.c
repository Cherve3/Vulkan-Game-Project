#include <string.h>

#include "simple_logger.h"
#include "gfc_matrix.h"
#include "gfc_vector.h"

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
	camera.position = vector3d(0, -10, 0);
	camera.forward = vector3d(0, 0, 1);
	camera.up = vector3d(0, 1, 0);
	camera.rotation = vector3d(0, 0, 0);
	camera.offset = vector3d(0, -10, -5);
	camera.speed = 0.5;

	vector3d_normalize(&camera.forward);
	vector3d_normalize(&camera.up);

		
}

void gf3d_camera_update(Vector3D pos, Vector3D rotate)
{
	float s = SDL_sinf(rotate.z - GFC_PI);
	float c = SDL_cosf(rotate.z - GFC_PI);
	Vector2D cpos;
	gfc_matrix_identity(camera.view);

	cpos.x = ((-20) * c) - ((-20) * s);
	cpos.y = ((-20) * s) + ((-20) * c);


	if (camera_height > 0 && rotate.y > 0)
	{
		camera_height -= rotate.y;
	}
	else if (camera_height < 15 && rotate.y < 0)
	{
		camera_height -= rotate.y;
	}

//	slog("Camera height %f", camera_height);
//	slog("cpos.x = %f  cpos.y = %f", cpos.x, cpos.y);
//	slog("rotate.y + %f", rotate.y);

	gf3d_camera_look_at(
		vector3d(cpos.x + pos.x, cpos.y + pos.y, pos.z + camera_height),
		pos,
		vector3d(0, 1, 0));

}
void camera_update(Vector2D forward, Vector3D position, Vector3D rotate, const int x_rel, const int y_rel)
{	//Take 1
	//gfc_matrix_new_translation(camera.view, camera.position);
	//gfc_matrix_rotate(camera.view, camera.view, -x_rel*GFC_DEGTORAD, camera.up);

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

	rotate.z += camera.yaw * GFC_DEGTORAD;

	//gfc_matrix_rotate(gf3d_get_camera(), gf3d_get_camera(), -camera.yaw*GFC_DEGTORAD, vector3d(0, 1, 0));
	forward.x = (0 * SDL_cosf(rotate.z)) - (1 * SDL_sinf(rotate.z));
	forward.y = (0 * SDL_sinf(rotate.z)) + (1 * SDL_cosf(rotate.z));

	float s = SDL_sinf(rotate.z - GFC_PI);
	float c = SDL_cosf(rotate.z - GFC_PI);

	Vector2D cpos;
	gfc_matrix_identity(camera.view);

	cpos.x = ((-20) * c) - ((-20) * s);
	cpos.y = ((-20) * s) + ((-20) * c);

	gf3d_camera_look_at(
		vector3d(cpos.x + position.x, cpos.y + position.y, position.z + camera_height),
		position,
		vector3d(0, 1, 0));


	/*Take 2
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

	*/

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