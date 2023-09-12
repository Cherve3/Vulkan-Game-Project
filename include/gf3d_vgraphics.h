#ifndef __GF3D_VGRAPHICS_H__
#define __GF3D_VGRAPHICS_H__

#include <vulkan/vulkan.h>
#include "matrix.h"
#include "vector.h"

#include "gf3d_pipeline.h"
#include "gf3d_commands.h"

#define GF3D_VGRAPHICS_DISCRETE 1   //Choosing whether to use discrete [1] or integrated graphics [0]

/**
 * @brief init Vulkan / SDL, setup device and initialize infrastructure for 3d graphics
 * @param windowName Name of the program window
 * @param renderWidth screen resolutions width or the x direction
 * @param renderHeight screen resolutions height or y direction
 * @param bgColor dafault background color of the window
 * @param fullscreen to start the window in full screen or not
 * @param enableValidation toggle for Vulkans validation layers
 */
void gf3d_vgraphics_init(
    char *windowName,
    int renderWidth,
    int renderHeight,
    Vector4 bgcolor,
    Bool fullscreen,
    Bool enableValidation
);

/**
 * @brief Default logical device
 * @return the data that represents the logical device that interfaces with the physical device
 */
VkDevice gf3d_vgraphics_get_default_logical_device();

/**
 * @brief Default physical device aka the GPU
 * @return the data that represents the physical GPU hardware
 */
VkPhysicalDevice gf3d_vgraphics_get_default_physical_device();

/**
 * @brief Vulkan view extent should be the same as resolution
 * @return the data that represents the view extent
 */
VkExtent2D gf3d_vgraphics_get_view_extent();

void gf3d_vgraphics_clear();

void gf3d_vgraphics_render_start();
void gf3d_vgraphics_render_end();

int gf3d_vgraphics_create_buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer * buffer, VkDeviceMemory * bufferMemory);

void gf3d_vgraphics_copy_buffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

/**
 * @brief check if the device supports memory of the given types
 * @param typefilter a search critera
 * @param properties more search critera
 * @return 0 if there is no matching memory type supported, the memory type index otherwise
 */
uint32_t gf3d_vgraphics_find_memory_type(uint32_t typeFilter, VkMemoryPropertyFlags properties);

void gf3d_vgraphics_rotate_z(float degrees);
void gf3d_vgraphics_rotate_x(float degrees);
void gf3d_vgraphics_rotate_y(float degrees);

VkBuffer gf3d_vgraphics_get_uniform_buffer_by_index(Uint32 index);

MVPMatrix gf3d_vgraphics_get_uniform_buffer_object();

Matrix4D *gf3d_vgraphics_get_ubo_view();

Matrix4D *gf3d_vgraphics_get_ubo_model();

Matrix4D *gf3d_vgraphics_get_ubo_proj();

SDL_Window *gf3d_vgraphics_get_window();

/**
 * @brief get the pipeline that is used to render 3d modelsin the world
 * @return NULL on error or the pipeline in question
 */
Pipeline *gf3d_vgraphics_get_graphics_model_pipeline();

/**
 * @brief get the pipeline that is used to render 2d images to the overlay
 * @return NULL on error or the pipeline in question
 */
Pipeline *gf3d_vgraphics_get_graphics_overlay_pipeline();

/**
 * @brief get a command from the graphics command pool
 * @return NULL if none are left, or an empty command
 */
Command *gf3d_vgraphics_get_graphics_command_pool();

VkImageView gf3d_vgraphics_create_image_view(VkImage image, VkFormat format);

VkSurfaceKHR *gf3d_vgraphics_get_surface();

Uint32  gf3d_vgraphics_get_current_buffer_frame();

#endif
