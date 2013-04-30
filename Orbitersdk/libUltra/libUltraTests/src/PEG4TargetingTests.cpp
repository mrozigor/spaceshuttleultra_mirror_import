#include "PEG4Targeting.h"
#include "StateVectorPropagator.h"
#include "UltraMath.h"
#include "gtest/gtest.h"

TEST(PEG4TargetingTest, DeorbitNoPerturbations) {
	const double GGRAV = 6.67259e-11;   // from OrbiterAPI.h
	const double mu = GGRAV*59.73698968e23; // from Orbiter.pdf
	
	//const VECTOR3 INITIAL_POS = _V(6029335.4, 1293149.0, -2336161.0);
	//const VECTOR3 INITIAL_VEL = _V(3331.99, -3435.89, 6229.58);
	const VECTOR3 INITIAL_POS = _V(2124770.282102, -3703885.902795, 5108751.560444);
	const VECTOR3 INITIAL_VEL = _V(-6090.732704, 2275.053541, 4182.568480);
	//const VECTOR3 ROT_VECTOR = crossp(INITIAL_VEL/length(INITIAL_VEL), INITIAL_POS/length(INITIAL_POS));
	const VECTOR3 ROT_VECTOR = crossp(INITIAL_POS/length(INITIAL_POS), INITIAL_VEL/length(INITIAL_VEL));
	const double TRANSFER_ANGLE = 131.499*RAD;
	const double TARGET_HEIGHT = 6371.0e3 + 65.832*NMI2M;
	const VECTOR3 TARGET_POS = RotateVector(ROT_VECTOR, TRANSFER_ANGLE, INITIAL_POS/length(INITIAL_POS))*TARGET_HEIGHT;
	const double C1 = 14992/MPS2FPS;
	const double C2 = -0.6017;

	// HT = 65.832 nm
	double transferTime = -1.0;
	VECTOR3 reqdVel = CalculatePEG7Targets(C1, C2, TRANSFER_ANGLE, INITIAL_POS, INITIAL_VEL, TARGET_POS, mu, transferTime) + INITIAL_VEL;
	ASSERT_GT(transferTime, 0.0);

	// CalculatePEG7Targets assumes no perturbations, so use KOST to check result
	kostStateVector initialState, finalState;
	initialState.pos = ConvertBetweenLHAndRHFrames(INITIAL_POS);
	initialState.vel = ConvertBetweenLHAndRHFrames(reqdVel);
	kostElements elements;
	kostStateVector2Elements(mu, &initialState, &elements, NULL);
	int iterations = kostElements2StateVectorAtTime(mu, &elements, &finalState, transferTime, 0.01*RAD, 100, 6371.0e3, 0.0);
	VECTOR3 finalPos = ConvertBetweenLHAndRHFrames(finalState.pos);
	VECTOR3 finalVel = ConvertBetweenLHAndRHFrames(finalState.vel);
	
	//EXPECT_NEAR(length(finalPos), TARGET_HEIGHT, 1e-3);
	//EXPECT_EQ(reqdVel.x, INITIAL_VEL.x);
	//EXPECT_EQ(reqdVel.y, INITIAL_VEL.y);
	//EXPECT_EQ(reqdVel.z, INITIAL_VEL.z);
	EXPECT_NEAR(length(reqdVel-INITIAL_VEL), 0.0, 300.0); // ensure DV is reasonable
	EXPECT_NEAR(dotp(reqdVel, ROT_VECTOR), 0.0, 1e-3); // ensure target velocity is in current orbit plane
	EXPECT_NEAR(dotp(finalPos/length(finalPos), INITIAL_POS/length(INITIAL_POS)), cos(TRANSFER_ANGLE), 0.001);
	EXPECT_NEAR(length(finalPos-TARGET_POS), 0.0, 1e2);
	// check velocity constraints
	double verticalVelocity = dotp(finalVel, finalPos/length(finalPos));
	double horizVelocity = sqrt(pow(length(finalVel), 2) - verticalVelocity*verticalVelocity);
	EXPECT_NEAR(verticalVelocity, C2*horizVelocity+C1, 1e0);
	//EXPECT_NEAR(finalPos.x, TARGET_POS.x, 1e-3);
	//EXPECT_NEAR(finalPos.y, TARGET_POS.y, 1e-3);
	//EXPECT_NEAR(finalPos.z, TARGET_POS.z, 1e-3);
}

