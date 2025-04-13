#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BACKGROUND 0
#define BACKGROUND_WIDTH 2560
#define BACKGROUND_HEIGHT 393

class CBackground : public CGameObject {
public:
	CBackground(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};