#include "KeyStateManager.h"

KeyStateManager::KeyStateManager()
{
	int keys[] = {VK_A, VK_S, VK_LEFT, VK_RIGHT, VK_DOWN, VK_UP};
	for (int key: keys)
	{
		oldKeyState[key] = false;
		newKeyState[key] = false;
	}
}

void KeyStateManager::Update()
{
	for (auto& key: isUpdated)
	{
		key.second = false;
	}
}

void KeyStateManager::Refresh(int key)
{
	if (isUpdated[key] == false)
	{
		oldKeyState[key] = newKeyState[key];
		newKeyState[key] = GetAsyncKeyState(key) & 0x8000;
		isUpdated[key] = true;
	}
}

bool KeyStateManager::IsHold(int key)
{
	Refresh(key);
	return newKeyState[key];
}

bool KeyStateManager::IsPressed(int key)
{
	Refresh(key);
	return oldKeyState[key] < newKeyState[key];
}

bool KeyStateManager::IsReleased(int key)
{
	Refresh(key);
	return oldKeyState[key] > newKeyState[key];
}