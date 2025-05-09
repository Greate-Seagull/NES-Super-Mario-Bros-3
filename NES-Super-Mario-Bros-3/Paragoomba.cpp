#include "Paragoomba.h"
#include "Game.h"
#include "PlayScene.h"

CParagoomba::CParagoomba(float x, float y):
	CGoomba(x, y)
{
	bbox_width = PARAGOOMBA_BBOX_WIDTH;
	bbox_height = PARAGOOMBA_BBOX_HEIGHT;
	ClearState();
	SetState(STATE_LIVE);
}

void CParagoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	switch (state)
	{
		case STATE_LIVE:
			Living(dt);
			break;
		case PARAGOOMBA_STATE_GOOMBA:
			CGoomba::Living(dt);
			break;
		case STATE_DIE:
			CGoomba::Dying(dt);
			break;
	}
}

void CParagoomba::Living(DWORD dt)
{
	ChaseMario(dt);
	CGoomba::Living(dt);
	Flutter();
}

void CParagoomba::ReactionToAttack1(CGameObject* by_another)
{
	switch (state)
	{
	case STATE_LIVE:
		UnderAttack((CHarmfulObject*)by_another);
		LoseWings();
		SetState(PARAGOOMBA_STATE_GOOMBA);
		break;
	case PARAGOOMBA_STATE_GOOMBA:
		CGoomba::ReactionToAttack1(by_another);
		break;
	}
}

void CParagoomba::SetState(int state)
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
		case PARAGOOMBA_STATE_GOOMBA:
			CGoomba::ToStateLiving();
			break;
		case STATE_DIE:
			ToStateDying();
			break;
	}
}

void CParagoomba::ToStateLiving()
{
	vx = -GOOMBA_VX;
	vy = PARAGOOMBA_VY;	
	wings = new CWing(x, y - WINGS_Y_OFFSET, 2, WINGS_DISTANCE_BETWEEN);
	SetMomentum(0);
}

void CParagoomba::ToStateDying()
{	
	die_start = 0;
	y += (bbox_height - PARAGOOMBA_BBOX_DIE_HEIGHT) / 2;
	bbox_height = PARAGOOMBA_BBOX_DIE_HEIGHT;
	vx = 0.0f;
	vy = 0.0f;
	LoseWings();
}

void CParagoomba::Prepare(DWORD dt)
{
	switch (state)
	{
	case STATE_LIVE:
		ChaseMario(dt);
		CMovableObject::Prepare(dt);
		break;
	case PARAGOOMBA_STATE_GOOMBA:
		CMovableObject::Prepare(dt);
		break;
	}
}

void CParagoomba::ChangeAnimation()
{
	int object = ANI_ID_PARAGOOMBA;

	int action = ANI_ID_GOOMBA_WALK;
	if (state == STATE_DIE)
	{
		action = ANI_ID_GOOMBA_DIE;
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
	delete wings;
	wings = nullptr;
}

void CParagoomba::Flutter()
{
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
