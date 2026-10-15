#include "world.h"
#include "game.h"
#include "map.h"
#include "sprite.h"
#include "array"
#include "camera.h"
#include <math.h>
#include "framework.h"
#define RADIUS_BALLS 25;
Vector2 World::get_ball_position(Ball b)
{
    Vector2 ball_position;
    float a = cos(b.angle);
    ball_position.x = main_character.player_sprite->pos_x+ main_character.player_sprite->texture->width/2 + cos(b.angle) * RADIUS_BALLS;
    ball_position.y = main_character.player_sprite->pos_y + main_character.player_sprite->texture->height/2 +  sin(b.angle) * RADIUS_BALLS;
    return ball_position;
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
    ghost_list = std::vector<Sprite>();
    std::array<int, 2> main_character_starting_position = map->getStartingPosition();
    main_character.player_sprite->pos_x = main_character_starting_position[0];
    main_character.player_sprite->pos_y = main_character_starting_position[1];
}

void World::add_ghost(float x, float y)
{
    Sprite ghost_sprite(Game::instance->images.ghost_image, Sprite::GHOST);
    ghost_list.push_back(ghost_sprite);
}

void World::init_level()
{
    level++;
    if (map) delete map;
    map = new Map(level);    
    
    std::array<int, 2> main_character_starting_position = map->getStartingPosition();
    main_character.player_sprite->pos_x = main_character_starting_position[0];
    main_character.player_sprite->pos_y = main_character_starting_position[1];
}

void World::render_ghosts(Camera cam) {
    for(auto& ghost : ghost_list) {

		if (!ghost.texture) continue; 

        if (cam.is_visible(ghost)) {
            Vector2 screen_pos = cam.world_to_camera(Vector2(ghost.pos_x, ghost.pos_y));
            Game::instance->framebuffer.drawImage(*ghost.texture, screen_pos.x,screen_pos.y);
        }

    }
};

void World::render_main_character(Camera cam) {
    if(main_character.player_sprite) {
        Vector2 screen_pos = cam.world_to_camera(Vector2(main_character.player_sprite->pos_x, main_character.player_sprite->pos_y));

        Game::instance->framebuffer.drawImage(*main_character.player_sprite->texture, screen_pos.x,screen_pos.y);
    }
};

