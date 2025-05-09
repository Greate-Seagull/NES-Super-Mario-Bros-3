#include "Game.h"
#include "PlayScene.h"

#include "PiranhaPlant.h"
#include "Mario.h"
#include "debug.h"

CPiranhaPlant::CPiranhaPlant(float x, float y):
	CCreature(x, y)
{
	bbox_height = PIRANHA_BBOX_HEIGHT;
	bbox_width = PIRANHA_BBOX_WIDTH;

	vx = PIRANHA_VX;
	vy = PIRANHA_VY;

	SetState(PIRANHA_STATE_EMERGE);
	life = PIRANHA_LIFE;	
}

void CPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	dt = 16;
	InPhase(dt, coObjects);	
	LookforMario();
	Move(dt);
}

void CPiranhaPlant::InPhase(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{	
	switch (state)
	{
		case PIRANHA_STATE_EMERGE:
			InPhaseEmerge();
			break;
		case PIRANHA_STATE_ATTACK:
			InPhaseAttack(dt);
			break;
		case PIRANHA_STATE_DIG:
			InPhaseDig();
			break;
		case PIRANHA_STATE_HIDE:			
			InPhaseHide(dt);
			break;
	}
}

void CPiranhaPlant::InPhaseEmerge()
{
	if (abs(y - start_y) >= bbox_height)
	{
		SetState((this->state + 1) % PIRANHA_NUMBER_STATE);
	}
}

void CPiranhaPlant::InPhaseAttack(DWORD dt)
{
	start_action_time += dt;
	if (start_action_time >= PIRANHA_ATTACK_TIME)
	{
		SetState((this->state + 1) % PIRANHA_NUMBER_STATE);
	}
}

void CPiranhaPlant::InPhaseDig()
{
	if (fabs(y - start_y) >= bbox_height)
	{
		SetState((this->state + 1) % PIRANHA_NUMBER_STATE);
	}
}

void CPiranhaPlant::InPhaseHide(DWORD dt)
{
	start_action_time += dt;
	if (fabs(target_dx) <= PIRANHA_DISTANCE_HIDE)
	{
		//just hide
	}
	else if (start_action_time >= PIRANHA_HIDE_TIME)
	{
		SetState((this->state + 1) % PIRANHA_NUMBER_STATE);
	}
}

void CPiranhaPlant::SetState(int state) //Start state
{
	if (this->state == state)
	{
		return;
	}

	this->state = state;

	switch (state)
	{
		case PIRANHA_STATE_EMERGE:
			ToStateEmerge();
			break;
		case PIRANHA_STATE_ATTACK:
			ToStateAttack();
			break;
		case PIRANHA_STATE_DIG:
			ToStateDig();
			break;
		case PIRANHA_STATE_HIDE:
			ToStateHide();
			break;
		case STATE_DIE:
			Delete();
			break;
	}
}

void CPiranhaPlant::ToStateEmerge()
{
	start_y = y;
	vy = -PIRANHA_VY;
}

void CPiranhaPlant::ToStateAttack()
{
	start_action_time = 0;
	vy = 0.0f;
}

void CPiranhaPlant::ToStateDig()
{
	start_y = y;
	vy = PIRANHA_VY;
}

void CPiranhaPlant::ToStateHide()
{
	start_action_time = 0;
	vy = 0.0f;
}

void CPiranhaPlant::ReactionToCarry(CGameObject* by_another)
{
	AgainstControl();
	HigherAttack(by_another);
}

void CPiranhaPlant::ReactionToTouch(CGameObject* by_another)
{
	HigherAttack(by_another);
}

void CPiranhaPlant::ReactionToAttack1(CGameObject* by_another)
{
	HigherAttack(by_another);
}

void CPiranhaPlant::ReactionToAttack2(CGameObject* by_another)
{
	Die();
}

void CPiranhaPlant::ReactionToAttack3(CGameObject* by_another)
{
	Die();
}

void CPiranhaPlant::Render()
{
	/*if (state == PIRANHA_STATE_RELOAD) 
	{
		return;
	}*/

	CAnimations* animations = CAnimations::GetInstance();

	ChangeAnimation();

	animations->Get(aniID)->Render(x, y);

	//RenderBoundingBox();
}

void CPiranhaPlant::ChangeAnimation()
{
	int object = ANI_ID_PIRANHA;

	int action = ANI_ID_PIRANHA_BITE;

	aniID = object + action;
}

void CPiranhaPlant::LookforMario()
{
	//Get target
	LPPLAYSCENE playScene = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	CMario* mario = (CMario*)playScene->GetPlayer();

	//Determine target position
	float mario_x, mario_y;
	mario->GetPosition(mario_x, mario_y);

	//aim at target
	target_dx = mario_x - x;
	target_dy = mario_y - y;	
}