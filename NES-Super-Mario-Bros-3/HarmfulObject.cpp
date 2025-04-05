#include "HarmfulObject.h"

CHarmfulObject::CHarmfulObject(float x, float y, bool isSharp):
	CMovableObject(x, y)
{
	this->isSharp = isSharp;
}

void CHarmfulObject::Attack()
{
}
