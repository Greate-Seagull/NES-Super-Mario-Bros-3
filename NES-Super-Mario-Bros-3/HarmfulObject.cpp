#include "HarmfulObject.h"

CHarmfulObject::CHarmfulObject(float x, float y):
	CMovableObject(x, y)
{
	isControl = false;
	highPower = false;
}

void CHarmfulObject::MeleeAttack(CGameObject* another)
{
	if (another)
	{
		another->Reaction(this, ACTION_ATTACK);
	}
}

void CHarmfulObject::Destroy(CGameObject* another)
{
	if (another && highPower)
	{
		//Effect boom
		another->Reaction(this, ACTION_DESTROY);
	}
}

void CHarmfulObject::FlyOut(int attack_direction)
{
	vx = attack_direction * ATTACK_BOOM_VX;

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
