#pragma once

#include "Block.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_QUESTION_BLOCK 12000
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define SHAKE_VELOCITY 5

class CQuestionBlock : public CBlock {
public:
	CQuestionBlock(float x, float y) : CBlock(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};