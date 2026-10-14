#ifndef MAP_H
#define MAP_H

#include "includes.h"
#include "framework.h"
#include "array"
#include "sprite.h"
#include "camera.h"
enum sCellType : uint8 {
    START, DOOR, WALL, EMPTY, BLOCK
};
enum sBlockType : uint8 {
    NONE, SMALL_DAMAGE, BIG_DAMAGE, FULL
};

struct sCell {
    sCellType type;
    sBlockType block;
};

class Map {
public:
    int width = 0;
    int height = 0;
    int tile_width = 16;
    int tile_height = 16;
    sCell* data;
    Map() {}
    Map(int level);
    void render_map(Camera cam);
    sCell& getCell(int x, int y) { return data[x + y * width]; }
    std::array<int, 2> getStartingPosition();
    std::vector<sCell*> what_cells_are_colliding(Sprite sprite);
    void decrease_block_level(sCell& cell);
    void decrease_block_level(int x_cell, int y_cell);
};
#endif