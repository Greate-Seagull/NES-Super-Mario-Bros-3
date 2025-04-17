#pragma once

#include "Block.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_STRIPED_BRICK 13000
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

class CStripedBrick : public CBlock {
public:
	CStripedBrick(float x, float y) : CBlock(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};