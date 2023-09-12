#include <assert.h>
#include <stdio.h>

#include "game.h"

#include "gf3d_model.h"
#include "gf3d_swapchain.h"
#include "gf3d_commands.h"
#include "gf3d_vgraphics.h"
#include "gf3d_obj_load.h"

typedef struct
{
    Model               *   model_list;
    Uint32                  max_models;
    Uint32                  chain_length;   /**<length of swap chain*/
    VkDevice                device;
    Pipeline            *   pipe;           /**<the pipeline associated with model rendering*/
}ModelManager;

static ModelManager gf3d_model = {0};

char file_path[60];

void gf3d_model_delete(Model* model);
void gf3d_model_update_basic_model_descriptor_set(Model* model, VkDescriptorSet descriptorSet, Uint32 chainIndex, Matrix4D modelMat);

void gf3d_model_manager_close()
{
    int i;
    for (i = 0; i < gf3d_model.max_models;i++)
    {
        gf3d_model_delete(&gf3d_model.model_list[i]);
    }
    if (gf3d_model.model_list)
    {
        free(gf3d_model.model_list);
    }
    memset(&gf3d_model,0,sizeof(ModelManager));
    slog("model manager closed");
}

void gf3d_model_manager_init(Uint32 max_models)
{
    if (max_models == 0)
    {
        slog("cannot initialize model manager with 0 models");
        return;
    }
    gf3d_model.chain_length = gf3d_swapchain_get_chain_length();
    gf3d_model.model_list = (Model *)gfc_allocate_array(sizeof(Model),max_models);
    gf3d_model.max_models = max_models;
    gf3d_model.device = gf3d_vgraphics_get_default_logical_device();
    gf3d_model.pipe = gf3d_vgraphics_get_graphics_model_pipeline();
    
    slog("model manager initilized");
    atexit(gf3d_model_manager_close);
}

void gf3d_model_delete(Model *model)
{
	int i;
	if (!model)return;
    if (!model->_inuse)return;

	gf3d_mesh_free(model->mesh);
	gf3d_texture_free(model->texture);
	memset(model, 0, sizeof(Model));
}

Model * gf3d_model_new()
{
    int i;
    for (i = 0; i < gf3d_model.max_models;i++)
    {
        if (!gf3d_model.model_list[i]._inuse)
        {
            gf3d_model_delete(&gf3d_model.model_list[i]);
            gf3d_model.model_list[i]._inuse = 1;
            return &gf3d_model.model_list[i];
        }
    }
    slog("unable to make a new model, out of space");
    return NULL;
}

Model * gf3d_model_load_animated(char * filename, Uint32 startFrame, Uint32 endFrame)
{
	TextLine assetname;
	Model *model;
	model = gf3d_model_new();
	int i, count;
	if (!model)return NULL;
	count = endFrame - startFrame;
	if (count <= 0)
	{
		gf3d_model_free(model);
		slog("frameRange is zero or negative: %i", count);
		return NULL;
	}
	model->frameCount = count;
	model->mesh = (Mesh**)gfc_allocate_array(sizeof(Mesh*), count);
	if (!model->mesh)
	{
		gf3d_model_free(model);
		return NULL;
	}
	for (i = 0; i < count; i++)
	{
		snprintf(assetname, GFCLINELEN, "models/%s_%06i.obj", filename, startFrame + i);
        snprintf(file_path, sizeof(file_path), "%s%s", FILE_PATH, assetname);
		slog("%s",assetname);
		model->mesh = gf3d_mesh_load(assetname);
	}

	snprintf(assetname, GFCLINELEN, "images/%s.png", filename);
	model->texture = gf3d_texture_load(assetname, NULL);

	return model;
}

Model * gf3d_model_load(char * filename)
{
    TextLine assetname;
    Model *model;
    model = gf3d_model_new();
    if (!model)return NULL;
    
    snprintf(assetname,GFCLINELEN,"models/%s.obj",filename);
    snprintf(file_path, sizeof(file_path), "%s%s", FILE_PATH, assetname);

    model->mesh = gf3d_mesh_load(file_path);

    if (!model->mesh)
    {
        gf3d_model_free(model);
        return NULL;
    }

    snprintf(assetname,GFCLINELEN,"images/%s.png",filename);
    snprintf(file_path, sizeof(file_path), "%s%s", FILE_PATH, assetname);

    model->texture = gf3d_texture_load(assetname, NULL);
    
    if (!model->texture)
    {
        snprintf(file_path, sizeof(file_path), "%s%s", FILE_PATH, "images/default.png");
        model->texture = gf3d_texture_load(file_path, NULL);
    }
    return model;
}

