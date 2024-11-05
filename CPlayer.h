#pragma once
#include <d3d9.h>
#include <d3dx9math.h>

#include "FPS.h"

class CPlayer
{
private:
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_LookAt;
	D3DXMATRIX m_PlayerWorld;
	D3DXMATRIX m_PlayerAxis; // x√‡ axis∞° LookAt vector
	BOOL m_IsLightOn;
	D3DLIGHT9 m_FlashLight;

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
	VOID SetPlayerWorld(D3DXMATRIX WorldMatrix)
	{
		m_PlayerWorld = WorldMatrix;
	}
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
	D3DXMATRIX GetPlayerWorld()
	{
		return m_PlayerWorld;
	}
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
	VOID Attack(LPPOINT CursorPosition);
};

