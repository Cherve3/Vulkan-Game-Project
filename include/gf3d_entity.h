#ifndef __GF3D_ENTITY_H__
#define __GF3D_ENTITY_H__

#include "gfc_types.h"
#include "gfc_matrix.h"

#include "gf3d_model.h"

/**
 *	@purpose this is the file to access the entity management system
 */

typedef struct Entity_S
{
	Uint8		_inuse;		/**<flag to make sure entities are not re-assigned while active*/
	Model		*model;
	Matrix4		modelMatrix;
	void		(*think)(struct Entity_S *self);
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
 *	@param bufferFrame the renderFrame to draw with
 *	@param commandBuffer the command to populate with this draw command
 */
void gf3d_entity_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer);

/**
 *	@brief free an entity so it can be reused by the system
 *	@param self the entity to free
 *	@note the pointer should be set to zero and no longer used.
 */
void gf3d_entity_free(Entity *self);

#endif
