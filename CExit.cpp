#include "CExit.h"

CExit::CExit()
{

}
CExit::~CExit()
{

}
VOID CExit::MakeExit(D3DXVECTOR3 position)
{
	MakeNotice(position);
	// Exit�� notice count���� �ʴ´�.
	CNotice::m_NoticeCount--;
	CNotice::m_bIsNotice = FALSE;
}
VOID CExit::DrawExitButton(LPDIRECT3DDEVICE9 device)
{
	device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_ExitButtonVertices, sizeof(UI_VERTEX));
}