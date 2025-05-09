#include "Goomba.h"
#include "Mario.h"

#include "Block.h"
#include "Platform.h"

CGoomba::CGoomba(float x, float y):
	CCreature(x, y)
{
	SetBoundingBox(GOOMBA_BBOX_WIDTH, GOOMBA_BBOX_HEIGHT);
	
	nx = DIRECTION_LEFT;
	life = GOOMBA_LIFE;
	SetState(STATE_LIVE);
}

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	//if (!e->obj->IsBlocking()) return; 
	if (dynamic_cast<CPlatform*>(e->obj))
		OnCollisionWithPlatform(e);
	else if (dynamic_cast<CBlock*>(e->obj))
		OnCollisionWithBlock(e);
	else if (dynamic_cast<CCreature*>(e->obj))
		OnCollisionWithCreature(e);
}

void CGoomba::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	if (e->ny)
	{
		vy = 0.0f;
		isOnGround = true;
	}
	if (e->nx)
	{
		vx = -vx;
	}
}

void CGoomba::OnCollisionWithBlock(LPCOLLISIONEVENT e)
{
	if (e->ny)
	{
		vy = 0.0f;
	}
	if (e->nx)
	{
		vx = -vx;
	}
}

void CGoomba::OnCollisionWithCreature(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CGoomba*>(e->obj));
		//Do nothing to fellows
	else
		Attack(e->obj);
}

void CGoomba::ReactionToAttack1(CGameObject* by_another)
{
	Die();
}

void CGoomba::ReactionToAttack2(CGameObject* by_another)
{
	Die();
}

void CGoomba::ReactionToAttack3(CGameObject* by_another)
{
	Die();
}

void CGoomba::SetState(int state)
{
	if (this->state == state)
	{
		return;
	}

	this->state = state;

	switch (state)
	{
		case STATE_LIVE:
			ToStateLiving();
			break;
		case STATE_DIE:
			ToStateDying();
			break;
	}
}

void CGoomba::ToStateLiving()
{
	vx = nx * GOOMBA_VX;
	vy = GOOMBA_VY;
}

void CGoomba::ToStateDying()
{
	die_start = 0;
	y += (bbox_height - GOOMBA_BBOX_HEIGHT_DIE) / 2.0f;
	bbox_height = GOOMBA_BBOX_HEIGHT_DIE;
	vx = 0.0f;
	vy = 0.0f;
}

void CGoomba::Living(DWORD dt)
{
	Move(dt);
}

void CGoomba::Dying(DWORD dt)
{
	die_start += dt;
	if (die_start >= GOOMBA_DIE_TIMEOUT)
	{
		this->Delete();
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	switch (state)
	{
	case STATE_LIVE:
		Living(dt);
		break;
	case STATE_DIE:
		Dying(dt);
		break;
	}
}

void CGoomba::ChangeAnimation()
{
	int object = ANI_ID_GOOMBA;
	int action = ANI_ID_GOOMBA_WALK;
	if (state == STATE_DIE)
	{
		action = ANI_ID_GOOMBA_DIE;
	}

	aniID = object + action;
}

void CGoomba::Render()
{
	ChangeAnimation();
	CAnimations::GetInstance()->Get(aniID)->Render(x,y);
	//RenderBoundingBox();
}