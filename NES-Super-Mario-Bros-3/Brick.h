#pragma once

#include "Block.h"
#include "BrickParticle.h"

#include "PlayScene.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BRICK 10000
#define ID_ANI_BRICK_DIE 12001

#define BRICK_WIDTH 16.0f
#define BRICK_BBOX_WIDTH 16.0f
#define BRICK_BBOX_HEIGHT 16.0f

#define BRICK_SHAKE_VY -0.16f

#define BRICK_STATE_TOGGLE 10

class CBrick : public CBlock {
protected:
	float origin_y;
	bool opposite = false;
	bool transform;
public:
	CBrick(float x, float y, int itemID = BLOCK_WITHOUT_ITEM, bool tranform = false) :CBlock(x, y, itemID)
	{
		bbox_width = BRICK_BBOX_WIDTH;
		bbox_height = BRICK_BBOX_HEIGHT;

		aniID = ID_ANI_BRICK;

		this->origin_y = y;
		this->transform = tranform;

		SetState(STATE_LIVE);
		SetBoundingBox(bbox_width, bbox_height);
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void InPhase(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void OnReactionTo(LPCOLLISIONEVENT e, int action);
	void SetState(int state);
	int GetItemID() { return itemID; }

	void TakeItem();
	void DetermineItem(CMario* mario);
	void TriggerItem(LPCOLLISIONEVENT e, int action);

	void Shaking(DWORD dt);
	void BlastingBrickParticles();

	bool IsTransformingBrick() { return transform; }
};