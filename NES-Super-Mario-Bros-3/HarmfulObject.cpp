#include "HarmfulObject.h"

CHarmfulObject::CHarmfulObject(float x, float y):
	CMovableObject(x, y)
{
	isControl = false;
	highPower = false;
}

void CHarmfulObject::Reaction(CGameObject* by_another, int action)
{
	switch (action)
	{
	case ACTION_CARRY:
		ReactionToCarry(by_another);
		break;
	case ACTION_TOUCH:
		ReactionToTouch(by_another);
		break;
	case ACTION_ATTACK_LEVEL_1:
		ReactionToAttack1(by_another);
		break;
	case ACTION_ATTACK_LEVEL_2:
		ReactionToAttack2(by_another);
		break;
	case ACTION_ATTACK_LEVEL_3:
		ReactionToAttack3(by_another);
		break;
	case EFFECT_BIGGER:
		ReactionToBigger(by_another);
		break;
	case EFFECT_RACOONIZE:
		ReactionToRacoonize(by_another);
		break;
	}
}

void CHarmfulObject::ReactionToCarry(CGameObject* by_another)
{
	AgainstControl();
	Attack(by_another);
}

void CHarmfulObject::ReactionToTouch(CGameObject* by_another)
{
	Attack(by_another);
}

void CHarmfulObject::ReactionToAttack1(CGameObject* by_another)
{
}

void CHarmfulObject::ReactionToAttack2(CGameObject* by_another)
{
}

void CHarmfulObject::ReactionToAttack3(CGameObject* by_another)
{
}

void CHarmfulObject::ReactionToBigger(CGameObject* by_another)
{
}

void CHarmfulObject::ReactionToRacoonize(CGameObject* by_another)
{
}

void CHarmfulObject::Attack(CGameObject* another)
{
	another->Reaction(this, ACTION_ATTACK_LEVEL_1);
}

void CHarmfulObject::HigherAttack(CGameObject* another)
{
	another->Reaction(this, ACTION_ATTACK_LEVEL_2);
}

void CHarmfulObject::Destroy(CGameObject* another)
{
	//Effect boom
	another->Reaction(this, ACTION_ATTACK_LEVEL_3);
}

void CHarmfulObject::FlyOut(int attack_direction)
{
	nx = attack_direction;
	vx = nx * ATTACK_BOOM_VX;

	ny = DIRECTION_UP;
	vy = ny * ATTACK_BOOM_VY;
}

void CHarmfulObject::SetHighPower()
{
	highPower = true;
}

void CHarmfulObject::LoseHighPower()
{
	highPower = false;
}

bool CHarmfulObject::IsControlled()
{
	return isControl;
}

void CHarmfulObject::SetControl()
{
	isControl = true;
}

void CHarmfulObject::Drop()
{
	isControl = false;
}

void CHarmfulObject::AgainstControl()
{
	isControl = false;
}
