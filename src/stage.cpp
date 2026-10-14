#include "stage.h"
#include "image.h"
#include "game.h"

PlayStage::PlayStage() {

}


void PlayStage::update(float dt) {
    int[2] prevPos = {Game::instance->world->main_character.player_sprite->pos_x, Game::instance->world->main_character.player_sprite->pos_y};
    if(Game::instance->input_manager.isKeyPressed(SDL_SCANCODE_UP)) {
        Game::instance->world->main_character.player_sprite->set_position(prevPos[0], prevPos[1] - 100 * dt);
    }
    if(Game::instance->input_manager.isKeyPressed(SDL_SCANCODE_DOWN)) {
        Game::instance->world->main_character.player_sprite->set_position(prevPos[0], prevPos[1] + 100 * dt);
    }
    if(Game::instance->input_manager.isKeyPressed(SDL_SCANCODE_LEFT)) {
        Game::instance->world->main_character.player_sprite->set_position(prevPos[0] - 100 * dt, prevPos[1]);
    }
    if(Game::instance->input_manager.isKeyPressed(SDL_SCANCODE_RIGHT)) {
        Game::instance->world->main_character.player_sprite->set_position(prevPos[0] + 100 * dt, prevPos[1]);
    }

    game->world->time_since_last_ghost += dt;
    if(game->world->time_since_last_ghost >= 15.0f/game->world->level) {
        //todo set a random position for the ghost wihtout colliding with the player or other ghosts
        game->world->add_ghost(50, 50);
        game->world->time_since_last_ghost = 0.0f;
    }

    //if player collides with ghost set go_to_next_stage to true
    for(auto ghost : game->world->ghost_list) {
        if(game->world->main_character.player_sprite->is_colliding(&ghost)) {
            go_to_next_stage = true;
        }
    }


    //if player collides with door set call init_level to go to the next level
    for(int y = 0; y < game->world->map->height; y++) {
        for(int x = 0; x < game->world->map->width; x++) {
            sCell cell = game->world->map->getCell(x, y);
            if(cell.type == sCell::DOOR) {
                if(game->world->main_character.player_sprite->is_colliding(&ghost)) {
                    game->world->init_level();
                }
            }
        }
    }
}

void PlayStage::render() {
    Game::instance->world->map->render_map();
    Game::instance->world->render_ghosts();
    Game::instance->world->render_main_character();

}