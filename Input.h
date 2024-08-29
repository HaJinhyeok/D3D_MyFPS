#pragma once
#include <Windows.h>

#define KEY_UP_REP		0 // Ű�� ��� �ȴ����� ����
#define KEY_DOWN		1 // Ű�� ���� ���� (�����ٰ� ����)
#define KEY_DOWN_REP	2 // Ű�� ��� ������ ����
#define KEY_UP			3 // Ű�� �ȴ��� ����(���ȴٰ� ����)

static int nKeyState[255];

VOID InitInput();
VOID UpdateInput();
BOOL GetKeyDown(int keycode);
BOOL GetKeyUp(int keycode);
BOOL GetKey(int keycode);