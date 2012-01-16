#include "StateVectorPropagator.h"
#include "gtest\gtest.h"

TEST(StateVectorPropagatorTest, OctavePropagation) {
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

TEST(StateVectorPropagatorTest, OrbiterPropagation10x) {
	StateVectorPropagator propagator(0.2, 50, 4000.0);
	propagator.SetParameters(96904.650960, 5973698968000000000000000.0, 6371010.0, 0.001083);
	propagator.UpdateStateVector(_V(5056798.528649, -547320.350203, -4271363.579233), _V(4602.657749, 3643.030605, 5068.732737), 268.665);
	double met = 268.665;
	for(int i=0;i<10000;i++) {
		met+=0.05; // 20 fps framerate
		propagator.Step(met, 0.05);
	}
	// compare to 'correct' results from Orbiter (10x time acceleration)
	VECTOR3 pos, vel;
	/*propagator.GetStateVectors(2403.979, pos, vel);
	EXPECT_NEAR(pos.x, -1758969.066978, 50.0);
	EXPECT_NEAR(pos.y, 2255303.794633, 50.0);
	EXPECT_NEAR(pos.z, 5975651.945430, 50.0);
	EXPECT_NEAR(vel.x, -7252.797987, 2.0);
	EXPECT_NEAR(vel.y, -2569.911904, 2.0);
	EXPECT_NEAR(vel.z, -1109.049837, 2.0);*/
	propagator.GetStateVectors(3490.362, pos, vel);
	EXPECT_NEAR(pos.x, -6467033.167073, 100.0);
	EXPECT_NEAR(pos.y, -1433782.906445, 10.0);
	EXPECT_NEAR(pos.z, 876563.970814, 10.0);
	EXPECT_NEAR(vel.x, -257.260709, 2.0);
	EXPECT_NEAR(vel.y, -3285.919386, 2.0);
	EXPECT_NEAR(vel.z, -6968.483088, 2.0);
}

TEST(StateVectorPropagatorTest, OrbiterPropagation1x) {
	StateVectorPropagator propagator(0.2, 50, 4000.0);
	propagator.SetParameters(96904.650960, 5973698968000000000000000.0, 6371010.0, 0.001083);
	propagator.UpdateStateVector(_V(4122635.433053, -1186636.293658, -5083779.558075), _V(5730.532653, 3429.424923, 3919.439554), 88.537000);
	double met = 88.537000;
	for(int i=0;i<10000;i++) {
		met+=0.05; // 20 fps framerate
		propagator.Step(met, 0.05);
	}
	// compare to 'correct' results from Orbiter (no time acceleration)
	VECTOR3 pos, vel;
	propagator.GetStateVectors(2740.014, pos, vel);
	EXPECT_NEAR(pos.x, -3999802.109840, 50.0);
	EXPECT_NEAR(pos.y, 1241300.201609, 50.0);
	EXPECT_NEAR(pos.z, 5156717.506392, 50.0);
	EXPECT_NEAR(vel.x, -5913.843901, 2.0);
	EXPECT_NEAR(vel.y, -3386.079311, 2.0);
	EXPECT_NEAR(vel.z, -3698.921817, 2.0);
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

TEST(StateVectorPropagatorTest, CalculatesTimeToAltAccurately) {
	StateVectorPropagator propagator(0.05, 20, 5000.0);
	propagator.SetParameters(96461.6, 59.73698968e23, 6371000.0, 0.00108263);
	//const VECTOR3 INITIAL_POS = _V(622964.481668, -3028206.045104, 5892997.255388);
	const VECTOR3 INITIAL_POS = _V(233311.676827, -2819821.588365, 6020231.977982);
	//const VECTOR3 INITIAL_VEL = _V(-6469.566849, 3350.014902, 2360.274191);
	const VECTOR3 INITIAL_VEL = _V(-6504.368186, 3588.428418, 1876.032923);
	//const double INITIAL_MET = 0.0;
	const double INITIAL_MET = 1374076.391000;
	propagator.UpdateStateVector(INITIAL_POS, INITIAL_VEL, INITIAL_MET);
	double met = INITIAL_MET;
	for(int i=0;i<10000;i++) {
		met+=0.05; // 20 fps framerate
		propagator.Step(met, 0.05);
	}
	//double EI_met = propagator.GetMETAtAltitude(met, 120e3+6371000.0);
	//double METAt119k = propagator.GetMETAtAltitude(met, 119e3+6371000.0);
	double METAt119k = propagator.GetMETAtAltitude(met, 119e3);
	//EXPECT_NEAR(EI_met, 1317, 5.0);
	EXPECT_NEAR(METAt119k, 1375279.239478, 5.0);
}

TEST(StateVectorPropagatorTest, CalculatesTimeToAltBeforePropagation) {
	StateVectorPropagator propagator(0.05, 20, 5000.0);
	propagator.SetParameters(96461.6, 59.73698968e23, 6371000.0, 0.00108263);
	const VECTOR3 INITIAL_POS = _V(233311.676827, -2819821.588365, 6020231.977982);
	const VECTOR3 INITIAL_VEL = _V(-6504.368186, 3588.428418, 1876.032923);
	const double INITIAL_MET = 1374076.391000;
	propagator.UpdateStateVector(INITIAL_POS, INITIAL_VEL, INITIAL_MET);
	//double METAt119k = propagator.GetMETAtAltitude(INITIAL_MET, 119e3+6371000.0);
	double METAt119k = propagator.GetMETAtAltitude(INITIAL_MET, 119e3);
	EXPECT_NEAR(METAt119k, 1375279.239478, 40.0);
}
