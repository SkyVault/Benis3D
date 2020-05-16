/**********************************************************************************************
 *
 *   raylib.lights - Some useful functions to deal with lights data
 *
 *   CONFIGURATION:
 *
 *   #define RLIGHTS_IMPLEMENTATION
 *       Generates the implementation of the library into the included file.
 *       If not defined, the library is in header only mode and can be included
 *in other headers or source files without problems. But only ONE file should
 *hold the implementation.
 *
 *   LICENSE: zlib/libpng
 *
 *   Copyright (c) 2017-2019 Victor Fisac (@victorfisac) and Ramon Santamaria
 *(@raysan5)
 *
 *   This software is provided "as-is", without any express or implied warranty.
 *In no event will the authors be held liable for any damages arising from the
 *use of this software.
 *
 *   Permission is granted to anyone to use this software for any purpose,
 *including commercial applications, and to alter it and redistribute it freely,
 *subject to the following restrictions:
 *
 *     1. The origin of this software must not be misrepresented; you must not
 *claim that you wrote the original software. If you use this software in a
 *product, an acknowledgment in the product documentation would be appreciated
 *but is not required.
 *
 *     2. Altered source versions must be plainly marked as such, and must not
 *be misrepresented as being the original software.
 *
 *     3. This notice may not be removed or altered from any source
 *distribution.
 *
 **********************************************************************************************/

#ifndef RLIGHTS_H
#define RLIGHTS_H

#include "raylib.h"

#define MAX_LIGHTS 6  // Max dynamic lights supported by shader

// Light data
typedef struct {
    int type;
    Vector3 position;
    Vector3 target;
    Color color;
    bool enabled;

    // Shader locations
    int enabledLoc;
    int typeLoc;
    int posLoc;
    int targetLoc;
    int colorLoc;
} Light;

// Light type
typedef enum { LIGHT_DIRECTIONAL, LIGHT_POINT } LightType;

static int lightsCount = 0;  // Current amount of created lights

Light CreateLight(int type, Vector3 position, Vector3 target, Color color,
                  Shader shader);  // Create a light and get shader locations
void UpdateLightValues(Shader shader,
                       Light light);  // Send light properties to shader
// void InitLightLocations(Shader shader, Light *light);     // Init light
// shader locations
#endif