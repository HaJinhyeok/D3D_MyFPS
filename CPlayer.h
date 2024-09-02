#pragma once
#include <d3d9.h>
#include <d3dx9math.h>

class CPlayer
{
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
	BOOL GetFlashlight()
	{
		return m_Flashlight;
	}

private:
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_LookAt;
	BOOL m_Flashlight;
};

