#include "Paragoomba.h"
#include "Game.h"
#include "PlayScene.h"

CParagoomba::CParagoomba(float x, float y):
	CGoomba(x, y)
{
	Refresh();
}

void CParagoomba::DefaultUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGoomba::Living(dt);
	Flutter();
}

void CParagoomba::OnReactionToAttack1(LPCOLLISIONEVENT e)
{
	switch (state)
	{
	case STATE_LIVE:
		CGoomba::OnReactionToAttack1(e);
		break;
	case PARAGOOMBA_STATE_WING:
		UnderAttack((CHarmfulObject*)e->src_obj);
		LoseWings();
		SetState(STATE_LIVE);
		break;
	}
}

void CParagoomba::OnReactionToAttack2(LPCOLLISIONEVENT e)
{
	LoseWings();
	CGoomba::OnReactionToAttack2(e);
}

void CParagoomba::OnReactionToAttack3(LPCOLLISIONEVENT e)
{
	LoseWings();
	CGoomba::OnReactionToAttack3(e);
}

void CParagoomba::ToDefaultState()
{
	vx = -GOOMBA_VX;
	vy = PARAGOOMBA_VY;
	wings = new CWing(x, y - WINGS_Y_OFFSET, 2, WINGS_DISTANCE_BETWEEN);
	SetMomentum(0);
}

void CParagoomba::DefaultPrepare(DWORD dt)
{
	ChaseMario(dt);
	CMovableObject::Prepare(dt);
}

void CParagoomba::ChangeAnimation()
{
	int object = ANI_ID_PARAGOOMBA;
	int action;

	switch (state)
	{
	case STATE_FLYINGOUT:
		action = ANI_ID_GOOMBA_WALK + ID_ANI_DIRECTION_UP;
		break;
	case STATE_DIE:
		action = ANI_ID_GOOMBA_DIE;
		break;
	default:
		action = ANI_ID_GOOMBA_WALK;
		break;
	}

	aniID = object + action;
}

void CParagoomba::Render()
{
	if(wings)
		wings->Render();
	CGoomba::Render();
}

//Chasing
void CParagoomba::LookforMario()
{
	//Get target
	LPPLAYSCENE playScene = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	CMario* mario = (CMario*)playScene->GetPlayer();

	//Determine target position
	float mario_x, mario_y;
	mario->GetPosition(mario_x, mario_y);

	//aim at target
	nx = (mario_x < x) ? DIRECTION_LEFT : DIRECTION_RIGHT;
	ny = 0;
}

void CParagoomba::ChaseMario(DWORD dt)
{
	if (momentum < 1)
	{
		start_momentum += dt;
		if (isOnGround && start_momentum >= PARAGOOMBA_CHASE_RUN_DURATION)
		{
			SetMomentum(momentum + 1);
		}
	}
	else if (momentum < PARAGOOMBA_CHASE_MAX_MOMENTUM)
	{
		if (isOnGround)
		{
			SetMomentum(momentum + 1);
		}
	}
	else
	{
		if (isOnGround)
		{
			SetMomentum(momentum + 1);
		}
	}
}

void CParagoomba::SetMomentum(int m)
{
	if (momentum == m)
	{
		return;
	}

	momentum = m % (PARAGOOMBA_CHASE_MAX_MOMENTUM + 1);

	if (momentum < 1)
	{
		LookforMario();
		vx = nx * GOOMBA_VX;
		vy = 0.0f;
		start_momentum = 0;
	}
	else if (momentum < PARAGOOMBA_CHASE_MAX_MOMENTUM)
	{		
		start_y = y;
		vy = PARAGOOMBA_VY;
	}
	else
	{
		vy = PARAGOOMBA_HIGH_JUMP_VY;
	}
}

//Swing
void CParagoomba::LoseWings()
{
	if (wings)
	{
		delete wings;
		wings = nullptr;
	}
}

void CParagoomba::Refresh()
{
	SetBoundingBox(PARAGOOMBA_BBOX_WIDTH, PARAGOOMBA_BBOX_HEIGHT);
	ClearState();
	SetState(PARAGOOMBA_STATE_WING);
}

void CParagoomba::Flutter()
{
	if (wings == nullptr)
		return;

	wings->SetPosition(x, y - WINGS_Y_OFFSET);

	if (vy > 0)
	{
		isOnGround = false;
		if (wings)
			wings->SetState(WINGS_STATE_OPEN);
	}
	else
	{		
		isOnGround = (vy == 0);
		if (wings)
			wings->SetState(WINGS_STATE_CLOSE);
	}
}
