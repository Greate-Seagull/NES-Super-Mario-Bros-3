#include "HarmfulObject.h"

CHarmfulObject::CHarmfulObject(float x, float y, float vx, float vy, float ax, float ay, bool isSharp):
	CMovableObject(x, y, vx, vy, ax, ay)
{
	this->isSharp = isSharp;
}

void CHarmfulObject::Attack()
{
}
