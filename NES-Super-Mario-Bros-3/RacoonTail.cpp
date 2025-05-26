#include "RacoonTail.h"

CRacoonTail::CRacoonTail(float x, float y) :
	CHarmfulObject(x, y)
{
	SetBoundingBox(RACOON_TAIL_BBOX_WIDTH, RACOON_TAIL_BBOX_HEIGHT);

	highPower = true;
}

void CRacoonTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CCollision::GetInstance()->SolveOverlap(this, coObjects);
}

void CRacoonTail::Render()
{
	RenderBoundingBox();
}

void CRacoonTail::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CCreature*>(e->obj))
		OnCollisionWithCreature(e);
	else if (dynamic_cast<CBlock*>(e->obj))
		OnCollisionWithBlock(e);
}

void CRacoonTail::OnCollisionWithCreature(LPCOLLISIONEVENT e)
{
	Destroy(e);
}

void CRacoonTail::OnCollisionWithBlock(LPCOLLISIONEVENT e)
{
	Destroy(e);
}
