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

void CParagoomba::InPhase(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	switch (state)
	{
		case STATE_LIVE:
			InPhaseLiving(dt, coObjects);
			break;
		case PARAGOOMBA_STATE_GOOMBA:
			CGoomba::InPhaseLiving(dt, coObjects);
			break;
		case STATE_DIE:
			/*if (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT)
			{
				this->Delete();
			}*/
			break;
	}
}

void CParagoomba::InPhaseLiving(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	ChaseMario(dt);
	CGoomba::InPhaseLiving(dt, coObjects);
	Flutter();
}

void CParagoomba::Reaction(CGameObject* by_another, int action)
{
	switch (state)
	{
		case STATE_LIVE:
			Reaction_LivingState(by_another, action);
			break;
		case PARAGOOMBA_STATE_GOOMBA:
			CGoomba::Reaction_LivingState(by_another, action);
			break;
	}
}

void CParagoomba::Reaction_LivingState(CGameObject* by_another, int action)
{
	switch (action)
	{
		case ACTION_ATTACK:
			UnderAttack((CHarmfulObject*)by_another);
			LoseWings();
			SetState(PARAGOOMBA_STATE_GOOMBA);
		case ACTION_DESTROY:
			Die();
			SetState(STATE_DIE);
			break;
		case ACTION_TOUCH:
			MeleeAttack(by_another);
			break;
		case ACTION_CARRY:
			AgainstControl();
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
	die_start = GetTickCount64();
	y += (bbox_height - PARAGOOMBA_BBOX_DIE_HEIGHT) / 2;
	bbox_height = PARAGOOMBA_BBOX_DIE_HEIGHT;
	vx = 0;
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
