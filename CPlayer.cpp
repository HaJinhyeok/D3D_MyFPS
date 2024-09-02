#include "CPlayer.h"
CPlayer::CPlayer()
{
	m_Position = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
	m_LookAt = D3DXVECTOR3(0.0f, 5.0f, 1.0f);
	m_Flashlight = TRUE;
}
CPlayer::~CPlayer()
{

}