#include <string.h>
#include <stdio.h>
#include <vulkan/vulkan.h>

#include "simple_logger.h"
#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf3d_validation.h"



// validation layers
typedef struct
{
    Uint32 layerCount;
    VkLayerProperties *availableLayers;
    const char* * layerNames;
}vValidation;

static vValidation gf3d_validation = {0};
static VkValidationFlagsEXT gf3d_validation_flags = { 0 };

void gf3d_validation_query_layer_properties()
{
	int i;
    vkEnumerateInstanceLayerProperties(&gf3d_validation.layerCount, NULL);
    slog("discovered %i validation layers",gf3d_validation.layerCount);
    
    if (!gf3d_validation.layerCount)return;
    
    gf3d_validation.availableLayers = (VkLayerProperties *)gfc_allocate_array(sizeof(VkLayerProperties),gf3d_validation.layerCount);
    vkEnumerateInstanceLayerProperties(&gf3d_validation.layerCount, gf3d_validation.availableLayers);
    
    gf3d_validation.layerNames = (const char* * )gfc_allocate_array(sizeof(const char *),gf3d_validation.layerCount);
    for (i = 0; i < gf3d_validation.layerCount;i++)
    {
        gf3d_validation.layerNames[i] = (const char *)gf3d_validation.availableLayers[i].layerName;
        slog("Validation layer available: %s",gf3d_validation.availableLayers[i].layerName);
    }
}

VkValidationFlagsEXT gf3d_validation_flag_properties(){
	int i;
	const char* const layer = { "VK_LAYER_LUNARG_screenshot" };
	slog("Layer List: %s", layer);

	gf3d_validation_flags.sType = VK_STRUCTURE_TYPE_VALIDATION_FLAGS_EXT;
	gf3d_validation_flags.pNext = NULL;
	gf3d_validation_flags.disabledValidationCheckCount = 1;
	gf3d_validation_flags.pDisabledValidationChecks = gfc_allocate_array(sizeof(const char *), gf3d_validation_flags.disabledValidationCheckCount);
	slog("Validation disable count: %i", gf3d_validation_flags.disabledValidationCheckCount);

	for (i = 0; i < gf3d_validation.layerCount; i++){
		if (strcmp(gf3d_validation.availableLayers[i].layerName, "VK_LAYER_LUNARG_screenshot") == 0){
			gf3d_validation_flags.pDisabledValidationChecks = layer;
		}
	}
	slog("Disabled Validation Checks: %s", layer);// gf3d_validation_flags.pDisabledValidationChecks);

	return gf3d_validation_flags;
}

void gf3d_validation_close()
{
    if (gf3d_validation.availableLayers)
    {
        free(gf3d_validation.availableLayers);
        gf3d_validation.availableLayers = NULL;
    }
    if (gf3d_validation.layerNames)
    {
        free(gf3d_validation.layerNames);
        gf3d_validation.layerNames = NULL;
    }
    memset(&gf3d_validation,0,sizeof(vValidation));
}

void gf3d_validation_init()
{
    gf3d_validation_query_layer_properties();
    atexit(gf3d_validation_close);
}

Bool gf3d_validation_check_layer_support(char *layerName)
{
    int i;
    for (i = 0; i < gf3d_validation.layerCount;i++)
    {
        if (strcmp(layerName,gf3d_validation.availableLayers[i].layerName) == 0)
        {
            return true;
        }
    }
    return false;
}

Uint32 gf3d_validation_get_validation_layer_count()
{
    return gf3d_validation.layerCount;
}

VkLayerProperties *gf3d_validation_get_validation_layer_data()
{
    return gf3d_validation.availableLayers;
}

VkValidationFlagsEXT gf3d_validation_disable_validations()
{
	return gf3d_validation_flags;
}

const char* const* gf3d_validation_get_validation_layer_names()
{
    return (const char* const* )gf3d_validation.layerNames;
}

/*eol@eof*/
