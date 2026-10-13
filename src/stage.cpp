#include "stage.h"
#include "image.h"
#include "game.h"

PlayStage::PlayStage() {

}


void PlayStage::update(float dt) {
    // Update implementation
}

void PlayStage::render() {
    Game::instance->world->map->render_map();
    Game::instance->world->render_ghosts();
    Game::instance->world->render_main_character();

}