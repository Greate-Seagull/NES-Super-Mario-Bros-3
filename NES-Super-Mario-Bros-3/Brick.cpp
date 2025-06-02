#include "Brick.h"

#include "PlayScene.h"

#include "PButton.h"
#include "SuperMushroom.h"
#include "SuperLeaf.h"
#include "Coin.h"

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

void CBrick::OnReactionTo(LPCOLLISIONEVENT e, int action)
{
	if (state == STATE_DIE)
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
				if (e->ny > 0) 
				{
					BlastingBrickParticles();
					this->Delete();
					break;
				}
				else return;
			}
			default: 
			{
				if (e->ny > 0) SetState(BRICK_STATE_TOGGLE);
				else return;
			}
		}
		break;
	}

	TriggerItem(e, action);
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

void CBrick::TakeItem()
{
	if (item)
		return;

	switch (itemID)
	{
	case OBJECT_TYPE_PBUTTON: 
		item = new CPButton(x, y - BRICK_WIDTH - 1.0f); 
		break;
	case OBJECT_TYPE_COIN:
	{
		CCoin* coin = new CCoin(x, y);
		coin->SetContained();
		item = coin;
		break;
	}
	case OBJECT_TYPE_SUPER_MUSHROOM:
		item = new CSuperMushroom(x, y);
		break;
	case OBJECT_TYPE_SUPER_LEAF:
		item = new CSuperLeaf(x, y);
		break;
	default:
		item = nullptr;
	}
}

void CBrick::DetermineItem(CMario* mario)
{
	if (mario->GetLife() < MARIO_LEVEL_BIG)
		itemID = OBJECT_TYPE_SUPER_MUSHROOM;
	else
		itemID = OBJECT_TYPE_SUPER_LEAF;
}

void CBrick::TriggerItem(LPCOLLISIONEVENT e, int action)
{
	if (item == nullptr)
	{
		LPPLAYSCENE ps = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();

		if (itemID == OBJECT_TYPE_AMBIGUOUS_EFFECT)
		{
			CMario* mario = dynamic_cast<CMario*>(ps->GetPlayer());
			DetermineItem(mario);
		}

		TakeItem();

		if (item == nullptr)
			return;

		item->Refresh();
		ps->Insert(item, ps->Find(this));
		
		if(itemID != OBJECT_TYPE_PBUTTON)
			item->OnReactionTo(e, action);

		if (itemID == OBJECT_TYPE_COIN)
			item = nullptr; //Clear to create more coins
	}
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
			toggle_number--;
			if (toggle_number > 0) SetState(STATE_LIVE);
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