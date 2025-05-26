#pragma once

#include "HarmfulObject.h"

class CCreature : public CHarmfulObject
{
protected:
	float life;
	int reaction;
	CHarmfulObject* weapon;
public:
	CCreature(float x, float y);

	virtual void IsLiving();
	virtual void UnderAttack(CHarmfulObject* by_another);
	virtual void Touch(LPCOLLISIONEVENT e);
	virtual void Carry(LPCOLLISIONEVENT e);
	virtual void Drop();
	virtual void Recover();	
	virtual void Die();
	virtual void SetLife(float life);
};