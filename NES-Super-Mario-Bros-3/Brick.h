#pragma once

#include "Block.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BRICK 10000

#define BRICK_WIDTH 16.0f
#define BRICK_BBOX_WIDTH 16.0f
#define BRICK_BBOX_HEIGHT 16.0f

class CBrick : public CBlock {
public:
	CBrick(float x, float y);
	//GameObject co ham update roi! "virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};"
	//GameObject co ham render roi! "virtual void Render();"
};