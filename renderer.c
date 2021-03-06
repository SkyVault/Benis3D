#include "renderer.h"

GfxState *create_gfx_state() {
  GfxState *gfx = malloc(sizeof(GfxState));
  for (int i = 0; i < MAX_NUMBER_OF_DRAWABLES; i++) {
    gfx->opaque_drawables[i].flags = 0;
    gfx->transparent_drawables[i].flags = 0;
  }
  gfx->num_opaque_drawables = 0;
  gfx->num_transparent_drawables = 0;
  gfx->num_gui_3d_drawables = 0;

  gfx->render_texture = rlLoadRenderTexture(RESOLUTION_WIDTH, RESOLUTION_HEIGHT,
                                            UNCOMPRESSED_R8G8B8A8, 24, true);
  // LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

  return gfx;
}

void update_billboard(EcsWorld *ecs, EntId ent) {}

void draw_billboard_ent(GfxState *gfx, Camera *camera, EcsWorld *ecs,
                        EntId ent) {
  EntStruct *self = get_ent(ecs, ent);

  if (ent < 0 || self == NULL)
    return;

  if (!has_comp(ecs, self, Billboard) || !has_comp(ecs, self, Transform))
    return;

  if (gfx->num_transparent_drawables < MAX_NUMBER_OF_DRAWABLES) {
    Billboard b = *get_comp(ecs, self, Billboard);
    gfx->transparent_drawables[gfx->num_transparent_drawables++] = (Drawable){
        .type = DrawType_Billboard,
        .flags = DrawFlag_Active,
        .transform = *get_comp(ecs, self, Transform),
        .billboard = b,
        .diffuse = WHITE,
        .region = (Rectangle){0, 0, b.texture.width, b.texture.height},
    };
  }
}

void draw_prop(GfxState *gfx, Assets *assets, Prop prop) {
  if (gfx->num_transparent_drawables < MAX_NUMBER_OF_DRAWABLES) {
    gfx->transparent_drawables[gfx->num_transparent_drawables++] = (Drawable){
        .type = DrawType_Billboard,
        .flags = DrawFlag_Active,
        .region = prop.region,
        .transform =
            (Transform){
                .translation = prop.position,
                .rotation = QuaternionIdentity(),
                .scale = (Vector3){1, 1, 1},
            },
        .billboard = (Billboard){.texture = assets->textures[TEX_PROPS],
                                 .material = {0},
                                 .scale = prop.scale},
        .diffuse = WHITE,
    };
  }
}

void draw_billboard(GfxState *gfx, Vector3 position, Texture texture,
                    Rectangle region, float scale) {
  if (gfx->num_transparent_drawables < MAX_NUMBER_OF_DRAWABLES) {
    gfx->transparent_drawables[gfx->num_transparent_drawables++] = (Drawable){
        .type = DrawType_Billboard,
        .flags = DrawFlag_Active,
        .region = region,
        .transform =
            (Transform){
                .translation = position,
                .rotation = QuaternionIdentity(),
                .scale = (Vector3){1, 1, 1},
            },
        .billboard =
            (Billboard){.texture = texture, .material = {0}, .scale = scale},
        .diffuse = WHITE,
    };
  }
}

void update_models(EcsWorld *ecs, EntId ent) {}

void add_opaque_drawable(GfxState *gfx, Drawable drawable) {}

void add_transparent_drawable(GfxState *gfx, Drawable drawable) {}

void draw_model(GfxState *gfx, Model *model, Transform transform,
                Color diffuse) {
  if (diffuse.a < 255) {
    if (gfx->num_transparent_drawables < MAX_NUMBER_OF_DRAWABLES) {
      gfx->transparent_drawables[gfx->num_transparent_drawables++] = (Drawable){
          .type = DrawType_Model,
          .flags = DrawFlag_Active,
          .transform = transform,
          .model = *model,
          .diffuse = diffuse,
      };
    }
  } else {
    if (gfx->num_opaque_drawables < MAX_NUMBER_OF_DRAWABLES) {
      gfx->opaque_drawables[gfx->num_opaque_drawables++] = (Drawable){
          .type = DrawType_Model,
          .flags = DrawFlag_Active,
          .transform = transform,
          .model = *model,
          .diffuse = diffuse,
      };
    }
  }
}

