#include "world.h"
#include "game.h"

World::World() {
    map = new Map(level);
    ghost_list = std::vector<Sprite>();
    level = 0;
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
}

void World::render_ghosts() {
    for(auto ghost : ghost_list) {
        Game::instance->framebuffer.drawImage(*ghost.texture, ghost.pos_x, ghost.pos_y);
    }
};

void World::render_main_character() {
    if(main_character.player_sprite) {
        Game::instance->framebuffer.drawImage(*main_character.player_sprite->texture, main_character.player_sprite->pos_x, main_character.player_sprite->pos_y);
    }
};
