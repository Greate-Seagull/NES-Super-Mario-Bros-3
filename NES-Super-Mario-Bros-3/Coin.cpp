#include "Coin.h"

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
}

bool CCoin::IsUnderOriginal()
{
	if (y < originalY) return false;
}