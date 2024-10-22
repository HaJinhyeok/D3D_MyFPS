#pragma once
#include"FPS.h"

class CNotice
{
	// notice�� position, player�� �浹 �������� Ȯ���ϴ� ����
private:
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_LookAt;
	D3DXMATRIX m_World;
	CUSTOMVERTEX m_Vertex[4];
	LPDIRECT3DVERTEXBUFFER9 m_pNoticeVB = NULL;

	//static BOOL m_bIsCollision;

public:
	CNotice();
	~CNotice();

	BOOL m_bIsNotice = TRUE;
	static WORD m_NoticeCount;

	VOID MakeNotice(D3DXVECTOR3 position);
	VOID MakeNoticeVB(LPDIRECT3DDEVICE9 device);
	VOID RotateNotice(D3DXVECTOR3 player_position);
	VOID DrawNotice(LPDIRECT3DDEVICE9 device);
	VOID ReleaseNoticeVB();

	BOOL IsPossibleInteraction(D3DXVECTOR3 playerPosition);
	BOOL __PtInRect(LPPOINT point);
	BOOL IsNotice()
	{
		return m_bIsNotice;
	}
	/*BOOL IsCollision()
	{
		return m_bIsCollision;
	}*/
	WORD GetNumOfNotice()
	{
		return m_NoticeCount;
	}
	D3DXMATRIX GetNoticeWorld()
	{
		return m_World;
	}
};

// billboard �������� �����߰ڴ�! �ٴڿ� ��°� ���ü� joat
// �ϴ� ���� map1���� �ʿ��� notice�� 5��, exit�� 1���̴�.
// �̷� ���� �� notice ��ü 5�� �� exit ��ü 1��(exit�� ��� �ʿ��� 1���� ���� ����) ����
// ȭ�� �� ����
// player�� ���� �Ÿ� ���� �������� �� ��ȣ�ۿ� ��ư Ȱ��ȭ