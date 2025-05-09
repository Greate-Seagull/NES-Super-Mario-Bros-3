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
		case ACTION_TOUCH:
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