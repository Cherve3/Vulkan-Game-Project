#include "rpg_projectile.h"

void rpg_fireball_spawn(Vector3D position)
{
	Fireball fireball;
	fireball.ent = gf3d_entity_new();
	fireball.ent->damage = 10;
	fireball.ent->position.x = position.x;
	fireball.ent->position.y = position.y + 1;
	fireball.ent->position.z = position.z + 2;
	fireball.ent->think = rpg_fireball_think;
	fireball.collider.radius = 1;
	fireball.collider.x = position.x;
	fireball.collider.y = position.y;
	fireball.collider.z = position.z;
	
}

void rpg_fireball_despawn()
{

}

void rpg_fireball_think(Entity *self)
{
	if (!self)return;
	self->velocity = vector3d(0, 0, 1);
	vector3d_add(self->position,self->position, self->velocity);
	
}