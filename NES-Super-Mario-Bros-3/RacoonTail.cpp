#include "RacoonTail.h"

CRacoonTail::CRacoonTail(float x, float y) :
	CHarmfulObject(x, y)
{
	bbox_width = RACOON_TAIL_BBOX_WIDTH;
	bbox_height = RACOON_TAIL_BBOX_HEIGHT;

	highPower = true;
}

void CRacoonTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CCollision::GetInstance()->ProcessOverlap(this, coObjects);
}

void CRacoonTail::Render()
{
	RenderBoundingBox();
}

void CRacoonTail::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CCreature*>(e->obj))
		OnCollisionWithCreature(e);
}

void CRacoonTail::OnCollisionWithCreature(LPCOLLISIONEVENT e)
{
	Destroy(e->obj);
}
