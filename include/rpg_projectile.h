#ifndef __RPG_PROJECTILE_H__
#define __RPG_PROJECTILE_H__

#include "gfc_types.h"

#include "gf3d_entity.h"

#include "rpg_collision.h"

typedef struct
{
	SphereCollider collider;
	Entity *ent;
}Fireball;

void rpg_fireball_spawn(Vector3D position);

void rpg_despawn();

void rpg_fireball_think();

#endif