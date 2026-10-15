#include "camera.h"

Camera::Camera(){
    position.x = 0;
    position.y = 0;
}

void Camera::set_position(float x, float y)
{
    position.x = x;
    position.y = y;
}

void Camera::follow(Sprite target)
{
    position.x = target.pos_x + target.width / 2 - width / 2;
    position.y = target.pos_y + target.height / 2 - height / 2;
}

Vector2 Camera::world_to_camera(const Vector2& world_coords) const
{
    return Vector2(world_coords.x - position.x, world_coords.y - position.y);
}

