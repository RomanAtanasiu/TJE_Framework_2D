#include "map.h"
#include <random>

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"
#include <cmath>

#include "game.h"

#define WIDTH_LEVEL 10
#define HEIGHT_LEVEL 10



Map::Map(int level) {
    width = WIDTH_LEVEL*(3.0*level/4.0);
    height = HEIGHT_LEVEL*(3.0*level/4.0);
    data = new sCell[width * height];


    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            sCell& cell = getCell(i,j);
            if(i == 0 || j == 0 || i == width-1 || j == height-1){
                cell.type = sCellType::WALL;
                cell.block = sBlockType::FULL;


            } else {
                float noise = stb_perlin_noise3(i, j, level, 0, 0, 0);
                noise = (noise + 1.0) / 2.0;

                float increasing_difficulty = 0.3;
                if(noise > 1.0-(exp(-increasing_difficulty*level))){
                    cell.type = sCellType::EMPTY;
                    cell.block = sBlockType::NONE;
                }  
                else if(noise > 1.0-(exp(-increasing_difficulty*(level + sBlockType::SMALL_DAMAGE/ (float) (sBlockType::FULL+1))))){
                    cell.type = sCellType::BLOCK;
                    cell.block = sBlockType::SMALL_DAMAGE;
                }
                 else if(noise > 1.0-(exp(-increasing_difficulty*(level + sBlockType::BIG_DAMAGE/ (float) (sBlockType::FULL+1))))){
                    cell.type = sCellType::BLOCK;
                    cell.block = sBlockType::BIG_DAMAGE;
                } else {
                    cell.type = sCellType::BLOCK;
                    cell.block = sBlockType::FULL;
                }

            }
        }
    }

    int start_y= (rand() % (width-2) ) +1; //+1 so its not corner wall
    int start_x= (rand() % (height-2)) +1;

    getCell(start_x, start_y).type = sCellType::START;
    getCell(start_x, start_y).block = sBlockType::NONE;

    int side = rand() % 4;
    int door_x, door_y;

    if (side == 0) {
        door_x = rand() % (width - 2) + 1;
        door_y = 0;
    }
    if (side == 1) {
        door_x = rand() % (width - 2) + 1;
        door_y = height - 1;
    }
    if (side == 2) {
        door_x = 0;
        door_y = rand() % (height - 2) + 1;
    }
    if (side == 3) {
        door_x = width - 1;
        door_y = rand() % (height - 2) + 1;
    }

    getCell(door_x, door_y).type = sCellType::DOOR;
    getCell(door_x, door_y).block = sBlockType::NONE;
}

void Map::render_map(Camera cam)
{
        for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            sCell& cell = getCell(i,j);
            Vector2 cell_position = cam.world_to_camera(Vector2(i * 16, j * 16));
            if(cell.type == sCellType::WALL){
                Game::instance->framebuffer.drawImage(*Game::instance->images.wall_image, cell_position.x, cell_position.y);
            } else if(cell.type == sCellType::BLOCK){

                Image* block_image;
                if(cell.block == sBlockType::FULL){
                    block_image = Game::instance->images.block_image;
                } else if(cell.block == sBlockType::BIG_DAMAGE){
                    block_image = Game::instance->images.block_big_damage_image;
                } else if(cell.block == sBlockType::SMALL_DAMAGE){
                    block_image = Game::instance->images.block_small_damage_image;
                } else{
                    block_image = Game::instance->images.empty_image;
				}

                Game::instance->framebuffer.drawImage(*block_image, cell_position.x, cell_position.y);
            } else if(cell.type == DOOR){
                Game::instance->framebuffer.drawImage(*Game::instance->images.door_image, cell_position.x, cell_position.y);
            } else{
                Game::instance->framebuffer.drawImage(*Game::instance->images.empty_image, cell_position.x, cell_position.y);
            }
        }
    }
}

std::array<int, 2> Map::getStartingPosition()
{
    for(int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            sCell& cell = getCell(i, j);
            if (cell.type == sCellType::START) {
                std::array<int, 2> position;
                position[0] = i*tile_width; 
                position[1] = j*tile_height;
                return position;
            }
        }
    }
    return std::array<int, 2>{-1, -1};
}

std::vector<sCell*> Map::what_cells_are_colliding(Sprite sprite)
{
    std::vector<sCell*> colliding_cells = {};
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if(sprite.is_colliding_with_cell_position(i,j,16)) {
                colliding_cells.push_back(&getCell(i, j));
			}
        }
    }
	return colliding_cells;
}

void Map::decrease_block_level(sCell& cell) {
    if (cell.type != BLOCK) {
        std::cout << "not a block";
        return;
    }
    if (cell.block > NONE) {
        //static cast idea generated from copilot, I wanted to decrease level, did not know I needed static cast
        cell.block = static_cast<sBlockType>(cell.block - 1); // Corrected line
        if (cell.block == NONE) cell.type = EMPTY;
    }
}

void Map::decrease_block_level(int x, int y)
{
    sCell& cell = getCell(x, y);
    if (cell.type != BLOCK) {
        std::cout << "not a block";
        return;
    }
    if (cell.block > NONE) {
        //static cast idea generated from copilot, I wanted to decrease level, did not know I needed static cast
        cell.block = static_cast<sBlockType>(cell.block - 1); // Corrected line
        if (cell.block == NONE) cell.type = EMPTY;
    }
}


