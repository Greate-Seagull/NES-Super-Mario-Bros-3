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

class CCoin : public CMovableObject { //GameObject không có vx, vy nữa! Đổi thành MovableObject tác dụng vẫn như cũ
protected:
	bool isToggled;
	bool isDisappear;
	float originalY;
public:
	CCoin(float x, float y) : CMovableObject(x, y) {}
	CCoin(float x, float y, bool isToggled) : CMovableObject(x, y)
	{
		this->isToggled = isToggled;
		this->isDisappear = false;
		originalY = y;
		vy = COIN_VELOCITY_Y;

		aniID = ID_ANI_COIN; //Thêm dòng này để dùng CGameObject::Render()

		bbox_width = COIN_BBOX_WIDTH; //Thêm dòng này để dùng CGameObject::GetBoundingBox(float& l, float& t, float& r, float& b)
		bbox_height = COIN_BBOX_HEIGHT;
	}
	void Render(); //GameObject có Render rồi! "virtual void Render();"
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b); //GameObject có GetBoundingBox rồi! "virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);"
	int IsBlocking() { return 0; }

	bool GetToggled() { return isToggled; }
	void SetDisappear(bool Disappear) { this->isDisappear = Disappear; }
	bool GetDisappear() { return isDisappear; }
	bool IsUnderOriginal();
};