#include "UltraMath.h"
#include "gtest/gtest.h"

TEST(SignedAngleTest, PositiveAngle) {
	const VECTOR3 v1 = _V(0, 0.965925826289, 0.258819045103);
	const VECTOR3 v2 = _V(0, 0.342020143326, 0.939692620786);
	const VECTOR3 ref = _V(1, 0, 0);
	ASSERT_NEAR(SignedAngle(v1, v2, ref), 55.0*RAD, 0.1*RAD);
}
TEST(SignedAngleTest, NegativeAngle) {
	const VECTOR3 v1 = _V(0, 0.965925826289, 0.258819045103);
	const VECTOR3 v2 = _V(0, 0.5, -0.866025403784);
	const VECTOR3 ref = _V(1, 0, 0);
	ASSERT_NEAR(SignedAngle(v1, v2, ref), -75.0*RAD, 0.1*RAD);
}
TEST(SignedAngleTest, ZeroAngle) {
	const VECTOR3 v1 = _V(0.342192955415, -0.171096477707, 0.92392097962);
	const VECTOR3 v2 = _V(0.342192955415, -0.171096477707, 0.92392097962);
	const VECTOR3 ref = _V(0., 0.92392097962, 0.171096477707);
	ASSERT_NEAR(SignedAngle(v1, v2, ref), 0.0*RAD, 0.01*RAD);
}
TEST(SignedAngleTest, OutOfPlaneReference) {
	const VECTOR3 v1 = _V(0, 0.965925826289, 0.258819045103);
	const VECTOR3 v2 = _V(0, 0.342020143326, 0.939692620786);
	const VECTOR3 ref = _V(0.707106781188, 0.707106781188, 0);
	ASSERT_NEAR(SignedAngle(v1, v2, ref), 55.0*RAD, 0.1*RAD);
}
