#include "Coin.h"
#include "Mario.h"

#define ACTION_COLLECT 0

void CCoin::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_COIN)->Render(x, y);

	//RenderBoundingBox();
}

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
		if (dynamic_cast<CMario*>(by_another)) this->Delete();
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

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}