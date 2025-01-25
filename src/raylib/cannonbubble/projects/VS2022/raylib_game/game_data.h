
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
	Vector2 pos;
	float size;
};

typedef struct bubble Bubble;

struct bubble {
	MBall balls[MAX_BALLS];
	bool active_balls[MAX_BALLS];
};

void TickBubble(Bubble* bubble);
bool IsBubbleAlive(const Bubble* bubble);

typedef struct gamestate GameState;

struct gamestate {
	Bubble bubbles[MAX_BUBBLES];
	float active_bubbles[MAX_BUBBLES];
};

void InitGameState(GameState* gameState);
void TickGameState(GameState* gameState);

#ifdef __cplusplus
}
#endif

#endif // GAMEDATA_H