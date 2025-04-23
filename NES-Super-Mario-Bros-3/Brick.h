#pragma once

#include "Block.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BRICK 10000
#define ID_ANI_BRICK_DIE 12001

#define BRICK_WIDTH 16.0f
#define BRICK_BBOX_WIDTH 16.0f
#define BRICK_BBOX_HEIGHT 16.0f

#define BRICK_SHAKE_VY -0.16f

#define BRICK_STATE_TOGGLE 10

#define ITEM_TYPE_NOTHING 0

class CBrick : public CBlock {
	//Containing
	int itemID;
	CGameObject* item;

	//Shaking
	float origin_y;
	bool opposite = false;
public:
	CBrick(float x, float y, int itemID = ITEM_TYPE_NOTHING);
	//GameObject co ham update roi! "virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};"
	//GameObject co ham render roi! "virtual void Render();"
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void InPhase(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void Reaction(CGameObject* by_another, int action);
	void SetState(int state);
	int GetItemID() { return itemID; }

	void TriggerItem();

	void Shaking(DWORD dt);
};