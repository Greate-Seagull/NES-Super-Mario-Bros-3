#pragma once

#include <d3dx10.h>
#include <unordered_map>

using namespace std;

#define VK_S 0x53
#define VK_A 0x41

class KeyStateManager
{
private:
	unordered_map<int, bool> newKeyState;
	unordered_map<int, bool> oldKeyState;
	unordered_map<int, bool> isUpdated;
public:
	KeyStateManager();

	void Update();
	void Refresh(int key);

	bool IsHold(int key);
	bool IsPressed(int key);
	bool IsReleased(int key);
};