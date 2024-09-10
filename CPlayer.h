#pragma once
#include <d3d9.h>
#include <d3dx9math.h>

class CPlayer
{
private:
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_LookAt;
	D3DXMATRIX m_PlayerWorld;
	D3DXMATRIX m_PlayerAxis; // x√‡ axis∞° LookAt vector
	BOOL m_Flashlight;

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
		m_Flashlight = flash;
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
	BOOL GetFlashlight()
	{
		return m_Flashlight;
	}
};

