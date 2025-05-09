#include "Brick.h"
#include "PlayScene.h"
#include "PButton.h"

#include "AssetIDs.h"

#include "Animation.h"
#include "Animations.h"

CBrick::CBrick(float x, float y, int itemID):
	CBlock(x, y)
{
	bbox_width = BRICK_BBOX_WIDTH;
	bbox_height = BRICK_BBOX_HEIGHT;

	aniID = ID_ANI_BRICK;

	this->itemID = itemID;
	this->origin_y = y;

	SetState(STATE_LIVE);
	SetBoundingBox(BRICK_BBOX_WIDTH, BRICK_BBOX_HEIGHT);
}

void CBrick::Render()
{
	if (state == STATE_DIE && itemID == 45) aniID = ID_ANI_BRICK_DIE;
	else aniID = ID_ANI_BRICK;
	CAnimations::GetInstance()->Get(aniID)->Render(x, y);

	RenderBoundingBox();
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	InPhase(dt, coObjects);
}

void CBrick::InPhase(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	switch (state)
	{
	case BRICK_STATE_TOGGLE:
		Shaking(dt);
		break;
	}
}

void CBrick::Reaction(CGameObject* by_another, int action)
{
	if (state == STATE_DIE)
		return;

	switch (action)
	{
	case ACTION_DESTROY:
		if (itemID == OBJECT_TYPE_PBUTTON) SetState(BRICK_STATE_TOGGLE);
		else
		{
			BrickBursting();
			this->Delete();
		}
		break;
	case ACTION_TOUCH:
		SetState(BRICK_STATE_TOGGLE);
		break;
	}

	TriggerItem();
}

void CBrick::SetState(int state)
{
	if (this->state == state)
		return;

	this->state = state;

	switch (state)
	{
	case STATE_LIVE:
		break;
	case BRICK_STATE_TOGGLE:
		break;
	case STATE_DIE:
		vy = 0.0f;
		break;
	}
}

void CBrick::TriggerItem()
{
	if (!this->isItemTriggered)
	{
		switch (itemID)
		{
		case OBJECT_TYPE_PBUTTON:
		{
			item = new CPButton(x, y - BRICK_WIDTH);
			break;
		}
		default: return;
		}

		item->Reaction(this, ACTION_ATTACK);
		LPPLAYSCENE ps = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
		ps->Add(item);
		this->isItemTriggered = true;
	}
}

void CBrick::BrickBursting()
{
	LPPLAYSCENE ps = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();

	CBrickParticle* p1 = new CBrickParticle(x, y, 1);
	CBrickParticle* p2 = new CBrickParticle(x, y, 2);
	CBrickParticle* p3 = new CBrickParticle(x, y, 3);
	CBrickParticle* p4 = new CBrickParticle(x, y, 4);

	ps->Add(p1);
	ps->Add(p2);
	ps->Add(p3);
	ps->Add(p4);
}

void CBrick::Shaking(DWORD dt)
{
	//Accelerate(0.0f, GAME_GRAVITY, dt);
	if (!opposite) y += BRICK_SHAKE_VY * dt;
	else y -= BRICK_SHAKE_VY * dt;

	if (y <= origin_y - BRICK_WIDTH / 2)
	{
		opposite = true;
	}
	if (y >= origin_y)
	{
		y = origin_y;
		opposite = false;
		if (itemID != 0) SetState(STATE_DIE);
		else SetState(STATE_LIVE);
	}
}
