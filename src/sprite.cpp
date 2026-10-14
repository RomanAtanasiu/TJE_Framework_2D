#include "image.h"
#include "sprite.h"
#include "map.h"
#include "camera.h"

Sprite::Sprite(Image *texture, sSpriteType type) {
    this->texture = texture;
    width = texture->width;
    height = texture->height;
    pos_x = 0;
    pos_y = 0;
    prev_pos_x = 0;
    prev_pos_y = 0;
    this->type = type;
}


void Sprite::set_position(float x, float y) {

    prev_pos_x = pos_x;
    prev_pos_y = pos_y;
    pos_x = x;
    pos_y = y;
}



bool Sprite::is_colliding_with_cell_position(int position_cell_x, int position_cell_y, int cell_size)
{
    float sprite_left = pos_x;
    float sprite_right = pos_x + width;
    float sprite_top = pos_y;
    float sprite_bottom = pos_y + height;

    float cell_left = position_cell_x * cell_size;
    float cell_right = cell_left + cell_size;
    float cell_top = position_cell_y * cell_size;
    float cell_bottom = cell_top + cell_size;

    return !(sprite_right <= cell_left || 
             sprite_left >= cell_right || 
             sprite_bottom <= cell_top || 
             sprite_top >= cell_bottom);
}

 


