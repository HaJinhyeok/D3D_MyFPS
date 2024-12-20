#pragma once
#include <d3d9.h>
#include <d3dx9math.h>

#include "FPS.h"

class CPlayer
{
private:
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_LookAt;
	// D3DXMATRIX m_PlayerWorld;
	D3DXMATRIX m_PlayerAxis; // x�� axis�� LookAt vector
	BOOL m_IsLightOn;
	D3DLIGHT9 m_FlashLight;
	deque<Bullet> m_Bullet;
	DWORD m_CurrentTime;
	const FLOAT m_BulletVelocity = 0.1f; // 1�ʴ� 20��ŭ == 1ms�� 0.02f
	// const WORD m_RPM = 600;
	// �÷��̾ �߻��� �Ѿ� ��ü�� �ϳ��ϳ� �� ���� �ִ°� �ƴ϶�,
	// ��ġ���� ���� �ִٰ� �� �����Ӹ��� ��ġ�� �Ѿ� �׸� �׷��ָ� ���� ������?
	// ��ġ���͸� �迭�̵� STL ť�� ���͵� �� ���������� �ɵ�
	// + ���ư��� ���� ����
	// �Ѿ� �ӵ��� RPM(Rounds Per Minute)�� player ��ü�� ������ �־����
	// �� �����Ӹ��� ���⺤�� Ȱ���ؼ� �� �Ѿ��� ���� �̵� ��ġ�� ���ϰ�, �� ��ġ�� ���� �����ϸ� �Ѿ� ��ü ������� ������

public:
	CPlayer();
	~CPlayer();

	VOID SetPosition(D3DXVECTOR3 position)
	{
		m_Position = position;
	}
	VOID SetLookAt(D3DXVECTOR3 look)
	{
		m_LookAt = look;
	}
	/*VOID SetPlayerWorld(D3DXMATRIX WorldMatrix)
	{
		m_PlayerWorld = WorldMatrix;
	}*/
	VOID SetPlayerAxis(D3DXMATRIX AxisMatrix)
	{
		m_PlayerAxis = AxisMatrix;
	}
	VOID SetFlashlight(BOOL flash)
	{
		m_IsLightOn = flash;
	}
	D3DXVECTOR3 GetPosition()
	{
		return m_Position;
	}
	D3DXVECTOR3 GetLookAt()
	{
		return m_LookAt;
	}
	/*D3DXMATRIX GetPlayerWorld()
	{
		return m_PlayerWorld;
	}*/
	D3DXMATRIX GetPlayerAxis()
	{
		return m_PlayerAxis;
	}
	BOOL IsFlashlightOn()
	{
		return m_IsLightOn;
	}
	D3DLIGHT9* GetPlayerLight()
	{
		return &m_FlashLight;
	}

	VOID Move(MOVE_DIRECTION direction, const char (*map)[NUM_OF_COLUMN+1]);
	VOID Rotate(BOOL bIsCCW);
	VOID Rotate(BOOL bIsCCW, BOOL bIsUpDown, FLOAT angle);
	VOID Attack(LPPOINT CursorPosition);
	VOID MoveBullet();
	VOID DrawBullet(LPDIRECT3DDEVICE9 device, LPD3DXMESH sphere);
};

