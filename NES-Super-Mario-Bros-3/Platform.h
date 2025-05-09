#pragma once

#include "GameObject.h"

// 
// The most popular type of object in Mario! 
// 

#define PLATFORM_TYPE_BLOCK_FALL 0
#define PLATFORM_TYPE_BLOCK_ANYWAY 1

class CPlatform : public CGameObject
{
protected:
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;

	int type; //block type

	int spriteIdBegin, spriteIdMiddle, spriteIdEnd;

public:
	CPlatform(float x, float y,
		float cell_width, float cell_height, int length, int type,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;

		this->type = type;

		this->spriteIdBegin = sprite_id_begin;
		this->spriteIdMiddle = sprite_id_middle;
		this->spriteIdEnd = sprite_id_end;
	}

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

	virtual int IsCollidable() { return 1; }
	virtual int IsBlocking() { return 1; }
	virtual int IsDirectionalBlocking() { return type == PLATFORM_TYPE_BLOCK_FALL; }
	virtual int IsDirectionColliable(float nx, float ny);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
};

typedef CPlatform* LPPLATFORM;