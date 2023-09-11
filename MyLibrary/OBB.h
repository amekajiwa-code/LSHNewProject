#pragma once
#include "GeometryUtils.h"
#define T_PI (3.141592f)
#define T_EPSILON ( 0.0001f)
#define RadianToDegree(radian) ( radian *( 180.0f / T_PI))
#define DegreeToRadian(degree) ( degree *( T_PI / 180.0f))

class OBB
{
public:
    Vector2 GetHeightVec(Rect rt);
    Vector2 GetWidthVec(Rect rt);
    bool RectToPoint(Vector2& point, Rect& rect);
    bool RectToRect(Rect& rect1, Rect& rect2);
};

