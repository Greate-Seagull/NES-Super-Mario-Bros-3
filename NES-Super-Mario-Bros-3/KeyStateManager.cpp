#include "KeyStateManager.h"

void KeyStateManager::Update()
{
	for (int i = 0; i < KEYBOARD_STATE_SIZE; i++)
	{
		oldKeyState[i] = newKeyState[i];
		newKeyState[i] = (GetAsyncKeyState(i) & 0x80) > 0;
	}
}

bool KeyStateManager::IsHold(char key)
{
	return oldKeyState[key] == newKeyState[key] &&
			newKeyState[key];
}

bool KeyStateManager::IsPressed(char key)
{
	return oldKeyState[key] < newKeyState[key];
}
