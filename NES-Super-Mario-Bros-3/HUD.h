#pragma once

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
#define MOMENTUM_COUNT 7

#define DIGIT_NEAR_SPACING 8

#define SCORE_OFFSET -60
#define CURRENCY_OFFSET 20
#define TIME_OFFSET 12
#define P_METER_OFFSET -60

#define OFFSET_Y_LINE1 -12
#define OFFSET_Y_LINE2 -4

#define P_PROGRESS_DELAY 150

class CDigit
{
protected:
	bool isEmpty;
	int digit;

	float x, y;
	int aniID;
public:
	CDigit(bool isEmpty, int digit)
	{
		this->isEmpty = isEmpty;
		this->digit = digit;
	}
	void Render()
	{
		switch (isEmpty)
		{
			case 0:	aniID = ID_ANI_DIGIT_BASE - digit; break;
			case 1:	aniID = ID_ANI_DIGIT_EMPTY; break;
		}

		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(aniID)->Render(x, y);
	}
	void SetPosition(float x, float y) { this->x = x; this->y = y; }
	void SetEmpty(bool isEmpty) { this->isEmpty = isEmpty; }
	void SetDigit(int digit) { this->digit = digit; }
};

class CPMeter
{
protected:
	int pType;
	bool isToggled;

	float x, y;
	int aniID;
public:
	CPMeter(int pType, bool isToggled)
	{
		this->pType = pType;
		this->isToggled = isToggled;
	}
	void Render()
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

		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(aniID)->Render(x, y);
	}
	void SetPosition(float x, float y) { this->x = x; this->y = y; }
	void SetToggle(bool isToggled) 
	{ 
		if (this->isToggled != isToggled)
			this->isToggled = isToggled;
	}
};

class CHud
{
protected:
	float x, y;
	int aniID;

	CDigit* scoreDigits[DIGIT_COUNT_SCORE];
	CDigit* coinDigits[DIGIT_COUNT_CURRENCY];
	CDigit* timeDigits[DIGIT_COUNT_TIME];
	CPMeter* pMeter[MOMENTUM_COUNT];
public:
	CHud();
	void UpdateTime(int time);
	void UpdateCoin(int coin);
	void UpdateScore(int score);
	void UpdaeMomentum(int momentum);
	void SetPosition(float x, float y);
	void Render();

	~CHud();
};