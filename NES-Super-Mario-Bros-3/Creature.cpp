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

void CCreature::Touch(CGameObject* another)
{
	if (another)
	{
		another->Reaction(this, ACTION_TOUCH);
	}
}

void CCreature::Carry(CHarmfulObject* another)
{
	if (another)
	{
		weapon = another;
		another->SetControl();

		another->Reaction(this, ACTION_CARRY);
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

void CCreature::Recover()
{
	life += 1.0f;
	SetState(STATE_LIVE);
}
