#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

#define COIN_VELOCITY_Y -0.55f
#define COIN_ACCELERATION_Y	0.05f

class CCoin : public CGameObject {
protected:
	bool isToggled;
	bool isDisappear;
	float originalY;
public:
	CCoin(float x, float y) : CGameObject(x, y) {}
	CCoin(float x, float y, bool isToggled) : CGameObject(x, y)
	{
		this->isToggled = isToggled;
		this->isDisappear = false;
		originalY = y;
		vy = COIN_VELOCITY_Y;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }

	void SetDisappear(bool Disappear) { this->isDisappear = Disappear; }
	bool GetDisappear() { return isDisappear; }
	bool IsUnderOriginal();
};