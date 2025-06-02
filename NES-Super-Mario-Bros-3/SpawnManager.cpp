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

	for (const auto& it : objectPositions)
	{
		CGameObject* obj = it.first;
		const auto& position = it.second;		

		if (game->IsInCam(obj))
			continue;

		if (obj->GetX() == position.first && obj->GetY() == position.second)
			continue;

		obj->SetPosition(position.first, position.second);


		if (game->IsInCam(obj))
			obj->SetPosition(DEFAULT_X, DEFAULT_Y); //Don't appear immediately
		else
			obj->Refresh();
	}
}

void CSpawnManager::Delete(CGameObject* obj)
{
	objectPositions.erase(obj);
}

void CSpawnManager::Clear()
{
	objectPositions.clear();
}
