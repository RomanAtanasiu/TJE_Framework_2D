#ifndef SPRITE_H
#define SPRITE_H
#include "image.h"

class Sprite {
    public:


        float pos_x;
        float pos_y;
        float width;
        float height;
        float prev_pos_x;
        float prev_pos_y;
        Image* texture;


        enum sSpriteType: uint8{
            PLAYER, GHOST, WALL, BACKGROUND, BALL, DOOR, BLOCK
        };
        sSpriteType type;

        Sprite (Image *texture, sSpriteType type);
        
        void set_position(float x, float y);

        bool is_colliding(Sprite* other) {
            return !(pos_x + width < other->pos_x || pos_x > other->pos_x + other->width || pos_y + height < other->pos_y || pos_y > other->pos_y + other->height);
        }
        bool is_colliding_with_cell_position(int position_cell_x, int position_cell_y, int cell_size);
    
};
#endif