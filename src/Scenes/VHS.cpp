#include "Scenes/VHS.h"
#include "Model.h"
#include "Shader.h"

VHS::VHS()
{
    InitializeScene();
}

VHS::~VHS()
{
    for(Model* ptr : models)
        delete ptr;
    for(Shader* ptr : shaders)
        delete ptr;
}

void VHS::InitializeScene()
{
    // Load ground
}