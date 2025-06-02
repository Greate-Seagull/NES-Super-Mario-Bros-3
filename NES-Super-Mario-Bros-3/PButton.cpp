#include "PButton.h"
#include "Mario.h"
#include "Coin.h"
#include "Brick.h"
#include "PlayScene.h"

#define SWITCH_DELAY 4000

void CPButton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isSwitched)
	{
		timeElapsed += dt;

		if (timeElapsed >= SWITCH_DELAY)
		{
			ReplaceCoinsWithBricks();
		}
	}

	aniID = ANI_PBUTTON_BASE + state;
}

void CPButton::OnReactionTo(LPCOLLISIONEVENT e, int action)
{
	if (dynamic_cast<CMario*>(e->src_obj) && state == PBUTTON_CONSTRUCTED)
	{
		if (e->ny < 0)
		{
			state = PBUTTON_PRESSED;
			isSwitched = true;
			ReplaceBricksWithCoins();
		}
	}
}

void CPButton::ReplaceBricksWithCoins()
{
	LPPLAYSCENE ps = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	vector<LPGAMEOBJECT> objects = ps->GetObjects();

	for (auto& obj: objects)
	{
		if (CBrick* b = dynamic_cast<CBrick*>(obj))
		{
			if (b->IsTransformingBrick())
			{
				float bX, bY;
				b->GetPosition(bX, bY);
				b->Delete();

				CCoin* coin = new CCoin(bX, bY);
				coin->Refresh();
				coin->SetTransformed();
				ps->Insert(coin, -1);
			}
		}
	}
}

void CPButton::ReplaceCoinsWithBricks()
{
	LPPLAYSCENE ps = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	vector<LPGAMEOBJECT> objects = ps->GetObjects();

	for (auto& obj: objects)
	{
		if (CCoin* coin = dynamic_cast<CCoin*>(obj))
		{
			if (coin->IsTransformed())
			{
				float cX, cY;
				obj->GetPosition(cX, cY);
				obj->Delete();

				obj = new CBrick(cX, cY, BLOCK_WITHOUT_ITEM_BREAKER);
				obj->Refresh();
				ps->Insert(obj, -1);
			}
		}
	}
}