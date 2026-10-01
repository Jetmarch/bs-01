#include <string.h>
#include "material.h"

static const MaterialDefinition MATERIAL_DEF_WATER = {
    .type = MATERIAL_WATER,
    .density = 1.0f,
    .viscosity = 1.0f};

static const MaterialDefinition MATERIAL_DEF_SALT = {
    .type = MATERIAL_SALT,
    .density = 2.16f,
    .viscosity = 0.0f};

static const MaterialDefinition MATERIAL_DEF_IRON = {
    .type = MATERIAL_IRON,
    .density = 7.87f,
    .viscosity = 0.0f};

static const MaterialDefinition *MATERIAL_DEFINITIONS[MATERIAL_TYPE_COUNT] = {
    [MATERIAL_NONE] = NULL,
    [MATERIAL_WATER] = &MATERIAL_DEF_WATER,
    [MATERIAL_SALT] = &MATERIAL_DEF_SALT,
    [MATERIAL_IRON] = &MATERIAL_DEF_IRON};

const MaterialDefinition *Material_GetDefinition(MaterialType type)
{
    if (type < 0 || type >= MATERIAL_TYPE_COUNT)
    {
        return NULL;
    }

    return MATERIAL_DEFINITIONS[type];
}
