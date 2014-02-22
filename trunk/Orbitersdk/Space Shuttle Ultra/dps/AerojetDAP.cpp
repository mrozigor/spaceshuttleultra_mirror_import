#include "AerojetDAP.h"
#include "../Atlantis.h"
#include "IDP.h"
#include <UltraMath.h>
#include "../ParameterValues.h"


namespace dps
{

const double VA = 23163.7/MPS2FPS;
const double VA1 = 21000.0/MPS2FPS;
const double VA2 = 27197.46/MPS2FPS;
const double VB1 = 19000.0/MPS2FPS;
const double VS1 = 23271.87/MPS2FPS;
const double VSIT2 = VS1*VS1;
const double VTRAN = 10500/MPS2FPS;
const double VQ = 5000/MPS2FPS;
const double VQ2 = VQ*VQ;

/**
 * Draws pitch ladder line on HUD.
 * Rotated font (for labelling line) is assumed to be selected into sketchpad
 * @param skp HUD Sketchpad instance
 * @param hps HUD HUDPAINTSPEC instance
 * @param ladderPitch pitch corresponding to line being drawn
 * @param orbiterPitch current pitch angle of orbiter
 * @param orbiterBank current bank angle of orbiter
 * @returns false if line is out of HUD area; true otherwise
 */
bool DrawHUDPitchLine(oapi::Sketchpad *skp, const HUDPAINTSPEC *hps, int ladderPitch, double orbiterPitch, double orbiterBank)
{
	char pszBuf[20];

	double curPitchDelta = static_cast<double>(ladderPitch) - orbiterPitch;
	int textHeight = skp->GetCharSize() & 65535; // lower 16 bits of GetCharSize

	VECTOR3 line_rot_vector;
	VECTOR3 line_dir_vector = RotateVectorZ(_V(1, 0, 0), orbiterBank);
	VECTOR3 line_pos = RotateVectorZ(_V(5*hps->Scale, curPitchDelta*hps->Scale, 0), orbiterBank);
	line_pos.x = hps->CX-line_pos.x;
	line_pos.y = hps->CY-line_pos.y;
	if(line_pos.y < 0 || line_pos.x < 0) return false;
	if(line_pos.y > hps->W || line_pos.x > hps->H) return false;
	VECTOR3 line_end = line_pos + line_dir_vector*10*hps->Scale;
	if(line_end.y < 0 || line_end.x < 0) return false;
	if(line_end.y > hps->W || line_end.x > hps->H) return false;
	
	// draw line
	if(ladderPitch < 0) { // 2 dashed lines
		VECTOR3 line_seg1_end = line_pos + line_dir_vector*0.85*hps->Scale;
		skp->Line(round(line_pos.x), round(line_pos.y),
			round(line_seg1_end.x), round(line_seg1_end.y));
		VECTOR3 line_seg2_start = line_seg1_end + line_dir_vector*0.3*hps->Scale;
		VECTOR3 line_seg2_end = line_pos + line_dir_vector*2*hps->Scale;
		skp->Line(round(line_seg2_start.x), round(line_seg2_start.y), round(line_seg2_end.x), round(line_seg2_end.y));
		VECTOR3 line_seg3_start = line_end - line_dir_vector*2*hps->Scale;
		VECTOR3 line_seg3_end = line_seg3_start + line_dir_vector*0.85*hps->Scale;
		skp->Line(round(line_seg3_start.x), round(line_seg3_start.y),
			round(line_seg3_end.x), round(line_seg3_end.y));
		VECTOR3 line_seg4_start = line_seg3_end + line_dir_vector*0.3*hps->Scale;
		skp->Line(round(line_seg4_start.x), round(line_seg4_start.y),
			round(line_end.x), round(line_end.y));
	}
	else { //  2 solid lines
		VECTOR3 line_seg1_end = line_pos + line_dir_vector*2*hps->Scale;
		skp->Line(round(line_pos.x), round(line_pos.y), round(line_seg1_end.x), round(line_seg1_end.y));
		VECTOR3 line_seg2_start = line_end - line_dir_vector*2*hps->Scale;		
		skp->Line(round(line_seg2_start.x), round(line_seg2_start.y), round(line_end.x), round(line_end.y));
	}
	// draw lines pointing toward horizon
	if(ladderPitch > 0) {
		line_rot_vector = RotateVectorZ(_V(0, 1, 0), orbiterBank);
		VECTOR3 left_line_end = line_pos + line_rot_vector*0.5*hps->Scale;
		skp->Line(round(left_line_end.x), round(left_line_end.y), round(line_pos.x), round(line_pos.y));
		VECTOR3 right_line_end = line_end + line_rot_vector*0.5*hps->Scale;
		skp->Line(round(right_line_end.x), round(right_line_end.y), round(line_end.x), round(line_end.y));
	}
	else if(ladderPitch < 0) {
		line_rot_vector = RotateVectorZ(_V(0, -1, 0), orbiterBank);
		VECTOR3 left_line_end = line_pos + line_rot_vector*0.5*hps->Scale;
		skp->Line(round(left_line_end.x), round(left_line_end.y), round(line_pos.x), round(line_pos.y));
		VECTOR3 right_line_end = line_end + line_rot_vector*0.5*hps->Scale;
		skp->Line(round(right_line_end.x), round(right_line_end.y), round(line_end.x), round(line_end.y));
	}

	// print angle
	if(ladderPitch != 0) {
		sprintf_s(pszBuf, 20, "%d", ladderPitch);
		//skp->Text(static_cast<int>(line_end.x)+3, static_cast<int>(line_end.y)-textHeight/2, pszBuf, strlen(pszBuf));
		int textWidth = skp->GetTextWidth(pszBuf);
		VECTOR3 textPos = line_end - line_dir_vector*(1+textWidth) + line_rot_vector*(1+textHeight);
		//VECTOR3 textEnd = line_end - line_rot_vector*2;
		skp->Text(round(textPos.x), round(textPos.y), pszBuf, strlen(pszBuf));
		//skp->TextBox(static_cast<int>(textPos.x), static_cast<int>(textPos.y),
			//static_cast<int>(textEnd.x), static_cast<int>(textEnd.y), cbuf, strlen(cbuf));
	}

	return true;
}

/**
 * Draws triangle on sketchpad
 * VECTOR3s are passed because this is what DrawHUDGuidanceTriangles uses
 */
void DrawTriangle(oapi::Sketchpad *skp, const VECTOR3& pt1, const VECTOR3& pt2, const VECTOR3& pt3)
{
	skp->Line(round(pt1.x), round(pt1.y), round(pt2.x), round(pt2.y));
	skp->Line(round(pt1.x), round(pt1.y), round(pt3.x), round(pt3.y));
	skp->Line(round(pt3.x), round(pt3.y), round(pt2.x), round(pt2.y));
}

/**
 * Draws guidance triangles (rotated to be || with horizon) on HUD
 * @param skp HUD Sketchpad instance
 * @param hps HUD HUDPAINTSPEC instance
 * @param trianglePitch pitch corresponding to triangle being drawn
 * @param orbiterPitch current pitch angle of orbiter
 * @param orbiterBank current bank angle of orbiter
 */
void DrawHUDGuidanceTriangles(oapi::Sketchpad *skp, const HUDPAINTSPEC *hps, double degTrianglePitch, double degOrbiterPitch, double degOrbiterBank)
{
	double curPitchDelta = degTrianglePitch - degOrbiterPitch;

	VECTOR3 line_dir_vector = RotateVectorZ(_V(1, 0, 0), degOrbiterBank);
	VECTOR3 line_rot_vector = RotateVectorZ(_V(0, 1, 0), degOrbiterBank);
	VECTOR3 line_pos = _V(hps->CX, hps->CY, 0) - line_rot_vector*(curPitchDelta*hps->Scale); // midpoint between triangles

	VECTOR3 leftTrianglePt1 = line_pos - line_dir_vector*hps->Scale;
	VECTOR3 leftTrianglePt2 = line_pos - line_dir_vector*1.7*hps->Scale - line_rot_vector*0.35*hps->Scale;
	VECTOR3 leftTrianglePt3 = leftTrianglePt2 + line_rot_vector*0.7*hps->Scale;
	DrawTriangle(skp, leftTrianglePt1, leftTrianglePt2, leftTrianglePt3);

	VECTOR3 rightTrianglePt1 = line_pos + line_dir_vector*hps->Scale;
	VECTOR3 rightTrianglePt2 = line_pos + line_dir_vector*1.7*hps->Scale - line_rot_vector*0.35*hps->Scale;
	VECTOR3 rightTrianglePt3 = rightTrianglePt2 + line_rot_vector*0.7*hps->Scale;
	DrawTriangle(skp, rightTrianglePt1, rightTrianglePt2, rightTrianglePt3);
}

VECTOR3 GetPositionVector(OBJHANDLE hPlanet, double lat, double lng, double rad)
{
	VECTOR3 v;
	oapiEquToLocal(hPlanet, lng, lat, rad, &v);
	return v;
}

AerojetDAP::AerojetDAP(SimpleGPCSystem* _gpc)
: SimpleGPCSoftware(_gpc, "AerojetDAP"),
bFirstStep(true), bSecondStep(false), bWONG(false), OrbiterMass(1.0),
//AOA_ElevonPitch(0.25, 0.10, 0.01, -1.0, 1.0, -50.0, 50.0), //NOTE: may be better to reduce integral limits and increase i gain
//Rate_ElevonPitch(0.75, 0.001, 0.005, -0.75, 0.75, -60.0, 60.0),
//Pitch_ElevonPitch(0.25, 0.10, 0.01, -1.0, 1.0, -50.0, 50.0),
//ElevonPitch(0.25, 0.10, 0.01, -1.0, 1.0, -50.0, 50.0), //NOTE: may be better to reduce integral limits and increase i gain
NZ_PitchRate(5.0, 0.0, 1.0, -0.5, 0.5, -0.5, 0.5),
ElevonPitch(0.10, 0.00, 0.25, -1.0, 1.0, -50.0, 50.0), //NOTE: may be better to reduce integral limits and increase i gain
//Roll_AileronRoll(0.15, 0.05, 0.00, -1.0, 1.0),
//Roll_AileronRoll(0.10, 0.20, 0.00, -1.0, 1.0),
//Yaw_RudderYaw(0.15, 0.05, 0.00, -1.0, 1.0),
Roll_AileronRoll(0.20, 0.00, 0.10, -1.0, 1.0),
Yaw_RudderYaw(0.50, 0.00, 0.10, -1.0, 1.0),
QBar_Speedbrake(1.5, 0.0, 0.1, -100.0, 100.0, -200.0, 200.0),
Vel_Speedbrake(2.5, 0.0, 0.2, -100.0, 100.0, -200.0, 200.0),
lastSBTCCommand(1.0),
EntryGuidanceMode(PREENTRY),
referenceDrag23(19.38/MPS2FPS), constDragStartVel(VQ),
tgtAltAveraging(5.0), vspeedAveraging(10.0),  vaccAveraging(20.0),
tgtBankSign(1.0), performedFirstRollReversal(false),
TAEMGuidanceMode(ACQ), HACDirection(OVHD), HACSide(L),
degTargetGlideslope(-20.0), // default OGS glideslope
prfnlBankFader(5.0), HAC_TurnRadius(20000.0/MPS2FPS), TotalRange(0.0),
lastNZUpdateTime(-1.0), nzAveraging(1.0), averageNZ(0.0),
filteredQBar(0.0),
NZCommand(0.0), TargetBank(0.0),
elevonPos(0.0), aileronPos(0.0), rudderPos(0.0),
HUDFlashTime(0.0), bHUDFlasher(true), SITE_ID(0), SEC(false)
{
	PMI = _V(1.0, 1.0, 1.0);
	RCSTorque.data[PITCH] = 0.5*ORBITER_PITCH_TORQUE;
	RCSTorque.data[YAW] = 0.5*ORBITER_YAW_TORQUE;
	RCSTorque.data[ROLL] = 0.5*ORBITER_ROLL_TORQUE;

	for(unsigned short i=0;i<3;i++) {
		ThrustersActive[i]=true;
		AerosurfacesActive[i]=false;
		RotatingAxis[i]=false;
	}

	//degTargetAttitude = _V(0.0, 0.0, 0.0);
	degCurrentAttitude = _V(0.0, 0.0, 0.0);
	degCurrentRates = _V(0.0, 0.0, 0.0);
	degTargetRates = _V(0.0, 0.0, 0.0);
	//degAeroTargetRates = _V(0.0, 0.0, 0.0);
	//degRCSTargetRates = _V(0.0, 0.0, 0.0);

	LoadLandingSiteList(); // this might be done later in creation process

	dTable = new DragTable();
}

AerojetDAP::~AerojetDAP()
{
	delete dTable;
}

void AerojetDAP::Realize()
{
	DiscreteBundle* pBundle=STS()->BundleManager()->CreateBundle("HC_INPUT", 16);
	for(int i=0;i<3;i++) {
		RHCInput[i].Connect(pBundle, i);
	}
	SpdbkThrotPort.Connect(pBundle, 6);	

	pBundle=STS()->BundleManager()->CreateBundle("AEROSURFACE_CMD", 16);
	//LeftElevonCommand.Connect(pBundle, 0);
	//RightElevonCommand.Connect(pBundle, 1);
	ElevonCommand.Connect(pBundle, 0);
	AileronCommand.Connect(pBundle, 1);

	pBundle=STS()->BundleManager()->CreateBundle("THRUSTER_CMD", 16);
	for(unsigned int i=0;i<3;i++) {
		ThrusterCommands[i].Connect(pBundle, i);
	}

	pBundle=BundleManager()->CreateBundle("CSS_CONTROLS", 4);
	PitchAuto.Connect(pBundle, 0);
	PitchCSSOut.Connect(pBundle, 1);
	RollYawAuto.Connect(pBundle, 2);
	RollYawCSSOut.Connect(pBundle, 3);

	pBundle=STS()->BundleManager()->CreateBundle("SPDBKTHROT_CONTROLS", 16);
	SpeedbrakeAuto.Connect(pBundle, 0);
	SpeedbrakeAutoOut.Connect(pBundle, 0);
	
	hEarth = STS()->GetGravityRef();
	InitializeRunwayData();
}

void AerojetDAP::OnPreStep(double SimT, double DeltaT, double MJD)
{
	// on first step, Orbiter gives some incorrect data, so ignore this step
	if(bFirstStep) {
		filteredQBar = STS()->GetDynPressure()*PA2PSF;
		lastSBTCCommand = SpdbkThrotPort.GetVoltage();
		bFirstStep = false;
		bSecondStep = true;
		return;
	}

	if(HUDFlashTime <= SimT) {
		HUDFlashTime = SimT+0.1;
		bHUDFlasher = !bHUDFlasher;
	}

	// select correct side for HAC
	if(HACDirection == STRT || STS()->GetAirspeed() > 9000.0/MPS2FPS) SelectHAC();

	// downmode to CSS if RHC is out of detent
	if(!Eq(RHCInput[PITCH].GetVoltage(), 0.0, 0.1)) PitchCSSOut.SetLine();
	if(!Eq(RHCInput[ROLL].GetVoltage(), 0.0, 0.1)) RollYawCSSOut.SetLine();

	//double distToRwy, delaz; // only used in MM304
	switch(GetMajorMode()) {
	case 304:
		{
		UpdateOrbiterData();
		GetAttitudeData(DeltaT);

		CheckControlActivation();
		//SetThrusterLevels();

		if(STS()->GetMachNumber() < 2.5) SetMajorMode(305);
		
		// control yaw regardless of AUTO/CSS mode
		//if(ThrustersActive[YAW])
			//ThrusterCommands[YAW].SetLine(GetThrusterCommand(YAW));

		// PITCH channel
		double targetAOA = CalculateTargetAOA(STS()->GetMachNumber());
		if(PitchAuto) {
			const double MAX_PITCH_RATE = 0.5;
			degTargetRates.data[PITCH] = range(-MAX_PITCH_RATE, 0.25*(targetAOA-degCurrentAttitude.data[PITCH]), MAX_PITCH_RATE);
			//if(ThrustersActive[PITCH]) degAeroTargetRates.data[PITCH] += degRCSTargetRates.data[PITCH]; // when thrusters are firing, bias aerosurface pitch rate to encourage elevons to correct error
			
			//if(ThrustersActive[PITCH])
				//ThrusterCommands[PITCH].SetLine(GetThrusterCommand(PITCH));

			/*double elevonPos = 0.0;
			double aileronPos = 0.0;
			//double rudderPos = 0.0;
			if(AerosurfacesActive[PITCH]) {
				elevonPos = range(-1.0, AOA_ElevonPitch.Step(degTargetAttitude.data[PITCH]-degCurrentAttitude.data[PITCH], DeltaT), 1.0);
			}
			if(AerosurfacesActive[ROLL]) {
				aileronPos = range(-1.0, Roll_AileronRoll.Step(degTargetAttitude.data[ROLL]-degCurrentAttitude.data[ROLL], DeltaT), 1.0);
			}
			ElevonCommand.SetLine(static_cast<float>(elevonPos));
			AileronCommand.SetLine(static_cast<float>(aileronPos));*/
						
			//sprintf_s(oapiDebugString(), 255, "TargetAOA: %f ElevonPos: %f error: %f", targetAOA, elevonPos, targetAOA-CurrentAttitude.data[PITCH]);
			//sprintf_s(oapiDebugString(), 255, "AOA: %f %f Beta: %f %f Bank: %f %f", degCurrentAttitude.data[PITCH], degCurrentRates.data[PITCH],
				//degCurrentAttitude.data[YAW], degCurrentRates.data[YAW], degCurrentAttitude.data[ROLL], degCurrentRates.data[ROLL]);
		}
		else {
			degTargetRates.data[PITCH] = CSSPitchInput(DeltaT);
		}

		// ROLL/YAW channel
		double tgtBank = CalculateTargetBank(STS()->GetMachNumber(), targetAOA, DeltaT, SimT);
		double tgtBankRate = 0.0;
		if(RollYawAuto)
		{
			double MAX_BANK_RATE = 3.0;
			if(STS()->GetMachNumber() < 23.5) MAX_BANK_RATE = 5.0;
			double BANK_GAIN = 0.20;
			if(ThrustersActive[ROLL]) BANK_GAIN = 1.0;
			tgtBankRate = range(-MAX_BANK_RATE, BANK_GAIN*(tgtBank-degCurrentAttitude.data[ROLL]), MAX_BANK_RATE);
		}
		else
		{
			tgtBankRate = CSSRollInput(DeltaT);
		}
		CalculateTargetRollYawRates(STS()->GetMachNumber(), STS()->GetAOA(), tgtBankRate, degTargetRates.data[ROLL], degTargetRates.data[YAW]);

		//sprintf_s(oapiDebugString(), 255, "Bank rate error: %f", degAeroTargetRates.data[ROLL]-degCurrentRates.data[ROLL]);
		//sprintf_s(oapiDebugString(), 255, "GALR: %f tgtBankRate: %f tgtRollRate: %f tgtYawRate: %f rollRateError: %f yawRateError: %f rollError: %f", GALR, tgtBankRate, degTargetRates.data[ROLL], degTargetRates.data[YAW], degTargetRates.data[ROLL]-degCurrentRates.data[ROLL], degTargetRates.data[YAW]-degCurrentRates.data[YAW], tgtBank-degCurrentAttitude.data[ROLL]);


		// set thruster and aerosurface commands
		for(int i=0;i<3;i++) {
			//if(ThrustersActive[i] && (ThrustersActive[ROLL] || i != YAW))
			if(ThrustersActive[i])
				ThrusterCommands[i].SetLine(static_cast<float>(GetThrusterCommand(static_cast<AXIS>(i), DeltaT)));
			else
				ThrusterCommands[i].SetLine(0.0f);
		}
		SetAerosurfaceCommands(DeltaT);
		SetSpeedbrakeCommand(TotalRange, DeltaT);

		break;
		}
	case 305:
		if(bWONG) {
			//oapiWriteLog("WONG");
			// load relief
			ElevonCommand.SetLine(static_cast<float>(10.0/33.0)); // elevons should be 10 deg down
			AileronCommand.SetLine(0.0f);
			//Nosewheel steering
			double airspeed=STS()->GetAirspeed();
			//sprintf_s(oapiDebugString(), 255, "Pitch: %f", STS()->GetPitch()*DEG);
			//oapiWriteLog(oapiDebugString());
			double steerforce = (95.0-airspeed);
			if(airspeed<6.0) steerforce*=(airspeed/6);
			//steerforce = 27500/3*steerforce*STS()->GetControlSurfaceLevel(AIRCTRL_RUDDER);
			steerforce = 275000/3*steerforce*RHCInput[YAW].GetVoltage();
			STS()->AddForce(_V(steerforce, 0, 0), _V(0, 0, 12.0));
			STS()->AddForce(_V(-steerforce, 0, 0), _V(0, 0, -12.0));
			//sprintf_s(oapiDebugString(), 255, "NWS force: %f", steerforce);
		}
		else {
			//oapiWriteLog("No WONG");

			UpdateOrbiterData();
			GetAttitudeData(DeltaT);

			CheckControlActivation();
			//SetThrusterLevels();

			// calculate dynamic pressure profile
			double qbar = STS()->GetDynPressure()*PA2PSF;
			double qbarDerivative = range(-5.0, 0.5583958*(qbar-filteredQBar), 5.0);
			filteredQBar += qbarDerivative*DeltaT;
			// skip QBD calculation for the moment

			// update NZ
			if(lastNZUpdateTime < SimT-NZ_UPDATE_INTERVAL) {
				lastNZUpdateTime = SimT;

				VECTOR3 lift, drag, gravity;
				STS()->GetLiftVector(lift);
				STS()->GetDragVector(drag);
				STS()->GetWeightVector(gravity);
				double curNZ = (lift.y+drag.y)/length(gravity);
				nzAveraging.NewValue(curNZ, SimT);

				averageNZ = nzAveraging.GetAvgValue();
			}

			switch(TAEMGuidanceMode) {
			case ACQ:
			case HDG:
				CalculateHACGuidance(DeltaT);
				break;
			case PRFNL:
			case OGS:
			case FLARE:
				CalculateTargetGlideslope(GetRunwayRelPos(), DeltaT);
				TargetBank = CalculatePrefinalBank(GetRunwayRelPos());
				break;
			}

			if(PitchAuto)
			{
				double NZSteadyState = cos(STS()->GetPitch())/cos(STS()->GetBank());
				double NZErr = NZCommand+NZSteadyState-averageNZ;
				degTargetRates.data[PITCH] = range(-0.5, 5.0*NZErr, 0.5);
				sprintf_s(oapiDebugString(), 255, "NZ Err: %f", NZErr);
			}
			else
			{
				degTargetRates.data[PITCH] = CSSPitchInput(DeltaT);
			}
			//degTargetRates.data[ROLL] = CSSRollInput(DeltaT);
			//double tgtBankRate = CSSRollInput(DeltaT);
			double tgtBankRate;
			if(RollYawAuto)
			{
				double MAX_BANK_RATE = 5.0;
				double BANK_GAIN = 1.0;
				tgtBankRate = range(-MAX_BANK_RATE, BANK_GAIN*(TargetBank-degCurrentAttitude.data[ROLL]), MAX_BANK_RATE);
			}
			else
			{
				tgtBankRate = CSSRollInput(DeltaT);
			}
			CalculateTargetRollYawRates(STS()->GetMachNumber(), STS()->GetAOA(), tgtBankRate, degTargetRates.data[ROLL], degTargetRates.data[YAW]);
			// set yaw rates
			//const double MAX_YAW_RATE = 10.0;
			//degTargetRates.data[YAW] = range(-MAX_YAW_RATE, -2.0*degCurrentAttitude.data[YAW], MAX_YAW_RATE);
			//degRCSTargetRates.data[YAW] = GetRCSRequiredRate(YAW, 0.0, DeltaT);

			// only yaw thrusters should be active at this point
			if(ThrustersActive[YAW])
				ThrusterCommands[YAW].SetLine(static_cast<float>(GetThrusterCommand(YAW, DeltaT)));
			else
				ThrusterCommands[YAW].SetLine(0.0f);
			ThrusterCommands[PITCH].SetLine(0.0f);
			ThrusterCommands[ROLL].SetLine(0.0f);

			SetAerosurfaceCommands(DeltaT);
			SetSpeedbrakeCommand(TotalRange, DeltaT);

			// check for weight-on-nose-gear
			if(STS()->GroundContact() && STS()->GetPitch() < -3.0*RAD) {
				bWONG = true;
				// set default positions of control surfaces to zero
				STS()->SetControlSurfaceLevel(AIRCTRL_RUDDER, 0.0);
				STS()->SetControlSurfaceLevel(AIRCTRL_ELEVATOR, 0.0);
				STS()->SetControlSurfaceLevel(AIRCTRL_AILERON, 0.0);
			}
		}
		break;
	}
	
	bFirstStep = false;
	bSecondStep = false;
}

bool AerojetDAP::OnMajorModeChange(unsigned int newMajorMode)
{
	if(newMajorMode ==304 || newMajorMode == 305) {
		// set Translation Commands to 0.0 during entry and landing
		DiscreteBundle* pBundle = BundleManager()->CreateBundle("THRUSTER_CMD", 16);
		DiscOutPort port;
		for(int i=0;i<3;i++) {
			port.Connect(pBundle, i+3);
			port.ResetLine();
		}

		if(newMajorMode == 305) {
			//TAEMGuidanceMode = ACQ; // initial value; only enter S-Turn phase if we have to
			// initialize filtered qbar value
			filteredQBar = STS()->GetDynPressure()*PA2PSF;
			// reduce roll gains 
			Roll_AileronRoll.SetGains(0.10, 0.00, 0.01);
		}
		else {
			// initialize both pitch and roll/yaw channels to AUTO
			DiscOutPort port;
			pBundle=BundleManager()->CreateBundle("CSS_CONTROLS", 4);
			port.Connect(pBundle, 0); // PITCH AUTO
			port.SetLine();
			port.Connect(pBundle, 2); // ROLL/YAW AUTO
			port.SetLine();
		}
		return true;
	}
	return false;
}

bool AerojetDAP::ItemInput(int spec, int item, const char* Data)
{
	if(spec == 50) {
		if(item == 3) {
			SEC = false;
			InitializeRunwayData();
			return true;
		}
		else if(item == 4) {
			SEC = true;
			InitializeRunwayData();
			return true;
		}
		else if(item == 6) {
			if(TAEMGuidanceMode <= ACQ) { // if we are already in HDG mode (on HAC), it's too late to change from OVHD to STRT
				if(HACDirection == OVHD) HACDirection = STRT;
				else HACDirection = OVHD;
				SelectHAC();
			}
		}
		else if(item == 41) {
			int nNew;
			sscanf_s(Data, "%d", &nNew);
			if(nNew>=0 && nNew<=static_cast<int>(vLandingSites.size())) {
				SITE_ID = nNew-1;
				InitializeRunwayData();
			}
			return true;
		}
	}
	return false;
}

bool AerojetDAP::OnPaint(int spec, vc::MDU* pMDU) const
{
	if(spec == 50) {
		PaintHORIZSITDisplay(pMDU);
		return true;
	}
	return false;
}

bool AerojetDAP::OnDrawHUD(const HUDPAINTSPEC* hps, oapi::Sketchpad* skp) const
{
	if(GetMajorMode() == 305) {
		char cbuf[255];
		VECTOR3 Velocity;
		double dOut;
		int commanded, act;

		// show gear deployment status
		if (STS()->GetGearState()==AnimState::OPEN) {
			skp->Text(hps->CX-60, hps->CY+5, "GR-DN", 5);
		}
		else if (STS()->GetGearState()==AnimState::OPENING) {
			skp->Text(hps->CX-60, hps->CY+5, "//GR//", 6);
		}

		STS()->GetHorizonAirspeedVector(Velocity);
		//dOut = 661.47 * STS()->GetMachNumber() * sqrt(STS()->GetAtmPressure()/101325.0);
		dOut = STS()->GetKEAS();
		sprintf_s(cbuf, 255, "KEAS:%.0f", dOut);
		skp->Text(hps->W-100,(hps->H)/2-25,cbuf,strlen(cbuf));
		dOut=(STS()->GetAltitude()*3.280833)-17;
		sprintf_s(cbuf, 255, "ALT:%.0f",dOut);
		skp->Text(10,(hps->H)/2-25,cbuf,strlen(cbuf));

		// draw pitch ladder
		double dPitch = STS()->GetPitch()*DEG;
		int nPitch = static_cast<int>(dPitch);
		int pitchBar = nPitch - (nPitch%5); // display lines at 5-degree increments
		double bank = STS()->GetBank()*DEG;
		
		// create rotated font for HUD pitch markings
		oapi::Font* pFont = oapiCreateFont(12, false, "Fixed", FONT_NORMAL, -static_cast<int>(10*bank));
		oapi::Font* pOldFont = skp->SetFont(pFont);
		// draw pitch lines
		int curPitchLine = pitchBar;
		while(DrawHUDPitchLine(skp, hps, curPitchLine, dPitch, bank))
			curPitchLine+=5;	
		curPitchLine = pitchBar-5;
		while(DrawHUDPitchLine(skp, hps, curPitchLine, dPitch, bank))
			curPitchLine-=5;
		// deselect rotated font
		skp->SetFont(pOldFont);
		oapiReleaseFont(pFont);
		//skp->Line(hps->CX-20, hps->CY, hps->CX+20, hps->CY);

		// draw cross at center
		skp->Line(hps->CX-5, hps->CY, hps->CX+5, hps->CY);
		skp->Line(hps->CX, hps->CY-5, hps->CX, hps->CY+5);

		// draw velocity vector
		double glideslope_center_y, glideslope_center_x;
		if(TAEMGuidanceMode>=PRFNL) {
			glideslope_center_y = hps->CY + STS()->GetAOA()*DEG*hps->Scale;
			glideslope_center_x = hps->CX - STS()->GetSlipAngle()*DEG*hps->Scale;
			if(prfnlBankFader > 1.0) {
				double HUD_Center_X = static_cast<double>(hps->CX);
				double HUD_Center_Y = static_cast<double>(hps->H)/2.0 - 25.0;
				glideslope_center_x = HUD_Center_X + (glideslope_center_x-HUD_Center_X)/prfnlBankFader;
				glideslope_center_y = HUD_Center_Y + (glideslope_center_y-HUD_Center_Y)/prfnlBankFader;
			}
		//double dBank = (bank-TargetBank)/prfnlBankFader;
		//prfnlBankFader -= oapiGetSimStep();
		//sprintf_s(oapiDebugString(), 255, "Fader bank: %f Actual target: %f", TargetBank+dBank, bank);
		//return TargetBank+dBank*oapiGetSimStep();
			skp->Ellipse(round(glideslope_center_x)-5, round(glideslope_center_y)-5, round(glideslope_center_x)+5, round(glideslope_center_y)+5);
		}
		else {
			// before PRFNL mode, we have square at center of HUD
			glideslope_center_y = static_cast<double>(hps->H)/2.0 - 25.0;
			glideslope_center_x = static_cast<double>(hps->CX);
			skp->Rectangle(round(glideslope_center_x)-5, round(glideslope_center_y)-5, round(glideslope_center_x)+5, round(glideslope_center_y)+5);
		}
		// lines are the same for both VV and center square modes
		skp->Line(round(glideslope_center_x)-10, round(glideslope_center_y), round(glideslope_center_x)-5, round(glideslope_center_y));
		skp->Line(round(glideslope_center_x)+9, round(glideslope_center_y), round(glideslope_center_x)+4, round(glideslope_center_y));
		skp->Line(round(glideslope_center_x), round(glideslope_center_y)-10, round(glideslope_center_x), round(glideslope_center_y)-5);

		if(TAEMGuidanceMode != FNLFL) {
			double guidance_center_x, guidance_center_y;
			VECTOR3 lift, drag;
			STS()->GetLiftVector(lift);
			STS()->GetDragVector(drag);
			//double NZ = (lift.y+drag.y)/gravity_force;
			double NZSteadyState = cos(STS()->GetPitch())/cos(STS()->GetBank());
			//sprintf_s(oapiDebugString(), 255, "NZ: %f NZ SS: %f NZ Comm Inc: %f", NZ, NZSteadyState, NZCommand);
			//sprintf_s(oapiDebugString(), 255, "NZ: %f NZ Command: %f NZ Inc: %f", NZ, NZAccError+NZSteadyState, NZAccError);
			//sprintf_s(oapiDebugString(), 255, " %s NZ: %f NZ Command: %f NZ SS: %f", oapiDebugString(), NZ, NZCommand, NZSteadyState);
			if(TAEMGuidanceMode < FLARE) guidance_center_y = glideslope_center_y - (10.0*(NZCommand+NZSteadyState-averageNZ))*hps->Scale;
			else guidance_center_y = glideslope_center_y - (5.0*(NZCommand+NZSteadyState-averageNZ))*hps->Scale;
			guidance_center_x = hps->CX + (STS()->GetBank()*DEG+TargetBank)*hps->Scale;
			//sprintf_s(oapiDebugString(), 255, "NZ: %f NZ Command: %f diff: %f", averageNZ, NZCommand, NZCommand+NZSteadyState-averageNZ);
			// if guidance diamond is within HUD area, draw it normally; otherwise, draw flashing diamond at edge of HUD
			bool bValid = true;
			if(guidance_center_x < 0.0) {
				bValid = false;
				guidance_center_x = 0.0;
			}
			else if(guidance_center_x > hps->W) {
				bValid = false;
				guidance_center_x = hps->W;
			}
			if(guidance_center_y < 0.0) {
				bValid = false;
				guidance_center_y = 0.0;
			}
			else if(guidance_center_y > hps->H-57) {
				bValid = false;
				guidance_center_y = hps->H-57;
			}
			if(bValid || bHUDFlasher) {
				skp->MoveTo(round(guidance_center_x)-5, round(guidance_center_y));
				skp->LineTo(round(guidance_center_x), round(guidance_center_y)+5);
				skp->LineTo(round(guidance_center_x)+5, round(guidance_center_y));
				skp->LineTo(round(guidance_center_x), round(guidance_center_y)-5);
				skp->LineTo(round(guidance_center_x)-5, round(guidance_center_y));
			}

			// draw guidance triangles
			if(TAEMGuidanceMode >= OGS) DrawHUDGuidanceTriangles(skp, hps, degTargetGlideslope, dPitch, bank);
		}

		switch(TAEMGuidanceMode) {
		case ACQ:
			strcpy_s(cbuf, "ACQ");
			break;
		case HDG:
			strcpy_s(cbuf, "HDG");
			break;
		case PRFNL:
			strcpy_s(cbuf, "PRFNL");
			break;
		case OGS:
			strcpy_s(cbuf, "OGS");
			break;
		case FLARE:
			strcpy_s(cbuf, "FLARE");
			break;
		case FNLFL:
			strcpy_s(cbuf, "FNLFL");
			break;
		}
		skp->Text(30, hps->H-85, cbuf, strlen(cbuf));

		//sprintf(oapiDebugString(), "%f", GetDrag());
		//MoveToEx(hDC, (hps->W/2)-25, hps->H-85, NULL);
		skp->MoveTo((hps->CX), hps->H-85);
		skp->LineTo((hps->CX)+50, hps->H-85);
		for(int i=0;i<=50;i+=10)
		{
			skp->Line((hps->CX)+i, hps->H-80, (hps->CX)+i, hps->H-90);
		}
		double spdb_tgt = STS()->GetTgtSpeedbrakePosition();
		double spdb_act = STS()->GetActSpeedbrakePosition();
		commanded=(int)(spdb_tgt*50);
		act=(int)(spdb_act*50);
		//actual
		skp->MoveTo((hps->CX)+act, hps->H-85);
		skp->LineTo((hps->CX)+act-5, hps->H-90);
		skp->LineTo((hps->CX)+act+5, hps->H-90);
		skp->LineTo((hps->CX)+act, hps->H-85);
		//commanded
		skp->MoveTo((hps->CX)+commanded, hps->H-85);
		skp->LineTo((hps->CX)+commanded-5, hps->H-80);
		skp->LineTo((hps->CX)+commanded+5, hps->H-80);
		skp->LineTo((hps->CX)+commanded, hps->H-85);

		//VECTOR3 rwypos = GetRunwayRelPos();
		//double distance = length(rwypos);
		//VECTOR3 rwynorm = rwypos;
		//normalise(rwynorm);
		//VECTOR3 end2 = rwypos + tmul(RwyRotMatrix,rwynorm)*4000;
		//double maxsin = sin(fov/2);
		////first end
		//double hsin = rwypos.z/distance - sin(STS()->GetPitch());
		//double wsin = rwypos.y/distance - sin(STS()->GetSlipAngle());
		//double hcomp = (hps->H/2) * (hsin/maxsin);
		//double wcomp = (hps->W/2) * (wsin/maxsin);


		////second end
		//double distance2 = length(end2);
		//double hsin2 = end2.z/distance2 - sin(STS()->GetPitch());
		//double wsin2 = end2.y/distance2 - sin(STS()->GetSlipAngle());
		//double hcomp2 = (hps->H/2) * (hsin2/maxsin);
		//double wcomp2 = (hps->W/2) * (wsin2/maxsin);
		//skp->MoveTo(hps->W/2 + wcomp,hps->H/2 - hcomp);
		//skp->LineTo(hps->W/2 + wcomp2,hps->H/2 - hcomp2);
		//sprintf(oapiDebugString(),"%lf %lf %lf %lf",end2.x,end2.y,end2.z,asin(wsin2)*DEG);

		double lat1, lon1, lat2, lon2;
		vLandingSites[SITE_ID].GetRwyPosition(true,lat1,lon1);
		VECTOR3 rwy1_end, lrwy1;
		oapiEquToGlobal(hEarth,lon1,lat1,oapiGetSize(hEarth),&rwy1_end);
		STS()->Global2Local(rwy1_end,lrwy1);

		vLandingSites[SITE_ID].GetRwyPosition(false,lat2,lon2);
		VECTOR3 rwy2_end, lrwy2;
		oapiEquToGlobal(hEarth,lon2,lat2,oapiGetSize(hEarth),&rwy2_end);
		STS()->Global2Local(rwy2_end,lrwy2);

		VECTOR3 rwyDir = lrwy2 - lrwy1;
		normalise(rwyDir);
		VECTOR3 lvlh = RotateVectorZ( RotateVectorX(_V(0, 1, 0), -dPitch), -bank);
		VECTOR3 rwyWidthDir = crossp(rwyDir, lvlh);

		const double RUNWAY_WIDTH = 100.0; // works for KSC; might not work for other runways
		VECTOR3 rwy1_l = lrwy1 - rwyWidthDir*(RUNWAY_WIDTH/2.0);
		VECTOR3 rwy1_r = lrwy1 + rwyWidthDir*(RUNWAY_WIDTH/2.0);
		VECTOR3 rwy2_l = lrwy2 - rwyWidthDir*(RUNWAY_WIDTH/2.0);
		VECTOR3 rwy2_r = lrwy2 + rwyWidthDir*(RUNWAY_WIDTH/2.0);

		VECTOR3 camPos;
		STS()->GetCameraOffset(camPos);

		VECTOR3 error[4];
		error[0] = camPos-rwy1_l;
		error[1] = camPos-rwy2_l;
		error[2] = camPos-rwy2_r;
		error[3] = camPos-rwy1_r;
		int rwy_pos_x[4], rwy_pos_y[4];
		for(int i=0;i<4;i++) {
			rwy_pos_x[i] = hps->CX + static_cast<int>( round(hps->Scale*DEG*atan(error[i].x/error[i].z)) );
			rwy_pos_y[i] = hps->CY + static_cast<int>( round(hps->Scale*DEG*atan(-error[i].y/error[i].z)) );
		}
		// check if at least one of the points is visible
		bool drawRunway = false;
		for(int i=0;i<4;i++) {
			if((rwy_pos_x[i] >= 0 && rwy_pos_x[i] < hps->W) || (rwy_pos_y[i] >= 0 && rwy_pos_y[i] < hps->H)) {
				drawRunway = true;
				break;
			}
		}
		if(drawRunway) {
			skp->Line(rwy_pos_x[0], rwy_pos_y[0], rwy_pos_x[1], rwy_pos_y[1]);
			skp->Line(rwy_pos_x[1], rwy_pos_y[1], rwy_pos_x[2], rwy_pos_y[2]);
			skp->Line(rwy_pos_x[2], rwy_pos_y[2], rwy_pos_x[3], rwy_pos_y[3]);
			skp->Line(rwy_pos_x[3], rwy_pos_y[3], rwy_pos_x[0], rwy_pos_y[0]);
		}

		//UPPER LEFT CORNER
		/*VECTOR3 left1 = lrwy1 - (rotDir*40);
		left1-=camPos;
		double wcomp_left1 = (left1.x/(left1.z*tan(fov))+1)*hps->W/2;
		double hcomp_left1 = (-left1.y/(left1.z*tan(fov))+1)*hps->H/2;

		//UPPER RIGHT CORNER
		VECTOR3 right1 = lrwy1 + (rotDir*40);
		right1-=camPos;
		double wcomp_right1 = (right1.x/(right1.z*tan(fov))+1)*hps->W/2;
		double hcomp_right1 = (-right1.y/(right1.z*tan(fov))+1)*hps->H/2;

		//LOWER RIGHT CORNER
		VECTOR3 right2 = lrwy2 + rotDir*40;
		right2-=camPos;
		double wcomp_right2 = (right2.x/(right2.z*tan(fov))+1)*hps->W/2;
		double hcomp_right2 = (-right2.y/(right2.z*tan(fov))+1)*hps->H/2;

		//LOWER LEFT CORNER
		VECTOR3 left2 = lrwy2 - rotDir*40;
		left2-=camPos;
		double wcomp_left2 = (left2.x/(left2.z*tan(fov))+1)*hps->W/2;
		double hcomp_left2 = (-left2.y/(left2.z*tan(fov))+1)*hps->H/2;

		//DRAW RUNWAY
		skp->MoveTo(wcomp_left1,hcomp_left1);
		skp->LineTo(wcomp_right1,hcomp_right1);
		skp->LineTo(wcomp_right2,hcomp_right2);
		skp->LineTo(wcomp_left2,hcomp_left2);
		skp->LineTo(wcomp_left1,hcomp_left1);*/




		////move to first end
		//VECTOR3 vector_end1 = lrwy2 - camPos;
		//double hcomp2 = (-vector_end1.y/(vector_end1.z*tan(fov))+1)*hps->H/2;
		//double wcomp2 = (vector_end1.x/(vector_end1.z*tan(fov))+1)*hps->W/2;
		//skp->MoveTo(wcomp2,hcomp2);
		//
		////draw line to second end
		//VECTOR3 vector_end2 = lrwy1 - camPos;
		//double hcomp1 = (-vector_end2.y/(vector_end2.z*tan(fov))+1)*hps->H/2;
		//double wcomp1 = (vector_end2.x/(vector_end2.z*tan(fov))+1)*hps->W/2;
		//skp->LineTo(wcomp1,hcomp1);

	}


	return true;
}

bool AerojetDAP::OnParseLine(const char* keyword, const char* value)
{
	if(!_strnicmp(keyword, "SITE_ID", 7)) {
		sscanf_s(value, "%d", &SITE_ID);
		return true;
	}
	else if(!_strnicmp(keyword, "SEC", 3)) {
		SEC = true;
		return true;
	}
	else if(!_strnicmp(keyword, "FIRST_RR", 8)) {
		performedFirstRollReversal = true;
		return true;
	}
	else if(!_strnicmp(keyword, "SIDE", 4)) {
		if(!_strnicmp(value, "L", 1)) HACSide = L;
		else HACSide = R;
		return true;
	}
	else if(!_strnicmp(keyword, "ENTRY_GUIDANCE", 14)) {
		int nTemp;
		sscanf_s(value, "%d", &nTemp);
		if(nTemp>=PREENTRY && nTemp<=TRANSITION) EntryGuidanceMode=static_cast<ENTRY_GUIDANCE_MODE>(nTemp);
		return true;
	}
	else if(!_strnicmp(keyword, "TAEM_GUIDANCE", 13)) {
		int nTemp;
		sscanf_s(value, "%d", &nTemp);
		if(nTemp>=0 && nTemp<=FNLFL) TAEMGuidanceMode=static_cast<TAEM_GUIDANCE_MODE>(nTemp);
		return true;
	}
	return false;
}

void AerojetDAP::OnSaveState(FILEHANDLE scn) const
{
	oapiWriteScenario_int(scn, "SITE_ID", SITE_ID);
	if(SEC) oapiWriteScenario_string(scn, "SEC", "TRUE");
	if(HACSide==L) oapiWriteScenario_string(scn, "SIDE", "L");
	else oapiWriteScenario_string(scn, "SIDE", "R");
	if(performedFirstRollReversal) oapiWriteScenario_string(scn, "FIRST_RR", "TRUE");
	oapiWriteScenario_int(scn, "ENTRY_GUIDANCE", static_cast<int>(EntryGuidanceMode));
	oapiWriteScenario_int(scn, "TAEM_GUIDANCE", static_cast<int>(TAEMGuidanceMode));
}

void AerojetDAP::PaintHORIZSITDisplay(vc::MDU* pMDU) const
{
	PrintCommonHeader("HORIZ SIT", pMDU);

	pMDU->mvprint(0, 6, "PRI");
	pMDU->mvprint(4, 6, vLandingSites[SITE_ID].GetPriRwyName().c_str());
	pMDU->mvprint(13, 6, "3");
	pMDU->mvprint(0, 7, "SEC");
	pMDU->mvprint(4, 7, vLandingSites[SITE_ID].GetSecRwyName().c_str());
	pMDU->mvprint(13, 7, "4");
	if(SEC) pMDU->mvprint(14, 7, "*");
	else pMDU->mvprint(14, 6, "*");

	pMDU->mvprint(0, 12, "G&N");
	if(HACDirection == OVHD) pMDU->mvprint(6, 12, "OVHD 6");
	else pMDU->mvprint(6, 12, "STRT 6");
	if(GetMajorMode() >= 304) {
		if(HACSide == L) {
			pMDU->mvprint(4, 12, "L");
			pMDU->mvprint(0, 13, "HSI L");
		}
		else {
			pMDU->mvprint(4, 12, "R");
			pMDU->mvprint(0, 13, "HSI R");
		}
	}

	const int BUG_POINT_X = 128;
	const int BUG_POINT_Y = 158;
	const int BUG_WIDTH1 = 1;
	const int BUG_HEIGHT1 = 6;
	const int BUG_WIDTH2 = BUG_WIDTH1 + 4;
	const int BUG_HEIGHT2 = BUG_HEIGHT1 + 4;

	const double YSGN = (HACSide==L) ? -1.0 : 1.0;
	const double HAC_CENTER_Y = YSGN * FINAL_RADIUS;

	// frame used for all vectors: LVLH-like frame with origin at shuttle position, X-axis along -ve velocity vector, Y-axis towards port side
	VECTOR3 velocity;
	STS()->GetHorizonAirspeedVector(velocity);
	double degHeading = DEG*atan2(velocity.x, velocity.z);
	double degHeadingError = vLandingSites[SITE_ID].GetRwyHeading(SEC) - degHeading ;
	while(degHeadingError < 0.0) degHeadingError+=360.0;
	while(degHeadingError > 360.0) degHeadingError-=360.0;
	VECTOR3 TgtPos = GetRunwayRelPos();

	VECTOR3 TouchdownPos = -RotateVectorZ(_V(TgtPos.x-IGS_AIMPOINT, TgtPos.y, 0.0), degHeadingError); // ignore height error
	VECTOR3 HACExitPos = -RotateVectorZ(_V(TgtPos.x-HAC_CENTER_X, TgtPos.y, 0.0), degHeadingError);
	// calculate scale factor for display
	double scale_distance = max(length(TouchdownPos), length(HACExitPos)+HAC_TurnRadius); // make sure HAC circle and touchdown point are visible
	scale_distance = range(20e3, scale_distance, 500e3); // limit distance covered by display to between 20km and 500km
	double scale = scale_distance/128; // screen area is 256 pixels by 256 pixels
	int touchdown_x = BUG_POINT_X - round(TouchdownPos.y/scale);
	int touchdown_y = round(TouchdownPos.x/scale) + BUG_POINT_Y;
	int hac_exit_x = BUG_POINT_X - round(HACExitPos.y/scale);
	int hac_exit_y = round(HACExitPos.x/scale) + BUG_POINT_Y;
	pMDU->Circle(touchdown_x, touchdown_y, 4);
	pMDU->Line(hac_exit_x, hac_exit_y, touchdown_x, touchdown_y);

	VECTOR3 HACCenter = -RotateVectorZ(_V(TgtPos.x-HAC_CENTER_X, TgtPos.y-HAC_CENTER_Y, 0.0), degHeadingError);
	int hac_center_x = BUG_POINT_X - round(HACCenter.y/scale);
	int hac_center_y = BUG_POINT_Y + round(HACCenter.x/scale);
	int hac_radius = round(HAC_TurnRadius/scale);
	pMDU->Circle(hac_center_x, hac_center_y, hac_radius);

	// draw position predictor circles
	// get speed and acceleration in LVLH-like frame
	VECTOR3 localForce;
	STS()->GetForceVector(localForce);
	VECTOR3 localAcceleration = localForce/STS()->GetMass();
	VECTOR3 horizonAcceleration;
	STS()->HorizonRot(localAcceleration, horizonAcceleration);
	VECTOR3 groundVelocity = _V(-sqrt(velocity.x*velocity.x + velocity.z*velocity.z), 0, 0); // initial velocity is always along display's y axis
	VECTOR3 groundAcceleration = RotateVectorZ(_V(-horizonAcceleration.z, -horizonAcceleration.x, 0), -degHeading);
	// to calculate position in future, split acceleration into (constant magnitude) radial and tangential components and do numerical integration
	// assumes aerodynamic forces (lift/drag) are constant
	double radial_acc = -groundAcceleration.y;
	double tangential_acc = -groundAcceleration.x;
	VECTOR3 pos = _V(0, 0, 0);
	const double DELTA_T = 0.1;
	for(double time=DELTA_T;time<=60.1;time+=DELTA_T) {
		pos += groundVelocity*DELTA_T + groundAcceleration*(0.5*DELTA_T*DELTA_T);
		// calculate acceleration from radial and tangential components
		VECTOR3 norm_vel = groundVelocity/length(groundVelocity);
		groundAcceleration = crossp(norm_vel, _V(0, 0, -1))*radial_acc + norm_vel*tangential_acc;
		groundVelocity = groundVelocity + groundAcceleration*DELTA_T;

		if(Eq(time, 20, 0.01) || Eq(time, 40, 0.01) || Eq(time, 60, 0.01)) {
			int pos_x = BUG_POINT_X - round(pos.y/scale);
			int pos_y = BUG_POINT_Y + round(pos.x/scale);
			pMDU->Circle(pos_x, pos_y, 4, dps::DEUATT_OVERBRIGHT);
		}
	}

	// draw shuttle bug (this is always at fixed position)
	pMDU->Line(BUG_POINT_X, BUG_POINT_Y, BUG_POINT_X+BUG_WIDTH1, BUG_POINT_Y+BUG_HEIGHT1, dps::DEUATT_OVERBRIGHT);
	pMDU->Line(BUG_POINT_X, BUG_POINT_Y, BUG_POINT_X-BUG_WIDTH1, BUG_POINT_Y+BUG_HEIGHT1, dps::DEUATT_OVERBRIGHT);
	pMDU->Line(BUG_POINT_X+BUG_WIDTH1, BUG_POINT_Y+BUG_HEIGHT1, BUG_POINT_X+BUG_WIDTH2, BUG_POINT_Y+BUG_HEIGHT2, dps::DEUATT_OVERBRIGHT);
	pMDU->Line(BUG_POINT_X-BUG_WIDTH1, BUG_POINT_Y+BUG_HEIGHT1, BUG_POINT_X-BUG_WIDTH2, BUG_POINT_Y+BUG_HEIGHT2, dps::DEUATT_OVERBRIGHT);
	pMDU->Line(BUG_POINT_X-BUG_WIDTH2, BUG_POINT_Y+BUG_HEIGHT2, BUG_POINT_X+BUG_WIDTH2, BUG_POINT_Y+BUG_HEIGHT2, dps::DEUATT_OVERBRIGHT);
	pMDU->Line(BUG_POINT_X, BUG_POINT_Y+BUG_HEIGHT2, BUG_POINT_X, BUG_POINT_Y+BUG_HEIGHT2+2, dps::DEUATT_OVERBRIGHT);
	//pMDU->Line(126, 160, 122, 166);
	//pMDU->Line(130, 160, 134, 166);
	//pMDU->Line(122, 166, 134, 166);
	//pMDU->Line(128, 166, 128, 168);
}

void AerojetDAP::SetAerosurfaceCommands(double DeltaT)
{
	/*double elevonPos = 0.0;
	double aileronPos = 0.0;
	double rudderPos = 0.0;*/
	if(AerosurfacesActive[PITCH]) {
		elevonPos = range(elevonPos-1.0*DeltaT, ElevonPitch.Step(degTargetRates.data[PITCH]-degCurrentRates.data[PITCH], DeltaT), elevonPos+1.0*DeltaT);
		elevonPos = range(-1.0, elevonPos, 1.0);
		//elevonPos = range(-1.0, ElevonPitch.Step(degTargetAttitude.data[PITCH]-degCurrentAttitude.data[PITCH], DeltaT), 1.0);
	}
	if(AerosurfacesActive[ROLL]) {
		aileronPos = range(-1.0, Roll_AileronRoll.Step(degTargetRates.data[ROLL]-degCurrentRates.data[ROLL], DeltaT), 1.0);
	}
	if(AerosurfacesActive[YAW]) {
		//rudderPos = Yaw_RudderYaw.Step(-STS()->GetSlipAngle()*DEG, DeltaT);
		rudderPos = Yaw_RudderYaw.Step(degTargetRates.data[YAW]-degCurrentRates.data[YAW], DeltaT);
	}
	ElevonCommand.SetLine(static_cast<float>(-elevonPos)); // PID controller output has opposite sign of required elevon direction
	AileronCommand.SetLine(static_cast<float>(-aileronPos));
	STS()->SetControlSurfaceLevel(AIRCTRL_RUDDER, rudderPos);

	//sprintf_s(oapiDebugString(), 255, "Roll: %f Target Roll: %f Roll Rate: %f Commanded Aileron: %f",
		//degCurrentAttitude.data[ROLL], degTargetAttitude.data[ROLL], degCurrentRates.data[ROLL], aileronPos);
}

void AerojetDAP::SetSpeedbrakeCommand(double range, double DeltaT)
{
	if(SpeedbrakeAuto) {
		// check for manual takeover
		if(!bSecondStep && !Eq(SpdbkThrotPort.GetVoltage(), lastSBTCCommand, 0.01)) SpeedbrakeAutoOut.ResetLine();
		lastSBTCCommand = SpdbkThrotPort.GetVoltage();
		
		STS()->SetSpeedbrake(CalculateSpeedbrakeCommand(range, DeltaT)/100.0);
	}
	else STS()->SetSpeedbrake(1.0f-SpdbkThrotPort.GetVoltage()); // full throttle corresponds to closed speedbrake
}

void AerojetDAP::SetThrusterLevels()
{
	//for the moment, use RHC input to control thruster firings
	for(unsigned short i=0;i<3;i++) {
		if(ThrustersActive[i]) {
			if(RHCInput[i].GetVoltage()>0.01) {
				ThrusterCommands[i].SetLine(1.0f);
			}
			else if(RHCInput[i].GetVoltage()<-0.01) {
				ThrusterCommands[i].SetLine(-1.0f);
			}
			else {
				ThrusterCommands[i].ResetLine();
			}
		}
		else {
			ThrusterCommands[i].ResetLine();
		}
	}
	/*if(PitchActive) {
		if(RHCInput[PITCH].GetVoltage()>0.01) {
			ThrusterCommands[PITCH].SetLine(1.0f);
		}
		else if(RHCInput[PITCH].GetVoltage()<-0.01) {
			ThrusterCommands[PITCH].SetLine(-1.0f);
		}
		else {
			ThrusterCommands[PITCH].ResetLine();
		}
	}
	else {
	}

	if(YawActive) {
		if(RHCInput.data[YAW]>0.01) {
		}
		else if(RHCInput.data[YAW]<-0.01) {
		}
		else {
		}
	}
	else {
	}

	if(RollActive) {
		if(RHCInput.data[ROLL]>0.01) {
		}
		else if(RHCInput.data[ROLL]<-0.01) {
		}
		else {
		}
	}
	else {
	}*/
}

double AerojetDAP::GetThrusterCommand(AXIS axis, double DeltaT)
{
	//const double RATE_DEADBAND = max(0.05, RotationRateChange(OrbiterMass, PMI.data[axis], RCSTorque.data[axis], DeltaT));
	double RATE_DEADBAND = 0.1;
	if(RotatingAxis[axis]) RATE_DEADBAND = 0.05;
	
	double rateError = degTargetRates.data[axis]-degCurrentRates.data[axis];
	if(abs(rateError) < RATE_DEADBAND) {
		RotatingAxis[axis] = false;
		return 0.0;
	}
	else {
		RotatingAxis[axis] = true;
		return sign(rateError);
	}
}

void AerojetDAP::CheckControlActivation()
{
	const double ROLL_OFF_PRESS=10*47.880259;
	const double PITCH_OFF_PRESS=40*47.880259;
	const double YAW_OFF_MACH=1.0;

	const double ELEVON_ON_PRESS=2.0*47.880259;
	const double RUDDER_ON_MACH=5.0;

	double dynamicPressure = STS()->GetDynPressure();
	double mach = STS()->GetMachNumber();

	if(ThrustersActive[ROLL] && dynamicPressure>ROLL_OFF_PRESS) {
		ThrustersActive[ROLL]=false;
	}
	if(ThrustersActive[PITCH] && dynamicPressure>PITCH_OFF_PRESS) {
		ThrustersActive[PITCH]=false;
	}
	if(ThrustersActive[YAW] && mach<YAW_OFF_MACH && dynamicPressure>1000.0) { // 2nd test is to make sure we're in atmosphere
		ThrustersActive[YAW]=false;
	}

	if(!AerosurfacesActive[PITCH] && dynamicPressure>ELEVON_ON_PRESS) {
		AerosurfacesActive[PITCH]=true;
		AerosurfacesActive[ROLL]=true;
	}
	if(!AerosurfacesActive[YAW] && mach<RUDDER_ON_MACH && dynamicPressure>1000.0) {
		AerosurfacesActive[YAW]=true;
	}
}

double AerojetDAP::CSSPitchInput(double DeltaT)
{
	return RHCInput[PITCH].GetVoltage()*6.0 + STS()->GetControlSurfaceLevel(AIRCTRL_ELEVATORTRIM)*2.5;
}


double AerojetDAP::CSSRollInput(double DeltaT)
{
	return RHCInput[ROLL].GetVoltage()*5.0;
}

void AerojetDAP::GetAttitudeData(double DeltaT)
{
	//double lastSideslip = degCurrentAttitude.data[YAW];
	VECTOR3 degLastAttitude = degCurrentAttitude;

	// get AOA, sideslip and bank
	if(GetMajorMode()==304)
		degCurrentAttitude.data[PITCH] = STS()->GetAOA()*DEG;
	else
		degCurrentAttitude.data[PITCH] = STS()->GetPitch()*DEG;
	degCurrentAttitude.data[YAW] = STS()->GetSlipAngle()*DEG;
	// for AUTO control, we are interested in bank around lift vector; for CSS control, use actual bank
	if(!RollYawAuto)
		degCurrentAttitude.data[ROLL] = -STS()->GetBank()*DEG;
	else
		degCurrentAttitude.data[ROLL] = CalculateCurrentLiftBank();

	STS()->GetAngularVel(degCurrentRates);
	degCurrentRates = _V(degCurrentRates.x, -degCurrentRates.y, degCurrentRates.z); // convert to body axis frame
	degCurrentRates*=DEG;
	VECTOR3 biasRates; // calculate rotation rate (body axis frame) required to maintain constant attitude
	if(GetMajorMode() == 304) {
		biasRates.data[PITCH] = 0.0;
	}
	else {
		biasRates.data[PITCH] = degCurrentRates.data[YAW]*tan(degCurrentAttitude.data[ROLL]*RAD); // pitch rate required to maintain constant pitch during turns
	}
	VECTOR3 lift, drag, gravity;
	STS()->GetLiftVector(lift);
	STS()->GetDragVector(drag);
	STS()->GetWeightVector(gravity);
	double NY = (lift.x+drag.x+gravity.x)/OrbiterMass;
	biasRates.data[YAW] = DEG*(NY/STS()->GetAirspeed()); // yaw rate required for turn coordination
	//sprintf_s(oapiDebugString(), 255, "Bias rate: %f", biasRates.data[YAW]);
	biasRates.data[ROLL] = 0.0;
	degCurrentRates -= biasRates;
}

double AerojetDAP::CalculateCurrentLiftBank() const
{
	VECTOR3 horizonVel;
	STS()->GetHorizonAirspeedVector(horizonVel);
	double gamma = asin(horizonVel.y/length(horizonVel));
	
	VECTOR3 liftVec, horLiftVec;
	bool bLift = STS()->GetLiftVector(liftVec);
	liftVec = RotateVectorX(liftVec, -gamma*DEG);
	if(bLift) {
		STS()->HorizonRot(liftVec, horLiftVec);
		double bankSign = -sign(STS()->GetBank());
		double liftBank = acos(horLiftVec.y/length(horLiftVec));
		return DEG*bankSign*liftBank;
	}
	else { // no lift, so just return normal bank angle
		return -DEG*STS()->GetBank();
	}
}

double AerojetDAP::CalculateRequiredLiftBank(double tgtVAcc) const
{
	const double MAX_BANK_COS = 0.173648; // limit bank to 80 degrees
	VECTOR3 gravityVec, relativeVelVec;
	STS()->GetWeightVector(gravityVec);
	oapiGetRelativeVel(STS()->GetHandle(), hEarth, &relativeVelVec);
	double relativeVel = length(relativeVelVec);
	double centrifugalForce = (relativeVel*relativeVel)/(STS()->GetAltitude()+oapiGetSize(hEarth));
	double reqdLiftForce = tgtVAcc - centrifugalForce + length(gravityVec)/OrbiterMass;
	double cosBank = range(MAX_BANK_COS, reqdLiftForce/(STS()->GetLift()/OrbiterMass), 1);
	return DEG*acos(cosBank);
}

void AerojetDAP::UpdateOrbiterData()
{
	OrbiterMass=STS()->GetMass();
	STS()->GetPMI(PMI);
}

double AerojetDAP::CalculateTargetAOA(double mach) const
{
	const double AOA_Lookup[24] = {
		40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 39.0, 38.0, 36.0, 34.0, 30.0, 27.0, 23.0, 19.0, 16.0, 15.0};
	//  25,   24,   23,   22,   21,   20,   19,   18,   17,   16,   15,   14,   13,   12,   11,   10,   9,    8,    7,    6,    5,    4,    3,    2.5
	
	if(mach > 3.0) {
		int index = max(25 - static_cast<int>(mach), 1);
		return linterp(static_cast<int>(mach), AOA_Lookup[index], static_cast<int>(mach)+1, AOA_Lookup[index-1], mach);
	}
	else {
		return linterp(3.0, AOA_Lookup[22], 2.5, AOA_Lookup[23], mach);
	}
}

double AerojetDAP::CalculateTargetBank(double mach, double targetAOA, double DeltaT, double SimT)
{
	// PREENTRY: hold constant bank of 0 degrees.
	// 1. Calculate target drag
	// 2. Calculate altitude required to achieve target drag (assuming current mach & aerosurface positions and target AOA)
	// 3. Differentiate reference altitude to get ref vspeed & vacc
	// 4. Calculate tgt vspeed and vacc to get reference altititude & vspeed
	// 5. Calculate bank required to provide target vacc

	double r, delaz;
	double cl, cm, cd;
	GetShuttleVerticalAeroCoefficients(mach, targetAOA, &(STS()->aerosurfaces), &cl, &cm, &cd);
	CalculateRangeAndDELAZ(r, delaz);
	UpdateRollDirection(mach, delaz);

	double target_drag = CalculateTargetDrag(DeltaT, r);
	double tgtBank = 0.0;

	if (EntryGuidanceMode != PREENTRY) {
		double speed = STS()->GetAirspeed();
		double newTargetAltitude = dTable->TargetAltitude(target_drag,speed,STS()->GetAOA()*DEG,STS()->GetMass(), cd);
		// update averaging
		UpdateRequiredStateAveraging(newTargetAltitude, DeltaT, SimT);
	
		VECTOR3 vec;
		STS()->GetHorizonAirspeedVector(vec);
		
		double target_altitude = tgtAltAveraging.GetAvgValue();
		double avg_vspeed = vspeedAveraging.GetAvgValue();
		double target_vspeed = avg_vspeed + 1e-2*(target_altitude - STS()->GetAltitude());
		double target_vacc = vaccAveraging.GetAvgValue() + 5e-2*(target_vspeed - vec.y);
		
		lastTgtAltitude = target_altitude;
		lastRefVSpeed = avg_vspeed;
		//last_vel = vec.y;

		double actBank = CalculateCurrentLiftBank();
		tgtBank = tgtBankSign*CalculateRequiredLiftBank(target_vacc);
		//if(ThrustersActive[PITCH]) {
		/*if(STS()->GetMachNumber() > 15.0) {
		// limit bank angle (after initial roll) to > 50 degrees
		// this limits wild roll oscillations
		if(abs(degCurrentAttitude.data[ROLL]) > 40.0 && abs(tgtBank) < 50.0) tgtBank = tgtBankSign*50.0;
		}*/
			
		//sprintf_s(oapiDebugString(), 255, "Altitude error: %lf Tgt Vspeed: %f Vspeed error: %f Ref Vacc: %f Tgt VAcc: %f Bank: %f Tgt bank: %f Bank error: %f", target_altitude-STS()->GetAltitude(), vspeedAveraging.GetAvgValue(), vspeedAveraging.GetAvgValue()-vec.y, vaccAveraging.GetAvgValue(), target_vacc, actBank, tgtBank, tgtBank-actBank);
		/*char cbuf[255];
		//sprintf_s(cbuf, 255, "Target drag: %lf, Target altitude: %lf, Altitude error: %lf Tgt Vspeed: %f Vspeed error: %f Tgt VAcc: %f VAcc error: %f",target_drag,target_altitude,target_altitude-STS()->GetAltitude(), lastVspeedSum/lastVspeeds.size(), lastVspeedSum/lastVspeeds.size()-vec.y, target_vacc, target_vacc-cur_vacc);
		//sprintf_s(cbuf, 255, "Altitude error: %lf Tgt Vspeed: %f Vspeed error: %f Ref Vacc: %f Tgt VAcc: %f Bank: %f Tgt bank: %f Bank error: %f Expected Vacc: %f", target_altitude-STS()->GetAltitude(), lastVspeedSum/lastVspeeds.size(), lastVspeedSum/lastVspeeds.size()-vec.y, lastVAccSum/lastVAccs.size(), target_vacc, actBank, tgtBank, tgtBank-actBank, expectedVacc);
		sprintf_s(cbuf, 255, "Altitude error: %lf Tgt Vspeed: %f Vspeed error: %f Ref Vacc: %f Tgt VAcc: %f Bank: %f Tgt bank: %f Bank error: %f", target_altitude-STS()->GetAltitude(), vspeedAveraging.GetAvgValue(), vspeedAveraging.GetAvgValue()-vec.y, vaccAveraging.GetAvgValue(), target_vacc, actBank, tgtBank, tgtBank-actBank);
		//sprintf_s(oapiDebugString(), 255, "Target drag: %lf, Actual drag: %lf, range: %lf, Target altitude: %lf, Altitude error: %lf Vspeed error: %f",target_drag,STS()->GetDrag()/STS()->GetMass(),r,target_altitude,target_altitude-STS()->GetAltitude(), target_vspeed-vec.y);
		switch(EntryGuidanceMode) {
		case TEMP_CONTROL:
			sprintf_s(oapiDebugString(), 255, "TEMP_CONTROL: %s", cbuf);
			break;
		case EQU_GLIDE:
			sprintf_s(oapiDebugString(), 255, "EQU_GLIDE: %s", cbuf);
			break;
		case CONST_DRAG:
			sprintf_s(oapiDebugString(), 255, "CONST_DRAG: %s", cbuf);
			break;
		case TRANSITION:
			sprintf_s(oapiDebugString(), 255, "TRANSITION: %s", cbuf);
			break;
		}*/
	}
	/*else {
		sprintf_s(oapiDebugString(), 255, "PREENTRY");
	}*/
	return tgtBank;
}

double AerojetDAP::CalculateGALR(double mach) const
{
	// values for Wraparound DAP (used for normal entry)
	// Baseline DAP (used for TAL, used to be used for normal entry) has different profile
	
	// lookup table from M1.0 to M7.0
	const double GALR_Lookup[7] = {0.2, 0.2, 0.5, 0.5, 1.40, 1.4, 1.7};
	const double Mach_values[7] = {0.9, 1.5, 2.0, 3.5, 4.25, 5.5, 7.0};

	if(mach > 7.0) return 1.7;
	else if(mach < 0.9) return 0.0;
	else {
		unsigned int idx = GetLowerIndex(Mach_values, 7, mach);
		return linterp(Mach_values[idx], GALR_Lookup[idx], Mach_values[idx+1], GALR_Lookup[idx+1], mach);
	}
}

void AerojetDAP::CalculateTargetRollYawRates(double mach, double radAOA, double degTgtBankRate, double& degTgtRollRate, double& degTgtYawRate) const
{
	const double GALR = CalculateGALR(mach);

	degTgtRollRate = (1.0-GALR)*degTgtBankRate*cos(radAOA) + (GALR*degCurrentRates.data[YAW]/tan(radAOA) + 2.0*degCurrentAttitude.data[YAW]*sin(radAOA)); // don't subtract Pcor (current roll rate); this is done in SetAerosurfaceCommands()
	degTgtYawRate = degTgtBankRate*sin(radAOA) - 0.5*degCurrentAttitude.data[YAW]*cos(radAOA); // turn coordination biase is added when calculating current yaw rate

	//sprintf_s(oapiDebugString(), 255, "GALR: %f tgtBankRate: %f tgtRollRate: %f tgtYawRate: %f rollRateError: %f yawRateError: %f", GALR, degTgtBankRate, degTgtRollRate, degTgtYawRate, degTgtRollRate-degCurrentRates.data[ROLL], degTgtYawRate-degCurrentRates.data[YAW]);
}

double AerojetDAP::CalculateTargetDrag(double DeltaT, double range)
{
	const double AK = -3.4573;
	const double AK1 = -4.76;
	const double ALFM = 33.0/MPS2FPS; // target const. drag level
	const double DT2MIN = 0.008/MPS2FPS;
	const double DELV = 2300/MPS2FPS;
	const double DRDDL = -1.5*NMI2M*MPS2FPS;
	const double DF = 20.80031/MPS2FPS;
	const double E1 = 0.01/MPS2FPS;
	const double EEF4 = 2.0e6/(MPS2FPS*MPS2FPS); // changed from value in 80FM23; document has erroneous value of 2.0e-6 instead of 2.0e6
	const double ETRAN = 0.5998473e8/(MPS2FPS*MPS2FPS);
	const double GS2 = 0.0001;
	const double RPT1 = 29.44*NMI2M;

	const double RCG1 = (VSIT2-VQ2)/(2*ALFM);
	const double RPT = RPT1 - ((ETRAN-EEF4)*log(DF/ALFM)/(ALFM-DF) + (VTRAN*VTRAN-VQ2)/(2.0*ALFM));
	
	const double CQ3_1 = -AK/(2.0*VB1*(VA-VB1));
	const double CQ2_1 = -2.0*VA*CQ3_1;
	const double CQ1_1 = 1.0 - VB1*(CQ2_1+CQ3_1*VB1);
	const double DX2 = CQ1_1 + VA1*(CQ2_1+CQ3_1*VA1);
	const double CQ3_2 = -(AK1*DX2)/(2.0*VA1*(VA2-VA1));
	const double CQ2_2 = -2.0*VA2*CQ3_2;
	const double CQ1_2 = DX2 - VA1*(CQ2_2+CQ3_2*VA1);

	const double CLG_INIT_DRAG = 4.25/MPS2FPS;
	const double D23C = 19.38/MPS2FPS;

	//VECTOR3 vel;
	//STS()->GetRelativeVel(hEarth, vel);
	//double relativeVel=length(vel);
	//char cbuf[255];
	double relativeVel = STS()->GetAirspeed();
	double oldConstDragLevel = constDragLevel;
	constDragLevel = (relativeVel*relativeVel-VQ2)/(2*(range-RPT));
	double constDragLevelDot = (constDragLevel - oldConstDragLevel)/DeltaT;
	double energyOverMass = G*STS()->GetAltitude() + 0.5*relativeVel*relativeVel; // EEF
	double C1 = (constDragLevel-DF)/(ETRAN-EEF4);
	double DREFP5 = DF + (energyOverMass-EEF4)*C1;
	//sprintf_s(cbuf, 255, "T2: %f T2DOT: %f", constDragLevel, constDragLevelDot);
	//oapiWriteLog(cbuf);
	switch(EntryGuidanceMode) {
	case PREENTRY:
		VECTOR3 lift, drag;
		double acceleration;
		STS()->GetLiftVector(lift);
		STS()->GetDragVector(drag);
		acceleration = length(lift+drag)/STS()->GetMass();
		if(acceleration > CLG_INIT_DRAG) EntryGuidanceMode = TEMP_CONTROL;
		return CLG_INIT_DRAG;
	case TEMP_CONTROL:
	case EQU_GLIDE:
		{
		//double constDragLevel = dTable->TargetDrag(range, relativeVel); // TODO: move this function out of DragTable class
		//double refDrag;
		//if(dTable->TargetDrag(range, relativeVel) > DT2MIN || relativeVel < (VQ+DELV)) {
		//if(constDragLevelDot > DT2MIN || relativeVel < (VQ+DELV)) {
		if(constDragLevelDot > DT2MIN || relativeVel < (constDragStartVel+DELV)) {
			double VB2 = VB1*VB1;
			if(relativeVel<VB1) VB2 = relativeVel*relativeVel;
			constDragStartVel = VQ;
			double D23L = ALFM*(VSIT2-VB2)/(VSIT2-VQ2);
			if(referenceDrag23 > D23L) constDragStartVel = sqrt(VSIT2-D23L*(VSIT2-VQ2)/referenceDrag23);
			//double A2 = NMI2M*(VSIT2-VB2)/2.0;
			double A2 = (VSIT2-VB2)/2.0;
			double equGlideRangeDrag = A2*log(ALFM/referenceDrag23); // REQ1
			double constDragRange = RCG1 - A2/referenceDrag23; // RCG
			double phase23RangeDrag = equGlideRangeDrag; // R231; should add range from Temp Control phase as well, but we are only doing calculation for Equ Glide phase
			//double R23 = range/NMI2M - constDragRange - RPT;
			double R23 = range - constDragRange - RPT;
			double newReferenceDrag23 = phase23RangeDrag/R23; // D231
			if(newReferenceDrag23 >= D23L)
				referenceDrag23 = newReferenceDrag23 + (A2*pow(1 - referenceDrag23/newReferenceDrag23, 2))/(2*R23);
			else
				referenceDrag23 = max(newReferenceDrag23, E1);
			refDrag = referenceDrag23;
			//sprintf_s(cbuf, 255, "D23L: %f A2: %f constDragLevel: %f referenceDrag23: %f", D23L, A2, constDragLevel, referenceDrag23);
			//sprintf_s(cbuf, 255, "R23: %f A2: %f constDragLevel: %f newReferenceDrag23: %f referenceDrag23: %f", R23, A2, constDragLevel, newReferenceDrag23, referenceDrag23);
			//oapiWriteLog(cbuf);
		}
		//else EntryGuidanceMode = CONST_DRAG;
		if(EntryGuidanceMode == TEMP_CONTROL) {
			if(relativeVel < VA1) {
				refDrag = CQ1_1 + relativeVel*(CQ2_1 + relativeVel*CQ3_1);
			}
			else {
				refDrag = CQ1_2 + relativeVel*(CQ2_2 + relativeVel*CQ3_2);
			}
			refDrag *= D23C;
			if(relativeVel < VB1) EntryGuidanceMode = EQU_GLIDE;
			//return refDrag*D23C; // D23 isn't constant in actual code
			//return refDrag*referenceDrag23;
		}
		// check for transition to equ glide phase
		// if in equ glide phase, use slightly different target drag if velocity > VB1 (otherwise, just use referenceDrag23)
		if(relativeVel < VA && relativeVel >= VB1) {
			double ALDCO = (1.0 - VB1*VB1/VSIT2)/referenceDrag23;
			double DREFP1 = (1.0 - relativeVel*relativeVel/VSIT2)/ALDCO; // if relativeVel<VB1, DREFP1 = referenceDrag23 (D23)
			//char cbuf[255];
			//sprintf_s(cbuf, 255, "ALDCO: %f DREFP1: %f referenceDrag23: %f", ALDCO, DREFP1, referenceDrag23);
			//oapiWriteLog(cbuf);
			//double DREFP1 = referenceDrag23;
			//double DREFP3 = DREFP1 + GS2*(RDTREF - RDTRF1); // actual code
			double DREFP3 = DREFP1; // simpler version so we don't need to calculated RDTREF and RDTRF1
			if(refDrag < DREFP3 || EntryGuidanceMode == EQU_GLIDE) {
				EntryGuidanceMode = EQU_GLIDE;
				refDrag = DREFP1;
			}
		}
		if(relativeVel < (constDragStartVel + DELV) && refDrag > constDragLevel) { // NOTE: GS3 = 0
			EntryGuidanceMode = CONST_DRAG;
		}

		return refDrag;
		//double aldco = (1 - (relativeVel*relativeVel/VSIT2))/D23C;
		//return (1 - 
		}
	case CONST_DRAG:
		if(relativeVel < (VTRAN + DELV) && constDragLevel > DREFP5) EntryGuidanceMode = TRANSITION;
		refDrag = constDragLevel;
		return constDragLevel;
	case TRANSITION:
		{
		double DREFPT = refDrag - DF;
		if(abs(DREFPT) < E1) refDrag = DF + E1*sign(DREFPT);
		if(refDrag < E1) refDrag = E1;
		DREFPT = refDrag - DF;
		C1 = DREFPT/(energyOverMass-EEF4);
		double transitionRange = log(refDrag/DF)/C1; // RER1
		double dRdD = min(1/(C1*refDrag) - transitionRange/DREFPT, DRDDL); // DRDD
		refDrag = refDrag + (range - transitionRange - RPT1)/dRdD;
		//double DREFP_MAX = ALIM*(STS()->GetDrag()/STS()->GetMass())/
		// skip drag limit code; not sure how load factor is defined (probably Lift/mass)
		return refDrag;
		}
	default:
		return -10.0; // indicate the target drag is not known
	}
}

void AerojetDAP::UpdateRequiredStateAveraging(double targetAltitude, double DeltaT, double SimT)
{
	tgtAltAveraging.NewValue(targetAltitude, SimT);

	double ref_vspeed = range(-250.0, (tgtAltAveraging.GetAvgValue()-lastTgtAltitude)/DeltaT, 250.0);
	//if(lastTargetAltitudes.size() <= 1) ref_vspeed = 0.0;
	vspeedAveraging.NewValue(ref_vspeed, SimT);

	double ref_vacc = range(-10.0, (vspeedAveraging.GetAvgValue()-lastRefVSpeed)/DeltaT, 10.0);
	//if(lastVspeeds.size() <= 1) ref_vacc = 0.0;
	vaccAveraging.NewValue(ref_vacc, SimT);
}

void AerojetDAP::UpdateRollDirection(double mach, double delaz)
{
	if(EntryGuidanceMode == PREENTRY) {
		tgtBankSign = -sign(delaz);
	}
	else {
		double delazLimit = 17.5;
		if(!performedFirstRollReversal) delazLimit = 10.5;
		if(mach < 4.0) delazLimit = range(10, (mach-2.8)*6.25 + 10.0, 17.5);
		if(abs(delaz) >= delazLimit) {
			tgtBankSign = -sign(delaz);
			performedFirstRollReversal = true;
		}
	}
}

void AerojetDAP::SelectHAC()
{
	VECTOR3 pos = GetRunwayRelPos();
	HAC_SIDE newHAC;
	if(HACDirection == OVHD) {
		if(pos.y < 0.0) newHAC = R;
		else newHAC = L;
	}
	else { // STRT
		if(pos.y < 0.0) newHAC = L;
		else newHAC = R;
	}
	if(HACSide != newHAC) {
		HACSide = newHAC;
		InitializeRunwayData();
	}
}

void AerojetDAP::CalculateHACGuidance(double DeltaT)
{
	const double YSGN = (HACSide==L) ? -1.0 : 1.0;
	const double DR3 = 8000.0/MPS2FPS;
	//const double FINAL_RADIUS = 14000.0/MPS2FPS;
	//const double HAC_RADIUS = 20000/MPS2FPS;
	//const double HAC_CENTER_X = OGS_AIMPOINT - 33020.0/MPS2FPS;
	//const double HAC_CENTER_Y = -FINAL_RADIUS;
	const double HAC_CENTER_Y = YSGN * FINAL_RADIUS;
	//const double R1 = 0.0;
	//const double R2 = 0.093/MPS2FPS;
	// NOTE: gains are in deg/ft, so multiply by conversion factor
	const double GR = 0.005*MPS2FPS;
	const double GRDOT = 0.2*MPS2FPS;

	//VECTOR3 TgtPos = RotateVectorX(GetRunwayRelPos(), RwyHeading);
	VECTOR3 TgtPos = GetRunwayRelPos();
	//sprintf_s(oapiDebugString(), 255, "RwyPos: %f %f %f", TgtPos.x, TgtPos.y, TgtPos.z);
	
	VECTOR3 horz_airspeed, velocity;
	STS()->GetHorizonAirspeedVector(horz_airspeed);
	//double airspeed = length(_V(horz_airspeed.x, 0.0, horz_airspeed.z));
	velocity = RotateVectorY(horz_airspeed, degRwyHeading);
	velocity = _V(velocity.z, velocity.x, -velocity.y);
	
	//double rtan = 0.0;
	double radius = length(_V(TgtPos.x-HAC_CENTER_X, TgtPos.y-HAC_CENTER_Y, 0.0)); // RCIRC
	double pst = DEG*atan2(HAC_CENTER_Y-TgtPos.y, HAC_CENTER_X-TgtPos.x); // temporary variable for calculating HAC turn angle
	double rtan = 0.0;
	if(radius > HAC_TurnRadius) {
		rtan = sqrt(radius*radius - HAC_TurnRadius*HAC_TurnRadius);
		pst -= DEG*YSGN*atan2(HAC_TurnRadius, rtan);
	}
	else {
		pst -= YSGN*90.0;
	}
	if(pst < -180.0) pst+=360.0;
	else if(pst > 180.0) pst-=360.0;
	double turn_angle = -pst*YSGN; // PSHAN
	if(turn_angle < 0.0) turn_angle+=360.0;
	HAC_TurnRadius = FINAL_RADIUS + R1*turn_angle + R2*turn_angle*turn_angle; // RTURN
	double HAC_range = FINAL_RADIUS*turn_angle + 0.5*R1*turn_angle*turn_angle + (1.0/3.0)*R2*turn_angle*turn_angle*turn_angle;
	TotalRange = HAC_range*RAD - HAC_CENTER_X;
	//double tgt_turn_rate = airspeed/HAC_RADIUS;
	//double rad_turn_rate = airspeed/radius; // turn rate for circle with current radius
	
	if(TAEMGuidanceMode == ACQ) {
		//double headingToRwy = -atan2(TgtPos.y, -TgtPos.x)*DEG; // minus sign seems to be needed to get math to work
		double courseToRwy = atan2(velocity.y, velocity.x)*DEG;
		double headingToHAC = pst-courseToRwy; // DPSAC
		if(headingToHAC < -180.0) headingToHAC+=360.0;
		else if(headingToHAC > 180.0) headingToHAC-=360.0;

		double degAvgBank = range(30.00, 63.33 - 13.33*STS()->GetMachNumber(), 50.00);
		double acqTurnRadius = sqrt(velocity.x*velocity.x+velocity.y*velocity.y)*length(velocity)/(G*tan(degAvgBank*RAD));
		double acqTurnArcLength = acqTurnRadius*abs(headingToHAC)*RAD;
		double a = acqTurnRadius*(1.0 - cos(headingToHAC*RAD));
		double b = rtan - acqTurnRadius*abs(sin(headingToHAC*RAD));
		double rangeFromAcqTurnToHAC = sqrt(a*a + b*b);
		TotalRange += acqTurnArcLength + rangeFromAcqTurnToHAC;

		//TargetBank = 2.5*headingToHAC;
		double bankLimit = 50.0;
		if(STS()->GetMachNumber() > 0.95) bankLimit = 30.0;
		TargetBank = sign(headingToHAC)*range(0.0, 2.5*abs(headingToHAC), bankLimit);
		// check for transition to HDG phase
		if(radius < 1.1*HAC_TurnRadius) TAEMGuidanceMode = HDG;
		
		//sprintf_s(oapiDebugString(), 255, "ACQ: X: %f Y: %f Z: %f pst: %f courseToRwy: %f headingToHAC: %f TargetBank: %f TotalRange: %f", velocity.x, velocity.y, velocity.z, pst, courseToRwy, headingToHAC, TargetBank, TotalRange);
		//oapiWriteLog(oapiDebugString());
	}
	else {
		double rdot = -(velocity.x*(HAC_CENTER_X-TgtPos.x) + velocity.y*(HAC_CENTER_Y-TgtPos.y))/radius;
		double phi_ref = DEG*(velocity.x*velocity.x+velocity.y*velocity.y - rdot*rdot)/(G*HAC_TurnRadius);
		double rdotrf = -DEG*sqrt(velocity.x*velocity.x+velocity.y*velocity.y)*(R1+2.0*R2*turn_angle)/HAC_TurnRadius;
		TargetBank = YSGN*max(0, phi_ref + (radius-HAC_TurnRadius)*GR + (rdot-rdotrf)*GRDOT);
	}
	
	NZCommand = CalculateNZCommand(velocity, TotalRange, -TgtPos.z, DeltaT);

	//sprintf_s(oapiDebugString(), 255, "X: %f Y: %f TgtRadius: %f Radius: %f NZ: %f Bank: %f Angle: %f Range: %f", TgtPos.x-HAC_CENTER_X, TgtPos.y-HAC_CENTER_Y,
			  //HAC_TurnRadius, radius, NZCommand, TargetBank, turn_angle, TotalRange);
	//sprintf_s(oapiDebugString(), 255, "PX: %f PY: %f PZ: %f VX: %f VY: %f VZ: %f", TgtPos.x, TgtPos.y, TgtPos.z,
		//velocity.x, velocity.y, velocity.z);

	//double headingToRwy = atan2(TgtPos.y, -TgtPos.x)*DEG;
	//sprintf_s(oapiDebugString(), 255, "HTR: %f dis: %f AoA: %f", headingToRwy, abs(HAC_CENTER_Z-TgtPos.z), STS()->GetAOA()*DEG);
	//if(abs(headingToRwy)>=0.0 && abs(headingToRwy)<8.0 && abs(HAC_CENTER_X-TgtPos.x) < 5300.0/MPS2FPS) {
	if(TotalRange < (-HAC_CENTER_X + DR3)) {
		sprintf_s(oapiDebugString(), 255, "Starting PRFNL phase range: %f angle: %f", TotalRange, turn_angle);
		//oapiWriteLog("Starting PRFNL phase");
		oapiWriteLog(oapiDebugString());
		TAEMGuidanceMode = PRFNL;
	}
}

void AerojetDAP::CalculateTargetGlideslope(const VECTOR3& TgtPos, double DeltaT)
{
	double HeadingError = atan2(TgtPos.y, -TgtPos.x)*DEG;
	//sprintf_s(oapiDebugString(), 255, "TPos X: %f Y: %f Z: %f", TgtPos.x, TgtPos.y, TgtPos.z);
	//sprintf_s(oapiDebugString(), 255, "X: %f Y: %f Z: %f HeadingError: %f", TgtPos.x, TgtPos.y, TgtPos.z, HeadingError);
	TotalRange = sqrt(TgtPos.x*TgtPos.x + TgtPos.y*TgtPos.y);
	if(TAEMGuidanceMode == PRFNL) {
		if(true) {			
			VECTOR3 horz_airspeed, velocity;
			STS()->GetHorizonAirspeedVector(horz_airspeed);
			velocity = RotateVectorY(horz_airspeed, degRwyHeading);
			velocity = _V(velocity.z, velocity.x, -velocity.y);
			NZCommand = CalculateNZCommand(velocity, TotalRange, -TgtPos.z, DeltaT);
		}
		else {
			//TargetGlideslope = CalculateOGS(TgtPos);
		}
		//TargetHeading = RwyHeading-HeadingError*3.0;
		if(STS()->GetAltitude() < 10000.0/MPS2FPS) // test is actually more complicated than this
			TAEMGuidanceMode = OGS;
	}
	if(TAEMGuidanceMode == OGS) {
		//if(STS()->GetAltitude() <= 1750.0/MPS2FPS) {
		if(STS()->GetAltitude() <= 2000.0/MPS2FPS) {
			//InitiatePreflare();
			TAEMGuidanceMode = FLARE;
		}
		else {
			if(true) {
				VECTOR3 horz_airspeed, velocity;
				STS()->GetHorizonAirspeedVector(horz_airspeed);
				velocity = RotateVectorY(horz_airspeed, degRwyHeading);
				velocity = _V(velocity.z, velocity.x, -velocity.y);

				NZCommand = CalculateNZCommand(velocity, sqrt(TgtPos.x*TgtPos.x+TgtPos.y*TgtPos.y), -TgtPos.z, DeltaT);
			}
			else {
				//TargetGlideslope = CalculateOGS(TgtPos);
			}
		}
	}
	if(TAEMGuidanceMode == FLARE) {
		//TargetGlideslope = CalculatePreflareGlideslope(TgtPos);
		NZCommand = CalculatePreflareNZ(TgtPos, DeltaT);

		// check for transition to FLNFL
		if(STS()->GetAltitude()<80.0) {
			VECTOR3 horz_airspeed;
			STS()->GetHorizonAirspeedVector(horz_airspeed);
			double altThreshold = (horz_airspeed.y - 8.0/MPS2FPS)*6.25 + 30.0/MPS2FPS;
			if(STS()->GetAltitude()<30.0 || STS()->GetAltitude()<altThreshold) TAEMGuidanceMode = FNLFL;
		}
	}
}

double AerojetDAP::CalculatePrefinalBank(const VECTOR3& RwyPos)
{
	// NOTE: gains are in deg/ft, so multiply by conversion factor
	const double GY = 0.07*MPS2FPS;
	const double GYDOT = 0.7*MPS2FPS;

	VECTOR3 horz_airspeed, velocity;
	STS()->GetHorizonAirspeedVector(horz_airspeed);
	velocity = RotateVectorY(horz_airspeed, degRwyHeading);
	velocity = _V(velocity.z, velocity.x, -velocity.y); // convert to rwy-coordinate frame

	//sprintf_s(oapiDebugString(), 255, "PX: %f PY: %f PZ: %f VX: %f VY: %f VZ: %f TgtBank: %f", RwyPos.x, RwyPos.y, RwyPos.z,
		//velocity.x, velocity.y, velocity.z, GY*RwyPos.y + GYDOT*velocity.y);

	double bank = -GY*RwyPos.y - GYDOT*velocity.y;
	if(prfnlBankFader > 1.0) {
		double dBank = (bank-TargetBank)/prfnlBankFader;
		prfnlBankFader -= oapiGetSimStep();
		//sprintf_s(oapiDebugString(), 255, "Fader bank: %f Actual target: %f", TargetBank+dBank, bank);
		return TargetBank+dBank*oapiGetSimStep();
	}
	return bank;
}

double AerojetDAP::CalculateNZCommand(const VECTOR3& velocity, double predRange, double curAlt, double DeltaT) const
{
	/**
	 * Range above which linear altitude reference is used
	 */
	//const double CUBIC_ALT_REF_CUTOFF_RANGE = 256527.82/MPS2FPS;
	const double CUBIC_ALT_REF_CUTOFF_RANGE = 308109.5/MPS2FPS;
	/**
	 * Reference height corresponding to range == CUBIC_ALT_REF_CUTOFF_RANGE
	 */
	//const double CUBIC_ALT_REF_CUTOFF_HEIGHT = 78161.826/MPS2FPS;
	const double CUBIC_ALT_REF_CUTOFF_HEIGHT = 84821.29/MPS2FPS;
	//const double LINEAR_GLIDESLOPE = 0.1112666;
	const double LINEAR_GLIDESLOPE = 0.1125953;
	/**
	 * Constants for cubic altitude reference
	 */
	//const double CUBIC_C3 = -4.7714787e-7 * MPS2FPS;
	//const double CUBIC_C4 = -2.4291527e-13 * MPS2FPS*MPS2FPS;
	const double CUBIC_C3 = -0.3641168e-6 * MPS2FPS;
	const double CUBIC_C4 = -0.9481026e-13 * MPS2FPS*MPS2FPS;
	/**
	 * Gain for calculating filteredDeltaNZComm
	 */
	const double CQG = 0.5583958;

	double rangeToALI = predRange + X_AL_INTERCEPT; // positive in TAEM, negative on final approach
	double tgtAlt, tgtGs;
	if(rangeToALI > CUBIC_ALT_REF_CUTOFF_RANGE) {
		tgtAlt = CUBIC_ALT_REF_CUTOFF_HEIGHT + LINEAR_GLIDESLOPE*(rangeToALI-CUBIC_ALT_REF_CUTOFF_RANGE);
		tgtGs = -LINEAR_GLIDESLOPE;
	}
	else {
		tgtAlt = Y_AL_INTERCEPT - AL_GS*rangeToALI;
		if(rangeToALI > 0.0) {
			//sprintf_s(oapiDebugString(), 255, "Cubic alt profile");
			tgtAlt += rangeToALI*rangeToALI*(CUBIC_C3 + rangeToALI*CUBIC_C4);
			tgtGs = AL_GS - rangeToALI*(2*CUBIC_C3 + 3*rangeToALI*CUBIC_C4);
			tgtGs = range(AL_GS, tgtGs, -LINEAR_GLIDESLOPE);
		}
		else {
			//sprintf_s(oapiDebugString(), 255, "Final approach alt profile");
			// for the moment, assume we're on final
			tgtGs = AL_GS;
		}
	}

	// convert altitude/glideslope errors to NZ command
	double horzSpeed = sqrt(velocity.x*velocity.x + velocity.y*velocity.y);

	double gain = range(0.3, 2.0 - (7.0e-5)*(curAlt*MPS2FPS), 1.0);
	double refHDot = horzSpeed*tgtGs;
	double HDotErr = refHDot + velocity.z;

	//sprintf_s(oapiDebugString(), 255, "Gain: %f TgtAlt: %f refHDot: %f HDotErr: %f DNZC: %f", gain, tgtAlt, refHDot, HDotErr, gain*0.01*(HDotErr + 0.1*gain*(tgtAlt-curAlt)));

	double NZC;
	double deltaNZComm = gain*0.01*(HDotErr + 0.1*gain*(tgtAlt-curAlt));	

	if(TAEMGuidanceMode == OGS) {
		NZC = deltaNZComm;
	}
	else {		
		double limitedDeltaNZComm = deltaNZComm; // this should be changed eventually
		double filteredDeltaNZComm = range(-0.1, (limitedDeltaNZComm-NZCommand)*CQG, 0.1);
		if(bSecondStep) NZC = deltaNZComm;
		else NZC = NZCommand + filteredDeltaNZComm*DeltaT;
	}
	return range(-0.5, NZC, 0.5);
}

double AerojetDAP::CalculateSpeedbrakeCommand(double predRange, double DeltaT)
{
	double mach = STS()->GetMachNumber();
	if(mach > 10.0) return 0.0;
	else if(mach >= 3.2) return 81.0*min(1.0, (10.0-mach)); // ramp from 0% at M10 to 81% at M9
	else if(mach > 0.95) return 16.0*max(0.0, (mach-2.5)/0.7) + 65.0; // ramp from 81% at M3.2 to 65% at M2.5
	//if(STS()->GetMachNumber() > 0.95) return 65.0;

	// NOTE: this an older version of the speedbrake command algorithm
	// newer versions also include energy term
	const double MAX_SPEEDBRAKE = 98.6;
	const double LOWER_LIM = max(0.0, 65.0-650*(0.95-STS()->GetMachNumber()));
	const double UPPER_LIM = min(MAX_SPEEDBRAKE, 65.0+336*(0.95-STS()->GetMachNumber()));
	const double PBRCQ = 89971.082/MPS2FPS;
	const double QBC1 = 3.6086999E-4*MPS2FPS;
	const double QBC2 = -1.1613301E-3*MPS2FPS;

	// use KEAS below 9000 feet and qbar above 9000 feet
	// actual shuttle uses blended logic between 9000 and 14000 feet
	if(STS()->GetAltitude() > 9000.0/MPS2FPS) {
		double rangeToALI = predRange + X_AL_INTERCEPT; // positive in TAEM, negative on final approach
		double refQbar = 0.0;
		if(rangeToALI < PBRCQ) {
			refQbar = range(180.0, 285.0+QBC2*rangeToALI, 285.0);
		}
		else {
			refQbar = range(180.0, 180.0+QBC1*(rangeToALI-PBRCQ), 220.0);
		}
		//sprintf_s(oapiDebugString(), 255, "Ref QBar: %f QBar: %f Error: %f", refQbar, filteredQBar, refQbar-filteredQBar);
		double QBarError = refQbar-filteredQBar;

		double command = 65.0 - QBar_Speedbrake.Step(QBarError, DeltaT);
		//sprintf_s(oapiDebugString(), 255, "Ref QBar: %f QBar: %f Error: %f Command: %f", refQbar, filteredQBar, refQbar-filteredQBar, command);
		//return range(LOWER_LIM, 65.0 - QBar_Speedbrake.Step(QBarError, DeltaT), UPPER_LIM);
		return range(LOWER_LIM, command, UPPER_LIM);
	}
	else if(STS()->GetAltitude() > 3000.0/MPS2FPS) {
		return range(LOWER_LIM, 65.0 - Vel_Speedbrake.Step(300.0-STS()->GetKEAS(), DeltaT), UPPER_LIM);
	}
	else {
		// what should happen is speedbrake is set to constant value below 3000 feet, and then updated at 500 feet
		// for the moment, just used fixed value of 15%
		return 15.0;
	}
}

void AerojetDAP::InitiatePreflare()
{
	TAEMGuidanceMode = FLARE;
	degTargetGlideslope = STS()->GetPitch() - STS()->GetAOA();
	//TargetGlideslope = (STS()->GetPitch() - STS()->GetAOA())*DEG;
}

/*double AerojetDAP::CalculatePreflareGlideslope(const VECTOR3 &RwyPos) const
{
	/**
	 * Offset between IGS aimpoint and rwy threshold
	 *
	const double IGS_AIMPOINT = -1000.0/MPS2FPS;
	/**
	 * Target glideslope at end of flare
	 *
	const double TGT_IGS = 1.5*RAD;

	return 0.0;

	/*double curGlideslope = STS()->GetPitch() - STS()->GetAOA();
	double speed = STS()->GetAirspeed();
	VECTOR3 TgtPos = _V(RwyPos.x+IGS_AIMPOINT, RwyPos.y, RwyPos.z);
	double flareRate = DEG*(speed * ( cos(TGT_IGS)-cos(-curGlideslope)+tan(TGT_IGS)*(sin(TGT_IGS)-sin(-curGlideslope)) )) / (-TgtPos.z+TgtPos.x*tan(TGT_IGS));

	sprintf_s(oapiDebugString(), 255, "Flare Rate: %f GS: %f", flareRate, TargetGlideslope);
	oapiWriteLog(oapiDebugString());
	//sprintf_s(oapiDebugString(), 255, "TgtPos: %f %f %f", TgtPos.x, TgtPos.y, TgtPos.z);
	//oapiWriteLog(oapiDebugString());
	
	double DeltaT = oapiGetSimStep();
	return min(TargetGlideslope+(DeltaT*flareRate), -1.5);
}*/

double AerojetDAP::CalculatePreflareNZ(const VECTOR3 &RwyPos, double DeltaT)
{
	/**
	 * Target glideslope at end of flare
	 */
	const double TGT_IGS = 1.5*RAD;
	/**
	 * Radius of preflare circle
	 */
	const double PREFLARE_RADIUS = 30146.0/MPS2FPS;

	//return 0.0;

	double curGlideslope = STS()->GetPitch() - STS()->GetAOA();
	double speed = STS()->GetAirspeed();
	VECTOR3 TgtPos = _V(RwyPos.x-IGS_AIMPOINT, RwyPos.y, RwyPos.z);
	double flareRate = (speed * ( cos(TGT_IGS)-cos(-curGlideslope)+tan(TGT_IGS)*(sin(TGT_IGS)-sin(-curGlideslope)) )) / (-TgtPos.z+TgtPos.x*tan(TGT_IGS));
	flareRate = max(flareRate, 0.0);

	//sprintf_s(oapiDebugString(), 255, "Flare Rate: %f GS: %f", flareRate, NZCommand);
	//oapiWriteLog(oapiDebugString());
	//sprintf_s(oapiDebugString(), 255, "TgtPos: %f %f %f", TgtPos.x, TgtPos.y, TgtPos.z);
	//oapiWriteLog(oapiDebugString());
	
	degTargetGlideslope = min(degTargetGlideslope+(DeltaT*DEG*flareRate), -1.5);
	double NZ = (PREFLARE_RADIUS*flareRate*flareRate)/G; // NZ required to accelerate in circle
	NZ += (degTargetGlideslope-DEG*curGlideslope)*0.2; // correct for error in glideslope
	return NZ;
}

void AerojetDAP::InitializeRunwayData()
{
	VECTOR3 end1, end2;
	/*if(!SEC) {
		end1 = GetPositionVector(hEarth, vLandingSites[SITE_ID].radPriLat, vLandingSites[SITE_ID].radPriLong, oapiGetSize(hEarth));
		end2 = GetPositionVector(hEarth, vLandingSites[SITE_ID].radSecLat, vLandingSites[SITE_ID].radSecLong, oapiGetSize(hEarth));
		degRwyHeading = vLandingSites[SITE_ID].degPriHeading;
	}
	else {			
		end1 = GetPositionVector(hEarth, vLandingSites[SITE_ID].radSecLat, vLandingSites[SITE_ID].radSecLong, oapiGetSize(hEarth));
		end2 = GetPositionVector(hEarth, vLandingSites[SITE_ID].radPriLat, vLandingSites[SITE_ID].radPriLong, oapiGetSize(hEarth));
		degRwyHeading = vLandingSites[SITE_ID].degPriHeading+180.0;
		if(degRwyHeading >= 360.0) degRwyHeading-=360.0;
	}*/
	double radLat, radLong;
	vLandingSites[SITE_ID].GetRwyPosition(!SEC, radLat, radLong); // get lat/long for end we want to land on
	end1 = GetPositionVector(hEarth, radLat, radLong, oapiGetSize(hEarth));
	vLandingSites[SITE_ID].GetRwyPosition(SEC, radLat, radLong); // get lat/long for opposite end of runway
	end2 = GetPositionVector(hEarth, radLat, radLong, oapiGetSize(hEarth));
	degRwyHeading = vLandingSites[SITE_ID].GetRwyHeading(!SEC);

	RwyPos = end1; // store un-normalized value
	end1 = end1/length(end1);
	end2 = end2/length(end2);
	VECTOR3 x = end2-end1;
	x = x/length(x);
	VECTOR3 y = crossp(end1, x);
	RwyRotMatrix = _M(x.x, x.y, x.z,
		y.x, y.y, y.z,
		-end1.x, -end1.y, -end1.z);
	RwyPos = mul(RwyRotMatrix, RwyPos);
	
	// calculate values used by entry guidance
	const double YSGN = (HACSide==L) ? -1.0 : 1.0;
	const double HAC_CENTER_Y = YSGN * FINAL_RADIUS;
	HAC_Center = tmul(RwyRotMatrix, RwyPos + _V(HAC_CENTER_X, HAC_CENTER_Y, 0.0));
	double hacRad;
	oapiLocalToEqu(hEarth, HAC_Center, &HAC_Long, &HAC_Lat, &hacRad);
}

VECTOR3 AerojetDAP::GetRunwayRelPos() const
{
	double dLat, dLong, dRad;
	STS()->GetEquPos(dLong, dLat, dRad);
	VECTOR3 v = GetPositionVector(hEarth, dLat, dLong, dRad);
	return mul(RwyRotMatrix, v)-RwyPos;
}

void AerojetDAP::CalculateRangeAndDELAZ(double& Range, double& delaz)
{
	const double YSGN = (HACSide==L) ? -1.0 : 1.0;
	
	double lng, lat, rad;
	double tgtLong, tgtLat;
	vLandingSites[SITE_ID].GetRwyPosition(!SEC, tgtLat, tgtLong);
	STS()->GetEquPos(lng, lat, rad);
	VECTOR3 shuttlePos = GetPositionVector(hEarth, lat, lng, rad);
	
	VECTOR3 v;
	STS()->GetHorizonAirspeedVector(v);
	double actualHeading = atan2(v.x, v.z);

	double cosGreatCircle = dotp(shuttlePos/length(shuttlePos), HAC_Center/length(HAC_Center)); // CTHVC
	double sinGreatCircle = sqrt(1 - cosGreatCircle*cosGreatCircle);
	double SINB = HAC_TurnRadius/oapiGetSize(hEarth);
	double CTVWP1 = cosGreatCircle + 0.5*cosGreatCircle*SINB*SINB;
	double SBARCR = SINB/sinGreatCircle;
	double temp = range(-1, CTVWP1*SBARCR, 1);
	double A2 = acos(temp);
	temp = range(-1, CTVWP1, 1);
	double rangeToWP1 = acos(temp)*oapiGetSize(hEarth);
	temp = range(-1, SBARCR, 1);
	double T8 = asin(temp);
	//double BARWP1 = actualHeading - YSGN*T8;
	double headingToHACCenter = atan2(sin(HAC_Long-lng)*cos(HAC_Lat), cos(lat)*sin(HAC_Lat) - sin(lat)*cos(HAC_Lat)*cos(HAC_Long-lng));
	double headingToWP1 = headingToHACCenter - YSGN*T8; // this is not quite the same as in documents, but should work
	double A3 = 0.5*PI - A2 + YSGN*(vLandingSites[SITE_ID].GetRwyHeading(!SEC)*RAD - headingToHACCenter);
	if(A3 < 0.0) A3 += 2*PI;
	double HAC_TurnAngle = A3*DEG;
	HAC_TurnRadius = FINAL_RADIUS + (R1 + R2*HAC_TurnAngle)*HAC_TurnAngle;
	double HAC_range = FINAL_RADIUS*HAC_TurnAngle + 0.5*R1*HAC_TurnAngle*HAC_TurnAngle + (1/3)*R2*HAC_TurnAngle*HAC_TurnAngle*HAC_TurnAngle;
	Range = rangeToWP1 + HAC_range*RAD - HAC_CENTER_X;

	delaz = DEG*(actualHeading-headingToWP1);
}

/*double AerojetDAP::CalculateRangeToRunway() const
{
	double lng, lat, rad;
	double tgtLong, tgtLat;
	STS()->GetEquPos(lng, lat, rad);
	vLandingSites[SITE_ID].GetRwyPosition(!SEC, tgtLat, tgtLong);
	double angle = oapiOrthodome(lng, lat, tgtLong, tgtLat);
	return abs(angle*oapiGetSize(hEarth));
}

double AerojetDAP::CalculateDELAZ() const
{
	double lng, lat, rad;
	double tgtLong, tgtLat;
	STS()->GetEquPos(lng, lat, rad);
	vLandingSites[SITE_ID].GetRwyPosition(!SEC, tgtLat, tgtLong);
	double tgtHeading = atan2(sin(tgtLong-lng)*cos(tgtLat), cos(lat)*sin(tgtLat) - sin(lat)*cos(tgtLat)*cos(tgtLong-lng));
	VECTOR3 v;
	STS()->GetHorizonAirspeedVector(v);
	double actualHeading = atan2(v.x, v.z);
	sprintf_s(oapiDebugString(), 255, "Heading: %f Tgt: %f", DEG*actualHeading, DEG*tgtHeading);
	return DEG*(tgtHeading-actualHeading);
}*/

void AerojetDAP::LoadLandingSiteList()
{
	// for reference, landing site tables can be found in Ascent Checklists
	vLandingSites.push_back(LandingSiteData(28.632944*RAD, -80.706035*RAD, 28.5970420*RAD, -80.6826540*RAD, 150.2505, "KSC15", "KSC33"));
	for(int i=2;i<=44;i++) vLandingSites.push_back(LandingSiteData(0.0, 0.0, 0.0, 0.0, 0.0, "NUL00", "NUL00")); // add filler data so landing site indices are correct
	vLandingSites.push_back(LandingSiteData(34.9173476*RAD, -117.8595079*RAD, 34.8941050*RAD, -117.9051869*RAD, 238.16, "EDW22", "EDW04"));
}

};
