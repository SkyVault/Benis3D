#ifndef BENIS_ASSEMBELR_H
#define BENIS_ASSEMBLER_H

#include "assets.h"
#include "behaviours.h"
#include "ecs.h"
#include "entity_type.h"
#include "physics.h"
#include "player.h"
#include "raylib.h"
#include "renderer.h"
#include "scripting.h"
#include "utils.h"
#include "map.h"

typedef EntId (*Ass)(Game*, float, float, float, float, float);

#define TO_ASS(k) k##_C,

EntId ACTOR_PLAYER_C(Game* game, float x, float y, float z, float vx, float vy);
EntId ACTOR_PINEAPPLE_BOMB_C(Game* game, float x, float y, float z, float vx,
                             float vy);

EntId ACTOR_GIRL_1_C(Game* game, float x, float y, float z, float vx, float vy);
EntId ACTOR_GIRL_2_C(Game* game, float x, float y, float z, float vx, float vy);
EntId ACTOR_GIRL_3_C(Game* game, float x, float y, float z, float vx, float vy);
EntId ACTOR_GIRL_4_C(Game* game, float x, float y, float z, float vx, float vy);

EntId ACTOR_SCRALAPUS_C(Game* game, float x, float y, float z, float vx, float vy);

EntId ACTOR_END_TARGET_C(Game* game, float x, float y, float z, float vx,
                         float vy);
inline EntId ACTOR_TOSSED_ORANGE_C(Game* game, float x, float y, float z,
                                   float vx, float vy);

EntId ACTOR_NUM_ENTITY_TYPES_C(Game* game, float x, float y, float z, float vx,
                               float vy);

EntId ACTOR_BLOCK_C(Game* game, float x, float y, float z, float vx, float vy);

#ifndef ASSEMBLERS
#define ASSEMBLERS
#define ENUM_STR(e) #e
static map_void_t *assemblers_dict = NULL;
static const Ass assemblers[ACTOR_NUM_ENTITY_TYPES + 1] = {
    ENTITY_TYPES(TO_ASS)};

static const char *assemblers_names[ACTOR_NUM_ENTITY_TYPES + 1] = {
    ENTITY_TYPES(TO_STR)};
#undef ENUM_STR
#endif  // ASSEMBLERS

void assemblers_init();
EntId assemble(int which, Game* game, float x, float y, float z, float vx,
               float vy);

#endif  // BENIS_ASSEMBLER_H
