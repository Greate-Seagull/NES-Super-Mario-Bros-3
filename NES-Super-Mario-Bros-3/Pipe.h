#pragma once

#include "Block.h"
#include "Animation.h"
#include "Animations.h"

#define PIPE_WIDTH 30
#define PIPE_BBOX_WIDTH 28

class CPipe : public CBlock {
protected:
	int ani_id;

	float pipe_height;
public:
	CPipe(float x, float y) : CBlock(x, y) {}
	CPipe(float x, float y, int ani_id, int pipe_height) : CBlock(x, y) {
		this->ani_id = ani_id;
		this->pipe_height = pipe_height;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};