TEST(PEG4TargetingTest, OMS2NoPerturbations) {
	const double GGRAV = 6.67259e-11;   // from OrbiterAPI.h
	const double mu = GGRAV*59.73698968e23; // from Orbiter.pdf
	
	//const VECTOR3 INITIAL_POS = _V(6029335.4, 1293149.0, -2336161.0);
	//const VECTOR3 INITIAL_VEL = _V(3331.99, -3435.89, 6229.58);
	const VECTOR3 INITIAL_POS = _V(4342290.291048, 1813126.366743, 4632555.251175);
	const VECTOR3 INITIAL_VEL = _V(-2522.182070, -5661.021045, 4598.745860);
	//const VECTOR3 ROT_VECTOR = crossp(INITIAL_VEL/length(INITIAL_VEL), INITIAL_POS/length(INITIAL_POS));
	const VECTOR3 ROT_VECTOR = crossp(INITIAL_POS/length(INITIAL_POS), INITIAL_VEL/length(INITIAL_VEL));
	//const double TRANSFER_ANGLE = 131.499*RAD;
	const double TRANSFER_ANGLE = 180.0*RAD;
	//const double TARGET_HEIGHT = 6371.0e3 + 65.832*NMI2M;
	const double TARGET_HEIGHT = 6371.0e3 + 300e3;
	const VECTOR3 TARGET_POS = RotateVector(ROT_VECTOR, TRANSFER_ANGLE, INITIAL_POS/length(INITIAL_POS))*TARGET_HEIGHT;
	//const double C1 = 14992/MPS2FPS;
	const double C1 = 0.0;
	//const double C2 = -0.6017;
	const double C2 = 0.0;

	// HT = 65.832 nm
	double transferTime = -1.0;
	VECTOR3 reqdVel = CalculatePEG7Targets(C1, C2, TRANSFER_ANGLE, INITIAL_POS, INITIAL_VEL, TARGET_POS, mu, transferTime) + INITIAL_VEL;
	std::cout << "X: " << (reqdVel.x-INITIAL_VEL.x)
			  << " Y: " << (reqdVel.y-INITIAL_VEL.y)
			  << " Z: " << (reqdVel.z-INITIAL_VEL.z)
			  << std::endl;
	ASSERT_GT(transferTime, 0.0);

	// CalculatePEG7Targets assumes no perturbations, so use KOST to check result
	kostStateVector initialState, finalState;
	initialState.pos = ConvertBetweenLHAndRHFrames(INITIAL_POS);
	initialState.vel = ConvertBetweenLHAndRHFrames(reqdVel);
	kostElements elements;
	kostStateVector2Elements(mu, &initialState, &elements, NULL);
	int iterations = kostElements2StateVectorAtTime(mu, &elements, &finalState, transferTime, 0.01*RAD, 100, 6371.0e3, 0.0);
	VECTOR3 finalPos = ConvertBetweenLHAndRHFrames(finalState.pos);
	VECTOR3 finalVel = ConvertBetweenLHAndRHFrames(finalState.vel);
	
	//EXPECT_NEAR(length(finalPos), TARGET_HEIGHT, 1e-3);
	//EXPECT_EQ(reqdVel.x, INITIAL_VEL.x);
	//EXPECT_EQ(reqdVel.y, INITIAL_VEL.y);
	//EXPECT_EQ(reqdVel.z, INITIAL_VEL.z);
	EXPECT_NEAR(length(reqdVel-INITIAL_VEL), 0.0, 300.0); // ensure DV is reasonable
	EXPECT_NEAR(dotp(reqdVel, ROT_VECTOR), 0.0, 1e-3); // ensure target velocity is in current orbit plane
	EXPECT_NEAR(dotp(finalPos/length(finalPos), INITIAL_POS/length(INITIAL_POS)), cos(TRANSFER_ANGLE), 0.001);
	EXPECT_NEAR(length(finalPos-TARGET_POS), 0.0, 1e2);
	// check velocity constraints
	double verticalVelocity = dotp(finalVel, finalPos/length(finalPos));
	double horizVelocity = sqrt(pow(length(finalVel), 2) - verticalVelocity*verticalVelocity);
	EXPECT_NEAR(verticalVelocity, C2*horizVelocity+C1, 1e0);
	//EXPECT_NEAR(finalPos.x, TARGET_POS.x, 1e-3);
	//EXPECT_NEAR(finalPos.y, TARGET_POS.y, 1e-3);
	//EXPECT_NEAR(finalPos.z, TARGET_POS.z, 1e-3);
}

/**
 * Tests PEG4Targeting class with J2 perturbations.
 * PEG4 targets are similar to deorbit burn targets.
 * Unfortunately, test is very similar to algorithm used by PEG4Targeting class to handle perturbations.
 */
