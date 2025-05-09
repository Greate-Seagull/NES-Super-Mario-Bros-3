#include "Coin.h"

#include "Mario.h"

void CCoin::Prepare(DWORD dt)
{
	if (isToggled)
		CMovableObject::Prepare(dt);
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isToggled)
	{
		Move(dt);

		if (y >= originalY)
		{
			Delete();
		}
	}
}

void CCoin::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CMario*>(e->obj))
		Delete();
}

void CCoin::Reaction(CGameObject* by_another, int action)
{
	switch (action)
	{
	case ACTION_TOUCH:
		if (dynamic_cast<CMario*>(by_another)) Delete();
		else SetToggled();
		break;
	default:
		Delete();
		break;
	}
}

void CCoin::SetToggled()
{
	isToggled = true;
	vy = COIN_VELOCITY_Y;
}

bool CCoin::IsUnderOriginal()
{
	if (y < originalY) return false;
}