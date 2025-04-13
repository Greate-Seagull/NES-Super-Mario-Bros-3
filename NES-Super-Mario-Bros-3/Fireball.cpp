#include "Fireball.h"
#include "Mario.h"
#include "debug.h"

CFireball::CFireball(float x, float y, int shoot_range, int nx, int ny) :
	CHarmfulObject(x, y)
{
	SetBoundingBox(FIREBALL_BBOX_WIDTH, FIREBALL_BBOX_HEIGHT);
	ApplyRange(shoot_range);
	ApplyDirection(nx, ny);
}

void CFireball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	dt = 16;
	//Only colliding with mario, no other objects
	//CCollision::GetInstance()->Process(this, dt, coObjects);
	Move(dt);
}

void CFireball::Render()
{
	CAnimations::GetInstance()->Get(ANI_ID_FIREBALL)->Render(x, y);
	RenderBoundingBox();
}

void CFireball::ApplyRange(int range)
{
	switch (range)
	{
		case FIREBALL_RANGE_SHORT:
			vx = FIREBALL_SHORT_VX;
			vy = FIREBALL_SHORT_VY;
			break;
		case FIREBALL_RANGE_LONG:
			vx = FIREBALL_LONG_VX;
			vy = FIREBALL_LONG_VY;
			break;
	}
}

void CFireball::ApplyDirection(int nx, int ny)
{
	vx *= nx;
	vy *= ny;
}

void CFireball::MeleeAttack(CHarmfulObject* another)
{
	if (CCreature* creature = dynamic_cast<CCreature*>(another))
	{
		creature->Reaction(this, ACTION_ATTACK);
	}
}

void CFireball::Reaction(CGameObject* by_another, int action)
{
	AgainstControl();
	if(CMario* mario = dynamic_cast<CMario*>(by_another))
		MeleeAttack(mario);
}

//void CFireball::OnCollisionWith(LPCOLLISIONEVENT e)
//{
//	if (dynamic_cast<CMario*>(e->obj))
//	{
//		OnCollisionWithMario(e);
//	}
//}

//void CFireball::OnCollisionWithMario(LPCOLLISIONEVENT e)
//{
//	CMario* mario = dynamic_cast<CMario*>(e->obj);
//	MeleeAttack(mario);
//}
