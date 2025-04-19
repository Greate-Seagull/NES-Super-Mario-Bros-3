#pragma once

#include "Platform.h"

class CContainer : public CPlatform
{
protected:
	int height;
	int spriteIdBeginMiddle, spriteIdMiddleMiddle, spriteIdEndMiddle;
	int spriteIdBeginEnd, spriteIdMiddleEnd, spriteIdEndEnd;

public:
	CContainer(float x, float y,
		float cell_width, float cell_height,
		int length, int height,
		int sprite_id_begin_begin, int sprite_id_middle_begin, int sprite_id_end_begin,
		int sprite_id_begin_middle, int sprite_id_middle_middle, int sprite_id_end_middle,
		int sprite_id_begin_end, int sprite_id_middle_end, int sprite_id_end_end)
		: CPlatform(x, y, cell_width, cell_height, length,
			sprite_id_begin_begin, sprite_id_middle_begin, sprite_id_end_begin)
	{
		this->height = height;
		this->spriteIdBeginMiddle = sprite_id_begin_middle;
		this->spriteIdMiddleMiddle = sprite_id_middle_middle;
		this->spriteIdEndMiddle = sprite_id_end_middle;
		this->spriteIdBeginEnd = sprite_id_begin_end;
		this->spriteIdMiddleEnd = sprite_id_middle_end;
		this->spriteIdEndEnd = sprite_id_end_end;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsDirectionColliable(float nx, float ny) 
	{
		if (nx == 0 && ny == -1) return 1;
		return 0;
	}
};