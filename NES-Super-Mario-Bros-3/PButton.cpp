#include "PButton.h"
#include "Mario.h"
#include "Coin.h"
#include "Brick.h"

#include "Game.h"
#include "debug.h"

#define SWITCH_DELAY 2000
float timeElapsed = 0;
bool isSwitched = false;

vector<CCoin*> coinsArchive;
vector<CBrick*> bricksArchive;

void CPButton::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ANI_PBUTTON_BASE + state)->Render(x, y);

	RenderBoundingBox();
}

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
			bricksArchive.clear();
		}
	}
}

void CPButton::Reaction(CGameObject* by_another, int action)
{
	switch (action)
	{
	case ACTION_PRESS_BUTTON:
	{
		if (dynamic_cast<CMario*>(by_another) && state == PBUTTON_CONSTRUCTED)
		{
			state = PBUTTON_PRESSED;
			isSwitched = true;
			Reaction_Brick_To_Coin();
		}
	}
	}
}

void CPButton::Reaction_Brick_To_Coin()
{
	vector<LPGAMEOBJECT> objArray;
	CGame::GetInstance()->GetCurrentScene()->GetObjects(objArray);
	for (int i = 0; i < objArray.size(); i++)
	{
		if (!objArray[i]->IsDeleted())
		{
			float bX, bY;
			objArray[i]->GetPosition(bX, bY);
			objArray[i]->Delete();

			CCoin* c = new CCoin(bX, bY, false);
			coinsArchive.push_back(c);
		}
	}

	for (int i = 0; i < coinsArchive.size(); i++)
	{
		CGame::GetInstance()->GetCurrentScene()->Add(coinsArchive[i]);
	}
}

void CPButton::Reaction_Coin_To_Brick()
{
	for (int i = 0; i < coinsArchive.size(); i++)
	{
		if (!coinsArchive[i]->IsDeleted())
		{
			float bX, bY;
			coinsArchive[i]->GetPosition(bX, bY);
			coinsArchive[i]->Delete();

			CBrick* b = new CBrick(bX, bY);
			bricksArchive.push_back(b);
		}
	}

	for (int i = 0; i < bricksArchive.size(); i++)
	{
		CGame::GetInstance()->GetCurrentScene()->Add(bricksArchive[i]);
	}
}