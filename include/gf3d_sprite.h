#ifndef __GF3D_SPRITE_H__
#define __GF3D_SPRITE_H__

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"
#include "gfc_text.h"

#include "gf3d_texture.h"
#include "gf3d_mesh.h"

#define SPRITE_ATTRIBUTE_COUNT 2

/**
* @purpose the sprite is a 2D texture.  Each can be drawn individually in the rendering pipeline.
*/
typedef struct
{
	Uint8                       _inuse;
	TextLine                    filename;
	Texture                    *texture;
	int							frameWidth;
	int							frameHeight;
	Uint32						FramesPerLine;
	VkDescriptorSet            *descriptorSet;
	VkBuffer                   *uniformBuffer;
	VkDeviceMemory             *uniformBufferMemory;
	Uint32                      uniformBufferCount;
}Sprite;


void gf3d_sprite_manager_init(Uint32 max_sprites, Uint32 chain_length, VkDevice device);

void gf3d_sprite_manager_close();

Sprite *gf3d_sprite_get_by_filename(char *filename);

Sprite * gf3d_sprite_new();

/**
 * @brief load a sprite for rendering
 * @param filename the file location of the sprite being loaded
 * @param frame_width the width of a sprite frame (-1 for full texture)
 * @param frame_height the height of a sprite frame (-1 for full texture)
 * @param frames_per_line how many frames are there for the sprite
 * @return returns the loaded sprite
 */
Sprite * gf3d_sprite_load(char * filename, int frame_width, int frame_height, Uint32 frames_per_line);

void gf3d_sprite_free(Sprite *sprite);

/**
* @brief update the descriptorSet with the sprite data needed to submit the draw command for the sprite provided
* @param sprite the sprite data to populate the descriptor set with
* @param descriptSet the descriptorSet to populate
* @param chainIndex the swap chain frame to do this for
* @param spriteMat the matrix to transform the sprite by
*/
void gf3d_sprite_update_basic_descriptor_set(Sprite *sprite, VkDescriptorSet descriptorSet, Uint32 chainIndex, Matrix4 spriteMat, Uint32 frame);

void gf3d_sprite_delete(Sprite *sprite);

void gf3d_sprite_render(Sprite *sprite, VkCommandBuffer commandBuffer, VkDescriptorSet *descriptorSet);

void gf3d_sprite_draw(Sprite *sprite, Vector2D position, Vector2D scale, Uint32 frame, Uint32 bufferFrame, VkCommandBuffer commandBuffer);

VkVertexInputBindingDescription *gf3d_sprite_get_bind_description();

VkVertexInputAttributeDescription *gf3d_sprite_get_attribute_descriptions(Uint32 *count);

void gf3d_sprite_create_uniform_buffer(Sprite *sprite);

void gf3d_sprite_create_vertex_buffer(Sprite *sprite);

void gf3d_sprite_update_uniform_buffer(Sprite *sprite, uint32_t currentImage, Matrix4 spriteMat, Uint32 frame);


#endif