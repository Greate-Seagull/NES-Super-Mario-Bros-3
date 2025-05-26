#include "HelpfulObject.h"

CHelpfulObject::CHelpfulObject(float x, float y) :
	CMovableObject(x, y)
{
}

void CHelpfulObject::LaunchEffect(LPCOLLISIONEVENT e)
{
	e->obj->OnReactionTo(e, effect);
}
