#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Background.h"
#include "HUD.h"
#include "Portal.h"
#include "DeadStateTrigger.h"
#include "Coin.h"
#include "Platform.h"
#include "VenusFireTrap.h"
#include "KoopaTroopa.h"
#include "SuperMushroom.h"
#include "SuperLeaf.h"
#include "QuestionBlock.h"
#include "Pipe.h"
#include "Container.h"
#include "Bush.h"
#include "StripedBrick.h"
#include "Cloud.h"
#include "MapIcon.h"
#include "BrickParticle.h"
#include "PButton.h"

#include "SampleKeyEventHandler.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	player = NULL;
	//key_handler = new CSampleKeyHandler(this);
	background = NULL;	
	hud = NULL;
	for (int i = 0; i < DIGIT_COUNT_SCORE; i++)
		scoreDigits[i] = NULL;
	for (int i = 0; i < DIGIT_COUNT_CURRENCY; i++)
		coinDigits[i] = NULL;
	for (int i = 0; i < DIGIT_COUNT_TIME; i++)
		timeDigits[i] = NULL;
	for (int i = 0; i < P_METER_COUNT; i++)
		pMeter[i] = NULL;
}

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define OFFSET 32

#define MAX_CAMERA_POSITION 1792

#define SCENE_SWITCH_WAIT_TIME 6000

#define MARIO_LEFT_BOUNDARY 8

float tempCamPosY = 0;

bool isPipeSpawned = false;
float newMarioX, newMarioY;
float newMarioLife = MARIO_LEVEL_SMALL;
float newMarioWarpDirection;

bool toggleSceneSwitch = false;
float wait_time = 0;
int next_level_scene;

#pragma region HUD INFORMATION
int coin = 0;

float timer = TIMER_VALUE;
bool timerPause = false;

#define SCORE_PER_SECOND 50
int score = 0;

int p_progress = 7;
float p_run_time = 0;

