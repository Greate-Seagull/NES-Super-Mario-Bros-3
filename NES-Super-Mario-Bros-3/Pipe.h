#pragma once

#include "Block.h"
#include "Animation.h"
#include "Animations.h"

#define PIPE_WIDTH 30.0f
#define PIPE_BBOX_WIDTH 28.0f

class CPipe : public CBlock {
protected:
	int face_direction;
	int warp_direction;

	int spriteIdBeginBegin, spriteIdEndBegin;
	int spriteIdBeginEnd, spriteIdEndEnd;

	float cell_width, cell_height;
	float height;
public:
	/*
	CPipe(float x, float y) : CBlock(x, y) {}
	CPipe(float x, float y, int ani_id, int pipe_height) : CBlock(x, y) {
		this->ani_id = ani_id;
		this->pipe_height = pipe_height;
	}*/
	CPipe(float x, float y,
		float cell_width, float cell_height, float height,
		int faceDirection, int warpDirection,
		int sprite_id_begin_begin, int sprite_id_end_begin,
		int sprite_id_begin_end, int sprite_id_end_end)
		: CBlock(x, y)
	{
		this->cell_width = cell_width;
		this->cell_height = cell_height;
		this->height = height;
		this->face_direction = faceDirection;
		this->warp_direction = warpDirection;
		this->spriteIdBeginBegin = sprite_id_begin_begin;
		this->spriteIdEndBegin = sprite_id_end_begin;
		this->spriteIdBeginEnd = sprite_id_begin_end;
		this->spriteIdEndEnd = sprite_id_end_end;
	}
	void Render();
	void Update(DWORD dt) {} //GameObject co ham update roi! "virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};"
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};