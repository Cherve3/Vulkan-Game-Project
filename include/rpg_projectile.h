#ifndef __RPG_PROJECTILE_H__
#define __RPG_PROJECTILE_H__

#include "gfc_types.h"

#include "gf3d_entity.h"

#include "rpg_collision.h"

typedef enum{
	Wind,
	Fire,
	Ice,
	Lightning,
	Water,
	Earth,
	Light,
	Dark
}Element;

typedef enum{
	ThrowItem,
	Weapon,
	Magic
}ProjectileType;

typedef struct
{
	Uint8 _inuse;
	Element element;
	ProjectileType type;
	SphereCollider collider;
	int damage;
	float cooldown;
	Entity *ent;
	Entity *owner;
}Projectile;

void rpg_chest_loot_free(Projectile *proj);

void rpg_chests_free(Projectile *proj);

void rpg_projectiles_close();

void rpg_projectile_init();

void rpg_fireball_spawn(Entity *player);

void rpg_despawn();

void rpg_fireball_think(Entity *self);

void rpg_fireball_update(Entity *self);

#endif