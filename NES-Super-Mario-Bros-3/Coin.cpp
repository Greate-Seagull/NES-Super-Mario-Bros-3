#include "Coin.h"
#include "Mario.h"

#define ACTION_COLLECT 0

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isToggled)
	{
		vy += COIN_ACCELERATION_Y;
		y += vy * dt;
	}
}

void CCoin::Reaction(CGameObject* by_another, int action)
{
	switch (action)
	{
	case ACTION_COLLECT:
		if (dynamic_cast<CMario*>(by_another)) isDisappear = true;
		break;
	}
}

void CCoin::Collect(CGameObject* obj)
{
	if (obj) {
		this->Reaction(obj, ACTION_COLLECT);
	}
}

bool CCoin::IsUnderOriginal()
{
	if (y <= originalY) return false;
	else return true;

}

bool CCoin::IsUnderOriginal()
{
	if (y < originalY) return false;
}