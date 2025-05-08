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
	if (dynamic_cast<CBlock*>(e->obj))
		OnCollisionWithBlock(e);
}

void CRacoonTail::OnCollisionWithCreature(LPCOLLISIONEVENT e)
{
	Destroy(e->obj);
}

void CRacoonTail::OnCollisionWithBlock(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CBrick*>(e->obj))
	{
		CBrick* src = (CBrick*)e->obj;
		src->Destroy();
	}
}