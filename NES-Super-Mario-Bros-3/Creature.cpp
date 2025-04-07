#include "Creature.h"

CCreature::CCreature(float x, float y, bool isSharp, float life):
	CHarmfulObject(x, y, isSharp)
{
	this->life = life;
}

void CCreature::ProcessLifeState()
{
	if (this->life == 0.0f)
	{
		state = STATE_DIE;
	}
}

string CCreature::ToString()
{
	return "Creature";
}
