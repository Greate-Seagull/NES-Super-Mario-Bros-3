#include "Block.h"

#include "PlayScene.h"

CBlock::CBlock(float x, float y, int itemID):
	CMovableObject(x, y)
{
	this->itemID = itemID;

}

void CBlock::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (CCreature* creature = dynamic_cast<CCreature*>(e->obj))
		creature->OnReactionTo(e, ACTION_ATTACK_LEVEL_3);
}

void CBlock::UseDefaultItemPosition()
{
	PutItem(this->x, this->y);
}

int CBlock::IsLinkedTo(CGameObject* item)
{
	return this->item == item;
}

void CBlock::TriggerItem()
{
	if (item)
	{
		LPPLAYSCENE ps = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
		ps->Insert(item, ps->Find(this));
	}
}