#pragma once
#include"FPS.h"

class CNotice
{
	// notice의 position, player와 충돌 상태인지 확인하는 변수
private:
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_LookAt;
	D3DXMATRIX m_World;
	CUSTOMVERTEX m_Vertex[4];
	LPDIRECT3DVERTEXBUFFER9 m_pNoticeVB = NULL;

	BOOL m_bIsNotice = TRUE;
	//static BOOL m_bIsCollision;
	static WORD m_NoticeCount;

public:
	CNotice();
	~CNotice();

	VOID MakeNotice(D3DXVECTOR3 position);
	VOID MakeNoticeVB(LPDIRECT3DDEVICE9 device);
	VOID RotateNotice(D3DXVECTOR3 player_position);
	VOID DrawNotice(LPDIRECT3DDEVICE9 device);
	VOID ReleaseNoticeVB();

	BOOL IsPossibleInteraction(D3DXVECTOR3 playerPosition);
	BOOL IsNotice()
	{
		return m_bIsNotice;
	}
	/*BOOL IsCollision()
	{
		return m_bIsCollision;
	}*/
	VOID AddNumOfNotice()
	{
		m_NoticeCount++;
	}
	WORD GetNumOfNotice()
	{
		return m_NoticeCount;
	}
	D3DXMATRIX GetNoticeWorld()
	{
		return m_World;
	}
};

// billboard 형식으로 만들어야겠다! 바닥에 까는건 가시성 joat
// 일단 현재 map1에서 필요한 notice는 5개, exit는 1개이다.
// 미로 생성 시 notice 객체 5개 및 exit 객체 1개(exit는 모든 맵에서 1개로 고정 생성) 생성
// 화면 상에 띄우기
// player가 일정 거리 내에 접근했을 때 상호작용 버튼 활성화