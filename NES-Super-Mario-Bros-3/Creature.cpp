#include "Creature.h"

CCreature::CCreature(float x, float y, float vx, float vy, float ax, float ay, bool isSharp, float life):
	CHarmfulObject(x, y, vx, vy, ax, ay, isSharp)
{
	this->life = life;
}