int cardID[HUD_CARD_COUNT] = { 0, 0, 0 };
#pragma endregion

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}
	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);
	
	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case NON_OBJECT_TYPE_BACKGROUND:
	{
		obj = new CBackground(x, y);
		background = (CBackground*)obj;
		return;
	}
	case NON_OBJECT_TYPE_ENDING:
	{
		float width = (float)atof(tokens[3].c_str());
		float height = (float)atof(tokens[4].c_str());
		int sprite_begin = atoi(tokens[5].c_str());
		int sprite_end = atoi(tokens[6].c_str());

		obj = new CEndLevel(x, y, width, height, sprite_begin, sprite_end);
		break;
	}
	case NON_OBJECT_TYPE_CARD_RANDOM:
	{
		int sprite_begin_begin = atoi(tokens[3].c_str());
		int sprite_middle_begin = atoi(tokens[4].c_str());
		int sprite_end_begin = atoi(tokens[5].c_str());
		int sprite_begin_middle = atoi(tokens[6].c_str());
		int sprite_middle_middle = atoi(tokens[7].c_str());
		int sprite_end_middle = atoi(tokens[8].c_str());
		int sprite_begin_end = atoi(tokens[9].c_str());
		int sprite_middle_end = atoi(tokens[10].c_str());
		int sprite_end_end = atoi(tokens[11].c_str());

		int next_level = atoi(tokens[12].c_str());

		obj = new CRandomCard(x, y,
			sprite_begin_begin, sprite_middle_begin, sprite_end_begin,
			sprite_begin_middle, sprite_middle_middle, sprite_end_middle,
			sprite_begin_end, sprite_middle_end, sprite_end_end);
		next_level_scene = next_level;
		break;
	}
	case DEAD_STATE_TRIGGER:
	{
		float width = (float)atof(tokens[3].c_str());
		float height = (float)atof(tokens[4].c_str());

		obj = new CDeadStateTrigger(x, y, width, height);
		break;
	}
	case NON_OBJECT_TYPE_MINI_BUSH: obj = new CMiniBush(x, y); break;
	case NON_OBJECT_TYPE_BIG_BUSH:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int height = atoi(tokens[5].c_str());
		int sprite_begin_begin = atoi(tokens[6].c_str());
		int sprite_end_begin = atoi(tokens[7].c_str());
		int sprite_begin_end = atoi(tokens[8].c_str());
		int sprite_end_end = atoi(tokens[9].c_str());

		obj = new CBigBush
			(x, y, cell_width, cell_height, height,
			sprite_begin_begin, sprite_end_begin,
			sprite_begin_end, sprite_end_end);
		break;
	}
	case NON_OBJECT_TYPE_CLOUD:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin_begin = atoi(tokens[6].c_str());
		int sprite_middle_begin = atoi(tokens[7].c_str());
		int sprite_end_begin = atoi(tokens[8].c_str());

		if (tokens.size() > 9)
		{
			int sprite_begin_end = atoi(tokens[9].c_str());
			int sprite_middle_end = atoi(tokens[10].c_str());
			int sprite_end_end = atoi(tokens[11].c_str());

			obj = new CCloud
			(x, y, cell_width, cell_height, length + 2,
				sprite_begin_begin, sprite_middle_begin, sprite_end_begin,
				sprite_begin_end, sprite_middle_end, sprite_end_end);
			break;
		}
		else
		{
			obj = new CCloud
			(x, y, cell_width, cell_height, length, 
				sprite_begin_begin, sprite_middle_begin, sprite_end_begin);
			break;
		}
	}
	case NON_OBJECT_TYPE_MAP_ICON:
	{
		int icon_type = atoi(tokens[3].c_str());

		obj = new CMapIcon(x, y, icon_type);
		break;
	}
	case NON_OBJECT_TYPE_HUD:
	{
		obj = new CHud(x, y);
		if (hud)
		{
			hud->SetPosition(x, y);
			break;
		}
		hud = (CHud*)obj;
		break;
	}

	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}

		if (isPipeSpawned)
		{
			x = newMarioX;
			y = newMarioY;
		}
		obj = new CMario(x,y); 
		player = (CMario*)obj;
		player->SetLife(newMarioLife);

		if (isPipeSpawned)
		{
			if (newMarioWarpDirection == 1) player->SetState(MARIO_PIPE_EXIT_UP);
			else if (newMarioWarpDirection == -1)
			{
				if (id != 4) player->SetState(MARIO_PIPE_EXIT_DOWN);
				else player->SetState(MARIO_PIPE_EXIT_UP);
			}
			isPipeSpawned = false;
		}

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(x,y); break;
	case OBJECT_TYPE_PARAGOOMBA: obj = new CParagoomba(x,y); break;
	case OBJECT_TYPE_VENUS_FIRE_TRAP: obj = new CVenusFireTrap(x, y); break;
	case OBJECT_TYPE_PIRANHA_PLANT: obj = new CPiranhaPlant(x, y); break;
	case OBJECT_TYPE_RED_KOOPA_TROOPA: obj = new CKoopaTroopa(x, y); break;
	case OBJECT_TYPE_BRICK:
	{
		if (tokens.size() == 4)
		{
			int itemID = atoi(tokens[3].c_str());
			obj = new CBrick(x, y, itemID);
			break;
		}
		else if (tokens.size() == 5)
		{
			int itemID = atoi(tokens[3].c_str());
			int bounceCount = atoi(tokens[4].c_str());
			obj = new CBrick(x, y, itemID, bounceCount);
			break;
		}
		else obj = new CBrick(x, y);
		break;
	}
	case OBJECT_TYPE_STRIPED_BRICK: obj = new CStripedBrick(x, y); break;
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;
	case OBJECT_TYPE_SUPER_MUSHROOM: obj = new CSuperMushroom(x, y); break;
	case OBJECT_TYPE_SUPER_LEAF: obj = new CSuperLeaf(x, y); break;
	case OBJECT_TYPE_PLATFORM:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int type = atoi(tokens[6].c_str());
		int sprite_begin = atoi(tokens[7].c_str());
		int sprite_middle = atoi(tokens[8].c_str());
		int sprite_end = atoi(tokens[9].c_str());
		int falling = atoi(tokens[10].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length, type,
			sprite_begin, sprite_middle, sprite_end,
			falling
		);

		break;
	}
	case OBJECT_TYPE_PBUTTON: obj = new CPButton(x, y); break;
	case OBJECT_TYPE_QUESTION_BLOCK: 
	{
		int itemID = atoi(tokens[3].c_str());
		obj = new CQuestionBlock(x, y, itemID);
		break;
	}
	case OBJECT_TYPE_PIPE:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int pipe_height = atoi(tokens[5].c_str());

		int sprite_id_begin_begin = atoi(tokens[6].c_str());
		int sprite_id_end_begin = atoi(tokens[7].c_str());
		int sprite_id_begin_end = atoi(tokens[8].c_str());
		int sprite_id_end_end = atoi(tokens[9].c_str());

		int face_direction = atoi(tokens[10].c_str());
		int warp_direction = atoi(tokens[11].c_str());

		int itemID = atoi(tokens[12].c_str());
		if (warp_direction != 0)
		{
			int scene_destination = atoi(tokens[13].c_str());
			float newX = (float)atof(tokens[14].c_str());
			float newY = (float)atof(tokens[15].c_str());

			obj = new CPipe(
				x, y,
				cell_width, cell_height, pipe_height,
				face_direction, warp_direction,
				sprite_id_begin_begin, sprite_id_end_begin,
				sprite_id_begin_end, sprite_id_end_end,
				itemID, scene_destination, newX, newY);
			break;
		}
		else obj = new CPipe(
			x, y,
			cell_width, cell_height, pipe_height, 
			face_direction, warp_direction, 
			sprite_id_begin_begin, sprite_id_end_begin, 
			sprite_id_begin_end, sprite_id_end_end,
			itemID);
		break;
	}

	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());

		float new_x = (float)atof(tokens[6].c_str());
		float new_y = (float)atof(tokens[7].c_str());
		
		obj = new CPortal(x, y, r, b, scene_id, new_x, new_y);
	}
	break;
	case OBJECT_TYPE_CONTAINER:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int height = atoi(tokens[6].c_str());

		int sprite_begin_begin = atoi(tokens[7].c_str());
		int sprite_middle_begin = atoi(tokens[8].c_str());
		int sprite_end_begin = atoi(tokens[9].c_str());
		int sprite_begin_middle = atoi(tokens[10].c_str());
		int sprite_middle_middle = atoi(tokens[11].c_str());
		int sprite_end_middle = atoi(tokens[12].c_str());
		int sprite_begin_end = atoi(tokens[13].c_str());
		int sprite_middle_end = atoi(tokens[14].c_str());
		int sprite_end_end = atoi(tokens[15].c_str());

		obj = new CContainer(
			x, y,
			cell_width, cell_height,
			length, height,
			sprite_begin_begin, sprite_middle_begin, sprite_end_begin,
			sprite_begin_middle, sprite_middle_middle, sprite_end_middle,
			sprite_begin_end, sprite_middle_end, sprite_end_end
		);

		break;
	}

	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	objects.push_back(obj);

	if (dynamic_cast<CHud*>(obj))
	{
		AddHudDetail(x, y);
		BeginCard();
	}
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	CCollision* collisionProcessor = CCollision::GetInstance();
	CCollisionTracker* collisionTracker = collisionProcessor->GetTracker();

	vector<LPGAMEOBJECT> nearbyObjects = FilterByPlayer();

	for (auto& obj : nearbyObjects)
	{
		obj->Prepare(dt);
	}

	vector<LPGAMEOBJECT> movingColliders;
	vector<LPGAMEOBJECT> staticColliders;

	vector<LPGAMEOBJECT> blockingColliders;
	vector<LPGAMEOBJECT> nonBlockingColliders;
	for (auto& obj : nearbyObjects)
	{	
		if (dynamic_cast<CCoin*>(obj))
			int t = 0;

		if (obj->IsCollidable())
		{
			//Collide with objects
			if (obj->IsBlocking())
				blockingColliders.push_back(obj);
			else
				nonBlockingColliders.push_back(obj);

			//Objects will use collision process
			if (obj->IsMoving()) //For sweptAABB
				movingColliders.push_back(obj);
			else if (obj->IsDirectionalBlocking() == false) //For overlap
				staticColliders.push_back(obj);

			collisionTracker->Allocate(obj);
		}
	}

	if (isPipeSpawned) DebugOutTitle(L"YES\n");
	else DebugOutTitle(L"NO\n");

	//solve collision with blocking objects first
	for (auto& obj : movingColliders) //For moving objects
		collisionProcessor->SolveCollisionWithBlocking(obj, dt, &blockingColliders);

	for (auto& obj : staticColliders) //For static objects
		collisionProcessor->SolveOverlap(obj, &movingColliders);

	//solve collision with non-blocking objects
	for (auto& obj : movingColliders)
		collisionProcessor->SolveCollisionWithNonBlocking(obj, dt, &nonBlockingColliders);

	for (auto& obj : nearbyObjects)
		obj->Update(dt, &nearbyObjects);

	UpdateCamera(dt);
	PushPlayer();

	// UPDATE HUD
	if (!timerPause) timer -= dt;
	UpdateTime();
	UpdateCoin();
	UpdateScore();

	if (toggleSceneSwitch)
	{
		wait_time += dt;
		if (wait_time >= SCENE_SWITCH_WAIT_TIME)
		{
			toggleSceneSwitch = false;
			wait_time = 0;

			if (next_level_scene != id)	CGame::GetInstance()->InitiateSwitchScene(next_level_scene);
			
			if (next_level_scene == 3)
			{
				float cx, cy;
				CGame::GetInstance()->GetCamPos(cx, cy);
				CGame::GetInstance()->SetCamPos(0, cy);
			}

			timer = TIMER_VALUE;
			timerPause = false;
		}
	}

	KeyStateManager* keyState = CGame::GetInstance()->GetKeyboard();
	float vx, vy;
	if (keyState->IsHold(VK_A))
	{
		player->GetSpeed(vx, vy);
		if (abs(vx) >= MARIO_SMALL_RUNNING_MAX_VX / 2)
		{
			UpdateRunTime(dt, true);
		}
		else UpdateRunTime(dt, false);
	}
	else UpdateRunTime(dt, false);

	UpdatePMeter();

	float px, py;
	player->GetPosition(px, py);

	PurgeDeletedObjects();
}

