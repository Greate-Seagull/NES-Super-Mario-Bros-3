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
#define ATTACK_BOOM_VY 0.386f

class CHarmfulObject : public CMovableObject
{
protected:
	bool isControl;
	bool highPower;
public:
	CHarmfulObject(float x, float y);
	virtual int IsCollidable() { return 1; }

	virtual void Reaction(CGameObject* by_another, int action);
	virtual void ReactionToCarry(CGameObject* by_another);
	virtual void ReactionToTouch(CGameObject* by_another);
	virtual void ReactionToAttack1(CGameObject* by_another);
	virtual void ReactionToAttack2(CGameObject* by_another);
	virtual void ReactionToAttack3(CGameObject* by_another);
	virtual void ReactionToBigger(CGameObject* by_another);
	virtual void ReactionToRacoonize(CGameObject* by_another);

	virtual void Attack(CGameObject* another);
	virtual void HigherAttack(CGameObject* another);
	virtual void Destroy(CGameObject* another);
	virtual void FlyOut(int attack_direction);
	virtual void SetHighPower();
	virtual void LoseHighPower();
	virtual bool IsControlled();
	virtual void SetControl();
	virtual void Drop();
	virtual void AgainstControl();
};
