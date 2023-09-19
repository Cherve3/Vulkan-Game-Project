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
	camera.position = vector3d(0, 0, 0);
	camera.velocity = vector3d(0, 0, 0);
	camera.forward	= vector3d(0, 0, -1);
	camera.up		= vector3d(0, 1, 0);
	camera.right	= vector3d(1, 0, 0);
	camera.rotation = vector3d(0, 0, 0);
	camera.speed	= 0.5;
	camera.zoom		= 20;

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
		camera.velocity.z += -1;
		slog("W: z: %.4f", camera.velocity.z);
	}
	if (keys[SDL_SCANCODE_A])
	{
		camera.velocity.x += 1;
		slog("A: x:%.4f", camera.velocity.x);
	}
	if (keys[SDL_SCANCODE_S])
	{
		camera.velocity.z += 1;
		slog("S: z: %.4f", camera.velocity.z);
	}
	if (keys[SDL_SCANCODE_D])
	{
		camera.velocity.x += -1;
		slog("D: x:%.4f", camera.velocity.x);
	}
	if (keys[SDL_SCANCODE_Q])
	{
		camera.velocity.y += 1;
		slog("Q: y:%.4f", camera.velocity.y);
	}
	if (keys[SDL_SCANCODE_E])
	{
		camera.velocity.y += -1;
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
	memcpy(view, camera.camMatrix, sizeof(Matrix4));
}

void gf3d_camera_set_view_mat4(Matrix4* view)
{
	if (!view)return;
	memcpy(camera.camMatrix, view, sizeof(Matrix4));
}

void gf3d_camera_update()
{		
	Vector3D position; 
	gf3d_camera_move();
	position.x = camera.position.x;
	position.y = -camera.position.y;
	position.z = camera.position.z;
	gfc_matrix_identity(camera.camMatrix);

	if (camera.target)
	{
		gf3d_camera_look_at(camera.position, camera.target->position, vector3d(0,1,0));
	}
	else
	{
		Vector3D xaxis, yaxis, zaxis;

		float cosPitch = cos(camera.rotation.x);
		float sinPitch = sin(camera.rotation.x);
		float cosYaw = cos(camera.rotation.z);
		float sinYaw = sin(camera.rotation.z);

		if (cosPitch >= 0)
			cosPitch == 0;

		vector3d_set(xaxis, cosYaw, 0, -sinYaw);
		vector3d_set(yaxis, sinYaw * sinPitch, cosPitch, cosYaw * sinPitch);
		vector3d_set(zaxis, sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw);

		camera.camMatrix[0][0] = xaxis.x;
		camera.camMatrix[0][1] = yaxis.x;
		camera.camMatrix[0][2] = zaxis.x;

		camera.camMatrix[1][0] = xaxis.y;
		camera.camMatrix[1][1] = yaxis.y;
		camera.camMatrix[1][2] = zaxis.y;

		camera.camMatrix[2][0] = -xaxis.z;
		camera.camMatrix[2][1] = -yaxis.z;
		camera.camMatrix[2][2] = -zaxis.z;

		camera.camMatrix[3][0] = vector3d_dot_product(xaxis, position);
		camera.camMatrix[3][1] = vector3d_dot_product(yaxis, position);
		camera.camMatrix[3][2] = vector3d_dot_product(zaxis, position);
	}
	gf3d_camera_print(camera.camMatrix);
	slog("\nCamera Position: %.4f, %.4f, %.4f\n", camera.position.x, camera.position.y, camera.position.z);
	vector3d_clear(camera.velocity);
}

Matrix4 *gf3d_get_camera()
{
	return camera.camMatrix;
}

void gf3d_camera_look_at(Vector3D position, Vector3D target, Vector3D up)
{
	gfc_matrix_view(
		camera.camMatrix,
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
	camera.velocity.x = -velocity.x;
	camera.velocity.y = -velocity.y;
	camera.velocity.z = -velocity.z;
}

void gf3d_camera_move()
{
	vector3d_add(camera.position, camera.position, camera.velocity);
 	gfc_matrix_translate(camera.camMatrix, camera.velocity);
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

void gf3d_camera_set_scale(Vector3D scale)
{
	if (!scale.x)camera.scale.x = 0;
	else camera.scale.x = 1 / scale.x;
	if (!scale.y)camera.scale.y = 0;
	else camera.scale.y = 1 / scale.y;
	if (!scale.z)camera.scale.z = 0;
	else camera.scale.z = 1 / scale.z;
}

gf3d_camera_print(Matrix4 mat)
{
	printf("\n\n[%.4f][%.4f][%.4f][%.4f]\n[%.4f][%.4f][%.4f][%.4f]\n", 
		mat[0][0],mat[0][1], mat[0][2], mat[0][3], mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
	printf("[%.4f][%.4f][%.4f][%.4f]\n[%.4f][%.4f][%.4f][%.4f]",
		mat[2][0], mat[2][1], mat[2][2], mat[2][3], mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}

/*eol@eof*/