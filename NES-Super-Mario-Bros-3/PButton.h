#pragma once

#include "HelpfulObject.h"
#include "Animation.h"
#include "Animations.h"

#define PBUTTON_CONSTRUCTED 0
#define PBUTTON_PRESSED 1

#define ANI_PBUTTON_BASE 7500

#define BUTTON_BBOX_WIDTH 16
#define BUTTON_BBOX_HEIGHT 16

#define ACTION_PRESS_BUTTON 10

class CPButton : public CHelpfulObject
{
public:
	CPButton(float x, float y) : CHelpfulObject(x, y)
	{
		state = PBUTTON_CONSTRUCTED;
		SetBoundingBox(BUTTON_BBOX_WIDTH, BUTTON_BBOX_HEIGHT);
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void Reaction_Brick_To_Coin();
	void Reaction_Coin_To_Brick();
	void Reaction(CGameObject* by_another, int action);

	int IsCollidable() { return 1; }
	int IsBlocking() { return 0; }
};