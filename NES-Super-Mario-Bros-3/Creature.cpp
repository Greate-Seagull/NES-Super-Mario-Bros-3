#include "Creature.h"

CCreature::CCreature(float x, float y):
	CHarmfulObject(x, y)
{
}

void CCreature::IsLiving()
{
	if (this->life == 0.0f)
	{
		this->SetState(STATE_DIE);
	}
}

void CCreature::UnderAttack(CHarmfulObject* by_another)
{
	SetLife(life - 1.0f);
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
	weapon = nullptr;
}

void CCreature::Recover()
{
	SetLife(life + 1.0f);
	SetState(STATE_LIVE);
}

void CCreature::Die()
{
	SetLife(0.0f);
	SetState(STATE_DIE);
}

void CCreature::SetLife(float life)
{
	this->life = life;
}
