#pragma once

#include "Block.h"
#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define QUESTION_BLOCK_WIDTH 16
#define QUESTION_BLOCK_BBOX_WIDTH 16.0f
#define QUESTION_BLOCK_BBOX_HEIGHT 16.0f

#define SHAKE_VELOCITY 0.1f
#define SHAKE_TIME 50

#define STATE_QUESTION_BLOCK_ON 1
#define STATE_QUESTION_BLOCK_OFF 0

#define ID_ANI_QUESTION_BLOCK_TOGGLE_ON 12000
#define ID_ANI_QUESTION_BLOCK_TOGGLE_OFF 12001

class CQuestionBlock : public CBlock {
protected:
	float y_legacy;

	float time_elapsed = 0.0f;
	bool isToggle = false;
	bool isOverBound = false;
public:
	CQuestionBlock(float x, float y) : CBlock(x, y)
	{
		this->y_legacy = y;
		SetState(STATE_QUESTION_BLOCK_ON);

		bbox_width = QUESTION_BLOCK_BBOX_WIDTH; //Thêm dòng này để dùng CGameObject::GetBoundingBox(float& l, float& t, float& r, float& b)
		bbox_height = QUESTION_BLOCK_BBOX_HEIGHT;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b); //GameObject có GetBoundingBox rồi! "virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);"

	void ShakeToggle();
};