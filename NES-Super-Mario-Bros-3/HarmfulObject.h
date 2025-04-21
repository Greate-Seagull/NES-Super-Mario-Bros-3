#pragma once

#include "MovableObject.h"

#define ACTION_ATTACK 0
#define ACTION_DESTROY 1

#define ATTACK_BOOM_VX 0.08f
#define ATTACK_BOOM_VY 0.386f

class CHarmfulObject : public CMovableObject
{
protected:
	bool isControl;
	bool highPower;
public:
	CHarmfulObject(float x, float y);
	virtual void MeleeAttack(CGameObject* another);
	virtual void Destroy(CGameObject* another);
	virtual void FlyOut(int attack_direction);
	virtual void SetHighPower();
	virtual void LoseHighPower();
	virtual bool IsControlled();
	virtual void SetControl();
	virtual void Drop();
	virtual void AgainstControl();
};
