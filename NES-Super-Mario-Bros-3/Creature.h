#pragma once

#include "HarmfulObject.h"

#define REACTION_NO_RESIST 0
#define REACTION_RESIST 1

#define ACTION_TOUCH 10
#define ACTION_CARRY 11
#define ACTION_DROP 12
#define ACTION_NOTHING 13

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
	virtual void DoPowerless(CHarmfulObject* another);
	virtual void SetLife(float life);
};