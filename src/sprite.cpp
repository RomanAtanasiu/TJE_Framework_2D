#include "image.h"
#include "sprite.h"


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