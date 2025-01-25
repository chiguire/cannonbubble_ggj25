/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"
#include "game_data.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif


//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

int screenWidth = 0;
int screenHeight = 0;

GameState gameState = { 0 };

Camera camera = { 0 };
Shader shader = { 0 };
float swirlCenter[2] = { 0, 0 };
int swirlCenterLoc = -1;
RenderTexture2D target = { 0 };

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;

    InitGameState(&gameState);

    shader = LoadShader(0, TextFormat("resources/shaders/glsl%i/metaball.fs", GLSL_VERSION));

    // Get variable (uniform) location on the shader to connect with the program
    // NOTE: If uniform variable could not be found in the shader, function returns -1
    swirlCenterLoc = GetShaderLocation(shader, "center");

    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    swirlCenter[0] = (float)screenWidth / 2;
    swirlCenter[1] = (float)screenHeight / 2;

    // Create a RenderTexture2D to be used for render to texture
    target = LoadRenderTexture(screenWidth, screenHeight);
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!
    TickGameState(&gameState);

    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    // Send new value to the shader to be used on drawing
    SetShaderValue(shader, swirlCenterLoc, swirlCenter, SHADER_UNIFORM_VEC2);
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginTextureMode(target);       // Enable drawing to texture
    ClearBackground(RAYWHITE);  // Clear texture background

    BeginMode3D(camera);        // Begin 3d mode drawing
    DrawGrid(10, 1.0f);     // Draw a grid
    EndMode3D();                // End 3d mode drawing, returns to orthographic 2d mode

    DrawText("TEXT DRAWN IN RENDER TEXTURE", 200, 10, 30, RED);
    EndTextureMode();               // End drawing to texture (now we have a texture available for next passes)

    ClearBackground(RAYWHITE);  // Clear screen background

    // Enable shader using the custom uniform
    BeginShaderMode(shader);
    // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
    DrawTextureRec(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2) { 0, 0 }, WHITE);
    EndShaderMode();

    // Draw some 2d text over drawn texture
    DrawText("(c) Barracks 3D model by Alberto Cano", screenWidth - 220, screenHeight - 20, 10, GRAY);
    DrawFPS(10, 10);
    
    Vector2 pos = { 20, 10 };
    DrawTextEx(font, "GAMEPLAY SCREEN", pos, font.baseSize*3.0f, 4, MAROON);
    DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}