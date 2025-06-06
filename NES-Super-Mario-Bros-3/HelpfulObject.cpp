#include "HelpfulObject.h"

CHelpfulObject::CHelpfulObject(float x, float y) :
	CMovableObject(x, y)
{
	effect = 0;
}

void CHelpfulObject::LaunchEffect(LPCOLLISIONEVENT e)
{
	e->obj->OnReactionTo(e, effect);
}

void CHelpfulObject::Refresh()
{
	if (effect)
	{
		this->Delete();
		return;
	}

	CMovableObject::Refresh();
}
