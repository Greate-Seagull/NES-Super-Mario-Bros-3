#pragma once

#include "Block.h"
#include "Animation.h"
#include "Animations.h"

#define TRIGGER_WIDTH 10
#define TRIGGER_HEIGHT 32

class CDeadStateTrigger : public CGameObject
{
protected:
	float width, height;
public:
	CDeadStateTrigger(float x, float y, float width, float height) : CGameObject(x, y)
	{
		this->width = width; //GameObject có bbox_width
		this->height = height; //GameObject có bbox_height

		//bbox_width = width;
		//bbox_height = height;
	}
	void Render();
	void Update(DWORD dt) {} //GameObject co ham update roi! "virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};"
	void GetBoundingBox(float& l, float& t, float& r, float& b); //GameObject có GetBoundingBox rồi! "virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);"
};