#ifndef BENIS_MAP_DEF_H
#define BENIS_MAP_DEF_H

#include <inttypes.h>

#include "constants.h"
#include "node.h"
#include "prop.h"
#include "raylib.h"
#include "rlights.h"

#define CUBE_SIZE GLOBAL_SCALE
#define CUBE_HEIGHT (CUBE_SIZE * 2)

#define WORLD_WIDTH (50)
#define WORLD_HEIGHT (50)

#define REGION_WIDTH (50)
#define REGION_HEIGHT (50)

#define MAX_NUM_LAYERS (10)
#define MAX_MODELS (100)
#define MAX_PROPS (10000)
#define MAX_ACTOR_SPAWNS (100)
#define MAX_EXITS (100)

typedef struct {
    uint8_t active;
    uint8_t model;
} Wall;

typedef struct {
    int type;
    Vector3 position;
} ActorSpawn;

typedef struct {
    Vector3 position;
    int id;
    int dest_id;
    char* dest_path;
} Exit;

typedef struct {
    int current_map;

    Model floor_tile_models[1];
    Model models[MAX_MODELS];
    Prop props[MAX_PROPS];
    Exit exits[MAX_EXITS];

    ActorSpawn spawns[MAX_ACTOR_SPAWNS];

    Node* scene_root; // The tree of all nodes
    Node* solids; // List of solid nodes, used for collisions

    int num_models;
    int num_props;
    int num_spawns;
    int num_exits;

    int width;
    int height;

    float player_x;
    float player_z;

    struct {
        char* buff;
        size_t len;
    } path;
} Region;

typedef struct {
    Region *grid[WORLD_WIDTH][WORLD_HEIGHT];
} GameWorld;

#endif  // BENIS_MAP_DEF_H
