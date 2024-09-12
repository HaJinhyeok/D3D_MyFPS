#include "FPS_function.h"

// �� ������ inside, intersection, outside �� � �������� �Ǻ�
// viewprojection�� ���� ����ȯ �Ϸ�� 6���� ���, �� ��ü�� ����, ��ü�� ������ ���� �ʿ�
POSITION_WITH_FRUSTUM CheckFrustumCulling(D3DXPLANE* FrustumPlane, D3DXVECTOR3 position, FLOAT radius)
{
    // 6�� �鿡 ���� �ϳ��� outside�̸�( distance <= -radius ) outside
    // 6�� �鿡 ���� �ϳ��� intersection�̸�( -radius < distance < radius ) intersection
    // �������� inside
    // ��¥�� intersection�� render�ؾ� �ϹǷ�, outside���� �ƴ����� �˻��ϸ� �ȴ�
    // ���� ��� ������ �Ÿ� ����
    // (distance) = abs( ax_1 + by_1 + cz_1 + d ) / sqrt( a^2 + b^2 + c^2 )
    FLOAT distance;
    for (int i = 0; i < 6; i++)
    {
        distance = 0.0f;
        distance +=
            FrustumPlane[i].a * position.x + FrustumPlane[i].b * position.y +
            FrustumPlane[i].c * position.z + FrustumPlane[i].d;
        distance /= sqrtf(powf(FrustumPlane[i].a, 2.0f) + powf(FrustumPlane[i].b, 2.0f) + powf(FrustumPlane[i].c, 2.0f));
        if (distance < -radius) return POSITION_WITH_FRUSTUM::outside;
        else if (distance >= -radius && distance <= radius) return POSITION_WITH_FRUSTUM::intersection;
    }
    return POSITION_WITH_FRUSTUM::inside;
}

// �� ��° ���� ������� �־�����, (�� �ܰ��� ������) �� ���θ� �����ϴ� ������ vertex ��ǥ�� �����ϰ� �� list�� return�Ѵ�
// �ϴ� ���� �ϳ���...
// �� ���� ������ ' '�� '*'�� �̷���� ���ڿ��� ǥ���ϰ�, �� ���ڿ��� ������ ���� vertex�� ������ �� �ְ� ����� ����Ʈ�ϵ�...
// ������ ���� vertex ������ ��������, �ƴϸ� �� ĭ �� ĭ�� ������·� ������ ������
CUSTOMVERTEX* MakeLabyrinth(int nMapNumber)
{
    if (nMapNumber == 1)
    {
        CUSTOMVERTEX* Labyrinth = new CUSTOMVERTEX[72 * 4];

        return Labyrinth;
    }
    else
        return NULL;
}