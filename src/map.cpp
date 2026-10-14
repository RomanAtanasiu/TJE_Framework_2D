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
                if(noise > 1.0-(exp(-increasing_difficulty*(level + sBlockType::SMALL_DAMAGE/ (sBlockType::FULL+1))))){
                    cell.type = sCellType::BLOCK;
                    cell.block = sBlockType::SMALL_DAMAGE;
                }
                if(noise > 1.0-(exp(-increasing_difficulty*(level + sBlockType::BIG_DAMAGE/ (sBlockType::FULL+1))))){
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
}

void Map::render_map()
{
        for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            sCell& cell = getCell(i,j);
            if(cell.type == sCellType::WALL){
                Game::instance->framebuffer.drawImage(*Game::instance->images.wall_image, i*16, j*16);
            } else if(cell.type == sCellType::BLOCK){
                Game::instance->framebuffer.drawImage(*Game::instance->images.block_image, i*16, j*16);
            } else if(cell.type == DOOR){
                Game::instance->framebuffer.drawImage(*Game::instance->images.door_image, i*16, j*16);
            } else{
                Game::instance->framebuffer.drawImage(*Game::instance->images.empty_image, i*16, j*16);
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


