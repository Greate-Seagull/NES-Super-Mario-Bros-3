#pragma once

#include "MovableObject.h"
#include "Platform.h"

#define FALLING_GRAVITY 0.00025f;

class CFloatingPlatform: public CPlatform
{
	float vx, vy;
	bool isFloating;
	
public:
	CFloatingPlatform(float x, float y, float vx, float vy,
		float cell_width, float cell_height, int width, int height, int type,
		int sprite_id_begin_begin, int sprite_id_middle_begin, int sprite_id_end_begin,
		int sprite_id_begin_middle, int sprite_id_middle_middle, int sprite_id_end_middle,
		int sprite_id_begin_end, int sprite_id_middle_end, int sprite_id_end_end)
		: CPlatform(x, y, cell_width, cell_height, width, height, PLATFORM_TYPE_BLOCK_FALL,
			sprite_id_begin_begin, sprite_id_middle_begin, sprite_id_end_begin,
			sprite_id_begin_middle, sprite_id_middle_middle, sprite_id_end_middle,
			sprite_id_begin_end, sprite_id_middle_end, sprite_id_end_end)
	{
		this->vx = vx;
		this->vy = vy;

		this->isFloating = true;
	}

	void GetSpeed(float& vx, float& vy);

	void Prepare(DWORD dt);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void OnReactionTo(LPCOLLISIONEVENT e, int action);
};