#include <string.h>

#include "simple_logger.h"
#include "gfc_types.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_vgraphics.h"
#include "gf3d_camera.h"


static Camera camera;

static float camera_height = 10;
static float camera_distance = 10;

void gf3d_camera_init()
{
	gfc_matrix_copy(camera.proj, gf3d_vgraphics_get_uniform_buffer_object().proj);
	gfc_matrix_copy(camera.model, gf3d_vgraphics_get_uniform_buffer_object().model);
	gfc_matrix_copy(camera.view, gf3d_vgraphics_get_uniform_buffer_object().view);
	camera.position = vector3d(0, 0, 0);
	camera.velocity = vector3d(0, 0, 0);
	camera.projVel = vector3d(0, 0, 0);
	camera.forward = vector3d(0, 0, -1);
	camera.up = vector3d(0, 1, 0);
	camera.right = vector3d(1, 0, 0);
	camera.rotation = vector3d(0, 0, 0);
	camera.offset = vector3d(0, 0, 0);
	camera.speed = 0.5;
	camera.zoom = 20;

	vector3d_normalize(&camera.forward);
	vector3d_normalize(&camera.up);
	vector3d_normalize(&camera.right);
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
	if (camera.zoom < 5)
		camera.zoom = 5;
}

void gf3d_camera_get_view_mat4(Matrix4* view)
{
	if (!view)return;
	memcpy(view, camera.view, sizeof(Matrix4));
}

void gf3d_camera_update()
{
	//const int x_rel, y_rel;
	//SDL_GetRelativeMouseState(&x_rel, &y_rel);

	//Matrix4 rotationX;
	//Matrix4 rotationY;
	//Matrix4 rotation;
	//gfc_matrix_identity(&rotationX);
	//gfc_matrix_identity(&rotationY);

	//camera.pitch += y_rel*GFC_DEGTORAD;
	//camera.yaw += x_rel *GFC_DEGTORAD;

	//if (camera.pitch >= GFC_PI_HALFPI)
	//{
	//	camera.pitch = GFC_PI_HALFPI;
	//}
	//else if (camera.pitch <= 0)
	//{
	//	camera.pitch = 0;
	//}
	//if (camera.yaw >= GFC_2PI)
	//{
	//	camera.yaw = 0;
	//}
	//
	//if (camera.target)
	//{	
	//	Vector2D cpos;
	//	float s = SDL_sinf(camera.yaw - 2.35619);
	//	float c = SDL_cosf(camera.yaw - 2.35619);

	//	gf3d_camera_zoom();
	//	cpos.x = ((-camera.zoom) * c) - ((-camera.zoom) * s);
	//	cpos.y = ((-camera.zoom) * s) + ((-camera.zoom) * c);

	//	gf3d_camera_look_at(
	//		vector3d(cpos.x + camera.target->position.x,  camera.target->position.y, cpos.y + camera.target->position.z),
	//		camera.target->position,
	//		camera.up);
	//}
	//else
	//{
	//	gf3d_camera_update_velocity();
	//	gf3d_camera_move();
	//	//gfc_vector3d_print(camera.velocity);
	//}

	////matrix4d_rotate_arbitrary(camera.yaw, camera.up, &rotationX);
	////matrix4d_rotate_arbitrary(camera.pitch, camera.up, &rotationY);

	////matrix4d_multiply(rotationX, rotationY, &rotation);
	////matrix4d_multiply(rotation, camera.view, &camera.view);
	//
	//vector3d_clear(camera.velocity);
	////matrix4d_print(camera.view);

	Vector3D xaxis, yaxis, zaxis, position;
	float cosPitch = cos(camera.rotation.x);
	float sinPitch = sin(camera.rotation.x);
	float cosYaw = cos(camera.rotation.z);
	float sinYaw = sin(camera.rotation.z);

	position.x = camera.position.x;
	position.y = -camera.position.z;        //inverting for Z-up
	position.z = camera.position.y;
	gfc_matrix_identity(camera.view);

	vector3d_set(xaxis, cosYaw, 0, -sinYaw);
	vector3d_set(yaxis, sinYaw * sinPitch, cosPitch, cosYaw * sinPitch);
	vector3d_set(zaxis, sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw);

	camera.view[0][0] = xaxis.x;
	camera.view[0][1] = yaxis.x;
	camera.view[0][2] = zaxis.x;

	camera.view[1][0] = xaxis.z;
	camera.view[1][1] = yaxis.z;
	camera.view[1][2] = zaxis.z;

	camera.view[2][0] = -xaxis.y;
	camera.view[2][1] = -yaxis.y;
	camera.view[2][2] = -zaxis.y;

	camera.view[3][0] = vector3d_dot_product(xaxis, position);
	camera.view[3][1] = vector3d_dot_product(yaxis, position);
	camera.view[3][2] = vector3d_dot_product(zaxis, position);
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
		up);
}

Vector3D gf3d_camera_get_position()
{
	return camera.position;
}

void gf3d_camera_set_position(Vector3D position)
{
	camera.position.x = -position.x;
	camera.position.y = -position.y;
	camera.position.z = -position.z;
}

Vector3D gf3d_camera_get_velocity()
{
	return camera.velocity;
}

Vector3D gf3d_camera_get_rotate()
{
	return camera.rotation;
}

void gf3d_camera_set_rotation(Vector3D rotation)
{
	camera.rotation.x = -rotation.x;
	camera.rotation.y = -rotation.y;
	camera.rotation.z = -rotation.z;
}

void gf3d_camera_set_velocity(Vector3D velocity)
{
	camera.velocity.x = velocity.x;
	camera.velocity.y = velocity.y;
	camera.velocity.z = velocity.z;
}

void gf3d_camera_move()
{
	vector3d_add(camera.position, camera.velocity, camera.position);
 	gfc_matrix_translate(camera.view, camera.velocity);
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