#ifndef CAMERA_H
#define CAMERA_H


#include "sprite.h"

class Camera
{
public:
    Vector2 position; 
    float width = 160;     
    float height = 120;     

    Camera();


    void set_position(float x, float y);

    void follow(Sprite target);

    Vector2 world_to_camera(const Vector2& world_coords) const;

};
#endif