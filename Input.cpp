#include "Input.h"

VOID InitInput()
{
	for (int i = 1; i < 255; i++)
		nKeyState[i] = KEY_UP_REP;
}

VOID UpdateInput()
{
	for (int i = 1; i < 255; i++)
	{
		// Ű�� �� ���� ���¿���
		if (nKeyState[i] == KEY_UP_REP)
		{
			// Ű�� ������
			// if ((GetAsyncKeyState(i) & 0x8000) == 0)
			if (GetAsyncKeyState(i) & 0x8000)
				nKeyState[i] = KEY_DOWN;
			// �� ������
			else
				nKeyState[i] = KEY_UP_REP;
		}
		// Ű�� ���� ���Ŀ�
		else if (nKeyState[i] == KEY_DOWN)
		{
			// Ű�� �� ������
			if (GetAsyncKeyState(i) & 0x8000)
				nKeyState[i] = KEY_DOWN_REP;
			// �ƴϸ� �� ���� ���·�
			else
				nKeyState[i] = KEY_UP;
		}
		// Ű�� ������ �߿�
		else if (nKeyState[i] == KEY_DOWN_REP)
		{
			// Ű�� ��� ������
			if (GetAsyncKeyState(i) & 0x8000)
				nKeyState[i] = KEY_DOWN_REP;
			// �ƴϸ� �� ���� ���·�
			else
				nKeyState[i] = KEY_UP;
		}
		// Ű�� �� ���� ���Ŀ�
		else if (nKeyState[i] == KEY_UP)
		{
			// Ű�� ������
			if (GetAsyncKeyState(i) & 0x8000)
				nKeyState[i] = KEY_DOWN;
			// �ƴϸ� ��� �� ���� ���·�
			else
				nKeyState[i] = KEY_UP_REP;
		}
	}
}
BOOL GetKeyDown(int keycode)
{
	if (nKeyState[keycode] == KEY_DOWN)
		return TRUE;
	else
		return FALSE;
}
BOOL GetKeyUp(int keycode)
{
	if (nKeyState[keycode] == KEY_UP)
		return TRUE;
	else
		return FALSE;
}
BOOL GetKey(int keycode)
{
	if (nKeyState[keycode] == KEY_DOWN || nKeyState[keycode] == KEY_DOWN_REP)
		return TRUE;
	else
		return FALSE;
}