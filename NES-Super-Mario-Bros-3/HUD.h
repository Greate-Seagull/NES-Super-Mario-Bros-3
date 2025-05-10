#pragma once

#include "Game.h"
#include "PlayScene.h"
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_HUD -7000

#define HUD_WIDTH 256
#define HUD_HEIGHT 48

#define DIGIT_WIDTH 8
#define DIGIT_HEIGHT 8

#define ID_ANI_DIGIT_EMPTY -7001

#define ID_ANI_DIGIT_BASE -7010

#define DIGIT_COUNT_SCORE 7
#define DIGIT_COUNT_CURRENCY 2
#define DIGIT_COUNT_TIME 3

#define DIGIT_NEAR_SPACING 8

class CDigit : public CGameObject {
protected:
	bool isEmpty;
	int digit;

	float originalX, originalY;
public:
	CDigit(float x, float y, bool isEmpty, int digit) : CGameObject(x, y)
	{
		this->isEmpty = isEmpty;
		this->digit = digit;

		originalX = x;
		originalY = y;

		SetBoundingBox(DIGIT_WIDTH, DIGIT_HEIGHT);
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
	{
		switch (isEmpty)
		{
		case 0:
			aniID = ID_ANI_DIGIT_BASE - digit;
			break;
		case 1:
			aniID = ID_ANI_DIGIT_EMPTY;
			break;
		}
		DebugOut(L"%f, %f\n", x, y);
	}
	//void Render();
	int IsCollidable() { return 1; }

	void GetOriginalPos(float& ox, float& oy) { ox = originalX; oy = originalY; }
};

class CHud : public CGameObject {
protected:
	float originalX, originalY;
public:
	CHud(float x, float y) : CGameObject(x, y) 
	{ 
		aniID = ID_ANI_HUD;
		originalX = x;
		originalY = y;

		SetBoundingBox(HUD_WIDTH, HUD_HEIGHT);
	}
	int IsCollidable() { return 0; }

	void GetOriginalPos(float& ox, float& oy) { ox = originalX; oy = originalY; }
};