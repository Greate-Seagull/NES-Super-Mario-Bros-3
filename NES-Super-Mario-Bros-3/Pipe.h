#pragma once

#include "Block.h"
#include "Animation.h"
#include "Animations.h"

#define PIPE_WIDTH 32
#define PIPE_BBOX_WIDTH 32
#define PIPE_BBOX_HEIGHT 43

class CPipe : public CBlock {
protected:
	int ani_id;
public:
	CPipe(float x, float y) : CBlock(x, y) {}
	CPipe(float x, float y, int ani_id) : CBlock(x, y) {
		this->ani_id = ani_id;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};