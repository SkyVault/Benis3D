#ifndef BENIS_GUI_H
#define BENIS_GUI_H

#include <stdbool.h>

#include "raylib.h"
#include "tween.h"
#include "utils.h"

typedef int NodeId;

struct NState {
    uint8_t active;
    uint8_t hot;
    uint8_t init;
    int cursor;
    float value;
    float last_value;
};

static struct {
    struct NState states[2048];
    float px, py;
    Font font;

    bool locked;
} GuiState = {.px = 0.0f, .py = 0.0f, .locked = false};

void InitGui();
void DoPanel(NodeId id, float x, float y, float width, float height);
void DoFrame(NodeId id, float x, float y, float width, float height);
bool DoBtn(NodeId id, float x, float y, float width, float height,
           const char* text);
void DoCenterXLabel(NodeId id, float outer_width, float y, int font_size,
                    const char* text);

void DoLabel(NodeId id, const char* str, float x, float y, float width,
             float height, int font_size);

void DoModal();
bool DoTextInput(NodeId id, char* buffer, size_t buffer_size, float x, float y,
                 float width, float height);

int DoToggleGroupV(NodeId id, const char* names, float x, float y,
                   float* out_width);

bool DoCheckBox(NodeId id, float x, float y, float width, float height);

Vector4 CTV4(Color c);
Color V4TC(Vector4 v);

Color DoColorPicker(NodeId id, float x, float y, float width, float height,
                    Color color);

float DoSlider(NodeId id, float x, float y, float width, float height,
               float min, float max);

void Lock();
void Unlock();

bool Hot(NodeId id);
bool Active(NodeId id);

#endif  // BENIS_GUI_H