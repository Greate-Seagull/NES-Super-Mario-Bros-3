#pragma once

#include "Platform.h"

class CContainer : public CPlatform
{
protected:

public:
	CContainer(float x, float y,
		float cell_width, float cell_height,
		int width, int height,
		int sprite_id_begin_begin, int sprite_id_middle_begin, int sprite_id_end_begin,
		int sprite_id_begin_middle, int sprite_id_middle_middle, int sprite_id_end_middle,
		int sprite_id_begin_end, int sprite_id_middle_end, int sprite_id_end_end)
		: CPlatform(x, y, cell_width, cell_height, width, height, PLATFORM_TYPE_BLOCK_FALL,
			sprite_id_begin_begin, sprite_id_middle_begin, sprite_id_end_begin,
			sprite_id_begin_middle, sprite_id_middle_middle, sprite_id_end_middle,
			sprite_id_begin_end, sprite_id_middle_end, sprite_id_end_end)
	{	
	}

	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};