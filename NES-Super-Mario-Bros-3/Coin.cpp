#include "Coin.h"

#include "Mario.h"
#include "PlayScene.h"

void CCoin::Prepare(DWORD dt)
{	
	if (isToggled)
		CMovableObject::Prepare(dt);
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isToggled)
	{
		Move(dt);

		if (y >= originalY)
		{
			/*LPPLAYSCENE currScene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
			currScene->InsertScore(x, y - 16, 100);*/
			Delete();
		}
	}		
}

int CCoin::IsLinkedTo(CGameObject* obj)
{
	return dynamic_cast<CMario*>(obj) == nullptr; 
}

void CCoin::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CMario*>(e->obj))
		Delete();
}

void CCoin::OnReactionTo(LPCOLLISIONEVENT e, int action)
{
	if (isContained)
	{
		CPlayScene* ps = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

		SetToggled();

		e->Reverse();
		e->src_obj = this;
		e->obj = ps->GetPlayer();
		e->obj->OnReactionTo(e, ACTION_TOUCH);
	}
	else
		Delete();
}

void CCoin::SetToggled()
{
	isToggled = true;
	vy = COIN_VELOCITY_Y;
}

bool CCoin::IsUnderOriginal()
{
	if (y < originalY) return false;
}