#include "CPlayer.h"
CPlayer::CPlayer()
{
	D3DXMATRIX tmpMatrix;
	m_Position = D3DXVECTOR3(55.0f, 5.0f, -65.0f); // v3EyeDefault
	m_LookAt = D3DXVECTOR3(55.0f, 5.0f, -60.0f);
	D3DXMatrixIdentity(&m_PlayerWorld);
	D3DXMatrixTranslation(&tmpMatrix, m_Position.x, m_Position.y, m_Position.z);
	D3DXMatrixMultiply(&m_PlayerWorld, &m_PlayerWorld, &tmpMatrix);
	D3DXMatrixIdentity(&m_PlayerAxis);
	m_Flashlight = TRUE;
}
CPlayer::~CPlayer()
{

}