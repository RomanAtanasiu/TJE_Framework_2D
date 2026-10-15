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

    Game::instance->world->time_survived += dt;
//cheat codes for testing
    {
        time_last_button_clicked += dt;

        if (Input::isKeyPressed(SDL_SCANCODE_P) && time_last_button_clicked > 0.1) {
            Game::instance->world->add_ball();

            time_last_button_clicked = 0;
        }

        if (Input::isKeyPressed(SDL_SCANCODE_O) && time_last_button_clicked > 1.0) {
            Game::instance->world->add_ghost();

            time_last_button_clicked = 0;
        }

    }



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
            Game::instance->current_stage = Game::instance->upgrade_stage;


            Game::instance->synth.playSample("data/sound/door.wav", 0.2);

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

    Game::instance->world->update_ghosts_position(Vector2(Game::instance->world->main_character.player_sprite->pos_x, Game::instance->world->main_character.player_sprite->pos_y), dt);

    //update balls position //and get collisions
    for(World::Ball &ball: Game::instance->world->balls_list) {
		ball.angle += Game::instance->world->main_character.balls_velocity * dt;
		ball.angle += Game::instance->world->main_character.balls_velocity * dt;
		if (ball.angle > 2.0*PI) ball.angle -= 2.0 * PI;
		
        
        Vector2 pos = Game::instance->world->get_ball_position(ball);
        sCell& actual_cell = Game::instance->world->map->getCell(int(pos.x) / 16, int(pos.y)/16);
        if (&actual_cell != ball.last_cell_impacted ) {
            ball.last_cell_impacted = &actual_cell;
            if (actual_cell.type == BLOCK) {
                Game::instance->world->map->decrease_block_level(actual_cell);
            }
        }
	}
    


    //if player collides with ghost set go_to_next_stage to true
    for (auto& ghost : Game::instance->world->ghost_list) {
        if (Game::instance->world->main_character.player_sprite->is_colliding(&ghost)) {
            if (Game::instance->music) {
                Game::instance->music->stop();
                Game::instance->music = nullptr;   
            }

            Game::instance->synth.playSample("data/sound/death_ghost.wav", 0.2);

            Game::instance->current_stage = Game::instance->end_stage;
            break;
        }
    }



    if (Game::instance->world->time_since_last_ghost >= (20.0f - std::min(2.0*Game::instance->world->level,3.0))) {
        Game::instance->world->add_ghost();
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