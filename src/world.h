#ifndef WORLD_H
#define WORLD_H


#include "map.h"
#include "sprite.h"
#include <vector>
#define MAX_GHOSTS 100
class World {
    public:
        Map* map = nullptr;
        int level = 1;
        struct{
            Sprite* player_sprite = nullptr;
            
            int taladro_damage = 1;
            float balls_velocity = 1;

            int num_balls = 0;
        } main_character;

        
        
        float time_since_last_ghost = 0;
        std::vector<Sprite> ghost_list;

        World();

        void add_ghost(float x, float y);
        void remove_ghosts() {ghost_list.clear();};
        void init_level();
        
        void render_ghosts();
        void render_main_character();
};
#endif