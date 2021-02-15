#ifndef __RPG_RAYCAST_H__
#define __RPG_RAYCAST_H__

#include "gfc_vector.h"

typedef struct
{
	Vector3D origin;
	Vector3D direction;
	float maxDistance;
}Raycast;

Raycast *raycast(Vector3D origin, Vector3D direction, float maxDistance);

#endif
