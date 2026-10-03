#include <string.h>
#include "material.h"

static const MaterialDefinition MATERIAL_DEF_WATER = {
    .type = MATERIAL_WATER,
    .gravity_strength = 1.0f,
    .flow_rate = 0.0f,
    .spread_rate = 0.0f,
    .density = 1.0f,
    .viscosity = 1.0f,
    .is_liquid = true};

static const MaterialDefinition MATERIAL_DEF_SALT = {
    .type = MATERIAL_SALT,
    .gravity_strength = 0.5f,
    .flow_rate = 0.0f,
    .spread_rate = 0.0f,
    .density = 2.16f,
    .viscosity = 0.0f,
    .is_liquid = false};

static const MaterialDefinition MATERIAL_DEF_IRON = {
    .type = MATERIAL_IRON,
    .gravity_strength = 2.0f,
    .flow_rate = 0.0f,
    .spread_rate = 0.0f,
    .density = 7.87f,
    .viscosity = 0.0f,
    .is_liquid = false};

static const MaterialDefinition MATERIAL_DEF_RUST = {
    .type = MATERIAL_RUST,
    .gravity_strength = 2.0f,
    .flow_rate = 0.0f,
    .spread_rate = 0.0f,
    .density = 5.0f,
    .viscosity = 0.0f,
    .is_liquid = false};

static const MaterialDefinition *MATERIAL_DEFINITIONS[MATERIAL_TYPE_COUNT] = {
    [MATERIAL_NONE] = NULL,
    [MATERIAL_WATER] = &MATERIAL_DEF_WATER,
    [MATERIAL_SALT] = &MATERIAL_DEF_SALT,
    [MATERIAL_IRON] = &MATERIAL_DEF_IRON,
    [MATERIAL_RUST] = &MATERIAL_DEF_RUST};

const MaterialDefinition *Material_GetDefinition(MaterialType type)
{
    if (type < 0 || type >= MATERIAL_TYPE_COUNT)
    {
        return NULL;
    }

    return MATERIAL_DEFINITIONS[type];
}
