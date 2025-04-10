#include "Creature.h"

CCreature::CCreature(float x, float y, float life):
	CHarmfulObject(x, y)
{
	this->life = life;
}

void CCreature::ProcessLife()
{
	if (this->life == 0.0f)
	{
		this->SetState(STATE_DIE);
	}
}

void CCreature::UnderAttack(CHarmfulObject* by_another)
{
	this->life -= 1;	
}

void CCreature::MeleeAttack(CHarmfulObject* another)
{
	if (CCreature* creature = dynamic_cast<CCreature*>(another))
	{
		creature->UnderAttack(this);
	}
}

string CCreature::ToString()
{
	return "Creature";
}
