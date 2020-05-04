#include "models.h"

GfxState* create_gfx_state() {
    GfxState* gfx = malloc(sizeof(GfxState));
    for (int i = 0; i < MAX_NUMBER_OF_DRAWABLES; i++)
        gfx->drawables[i].flags = 0;
    gfx->num_drawables = 0;
    return gfx;
}

void update_billboard(EcsWorld* ecs, EntId ent) {}

void draw_billboard(GfxState* gfx, Camera* camera, EcsWorld* ecs, EntId ent) {
    EntStruct* self = get_ent(ecs, ent);

    if (!has_comp(ecs, self, Billboard) || !has_comp(ecs, self, Transform))
        return;

    if (gfx->num_drawables < MAX_NUMBER_OF_DRAWABLES) {
        gfx->drawables[gfx->num_drawables++] = (Drawable){
            .type = DrawType_Billboard,
            .flags = DrawFlag_Active,
            .transform = *get_comp(ecs, self, Transform),
            .billboard = *get_comp(ecs, self, Billboard),
            .diffuse = RAYWHITE,
        };
    }
}

void update_models(EcsWorld* ecs, EntId ent) {}

void draw_models(GfxState* gfx, EcsWorld* ecs, EntId ent) {
    EntStruct* self = get_ent(ecs, ent);

    if (!has_comp(ecs, self, Model) || !has_comp(ecs, self, Transform)) return;

    if (gfx->num_drawables < MAX_NUMBER_OF_DRAWABLES) {
        Model* model = get_comp(ecs, self, Model);
        gfx->drawables[gfx->num_drawables++] = (Drawable){
            .type = DrawType_Model,
            .flags = DrawFlag_Active,
            .transform = *get_comp(ecs, self, Transform),
            .model = *model,
            .diffuse = model->materials[0].maps[MAP_DIFFUSE].color,
        };
    }
}

void flush_graphics(GfxState* gfx, Camera* camera) {
    // for (int i = MAX_NUMBER_OF_DRAWABLES - 1; i >= 0; i--) {
    for (int i = 0; i <= MAX_NUMBER_OF_DRAWABLES; i++) {
        if (!(gfx->drawables[i].flags & DrawFlag_Active)) continue;

        Drawable* d = &gfx->drawables[i];

        Vector3 pos = d->transform.translation;

        Matrix m = MatrixIdentity();
        m = MatrixMultiply(m, QuaternionToMatrix(d->transform.rotation));
        m = MatrixMultiply(m, MatrixTranslate(pos.x, pos.y, pos.z));

        if (d->type == DrawType_Billboard) {
            DrawBillboard(*camera, d->billboard.texture, pos,
                          d->billboard.scale, d->diffuse);

        } else if (d->type == DrawType_Model) {
            d->model.transform = m;
            DrawModel(d->model, d->transform.translation, 1, d->diffuse);
        }
    }

    gfx->num_drawables = 0;
}
