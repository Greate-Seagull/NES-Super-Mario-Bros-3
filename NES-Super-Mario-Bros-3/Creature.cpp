#include "Creature.h"

CCreature::CCreature(float x, float y):
	CHarmfulObject(x, y)
{
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
	e->obj->OnReactionTo(e, ACTION_CARRY);
}

void CCreature::Pickup(CCreature* weapon)
{
	this->weapon = weapon;
}

void CCreature::Drop()
{
	weapon = nullptr;
}

void CCreature::AgainstControl()
{	
	carrier->CCreature::Drop();
	carrier = nullptr;
}

void CCreature::AcceptControl(CCreature* carrier)
{
	this->carrier = carrier;
	carrier->Pickup(this);
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

void CCreature::Refresh()
{
	CHarmfulObject::Refresh();
}
