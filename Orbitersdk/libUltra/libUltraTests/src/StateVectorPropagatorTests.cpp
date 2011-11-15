#include "StateVectorPropagator.h"
#include "gtest\gtest.h"

TEST(StateVectorPropagatorTest, PropagatesStateVector) {
	StateVectorPropagator propagator(0.2, 50, 4000.0);
	propagator.SetParameters(100.0, 5.9742e24, 6371000.0, 0.00108263);
	//propagator.UpdateStateVector(_V(6571000.0, 0.0, 0.0), _V(5444.72, 5444.72, 0.0), 0.0);
	propagator.UpdateStateVector(_V(5777255.469, 3335500.0, 0.0), _V(0.0, 363.1735531, 7355.460236), 0.0);
	double met = 0.0;
	for(int i=0;i<10000;i++) {
		met+=0.05; // 20 fps framerate
		propagator.Step(met, 0.05);
	}
	// compare to 'correct' results calculated using Octave
	VECTOR3 pos, vel;
	propagator.GetStateVectors(3600.0, pos, vel);
	EXPECT_NEAR(pos.x, 1.1569e6, 100.0);
	EXPECT_NEAR(pos.y, 4.0410e5, 100.0);
	EXPECT_NEAR(pos.z, -5.8983e6, 100.0);
	EXPECT_NEAR(vel.x, 7.0350e3, 10.0);
	EXPECT_NEAR(vel.y, 4.1099e3, 10.0);
	EXPECT_NEAR(vel.z, 8.6403e2, 10.0);
}

TEST(StateVectorPropagatorTest, CalculatesApogeeAndPerigee) {
	StateVectorPropagator propagator(0.05, 20, 5000.0);
	propagator.SetParameters(96461.6, 59.73698968e23, 6371000.0, 0.00108263);
	const VECTOR3 INITIAL_POS = _V(6316623.630082, 1919797.653409, 295929.526463);
	const VECTOR3 INITIAL_VEL = _V(-1258.562315, 2930.574922, 7111.192953);
	const double INITIAL_MET = 975.550000;
	propagator.UpdateStateVector(INITIAL_POS, INITIAL_VEL, INITIAL_MET);
	double met = INITIAL_MET;
	for(int i=0;i<10000;i++) {
		met+=0.05; // 20 fps framerate
		propagator.Step(met, 0.05);
	}
	// correct results found by running Orbiter, mostly at 10x time acceleration
	// Perigee: 1500 sec MET, 228.5 km
	// Apogee: 4183 sec MET, 325.3 km
	double PeD, METAtPerigee;
	//propagator.GetPerigeeData(PeD, METAtPerigee);
	propagator.GetPerigeeData(met, PeD, METAtPerigee);
	EXPECT_NEAR(PeD, 6599500.0, 150.0);
	EXPECT_NEAR(METAtPerigee, 1500.0, 10.0);
	double ApD, METAtApogee;
	//propagator.GetApogeeData(ApD, METAtApogee);
	propagator.GetApogeeData(met, ApD, METAtApogee);
	EXPECT_NEAR(ApD, 6696300.0, 150.0);
	EXPECT_NEAR(METAtApogee, 4183.0, 10.0);
}

TEST(StateVectorPropagatorTest, ReturnsValidResultBeforeStepCalled) {
	StateVectorPropagator propagator(0.05, 20, 5000.0);
	propagator.SetParameters(96461.6, 59.73698968e23, 6371000.0, 0.00108263);
	const VECTOR3 INITIAL_POS = _V(6316623.630082, 1919797.653409, 295929.526463);
	const VECTOR3 INITIAL_VEL = _V(-1258.562315, 2930.574922, 7111.192953);
	const double INITIAL_MET = 975.550000;
	const VECTOR3 EXPECTED_POS = _V(6225096.380127, 2091257.846326, 722949.403981);
	const VECTOR3 EXPECTED_VEL = _V(-1780.828702, 2763.039726, 7068.761503);
	const double FINAL_MET = 1035.753000;
	propagator.UpdateStateVector(INITIAL_POS, INITIAL_VEL, INITIAL_MET);
	VECTOR3 pos, vel;
	propagator.GetStateVectors(FINAL_MET, pos, vel);
	// we need propagator to return reasonably accurate results (and not throw execption) even if Step() isn't called
	EXPECT_NEAR(length(pos), length(EXPECTED_POS), 200.0);
	EXPECT_NEAR(length(vel), length(EXPECTED_VEL), 200.0);
}
