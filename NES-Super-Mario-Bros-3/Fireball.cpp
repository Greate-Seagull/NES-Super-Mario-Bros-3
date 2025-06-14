#include "Fireball.h"
#include "Mario.h"

CFireball::CFireball(float x, float y, int shoot_range, int nx, int ny) :
	CHarmfulObject(x, y)
{
	aniID = ANI_ID_FIREBALL;

	SetBoundingBox(FIREBALL_BBOX_WIDTH, FIREBALL_BBOX_HEIGHT);
	ApplyRange(shoot_range);
	ApplyDirection(nx, ny);
}

void CFireball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (vx == 0.0f && vy == 0.0f)
		return;

	Move(dt);
}

void CFireball::Render()
{
	if (vx == 0.0f && vy == 0.0f)
		return;

	CGameObject::Render();
}

void CFireball::ApplyRange(int range)
{
	switch (range)
	{
		case FIREBALL_RANGE_DISARM:
			vx = 0.0f;
			vy = 0.0f;
			break;
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

//void CFireball::OnCollisionWith(LPCOLLISIONEVENT e)
//{
//	if (CMario* mario = dynamic_cast<CMario*>(e->obj))
//		Attack(mario);
//}

void CFireball::OnReactionTo(LPCOLLISIONEVENT e, int action)
{
	if (dynamic_cast<CMario*>(e->src_obj))
	{
		e->Reverse();
		Attack(e);
	}
}

int CFireball::IsCollidable()
{
	return vx || vy;
}