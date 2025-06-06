#include "Enemy.h"
#include "Game.h"
#include "PlayScene.h"

CEnemy::CEnemy(float x, float y)
	:CCreature(x, y)
{
}

void CEnemy::LookForMario()
{
	//Get target
	LPPLAYSCENE playScene = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	CMario* mario = (CMario*)playScene->GetPlayer();

	nx = (mario->GetX() <= x) ? DIRECTION_LEFT : DIRECTION_RIGHT;
	ny = (mario->GetY() <= y) ? DIRECTION_UP : DIRECTION_DOWN;
}

void CEnemy::Refresh()
{	
	CCreature::Refresh();
	LookForMario();
}
