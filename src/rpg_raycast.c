#include "gf3d_entity.h"

#include "rpg_raycast.h"
#include "simple_logger.h"

Raycast *raycast(Vector3 origin, Vector3 direction, float maxDistance)
{
	Raycast *ray;
	ray->origin = origin;
	ray->direction = direction;
	ray->maxDistance = maxDistance;

	return ray;
}

Bool isColliding(Raycast ray, Entity *ent)
{
	slog("Checking ray collision");
	

	return false;
}

/*eol@eof*/