void CPlayScene::PushPlayer()
{
	float pX, pY;
	player->GetPosition(pX, pY);
	float cX, cY;
	CGame::GetInstance()->GetCamPos(cX, cY);
	if (pX < cX + MARIO_LEFT_BOUNDARY)
		player->SetPosition(cX + MARIO_LEFT_BOUNDARY, pY);
}

void CPlayScene::AddHudDetail(float x, float y)
{
	for (int i = 0; i < DIGIT_COUNT_SCORE; i++)
	{
		scoreDigits[i] = new CDigit(x + SCORE_OFFSET + i * DIGIT_NEAR_SPACING, y + OFFSET_Y_LINE2, false, 0);
		objects.push_back(scoreDigits[i]);
	}
	for (int i = 0; i < DIGIT_COUNT_CURRENCY; i++)
	{
		coinDigits[i] = new CDigit(x + CURRENCY_OFFSET + i * DIGIT_NEAR_SPACING, y + OFFSET_Y_LINE1, false, 0);
		objects.push_back(coinDigits[i]);
	}
	for (int i = 0; i < DIGIT_COUNT_TIME; i++)
	{
		timeDigits[i] = new CDigit(x + TIME_OFFSET + i * DIGIT_NEAR_SPACING, y + OFFSET_Y_LINE2, false, 0);
		objects.push_back(timeDigits[i]);
	}
	for (int i = 0; i < P_METER_COUNT; i++)
	{
		int pType = 0;
		int pOffset = 0;
		if (i == P_METER_COUNT - 1)
		{
			pType = 1;
			pOffset = P_SWITCH_WIDTH / 4;
		}
		pMeter[i] = new CPMeter(x + pOffset + P_METER_OFFSET + i * DIGIT_NEAR_SPACING, y + OFFSET_Y_LINE1, pType, false);
		objects.push_back(pMeter[i]);
	}
	for (int i = 0; i < HUD_CARD_COUNT; i++)
	{
		float xOffset = 7;
		float yOffset = 4;
		cards[i] = new CHUDCard(x + CARD_OFFSET + xOffset + i * CARD_NEAR_SPACING, y + OFFSET_Y_LINE1 + yOffset, 0);
		objects.push_back(cards[i]);
	}
}

