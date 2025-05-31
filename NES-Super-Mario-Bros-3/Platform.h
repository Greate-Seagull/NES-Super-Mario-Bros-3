#pragma once

#include "GameObject.h"
#include "PiranhaPlant.h"

// 
// The most popular type of object in Mario! 
// 

#define PLATFORM_TYPE_BLOCK_FALL 0
#define PLATFORM_TYPE_BLOCK_ANYWAY 1

class CPlatform : public CGameObject
{
protected:
	int height;
	int width;				// Unit: cell 
	float cellWidth;
	float cellHeight;

	int type; //block type

	int spriteIdBegin, spriteIdMiddle, spriteIdEnd;
	int spriteIdBeginMiddle, spriteIdMiddleMiddle, spriteIdEndMiddle;
	int spriteIdBeginEnd, spriteIdMiddleEnd, spriteIdEndEnd;

	int falling;

public:
	CPlatform(float x, float y,
		float cell_width, float cell_height, int width, int height, int type,
		int sprite_id_begin_begin, int sprite_id_middle_begin, int sprite_id_end_begin,
		int sprite_id_begin_middle, int sprite_id_middle_middle, int sprite_id_end_middle,
		int sprite_id_begin_end, int sprite_id_middle_end, int sprite_id_end_end) 
		: CGameObject(x, y)
	{
		this->height = height;
		this->width = width;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		SetBoundingBox(width * cellWidth, height * cellHeight);

		this->type = type;

		this->spriteIdBegin = sprite_id_begin_begin;
		this->spriteIdMiddle = sprite_id_middle_begin;
		this->spriteIdEnd = sprite_id_end_begin;
		this->spriteIdBeginMiddle = sprite_id_begin_middle;
		this->spriteIdMiddleMiddle = sprite_id_middle_middle;
		this->spriteIdEndMiddle = sprite_id_end_middle;
		this->spriteIdBeginEnd = sprite_id_begin_end;
		this->spriteIdMiddleEnd = sprite_id_middle_end;
		this->spriteIdEndEnd = sprite_id_end_end;
	}

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

	virtual int IsLinkedTo(CGameObject* obj);
	virtual int IsCollidable() { return 1; }
	virtual int IsBlocking() { return 1; }
	virtual int IsDirectionalBlocking() { return type == PLATFORM_TYPE_BLOCK_FALL; }
	virtual int IsDirectionColliable(float nx, float ny);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
};

typedef CPlatform* LPPLATFORM;