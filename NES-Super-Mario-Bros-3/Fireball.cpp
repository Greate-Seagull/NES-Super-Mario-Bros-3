#include "Fireball.h"
#include "Creature.h"
#include "debug.h"

CFireball::CFireball(float x, float y, int shoot_range, int nx, int ny) :
	CHarmfulObject(x, y)
{
	ApplyRange(shoot_range);
	ApplyDirection(nx, ny);
}

void CFireball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	dt = 16;
	CCollision::GetInstance()->Process(this, dt, coObjects);
	Move(dt);
}

void CFireball::Render()
{
	CAnimations::GetInstance()->Get(ANI_ID_FIREBALL)->Render(x, y);
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
