#include "FPS_function.h"

// 각 도형이 inside, intersection, outside 중 어떤 상태인지 판별
// viewprojection에 대해 역변환 완료된 6개의 평면, 각 구체의 정점, 구체의 반지름 정보 필요
POSITION_WITH_FRUSTUM CheckFrustumCulling(D3DXPLANE* FrustumPlane, D3DXVECTOR3 position, FLOAT radius)
{
    // 6개 면에 대해 하나라도 outside이면( distance <= -radius ) outside
    // 6개 면에 대해 하나라도 intersection이면( -radius < distance < radius ) intersection
    // 나머지는 inside
    // 어짜피 intersection도 render해야 하므로, outside인지 아닌지만 검사하면 된다
    // 점과 평면 사이의 거리 공식
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