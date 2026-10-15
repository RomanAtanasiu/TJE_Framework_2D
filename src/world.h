#ifndef WORLD_H
#define WORLD_H


#include "map.h"
#include "sprite.h"
#include <vector>
#include "camera.h"
#define MAX_GHOSTS 100


class World {
    public:
        Map* map = nullptr;
        int level = 1;
        struct{
            Sprite* player_sprite = nullptr;
            
            float taladro_time = 0.1;
            float time_since_last_impact = 0.1;

            int taladro_damage = 1;
            float balls_velocity = 0.5;


        } main_character;

        struct Ball {
            float angle;
            sCell* last_cell_impacted = nullptr;
        };


        std::vector<Ball> balls_list = {};
        Vector2 get_ball_position(Ball b);

        //only adding balls during changing levels
        void reasemble_balls();

        void add_ball();

		void render_balls(Camera cam);

        float time_since_last_ghost = 0;
        std::vector<Sprite> ghost_list;

        World();

        void add_ghost(float x, float y);
        void remove_ghosts() {ghost_list.clear();};
        void init_level();
        
        void render_ghosts(Camera cam);
        void render_main_character(Camera cam);
};
#endif