void CPlayScene::UpdateTime()
{
	int currentTime = (int)(timer / 1000);
	string str_currentTime = to_string(currentTime);

	for (int i = 0; i < DIGIT_COUNT_TIME - str_currentTime.size(); i++)
		str_currentTime = "0" + str_currentTime;

	for (int i = 0; i < DIGIT_COUNT_TIME; i++)
	{
		string digit_str;
		digit_str = digit_str + str_currentTime[i];
		int digit = atoi(digit_str.c_str());
		timeDigits[i]->SetDigit(digit);
	}
}

void CPlayScene::CollectCoin() { coin++; }

void CPlayScene::UpdateCoin()
{
	string str_coin = to_string(coin);
	int str_length = str_coin.size();
	
	if (str_length == 1)
	{
		coinDigits[0]->SetEmpty(true);
		coinDigits[1]->SetDigit(coin);
	}
	else
	{
		string digit_0_str, digit_1_str;

		digit_0_str = digit_0_str + str_coin[str_length - 2];
		digit_1_str = digit_1_str + str_coin[str_length - 1];

		int digit_0 = atoi(digit_0_str.c_str());
		int digit_1 = atoi(digit_1_str.c_str());

		if (digit_0 == 0)
		{
			coinDigits[0]->SetEmpty(true);
		}
		else {
			coinDigits[0]->SetEmpty(false);
			coinDigits[0]->SetDigit(digit_0);
		}
		coinDigits[1]->SetDigit(digit_1);
	}
}

