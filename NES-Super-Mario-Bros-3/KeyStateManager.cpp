#include "KeyStateManager.h"

KeyStateManager::KeyStateManager()
{
	for (int i = 0; i < KEYBOARD_STATE_SIZE; i++)
	{
		oldKeyState[i] = false;
		newKeyState[i] = false;
	}
}

void KeyStateManager::Update()
{
	for (int i = 0; i < KEYBOARD_STATE_SIZE; i++)
	{
		oldKeyState[i] = newKeyState[i];
		newKeyState[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
	}
}

bool KeyStateManager::IsHold(int key)
{
	return newKeyState[key];
}

bool KeyStateManager::IsPressed(int key)
{
	return oldKeyState[key] < newKeyState[key];
}

bool KeyStateManager::IsReleased(int key)
{
	return oldKeyState[key] > newKeyState[key];
}