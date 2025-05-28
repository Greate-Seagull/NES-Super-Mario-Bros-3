#pragma once

#include "MovableObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 40000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10.0f
#define COIN_BBOX_HEIGHT 16.0f

#define COIN_VELOCITY_Y -0.3f

class CCoin : public CMovableObject {
protected:
	bool isToggled;
	bool isDisappear;
	float originalY;

	bool isContained;
	bool isTransformed;
public:
	CCoin(float x, float y) : CMovableObject(x, y) 
	{
		isToggled = false;
		originalY = y;
		isContained = false;

		aniID = ID_ANI_COIN;

		SetBoundingBox(COIN_BBOX_WIDTH, COIN_BBOX_HEIGHT);
	}
	/*CCoin(float x, float y, bool isToggled) : CMovableObject(x, y)
	{
		this->isToggled = isToggled;
		this->isDisappear = false;
		originalY = y;
		vy = COIN_VELOCITY_Y;

		aniID = ID_ANI_COIN;
		
		this->SetBoundingBox(COIN_BBOX_WIDTH, COIN_BBOX_HEIGHT);
	}*/

	void Prepare(DWORD dt);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	int IsCollidable() { return isToggled == false; }
	//int IsBlocking() { return 0; }
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void OnReactionTo(LPCOLLISIONEVENT e, int action);

	bool GetToggled() { return isToggled; }
	void SetToggled();

	void SetDisappear(bool Disappear) { this->isDisappear = Disappear; }
	bool GetDisappear() { return isDisappear; }
	bool IsUnderOriginal();

	void SetContained() { isContained = true; }
	void SetTransformed() { isTransformed = true; }
	bool IsTransformed() { return isTransformed; }
};