void CPlayScene::UpdateScore()
{
	string str_score = to_string(score);
	int str_length = str_score.size();
	
	for (int i = 0; i < DIGIT_COUNT_SCORE - str_length; i++)
		str_score = "0" + str_score;

	str_length = str_score.size();
	for (int i = 0; i < DIGIT_COUNT_SCORE; i++)
	{
		string digit_str;
		digit_str = digit_str + str_score[str_length - (DIGIT_COUNT_SCORE - i)];
		int digit = atoi(digit_str.c_str());
		scoreDigits[i]->SetDigit(digit);
	}
}

void CPlayScene::UpdatePMeter()
{
	for (int i = 0; i < p_progress; i++)
		pMeter[i]->SetToggle(true);
	for (int i = p_progress; i < P_METER_COUNT; i++)
		pMeter[i]->SetToggle(false);
}

void CPlayScene::UpdateRunTime(DWORD dt, bool isProgress)
{
	if (isProgress)
	{
		if ((int)(p_run_time / P_PROGRESS_DELAY) < P_METER_COUNT) p_run_time += dt;
	}
	else
	{
		if (p_run_time > 0)	p_run_time -= dt / 2;
		else if (p_run_time < 0) p_run_time = 0;
	}
	p_progress = p_run_time / P_PROGRESS_DELAY;
}

