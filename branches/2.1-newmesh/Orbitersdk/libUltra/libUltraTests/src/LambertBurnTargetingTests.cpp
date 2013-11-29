#include "LambertBurnTargeting.h"
#include "gtest/gtest.h"

TEST(LambertBurnTargetingTest, Test1) {
	VECTOR3 INITIAL_POS = _V(0.5, 0.6, 0.7);
	VECTOR3 FINAL_POS = _V(0, 1.0, 0.0);
	const double TRANSFER_TIME = 0.9667663;
	const double ORBIT_PERIOD = 6.314;
	const double mu = 1.0;
	VECTOR3 initialVelocity, finalVelocity;
	bool converged = SolveLambertProblem(INITIAL_POS, FINAL_POS, TRANSFER_TIME, ORBIT_PERIOD, mu, initialVelocity, finalVelocity);
	VECTOR3 INITIAL_VEL = _V(-0.36167749, 0.76973587, -0.50634848);
	VECTOR3 FINAL_VEL = _V(-0.60187442, -0.02234181, -0.84262419);
	EXPECT_TRUE(converged);
	EXPECT_NEAR(initialVelocity.x, INITIAL_VEL.x, 1e-3);
	EXPECT_NEAR(initialVelocity.y, INITIAL_VEL.y, 1e-3);
	EXPECT_NEAR(initialVelocity.z, INITIAL_VEL.z, 1e-3);
	EXPECT_NEAR(finalVelocity.x, FINAL_VEL.x, 1e-3);
	EXPECT_NEAR(finalVelocity.y, FINAL_VEL.y, 1e-3);
	EXPECT_NEAR(finalVelocity.z, FINAL_VEL.z, 1e-3);
}

TEST(LambertBurnTargetingTest, Test2) {
	VECTOR3 INITIAL_POS = _V(-6069.39667, 1845.10683, 894.38397);
	VECTOR3 FINAL_POS = _V(2767.54294, -4920.02168, -3623.44589);
	const double TRANSFER_TIME = 2000.0;
	const double ORBIT_PERIOD = 5428;
	const double mu = 398600.4;
	VECTOR3 initialVelocity, finalVelocity;
	bool converged = SolveLambertProblem(INITIAL_POS, FINAL_POS, TRANSFER_TIME, ORBIT_PERIOD, mu, initialVelocity, finalVelocity);
	VECTOR3 INITIAL_VEL = _V(-2.73191506, -5.7292738, -4.7691369);
	VECTOR3 FINAL_VEL = _V(6.8333702, 2.237934, 2.3951986);
	EXPECT_TRUE(converged);
	EXPECT_NEAR(initialVelocity.x, INITIAL_VEL.x, 1e-3);
	EXPECT_NEAR(initialVelocity.y, INITIAL_VEL.y, 1e-3);
	EXPECT_NEAR(initialVelocity.z, INITIAL_VEL.z, 1e-3);
	EXPECT_NEAR(finalVelocity.x, FINAL_VEL.x, 1e-3);
	EXPECT_NEAR(finalVelocity.y, FINAL_VEL.y, 1e-3);
	EXPECT_NEAR(finalVelocity.z, FINAL_VEL.z, 1e-3);
}
