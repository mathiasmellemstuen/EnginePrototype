#ifndef ENGINEPROTOTYPE_VALIDATIONLAYERS
#define ENGINEPROTOTYPE_VALIDATIONLAYERS 
#include <vector>

const std::vector<const char*> validationLayers = {
    #ifdef __WIN32__ //Validation layers does not work on linux for some reason... Disabling it until finding out why. TODO: Fix validation layers on linux.
    "VK_LAYER_KHRONOS_validation"
    #endif 
};

#ifdef NOTDEBUG 
    const bool enableValidationLayers = false; 
#else 
    const bool enableValidationLayers = true; 
#endif

bool checkValidationLayerSupport();

#endif