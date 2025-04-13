#pragma once

#include "GameObject.h"

// 
// The most popular type of object in Mario! 
// 
class CCloud : public CGameObject
{
protected:
	int length;	// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteIdBeginBegin, spriteIdMiddleBegin, spriteIdEndBegin;
	int spriteIdBeginEnd, spriteIdMiddleEnd, spriteIdEndEnd;

public:
	CCloud(float x, float y,
		float cell_width, float cell_height,
		int length,
		int sprite_id_begin_begin, int sprite_id_middle_begin, int sprite_id_end_begin,
		int sprite_id_begin_end, int sprite_id_middle_end, int sprite_id_end_end) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteIdBeginBegin = sprite_id_begin_begin;
		this->spriteIdMiddleBegin = sprite_id_middle_begin;
		this->spriteIdEndBegin = sprite_id_end_begin;
		this->spriteIdBeginEnd = sprite_id_begin_end;
		this->spriteIdMiddleEnd = sprite_id_middle_end;
		this->spriteIdEndEnd = sprite_id_end_end;
	}

	virtual void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};