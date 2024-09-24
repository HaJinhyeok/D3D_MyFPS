#include "CFrustum.h"

CFrustum::CFrustum()
{

}
CFrustum::~CFrustum()
{

}
VOID CFrustum::MakeFrustum(D3DXMATRIX* pMatViewProj)
{
	D3DXMATRIX pInv;
	m_vertex[0].x = -1.0f;	m_vertex[0].y = -1.0f;	m_vertex[0].z = 0.0f;
	m_vertex[1].x = 1.0f;	m_vertex[1].y = -1.0f;	m_vertex[1].z = 0.0f;
	m_vertex[2].x = 1.0f;	m_vertex[2].y = -1.0f;	m_vertex[2].z = 1.0f;
	m_vertex[3].x = -1.0f;	m_vertex[3].y = -1.0f;	m_vertex[3].z = 1.0f;
	m_vertex[4].x = -1.0f;	m_vertex[4].y = 1.0f;	m_vertex[4].z = 0.0f;
	m_vertex[5].x = 1.0f;	m_vertex[5].y = 1.0f;	m_vertex[5].z = 0.0f;
	m_vertex[6].x = 1.0f;	m_vertex[6].y = 1.0f;	m_vertex[6].z = 1.0f;
	m_vertex[7].x = -1.0f;	m_vertex[7].y = 1.0f;	m_vertex[7].z = 1.0f;

	D3DXMatrixInverse(&pInv, NULL, pMatViewProj);
}
BOOL CFrustum::bIsInFrustum(D3DXVECTOR3* position, FLOAT distance)
{

}