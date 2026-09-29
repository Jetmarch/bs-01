#ifndef MATERIAL_H
#define MATERIAL_H

typedef enum MaterialType {
    MATERIAL_NONE,
    MATERIAL_WATER,
    MATERIAL_SALT,
    MATERIAL_IRON,
    MATERIAL_TYPE_COUNT
} MaterialType;

typedef struct MaterialDefinition {
    MaterialType type;

    float density;
    float viscosity;
} MaterialDefinition;

const MaterialDefinition* Material_GetDefinition(MaterialType type);

#endif
