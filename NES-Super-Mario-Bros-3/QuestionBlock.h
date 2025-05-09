﻿#pragma once

#include "Block.h"

#define QUESTION_BLOCK_BBOX_WIDTH 16.0f
#define QUESTION_BLOCK_BBOX_HEIGHT 16.0f

#define QUESTION_BLOCK_SHAKE_VY -0.16f

#define QUESTION_BLOCK_STATE_TOGGLE 10

#define ID_ANI_QUESTION_BLOCK 12000

class CQuestionBlock : public CBlock {
protected:
	//Shaking
	float origin_y;

public:
	CQuestionBlock(float x, float y, int itemID = OBJECT_TYPE_COIN) : CBlock(x, y)
	{
		this->itemID = itemID;

		this->origin_y = y;

		SetState(STATE_LIVE);
		SetBoundingBox(QUESTION_BLOCK_BBOX_WIDTH, QUESTION_BLOCK_BBOX_HEIGHT);

		TakeItem();
	}

	void Prepare(DWORD dt);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void Render();
	void Reaction(CGameObject* by_another, int action);
	void SetState(int state);

	void TakeItem();
	void TriggerItem();
};