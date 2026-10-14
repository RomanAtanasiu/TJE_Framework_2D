#include "stage.h"
#include "image.h"
#include "game.h"
#include "input.h"
#include <vector>
#include "camera.h"
#include "world.h"
#define PI 3.14159265
PlayStage::PlayStage() {
    camera = Camera();
}


void PlayStage::update(float dt) {


    Game::instance->world->main_character.time_since_last_impact -= dt;
    Game::instance->world->time_since_last_ghost += dt;


    float prevPos[2] = { Game::instance->world->main_character.player_sprite->pos_x, Game::instance->world->main_character.player_sprite->pos_y };
    if (Input::isKeyPressed(SDL_SCANCODE_UP)) {
        Game::instance->world->main_character.player_sprite->set_position(prevPos[0], prevPos[1] - 100 * dt);
    }
    if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) {
        Game::instance->world->main_character.player_sprite->set_position(prevPos[0], prevPos[1] + 100 * dt);
    }
    if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) {
        Game::instance->world->main_character.player_sprite->set_position(prevPos[0] - 100 * dt, prevPos[1]);
    }
    if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {
        Game::instance->world->main_character.player_sprite->set_position(prevPos[0] + 100 * dt, prevPos[1]);
    }



    std::vector<sCell*> collided_cells = Game::instance->world->map->what_cells_are_colliding(*Game::instance->world->main_character.player_sprite);

    for (auto cell : collided_cells) {
        if (cell->type == DOOR) {
            Game::instance->world->init_level();
            break;
        }
        else if (cell->type == BLOCK) {
            //if we can't destroy the block, return to previous position
            if (Game::instance->world->main_character.time_since_last_impact > 0) {
                Game::instance->world->main_character.player_sprite->set_position(prevPos[0], prevPos[1]);

            }
            else {
                Game::instance->world->map->decrease_block_level(*cell);
                //if cell not fully destoryed, can't go there
                if ((cell->type != EMPTY)) 
                    Game::instance->world->main_character.player_sprite->set_position(prevPos[0], prevPos[1]);

            }
        }
        else if (cell->type == WALL) {
            Game::instance->world->main_character.player_sprite->set_position(prevPos[0], prevPos[1]);
        }
    }

    
    //update balls position
    for(World::Ball &ball: Game::instance->world->balls_list) {
		ball.angle += Game::instance->world->main_character.balls_velocity * dt;
		ball.angle += Game::instance->world->main_character.balls_velocity * dt;
		if (ball.angle > 2.0*PI) ball.angle -= 2.0 * PI;
		Game::instance->world->get_ball_position(ball);
	}
    


    //if player collides with ghost set go_to_next_stage to true
    for (auto& ghost : Game::instance->world->ghost_list) {
        if (Game::instance->world->main_character.player_sprite->is_colliding(&ghost)) {
            go_to_next_stage = true;
        }
    }



    if (Game::instance->world->time_since_last_ghost >= 15.0f / Game::instance->world->level) {
        //todo set a random position for the ghost wihtout colliding with the player or other ghosts
        Game::instance->world->add_ghost(50, 50);
        Game::instance->world->time_since_last_ghost = 0.0f;
    }
    //can destroy blocks again
    if (Game::instance->world->main_character.time_since_last_impact <= 0) Game::instance->world->main_character.time_since_last_impact = Game::instance->world->main_character.taladro_time;
    
	camera.follow(*Game::instance->world->main_character.player_sprite);

}

void PlayStage::render() {
    Game::instance->world->map->render_map(camera);
    Game::instance->world->render_ghosts(camera);
    Game::instance->world->render_main_character(camera);
    Game::instance->world->render_balls(camera);

	//render balls

    

}