TEST(PEG4TargetingTest, DeorbitPerturbations) {
	const double GGRAV = 6.67259e-11;   // from OrbiterAPI.h
	const double EARTH_MASS = 59.73698968e23;
	const double EARTH_RADIUS = 6371.0e3;
	const double mu = GGRAV*EARTH_MASS; // from Orbiter.pdf
	const double J2 = 0.001083;
	
	const VECTOR3 INITIAL_POS = _V(2124770.282102, -3703885.902795, 5108751.560444);
	const VECTOR3 INITIAL_VEL = _V(-6090.732704, 2275.053541, 4182.568480);
	const double TRANSFER_ANGLE = 131.499*RAD;
	//const double TARGET_HEIGHT = EARTH_RADIUS + 65.832*NMI2M;
	const double TARGET_HEIGHT = 65.832*NMI2M;
	const double C1 = 14992/MPS2FPS;
	const double C2 = -0.6017;
	const double ACCELERATION = 0.3; // shuttle OMS (both) acceleration should be about 0.5 m/s^2

	// HT = 65.832 nm
	//double transferTime = -1.0;
	PEG4Targeting targeting;
	targeting.SetPlanetParameters(EARTH_MASS, EARTH_RADIUS, J2);
	targeting.SetPEG4Targets(C1, C2, TARGET_HEIGHT, TRANSFER_ANGLE, INITIAL_POS, INITIAL_VEL, ACCELERATION);
	while(true) {
		if(targeting.Step()) break;
	}
	ASSERT_TRUE(targeting.Converged());

	//VECTOR3 reqdVel = targeting.GetDeltaV() + INITIAL_VEL;

	const double PROPAGATOR_STEP = 1e-2;
	const double PROPAGATOR_ITERATIONS = 100;
	StateVectorPropagator propagator(PROPAGATOR_STEP, PROPAGATOR_ITERATIONS, 3600.0);
	OMSBurnPropagator omsProp;
	omsProp.SetBurnData(0.0, targeting.GetDeltaV(), ACCELERATION);
	propagator.SetParameters(1.0, EARTH_MASS, EARTH_RADIUS, J2);
	propagator.DefinePerturbations(&omsProp);
	propagator.UpdateStateVector(INITIAL_POS, INITIAL_VEL, 0.0, true);
	int propagatorSteps = static_cast<int>(targeting.GetTransferTime()/(PROPAGATOR_STEP*PROPAGATOR_ITERATIONS));
	for(int i=0;i<propagatorSteps;i++) propagator.Step(0.00, 0.1);
	VECTOR3 finalPos, finalVel;
	propagator.GetStateVectors(targeting.GetTransferTime(), finalPos, finalVel);
	
	const VECTOR3 ROT_VECTOR = crossp(INITIAL_POS/length(INITIAL_POS), INITIAL_VEL/length(INITIAL_VEL));
	//const VECTOR3 TARGET_POS = RotateVector(ROT_VECTOR, TRANSFER_ANGLE, INITIAL_POS/length(INITIAL_POS))*(TARGET_HEIGHT+EARTH_RADIUS);

	EXPECT_NEAR(length(targeting.GetDeltaV()), 0.0, 300.0); // ensure DV is reasonable
	EXPECT_NEAR(dotp(targeting.GetDeltaV()/length(targeting.GetDeltaV()), ROT_VECTOR), 0.0, 1e-2); // ensure target velocity is in current orbit plane (limit: 0.6 degrees)
	EXPECT_NEAR(dotp(finalPos/length(finalPos), INITIAL_POS/length(INITIAL_POS)), cos(TRANSFER_ANGLE), 0.002); // check transfer angle (limit: 0.1 degrees)
	EXPECT_NEAR(length(finalPos), TARGET_HEIGHT+EARTH_RADIUS, 1e2); // check height at target point
	//EXPECT_NEAR(length(finalPos-TARGET_POS), 0.0, 1e2);
	// check velocity constraints
	double verticalVelocity = dotp(finalVel, finalPos/length(finalPos));
	double horizVelocity = sqrt(pow(length(finalVel), 2) - verticalVelocity*verticalVelocity);
	EXPECT_NEAR(verticalVelocity, C2*horizVelocity+C1, 1e0);
	//EXPECT_NEAR(finalPos.x, TARGET_POS.x, 1e-3);
	//EXPECT_NEAR(finalPos.y, TARGET_POS.y, 1e-3);
	//EXPECT_NEAR(finalPos.z, TARGET_POS.z, 1e-3);
}

