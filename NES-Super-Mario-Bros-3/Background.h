#pragma once

#include "Game.h"
#include "PlayScene.h"
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BACKGROUND -10000

#define BACKGROUND_WIDTH 2560
#define BACKGROUND_HEIGHT 393

class CBackground : public CGameObject {
public:
	CBackground(float x, float y) : CGameObject(x, y) { aniID = ID_ANI_BACKGROUND; }
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

class CReward : public CGameObject
{
protected:
	bool run;

	int time_elapsed;
	int type;
public:
	CReward(float x, float y) : CGameObject(x, y)
	{ 
		run = true;
		time_elapsed = 0;
		type = 3;		
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	void Switch(bool run) { this->run = run; }
};

class CRandomCard : public CGameObject
{
protected:
	int cellWidth, cellHeight;
	int spriteIdBeginBegin, spriteIdMiddleBegin, spriteIdEndBegin;
	int spriteIdBeginMiddle, spriteIdMiddleMiddle, spriteIdEndMiddle;
	int spriteIdBeginEnd, spriteIdMiddleEnd, spriteIdEndEnd;

	CReward* reward;
public:
	CRandomCard(float x, float y,
		int sprite_id_begin_begin, int sprite_id_middle_begin, int sprite_id_end_begin,
		int sprite_id_begin_middle, int sprite_id_middle_middle, int sprite_id_end_middle,
		int sprite_id_begin_end, int sprite_id_middle_end, int sprite_id_end_end)
		: CGameObject(x, y)
	{
		this->cellWidth = 16;
		this->cellHeight = 16;
		this->spriteIdBeginBegin = sprite_id_begin_begin;
		this->spriteIdMiddleBegin = sprite_id_middle_begin;
		this->spriteIdEndBegin = sprite_id_end_begin;
		this->spriteIdBeginMiddle = sprite_id_begin_middle;
		this->spriteIdMiddleMiddle = sprite_id_middle_middle;
		this->spriteIdEndMiddle = sprite_id_end_middle;
		this->spriteIdBeginEnd = sprite_id_begin_end;
		this->spriteIdMiddleEnd = sprite_id_middle_end;
		this->spriteIdEndEnd = sprite_id_end_end;

		reward = new CReward(x + cellWidth, y - cellHeight);
		LPPLAYSCENE playScene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
		playScene->Insert(reward, -1);
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	int IsCollidable() { return 1; }
	int IsBlocking() { return 0; }

	void Switch(bool run) { reward->Switch(run); }

	~CRandomCard() { delete reward; }
};

class CEndLevel : public CGameObject {
protected:
	int width, height;
	int spriteIdBegin, spriteIdEnd;

	float cell_width, cell_height;
public:
	CEndLevel(float x, float y,
		int width, int height,
		int sprite_id_begin, int sprite_id_end)
		: CGameObject(x, y)
	{
		this->cell_width = 16.0f;
		this->cell_height = 16.0f;
		this->width = width;
		this->height = height;
		this->spriteIdBegin = sprite_id_begin;
		this->spriteIdEnd = sprite_id_end;

		this->bbox_width = cell_width * this->width;
		this->bbox_height = cell_height * this->height;
	}
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
