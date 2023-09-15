#ifndef __GF3D_ENTITY_H__
#define __GF3D_ENTITY_H__

#include "gfc_types.h"
#include "gfc_matrix.h"

#include "gf3d_model.h"

#include "rpg_collision.h"

/**
 *	@purpose this is the file to access the entity management system
 */

typedef enum{
	ITEM,
	INTERACT,
	PICKUP,
	PLAYER,
	NONPLAYER,
	MONSTER,
	WORLD,
	STRUCTURE,
	PROJECTILE
}EntityType;

typedef struct Entity_S
{
	Uint8		_inuse;		/**<flag to make sure entities are not re-assigned while active*/
	Uint8		hidden;
	char *		name;
	struct Entity_S   *parent;
	EntityType	type;

	Vector3D	position;
	Vector3D	rotation;
	Vector3D	direction;
	Vector3D	velocity;
	float	rotate;
	Vector2D	forward;

	Model		*model;
	Matrix4	modelMatrix;
	Color		color;
	BoxCollider boxCollider;

	Bool		animated;

	void		(*update)	(struct Entity_S *self);
	void		(*think)	(struct Entity_S *self);
	void		(*interact) (struct Entity_S *self);
	void		(*touch)	(struct Entity_S *self, struct Entity_S *other);
	void		(*damage)	(struct Entity_S *self);
	void		(*die)		(struct Entity_S *self);

	void *data;		// Expecting typedef Struct of entity (ex. Item, Goblin, etc.)

}Entity;

/**
 *	@brief initializes the internal structures for managing the entity system
 *	@param maxEntites this will be the maximum number of concurrent entities to be supported
 */
void gf3d_entity_init(Uint32 maxEntites);

/**
 *	@brief search for an empty entity slot and return a pointer to the blank entity
	@return NULL on error or a pointer to an entity otherwise
 */
Entity *gf3d_entity_new();

/**
 *	@brief queues up the entity to be drawn for the given draw command
 *	@param self the entity to draw
 *	@param bufferFrame the renderFrame to draw with
 *	@param commandBuffer the command to populate with this draw command
 */
void gf3d_entity_draw(Entity *self, Uint32 bufferFrame, VkCommandBuffer commandBuffer);

/**
 *	@brief draw all active entities for a draw call
 */
void gf3d_entity_draw_all();

/**
*	@brief call update function for all active entities
*/
void gf3d_entity_update_all();

/**
 *	@brief call interact function for active entity
 */
void gf3d_entity_interact(Entity *self);

/**
*	@brief call touch function for active entity
*/
void gf3d_entity_touch(Entity *self);

/**
*	@brief call damage function for active entity
*/
void gf3d_entity_damage(Entity *self);

/**
*	@brief call die function for active entity
*/
void gf3d_entity_die(Entity *self);

/**
 *	@brief free an entity so it can be reused by the system
 *	@param self the entity to free
 *	@note the pointer should be set to zero and no longer used.
 */
void gf3d_entity_free(Entity *self);

/**
 *	@brief check if the entity collided with another entity
 *	@param self the entity colliding
 */
void gf3d_entity_collision_test(Entity *self);

/**
*	@brief get the list of all entities 
*	@return a pointer to entity list
*/
Entity* gf3d_get_entity_list();

int gf3d_get_entity_list_count();

#endif
