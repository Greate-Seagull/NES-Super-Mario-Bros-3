#include "Creature.h"

CCreature::CCreature(float x, float y):
	CHarmfulObject(x, y)
{
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
		creature->Reaction(this, ACTION_ATTACK);
	}
}

void CCreature::Touch(CHarmfulObject* another)
{
	if (CCreature* creature = dynamic_cast<CCreature*>(another))
	{
		creature->Reaction(this, ACTION_TOUCH);
	}
}

void CCreature::Carry(CHarmfulObject* another)
{
	weapon = another;
	another->SetControl();

	if (CCreature* creature = dynamic_cast<CCreature*>(another))
	{
		creature->Reaction(this, ACTION_CARRY);
	}
}

void CCreature::Drop()
{
	if (weapon)
	{
		weapon->Drop();
		weapon = nullptr;
	}
}

void CCreature::Reaction(CHarmfulObject* by_another, int action)
{
	reaction = REACTION_NO_RESIST;
}

void CCreature::Recover()
{
	life += 1.0f;
	SetState(STATE_LIVE);
}

void CCreature::AgainstControl()
{
	isControl = false;
}