void CPlayScene::BeginCard()
{
	for (int i = 0; i < HUD_CARD_COUNT; i++)
	{
		cards[i]->SetType(cardID[i]);
	}
}

void CPlayScene::InsertCard(int type)
{
	for (int i = 0; i < HUD_CARD_COUNT; i++)
	{
		if (cardID[i] != 0) continue;
		else
		{
			cardID[i] = type;
			cards[i]->SetType(type);
			break;
		}
	}
}

void CPlayScene::CollectingScore()
{
	timerPause = true;
	if (timer > 0)
	{
		if (timer >= 10000)
		{
			score = score + 10 * SCORE_PER_SECOND;
			timer -= 10000;
		}
		else if (timer >= 1000)
		{
			score = score + SCORE_PER_SECOND;
			timer -= 1000;
		}
	}
}

void CPlayScene::ToggleSceneSwitch()
{
	toggleSceneSwitch = true;
	timerPause = true;
}

void CPlayScene::Render()
{
	//if (background) background->Render();

	vector<LPGAMEOBJECT> process_list = FilterByCam();

	for (int i = 0; i < process_list.size(); i++)
	{
		process_list[i]->Render();
	}
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;
	background = NULL;
	hud = NULL;
	for (int i = 0; i < DIGIT_COUNT_SCORE; i++)
		scoreDigits[i] = NULL;
	for (int i = 0; i < DIGIT_COUNT_TIME; i++)
		timeDigits[i] = NULL;
	for (int i = 0; i < DIGIT_COUNT_CURRENCY; i++)
		coinDigits[i] = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::Insert(LPGAMEOBJECT newObj, int index)
{
	if (!newObj)
		return;
	else if (index < 0)
		objects.push_back(newObj);
	else
		objects.insert(objects.begin() + index, newObj);
}

int CPlayScene::Find(LPGAMEOBJECT obj)
{
	if (obj)
	{
		for (int i = 0; i < objects.size(); i++)
			if (objects.at(i) == obj)
				return i;
	}
	return -1;
}

void CPlayScene::LoadWarpedMario(float newX, float newY, float newLife, float newDirection)
{
	newMarioX = newX;
	newMarioY = newY;
	newMarioLife = newLife;
	newMarioWarpDirection = newDirection;
}

void CPlayScene::SaveMarioLife()
{
	newMarioLife = player->GetLife();
}

void CPlayScene::TogglePipeSwitch(bool pipeSwitch)
{
	isPipeSpawned = pipeSwitch;
}

vector<LPGAMEOBJECT> CPlayScene::GetBrickObjects()
{
	vector<LPGAMEOBJECT> listOfBricks;
	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<CBrick*>(objects[i])) listOfBricks.push_back(objects[i]);
	}
	return listOfBricks;
}

bool CPlayScene::IsInRange(LPGAMEOBJECT obj, float start_x, float end_x, float start_y, float end_y)
{
	float left, top, right, bottom;
	obj->GetBoundingBox(left, top, right, bottom);

	bool horizontally_inside = (left <= end_x) && (right >= start_x);
	bool vertically_inside = (top <= end_y) && (bottom >= start_y);

	return horizontally_inside && vertically_inside;

	//return !(left > max_x || right < min_x || top > max_y || bottom < min_y);
}

vector<LPGAMEOBJECT> CPlayScene::FilterByPlayer(float range)
{
	float player_x, player_y;
	player->GetPosition(player_x, player_y);

	float range_width = range;
	float range_height = range;

	float start_x = player_x - range_width, end_x = player_x + range_width;
	float start_y = player_y - range_height, end_y = player_y + range_height;

	vector<LPGAMEOBJECT> process_list;
	for (size_t i = 0; i < objects.size(); i++)
	{
		/*if (dynamic_cast<CHud*>(objects[i])) process_list.push_back(objects[i]);
		else */if (IsInRange(objects[i], start_x, end_x, start_y, end_y))
			process_list.push_back(objects[i]);
	}

	return process_list;
}

