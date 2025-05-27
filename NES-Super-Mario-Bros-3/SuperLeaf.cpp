#include "SuperLeaf.h"

#include "Mario.h"
#include "PlayScene.h"

CSuperLeaf::CSuperLeaf(float x, float y):
	CHelpfulObject(x, y)
{
	SetBoundingBox(LEAF_BBOX_WIDTH, LEAF_BBOX_HEIGHT);

	effect = EFFECT_RACOONIZE;

	SetState(LEAF_STATE_SLEEP);
}

void CSuperLeaf::Prepare(DWORD dt)
{
	switch (state)
	{
	case LEAF_STATE_BLOWN:
		if (vy > 0) SetState(LEAF_STATE_FALL);
		CMovableObject::Prepare(dt);
		break;
	case LEAF_STATE_FALL:
		ChangeDirection();
		Oscillate(dt);
		break;
	}
}

void CSuperLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Move(dt);	
}

void CSuperLeaf::SetState(int state)
{
	if (this->state == state)
		return;

	this->state = state;

	switch (state)
	{
		case LEAF_STATE_SLEEP:
			ToStateSleeping();
			break;
		case LEAF_STATE_BLOWN:
			ToStateBlownUp();
			break;
		case LEAF_STATE_FALL:
			ToStateFalling();
			break;
		case STATE_DIE:
			this->Delete();
			break;
	}
}

void CSuperLeaf::ToStateSleeping()
{
	vx = 0.0f;
	vy = 0.0f;

	nx = DIRECTION_LEFT;
	ny = DIRECTION_UP;
}

void CSuperLeaf::ToStateBlownUp()
{
	vx = 0.0f;
	vy = LEAF_BLOWNUP_VY;

	//nx remains;
	ny = (vy > 0) ? DIRECTION_DOWN : DIRECTION_UP;
}

void CSuperLeaf::ToStateFalling()
{
	origin_x = x;
	origin_y = y;

	x_oscillate_phase = 0.0f;
	y_oscillate_phase = (float)M_PI / 2.5f;

	time = 0;

	ChangeDirection();
}

void CSuperLeaf::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (CMario* mario = dynamic_cast<CMario*>(e->obj))
	{
		LPPLAYSCENE currentScene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
		currentScene->InsertScore(x, y - 16, 1000);

		if (mario->GetLife() != MARIO_LEVEL_RACOON) LaunchEffect(mario);
		SetState(STATE_DIE);
	}
}

void CSuperLeaf::Reaction(CGameObject* by_another, int action)
{
	switch (state)
	{
		case LEAF_STATE_SLEEP:
			ReactionInSleepingState(by_another, action);
			break;
		case LEAF_STATE_BLOWN:
		case LEAF_STATE_FALL:
			ReactionInFallingState(by_another, action);
			break;
	}
}

void CSuperLeaf::ReactionInSleepingState(CGameObject* by_another, int action)
{
	SetState(LEAF_STATE_BLOWN);
}

void CSuperLeaf::ReactionInFallingState(CGameObject* by_another, int action)
{
	if (CMario* mario = dynamic_cast<CMario*>(by_another))
	{
		LaunchEffect(mario);
		SetState(STATE_DIE);
	}
}

void CSuperLeaf::ChangeAnimation()
{
	aniID = ANI_ID_LEAF;

	aniID += (nx <= 0) ? ANI_ID_LEAF_LEFT : ANI_ID_LEAF_RIGHT;
}

void CSuperLeaf::Render()
{
	ChangeAnimation();
	CAnimations::GetInstance()->Get(aniID)->Render(x, y);
	//RenderBoundingBox();
}

void CSuperLeaf::Oscillate(DWORD dt)
{
	float new_x = x, new_y = y;

	//Oscillate
	time += dt;
	new_x = origin_x - LEAF_X_AMPLITUDE / LEAF_X_FREQUENCY * cos(LEAF_X_FREQUENCY * time + x_oscillate_phase) + LEAF_X_AMPLITUDE / LEAF_X_FREQUENCY; //use integral of harmonic oscillation eqation instead of harmonic oscillation eqation for more accurate position
	new_y = origin_y - LEAF_Y_AMPLITUDE / LEAF_Y_FREQUENCY * sin(LEAF_Y_FREQUENCY * time + y_oscillate_phase) + LEAF_Y_AMPLITUDE / 3 * time + LEAF_Y_AMPLITUDE / LEAF_Y_FREQUENCY;

	//Update velocity
	vx = (new_x - x) / dt;
	vy = (new_y - y) / dt;
}

void CSuperLeaf::ChangeDirection()
{
	if (vy > 0 && ny == DIRECTION_UP) //Direction contradiction
	{
		nx = -nx;
		ny = -ny;
	}
	else if (vy < 0 && ny == DIRECTION_DOWN)
	{
		ny = -ny;
	}
}
