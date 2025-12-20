#include "character.h"

void init_character(Character character){
    character.position = (Vector2){0., 0.};
    character.direction = (Vector2){0., 0.};
    character.velocity = (Vector2){0., 0.};
}