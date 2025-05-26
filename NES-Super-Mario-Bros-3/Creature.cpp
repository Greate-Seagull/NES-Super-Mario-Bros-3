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

void CCreature::Touch(LPCOLLISIONEVENT e)
{
	e->obj->OnReactionTo(e, ACTION_TOUCH);
}

void CCreature::Carry(LPCOLLISIONEVENT e)
{
	if (CHarmfulObject* obj = dynamic_cast<CHarmfulObject*>(e->obj))
	{
		weapon = obj;
		obj->SetControl();
		obj->OnReactionTo(e, ACTION_CARRY);
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
