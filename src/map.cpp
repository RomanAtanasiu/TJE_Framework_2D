#include "map.h"
#include <random>

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"
#include <cmath>

#include "game.h"
#include <vector>
#include <algorithm>
#include <functional>

#define NOISE_SCALE 0.15f 

#define WIDTH_LEVEL 10
#define HEIGHT_LEVEL 10



Map::Map(int level) {
    width = WIDTH_LEVEL+(level*4);
    height = HEIGHT_LEVEL+(level*4);
    data = new sCell[width * height];


    // progreso: 0 en los niveles 1-3, sube hasta 1 en el nivel 10
    float t = std::min(1.0f, std::max(0.0f, (level - 3) / 7.0f));

    // fracción de celdas interiores de cada tipo (el resto son bloques enteros)
    float empty_fraction = 0.50f - 0.42f * t;   // 50% -> 8%  vacío
    float small_fraction = 0.18f - 0.14f * t;   // 18% -> 4%  bloques de 1 golpe
    float big_fraction = 0.15f - 0.07f * t;   // 15% -> 8%  bloques de 2 golpes

    // 1) ruido de todas las celdas interiores
    float offset_x = rand() % 1000;   // desplaza el ruido para que cada partida sea distinta
    float offset_y = rand() % 1000;
    std::vector<float> noise_values(width * height, 0.0f);
    std::vector<float> sorted_noise;
    for (int i = 1; i < width - 1; i++) {
        for (int j = 1; j < height - 1; j++) {
            float noise = stb_perlin_noise3(offset_x + i * NOISE_SCALE, offset_y + j * NOISE_SCALE, level, 0, 0, 0);
            noise = (noise + 1.0f) / 2.0f;
            noise_values[i + j * width] = noise;
            sorted_noise.push_back(noise);
        }
    }

    // 2) umbrales a partir de la distribución real del ruido (ordenado de mayor a menor)
    std::sort(sorted_noise.begin(), sorted_noise.end(), std::greater<float>());
    int n = sorted_noise.size();
    float empty_threshold = sorted_noise[std::min(n - 1, (int)(empty_fraction * n))];
    float small_damage_threshold = sorted_noise[std::min(n - 1, (int)((empty_fraction + small_fraction) * n))];
    float big_damage_threshold = sorted_noise[std::min(n - 1, (int)((empty_fraction + small_fraction + big_fraction) * n))];

    // 3) tipo de cada celda
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            sCell& cell = getCell(i, j);
            if (i == 0 || j == 0 || i == width - 1 || j == height - 1) {
                cell.type = sCellType::WALL;
                cell.block = sBlockType::FULL;
            }
            else {
                float noise = noise_values[i + j * width];
                if (noise > empty_threshold) {
                    cell.type = sCellType::EMPTY;
                    cell.block = sBlockType::NONE;
                }
                else if (noise > small_damage_threshold) {
                    cell.type = sCellType::BLOCK;
                    cell.block = sBlockType::SMALL_DAMAGE;
                }
                else if (noise > big_damage_threshold) {
                    cell.type = sCellType::BLOCK;
                    cell.block = sBlockType::BIG_DAMAGE;
                }
                else {
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
    int door_x = 1, door_y = 0;

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


