#include "Brick.h"
#include "PButton.h"
#include "PlayScene.h"
#include "SuperMushroom.h"
#include "SuperLeaf.h"
#include "Coin.h"
#include "KoopaTroopa.h"

#include "debug.h"

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	InPhase(dt, coObjects);

	if (state == STATE_DIE && itemID != BLOCK_WITHOUT_ITEM) aniID = ID_ANI_BRICK_DIE;
	else aniID = ID_ANI_BRICK;
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
	if (state == STATE_DIE || by_another == NULL)
		return;

	switch (action)
	{
	case ACTION_ATTACK_LEVEL_3:
		switch (itemID)
		{
			case BLOCK_WITHOUT_ITEM:
			{
				BlastingBrickParticles();
				this->Delete();
				break;
			}
			case BLOCK_WITHOUT_ITEM_BREAKER:
			{
				BlastingBrickParticles();
				this->Delete();
				break;
			}
			default: SetState(BRICK_STATE_TOGGLE); break;
		}
		break;
	case ACTION_TOUCH:
		switch (itemID)
		{
			case BLOCK_WITHOUT_ITEM_BREAKER:
			{
				BlastingBrickParticles();
				this->Delete();
				break;
			}
			default: SetState(BRICK_STATE_TOGGLE); break;
		}
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
	switch (itemID)
	{
		case OBJECT_TYPE_PBUTTON: item = new CPButton(x, y - BRICK_WIDTH); break;
		case OBJECT_TYPE_SUPER_MUSHROOM: item = new CSuperMushroom(x, y); break;
		case OBJECT_TYPE_SUPER_LEAF: item = new CSuperLeaf(x, y); break;
		case OBJECT_TYPE_COIN: item = new CCoin(x, y); break;
	default: return;
	}

	item->Reaction(this, ACTION_TOUCH);
	LPPLAYSCENE ps = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	int index = ps->Find(this);
	if (itemID != OBJECT_TYPE_PBUTTON) ps->Insert(item, index - 1);
	else ps->Insert(item, -1);
}

void CBrick::Shaking(DWORD dt)
{
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
		if (itemID != BLOCK_WITHOUT_ITEM)
		{
			bounceCount--;
			if (bounceCount > 0) SetState(STATE_LIVE);
			else SetState(STATE_DIE);
		}
		else SetState(STATE_LIVE);
	}
}

void CBrick::BlastingBrickParticles()
{
	CBrickParticle* brickParticles[4];
	brickParticles[0] = new CBrickParticle(x - 8, y - 8, 1);
	brickParticles[1] = new CBrickParticle(x + 8, y - 8, 2);
	brickParticles[2] = new CBrickParticle(x - 8, y + 8, 3);
	brickParticles[3] = new CBrickParticle(x + 8, y + 8, 4);

	LPPLAYSCENE currentScene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	for (int i = 0; i < 4; i++)
	{
		currentScene->Insert(brickParticles[i], -1);
	}
}