/**
 * Similar to DeorbitPerturbations test; PEG4 targets similar to OMS-2 targets.
 */
TEST(PEG4TargetingTest, OMS2Perturbations) {
	const double GGRAV = 6.67259e-11;   // from OrbiterAPI.h
	const double EARTH_MASS = 59.73698968e23;
	const double EARTH_RADIUS = 6371.0e3;
	const double mu = GGRAV*EARTH_MASS; // from Orbiter.pdf
	const double J2 = 0.001083;
	
	const VECTOR3 INITIAL_POS = _V(4342290.291048, 1813126.366743, 4632555.251175);
	const VECTOR3 INITIAL_VEL = _V(-2522.182070, -5661.021045, 4598.745860);
	const double TRANSFER_ANGLE = 181.0*RAD;
	const double TARGET_HEIGHT = 300e3;
	const double C1 = 0.0;
	const double C2 = 0.0;
	const double ACCELERATION = 0.4; // shuttle OMS (both) acceleration should be about 0.5 m/s^2

	// HT = 65.832 nm
	//double transferTime = -1.0;
	PEG4Targeting targeting;
	targeting.SetPlanetParameters(EARTH_MASS, EARTH_RADIUS, J2);
	targeting.SetPEG4Targets(C1, C2, TARGET_HEIGHT, TRANSFER_ANGLE, INITIAL_POS, INITIAL_VEL, ACCELERATION);
	while(true) {
		if(targeting.Step()) break;
	}
	ASSERT_TRUE(targeting.Converged());

	//VECTOR3 reqdVel = targeting.GetDeltaV() + INITIAL_VEL;

	const double PROPAGATOR_STEP = 1e-2;
	const double PROPAGATOR_ITERATIONS = 100;
	StateVectorPropagator propagator(PROPAGATOR_STEP, PROPAGATOR_ITERATIONS, 3600.0);
	OMSBurnPropagator omsProp;
	omsProp.SetBurnData(0.0, targeting.GetDeltaV(), ACCELERATION);
	propagator.SetParameters(1.0, EARTH_MASS, EARTH_RADIUS, J2);
	propagator.DefinePerturbations(&omsProp);
	propagator.UpdateStateVector(INITIAL_POS, INITIAL_VEL, 0.0, true);
	int propagatorSteps = static_cast<int>(targeting.GetTransferTime()/(PROPAGATOR_STEP*PROPAGATOR_ITERATIONS));
	for(int i=0;i<propagatorSteps;i++) propagator.Step(0.00, 0.1);
	VECTOR3 finalPos, finalVel;
	propagator.GetStateVectors(targeting.GetTransferTime(), finalPos, finalVel);
	
	const VECTOR3 ROT_VECTOR = crossp(INITIAL_POS/length(INITIAL_POS), INITIAL_VEL/length(INITIAL_VEL));
	//const VECTOR3 TARGET_POS = RotateVector(ROT_VECTOR, TRANSFER_ANGLE, INITIAL_POS/length(INITIAL_POS))*(TARGET_HEIGHT+EARTH_RADIUS);

	EXPECT_NEAR(length(targeting.GetDeltaV()), 0.0, 300.0); // ensure DV is reasonable
	EXPECT_NEAR(dotp(targeting.GetDeltaV()/length(targeting.GetDeltaV()), ROT_VECTOR), 0.0, 1e-2); // ensure target velocity is in current orbit plane (limit: 0.6 degrees)
	EXPECT_NEAR(dotp(finalPos/length(finalPos), INITIAL_POS/length(INITIAL_POS)), cos(TRANSFER_ANGLE), 0.002); // check transfer angle (limit: 0.1 degrees)
	EXPECT_NEAR(length(finalPos), TARGET_HEIGHT+EARTH_RADIUS, 1e2); // check height at target point
	//EXPECT_NEAR(length(finalPos-TARGET_POS), 0.0, 1e2);
	// check velocity constraints
	double verticalVelocity = dotp(finalVel, finalPos/length(finalPos));
	double horizVelocity = sqrt(pow(length(finalVel), 2) - verticalVelocity*verticalVelocity);
	EXPECT_NEAR(verticalVelocity, C2*horizVelocity+C1, 2e0);
	//EXPECT_NEAR(finalPos.x, TARGET_POS.x, 1e-3);
	//EXPECT_NEAR(finalPos.y, TARGET_POS.y, 1e-3);
	//EXPECT_NEAR(finalPos.z, TARGET_POS.z, 1e-3);
}

