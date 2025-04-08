#pragma once

#include "Block.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_PIPE 14000
#define PIPE_WIDTH 32
#define PIPE_BBOX_WIDTH 32
#define PIPE_BBOX_HEIGHT 51

class CPipe : public CBlock {
public:
	CPipe(float x, float y) : CBlock(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};