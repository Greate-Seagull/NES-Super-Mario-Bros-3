#include "PlayScene.h"

#include "EffectsManager.h"

CEffectsManager* CEffectsManager::__instance = NULL;

CEffectsManager* CEffectsManager::GetInstance()
{
	if (__instance == NULL)
		__instance = new CEffectsManager();

	return __instance;
}

void CEffectsManager::CreateAttackEffect(float x, float y)
{
	CBoomEffect* effect = new CBoomEffect(x, y);
	LPPLAYSCENE ps = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	ps->Insert(effect, -1);
}
