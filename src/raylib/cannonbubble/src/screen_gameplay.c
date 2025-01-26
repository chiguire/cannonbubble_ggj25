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
#include "raymath.h"
#include "screens.h"
#include "game_data.h"
#include <math.h>

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

int iTimeDeltaLoc = 0;
float frameTime = 0.0f;

int ballsPosLoc = 0;
Vector2 ballsPos[4] = { { -50, -50 }, { 40, -50 }, { 40, 60 }, { -60, 55 } };

int ballsRadiusLoc = 0;
float ballsRadius[4] = { 15, 20, 25, 12 };

int ballsHowManyLoc = 0;
int ballsHowMany = 4;

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

    shader = LoadShader(0, TextFormat("resources/shaders/glsl%i/metaball.fs.glsl", GLSL_VERSION));

    // Get variable (uniform) location on the shader to connect with the program
    // NOTE: If uniform variable could not be found in the shader, function returns -1
    iTimeDeltaLoc = GetShaderLocation(shader, "iTimeDelta");
    frameTime = 0.0f;

    ballsPosLoc = GetShaderLocation(shader, "ballsPos");
    
    ballsRadiusLoc = GetShaderLocation(shader, "ballsRadius");

    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    // Create a RenderTexture2D to be used for render to texture
    target = LoadRenderTexture(screenWidth, screenHeight);
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!
    TickGameState(&gameState);

    ballsPos[0] = Vector2Add((Vector2) { -50 , -50 }, (Vector2) { cosf(frameTime), sinf(frameTime) });
    ballsPos[1] = Vector2Add((Vector2) { 40, -50 }, (Vector2) { cosf(frameTime*2), -sinf(frameTime) });
    ballsPos[2] = Vector2Add((Vector2) { 40, 60 }, (Vector2) { -cosf(frameTime), sinf(frameTime*2) });
    ballsPos[3] = Vector2Add((Vector2) { -60, 55 }, (Vector2) { -cosf(frameTime*3), -sinf(frameTime*2) });

    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }
    frameTime += GetFrameTime();
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    // METABALL RENDERING
    // Send new value to the shader to be used on drawing
    SetShaderValue(shader, iTimeDeltaLoc, &frameTime, SHADER_UNIFORM_FLOAT);
    SetShaderValueV(shader, ballsPosLoc, ballsPos, SHADER_UNIFORM_VEC2, 4);
    SetShaderValueV(shader, ballsRadiusLoc, ballsRadius, SHADER_UNIFORM_VEC2, 4);
    SetShaderValue(shader, ballsHowManyLoc, &ballsHowMany, SHADER_UNIFORM_INT);

    //----------------------------------------------------------------------------------
    // Draw
    //----------------------------------------------------------------------------------
    BeginTextureMode(target);       // Enable drawing to texture
    ClearBackground(WHITE);  // Clear texture background
    EndTextureMode();               // End drawing to texture (now we have a texture available for next passes)

    // THE REST OF THE GAME
    ClearBackground(RAYWHITE);  // Clear screen background

    // Enable shader using the custom uniform
    BeginShaderMode(shader);
    // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
    DrawTextureRec(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2) { 0, 0 }, WHITE);
    EndShaderMode();

    DrawFPS(10, 10);
    
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