#pragma once

#include <d3dx10.h>

#define KEYBOARD_STATE_SIZE 256

#define VK_S 0x53
#define VK_A 0x41

class KeyStateManager
{
private:
	bool newKeyState[KEYBOARD_STATE_SIZE];
	bool oldKeyState[KEYBOARD_STATE_SIZE];
public:
	KeyStateManager();

	void Update();

	bool IsHold(int key);
	bool IsPressed(int key);
	bool IsReleased(int key);
};