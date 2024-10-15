#pragma once
#include"FPS.h"

class CNotice
{
	// notice�� position, player�� �浹 �������� Ȯ���ϴ� ����
private:
	D3DXVECTOR3 m_Position;
	CUSTOMVERTEX m_Vertex[4];
	LPDIRECT3DVERTEXBUFFER9 m_pNoticeVB = NULL;

	BOOL m_bIsNotice = TRUE;
	static BOOL m_bIsCollision;
	static WORD m_NoticeCount;

public:
	CNotice();
	~CNotice();

	VOID MakeNotice(D3DXVECTOR3 position);
	VOID MakeNoticeVB(LPDIRECT3DDEVICE9 device);
	VOID DrawNotice(LPDIRECT3DDEVICE9 device);
	VOID ReleaseNoticeVB();

	BOOL IsPossibleInteraction(D3DXVECTOR3 playerPosition);
	BOOL IsNotice()
	{
		return m_bIsNotice;
	}
	BOOL IsCollision()
	{
		return m_bIsCollision;
	}
	VOID AddNumOfNotice()
	{
		m_NoticeCount++;
	}
	WORD GetNumOfNotice()
	{
		return m_NoticeCount;
	}
};

// �ϴ� ���� map1���� �ʿ��� notice�� 5��, exit�� 1���̴�.
// �̷� ���� �� notice ��ü 5�� �� exit ��ü 1��(exit�� ��� �ʿ��� 1���� ���� ����) ����
// ȭ�� �� ����
// player�� ���� �Ÿ� ���� �������� �� ��ȣ�ۿ� ��ư Ȱ��ȭ