#pragma once

#include "Game.h"
#include "PlayScene.h"
#include "HUD.h"
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BACKGROUND -10000

#define ID_ANI_REWARD_BASE -60
#define ID_ANI_COLLECTED_REWARD_BASE -70

#define BACKGROUND_WIDTH 2560
#define BACKGROUND_HEIGHT 393

class CBackground : public CGameObject {
public:
	CBackground(float x, float y) : CGameObject(x, y) { aniID = ID_ANI_BACKGROUND; }
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

#define COURSE_CLEAR_TEXT -300
#define YOU_GOT_A_CARD_TEXT -301

#define COURSE_CLEAR_WIDTH 128
#define COURSE_CLEAR_HEIGHT 16

#define YOU_GOT_A_CARD_WIDTH 128
#define YOU_GOT_A_CARD_HEIGHT 16

class CClearText : public CGameObject {
public:
	CClearText(float x, float y, int aniID) : CGameObject(x, y)
	{
		this->aniID = aniID;

		if (aniID == COURSE_CLEAR_TEXT) SetBoundingBox(COURSE_CLEAR_WIDTH, COURSE_CLEAR_HEIGHT);
		else if (aniID == YOU_GOT_A_CARD_TEXT) SetBoundingBox(YOU_GOT_A_CARD_WIDTH, YOU_GOT_A_CARD_HEIGHT);
	}
};

#define REWARD_WIDTH 19
#define REWARD_HEIGHT 26

#define REWARD_FLOATING_SPEED 0.05f

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
		aniID = ID_ANI_REWARD_BASE - this->type;
		SetBoundingBox(REWARD_WIDTH, REWARD_HEIGHT);
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void Switch(bool run) { this->run = run; }
	int GetType() { return type; }
};

class CRandomCard : public CGameObject
{
protected:
	int cellWidth, cellHeight;
	int spriteIdBeginBegin, spriteIdMiddleBegin, spriteIdEndBegin;
	int spriteIdBeginMiddle, spriteIdMiddleMiddle, spriteIdEndMiddle;
	int spriteIdBeginEnd, spriteIdMiddleEnd, spriteIdEndEnd;

	CReward* reward;

	CClearText* courseClear;
	CClearText* youGotACard;
	CHUDCard* card;
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

	void Switch(bool run)
	{ 
		reward->Switch(run);
		if (!run)
		{
			courseClear = new CClearText(x, y - 64, COURSE_CLEAR_TEXT);
			youGotACard = new CClearText(x - 16, y - 48, YOU_GOT_A_CARD_TEXT);
			card = new CHUDCard(x + 64, y - 40, reward->GetType());

			LPPLAYSCENE playScene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
			playScene->Insert(courseClear, -1);
			playScene->Insert(youGotACard, -1);
			playScene->Insert(card, -1);
			playScene->InsertCard(reward->GetType());
		}
	}

	~CRandomCard() 
	{
		delete reward;
		if (courseClear) delete courseClear;
		if (youGotACard) delete youGotACard;
	}
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
