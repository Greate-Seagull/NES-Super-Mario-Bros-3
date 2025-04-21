#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

// 
// The most popular type of object in Mario! 
// 
class CBigBush : public CGameObject
{
protected:
	int height;	// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteIdBeginBegin, spriteIdEndBegin;
	int spriteIdBeginEnd, spriteIdEndEnd;

public:
	CBigBush(float x, float y,
		float cell_width, float cell_height,
		int height,
		int sprite_id_begin_begin, int sprite_id_end_begin,
		int sprite_id_begin_end, int sprite_id_end_end) :CGameObject(x, y)
	{
		this->height = height;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteIdBeginBegin = sprite_id_begin_begin;
		this->spriteIdEndBegin = sprite_id_end_begin;
		this->spriteIdBeginEnd = sprite_id_begin_end;
		this->spriteIdEndEnd = sprite_id_end_end;
	}

	virtual void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

#define ID_ANI_MINI_BUSH -5000

#define BUSH_BBOX_WIDTH 16
#define BUSH_BBOX_HEIGHT 16

class CMiniBush : public CGameObject {
public:
	CMiniBush(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};