void gf3d_model_free(Model *model)
{
    gf3d_model_delete(model);
}

void gf3d_model_draw(Model *model, Matrix4D modelMat)
{
    VkDescriptorSet *descriptorSet = NULL;
    VkCommandBuffer commandBuffer;
    Uint32 bufferFrame;
    if (!model)
    {
        slog("cannot render a NULL model");
        return;
    }
    commandBuffer = gf3d_mesh_get_model_command_buffer();
    bufferFrame = gf3d_vgraphics_get_current_buffer_frame();
    descriptorSet = gf3d_pipeline_get_descriptor_set(gf3d_model.pipe, bufferFrame);
    if (descriptorSet == NULL)
    {
        slog("failed to get a free descriptor Set for model rendering");
        return;
    }
    gf3d_model_update_basic_model_descriptor_set(model,*descriptorSet,bufferFrame,modelMat);
    gf3d_mesh_render(model->mesh, commandBuffer, descriptorSet);
}

void gf3d_model_update_uniform_buffer(Model *model,UniformBuffer *ubo, Matrix4D modelMat)
{
    void* data;
    MVPMatrix mvp;
    MVPMatrix temp;
    mvp = gf3d_vgraphics_get_uniform_buffer_object();

    matrix4d_copy(modelMat,temp.model);
    matrix4d_copy(mvp.view, temp.view);
    matrix4d_copy(mvp.proj, temp.proj);

    vkMapMemory(gf3d_model.device, ubo->uniformBufferMemory, 0, sizeof(MVPMatrix), 0, &data);
    
        memcpy(data, &temp, sizeof(MVPMatrix));

    vkUnmapMemory(gf3d_model.device, ubo->uniformBufferMemory);
}

void gf3d_model_update_basic_model_descriptor_set(Model *model,VkDescriptorSet descriptorSet,Uint32 chainIndex,Matrix4D modelMat)
{
    VkDescriptorImageInfo imageInfo = {0};
    VkWriteDescriptorSet descriptorWrite[2] = {0};
    VkDescriptorBufferInfo bufferInfo = {0};
    UniformBuffer* ubo = NULL;

    if (!model)
    {
        slog("no model provided for descriptor set update");
        return;
    }
    if (descriptorSet == VK_NULL_HANDLE)
    {
        slog("null handle provided for descriptorSet");
        return;
    }
    ubo = gf3d_uniform_buffer_list_get_buffer(gf3d_model.pipe->uboList, chainIndex);
    if (!ubo)
    {
        slog("failed to get a free uniform buffer for draw call");
        return;
    }
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = model->texture->textureImageView;
    imageInfo.sampler = model->texture->textureSampler;

    gf3d_model_update_uniform_buffer(model,ubo,modelMat);
    bufferInfo.buffer = ubo->uniformBuffer;
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(MVPMatrix);
    
    descriptorWrite[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite[0].dstSet = descriptorSet;
    descriptorWrite[0].dstBinding = 0;
    descriptorWrite[0].dstArrayElement = 0;
    descriptorWrite[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite[0].descriptorCount = 1;
    descriptorWrite[0].pBufferInfo = &bufferInfo;

    descriptorWrite[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite[1].dstSet = descriptorSet;
    descriptorWrite[1].dstBinding = 1;
    descriptorWrite[1].dstArrayElement = 0;
    descriptorWrite[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrite[1].descriptorCount = 1;                        
    descriptorWrite[1].pImageInfo = &imageInfo;
    descriptorWrite[1].pTexelBufferView = NULL; // Optional

    vkUpdateDescriptorSets(gf3d_model.device, 2, descriptorWrite, 0, NULL);
}

void gf3d_model_draw_anim(Model *model, Uint32 bufferFrame, VkCommandBuffer commandBuffer, Matrix4D modelMat, Uint32 frame)
{
	VkDescriptorSet *descriptorSet = NULL;
	if (!model)
	{
		slog("cannot render a NULL model");
		return;
	}
	if (frame >= model->frameCount)
	{
		slog("cannot render model frame %i, greater than frameCount", frame);
		return;
	}
	descriptorSet = gf3d_pipeline_get_descriptor_set(gf3d_model.pipe, bufferFrame);
	if (descriptorSet == NULL)
	{
		slog("failed to get a free descriptor Set for model rendering");
		return;
	}
	gf3d_model_update_basic_model_descriptor_set(model, *descriptorSet, bufferFrame, modelMat);
	gf3d_mesh_render(model->mesh, commandBuffer, descriptorSet);

}

/*eol@eof*/
