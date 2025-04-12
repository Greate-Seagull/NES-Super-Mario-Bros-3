#include "HarmfulObject.h"

CHarmfulObject::CHarmfulObject(float x, float y):
	CMovableObject(x, y)
{
	isControl = false;
}

bool CHarmfulObject::IsControlled()
{
	return isControl;
}

void CHarmfulObject::SetControl()
{
	isControl = true;
}

void CHarmfulObject::Drop()
{
	isControl = false;
}
