#pragma once
#include <d3d9.h>
#include <d3dx9math.h>

#include "main.h"

class CPlayer
{
private:
	D3DXVECTOR3 m_LookAt;
	D3DXMATRIX m_PlayerWorld; // x, y, z�� ���� + position ����
	BOOL m_IsLightOn;
	D3DLIGHT9 m_FlashLight;
	deque<Bullet> m_Bullet;
	DWORD m_CurrentMoveTime;
	DWORD m_CurrentRotateTime;
	const FLOAT m_BulletVelocity = 0.1f; 
	// const WORD m_RPM = 600;
	// �÷��̾ �߻��� �Ѿ� ��ü�� �ϳ��ϳ� �� ���� �ִ°� �ƴ϶�,
	// ��ġ���� ���� �ִٰ� �� �����Ӹ��� ��ġ�� �Ѿ� �׸� �׷��ָ� ���� ������?
	// ��ġ���͸� �迭�̵� STL ť�� ���͵� �� ���������� �ɵ�
	// + ���ư��� ���� ����
	// �Ѿ� �ӵ��� RPM(Rounds Per Minute)�� player ��ü�� ������ �־����
	// �� �����Ӹ��� ���⺤�� Ȱ���ؼ� �� �Ѿ��� ���� �̵� ��ġ�� ���ϰ�, �� ��ġ�� ���� �����ϸ� �Ѿ� ��ü ������� ������

public:
	CPlayer();

	VOID SetPosition(D3DXVECTOR3 position)
	{
		m_PlayerWorld._41 = position.x;
		m_PlayerWorld._42 = position.y;
		m_PlayerWorld._43 = position.z;
	}
	VOID SetLookAt(D3DXVECTOR3 look)
	{
		m_LookAt = look;
	}
	VOID SetPlayerWorld(D3DXMATRIX WorldMatrix)
	{
		m_PlayerWorld = WorldMatrix;
	}
	VOID SetFlashlight(BOOL flash)
	{
		m_IsLightOn = flash;
	}
	D3DXVECTOR3 GetPosition()
	{
		return D3DXVECTOR3(m_PlayerWorld._41, m_PlayerWorld._42, m_PlayerWorld._43);
	}
	D3DXVECTOR3 GetLookAt()
	{
		return m_LookAt;
	}
	D3DXMATRIX GetPlayerWorld()
	{
		return m_PlayerWorld;
	}
	BOOL IsFlashlightOn()
	{
		return m_IsLightOn;
	}
	D3DLIGHT9* GetPlayerLight()
	{
		return &m_FlashLight;
	}

	BOOL Move(MOVE_DIRECTION direction, const char (*map)[NUM_OF_COLUMN+1], BOOL NoClip);
	VOID Rotate(BOOL bIsCCW);
	VOID Rotate(BOOL bIsCCW, BOOL bIsUpDown, FLOAT angle);
	VOID Attack(LPPOINT CursorPosition);
	VOID MoveBullet();
	VOID DrawBullet(LPDIRECT3DDEVICE9 device, LPD3DXMESH sphere);
};