void draw_gui_model(GfxState *gfx, Model *model, Rectangle where,
                    Vector3 rotation, Color diffuse) {

  if (model->meshCount <= 0) return;

  Transform transform = transform_identity();

  BoundingBox box = MeshBoundingBox(model->meshes[0]);

  float w = box.max.x - box.min.x;
  float h = box.max.y - box.min.y;

  float scaleW = (where.width / w) / 8;
  float scaleH = (where.height / h) / 8;

  float x = where.x/8 + w + scaleW / 2;
  float y = -(where.y/8 + h + scaleH / 2);

  float aspect = (float)GetScreenHeight() / (float)GetScreenWidth();

  const float cx = -80;
  const float cy = 80 * aspect;

  transform.translation = (Vector3){cx + x, cy + y, 0};
  transform.scale = (Vector3){scaleW, scaleH, (scaleW + scaleH) / 2.0f};
  transform.rotation = QuaternionFromEuler(rotation.x, rotation.y, rotation.z);

  if (gfx->num_gui_3d_drawables < MAX_NUMBER_OF_3D_GUI_DRAWABLES) {
    gfx->gui_3d_drawables[gfx->num_gui_3d_drawables++] = (Drawable){
        .type = DrawType_Model,
        .flags = DrawFlag_Active,
        .transform = transform,
        .model = *model,
        .diffuse = WHITE,
    };
  }
}

void draw_models(GfxState *gfx, EcsWorld *ecs, EntId ent) {
  EntStruct *self = get_ent(ecs, ent);

  if (ent < 0 || self == NULL)
    return;

  if (!has_comp(ecs, self, Model) || !has_comp(ecs, self, Transform))
    return;

  Model *model = get_comp(ecs, self, Model);
  Color diffuse = model->materials[0].maps[MAP_DIFFUSE].color; // paranoia?

  if (diffuse.a < 255) {
    if (gfx->num_transparent_drawables < MAX_NUMBER_OF_DRAWABLES) {
      gfx->transparent_drawables[gfx->num_transparent_drawables++] = (Drawable){
          .type = DrawType_Model,
          .flags = DrawFlag_Active,
          .transform = *get_comp(ecs, self, Transform),
          .model = *model,
          .diffuse = diffuse,
      };
    }
  } else {
    if (gfx->num_opaque_drawables < MAX_NUMBER_OF_DRAWABLES) {
      gfx->opaque_drawables[gfx->num_opaque_drawables++] = (Drawable){
          .type = DrawType_Model,
          .flags = DrawFlag_Active,
          .transform = *get_comp(ecs, self, Transform),
          .model = *model,
          .diffuse = diffuse,
      };
    }
  }
}

static Camera *scamera = NULL;

int sorting(const void *_a, const void *_b) {
  Drawable *a = (Drawable *)_a;
  Drawable *b = (Drawable *)_b;

  if (scamera == NULL)
    return 0;

  // Cache this?
  float ad = Vector3Distance(a->transform.translation, scamera->position);
  float bd = Vector3Distance(b->transform.translation, scamera->position);

  return (ad < bd) ? 1 : -1;
}

void render_drawable(Drawable *d, Camera *camera) {
  Vector3 pos = d->transform.translation;
  Vector3 scale = d->transform.scale;

  Matrix m = MatrixIdentity();
  m = MatrixMultiply(m, MatrixScale(scale.x, scale.y, scale.z));
  m = MatrixMultiply(m, QuaternionToMatrix(d->transform.rotation));
  m = MatrixMultiply(m, MatrixTranslate(pos.x, pos.y, pos.z));

  const Vector4 fogColor = (Vector4){0.f, 0.f, 0.f, 1.0f};
  const float fogDensity = 0.015;

  if (d->type == DrawType_Billboard) {
    Vector3 f = {d->diffuse.r / 255.f, d->diffuse.g / 255.f,
                 d->diffuse.b / 255.f};

    Vector2 sub = (Vector2){
        d->transform.translation.x,
        d->transform.translation.z,
    };

    float dist =
        Vector2Distance(sub, (Vector2){camera->position.x, camera->position.z});

    float fogFactor = 1.0f / exp((dist * fogDensity * 8) * (dist * fogDensity));

    fogFactor = fogFactor < 0 ? 0 : (fogFactor > 1.0f ? 1.0f : fogFactor);

    f.x = (f.x + (f.x * fogFactor)) - 1.0f;
    f.y = (f.y + (f.y * fogFactor)) - 1.0f;
    f.z = (f.z + (f.z * fogFactor)) - 1.0f;

    if (f.x < 0) f.x = 0;
    if (f.y < 0) f.y = 0;
    if (f.z < 0) f.z = 0;

    if (f.x > 1) f.x = 1;
    if (f.y > 1) f.y = 1;
    if (f.z > 1) f.z = 1;

    d->diffuse = VEC3_TO_COLOR(f);

    DrawBillboardRec(*camera, d->billboard.texture, d->region, pos,
                     d->billboard.scale, d->diffuse);

  } else if (d->type == DrawType_Model) {
    d->model.transform = m;
    DrawModel(d->model, Vector3Zero(), 1, d->diffuse);
  }
}

void begin_rendering(GfxState *gfx) {
  BeginTextureMode(gfx->render_texture);
  ClearBackground((Color){0, 0, 0, 100});
}

void end_rendering(GfxState *gfx) { EndTextureMode(); }

