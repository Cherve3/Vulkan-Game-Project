#include <string.h>

#include "simple_logger.h"
#include "gfc_matrix.h"

#include "gf3d_vgraphics.h"
#include "gf3d_camera.h"


static Camera camera;

void gf3d_camera_init()
{
	camera.view = gf3d_vgraphics_get_ubo_view();
	camera.position = vector3d(0, -10, -20);
	camera.forward = vector3d(0, 1, 0);
	camera.up = vector3d(0, 0, 1);
	camera.rotation = vector3d(0, 0, 0);

	vector3d_normalize(&camera.forward);
	vector3d_normalize(&camera.up);

	//Translate Camera Z
	//gfc_matrix_rotate(camera.view, camera.view, 90, vector_forward());
	gfc_matrix_new_translation(camera.view, camera.position);
	float angle = 180 * GFC_DEGTORAD;
	gfc_matrix_rotate(camera.view,camera.view,angle, vector_up());
	
}

void camera_update()
{
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
	}
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