#include "PButton.h"
#include "Mario.h"
#include "Coin.h"
#include "Brick.h"
#include "PlayScene.h"

#include "Game.h"
#include "debug.h"

#define SWITCH_DELAY 4000
float timeElapsed = 0;
bool isSwitched = false;
vector<LPGAMEOBJECT> coinsArchive;

void CPButton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isSwitched)
	{
		timeElapsed += dt;

		if (timeElapsed >= SWITCH_DELAY)
		{
			Reaction_Coin_To_Brick();
			isSwitched = false;

			coinsArchive.clear();
		}
	}

	aniID = ANI_PBUTTON_BASE + state;
}

void CPButton::OnReactionTo(CGameObject* by_another, int action)
{
	switch (action)
	{
	case ACTION_TOUCH:
	{
		if (dynamic_cast<CMario*>(by_another) && state == PBUTTON_CONSTRUCTED)
		{
			state = PBUTTON_PRESSED;
			isSwitched = true;
			Reaction_Brick_To_Coin();
		}
		break;
	}
	}
}

void CPButton::Reaction_Brick_To_Coin()
{
	LPPLAYSCENE ps = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	vector<LPGAMEOBJECT> objArray = ps->GetBrickObjects();

	for (int i = 0; i < objArray.size(); i++)
	{
		if (objArray[i])
		{
			CBrick* b = (CBrick*)objArray[i];
			if (b->GetItemID() == BLOCK_WITHOUT_ITEM)
			{
				if (!objArray[i]->IsDeleted())
				{
					float bX, bY;
					objArray[i]->GetPosition(bX, bY);
					objArray[i]->Delete();

					objArray[i] = new CCoin(bX, bY);
					ps->Insert(objArray[i], -1);

					coinsArchive.push_back(objArray[i]);
				}
			}
		}
	}
}

void CPButton::Reaction_Coin_To_Brick()
{
	LPPLAYSCENE ps = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();

	for (int i = 0; i < coinsArchive.size(); i++)
	{
		if (!coinsArchive[i]->IsDeleted())
		{
			if (coinsArchive[i]->GetState() == STATE_DIE)
			{
				float bX, bY;
				coinsArchive[i]->GetPosition(bX, bY);
				coinsArchive[i]->Delete();

				coinsArchive[i] = new CBrick(bX, bY, BLOCK_WITHOUT_ITEM);
				ps->Insert(coinsArchive[i], -1);
			}
		}
	}
}