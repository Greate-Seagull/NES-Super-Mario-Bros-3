#include "StripedBrick.h"

void CStripedBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_STRIPED_BRICK)->Render(x, y);
}

void CStripedBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - STRIPED_BRICK_BBOX_WIDTH / 2;
	t = y - STRIPED_BRICK_BBOX_HEIGHT / 2;
	r = l + STRIPED_BRICK_BBOX_WIDTH;
	b = t + STRIPED_BRICK_BBOX_HEIGHT;
}