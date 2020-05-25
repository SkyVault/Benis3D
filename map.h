#ifndef BENIS_MAP_H
#define BENIS_MAP_H

#include <ctype.h>
#include <janet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "assets.h"
#include "components.h"
#include "ecs.h"
#include "game.h"
#include "map_type.h"
#include "physics_type.h"
#include "raylib.h"
#include "raymath.h"
#include "rlights.h"

static const Prop prop_types[] = {
    {.region = {0, 0, 419, 420}, .position = {0, 0, 0}, .scale = 1.0f},
    {.region = {420, 0, 419, 420}, .position = {0, 0, 0}, .scale = 1.0f},
    {.region = {840, 0, 419, 420}, .position = {0, 0, 0}, .scale = 1.0f},
};

Map *load_map_from_script(const char *path, Game *game);

void destroy_map(Map *map, Game *game);
void reload_map(Map *map, Game *game);

void update_map(Map *map, Game *game);
void render_map(Map *map, GfxState *gfx, Game *game);

void add_exit(Map *map, Vector3 position, int id, int dest_id,
              const char *dest_path);

#endif  // BENIS_MAP_H
