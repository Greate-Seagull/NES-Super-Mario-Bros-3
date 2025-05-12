#pragma once

#include "Game.h"
#include "PlayScene.h"
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define HUD_WIDTH 256
#define HUD_HEIGHT 48

#define DIGIT_WIDTH 8
#define DIGIT_HEIGHT 8

#define P_ARROW_WIDTH 8
#define P_ARROW_HEIGHT 8
#define P_SWITCH_WIDTH 16
#define P_SWITCH_HEIGHT 8

#define ID_ANI_HUD -7000

#define ID_ANI_DIGIT_EMPTY -7001
#define ID_ANI_DIGIT_BASE -7010

#define ID_ANI_P_ARROW_OFF -7020
#define ID_ANI_P_ARROW_ON -7021
#define ID_ANI_P_SWITCH_OFF -7030
#define ID_ANI_P_SWITCH_ON -7031

#define DIGIT_COUNT_SCORE 7
#define DIGIT_COUNT_CURRENCY 2
#define DIGIT_COUNT_TIME 3
#define P_METER_COUNT 7

#define DIGIT_NEAR_SPACING 8

#define SCORE_OFFSET 68
#define CURRENCY_OFFSET 148
#define TIME_OFFSET 140

#define P_METER_OFFSET 68

#define OFFSET_Y_LINE1 168
#define OFFSET_Y_LINE2 176

#define TIMER_VALUE 300000

#define P_PROGRESS_DELAY 250

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
			case 0:	aniID = ID_ANI_DIGIT_BASE - digit; break;
			case 1:	aniID = ID_ANI_DIGIT_EMPTY; break;
		}
	}

	void GetOriginalPos(float& ox, float& oy) { ox = originalX; oy = originalY; }
	void SetEmpty(bool isEmpty) { this->isEmpty = isEmpty; }
	void SetDigit(int digit) { this->digit = digit; }
};

class CPMeter : public CGameObject {
protected:
	int pType;
	bool isToggled;

	float originalX, originalY;
public:
	CPMeter(float x, float y, int pType, bool isToggled) : CGameObject(x, y)
	{
		this->pType = pType;
		this->isToggled = isToggled;

		originalX = x;
		originalY = y;

		if (pType == 0) SetBoundingBox(P_ARROW_WIDTH, P_ARROW_HEIGHT);
		else SetBoundingBox(P_SWITCH_WIDTH, P_SWITCH_HEIGHT);
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
	{
		switch (pType)
		{
			case 0:
				switch (isToggled)
				{
				case 0:	aniID = ID_ANI_P_ARROW_OFF; break;
				case 1:	aniID = ID_ANI_P_ARROW_ON; break;
				}
				break;
			case 1:
				switch (isToggled)
				{
				case 0: aniID = ID_ANI_P_SWITCH_OFF; break;
				case 1: aniID = ID_ANI_P_SWITCH_ON; break;
				}
				break;
		}
	}

	void GetOriginalPos(float& ox, float& oy) { ox = originalX; oy = originalY; }
	void SetToggle(bool isToggled) 
	{ 
		if (this->isToggled != isToggled)
			this->isToggled = isToggled;
	}
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

	void GetOriginalPos(float& ox, float& oy) { ox = originalX; oy = originalY; }
};