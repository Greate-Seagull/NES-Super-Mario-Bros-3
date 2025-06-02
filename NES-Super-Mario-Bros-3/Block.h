#pragma once

#include "MovableObject.h"

#include "AssetIDs.h"

#define BLOCK_WITHOUT_ITEM -1
#define BLOCK_WITHOUT_ITEM_BREAKER -2

class CBlock : public CMovableObject
{
protected:
	//Containing
	int itemID;
	CGameObject* item;

public:

	CBlock(float x, float y, int itemID = BLOCK_WITHOUT_ITEM);

	virtual void Prepare(DWORD dt) {};

	virtual int IsCollidable() { return 1; }
	virtual int IsBlocking() { return 1; }
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual int IsLinkedTo(CGameObject* item);

	virtual void TakeItem() {};
	virtual void UseDefaultItemPosition();
	virtual void PutItem(float x, float y) { item->SetPosition(x, y); }	
};