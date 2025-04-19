#include "DeadStateTrigger.h"
#include "Collision.h"

void CDeadStateTrigger::Render()
{
	RenderBoundingBox();
}

void CDeadStateTrigger::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - width / 2;
	t = y - height / 2;
	r = l + width;
	b = t + height;
}