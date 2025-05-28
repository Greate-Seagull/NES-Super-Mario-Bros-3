#pragma once

#include "Goomba.h"

#define PARAGOOMBA_BBOX_HEIGHT 15.0f
#define PARAGOOMBA_BBOX_WIDTH 16.0f
#define PARAGOOMBA_BBOX_DIE_HEIGHT 9

//animations
//object
#define ANI_ID_PARAGOOMBA 21000

class CParagoomba : public CGoomba
{
public:
	CParagoomba(float x, float y, bool haveWings = false);
	int GetObjectAniID() { return ANI_ID_PARAGOOMBA; }
};