#pragma once

#include "HelpfulObject.h"

#define PBUTTON_CONSTRUCTED 0
#define PBUTTON_PRESSED 1

#define ANI_PBUTTON_BASE 7500

#define BUTTON_BBOX_WIDTH 16.0f
#define BUTTON_BBOX_HEIGHT 16.0f

#define ACTION_PRESS_BUTTON 10

class CPButton : public CHelpfulObject
{
	float timeElapsed = 0;
	bool isSwitched = false;
public:
	CPButton(float x, float y) : CHelpfulObject(x, y)
	{
		state = PBUTTON_CONSTRUCTED;
		SetBoundingBox(BUTTON_BBOX_WIDTH, BUTTON_BBOX_HEIGHT);
	}
	void Prepare(DWORD dt) {};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void ReplaceBricksWithCoins();
	void ReplaceCoinsWithBricks();
	void OnReactionTo(LPCOLLISIONEVENT e, int action);

	int IsCollidable() { return isSwitched == false; }
	int IsBlocking() { return 1; }
};