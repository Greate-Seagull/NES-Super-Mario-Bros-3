#include "Goomba.h"
#include "Mario.h"
#include "Platform.h"

CGoomba::CGoomba(float x, float y):
	CCreature(x, y)
{
	SetBoundingBox(GOOMBA_BBOX_WIDTH, GOOMBA_BBOX_HEIGHT);
	
	nx = DIRECTION_LEFT;
	life = GOOMBA_LIFE;
	//die_start = -1;
	SetState(STATE_LIVE);
}

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	//if (!e->obj->IsBlocking()) return; 
	//if (dynamic_cast<CGoomba*>(e->obj)) return; 
	if (dynamic_cast<CPlatform*>(e->obj))
	{
		OnCollisionWithPlatform(e);
	}
}

void CGoomba::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	if (e->ny)
	{
		vy = 0;
		isOnGround = true;
	}
	if (e->nx)
	{
		vx = -vx;
	}
}

void CGoomba::Reaction(CGameObject* by_another, int action)
{
	switch (state)
	{
		case STATE_LIVE:
			Reaction_LivingState(by_another, action);
			break;
	}
}

void CGoomba::Reaction_LivingState(CGameObject* by_another, int action)
{
	switch (action)
	{
		case ACTION_ATTACK:
		case ACTION_DESTROY:
			Die();
			break;
		case ACTION_TOUCH:
			MeleeAttack(by_another);
			break;
		case ACTION_CARRY:
			AgainstControl();
			break;
	}
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
}

void CGoomba::InPhase(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	switch (state)
	{
		case STATE_LIVE:	
			InPhaseLiving(dt, coObjects);
			break;
		case STATE_DIE:
			InPhaseDying(dt, coObjects);		
			break;
	}
}

void CGoomba::InPhaseLiving(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Accelerate(0.0f, GAME_GRAVITY, dt);
	CCollision::GetInstance()->Process(this, dt, coObjects);
	Move(dt);
}

void CGoomba::InPhaseDying(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	die_start += dt;
	if (die_start >= GOOMBA_DIE_TIMEOUT)
	{
		this->Delete();
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	dt = 16;
	InPhase(dt, coObjects);	
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