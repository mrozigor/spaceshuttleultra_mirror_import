#include "OrbitTgtSoftware.h"
#include "../Atlantis.h"
#include "../vc/MDU.h"
#include "UltraMath.h"
#include "../util/Stopwatch.h"

namespace dps
{

OrbitTgtSoftware::OrbitTgtSoftware(SimpleGPCSystem * _gpc)
: SimpleGPCSoftware(_gpc, "OrbitTgtSoftware"),
transferTime(0), elevation(0), bCalculatingT1Burn(false), bMNVRDataCalculated(false), bValuesChanged(false),
pOMSBurnSoftware(NULL), pStateVectorSoftware(NULL)
{
	for(int i=0;i<4;i++) {
		TIG_T1[i] = 0;
		TIG_T2[i] = 0;
		BASE_TIME[i] = 0;
	}

	relPos_T1 = _V(0, 0, 0);
	relVel_T1 = _V(0, 0, 0);
	relPos_T2 = _V(0, 0, 0);
}

OrbitTgtSoftware::~OrbitTgtSoftware()
{
}

void OrbitTgtSoftware::Realize()
{
	pOMSBurnSoftware = static_cast<OMSBurnSoftware*>(FindSoftware("OMSBurnSoftware"));
	pStateVectorSoftware = static_cast<StateVectorSoftware*>(FindSoftware("StateVectorSoftware"));

	OBJHANDLE hEarth = STS()->GetGravityRef();
	double J2 = 0;
	if(STS()->NonsphericalGravityEnabled()) J2 = oapiGetPlanetJCoeff(hEarth, 0);
	burnTargeting.SetPlanetParameters(oapiGetMass(hEarth), oapiGetSize(hEarth), J2);

	// if T1 TIG or DT were loaded from scenario file, recalculate T2 TIG
	double tig_t2 = ConvertDDHHMMSSToSeconds(TIG_T1) + transferTime*60;
	ConvertSecondsToDDHHMMSS(tig_t2, TIG_T2);
}

void OrbitTgtSoftware::OnPreStep(double SimT, double DeltaT, double MJD)
{
	if(bCalculatingT1Burn) {
		burnTargeting.Step();
		LambertBurnTargeting::RESULT res = burnTargeting.CurrentState();
		if(res == LambertBurnTargeting::CONVERGED) {
			VECTOR3 initialVelocity, finalVelocity;
			burnTargeting.GetData(initialVelocity, finalVelocity);
			GetT1BurnData(initialVelocity);
			bCalculatingT1Burn = false;
		}
		else if(res == LambertBurnTargeting::ERR) {
			bCalculatingT1Burn = false;
			bMNVRDataCalculated = false;
		}
	}
}

bool OrbitTgtSoftware::OnMajorModeChange(unsigned int newMajorMode)
{
	if(newMajorMode == 201 || newMajorMode == 202) return true;
	return false;
}

bool OrbitTgtSoftware::ItemInput(int spec, int item, const char* Data)
{
	if(spec != 34) return false;

	//int nValue;
	//double dValue, dTemp;
	double dTemp;
	switch(item) {
	case 2:
	case 3:
	case 4:
	case 5:
		TIG_T1[item-2] = atoi(Data);
		// recalculate T2 TIG (defined as T1 TIG + DT)
		dTemp = ConvertDDHHMMSSToSeconds(TIG_T1) + transferTime*60;
		ConvertSecondsToDDHHMMSS(dTemp, TIG_T2);
		bValuesChanged = true;
		return true;
	case 13:
	case 14:
	case 15:
	case 16:
		TIG_T2[item-13] = atoi(Data);
		transferTime = ConvertDDHHMMSSToSeconds(TIG_T2) - ConvertDDHHMMSSToSeconds(TIG_T1);
		bValuesChanged = true;
		return true;
	case 17:
		transferTime = atof(Data);
		dTemp = ConvertDDHHMMSSToSeconds(TIG_T1) + transferTime*60;
		ConvertSecondsToDDHHMMSS(dTemp, TIG_T2);
		bValuesChanged = true;
		return true;
	case 18:
	case 19:
	case 20:
		relPos_T2.data[item-18] = atof(Data);
		bValuesChanged = true;
		return true;
	case 21:
	case 22:
	case 23:
	case 24:
		BASE_TIME[item-21] = atoi(Data);
		return true;
	case 28:
		bCalculatingT1Burn = true;
		StartCalculatingT1Burn();
		return true;
	}

	return false;
}

bool OrbitTgtSoftware::OnPaint(int spec, vc::MDU* pMDU) const
{
	if(spec != 34) return false;

	char cbuf[51];

	PrintCommonHeader("   ORBIT TGT", pMDU);

	pMDU->mvprint(1, 2, "MNVR");
	pMDU->mvprint(11, 2, "TIG");
	pMDU->Delta(22, 2);
	pMDU->mvprint(23, 2, "VX");
	pMDU->Delta(29, 2);
	pMDU->mvprint(30, 2, "VY");
	pMDU->Delta(36, 2);
	pMDU->mvprint(37, 2, "VZ");
	pMDU->Delta(44, 2);
	pMDU->mvprint(45, 2, "VT");

	if(bMNVRDataCalculated) {
		double tig = ConvertDDHHMMSSToSeconds(TIG_T1);
		if(STS()->GetMET() > tig) pMDU->mvprint(4, 3, "*");
		sprintf_s(cbuf, 51, "%03.0f/%02.0f:%02.0f:%02.0f", TIG_T1[0], TIG_T1[1], TIG_T1[2], TIG_T1[3]);
		pMDU->mvprint(6, 3, cbuf);
		sprintf_s(cbuf, 51, "%+6.1f", DeltaV.x*MPS2FPS);
		pMDU->mvprint(20, 3, cbuf);
		sprintf_s(cbuf, 51, "%+5.1f", DeltaV.y*MPS2FPS);
		pMDU->mvprint(28, 3, cbuf);
		sprintf_s(cbuf, 51, "%+5.1f", DeltaV.z*MPS2FPS);
		pMDU->mvprint(35, 3, cbuf);
		sprintf_s(cbuf, 51, "%+6.1f", length(DeltaV)*MPS2FPS);
		pMDU->mvprint(42, 3, cbuf);
	}

	pMDU->mvprint(1, 6, "INPUTS");
	pMDU->mvprint(1, 7, "1 TGT NO");
	pMDU->mvprint(1, 8, "2 T1 TIG");
	sprintf_s(cbuf, 51, "%03.0f/%02.0f:%02.0f:%02.0f", TIG_T1[0], TIG_T1[1], TIG_T1[2], TIG_T1[3]);
	pMDU->mvprint(13, 8, cbuf);
	pMDU->mvprint(1, 9, "6   EL");
	for(int y=10;y<=15;y++) pMDU->Delta(5, y);
	pMDU->mvprint(1, 10, "7    X/DNRNG");
	pMDU->mvprint(1, 11, "8    Y");
	pMDU->Delta(8, 12);
	pMDU->mvprint(1, 12, "9    Z/ H");
	for(int y=13;y<=15;y++) pMDU->DotCharacter(6, y);
	pMDU->mvprint(0, 13, "10    X");
	pMDU->mvprint(0, 14, "11    Y");
	pMDU->mvprint(0, 15, "12    Z");
	pMDU->mvprint(0, 16, "13 T2 TIG");
	sprintf_s(cbuf, 51, "%03.0f/%02.0f:%02.0f:%02.0f", TIG_T2[0], TIG_T2[1], TIG_T2[2], TIG_T2[3]);
	pMDU->mvprint(13, 16, cbuf);
	for(int y=17;y<=20;y++) pMDU->Delta(5, y);
	pMDU->mvprint(0, 17, "17    T");
	sprintf_s(cbuf, 51, "%+6.1f", transferTime);
	pMDU->mvprint(18, 17, cbuf);
	pMDU->mvprint(0, 18, "18    X");
	sprintf_s(cbuf, 51, "%+7.2f", relPos_T2.x);
	pMDU->mvprint(18, 18, cbuf);
	pMDU->mvprint(0, 19, "19    Y");
	sprintf_s(cbuf, 51, "%+7.2f", relPos_T2.y);
	pMDU->mvprint(18, 19, cbuf);
	pMDU->mvprint(0, 20, "20    Z");
	sprintf_s(cbuf, 51, "%+7.2f", relPos_T2.z);
	pMDU->mvprint(18, 20, cbuf);
	pMDU->mvprint(0, 21, "21 BASE TIME");
	sprintf_s(cbuf, 51, "%03.0f/%02.0f:%02.0f:%02.0f", BASE_TIME[0], BASE_TIME[1], BASE_TIME[2], BASE_TIME[3]);
	pMDU->mvprint(13, 21, cbuf);

	pMDU->mvprint(37, 6, "CONTROLS");
	pMDU->mvprint(37, 9, "COMPUTE T1 28");
	if(bCalculatingT1Burn) pMDU->mvprint(50, 9, "*");
	pMDU->mvprint(37, 10, "COMPUTE T2 29");
	return true;
}

bool OrbitTgtSoftware::OnParseLine(const char* keyword, const char* value)
{
	if(!_strnicmp(keyword, "TIG_T1", 6)) {
		sscanf_s(value, "%lf%lf%lf%lf", &TIG_T1[0], &TIG_T1[1], &TIG_T1[2], &TIG_T1[3]);
		return true;
	}
	else if(!_strnicmp(keyword, "DT", 2)) {
		sscanf_s(value, "%lf", &transferTime);
		return true;
	}
	else if(!_strnicmp(keyword, "RELPOS_T2", 9)) {
		sscanf_s(value, "%lf%lf%lf", &relPos_T2.x, &relPos_T2.y, &relPos_T2.z);
		return true;
	}
	else if(!_strnicmp(keyword, "BASE_TIME", 9)) {
		sscanf_s(value, "%lf%lf%lf%lf", &BASE_TIME[0], &BASE_TIME[1], &BASE_TIME[2], &BASE_TIME[3]);
		return true;
	}
	return false;
}

void OrbitTgtSoftware::OnSaveState(FILEHANDLE scn) const
{
	char cbuf[255];
	sprintf_s(cbuf, 255, "%0.0f %0.0f %0.0f %0.1f", TIG_T1[0], TIG_T1[1], TIG_T1[2], TIG_T1[3]);
	oapiWriteScenario_string(scn, "TIG_T1", cbuf);
	oapiWriteScenario_float(scn, "DT", transferTime);
	oapiWriteScenario_vec(scn, "RELPOS_T2", relPos_T2);
	sprintf_s(cbuf, 255, "%0.0f %0.0f %0.0f %0.1f", BASE_TIME[0], BASE_TIME[1], BASE_TIME[2], BASE_TIME[3]);
	oapiWriteScenario_string(scn, "BASE_TIME", cbuf);
}

void OrbitTgtSoftware::StartCalculatingT1Burn()
{
	double t1_tig = ConvertDDHHMMSSToSeconds(TIG_T1);
	double t2_tig = ConvertDDHHMMSSToSeconds(TIG_T2);
	//VECTOR3 t1_pos, t1_vel, t2_pos, t2_vel;
	//VECTOR3 t1_orbiter_pos, t1_orbiter_vel, t2_target_pos, t2_target_vel;
	VECTOR3 t2_target_pos, t2_target_vel;
	//pStateVectorSoftware->GetPropagatedStateVectors(t1_tig, t1_orbiter_pos, t1_orbiter_vel);
	pStateVectorSoftware->GetPropagatedStateVectors(t1_tig, t1Pos, t1Vel);
	pStateVectorSoftware->GetTargetStateVectors(t2_tig, t2_target_pos, t2_target_vel);

	// NOTE: X-axis is curved along circular orbit with radius equal to target altitude
	// get reference frame for T2 position errors (ITEMs 18-20)
	// X: from target in +ve velocity vector Z: from target towards center of Earth
	//VECTOR3 x_unit = t2_target_vel/length(t2_target_vel);
	//VECTOR3 z_unit = -t2_target_pos/length(t2_target_pos);
	//VECTOR3 y_unit = crossp(z_unit, x_unit);
	//y_unit = y_unit/length(y_unit); // velocity and position vectors may not be exactly perpendicular
	//x_unit = crossp(y_unit, z_unit);
	// calculate target position at T2
	MATRIX3 T2_Target_Matrix = GetGlobalToLVLHMatrix(t2_target_pos, t2_target_vel, true);
	VECTOR3 positionOffset = relPos_T2*1e3/MPS2FPS; // offset is entered in kft
	VECTOR3 t2_orbiter_pos = t2_target_pos + tmul(T2_Target_Matrix, _V(0, positionOffset.y, positionOffset.z));
	// rotate target vessel position vector by appropriate amount to get X-axis offset
	double XOffsetAngle = positionOffset.x/length(t2_target_pos);
	VECTOR3 y_unit = _V(T2_Target_Matrix.m21, T2_Target_Matrix.m22, T2_Target_Matrix.m23);
	t2_orbiter_pos = RotateVector(y_unit, XOffsetAngle, t2_orbiter_pos);

	ELEMENTS el;
	ORBITPARAM oparam;
	OBJHANDLE hEarth = STS()->GetGravityRef();
	STS()->GetElements(hEarth, el, &oparam); // SolveLambertProblem function only needs approximate period, so current elements are acceptable
	//double mu = GGRAV*(oapiGetMass(hEarth) + STS()->GetMass());

	burnTargeting.SetTargetingData(t1Pos, t2_orbiter_pos, transferTime*60.0, oparam.T, STS()->GetMass());
}

void OrbitTgtSoftware::GetT1BurnData(const VECTOR3& targetEquVelocity)
{
	double t1_tig = ConvertDDHHMMSSToSeconds(TIG_T1);
	//VECTOR3 t1_orbiter_pos, t1_orbiter_vel;
	//pStateVectorSoftware->GetPropagatedStateVectors(t1_tig, t1_orbiter_pos, t1_orbiter_vel);

	// convert DeltaV to LVLH frame
	//VECTOR3 earthFrameDV = targetEquVelocity - t1_orbiter_vel;
	//MATRIX3 RotMatrix = GetGlobalToLVLHMatrix(t1_orbiter_pos, t1_orbiter_vel, true);
	//MATRIX3 RotMatrix = GetGlobalToLVLHMatrix(t1_orbiter_pos, t1_orbiter_vel);
	VECTOR3 earthFrameDV = targetEquVelocity - t1Vel;
	MATRIX3 RotMatrix = GetGlobalToLVLHMatrix(t1Pos, t1Vel, true);
	DeltaV = mul(RotMatrix, earthFrameDV);
	bMNVRDataCalculated = true;
	pOMSBurnSoftware->SetManeuverData(t1_tig, DeltaV);
}

};
