#include "CNotice.h"

BOOL CNotice::m_bIsCollision = FALSE;

WORD CNotice::m_NoticeCount = 0;

CNotice::CNotice()
{

}
CNotice::~CNotice()
{

}
VOID CNotice::MakeNotice(D3DXVECTOR3 position)
{
	m_Position = position;
	for (int i = 0; i < 4; i++)
	{
		m_Vertex[i].v3VerNormal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}
	m_Vertex[0].v2VerTex = D3DXVECTOR2(0.0f, 0.0f);
	m_Vertex[1].v2VerTex = D3DXVECTOR2(1.0f, 0.0f);
	m_Vertex[2].v2VerTex = D3DXVECTOR2(1.0f, 1.0f);
	m_Vertex[3].v2VerTex = D3DXVECTOR2(0.0f, 1.0f);

	m_Vertex[0].v3VerPos = D3DXVECTOR3(m_Position.x - LENGTH_OF_TILE / 2, 0.01f, m_Position.z + LENGTH_OF_TILE / 2);
	m_Vertex[1].v3VerPos = D3DXVECTOR3(m_Position.x + LENGTH_OF_TILE / 2, 0.01f, m_Position.z + LENGTH_OF_TILE / 2);
	m_Vertex[2].v3VerPos = D3DXVECTOR3(m_Position.x + LENGTH_OF_TILE / 2, 0.01f, m_Position.z - LENGTH_OF_TILE / 2);
	m_Vertex[3].v3VerPos = D3DXVECTOR3(m_Position.x - LENGTH_OF_TILE / 2, 0.01f, m_Position.z - LENGTH_OF_TILE / 2);
}
VOID CNotice::MakeNoticeVB(LPDIRECT3DDEVICE9 device)
{
	device->CreateVertexBuffer(sizeof(CUSTOMVERTEX) * 4, 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pNoticeVB, NULL);
	VOID** NoticeVertices;
	m_pNoticeVB->Lock(0, sizeof(CUSTOMVERTEX) * 4, (void**)&NoticeVertices, 0);
	memcpy(NoticeVertices, m_Vertex, sizeof(CUSTOMVERTEX) * 4);
	m_pNoticeVB->Unlock();
}
VOID CNotice::DrawNotice(LPDIRECT3DDEVICE9 device)
{
	device->SetStreamSource(0, m_pNoticeVB, 0, sizeof(CUSTOMVERTEX));
	device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
}
VOID CNotice::ReleaseNoticeVB()
{
	if (m_pNoticeVB != NULL)
		m_pNoticeVB->Release();
}
BOOL CNotice::IsPossibleInteraction(D3DXVECTOR3 playerPosition)
{
	// 충돌을 검사할 블록의 왼쪽아래(minX, minZ)와 오른쪽위(maxX,maxZ) 두 점
	D3DXVECTOR2 NoticePoint[2];
	NoticePoint[0].x = m_Position.x - LENGTH_OF_TILE / 2;
	NoticePoint[0].y = m_Position.z - LENGTH_OF_TILE / 2;
	NoticePoint[1].x = m_Position.x + LENGTH_OF_TILE / 2;
	NoticePoint[1].y = m_Position.z + LENGTH_OF_TILE / 2;

	//충돌 시
	if (NoticePoint[0].x <= playerPosition.x + PLAYER_RADIUS && NoticePoint[1].x >= playerPosition.x - PLAYER_RADIUS
		&& NoticePoint[0].y <= playerPosition.z + PLAYER_RADIUS && NoticePoint[1].y >= playerPosition.z - PLAYER_RADIUS)
	{
		OutputDebugStringA("Notice Collision!!!\n");
		m_bIsCollision = TRUE;

		return TRUE;
	}
	// 충돌을 벗어나면 다시 원래 시점으로 복구
	else
	{
		m_bIsCollision = FALSE;

		return FALSE;
	}
}
