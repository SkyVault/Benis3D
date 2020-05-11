#include "gui.h"

void InitGui() { GuiState.font = LoadFont("resources/alagard_font.png"); }

void DoFrame(NodeId id, float x, float y, float width, float height) {
    struct NState* state = &GuiState.states[id];

    Color color =
        (!Hot(id)) ? (Color){100, 100, 100, 255} : (Color){200, 200, 200, 255};

    DrawRectangle(x, y, width, height, color);
    DrawRectangleLinesEx((Rectangle){x, y, width, height}, 3, RAYWHITE);
}

bool DoBtn(NodeId id, float x, float y, float width, float height,
           const char* text) {
    struct NState* state = &GuiState.states[id];

    Rectangle rect = {x + GuiState.px, y + GuiState.py, width, height};

    state->hot = CheckCollisionPointRec(GetMousePosition(), rect);
    state->active = state->hot && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    const float size = 30.0f;

    Vector2 measure = MeasureTextEx(GuiState.font, text, size, 1);

    // Draw the button
    DoFrame(id, x, y, width, height);
    DrawTextEx(GuiState.font, text,
               (Vector2){
                   x + width / 2 - measure.x / 2,
                   y + height / 2 - measure.y / 2,
               },
               size, 1, WHITE);

    return Active(id);
}

void DoCenterXLabel(NodeId id, float outer_width, float y, int font_size,
                    const char* text) {
    Vector2 measure = MeasureTextEx(GuiState.font, text, font_size, 1);

    DrawTextEx(GuiState.font, text,
               (Vector2){
                   outer_width / 2 - measure.x / 2,
                   y,
               },
               font_size, 1, WHITE);
}

void DoModal() {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                  (Color){0, 0, 0, 200});
}

void DoTextInput(NodeId id, char* buffer, size_t buffer_size, float x, float y,
                 float width, float height) {
    struct NState* state = &GuiState.states[id];
    DoFrame(id, x, y, width, height);

    const int fsize = 25;
    Vector2 size = MeasureTextEx(GuiState.font, buffer, fsize, 1);
    DrawTextEx(GuiState.font, buffer,
               (Vector2){x + 4, y + height / 2 - size.y / 2}, fsize, 1, BLACK);

    state->hot = CheckCollisionPointRec(GetMousePosition(),
                                        (Rectangle){x, y, width, height});

    if (!state->active) {
        state->active = IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && state->hot;
    } else {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !state->hot)
            state->active = false;

        int key = GetKeyPressed();
        if (key > 0) {
            buffer[state->cursor++] = (char)key;
        }
    }

    if (state->active && (((int)(GetTime() * 3.0f) % 2) == 0)) {
        DrawRectangle(x + 4 + size.x, y + 4, 4, height - 8, BLACK);
    }
}

void Lock() { GuiState.locked = true; }

void Unlock() { GuiState.locked = false; }

bool Active(NodeId id) {
    return GuiState.states[id].active && !GuiState.locked;
}

bool Hot(NodeId id) { return GuiState.states[id].hot && !GuiState.locked; }