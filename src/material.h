#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdbool.h>

typedef enum MaterialType
{
    MATERIAL_NONE,
    MATERIAL_WATER,
    MATERIAL_SALT,
    MATERIAL_IRON,
    MATERIAL_RUST,
    MATERIAL_TYPE_COUNT
} MaterialType;

typedef struct MaterialDefinition
{
    MaterialType type;

    float gravity_strength;
    float flow_rate;
    float spread_rate;

    float density;
    float viscosity;
    bool is_liquid;
} MaterialDefinition;

const MaterialDefinition *Material_GetDefinition(MaterialType type);

#endif
