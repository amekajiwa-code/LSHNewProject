#include "OBB.h"
#include <cmath>

Vector2 OBB::GetHeightVec(Rect rt)
{
    Vector2 heightVec;

    // 높이 벡터 계산
    heightVec.mX = rt.m_fWidth * cos(DegreeToRadian(rt.mAngle - 90)) * 0.5f;
    heightVec.mY = rt.m_fWidth * sin(DegreeToRadian(rt.mAngle - 90)) * 0.5f;

    return heightVec;
}

Vector2 OBB::GetWidthVec(Rect rt)
{
    Vector2 widthVec;

    // 너비 벡터 계산
    widthVec.mX = rt.m_fHeight * cos(DegreeToRadian(rt.mAngle)) * 0.5f;
    widthVec.mY = rt.m_fHeight * sin(DegreeToRadian(rt.mAngle)) * 0.5f;

    return widthVec;
}

bool OBB::RectToRect(Rect& rect1, Rect& rect2)
{
    // 두 OBB의 중심 간 거리 벡터 계산
    Vector2 distanceVec = rect2.m_Center - rect1.m_Center;

    // 각 OBB의 방향 벡터 계산
    Vector2 vec[4] = { GetHeightVec(rect1), GetHeightVec(rect2), GetWidthVec(rect1), GetWidthVec(rect2) };
    float sum = 0.0f;

    for (int i = 0; i < 4; ++i)
    {
        // 벡터 정규화
        vec[i].Normalize();
        float dotProduct = fabs(vec[i] | distanceVec); // 절대값을 취한 내적 계산
        sum += dotProduct;

        // 한 방향 벡터와 거리 벡터 사이의 내적을 이용하여 충돌 검사
        if (dotProduct > (rect1.m_fWidth * 0.5f + rect2.m_fWidth * 0.5f))
            return false;
    }

    // 모든 방향 벡터에 대한 내적이 합보다 크다면 충돌
    if (fabs(distanceVec | vec[0]) > sum || fabs(distanceVec | vec[1]) > sum)
        return false;

    return true;
}
