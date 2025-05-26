#include "PiranhaPlant.h"
#include "Game.h"
#include "PlayScene.h"

CPiranhaPlant::CPiranhaPlant(float x, float y):
	CEnemy(x, y)
{
	Refresh();
}

void CPiranhaPlant::SetPosition(float x, float y)
{
	CGameObject::SetPosition(x, y);
	start_y = y;
}

void CPiranhaPlant::Prepare(DWORD dt)
{
	switch (state)
	{
	case PIRANHA_STATE_EMERGE:
		Emerging(dt);
		break;
	case PIRANHA_STATE_ATTACK:
		Attacking(dt);
		break;
	case PIRANHA_STATE_DIG:
		Digging(dt);
		break;
	case PIRANHA_STATE_HIDE:
		Hiding(dt);
		break;
	case STATE_DIE:
		Dying(dt);
		break;
	}	
}

void CPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	LookForMario();
	Move(dt);
}

void CPiranhaPlant::Emerging(DWORD dt)
{
	if (y <= start_y - bbox_height)
	{
		y = start_y - bbox_height;
		SetState((this->state + 1) % PIRANHA_NUMBER_STATE);
	}
}

void CPiranhaPlant::Attacking(DWORD dt)
{
	start_action_time += dt;
	if (start_action_time >= PIRANHA_ATTACK_TIME)
	{
		SetState((this->state + 1) % PIRANHA_NUMBER_STATE);
	}
}

void CPiranhaPlant::Digging(DWORD dt)
{
	if (y >= start_y)
	{
		y = start_y;
		SetState((this->state + 1) % PIRANHA_NUMBER_STATE);
	}
}

void CPiranhaPlant::Hiding(DWORD dt)
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

void CPiranhaPlant::Dying(DWORD dt)
{
	die_time += dt;
	if (die_time >= PIRANHA_DIE_TIME)
	{
		Delete();
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
			ToStateDie();
			break;
	}
}

void CPiranhaPlant::ToStateEmerge()
{
	vy = -PIRANHA_VY;
}

void CPiranhaPlant::ToStateAttack()
{
	start_action_time = 0;
	vy = 0.0f;
}

void CPiranhaPlant::ToStateDig()
{
	vy = PIRANHA_VY;
}

void CPiranhaPlant::ToStateHide()
{
	start_action_time = 0;
	vy = 0.0f;
}

void CPiranhaPlant::ToStateDie()
{
	die_time = 0;
}

void CPiranhaPlant::OnReactionToCarrying(LPCOLLISIONEVENT e)
{
	if(pot)
	{
		AgainstControl();

		if (dynamic_cast<CMario*>(e->src_obj))
		{
			e->Reverse();
			Attack(e);
		}
	}
	else
	{
		pot = e->src_obj;
	}
}

void CPiranhaPlant::OnReactionToTouching(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CMario*>(e->src_obj))
	{
		e->Reverse();
		Attack(e);
	}
}

void CPiranhaPlant::OnReactionToAttack1(LPCOLLISIONEVENT e)
{
	e->Reverse();
	Attack(e);
}

void CPiranhaPlant::OnReactionToAttack2(LPCOLLISIONEVENT e)
{
	Die();
}

void CPiranhaPlant::OnReactionToAttack3(LPCOLLISIONEVENT e)
{
	Die();
}

void CPiranhaPlant::LookForMario()
{
	//Get target
	LPPLAYSCENE playScene = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	CMario* mario = (CMario*)playScene->GetPlayer();

	//aim at target
	target_dx = mario->GetX() - x;
	target_dy = mario->GetY() - y;
}

void CPiranhaPlant::Refresh()
{
	start_y = y;

	SetBoundingBox(PIRANHA_BBOX_WIDTH, PIRANHA_BBOX_HEIGHT);

	maxVx = PIRANHA_VX;
	vx = PIRANHA_VX;
	vy = PIRANHA_VY;

	SetState(PIRANHA_STATE_EMERGE);
	life = PIRANHA_LIFE;
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

	int action;
	switch (state)
	{
	case STATE_DIE:
		action = ANI_ID_PIRANHA_DIE;
		break;
	default:
		action = ANI_ID_PIRANHA_BITE;
		break;
	}

	aniID = object + action;
}
