#pragma once

#include "Block.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_STRIPED_BRICK 15000
#define STRIPED_BRICK_WIDTH 16.0f
#define STRIPED_BRICK_BBOX_WIDTH 16.0f
#define STRIPED_BRICK_BBOX_HEIGHT 16.0f

class CStripedBrick : public CBlock {
public:
	CStripedBrick(float x, float y) : CBlock(x, y) { bbox_width = STRIPED_BRICK_BBOX_WIDTH; bbox_height = STRIPED_BRICK_BBOX_HEIGHT; //Thêm dòng này để dùng CGameObject::GetBoundingBox(float& l, float& t, float& r, float& b)
													 aniID = ID_ANI_STRIPED_BRICK; /* Thêm dòng này để dùng CGameObject::Render() */ 
												   }
	void Render(); //GameObject co ham render roi! "virtual void Render();"
	void Update(DWORD dt) {} //GameObject co ham update roi! "virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};"
	void GetBoundingBox(float& l, float& t, float& r, float& b); //GameObject có GetBoundingBox rồi! "virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);"
};