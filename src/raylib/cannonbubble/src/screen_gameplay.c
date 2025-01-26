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

Shader shader = { 0 };

int iTimeDeltaLoc = 0;
float frameTime = 0.0f;

int ballsPosLoc = 0;
Vector2 ballsPos[4] = { { 0.46f , 0.57f }, { 0.5f, 0.55f }, { 0.4f, 0.6f }, { 0.6f, 0.7f } };

int ballsRadiusLoc = 0;
float ballsRadius[4] = { 0.05, 0.09, 0.12, 0.075 };

int ballsHowManyLoc = 0;
int ballsHowMany = 1;

int iResolutionLoc = 0;
Vector2 iResolution = { 0 };

int iCenterLoc = 0;
Vector2 iCenter = { 0 };

Camera2D camera = { 0 };

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

    ballsHowManyLoc = GetShaderLocation(shader, "ballsHowMany");

    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    
    iResolutionLoc = GetShaderLocation(shader, "iResolution");
    iResolution = (Vector2){ (float)screenWidth, (float)screenHeight };

    iCenterLoc = GetShaderLocation(shader, "iCenter");
    iCenter = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };

    camera.target = (Vector2){ 0, 0 };
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!
    TickGameState(&gameState);

    ballsPos[0] = Vector2Add((Vector2) { 0.46f, 0.57f }, (Vector2) { cosf(frameTime)*0.001f, sinf(frameTime)*0.001f });
    ballsPos[1] = Vector2Add((Vector2) { 0.5f, 0.55f }, (Vector2) { cosf(frameTime*2) * 0.001f, -sinf(frameTime) * 0.001f
    });
    ballsPos[2] = Vector2Add((Vector2) { 0.4f, 0.6f }, (Vector2) { -cosf(frameTime) * 0.001f, sinf(frameTime*2)*0.001f });
    ballsPos[3] = Vector2Add((Vector2) { 0.6f, 0.7f }, (Vector2) { -cosf(frameTime*3) * 0.001f, -sinf(frameTime*2)*0.001f });

    frameTime += GetFrameTime();

    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }
}

void RenderTextureGameplayScreen(void)
{
    // METABALL RENDERING
    // Send new value to the shader to be used on drawing
    SetShaderValue(shader, iTimeDeltaLoc, &frameTime, SHADER_UNIFORM_FLOAT);
    SetShaderValueV(shader, ballsPosLoc, ballsPos, SHADER_UNIFORM_VEC2, 4);
    SetShaderValueV(shader, ballsRadiusLoc, ballsRadius, SHADER_UNIFORM_VEC2, 4);
    SetShaderValue(shader, ballsHowManyLoc, &ballsHowMany, SHADER_UNIFORM_INT);

    SetShaderValue(shader, iResolutionLoc, &iResolution, SHADER_UNIFORM_VEC2);
    SetShaderValue(shader, iCenterLoc, &iCenter, SHADER_UNIFORM_VEC2);
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    // THE REST OF THE GAME
    ClearBackground(DARKPURPLE);  // Clear screen background

    // Enable shader using the custom uniform
    BeginShaderMode(shader);
    // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
    // DrawTextureRec(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2) { 0, 0 }, WHITE);
    BeginMode2D(camera);
    DrawRectangle(screenWidth * -0.5f, screenHeight * -0.5f, screenWidth, screenHeight, WHITE);
    EndMode2D();
    EndShaderMode();

    DrawFPS(10, 10);
    
    DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
    UnloadShader(shader);
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}