#pragma once

#include "MovableObject.h"

#define ACTION_CARRY -1
#define ACTION_TOUCH 0
#define ACTION_ATTACK_LEVEL_1 1
#define ACTION_ATTACK_LEVEL_2 2
#define ACTION_ATTACK_LEVEL_3 3

#define EFFECT_BIGGER 10
#define EFFECT_RACOONIZE 11

#define ATTACK_BOOM_VX 0.08f
#define ATTACK_BOOM_VY 0.3f

class CHarmfulObject : public CMovableObject
{
protected:
	bool isControl;
	bool highPower;
public:
	CHarmfulObject(float x, float y);
	virtual int IsCollidable() { return 1; }

	virtual void OnReactionTo(LPCOLLISIONEVENT e, int action);
	virtual void OnReactionToCarrying(LPCOLLISIONEVENT e);
	virtual void OnReactionToTouching(LPCOLLISIONEVENT e);
	virtual void OnReactionToAttack1(LPCOLLISIONEVENT e);
	virtual void OnReactionToAttack2(LPCOLLISIONEVENT e);
	virtual void OnReactionToAttack3(LPCOLLISIONEVENT e);
	virtual void OnReactionToBigger(LPCOLLISIONEVENT e);
	virtual void OnReactionToRacoonize(LPCOLLISIONEVENT e);

	virtual void Attack(LPCOLLISIONEVENT e);
	virtual void HigherAttack(LPCOLLISIONEVENT e);
	virtual void Destroy(LPCOLLISIONEVENT e);
	virtual void FlyOut(int attack_direction);
	virtual void SetHighPower();
	virtual void LoseHighPower();
	virtual bool IsControlled();
	virtual void SetControl();
	virtual void Drop();
	virtual void AgainstControl();
};
