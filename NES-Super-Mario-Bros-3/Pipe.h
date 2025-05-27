#pragma once

#include "Block.h"
#include "Animation.h"
#include "Animations.h"

#define PIPE_WIDTH 30.0f
#define PIPE_BBOX_WIDTH 30.0f
#define PIPE_BBOX_INSIDE_WIDTH 16.0f

class CPipe : public CBlock {
protected:
	int face_direction;
	int warp_direction;
	int scene_destination;

	int spriteIdBeginBegin, spriteIdEndBegin;
	int spriteIdBeginEnd, spriteIdEndEnd;

	float cell_width, cell_height;
	float height;

	float newX, newY;
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
		int sprite_id_begin_end, int sprite_id_end_end, int itemID,
		int scene_destination = 0, float newX = 0, float newY = 0)
		: CBlock(x, y, itemID)
	{
		this->cell_width = cell_width;
		this->cell_height = cell_height;
		this->height = height;
		this->face_direction = faceDirection;
		this->warp_direction = warpDirection;
		this->scene_destination = scene_destination;
		this->spriteIdBeginBegin = sprite_id_begin_begin;
		this->spriteIdEndBegin = sprite_id_end_begin;
		this->spriteIdBeginEnd = sprite_id_begin_end;
		this->spriteIdEndEnd = sprite_id_end_end;

		this->bbox_height = cell_height * height;
		this->bbox_width = PIPE_BBOX_WIDTH;

		this->newX = newX;
		this->newY = newY;

		TakeItem();
		TriggerItem();
	}
	void Render();
	void RenderBoundingBox();	

	void OnCollisionWith(LPCOLLISIONEVENT e);

	void OnReactionTo(LPCOLLISIONEVENT e, int action);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void GetBoundingBoxInside(float& left, float& top, float& right, float& bottom);

	void TakeItem();
	void UseDefaultItemPosition();
	void TriggerItem();

	int GetDestination() { return scene_destination; }
	float GetDestinationX() { return newX; }
	float GetDestinationY() { return newY; }
};