void draw_final_texture_to_screen(GfxState *gfx) {
  DrawTexturePro(
      gfx->render_texture.texture,
      (Rectangle){0, 0, gfx->render_texture.texture.width,
                  -gfx->render_texture.texture.height},
      (Rectangle){0, 0, GetScreenWidth(), GetScreenWidth() * RESOLUTION_ASPECT},
      (Vector2){0, 0}, 0.0f, WHITE);
}

void draw_root_node(GfxState *gfx, Node *node) {
  if (!node)
    return;

  Transform transform = get_transform_from_node(node);

  // TODO(Dustin): @important dont draw the gizmo when
  // not in edit mode, we probably have to move this to the editor somehow
  // DrawGizmo(transform.translation);

  switch (node->type) {
  case NODE_TYPE_MODEL: {
    draw_model(gfx, &node->model, transform, WHITE);
    break;
  }

  case NODE_TYPE_BILLBOARD: {
    draw_billboard(gfx, transform.translation, node->billboard.texture,
                   (Rectangle){0, 0, node->billboard.texture.width,
                               node->billboard.texture.height},
                   1.0f);
    break;
  }

  default: {
  }

  }

  if (node->next) {
    draw_root_node(gfx, node->next);
  }

  if (node->child) {
    draw_root_node(gfx, node->child);
  }
}

// Draws the model into the rectangle on screen
void draw_model_2d(GfxState *gfx, Model model, Rectangle region) {
  Camera camera = {};
  camera.position = (Vector3){0.0f, 0, 0.0f};
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  camera.fovy = 45.0f;
  camera.type = CAMERA_PERSPECTIVE;

  BeginMode3D(camera);
  DrawModel(model, (Vector3){0, 0, 0}, 1.0, WHITE);
  EndMode3D();
}

void flush_graphics(GfxState *gfx, Camera *camera) {
  scamera = camera;

  // for (int i = MAX_NUMBER_OF_DRAWABLES - 1; i >= 0; i--) {
  for (int i = 0; i <= gfx->num_opaque_drawables; i++) {
    if (!(gfx->opaque_drawables[i].flags & DrawFlag_Active))
      continue;
    Drawable *d = &gfx->opaque_drawables[i];
    render_drawable(d, camera);
    gfx->opaque_drawables[i] = (Drawable){0};
  }

  gfx->num_opaque_drawables = 0;

  qsort(gfx->transparent_drawables, gfx->num_transparent_drawables,
        sizeof(Drawable), sorting);

  for (int i = 0; i <= gfx->num_transparent_drawables; i++) {
    if (!(gfx->transparent_drawables[i].flags & DrawFlag_Active))
      continue;
    Drawable *d = &gfx->transparent_drawables[i];
    render_drawable(d, camera);
    gfx->transparent_drawables[i] = (Drawable){0};
  }
  gfx->num_transparent_drawables = 0;
}

void flush_gui_3d_graphics(GfxState *gfx) {
  Camera ocamera = (Camera){
      .position = (Vector3){0.0f, 0.0f, 1.0f},
      .target = (Vector3){0.0f, 0.0f, 0.0f},
      .up = (Vector3){0.0f, 1.0f, 0.0f},
      .fovy = FOV,
      .type = CAMERA_ORTHOGRAPHIC,
  };

  rlglDraw(); // Draw Buffers (Only OpenGL 3+ and ES2)

  rlMatrixMode(RL_PROJECTION); // Switch to projection matrix
  rlPushMatrix(); // Save previous matrix, which contains the settings for the
  // 2d ortho projection
  rlLoadIdentity(); // Reset current matrix (projection)

  float aspect = (float)GetScreenWidth() / (float)GetScreenHeight();

  // Setup orthographic projection
  double top = ocamera.fovy / 2.0;
  double right = top * aspect;

  rlOrtho(-right, right, -top, top, -100, 100);

  // NOTE: zNear and zFar values are important when computing depth buffer
  // values

  rlMatrixMode(RL_MODELVIEW); // Switch back to modelview matrix
  rlLoadIdentity();           // Reset current matrix (modelview)

  // Setup Camera view
  Matrix matView = MatrixLookAt(ocamera.position, ocamera.target, ocamera.up);
  rlMultMatrixf(MatrixToFloat(
      matView)); // Multiply modelview matrix by view matrix (camera)

  rlEnableDepthTest(); // Enable DEPTH_TEST for 3D

  for (int i = 0; i <= gfx->num_gui_3d_drawables; i++) {
    if (!(gfx->gui_3d_drawables[i].flags & DrawFlag_Active))
      continue;

    Drawable *d = &gfx->gui_3d_drawables[i];
    render_drawable(d, &ocamera);
    gfx->gui_3d_drawables[i] = (Drawable){0};
  }
  gfx->num_gui_3d_drawables = 0;

  EndMode3D();
}

Transform transform_identity() {
  Transform transform;
  transform.translation = Vector3Zero();
  transform.rotation = QuaternionIdentity();
  transform.scale = Vector3One();
  return transform;
}

Transform transform_translation(Vector3 v) {
  Transform transform = transform_identity();
  transform.translation = v;
  return transform;
}
