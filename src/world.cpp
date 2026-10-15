#include "world.h"
#include "game.h"
#include "map.h"
#include "sprite.h"
#include "array"
#include "camera.h"
#include "framework.h"
#include <cmath>
#include <algorithm>

#define GHOST_CHASE_DISTANCE 60   

#define RADIUS_BALLS 25;
Vector2 World::get_ball_position(Ball b)
{
    Vector2 ball_position;
    float a = cos(b.angle);
    ball_position.x = main_character.player_sprite->pos_x+ main_character.player_sprite->texture->width/2 + std::cos(b.angle) * RADIUS_BALLS;
    ball_position.y = main_character.player_sprite->pos_y + main_character.player_sprite->texture->height/2 +  std::sin(b.angle) * RADIUS_BALLS;
    return ball_position;
}

void World::duplicate_balls() {
    int size = balls_list.size();
    for (int i = 0; i < size; i++){
        add_ball();
    }
}

void World::add_ball()
{
    Ball ball;
    balls_list.push_back(ball);
    reasemble_balls();
}

void World::reasemble_balls()
{
	int num_balls = balls_list.size();
    for (int i = 0; i < num_balls; i++) {
		Ball& ball = balls_list[i];

		ball.angle = ((2 * PI)*i) / (num_balls);

        ball.last_cell_impacted = nullptr;
    }
}

void World::render_balls(Camera cam)
{
    for(auto& ball : balls_list) {
        Vector2 ball_position = get_ball_position(ball);
        Vector2 screen_pos = cam.world_to_camera(ball_position);
		Color ball_color = Color(0, 0, 255); 
        Game::instance->framebuffer.drawRectangle(screen_pos.x,screen_pos.y,1,1, ball_color);

	}
}

World::World() {
    main_character.player_sprite = new Sprite(Game::instance->images.player_image, Sprite::sSpriteType::PLAYER);
    Ball ball;
	ball.angle = 0.0f;
	balls_list.push_back(ball);
    map = new Map(level);
    ghost_list = {};
    std::array<int, 2> main_character_starting_position = map->getStartingPosition();
    main_character.player_sprite->pos_x = main_character_starting_position[0];
    main_character.player_sprite->pos_y = main_character_starting_position[1];
}


void World::update_ghosts_position(Vector2 player_pos, float dt)
{
    float ghost_speed_normalized = (ghost_speed + level/20.0) * dt;
    for (size_t i = 0; i < ghost_list.size(); i++) {
        Sprite& ghost = ghost_list[i];
        float dx = player_pos.x - ghost.pos_x;
        float dy = player_pos.y - ghost.pos_y;
        float distance_from_player = sqrt(dx * dx + dy * dy);

        float target_x, target_y;

        int random = rand() % 3;
        if (distance_from_player < GHOST_CHASE_DISTANCE || random == 0) {
            target_x = ghost.pos_x + dx * ghost_speed_normalized;
            target_y = ghost.pos_y + dy * ghost_speed_normalized;
        }
        else {
            //if rendom 33 of chances of chasing you

            int rand_x = (rand() % 3) - 1;
            int rand_y = (rand() % 3) - 1;
           

            target_x = ghost.pos_x + (float) rand_x * ghost_speed_normalized;
            target_y = ghost.pos_y + (float) rand_y * ghost_speed_normalized;

            target_x = std::max(16.0f, std::min(target_x, (map->width - 2) * 16.0f));
            target_y = std::max(16.0f, std::min(target_y, (map->height - 2) * 16.0f));
        }
        ghost.set_position(target_x, target_y);
    }
}
/* wanted to add tweening with ai, but 4.0 not available in ubuntu
void World::update_ghosts_position(Vector2 player_position, float dt)
{
    for (size_t i = 0; i < ghost_list.size(); i++) {
        Sprite& ghost = ghost_list[i];
        auto& tween = ghost_tweens[i];

        if (tween.progress() >= 1.0f) {
            float dx = player_position.x - ghost.pos_x;
            float dy = player_position.y - ghost.pos_y;
            float distance_from_player = sqrt(dx * dx + dy * dy);

            float target_x, target_y;
            if (distance_from_player < GHOST_CHASE_DISTANCE) {
                target_x = ghost.pos_x + dx / (distance_from_player + 0.001f) * ghost_speed;
                target_y = ghost.pos_y + dy / (distance_from_player + 0.001f) * ghost_speed;
            }
            else {
                target_x = ghost.pos_x + (rand() % (2 * ghost_speed + 1)) - ghost_speed;
                target_y = ghost.pos_y + (rand() % (2 * ghost_speed + 1)) - ghost_speed;
            }

            target_x = std::max(16.0f, std::min(target_x, (map->width - 2) * 16.0f));
            target_y = std::max(16.0f, std::min(target_y, (map->height - 2) * 16.0f));

            tween = tweeny::from(ghost.pos_x, ghost.pos_y).to(target_x, target_y).during(500).via(tweeny::easing::elasticEaseOut);
        }

        auto pos = tween.step((int)(dt * 1000));   
        ghost.set_position(std::get<0>(pos), std::get<1>(pos));
    }
*/

void World::add_ghost() {
    int side = rand() % 4;
    int door_x, door_y;

    do {
        if (side == 0) {
            door_x = rand() % (map->width - 2) + 1;
            door_y = 0;
        }
        if (side == 1) {
            door_x = rand() % (map->width - 2) + 1;
            door_y = map->height - 1;
        }
        if (side == 2) {
            door_x = 0;
            door_y = rand() % (map->height - 2) + 1;
        }
        if (side == 3) {
            door_x = map->width - 1;
            door_y = rand() % (map->height - 2) + 1;
        }
    } while (Vector2(door_x * 16, door_y * 16).distance(Vector2(main_character.player_sprite->pos_x, main_character.player_sprite->pos_y)) < 5 * 16);

    add_ghost(door_x * 16, door_y * 16);
}

void World::add_ghost(float x, float y)
{
    Sprite ghost_sprite(Game::instance->images.ghost_image, Sprite::GHOST);
    ghost_sprite.set_position(x, y);
    ghost_list.push_back(ghost_sprite);
    //ghost_tweens.push_back(tweeny::from(x, y).to(x, y).during(1));
}

void World::init_level()
{
    level++;
    if (map) delete map;
    map = new Map(level);    
    ghost_list.clear();
    for (int i = 0; i < level; i++) {
        add_ghost();
    }
    std::array<int, 2> main_character_starting_position = map->getStartingPosition();
    main_character.player_sprite->pos_x = main_character_starting_position[0];
    main_character.player_sprite->pos_y = main_character_starting_position[1];
}

void World::render_ghosts(Camera cam) {
    for(auto& ghost : ghost_list) {

		if (!ghost.texture) continue; 
            Vector2 screen_pos = cam.world_to_camera(Vector2(ghost.pos_x, ghost.pos_y));
            Game::instance->framebuffer.drawImage(*ghost.texture, screen_pos.x,screen_pos.y);
    }
};

void World::render_main_character(Camera cam) {
    if(main_character.player_sprite) {
        Vector2 screen_pos = cam.world_to_camera(Vector2(main_character.player_sprite->pos_x, main_character.player_sprite->pos_y));

        Game::instance->framebuffer.drawImage(*main_character.player_sprite->texture, screen_pos.x,screen_pos.y);
    }
};

