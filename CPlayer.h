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
	D3DXMATRIX m_PlayerAxis; // x축 axis가 LookAt vector
	BOOL m_IsLightOn;
	D3DLIGHT9 m_FlashLight;
	deque<Bullet> m_Bullet;
	DWORD m_CurrentTime;
	const FLOAT m_BulletVelocity = 0.1f; // 1초당 20만큼 == 1ms에 0.02f
	// const WORD m_RPM = 600;
	// 플레이어가 발사한 총알 객체를 하나하나 다 갖고 있는게 아니라,
	// 위치값만 갖고 있다가 매 프레임마다 위치에 총알 그림 그려주면 되지 않을까?
	// 위치벡터만 배열이든 STL 큐나 벡터든 쭉 갖고있으면 될듯
	// + 날아가는 방향 벡터
	// 총알 속도와 RPM(Rounds Per Minute)은 player 객체가 가지고 있어야함
	// 매 프레임마다 방향벡터 활용해서 각 총알의 예상 이동 위치를 구하고, 그 위치가 벽과 접촉하면 총알 객체 사라지게 만들자

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

