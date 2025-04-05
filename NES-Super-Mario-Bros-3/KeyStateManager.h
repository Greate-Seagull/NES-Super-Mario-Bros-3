#pragma once

#include <d3dx10.h>

#define KEYBOARD_STATE_SIZE 256

class KeyStateManager
{
private:
	bool newKeyState[KEYBOARD_STATE_SIZE];
	bool oldKeyState[KEYBOARD_STATE_SIZE];
public:
	KeyStateManager() {};

	void Update();

	bool IsHold(char key);
	bool IsPressed(char key);
};