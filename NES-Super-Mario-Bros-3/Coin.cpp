#include "Coin.h"

#include "Mario.h"

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isToggled)
	{
		Accelerate(0.0f, GAME_GRAVITY, dt);
		Move(dt);

		if (y >= originalY)
		{
			Delete();
		}
	}
	else
	{
		//CCollision::GetInstance()->ProcessOverlap(this, coObjects);
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
		case ACTION_ATTACK:
			isToggled = true;
			break;
		default:
			Delete();
			break;
	}
}

bool CCoin::IsUnderOriginal()
{
	if (y < originalY) return false;
}