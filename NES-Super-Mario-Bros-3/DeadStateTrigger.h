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
		this->width = width;
		this->height = height;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};