#include "SuperLeaf.h"

#include "Mario.h"

CSuperLeaf::CSuperLeaf(float x, float y):
	CHelpfulObject(x, y)
{
	bbox_height = LEAF_BBOX_HEIGHT;
	bbox_width = LEAF_BBOX_WIDTH;

	effect = EFFECT_RACOONIZE;

	SetState(LEAF_STATE_SLEEP);
}

void CSuperLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	dt = 16;
	InPhase(dt, coObjects);
}

void CSuperLeaf::InPhase(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	switch (state)
	{
		case LEAF_STATE_SLEEP:
			//Do nothing just sleep
			break;
		case LEAF_STATE_BLOWN:
			InPhaseBlownState(dt, coObjects);
			break;
		case LEAF_STATE_FALL:
			InPhaseFallingState(dt, coObjects);
			break;
	}
}

void CSuperLeaf::InPhaseBlownState(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Accelerate(0.0f, GAME_GRAVITY, dt);
	Move(dt);

	if (vy > 0)
	{
		SetState(LEAF_STATE_FALL);
	}
}

void CSuperLeaf::InPhaseFallingState(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Oscillate(dt);
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
}

void CSuperLeaf::ToStateBlownUp()
{
	vx = 0.0f;
	vy = LEAF_BLOWNUP_VY;
}

void CSuperLeaf::ToStateFalling()
{
	origin_x = x;
	origin_y = y;
	vx = 0.0f; //calculate by The equation of harmonic oscillation
	vy = 0.0f; //calculate by The equation of harmonic oscillation
	x_oscillate_phase = 0.0f;
	y_oscillate_phase = (float)M_PI / 2.5f;
	time = 0;
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
	RenderBoundingBox();
}

void CSuperLeaf::Oscillate(DWORD dt)
{
	time += dt;
	x = origin_x - LEAF_X_AMPLITUDE / LEAF_X_FREQUENCY * cos(LEAF_X_FREQUENCY * time + x_oscillate_phase) + LEAF_X_AMPLITUDE / LEAF_X_FREQUENCY;
	//y += LEAF_GRAVITY + LEAF_Y_AMPLITUDE * sin(y_oscillate_phase);
	y = origin_y - LEAF_Y_AMPLITUDE / LEAF_Y_FREQUENCY * sin(LEAF_Y_FREQUENCY * time + y_oscillate_phase) + LEAF_Y_AMPLITUDE / 3 * time + LEAF_Y_AMPLITUDE / LEAF_Y_FREQUENCY;
	//x_oscillate_phase = (LEAF_X_FREQUENCY * dt);
	//y_oscillate_phase += LEAF_Y_FREQUENCY * dt;
}
