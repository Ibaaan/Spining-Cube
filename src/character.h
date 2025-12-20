#pragma once
typedef struct
{
    double x;
    double y;
} Vector2;

typedef struct{
    Vector2 position;
    Vector2 direction;
    Vector2 velocity;
}Character;

void move_character(Character character);
void init_character(Character character);
