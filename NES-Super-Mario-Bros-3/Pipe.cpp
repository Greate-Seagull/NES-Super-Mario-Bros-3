#include "Pipe.h"

void CPipe::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ani_id)->Render(x, y);
	RenderBoundingBox();
}

void CPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - PIPE_BBOX_WIDTH / 2;
	t = y - pipe_height / 2;
	r = l + PIPE_BBOX_WIDTH;
	b = t + pipe_height;
}