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
	// Exit는 notice count하지 않는다.
	CNotice::m_NoticeCount--;
	CNotice::m_bIsNotice = FALSE;
}