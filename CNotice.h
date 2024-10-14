#pragma once
#include"FPS.h"

class CNotice
{
	// notice�� position, player�� �浹 �������� Ȯ���ϴ� ����
private:
	D3DXVECTOR3 m_Position;
	CUSTOMVERTEX m_Vertex[4];

	BOOL m_bIsNotice = TRUE;
	static BOOL m_bIsCollision;

public:
	CNotice();
	CNotice(D3DXVECTOR3 position);
	~CNotice();
	BOOL IsPossibleInteraction(D3DXVECTOR3 playerPosition);
};

// �ϴ� ���� map1���� �ʿ��� notice�� 5��, exit�� 1���̴�.
// �̷� ���� �� notice ��ü 5�� �� exit ��ü 1��(exit�� ��� �ʿ��� 1���� ���� ����) ����
// ȭ�� �� ����
// player�� ���� �Ÿ� ���� �������� �� ��ȣ�ۿ� ��ư Ȱ��ȭ