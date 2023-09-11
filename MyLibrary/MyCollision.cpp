#include "MyCollision.h"

bool MyCollision::CheckCollision(const Box& box1, const Box& box2) {
	// 두 박스의 최소점과 최대점 좌표를 가져옵니다.
	Vector3 minPoint1 = box1.mFrontTopLeft;
	Vector3 maxPoint1 = box1.mBackBottomRight;
	Vector3 minPoint2 = box2.mFrontTopLeft;
	Vector3 maxPoint2 = box2.mBackBottomRight;

	// 두 박스가 충돌하는지 여부를 검사합니다.
	if (minPoint1.getX() <= maxPoint2.getX() && maxPoint1.getX() >= minPoint2.getX() &&
		minPoint1.getY() <= maxPoint2.getY() && maxPoint1.getY() >= minPoint2.getY() &&
		minPoint1.getZ() <= maxPoint2.getZ() && maxPoint1.getZ() >= minPoint2.getZ()) {
		// 두 박스가 충돌하면 true를 반환합니다.

		return true;
	}

	// 두 박스가 충돌하지 않으면 false를 반환합니다.
	return false;
}