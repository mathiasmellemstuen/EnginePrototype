#ifndef ENGINEPROTOTYPE_VALIDATIONLAYERS
#define ENGINEPROTOTYPE_VALIDATIONLAYERS 
#include <vector>

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NOTDEBUG 
    const bool enableValidationLayers = false; 
#else 
    const bool enableValidationLayers = true; 
#endif

bool checkValidationLayerSupport();

#endif