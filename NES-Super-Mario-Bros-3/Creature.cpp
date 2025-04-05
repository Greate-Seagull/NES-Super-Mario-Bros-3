#include "Creature.h"

CCreature::CCreature(float x, float y, bool isSharp, float life):
	CHarmfulObject(x, y, isSharp)
{
	this->life = life;
}
