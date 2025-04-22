#pragma once

#include "MovableObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 40000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10.0f
#define COIN_BBOX_HEIGHT 16.0f

#define COIN_VELOCITY_Y -0.55f
#define COIN_ACCELERATION_Y	0.05f

class CCoin : public CMovableObject {
protected:
	bool isToggled;
	bool isDisappear;
	float originalY;
public:
	CCoin(float x, float y) : CMovableObject(x, y) 
	{
		this->isDisappear = false;
		originalY = y;
		vy = COIN_VELOCITY_Y;

		aniID = ID_ANI_COIN;

		this->SetBoundingBox(COIN_BBOX_WIDTH, COIN_BBOX_HEIGHT);
	}
	CCoin(float x, float y, bool isToggled) : CMovableObject(x, y)
	{
		this->isToggled = isToggled;
		this->isDisappear = false;
		originalY = y;
		vy = COIN_VELOCITY_Y;

		aniID = ID_ANI_COIN;
		
		this->SetBoundingBox(COIN_BBOX_WIDTH, COIN_BBOX_HEIGHT);
	}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	int IsCollidable() { return 1; } //Them de lay colliable object
	int IsBlocking() { return 0; }

	void Reaction(CGameObject* by_another, int action);

	void SetDisappear(bool Disappear) { this->isDisappear = Disappear; }
	bool GetDisappear() { return isDisappear; }
	bool IsUnderOriginal();
};