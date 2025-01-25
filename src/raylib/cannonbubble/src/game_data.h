
#ifndef GAMEDATA_H
#define GAMEDATA_H

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

#include "raylib.h"

#define MAX_BALLS 100
#define MAX_BUBBLES 50

typedef struct mball MBall;

struct mball {
	Vector2 pos; // Relative to bubble pos
	float size;
};

void InitBall(MBall* ball);

typedef struct bubble Bubble;

struct bubble {
	Vector2 pos;
	MBall balls[MAX_BALLS];
	bool active_balls[MAX_BALLS];
};

void InitBubble(Bubble* bubble);
void AddBallToBubble(Bubble* bubble, Vector2 pos, float size);
void TickBubble(Bubble* bubble);
bool IsBubbleAlive(const Bubble* bubble);

typedef struct gamestate GameState;

struct gamestate {
	Bubble bubbles[MAX_BUBBLES];
	float active_bubbles[MAX_BUBBLES];
};

void InitGameState(GameState* gameState);
void TickGameState(GameState* gameState);

int FindNextAvailableBubble(GameState* gameState);
void CreateBubble(GameState* gameState);

#ifdef __cplusplus
}
#endif

#endif // GAMEDATA_H