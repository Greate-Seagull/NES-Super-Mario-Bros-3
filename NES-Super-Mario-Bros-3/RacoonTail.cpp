#include "RacoonTail.h"

CRacoonTail::CRacoonTail(float x, float y) :
	CHarmfulObject(x, y)
{
	SetBoundingBox(RACOON_TAIL_BBOX_WIDTH, RACOON_TAIL_BBOX_HEIGHT);
}

void CRacoonTail::Prepare(DWORD dt)
{
	vx = dx / dt;
	vy = dy / dt;
}

void CRacoonTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	isFliedOut = false; //Never be killed
	Move(dt);
}

void CRacoonTail::Render()
{
	//RenderBoundingBox();
}

int CRacoonTail::IsLinkedTo(CGameObject* obj)
{
	return obj == owner;
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

void CRacoonTail::AcceptOwner(CGameObject* owner)
{
	if (this->owner == nullptr)
		this->owner = owner;
}

void CRacoonTail::Refresh()
{
	CHarmfulObject::Refresh();

	dx = 0.0f;
	dy = 0.0f;
}