/**
 * Tests case when transfer angle is 180 degrees.
 * In this case, initial & target position vectors do not define orbit plane.
 */
TEST(PEG4TargetingTest, TransferAngle180) {
	const double GGRAV = 6.67259e-11;   // from OrbiterAPI.h
	const double EARTH_MASS = 59.73698968e23;
	const double EARTH_RADIUS = 6371.0e3;
	const double mu = GGRAV*EARTH_MASS; // from Orbiter.pdf
	const double J2 = 0.001083;
	
	const VECTOR3 INITIAL_POS = _V(4342290.291048, 1813126.366743, 4632555.251175);
	const VECTOR3 INITIAL_VEL = _V(-2522.182070, -5661.021045, 4598.745860);
	const double TRANSFER_ANGLE = 180.0*RAD;
	const double TARGET_HEIGHT = 300e3;
	const double C1 = 0.0;
	const double C2 = 0.0;
	const double ACCELERATION = 0.4; // shuttle OMS (both) acceleration should be about 0.5 m/s^2

	// HT = 65.832 nm
	//double transferTime = -1.0;
	PEG4Targeting targeting;
	targeting.SetPlanetParameters(EARTH_MASS, EARTH_RADIUS, J2);
	targeting.SetPEG4Targets(C1, C2, TARGET_HEIGHT, TRANSFER_ANGLE, INITIAL_POS, INITIAL_VEL, ACCELERATION);
	while(true) {
		if(targeting.Step()) break;
	}
	ASSERT_TRUE(targeting.Converged());

	//VECTOR3 reqdVel = targeting.GetDeltaV() + INITIAL_VEL;

	const double PROPAGATOR_STEP = 1e-2;
	const double PROPAGATOR_ITERATIONS = 100;
	StateVectorPropagator propagator(PROPAGATOR_STEP, PROPAGATOR_ITERATIONS, 3600.0);
	OMSBurnPropagator omsProp;
	omsProp.SetBurnData(0.0, targeting.GetDeltaV(), ACCELERATION);
	propagator.SetParameters(1.0, EARTH_MASS, EARTH_RADIUS, J2);
	propagator.DefinePerturbations(&omsProp);
	propagator.UpdateStateVector(INITIAL_POS, INITIAL_VEL, 0.0, true);
	int propagatorSteps = static_cast<int>(targeting.GetTransferTime()/(PROPAGATOR_STEP*PROPAGATOR_ITERATIONS));
	for(int i=0;i<propagatorSteps;i++) propagator.Step(0.00, 0.1);
	VECTOR3 finalPos, finalVel;
	propagator.GetStateVectors(targeting.GetTransferTime(), finalPos, finalVel);
	
	const VECTOR3 ROT_VECTOR = crossp(INITIAL_POS/length(INITIAL_POS), INITIAL_VEL/length(INITIAL_VEL));
	//const VECTOR3 TARGET_POS = RotateVector(ROT_VECTOR, TRANSFER_ANGLE, INITIAL_POS/length(INITIAL_POS))*(TARGET_HEIGHT+EARTH_RADIUS);

	EXPECT_NEAR(length(targeting.GetDeltaV()), 0.0, 300.0); // ensure DV is reasonable
	EXPECT_NEAR(dotp(targeting.GetDeltaV()/length(targeting.GetDeltaV()), ROT_VECTOR), 0.0, 1e-2); // ensure target velocity is in current orbit plane (limit: 0.6 degrees)
	EXPECT_NEAR(dotp(finalPos/length(finalPos), INITIAL_POS/length(INITIAL_POS)), cos(TRANSFER_ANGLE), 0.002); // check transfer angle (limit: 0.1 degrees)
	EXPECT_NEAR(length(finalPos), TARGET_HEIGHT+EARTH_RADIUS, 1e2); // check height at target point
	//EXPECT_NEAR(length(finalPos-TARGET_POS), 0.0, 1e2);
	// check velocity constraints
	double verticalVelocity = dotp(finalVel, finalPos/length(finalPos));
	double horizVelocity = sqrt(pow(length(finalVel), 2) - verticalVelocity*verticalVelocity);
	EXPECT_NEAR(verticalVelocity, C2*horizVelocity+C1, 1e0);
	//EXPECT_NEAR(finalPos.x, TARGET_POS.x, 1e-3);
	//EXPECT_NEAR(finalPos.y, TARGET_POS.y, 1e-3);
	//EXPECT_NEAR(finalPos.z, TARGET_POS.z, 1e-3);
}
