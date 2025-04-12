#pragma once

#include "HarmfulObject.h"

#define ACTION_TOUCH 0
#define ACTION_ATTACK 1
#define ACTION_CARRY 2

#define REACTION_NO_RESIST 0
#define REACTION_RESIST 1

class CCreature : public CHarmfulObject
{
protected:
	float life;
	int reaction;
	CHarmfulObject* weapon;
public:
	CCreature(float x, float y);
	virtual void ProcessLife();
	virtual void UnderAttack(CHarmfulObject* by_another);
	virtual void MeleeAttack(CHarmfulObject* another);
	virtual void Touch(CHarmfulObject* another);
	virtual void Carry(CHarmfulObject* another);
	virtual void Drop();
	virtual void Reaction(CHarmfulObject* by_another, int action = ACTION_TOUCH);
	virtual void Recover();
	virtual void AgainstControl();
};