#include "HarmfulObject.h"
#include "debug.h"

CHarmfulObject::CHarmfulObject(float x, float y):
	CMovableObject(x, y)
{
	isControl = false;
	highPower = false;
}

void CHarmfulObject::OnReactionTo(LPCOLLISIONEVENT e, int action)
{
	switch (action)
	{
	case ACTION_CARRY:
		OnReactionToCarrying(e);
		break;
	case ACTION_TOUCH:
		OnReactionToTouching(e);
		break;
	case ACTION_ATTACK_LEVEL_1:
		OnReactionToAttack1(e);
		break;
	case ACTION_ATTACK_LEVEL_2:
		OnReactionToAttack2(e);
		break;
	case ACTION_ATTACK_LEVEL_3:
		OnReactionToAttack3(e);
		break;
	case EFFECT_BIGGER:
		OnReactionToBigger(e);
		break;
	case EFFECT_RACOONIZE:
		OnReactionToRacoonize(e);
		break;
	}
}

void CHarmfulObject::OnReactionToCarrying(LPCOLLISIONEVENT e)
{
	AgainstControl();

	e->Reverse();
	Attack(e);
}

void CHarmfulObject::OnReactionToTouching(LPCOLLISIONEVENT e)
{
	e->Reverse();
	Attack(e);
}

void CHarmfulObject::OnReactionToAttack1(LPCOLLISIONEVENT e)
{
}

void CHarmfulObject::OnReactionToAttack2(LPCOLLISIONEVENT e)
{
}

void CHarmfulObject::OnReactionToAttack3(LPCOLLISIONEVENT e)
{
	FlyOut(this->x <= e->src_obj->GetX() ? DIRECTION_LEFT : DIRECTION_RIGHT);
}

void CHarmfulObject::OnReactionToBigger(LPCOLLISIONEVENT e)
{
}

void CHarmfulObject::OnReactionToRacoonize(LPCOLLISIONEVENT e)
{
}

void CHarmfulObject::Attack(LPCOLLISIONEVENT e)
{
	e->obj->OnReactionTo(e, ACTION_ATTACK_LEVEL_1);
}

void CHarmfulObject::HigherAttack(LPCOLLISIONEVENT e)
{
	e->obj->OnReactionTo(e, ACTION_ATTACK_LEVEL_2);
}

void CHarmfulObject::Destroy(LPCOLLISIONEVENT e)
{
	float effect_x = x - e->nx * bbox_width / 2.0f;
	float effect_y = y - e->ny * bbox_height / 2.0f;
	CEffectsManager::GetInstance()->CreateAttackEffect(effect_x, effect_y);
	e->obj->OnReactionTo(e, ACTION_ATTACK_LEVEL_3);
}

void CHarmfulObject::FlyOut(int attack_direction)
{
	SetState(STATE_FLYINGOUT);

	nx = attack_direction;
	vx = nx * fabs(maxVx);

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
