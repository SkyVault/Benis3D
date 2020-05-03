#ifndef BENIS_MAP_DEF_H
#define BENIS_MAP_DEF_H

#include "raylib.h"

#define CUBE_SIZE (5)
#define CUBE_HEIGHT (CUBE_SIZE * 2)

#define MAX_MAP_WIDTH (100)
#define MAX_MAP_HEIGHT (100)

#define MAX_NUM_LAYERS (4)

typedef struct {
    int active;
    int model;
} Wall;

typedef struct {
    int current_map;
    int num_layers;

    Wall walls[MAX_NUM_LAYERS][MAX_MAP_WIDTH * MAX_MAP_HEIGHT];

    Model floor_tile_models[1];
    Model models[100];

    int width;
    int height;

} Map;

#endif  // BENIS_MAP_DEF_H
