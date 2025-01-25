#include "game_data.h"

void InitBall(MBall* ball)
{
	ball->pos = (Vector2){ 0, 0 };
}

void InitBubble(Bubble* bubble)
{
	bubble->pos = (Vector2){ 0, 0 };
	for (int i = 0; i != MAX_BALLS; i++)
	{
		bubble->active_balls[i] = false;
		InitBall(&(bubble->balls[i]));
	}
}

void TickBubble(Bubble* bubble)
{

}

bool IsBubbleAlive(const Bubble* bubble)
{

}

void InitGameState(GameState* gameState)
{
	for (int i = 0; i != MAX_BUBBLES; i++)
	{
		gameState->active_bubbles[i] = false;
		InitBubble(&(gameState->bubbles[i]));
	}

}

void TickGameState(GameState* gameState)
{

}

int FindNextAvailableBubble(GameState* gameState)
{
	for (int i = 0; i != MAX_BUBBLES; i++)
	{
		if (gameState->active_bubbles[i] == false)
		{
			InitBubble(&(gameState->bubbles[i]));
			return i;
		}
	}
	TraceLog(LOG_ERROR, "Ran out of bubbles");
	return -1;
}

void CreateBubble(GameState* gameState)
{
	int new_bubble = FindNextAvailableBubble(gameState);
	gameState->active_bubbles[new_bubble] = true;
	InitBubble(&(gameState->bubbles[new_bubble]));
}