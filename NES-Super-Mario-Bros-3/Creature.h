#pragma once

#include "HarmfulObject.h"

#define REACTION_NO_RESIST 0
#define REACTION_RESIST 1

#define ACTION_TOUCH 10
#define ACTION_CARRY 11

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
	virtual void Touch(CGameObject* another);
	virtual void Carry(CHarmfulObject* another);
	virtual void Drop();
	virtual void Recover();	
	virtual void Die();
};