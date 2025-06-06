#pragma once

#include "HarmfulObject.h"

class CCreature : public CHarmfulObject
{
protected:
	float life;
	CCreature* weapon;
	CCreature* carrier;
public:
	CCreature(float x, float y);

	virtual void UnderAttack(CHarmfulObject* by_another);
	virtual void Touch(LPCOLLISIONEVENT e);
	virtual void Carry(LPCOLLISIONEVENT e);
	virtual void Pickup(CCreature* weapon);
	virtual void Drop();
	virtual void AgainstControl();
	virtual void AcceptControl(CCreature* carrier);
	virtual void Recover();	
	virtual void Die();
	virtual void SetLife(float life);

	virtual void Refresh();
};