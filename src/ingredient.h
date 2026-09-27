#ifndef INGREDIENT_H
#define INGREDIENT_H

#include <stdint.h>
#include "grid.h"

typedef enum IngredientType {
    INGREDIENT_SALT,

    INGREDIENT_TYPE_COUNT
} IngredientType;

typedef struct Ingredient {
    IngredientType type;
    Grid grid;
} Ingredient;


typedef struct IngredientList {
    Ingredient* list;
} IngredientList;

bool Ingredient_InitList(IngredientList* ingredient_list);
Ingredient* Ingredient_Get(IngredientList* ingredient_list, IngredientType type);
void Ingredient_FreeList(IngredientList* ingredient_list);

#endif
