#include "Block.h"

#include "PlayScene.h"
#include "Platform.h"

CBlock::CBlock(float x, float y, int itemID):
	CMovableObject(x, y)
{
	this->itemID = itemID;
}

void CBlock::OnCollisionWith(LPCOLLISIONEVENT e)
{
	e->Reverse();
	e->src_obj->OnCollisionWith(e);
}

void CBlock::UseDefaultItemPosition()
{
	PutItem(this->x, this->y);
}

int CBlock::IsLinkedTo(CGameObject* item)
{
	return this->item == item ||
		dynamic_cast<CFireball*>(item) ||
		dynamic_cast<CPlatform*>(item) ||
		dynamic_cast<CBlock*>(item);
}