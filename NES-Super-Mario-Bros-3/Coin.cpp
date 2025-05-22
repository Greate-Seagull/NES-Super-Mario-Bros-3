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

void CCoin::OnReactionTo(LPCOLLISIONEVENT e, int action)
{
	if(isContained)
		SetToggled();
	else
		Delete();
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