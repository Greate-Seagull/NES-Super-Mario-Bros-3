#include "SpawnManager.h"
#include "Game.h"
#include "PlayScene.h"

void CSpawnManager::Add(CGameObject* obj)
{
	objectPositions[obj] = make_pair(obj->GetX(), obj->GetY());
}

void CSpawnManager::Spawn()
{
	CGame* game = CGame::GetInstance();

	pair<float, float> position;
	for (auto it : objectPositions)
	{
		if (game->IsInCam(it.first))
			continue;		

		position = it.second;
		if (it.first->GetX() == position.first && it.first->GetY() == position.second)
			continue;

		it.first->SetPosition(position.first, position.second);

		if (game->IsInCam(it.first))
			it.first->SetPosition(DEFAULT_X, DEFAULT_Y); //Don't appear immediately
		else
			it.first->Refresh();
	}
}

void CSpawnManager::Delete(CGameObject* obj)
{
	objectPositions.erase(obj);
}
