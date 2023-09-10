#include <string.h>

#include "simple_logger.h"
#include "gfc_types.h"
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
	camera.position = vector3d_create(0, 0, 0);
	camera.velocity = vector3d_create(0, 0, 0);
	camera.projVel = vector3d_create(0, 0, 0);
	camera.forward = vector3d_create(0, 0, -1);
	camera.up = vector3d_create(0, 1, 0);
	camera.right = vector3d_create(1, 0, 0);
	camera.rotation = vector3d_create(0, 0, 0);
	camera.offset = vector3d_create(0, 0, 0);
	camera.speed = 0.5;
	camera.zoom = 20;

	vector3d_normal(&camera.forward);
	vector3d_normal(&camera.up);
}

void gf3d_camera_update_velocity()
{
	const Uint8* keys;
	keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_W])
	{
		camera.velocity.z += camera.forward.z;
		slog("W: z: %.4f", camera.velocity.z);
	}
	if (keys[SDL_SCANCODE_A])
	{
		camera.velocity.x -= camera.right.x;
		slog("A: x:%.4f", camera.velocity.x);
	}
	if (keys[SDL_SCANCODE_S])
	{
		camera.velocity.z -= camera.forward.z;
		slog("S: z: %.4f", camera.velocity.z);
	}
	if (keys[SDL_SCANCODE_D])
	{
		camera.velocity.x += camera.right.x;
		slog("D: x:%.4f", camera.velocity.x);
	}
	if (keys[SDL_SCANCODE_Q])
	{
		camera.velocity.y -= camera.up.y;
		slog("Q: y:%.4f", camera.velocity.y);
	}
	if (keys[SDL_SCANCODE_E])
	{
		camera.velocity.y += camera.up.y;
		slog("E: y:%.4f", camera.velocity.y);
	}
}

void gf3d_camera_zoom()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_MOUSEWHEEL)
		{
			if (event.wheel.y > 0) // scroll up
			{
				camera.zoom -= event.wheel.y;

			}
			else if (event.wheel.y < 0) // scroll down
			{
				camera.zoom -= event.wheel.y;
			}
		}
	}

	if (camera.zoom > 200)
		camera.zoom = 200;
	if (camera.zoom < 2)
		camera.zoom = 2;
}

void gf3d_camera_update()
{
	const int x_rel, y_rel;
	SDL_GetRelativeMouseState(&x_rel, &y_rel);

	Matrix4D rotationX;
	Matrix4D rotationY;
	Matrix4D rotation;
	matrix4d_identity(&rotationX);
	matrix4d_identity(&rotationY);

	camera.pitch += y_rel*GFC_DEGTORAD;
	camera.yaw += x_rel *GFC_DEGTORAD;

	if (camera.pitch >= GFC_PI_HALFPI)
	{
		camera.pitch = GFC_PI_HALFPI;
	}
	else if (camera.pitch <= 0)
	{
		camera.pitch = 0;
	}
	if (camera.yaw >= GFC_2PI)
	{
		camera.yaw = 0;
	}
	
	if (camera.target)
	{	
		Vector2 cpos;
		float s = SDL_sinf(camera.yaw - 2.35619);
		float c = SDL_cosf(camera.yaw - 2.35619);

		gf3d_camera_zoom();
		cpos.x = ((-camera.zoom) * c) - ((-camera.zoom) * s);
		cpos.y = ((-camera.zoom) * s) + ((-camera.zoom) * c);

		gf3d_camera_look_at(
			vector3d_create(cpos.x + camera.target->position.x,  camera.target->position.y, cpos.y + camera.target->position.z),
			camera.target->position,
			camera.up);
	}
	else
	{
		gf3d_camera_update_velocity();
		gf3d_camera_move();
		vector3d_print(camera.velocity);
	}

	//matrix4d_rotate_arbitrary(camera.yaw, camera.up, &rotationX);
	//matrix4d_rotate_arbitrary(camera.pitch, camera.up, &rotationY);

	//matrix4d_multiply(rotationX, rotationY, &rotation);
	//matrix4d_multiply(rotation, camera.view, &camera.view);
	
	vector3d_zero(&camera.velocity);
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

Vector3 gf3d_camera_get_velocity()
{
	return camera.velocity;
}

Vector3 gf3d_camera_get_rotate()
{
	return camera.rotation;
}

void gf3d_camera_set_rotation(Vector3 rotation)
{
	camera.rotation.x = rotation.x;
	camera.rotation.y = rotation.y;
	camera.rotation.z = rotation.z;
}

void gf3d_camera_set_velocity(Vector3 velocity)
{
	camera.velocity.x = velocity.x;
	camera.velocity.y = velocity.y;
	camera.velocity.z = velocity.z;
}

void gf3d_camera_move()
{
	vector3d_addition(camera.position, camera.velocity, &camera.position);
 	matrix4d_translate(camera.velocity, camera.view);
}

void gf3d_camera_set_target_entity(Entity* target)
{
	if (!target) 
	{
		slog("Camera target is NULL");
		return;
	}
	camera.target = target;


}

/*eol@eof*/