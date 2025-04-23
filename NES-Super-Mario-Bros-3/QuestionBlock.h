#pragma once

#include "Block.h"

#include "AssetIDs.h"
#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define QUESTION_BLOCK_BBOX_WIDTH 16.0f
#define QUESTION_BLOCK_BBOX_HEIGHT 16.0f

#define QUESTION_BLOCK_SHAKE_VY -0.16f

#define QUESTION_BLOCK_STATE_TOGGLE 10

#define ID_ANI_QUESTION_BLOCK 12000

#define OBJECT_TYPE_FIXED 0
#define OBJECT_TYPE_LIFE_DEPENDS 1

class CQuestionBlock : public CBlock {
protected:
	//Containing
	int itemTypeID;
	CGameObject* item;

	//Shaking
	float origin_y;

public:
	CQuestionBlock(float x, float y, int itemTypeID = OBJECT_TYPE_FIXED) : CBlock(x, y)
	{
		this->itemTypeID = itemTypeID;

		this->origin_y = y;

		SetState(STATE_LIVE);
		SetBoundingBox(QUESTION_BLOCK_BBOX_WIDTH, QUESTION_BLOCK_BBOX_HEIGHT);
	}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void InPhase(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void Render();
	void Reaction(CGameObject* by_another, int action);
	void SetState(int state);

	void TriggerItem();

	void Shaking(DWORD dt);
};