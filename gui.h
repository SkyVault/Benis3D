#ifndef BENIS_GUI_H
#define BENIS_GUI_H

#include <stdbool.h>

#include "raylib.h"
#include "tween.h"

typedef int NodeId;

struct NState {
    int active;
    int hot;
    float value;
    int cursor;
};

static struct {
    struct NState states[2048];
    float px, py;
    Font font;

    bool locked;
} GuiState = {.px = 0.0f, .py = 0.0f, .locked = false};

void InitGui();
void DoFrame(NodeId id, float x, float y, float width, float height);
bool DoBtn(NodeId id, float x, float y, float width, float height,
           const char* text);
void DoCenterXLabel(NodeId id, float outer_width, float y, int font_size,
                    const char* text);

void DoModal();
void DoTextInput(NodeId id, char* buffer, size_t buffer_size, float x, float y,
                 float width, float height);

void Lock();
void Unlock();

bool Hot(NodeId id);
bool Active(NodeId id);

#endif  // BENIS_GUI_H