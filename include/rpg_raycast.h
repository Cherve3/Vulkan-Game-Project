#ifndef __RPG_RAYCAST_H__
#define __RPG_RAYCAST_H__

#include "gfc_vector.h"

typedef struct
{
	Vector3 origin;
	Vector3 direction;
	float maxDistance;
}Raycast;

Raycast *raycast(Vector3 origin, Vector3 direction, float maxDistance);

#endif