vector<LPGAMEOBJECT> CPlayScene::FilterByCam()
{
	CGame* game = CGame::GetInstance();
	
	float start_x, end_x, start_y, end_y;
	game->GetCamPos(start_x, start_y);
	end_x = start_x + game->GetBackBufferWidth();
	end_y = start_y + game->GetBackBufferHeight();

	/*end_x = start_x + 256.0f;
	end_y = start_y + 192.0f;*/

	vector<LPGAMEOBJECT> process_list;
	for (size_t i = 0; i < objects.size(); i++)
	{
		/*if (dynamic_cast<CHud*>(objects[i])) process_list.push_back(objects[i]);
		else */if (IsInRange(objects[i], start_x, end_x, start_y, end_y))
			process_list.push_back(objects[i]);
	}

	return process_list;
}

void CPlayScene::UpdateCamera(DWORD dt)
{
	// Update camera to follow mario
	CGame* game = CGame::GetInstance();
	LPPLAYSCENE curr = (LPPLAYSCENE)game->GetCurrentScene();
	int sceneID = curr->GetCurrentSceneID();

	float cx, cy;
	game->GetCamPos(cx, cy);
	if (sceneID != 3)
	{
		player->GetPosition(cx, cy);

		float player_bbox_height = player->GetBBoxHeight();

		cx = cx - game->GetBackBufferWidth() / 2.0f;
		cx = fmax(0.0f, cx);

		if (player->IsFlying())
		{
			cy = cy - game->GetBackBufferHeight() / 2.0f;
			cy = fmin(CAM_MAX_Y, cy);
		}
		else
		{
			cy = CAM_MAX_Y;
			//cy = cy + player_bbox_height / 2.0f + 16.0f - game->GetBackBufferHeight();
		}

		/*if (GetAsyncKeyState(VK_UP) & 0x8000) cy -= 10;
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) cy += 10;*/
	}
	else
	{
		if (cx >= MAX_CAMERA_POSITION)
		{
			cx = MAX_CAMERA_POSITION;
		}
		else
		{
			cx += dt * CAM_SPEED;
		}
		cy = CAM_MAX_Y;
	}

	// UPDATE HUD
	float ox, oy; //for hud
	hud->GetOriginalPos(ox, oy);
	hud->SetPosition((int)(ox + cx), (int)(oy + cy));

	float odx, ody; //for digits & cards
	for (int i = 0; i < DIGIT_COUNT_SCORE; i++)
	{
		scoreDigits[i]->GetOriginalPos(odx, ody);
		scoreDigits[i]->SetPosition((int)(odx + cx - ox), (int)(ody + cy - oy));
	}
	for (int i = 0; i < DIGIT_COUNT_TIME; i++)
	{
		timeDigits[i]->GetOriginalPos(odx, ody);
		timeDigits[i]->SetPosition((int)(odx + cx - ox), (int)(ody + cy - oy));
	}
	for (int i = 0; i < DIGIT_COUNT_CURRENCY; i++)
	{
		coinDigits[i]->GetOriginalPos(odx, ody);
		coinDigits[i]->SetPosition((int)(odx + cx - ox), (int)(ody + cy - oy));
	}
	for (int i = 0; i < P_METER_COUNT; i++)
	{
		pMeter[i]->GetOriginalPos(odx, ody);
		pMeter[i]->SetPosition((int)(odx + cx - ox), (int)(ody + cy - oy));
	}
	for (int i = 0; i < HUD_CARD_COUNT; i++)
	{
		cards[i]->GetOriginalPos(odx, ody);
		cards[i]->SetPosition((int)(odx + cx - ox), (int)(ody + cy - oy));
	}

	CGame::GetInstance()->SetCamPos(cx, cy);
}

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}