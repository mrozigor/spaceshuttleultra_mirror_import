#include "AerojetDAP.h"
#include "../Atlantis.h"
#include "RHC_SOP.h"
#include "RPTA_SOP.h"
#include "SBTC_SOP.h"
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
		skp->Line(Round(line_pos.x), Round(line_pos.y),
			Round(line_seg1_end.x), Round(line_seg1_end.y));
		VECTOR3 line_seg2_start = line_seg1_end + line_dir_vector*0.3*hps->Scale;
		VECTOR3 line_seg2_end = line_pos + line_dir_vector*2*hps->Scale;
		skp->Line(Round(line_seg2_start.x), Round(line_seg2_start.y), Round(line_seg2_end.x), Round(line_seg2_end.y));
		VECTOR3 line_seg3_start = line_end - line_dir_vector*2*hps->Scale;
		VECTOR3 line_seg3_end = line_seg3_start + line_dir_vector*0.85*hps->Scale;
		skp->Line(Round(line_seg3_start.x), Round(line_seg3_start.y),
			Round(line_seg3_end.x), Round(line_seg3_end.y));
		VECTOR3 line_seg4_start = line_seg3_end + line_dir_vector*0.3*hps->Scale;
		skp->Line(Round(line_seg4_start.x), Round(line_seg4_start.y),
			Round(line_end.x), Round(line_end.y));
	}
	else { //  2 solid lines
		VECTOR3 line_seg1_end = line_pos + line_dir_vector*2*hps->Scale;
		skp->Line(Round(line_pos.x), Round(line_pos.y), Round(line_seg1_end.x), Round(line_seg1_end.y));
		VECTOR3 line_seg2_start = line_end - line_dir_vector*2*hps->Scale;		
		skp->Line(Round(line_seg2_start.x), Round(line_seg2_start.y), Round(line_end.x), Round(line_end.y));
	}
	// draw lines pointing toward horizon
	if(ladderPitch > 0) {
		line_rot_vector = RotateVectorZ(_V(0, 1, 0), orbiterBank);
		VECTOR3 left_line_end = line_pos + line_rot_vector*0.5*hps->Scale;
		skp->Line(Round(left_line_end.x), Round(left_line_end.y), Round(line_pos.x), Round(line_pos.y));
		VECTOR3 right_line_end = line_end + line_rot_vector*0.5*hps->Scale;
		skp->Line(Round(right_line_end.x), Round(right_line_end.y), Round(line_end.x), Round(line_end.y));
	}
	else if(ladderPitch < 0) {
		line_rot_vector = RotateVectorZ(_V(0, -1, 0), orbiterBank);
		VECTOR3 left_line_end = line_pos + line_rot_vector*0.5*hps->Scale;
		skp->Line(Round(left_line_end.x), Round(left_line_end.y), Round(line_pos.x), Round(line_pos.y));
		VECTOR3 right_line_end = line_end + line_rot_vector*0.5*hps->Scale;
		skp->Line(Round(right_line_end.x), Round(right_line_end.y), Round(line_end.x), Round(line_end.y));
	}

	// print angle
	if(ladderPitch != 0) {
		sprintf_s(pszBuf, 20, "%d", ladderPitch);
		//skp->Text(static_cast<int>(line_end.x)+3, static_cast<int>(line_end.y)-textHeight/2, pszBuf, strlen(pszBuf));
		int textWidth = skp->GetTextWidth(pszBuf);
		VECTOR3 textPos = line_end - line_dir_vector*(1+textWidth) + line_rot_vector*(1+textHeight);
		//VECTOR3 textEnd = line_end - line_rot_vector*2;
		skp->Text(Round(textPos.x), Round(textPos.y), pszBuf, strlen(pszBuf));
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
	skp->Line(Round(pt1.x), Round(pt1.y), Round(pt2.x), Round(pt2.y));
	skp->Line(Round(pt1.x), Round(pt1.y), Round(pt3.x), Round(pt3.y));
	skp->Line(Round(pt3.x), Round(pt3.y), Round(pt2.x), Round(pt2.y));
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
bFirstStep(true), bSecondStep(false), bWOW(false), bWONG(false), OrbiterMass(93000.0),
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
EntryGuidanceMode(PREENTRY),
referenceDrag23(19.38/MPS2FPS), constDragStartVel(VQ),
tgtAltAveraging(5.0), vspeedAveraging(10.0),  vaccAveraging(20.0), vspeedUpdateSimT(-1.0), vaccUpdateSimT(-1.0),
tgtBankSign(1.0), performedFirstRollReversal(false),
TAEMGuidanceMode(ACQ), HACDirection(OVHD), HACSide(L), SBControlLogic(NOM),
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

	// default nominal aimpoint
	OGS_AIMPOINT = OGS_AIMPOINT_NOM;
	X_AL_INTERCEPT = OGS_AIMPOINT + Y_AL_INTERCEPT/AL_GS;
	HAC_CENTER_X = OGS_AIMPOINT - 33020.0/MPS2FPS;

	TimeToHAC = 0;
	NZErr = 0;
	DistanceToHACCenter = 0;

	ET_MachHistory[0] = 0;
	ET_MachHistory[1] = 0;
	ET_MachHistory[2] = 0;
	ET_MachHistory[3] = 0;
	ET_MachHistory[4] = 0;
	ET_RangeHistory[0] = 0;
	ET_RangeHistory[1] = 0;
	ET_RangeHistory[2] = 0;
	ET_RangeHistory[3] = 0;
	ET_RangeHistory[4] = 0;
	ET_AltitudeHistory[0] = 0;
	ET_AltitudeHistory[1] = 0;
	ET_AltitudeHistory[2] = 0;
	ET_AltitudeHistory[3] = 0;
	ET_AltitudeHistory[4] = 0;
	ET_History_updatetime = 0;
	ET_Mach = 0;
	ETVS_Range = 0;
	ETVS_Altitude = 0;

	dclt_sw_on[0] = false;
	dclt_sw_on[1] = false;
	declutter_level[0] = 0;
	declutter_level[1] = 0;

	AutoFCSPitch = true;
	AutoFCSRoll = true;

	SpeedbrakeState = AUTO;
	AutoSpeedbrakeCommand = 0.0;

	tCSS = -1;
	tGear = -1;
}

AerojetDAP::~AerojetDAP()
{
	delete dTable;
}

void AerojetDAP::Realize()
{
	DiscreteBundle* pBundle=STS()->BundleManager()->CreateBundle("AEROSURFACE_CMD", 16);
	//LeftElevonCommand.Connect(pBundle, 0);
	//RightElevonCommand.Connect(pBundle, 1);
	ElevonCommand.Connect(pBundle, 0);
	AileronCommand.Connect(pBundle, 1);
	RudderCommand.Connect(pBundle, 2);

	pBundle=STS()->BundleManager()->CreateBundle("THRUSTER_CMD", 16);
	for(unsigned int i=0;i<3;i++) {
		ThrusterCommands[i].Connect(pBundle, i);
	}

	pBundle = BundleManager()->CreateBundle( "CSS_CONTROLS", 16 );
	CDRPitchAuto.Connect( pBundle, 0 );
	CDRPitchAutoLT.Connect( pBundle, 1 );
	CDRPitchCSS.Connect( pBundle, 2 );
	CDRPitchCSSLT.Connect( pBundle, 3 );
	CDRRollYawAuto.Connect( pBundle, 4 );
	CDRRollYawAutoLT.Connect( pBundle, 5 );
	CDRRollYawCSS.Connect( pBundle, 6 );
	CDRRollYawCSSLT.Connect( pBundle, 7 );
	PLTPitchAuto.Connect( pBundle, 8 );
	PLTPitchAutoLT.Connect( pBundle, 9 );
	PLTPitchCSS.Connect( pBundle, 10 );
	PLTPitchCSSLT.Connect( pBundle, 11 );
	PLTRollYawAuto.Connect( pBundle, 12 );
	PLTRollYawAutoLT.Connect( pBundle, 13 );
	PLTRollYawCSS.Connect( pBundle, 14 );
	PLTRollYawCSSLT.Connect( pBundle, 15 );

	pBundle=STS()->BundleManager()->CreateBundle("SPDBKTHROT_CONTROLS", 16);
	CDR_SPDBK_THROT.Connect( pBundle, 0 );// HACK should first go to switch RM
	CDR_SPDBK_THROT_AUTO_LT.Connect( pBundle, 1 );
	CDR_SPDBK_THROT_MAN_LT.Connect( pBundle, 2 );
	PLT_SPDBK_THROT.Connect( pBundle, 3 );// HACK should first go to switch RM
	PLT_SPDBK_THROT_AUTO_LT.Connect( pBundle, 4 );
	PLT_SPDBK_THROT_MAN_LT.Connect( pBundle, 5 );

	pBundle = STS()->BundleManager()->CreateBundle( "HUD_CDR", 16 );
	HUDPower[0].Connect( pBundle, 0 );
	pHUDDCLT[0].Connect( pBundle, 1 );

	pBundle = STS()->BundleManager()->CreateBundle( "HUD_PLT", 16 );
	HUDPower[1].Connect( pBundle, 0 );
	pHUDDCLT[1].Connect( pBundle, 1 );

	pRHC_SOP = static_cast<RHC_SOP*> (FindSoftware( "RHC_SOP" ));
	
	pRPTA_SOP = static_cast<RPTA_SOP*> (FindSoftware( "RPTA_SOP" ));

	pSBTC_SOP = static_cast<SBTC_SOP*> (FindSoftware( "SBTC_SOP" ));
	
	hEarth = STS()->GetGravityRef();
	InitializeRunwayData();
}

void AerojetDAP::OnPreStep(double SimT, double DeltaT, double MJD)
{
	if(GetMajorMode() != 304 && GetMajorMode() != 305) return; // AerojetDAP software runs in MMs 301-303 as well to drive SPEC 50 HORIZ SIT display, but does not perform any control operations

	// on first step, Orbiter gives some incorrect data, so ignore this step
	if(bFirstStep) {
		filteredQBar = STS()->GetDynPressure()*PA2PSF;
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

	// check if AUTO or CSS
	if (AutoFCSPitch == true)
	{
		if (CDRPitchCSS.IsSet() || PLTPitchCSS.IsSet())
		{
			// to CSS
			AutoFCSPitch = false;
			CDRPitchAutoLT.ResetLine();
			PLTPitchAutoLT.ResetLine();
			CDRPitchCSSLT.SetLine();
			PLTPitchCSSLT.SetLine();
		}
	}
	else
	{
		if (CDRPitchAuto.IsSet() || PLTPitchAuto.IsSet())
		{
			// to AUTO
			AutoFCSPitch = true;
			CDRPitchAutoLT.SetLine();
			PLTPitchAutoLT.SetLine();
			CDRPitchCSSLT.ResetLine();
			PLTPitchCSSLT.ResetLine();
		}
	}
	if (AutoFCSRoll == true)
	{
		if (CDRRollYawCSS.IsSet() || PLTRollYawCSS.IsSet())
		{
			// to CSS
			AutoFCSRoll = false;
			CDRRollYawAutoLT.ResetLine();
			PLTRollYawAutoLT.ResetLine();
			CDRRollYawCSSLT.SetLine();
			PLTRollYawCSSLT.SetLine();
		}
	}
	else
	{
		if (CDRRollYawAuto.IsSet() || PLTRollYawAuto.IsSet())
		{
			// to AUTO
			AutoFCSRoll = true;
			CDRRollYawAutoLT.SetLine();
			PLTRollYawAutoLT.SetLine();
			CDRRollYawCSSLT.ResetLine();
			PLTRollYawCSSLT.ResetLine();
		}
	}
	// downmode to CSS if RHC is out of detent
	if (AutoFCSPitch == true)
	{
		if (pRHC_SOP->GetPitchManTakeOver() == true)
		{
			// to CSS
			AutoFCSPitch = false;
			CDRPitchAutoLT.ResetLine();
			PLTPitchAutoLT.ResetLine();
			CDRPitchCSSLT.SetLine();
			PLTPitchCSSLT.SetLine();
		}
	}
	
	if (AutoFCSRoll == true)
	{
		if (pRHC_SOP->GetRollManTakeOver() == true)
		{
			// to CSS
			AutoFCSRoll = false;
			CDRRollYawAutoLT.ResetLine();
			PLTRollYawAutoLT.ResetLine();
			CDRRollYawCSSLT.SetLine();
			PLTRollYawCSSLT.SetLine();
		}
	}

	//double distToRwy, delaz; // only used in MM304
	switch(GetMajorMode()) {
	case 304:
		{
		UpdateOrbiterData();
		GetAttitudeData(DeltaT);

		CheckControlActivation();

		if(STS()->GetMachNumber() < 2.5) SetMajorMode(305);
		
		// control yaw regardless of AUTO/CSS mode
		//if(ThrustersActive[YAW])
			//ThrusterCommands[YAW].SetLine(GetThrusterCommand(YAW));

		// PITCH channel
		double targetAOA = CalculateTargetAOA(STS()->GetMachNumber());
		if (AutoFCSPitch == true) {
			const double MAX_PITCH_RATE = 0.5;
			degTargetRates.data[PITCH] = range(-MAX_PITCH_RATE, 0.25*(targetAOA-degCurrentAttitude.data[PITCH]), MAX_PITCH_RATE);
		}
		else {
			degTargetRates.data[PITCH] = CSSPitchInput(DeltaT);
		}

		// ROLL/YAW channel
		double tgtBank = CalculateTargetBank(STS()->GetMachNumber(), targetAOA, DeltaT, SimT);
		double tgtBankRate = 0.0;
		if (AutoFCSRoll == true)
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

		if ((AutoFCSRoll == false) && (AerosurfacesActive[YAW] == true))
		{
			RudderCommand.SetLine( (float)(pRPTA_SOP->GetYawCommand() / 22.8) );
		}

		// save data for displays
		ET_Mach = STS()->GetAirspeed() * MPS2FPS * 0.001;
		ETVS_Range = GetRangeToRunway();
		ETVS_Altitude = STS()->GetAltitude() * MPS2FPS;
		if (SimT >= ET_History_updatetime)
		{
			if (ET_Mach > 14) ET_History_updatetime = SimT + 28.8;// ET1, ET2
			else ET_History_updatetime = SimT + 15.36;// ET3, ET4, ET5
			memmove( ET_MachHistory + 1, ET_MachHistory, sizeof(double) * 4 );
			ET_MachHistory[0] = ET_Mach;
			memmove( ET_RangeHistory + 1, ET_RangeHistory, sizeof(double) * 4 );
			ET_RangeHistory[0] = ETVS_Range;
			memmove( ET_AltitudeHistory + 1, ET_AltitudeHistory, sizeof(double) * 4 );
			ET_AltitudeHistory[0] = ETVS_Altitude;
		}
		break;
		}
	case 305:
		if(bWONG) {
			//oapiWriteLog("WONG");
			double airspeed=STS()->GetAirspeed();
			// load relief
			ElevonCommand.SetLine(static_cast<float>(10.0/33.0)); // elevons should be 10 deg down
			AileronCommand.SetLine(0.0f);
			if ((airspeed * MPS2KTS) > 100) RudderCommand.SetLine( (float)(pRPTA_SOP->GetYawCommand() / 22.8) );// rudder available above 100 KGS
			else RudderCommand.SetLine(0.0f);
			//Nosewheel steering
			double steerforce = min( 120000, 120000 * (airspeed / 50.0) ) * (pRPTA_SOP->GetYawCommand() / 22.8);
			STS()->AddForce(_V(steerforce, 0, 0), _V(0, 0, 14.9));
		}
		else {
			//oapiWriteLog("No WONG");

			UpdateOrbiterData();
			GetAttitudeData(DeltaT);

			CheckControlActivation();

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

			double NZSteadyState = cos(STS()->GetPitch())/cos(STS()->GetBank());
			NZErr = NZCommand+NZSteadyState-averageNZ;
			if (AutoFCSPitch == true)
			{
				degTargetRates.data[PITCH] = range(-0.5, 5.0*NZErr, 0.5);
			}
			else
			{
				degTargetRates.data[PITCH] = CSSPitchInput(DeltaT);
			}
			//degTargetRates.data[ROLL] = CSSRollInput(DeltaT);
			//double tgtBankRate = CSSRollInput(DeltaT);
			double tgtBankRate;
			if (AutoFCSRoll == true)
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

			if (AutoFCSRoll == false)
			{
				RudderCommand.SetLine( (float)range( -1.0, rudderPos + (pRPTA_SOP->GetYawCommand() / 22.8), 1.0 ) );
			}

			// check for weight-on-weels
			if(STS()->GroundContact()) {
				bWOW = true;
				// use rudder to steer shuttle after touchdown
				//RudderCommand.SetLine( (float)(pRPTA_SOP->GetYawCommand() / 22.8) );
				// check for weight-on-nose-gear
				if(STS()->GetPitch() < -3.0*RAD) {
					bWONG = true;
					// set default positions of control surfaces to zero
					STS()->SetControlSurfaceLevel(AIRCTRL_RUDDERTRIM, 0.0);
					STS()->SetControlSurfaceLevel(AIRCTRL_RUDDER, 0.0);
					STS()->SetControlSurfaceLevel(AIRCTRL_ELEVATOR, 0.0);
					STS()->SetControlSurfaceLevel(AIRCTRL_AILERON, 0.0);
				}
			}

			// save data for displays
			ETVS_Range = GetRangeToRunway();
			ETVS_Altitude = STS()->GetAltitude() * MPS2FPS;
		}
		break;
	}

	for (int i = 0; i < 2; i++)
	{
		if (HUDPower[i].IsSet() && pHUDDCLT[i].IsSet())
		{
			if (!dclt_sw_on[i])
			{
				dclt_sw_on[i] = true;
				declutter_level[i]++;
				if (declutter_level[i] == 4) declutter_level[i] = 0;
			}
		}
		else dclt_sw_on[i] = false;
	}

	if (tCSS == -1)
	{
		if (!GetAutoPitchState() && !GetAutoRollYawState()) tCSS = DeltaT;
	}
	else
	{
		if (GetAutoPitchState() || GetAutoRollYawState()) tCSS = -1;
		else tCSS += DeltaT;
	}

	if (tGear == -1)
	{
		if (STS()->GetGearState() == AnimState::OPEN) tGear = DeltaT;
	}
	else tGear += DeltaT;
	
	bFirstStep = false;
	bSecondStep = false;
}

bool AerojetDAP::OnMajorModeChange(unsigned int newMajorMode)
{
	if(newMajorMode == 304 || newMajorMode == 305) {
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
			Roll_AileronRoll.SetGains(0.05, 0.00, 0.01);

			if (AutoFCSPitch == false)
			{
				CDRPitchAutoLT.ResetLine();
				PLTPitchAutoLT.ResetLine();
				CDRPitchCSSLT.SetLine();
				PLTPitchCSSLT.SetLine();
			}
			else
			{
				CDRPitchAutoLT.SetLine();
				PLTPitchAutoLT.SetLine();
				CDRPitchCSSLT.ResetLine();
				PLTPitchCSSLT.ResetLine();
			}
			if (AutoFCSRoll == false)
			{
				CDRRollYawAutoLT.ResetLine();
				PLTRollYawAutoLT.ResetLine();
				CDRRollYawCSSLT.SetLine();
				PLTRollYawCSSLT.SetLine();
			}
			else
			{
				CDRRollYawAutoLT.SetLine();
				PLTRollYawAutoLT.SetLine();
				CDRRollYawCSSLT.ResetLine();
				PLTRollYawCSSLT.ResetLine();
			}

			if (SpeedbrakeState == MAN_CDR)
			{
				CDR_SPDBK_THROT_AUTO_LT.ResetLine();
				PLT_SPDBK_THROT_AUTO_LT.ResetLine();
				CDR_SPDBK_THROT_MAN_LT.SetLine();
				PLT_SPDBK_THROT_MAN_LT.ResetLine();
			}
			else if (SpeedbrakeState == MAN_PLT)
			{
				CDR_SPDBK_THROT_AUTO_LT.ResetLine();
				PLT_SPDBK_THROT_AUTO_LT.ResetLine();
				CDR_SPDBK_THROT_MAN_LT.ResetLine();
				PLT_SPDBK_THROT_MAN_LT.SetLine();
			}
			else
			{
				CDR_SPDBK_THROT_AUTO_LT.SetLine();
				PLT_SPDBK_THROT_AUTO_LT.SetLine();
				CDR_SPDBK_THROT_MAN_LT.ResetLine();
				PLT_SPDBK_THROT_MAN_LT.ResetLine();
			}
		}
		else
		{
			// when entering MM304 from MM303 init FCS to AUTO, otherwise it's a scenario start so leave it to the scenario data
			if (GetMajorMode() == 303)
			{
				AutoFCSPitch = true;
				CDRPitchAutoLT.SetLine();
				PLTPitchAutoLT.SetLine();
				CDRPitchCSSLT.ResetLine();
				PLTPitchCSSLT.ResetLine();
				AutoFCSRoll = true;
				CDRRollYawAutoLT.SetLine();
				PLTRollYawAutoLT.SetLine();
				CDRRollYawCSSLT.ResetLine();
				PLTRollYawCSSLT.ResetLine();

				SpeedbrakeState = AUTO;
				CDR_SPDBK_THROT_AUTO_LT.SetLine();
				PLT_SPDBK_THROT_AUTO_LT.SetLine();
				CDR_SPDBK_THROT_MAN_LT.ResetLine();
				PLT_SPDBK_THROT_MAN_LT.ResetLine();
			}
			else
			{
				if (AutoFCSPitch == false)
				{
					CDRPitchAutoLT.ResetLine();
					PLTPitchAutoLT.ResetLine();
					CDRPitchCSSLT.SetLine();
					PLTPitchCSSLT.SetLine();
				}
				else
				{
					CDRPitchAutoLT.SetLine();
					PLTPitchAutoLT.SetLine();
					CDRPitchCSSLT.ResetLine();
					PLTPitchCSSLT.ResetLine();
				}
				if (AutoFCSRoll == false)
				{
					CDRRollYawAutoLT.ResetLine();
					PLTRollYawAutoLT.ResetLine();
					CDRRollYawCSSLT.SetLine();
					PLTRollYawCSSLT.SetLine();
				}
				else
				{
					CDRRollYawAutoLT.SetLine();
					PLTRollYawAutoLT.SetLine();
					CDRRollYawCSSLT.ResetLine();
					PLTRollYawCSSLT.ResetLine();
				}

				if (SpeedbrakeState == MAN_CDR)
				{
					CDR_SPDBK_THROT_AUTO_LT.ResetLine();
					PLT_SPDBK_THROT_AUTO_LT.ResetLine();
					CDR_SPDBK_THROT_MAN_LT.SetLine();
					PLT_SPDBK_THROT_MAN_LT.ResetLine();
				}
				else if (SpeedbrakeState == MAN_PLT)
				{
					CDR_SPDBK_THROT_AUTO_LT.ResetLine();
					PLT_SPDBK_THROT_AUTO_LT.ResetLine();
					CDR_SPDBK_THROT_MAN_LT.ResetLine();
					PLT_SPDBK_THROT_MAN_LT.SetLine();
				}
				else
				{
					CDR_SPDBK_THROT_AUTO_LT.SetLine();
					PLT_SPDBK_THROT_AUTO_LT.SetLine();
					CDR_SPDBK_THROT_MAN_LT.ResetLine();
					PLT_SPDBK_THROT_MAN_LT.ResetLine();
				}
			}
		}
		return true;
	}
	else if(newMajorMode == 301 || newMajorMode == 302 || newMajorMode == 303) { // SPEC 50 (HORIZ SIT) display can be used, so allow AerojetDAP software to run
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
			HACDirection = OVHD;// reset to overhead
			SelectHAC();
			return true;
		}
		else if(item == 4) {
			SEC = true;
			InitializeRunwayData();
			HACDirection = OVHD;// reset to overhead
			SelectHAC();
			return true;
		}
		else if(item == 6) {
			if(TAEMGuidanceMode <= ACQ) { // if we are already in HDG mode (on HAC), it's too late to change from OVHD to STRT
				if(HACDirection == OVHD) HACDirection = STRT;
				else HACDirection = OVHD;
				SelectHAC();
			}
		}
		else if (item == 8)
		{
			if (GetApproachAndLandState() == false)// valid until A/L
			{
				if (OGS_AIMPOINT == OGS_AIMPOINT_NOM) OGS_AIMPOINT = OGS_AIMPOINT_CLOSE;
				else OGS_AIMPOINT = OGS_AIMPOINT_NOM;

				X_AL_INTERCEPT = OGS_AIMPOINT + Y_AL_INTERCEPT/AL_GS;
				HAC_CENTER_X = OGS_AIMPOINT - 33020.0/MPS2FPS;
			}
			return true;
		}
		else if (item == 39)
		{
			if (GetApproachAndLandState() == false)// valid until A/L
			{
				if (SBControlLogic == NOM) SBControlLogic = SHORT;
				else if (SBControlLogic == SHORT) SBControlLogic = ELS;
				else SBControlLogic = NOM;
			}
			return true;
		}
		else if(item == 41) {
			int nNew;
			sscanf_s(Data, "%d", &nNew);
			if(nNew>=0 && nNew<=static_cast<int>(vLandingSites.size())) {
				SITE_ID = nNew-1;
				SEC = false;// reset to PRI
				HACDirection = OVHD;// reset to overhead
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
	else if (spec == dps::MODE_UNDEFINED)
	{
		if (GetMajorMode() == 304)
		{
			// MM304
			if (ET_Mach > 17) PaintENTRYTRAJ1Display( pMDU );// EntryTraj1 (24.5kfps-17kfps / 3800nm-800nm)
			else if (ET_Mach > 14) PaintENTRYTRAJ2Display( pMDU );// EntryTraj2 (17kfps-14kfps / 1300nm-425nm)
			else if (ET_Mach > 10.5) PaintENTRYTRAJ3Display( pMDU );// EntryTraj3 (14kfps-10.5kfps / 800nm-315nm)
			else if ((ET_Mach * ETVS_Altitude) > 750000) PaintENTRYTRAJ4Display( pMDU );// EntryTraj4 (1.8Mft-750kft(10kfps-6.5kfps) / 480nm-145nm)
			else PaintENTRYTRAJ5Display( pMDU );// EntryTraj5 (750kft-200kft(6.5kfps-2.5kfps) / 220nm-55nm)
			return true;
		}
		else
		{
			// MM305
			if ((ETVS_Altitude > 30000) || (ETVS_Range > 25)) PaintVERTSIT1Display( pMDU );// 100kft-30kft / 70nm-10nm
			else PaintVERTSIT2Display( pMDU );// 30kft-8kft / 25nm-5nm
		}
		return true;
	}
	return false;
}

bool AerojetDAP::OnDrawHUD(const HUDPAINTSPEC* hps, oapi::Sketchpad* skp) const
{
	// if in PLT seat use PLT settings, otherwise use CDR
	bool power = HUDPower[0].IsSet();
	int dclt = declutter_level[0];
	if (STS()->VCMode == 1)// VC_PLT
	{
		power = HUDPower[1].IsSet();
		dclt = declutter_level[1];
	}

	if ((GetMajorMode() == 305) && power)
	{
		// draw cross at center
		skp->Line(hps->CX-5, hps->CY, hps->CX+5, hps->CY);
		skp->Line(hps->CX, hps->CY-5, hps->CX, hps->CY+5);

		if (dclt != 3)
		{
			char cbuf[255];
			double dPitch = STS()->GetPitch()*DEG;
			double bank = STS()->GetBank()*DEG;
			VECTOR3 Velocity;
			STS()->GetHorizonAirspeedVector(Velocity);

			// show gear deployment status
			if ((tGear > 0) && (tGear < 5)) skp->Text(hps->CX-60, hps->CY+5, "GR-DN", 5);
			else if (STS()->GetGearState()==AnimState::OPENING) skp->Text(hps->CX-60, hps->CY+5, "//GR//", 6);

			// guidance mode
			if (!bWOW)
			{
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
			}

			// speedbrake
			skp->MoveTo((hps->CX), hps->H-85);
			skp->LineTo((hps->CX)+50, hps->H-85);
			for(int i=0;i<=50;i+=10)
			{
				skp->Line((hps->CX)+i, hps->H-80, (hps->CX)+i, hps->H-90);
			}
			double spdb_tgt = STS()->GetTgtSpeedbrakePosition();
			double spdb_act = STS()->GetActSpeedbrakePosition();
			int commanded=(int)(spdb_tgt*50);
			int act=(int)(spdb_act*50);
			//actual
			skp->MoveTo((hps->CX)+act, hps->H-85);
			skp->LineTo((hps->CX)+act-4, hps->H-90);
			skp->LineTo((hps->CX)+act+4, hps->H-90);
			skp->LineTo((hps->CX)+act, hps->H-85);
			//commanded
			skp->MoveTo((hps->CX)+commanded, hps->H-85);
			skp->LineTo((hps->CX)+commanded-4, hps->H-80);
			skp->LineTo((hps->CX)+commanded+4, hps->H-80);
			skp->LineTo((hps->CX)+commanded, hps->H-85);
			skp->Line( hps->CX + commanded, hps->H - 80, hps->CX + commanded, hps->H - 75 );

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
				if (!bWOW) skp->Ellipse(Round(glideslope_center_x)-5, Round(glideslope_center_y)-5, Round(glideslope_center_x)+5, Round(glideslope_center_y)+5);
			}
			else {
				// before PRFNL mode, we have square at center of HUD
				glideslope_center_y = static_cast<double>(hps->H)/2.0 - 25.0;
				glideslope_center_x = static_cast<double>(hps->CX);
				skp->Rectangle(Round(glideslope_center_x)-5, Round(glideslope_center_y)-5, Round(glideslope_center_x)+5, Round(glideslope_center_y)+5);
			}
			// lines are the same for both VV and center square modes
			if (!bWOW)
			{
				skp->Line(Round(glideslope_center_x)-10, Round(glideslope_center_y), Round(glideslope_center_x)-5, Round(glideslope_center_y));
				skp->Line(Round(glideslope_center_x)+9, Round(glideslope_center_y), Round(glideslope_center_x)+4, Round(glideslope_center_y));
				skp->Line(Round(glideslope_center_x), Round(glideslope_center_y)-10, Round(glideslope_center_x), Round(glideslope_center_y)-5);
			}

			if(TAEMGuidanceMode != FNLFL) {
				double guidance_center_x, guidance_center_y;
				VECTOR3 lift, drag;
				STS()->GetLiftVector(lift);
				STS()->GetDragVector(drag);
				double NZSteadyState = cos(STS()->GetPitch())/cos(STS()->GetBank());
				if(TAEMGuidanceMode < FLARE) guidance_center_y = glideslope_center_y - (10.0*(NZCommand+NZSteadyState-averageNZ))*hps->Scale;
				else guidance_center_y = glideslope_center_y - (5.0*(NZCommand+NZSteadyState-averageNZ))*hps->Scale;
				guidance_center_x = hps->CX + (STS()->GetBank()*DEG+TargetBank)*hps->Scale;
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
					skp->MoveTo(Round(guidance_center_x)-5, Round(guidance_center_y));
					skp->LineTo(Round(guidance_center_x), Round(guidance_center_y)+5);
					skp->LineTo(Round(guidance_center_x)+5, Round(guidance_center_y));
					skp->LineTo(Round(guidance_center_x), Round(guidance_center_y)-5);
					skp->LineTo(Round(guidance_center_x)-5, Round(guidance_center_y));
				}

				// draw guidance triangles
				if(TAEMGuidanceMode >= OGS) DrawHUDGuidanceTriangles(skp, hps, degTargetGlideslope, dPitch, bank);
			}

			// nz accel
			if (TAEMGuidanceMode < PRFNL)
			{
				sprintf_s( cbuf, 255, "%.1f G", GetNZ() );
				skp->Text( hps->CX - 50, hps->CY + 50, cbuf, strlen( cbuf ) );
			}

			// css status
			if (!bWOW)
			{
				if (tCSS > 0)
				{
					if (tCSS < 5)
					{
						if (bHUDFlasher) skp->Text( hps->W - 75, 70, "CSS", 3 );// window 2 (flashing)
					}
					else skp->Text( 20, hps->H - 70, "CSS", 3 );// window 4
				}
			}

			if (dclt == 0)
			{
				// runway overlay
				VECTOR3 rwy1_end, lrwy1;
				oapiLocalToGlobal(hEarth, &RwyStart_EarthLocal, &rwy1_end);
				STS()->Global2Local(rwy1_end,lrwy1);

				VECTOR3 rwy2_end, lrwy2;
				oapiLocalToGlobal(hEarth, &RwyEnd_EarthLocal, &rwy2_end);
				STS()->Global2Local(rwy2_end,lrwy2);

				VECTOR3 rwyDir = lrwy2 - lrwy1;
				normalise(rwyDir);
				VECTOR3 lvlh = RotateVectorZ( RotateVectorX(_V(0, 1, 0), -dPitch), -bank);
				VECTOR3 rwyWidthDir = crossp(rwyDir, lvlh);

				double rwyWidth = vLandingSites[SITE_ID].GetRwyWidth(!SEC);
				VECTOR3 rwy1_l = lrwy1 - rwyWidthDir*(rwyWidth/2.0);
				VECTOR3 rwy1_r = lrwy1 + rwyWidthDir*(rwyWidth/2.0);
				VECTOR3 rwy2_l = lrwy2 - rwyWidthDir*(rwyWidth/2.0);
				VECTOR3 rwy2_r = lrwy2 + rwyWidthDir*(rwyWidth/2.0);

				VECTOR3 camPos;
				STS()->GetCameraOffset(camPos);

				VECTOR3 error[4];
				error[0] = camPos-rwy1_l;
				error[1] = camPos-rwy2_l;
				error[2] = camPos-rwy2_r;
				error[3] = camPos-rwy1_r;
				int rwy_pos_x[4], rwy_pos_y[4];
				for(int i=0;i<4;i++) {
					rwy_pos_x[i] = hps->CX + static_cast<int>( Round(hps->Scale*DEG*atan(error[i].x/error[i].z)) );
					rwy_pos_y[i] = hps->CY + static_cast<int>( Round(hps->Scale*DEG*atan(-error[i].y/error[i].z)) );
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
			}

			// draw pitch ladder
			int nPitch = static_cast<int>(dPitch);
			int pitchBar = nPitch - (nPitch%5); // display lines at 5-degree increments
			if ((dclt == 0) || (dclt == 1))
			{
				// create rotated font for HUD pitch markings
				oapi::Font* pFont = oapiCreateFont(12, false, "Fixed", FONT_NORMAL, -static_cast<int>(10*bank));
				oapi::Font* pOldFont = skp->SetFont(pFont);
				// draw pitch lines
				int curPitchLine = pitchBar+5;
				while(DrawHUDPitchLine(skp, hps, curPitchLine, dPitch, bank))
					curPitchLine+=5;	
				curPitchLine = pitchBar-5;
				while(DrawHUDPitchLine(skp, hps, curPitchLine, dPitch, bank))
					curPitchLine-=5;
				// deselect rotated font
				skp->SetFont(pOldFont);
				oapiReleaseFont(pFont);
			}

			// horizon line
			DrawHUDPitchLine( skp, hps, 0, dPitch, bank );

			// alt/vel
			double keas = STS()->GetKEAS();
			double alt = (int)(STS()->GetAltitude()*3.280833)-17;
			if (dclt == 2)
			{
				// numeric
				sprintf_s(cbuf, 255, "%3.0f", keas);
				if (!bWOW)
				{
					skp->Text( (int)glideslope_center_x - 45, (int)glideslope_center_y - 15, cbuf, strlen( cbuf ) );

					int tmpalt;
					if (alt > 1000) tmpalt = Round( alt * 0.005 ) * 200;
					else if (alt > 400) tmpalt = Round( alt * 0.01 ) * 100;
					else if (alt > 50) tmpalt = Round( alt * 0.1 ) * 10;
					else tmpalt = Round( alt );
					sprintf_s( cbuf, 255, "%d", tmpalt );
					skp->Text( (int)glideslope_center_x + 25, (int)glideslope_center_y - 15, cbuf, strlen( cbuf ) );
				}
				else if (!bWONG) skp->Text( hps->CX - 26, hps->CY - 14, cbuf, strlen( cbuf ) );
				else
				{
					skp->Text( hps->CX - 40, hps->CY - 14, "G", 1 );
					sprintf_s(cbuf, 255, "%3.0f", keas);// TODO ground speed
					skp->Text( hps->CX - 26, hps->CY - 14, cbuf, strlen( cbuf ) );
				}
			}
			else
			{
				// tape
				int yline = (int)(hps->H / 2.0) - 25;
				int ikeas = Round( keas * 0.2 ) * 5;
				int offset = yline + (int)((ikeas - keas) * 4);// 5/20px
				int tmp;
				int ytmp;
				for (int i = 0; i < 7; i++)
				{
					tmp = ikeas + ((i - 3) * 5);
					if (tmp < 0) continue;
					ytmp = offset + ((i - 3) * 20);
					if (tmp % 10 == 0)
					{
						// number
						sprintf_s( cbuf, 255, "%d", tmp );
						skp->Text( 20, ytmp - 7, cbuf, strlen( cbuf ) );
					}
					else skp->Line( 20, ytmp, 25, ytmp );// line
				}
				skp->Line( 10, yline - 1, 20, yline - 1 );
				skp->Line( 10, yline, 20, yline );
				skp->Line( 10, yline + 1, 20, yline + 1 );

				int tmpalt = Round( alt );
				int ialt = Round( alt * 0.002 ) * 500;
				offset = yline + (int)((tmpalt - ialt) * 0.04);// 500/20px
				for (int i = 0; i < 7; i++)
				{
					tmp = ialt + ((3 - i) * 500);
					if (tmp < 0) break;
					ytmp = offset - ((3 - i) * 20);
					if (tmp % 5000 == 0)
					{
						// number w 'K'
						sprintf_s( cbuf, 255, "%2dK", (int)(tmp * 0.001) );
						skp->Text( hps->W - 40, ytmp - 7, cbuf, strlen( cbuf ) );
					}
					else if (tmp % 1000 == 0)
					{
						// number w/o 'K'
						sprintf_s( cbuf, 255, "%3d", (int)(tmp * 0.001) );
						skp->Text( hps->W - 40, ytmp - 7, cbuf, strlen( cbuf ) );
					}
					else skp->Line( hps->W - 25, ytmp, hps->W - 20, ytmp );// line
				}
				skp->Line( hps->W - 20, yline - 1, hps->W - 10, yline - 1 );
				skp->Line( hps->W - 20, yline, hps->W - 10, yline );
				skp->Line( hps->W - 20, yline + 1, hps->W - 10, yline + 1 );
			}
		}
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
	else if (!_strnicmp( keyword, "CLOSE_AIM_POINT", 15 ))
	{
		OGS_AIMPOINT = OGS_AIMPOINT_CLOSE;
		X_AL_INTERCEPT = OGS_AIMPOINT + Y_AL_INTERCEPT/AL_GS;
		HAC_CENTER_X = OGS_AIMPOINT - 33020.0/MPS2FPS;
		return true;
	}
	else if (!_strnicmp( keyword, "SB_CONTROL_LOGIC", 16 ))
	{
		int nTemp;
		sscanf_s( value, "%d", &nTemp );
		if ((nTemp >= 0) && (nTemp <= ELS)) SBControlLogic = static_cast<SB_CONTROL_LOGIC>(nTemp);
		return true;
	}
	if (!_strnicmp( keyword, "FCS_PITCH", 9 ))
	{
		if (!_strnicmp( value, "CSS", 3 )) AutoFCSPitch = false;
		else AutoFCSPitch = true;
		return true;
	}
	if (!_strnicmp( keyword, "FCS_ROLL", 8 ))
	{
		if (!_strnicmp( value, "CSS", 3 )) AutoFCSRoll = false;
		else AutoFCSRoll = true;
		return true;
	}
	if (!_strnicmp( keyword, "SPEEDBRAKE_STATE", 16 ))
	{
		if (!_strnicmp( value, "CDR", 3 )) SpeedbrakeState = MAN_CDR;
		else if (!_strnicmp( value, "PLT", 3 )) SpeedbrakeState = MAN_PLT;
		else SpeedbrakeState = AUTO;
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
	if (OGS_AIMPOINT == OGS_AIMPOINT_CLOSE) oapiWriteScenario_string( scn, "CLOSE_AIM_POINT", "TRUE" );
	oapiWriteScenario_int( scn, "SB_CONTROL_LOGIC", static_cast<int>(SBControlLogic) );
	if (AutoFCSPitch == false) oapiWriteScenario_string( scn, "FCS_PITCH", "CSS" );
	if (AutoFCSRoll == false) oapiWriteScenario_string( scn, "FCS_ROLL", "CSS" );
	if (SpeedbrakeState == MAN_CDR) oapiWriteScenario_string( scn, "SPEEDBRAKE_STATE", "CDR" );
	else if (SpeedbrakeState == MAN_PLT) oapiWriteScenario_string( scn, "SPEEDBRAKE_STATE", "PLT" );
	return;
}

void AerojetDAP::PaintHORIZSITDisplay(vc::MDU* pMDU) const
{
	char cbuf[51];
	PrintCommonHeader("    HORIZ SIT", pMDU);

	pMDU->mvprint( 0, 1, "PTI" );
	pMDU->mvprint( 10, 1, "1" );
	pMDU->mvprint( 1, 2, "INDEX" );
	pMDU->mvprint( 13, 1, "ALTM" );
	pMDU->mvprint( 13, 2, "9" );

	pMDU->mvprint(0, 5, "41 LAND SITE");
	sprintf_s(cbuf, 51, "%d", SITE_ID+1);
	pMDU->mvprint(13, 5, cbuf);
	pMDU->mvprint(0, 6, "PRI");
	pMDU->mvprint(4, 6, vLandingSites[SITE_ID].GetPriRwyName().c_str());
	pMDU->mvprint(13, 6, "3");
	pMDU->mvprint(0, 7, "SEC");
	pMDU->mvprint(4, 7, vLandingSites[SITE_ID].GetSecRwyName().c_str());
	pMDU->mvprint(13, 7, "4");
	if(SEC) pMDU->mvprint(14, 7, "*");
	else pMDU->mvprint(14, 6, "*");
	pMDU->mvprint( 0, 8, "TAC" );
	pMDU->mvprint( 13, 8, "5" );
	pMDU->mvprint( 0, 9, "GPS FOM" );
	pMDU->mvprint( 13, 9, "RA" );
	pMDU->mvprint( 12, 10, "46" );

	pMDU->mvprint( 0, 11, "TAEM TGT" );
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
	pMDU->mvprint( 0, 14, "NEP" );// TODO
	//if (ENTRY_POINT == NEP) pMDU->mvprint( 0, 14, "NEP" );
	//else pMDU->mvprint( 0, 14, "MEP" );
	pMDU->mvprint( 11, 14, "7" );
	pMDU->mvprint( 0, 15, "AIM" );
	pMDU->mvprint( 11, 15, "8" );
	if (OGS_AIMPOINT == OGS_AIMPOINT_NOM) pMDU->mvprint( 6, 15, "NOM" );
	else pMDU->mvprint( 6, 15, "CLSE" );
	pMDU->mvprint( 0, 16, "S/B" );
	pMDU->mvprint( 11, 16, "39" );
	if (SBControlLogic == NOM) pMDU->mvprint( 7, 16, "NOM" );
	else if (SBControlLogic == SHORT) pMDU->mvprint( 5, 16, "SHORT", dps::DEUATT_OVERBRIGHT );
	else pMDU->mvprint( 7, 16, "ELS", dps::DEUATT_OVERBRIGHT );

	pMDU->mvprint( 42, 2, "NAV DATA" );
	pMDU->Delta( 44, 3 );
	pMDU->mvprint( 45, 3, "X 10" );
	pMDU->Delta( 44, 5 );
	pMDU->mvprint( 45, 5, "Y 11" );
	pMDU->Delta( 44, 7 );
	pMDU->mvprint( 45, 7, "Z 12" );
	pMDU->Delta( 44, 9 );
	pMDU->mvprint( 45, 9, "X 13" );
	pMDU->DotCharacter( 45, 9 );
	pMDU->Delta( 44, 11 );
	pMDU->mvprint( 45, 11, "Y 14" );
	pMDU->DotCharacter( 45, 11 );
	pMDU->Delta( 44, 13 );
	pMDU->mvprint( 45, 13, "Z 15" );
	pMDU->DotCharacter( 45, 13 );
	pMDU->mvprint( 44, 15, "LOAD 16" );
	pMDU->mvprint( 39, 16, "18  T" );
	pMDU->Delta( 42, 16 );

	pMDU->mvprint( 0, 17, "NAV" );
	pMDU->mvprint( 0, 18, "TAC AZ" );
	pMDU->mvprint( 3, 19, "RNG" );
	pMDU->mvprint( 0, 20, "GPS" );
	pMDU->mvprint( 0, 21, "DRAG H" );
	pMDU->mvprint( 0, 22, "ADTA H" );
	pMDU->mvprint( 6, 17, "RESID" );
	pMDU->mvprint( 12, 17, "RATIO" );
	pMDU->mvprint( 17, 17, "AUT" );
	pMDU->mvprint( 17, 18, "19" );
	pMDU->mvprint( 17, 20, "42" );
	pMDU->mvprint( 17, 21, "22" );
	pMDU->mvprint( 17, 22, "25" );
	pMDU->mvprint( 20, 17, "INH" );
	pMDU->mvprint( 20, 18, "20" );
	pMDU->mvprint( 20, 20, "43" );
	pMDU->mvprint( 20, 21, "23" );
	pMDU->mvprint( 20, 22, "26" );
	pMDU->mvprint( 23, 17, "FOR" );
	pMDU->mvprint( 23, 18, "21" );
	pMDU->mvprint( 23, 20, "44" );
	pMDU->mvprint( 23, 21, "24" );
	pMDU->mvprint( 23, 22, "27" );
	pMDU->mvprint( 0, 23, "ADTA TO G&C" );
	pMDU->mvprint( 17, 23, "28" );
	pMDU->mvprint( 20, 23, "29" );
	pMDU->mvprint( 23, 23, "30" );
	pMDU->mvprint( 27, 17, "TAC 1" );
	pMDU->mvprint( 28, 20, "DES 31" );
	pMDU->mvprint( 35, 17, "TAC 2" );
	pMDU->mvprint( 36, 20, "DES 32" );
	pMDU->mvprint( 43, 17, "TAC 3" );
	pMDU->mvprint( 44, 20, "DES 33" );
	pMDU->mvprint( 28, 21, "ABS 34" );
	pMDU->mvprint( 36, 21, "DELTA 35" );
	pMDU->mvprint( 26, 22, "GPS S" );
	pMDU->mvprint( 32, 22, "RN" );
	pMDU->mvprint( 42, 22, "AZ" );
	pMDU->mvprint( 26, 23, "AIF_G S47" );
	pMDU->mvprint( 37, 23, "48" );
	pMDU->mvprint( 41, 23, "49" );
	if (0) pMDU->mvprint( 29, 15, "MLS", dps::DEUATT_OVERBRIGHT );// TODO

	const int BUG_POINT_X = 264;
	const int BUG_POINT_Y = 210;

	const double YSGN = (HACSide==L) ? -1.0 : 1.0;
	const double HAC_CENTER_Y = YSGN * FINAL_RADIUS;

	// frame used for all vectors: LVLH-like frame with origin at shuttle position, X-axis along -ve velocity vector, Y-axis towards port side
	VECTOR3 velocity;
	STS()->GetHorizonAirspeedVector(velocity);
	double degHeading = DEG*atan2(velocity.x, velocity.z);
	double degHeadingError = vLandingSites[SITE_ID].GetRwyHeading(!SEC) + 180 - degHeading;
	while(degHeadingError < 0.0) degHeadingError+=360.0;
	while(degHeadingError > 360.0) degHeadingError-=360.0;
	VECTOR3 TgtPos = GetRunwayRelPos();

	VECTOR3 TouchdownPos = -RotateVectorZ(_V(TgtPos.x-IGS_AIMPOINT, TgtPos.y, 0.0), degHeadingError); // ignore height error
	VECTOR3 HACExitPos = -RotateVectorZ(_V(TgtPos.x-HAC_CENTER_X, TgtPos.y, 0.0), degHeadingError);
	// calculate scale factor for display
	double scale_distance = max(length(TouchdownPos), length(HACExitPos)+HAC_TurnRadius); // make sure HAC circle and touchdown point are visible
	scale_distance = range(20e3, scale_distance, 500e3); // limit distance covered by display to between 20km and 500km
	double scale = scale_distance/182; // screen area is 512 pixels by 364 pixels (using 364 as limit)
	int touchdown_x = BUG_POINT_X - Round(TouchdownPos.y/scale);
	int touchdown_y = Round(TouchdownPos.x/scale) + BUG_POINT_Y;
	int hac_exit_x = BUG_POINT_X - Round(HACExitPos.y/scale);
	int hac_exit_y = Round(HACExitPos.x/scale) + BUG_POINT_Y;
	pMDU->Circle(touchdown_x, touchdown_y, 5);
	pMDU->Line(hac_exit_x, hac_exit_y, touchdown_x, touchdown_y);

	if (GetApproachAndLandState() == false)
	{
		VECTOR3 HACCenter = -RotateVectorZ(_V(TgtPos.x-HAC_CENTER_X, TgtPos.y-HAC_CENTER_Y, 0.0), degHeadingError);
		int hac_center_x = BUG_POINT_X - Round(HACCenter.y/scale);
		int hac_center_y = BUG_POINT_Y + Round(HACCenter.x/scale);
		int hac_radius = Round(HAC_TurnRadius/scale);
		pMDU->Circle(hac_center_x, hac_center_y, hac_radius);
	}

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
			int pos_x = BUG_POINT_X - Round(pos.y/scale);
			int pos_y = BUG_POINT_Y + Round(pos.x/scale);
			pMDU->Circle(pos_x, pos_y, 5, dps::DEUATT_OVERBRIGHT);
		}
	}

	// draw shuttle bug (this is always at fixed position)
	double nz = GetNZ();
	char att = 0;
	if ((nz > 2.5) || (nz < -1)) att = dps::DEUATT_FLASHING;// limits are true most of the time, but not 100% accurate

	pMDU->OrbiterSymbolTop( BUG_POINT_X, BUG_POINT_Y, att );

	sprintf_s( cbuf, 51, "%3.1f", nz );
	pMDU->mvprint( 21, 15, cbuf, att );

	// scales
	if (((GetMajorMode() == 305) || (GetMajorMode() == 603)) && (STS()->GetAltitude() > 2133.6))// blank under 7kft
	{
		// top scale
		pMDU->Line( 222, 50, 402, 50 );
		// side scale
		pMDU->Line( 418, 73, 418, 195 );
		pMDU->Line( 415, 73, 421, 73 );
		pMDU->Line( 415, 195, 421, 195 );
		pMDU->Line( 415, 104, 421, 104 );
		pMDU->Line( 415, 134, 421, 134 );
		pMDU->Line( 415, 165, 421, 165 );

		if (GetOnHACState() == false)
		{
			// ACQ
			// top scale
			char att = 0;
			int pos;
			double t2h = fabs( TimeToHAC );
			pMDU->Line( 222, 47, 222, 53 );
			pMDU->Line( 402, 47, 402, 53 );
			if (TimeToHAC < 0)
			{
				pMDU->mvprint( 21, 4, "0 1 2 3" );
				pMDU->Line( 240, 47, 240, 53 );
				pMDU->Line( 258, 47, 258, 53 );
				pMDU->Line( 276, 47, 276, 53 );
			}
			else
			{
				pMDU->mvprint( 34, 4, "3 2 1 0" );
				pMDU->Line( 348, 47, 348, 53 );
				pMDU->Line( 366, 47, 366, 53 );
				pMDU->Line( 384, 47, 384, 53 );
			}
			
			if (t2h < 20)
			{
				if (t2h > 10)
				{
					att = dps::DEUATT_FLASHING;
					t2h = 10;
				}
				else t2h = floor( t2h );

				if (t2h == 0) att = dps::DEUATT_FLASHING;

				if (TimeToHAC < 0) pos = 222 + Round( t2h * 18 );
				else pos = 402 - Round( t2h * 18 );
				pMDU->Line( pos, 48, pos - 6, 40, att );
				pMDU->Line( pos - 6, 40, pos + 6, 40, att );
				pMDU->Line( pos + 6, 40, pos, 48, att );
			}

			// side scale
			pMDU->mvprint( 37, 5, "5.0K" );
			pMDU->mvprint( 37, 14, "5.0K" );
			att = 0;
			double err = 0;// TODO

			if (err > 5000)
			{
				att = dps::DEUATT_FLASHING;
				pos = 195;
			}
			else if (err < -5000)
			{
				att = dps::DEUATT_FLASHING;
				pos = 73;
			}
			else pos = Round( err * 0.0122 ) + 134;

			pMDU->LeftArrowFull( 420, pos, att );
		}
		else if (GetPrefinalState() == false)
		{
			// HDG
			// top scale
			pMDU->Line( 222, 47, 222, 53 );
			pMDU->Line( 402, 47, 402, 53 );
			pMDU->mvprint( 20, 4, "5.0K" );
			pMDU->mvprint( 38, 4, "5.0K" );
			pMDU->Line( 268, 47, 268, 53 );
			pMDU->Line( 312, 47, 312, 53 );
			pMDU->Line( 358, 47, 358, 53 );
			char att = 0;
			int pos;
			double err = -GetHACRadialError();

			if (err > 5000)
			{
				att = dps::DEUATT_FLASHING;
				pos = 402;
			}
			else if (err < -5000)
			{
				att = dps::DEUATT_FLASHING;
				pos = 222;
			}
			else pos = Round( err * 0.018 ) + 312;

			pMDU->OrbiterSymbolTop( pos, 36, att );

			// side scale
			pMDU->mvprint( 37, 5, "5.0K" );
			pMDU->mvprint( 37, 14, "5.0K" );
			att = 0;
			err = 0;// TODO

			if (err > 5000)
			{
				att = dps::DEUATT_FLASHING;
				pos = 195;
			}
			else if (err < -5000)
			{
				att = dps::DEUATT_FLASHING;
				pos = 73;
			}
			else pos = Round( err * 0.0122 ) + 134;

			pMDU->LeftArrowFull( 420, pos, att );
		}
		else
		{
			// PRFNL and A/L
			// top scale
			pMDU->Line( 222, 47, 222, 53 );
			pMDU->Line( 402, 47, 402, 53 );
			pMDU->mvprint( 20, 4, "2.5K" );
			pMDU->mvprint( 38, 4, "2.5K" );
			pMDU->Line( 268, 47, 268, 53 );
			pMDU->Line( 312, 47, 312, 53 );
			pMDU->Line( 358, 47, 358, 53 );
			char att = 0;
			int pos;
			double err = GetYRunwayPositionError();

			if (err > 2500)
			{
				att = dps::DEUATT_FLASHING;
				pos = 402;
			}
			else if (err < -2500)
			{
				att = dps::DEUATT_FLASHING;
				pos = 222;
			}
			else pos = Round( err * 0.036 ) + 312;

			pMDU->OrbiterSymbolTop( pos, 36, att );

			// side scale
			pMDU->mvprint( 37, 5, "1.0K" );
			pMDU->mvprint( 37, 14, "1.0K" );
			att = 0;
			err = GetGlideSlopeDistance();

			if (err > 1000)
			{
				att = dps::DEUATT_FLASHING;
				pos = 195;
			}
			else if (err < -1000)
			{
				att = dps::DEUATT_FLASHING;
				pos = 73;
			}
			else pos = Round( err * 0.061 ) + 134;

			pMDU->LeftArrowFull( 420, pos, att );
		}
	}

	// lines
	pMDU->Line( 0, 14, 110, 14 );
	pMDU->Line( 110, 14, 110, 56 );
	pMDU->Line( 0, 56, 110, 56 );

	pMDU->Line( 0, 154, 150, 154 );

	pMDU->Line( 0, 238, 512, 238 );
	pMDU->Line( 260, 294, 450, 294 );
	pMDU->Line( 260, 308, 510, 308 );
	pMDU->Line( 0, 322, 260, 322 );

	pMDU->Line( 60, 238, 60, 322 );
	pMDU->Line( 120, 238, 120, 322 );
	pMDU->Line( 170, 238, 170, 336 );
	pMDU->Line( 200, 238, 200, 336 );
	pMDU->Line( 230, 238, 230, 336 );
	pMDU->Line( 260, 238, 260, 336 );
	pMDU->Line( 350, 238, 350, 280 );
	pMDU->Line( 430, 238, 430, 280 );
	pMDU->Line( 450, 294, 450, 308 );
}

void AerojetDAP::PaintENTRYTRAJ1Display( vc::MDU* pMDU ) const
{
	char cbuf[8];
	PrintCommonHeader( "  ENTRY TRAJ 1", pMDU );

	pMDU->Alpha( 1, 1 );
	pMDU->mvprint( 0, 2, "50" );
	pMDU->mvprint( 0, 6, "45" );
	pMDU->mvprint( 0, 10, "40" );
	pMDU->mvprint( 0, 14, "35" );
	pMDU->mvprint( 0, 18, "30" );
	pMDU->mvprint( 0, 22, "25" );
	pMDU->mvprint( 4, 1, "D" );
	pMDU->mvprint( 4, 2, "50" );
	pMDU->mvprint( 4, 6, "40" );
	pMDU->mvprint( 4, 10, "30" );
	pMDU->mvprint( 4, 14, "20" );
	pMDU->mvprint( 4, 18, "10" );
	pMDU->mvprint( 4, 22, "0" );
	pMDU->mvprint( 7, 4, "1 BIAS" );
	pMDU->mvprint( 7, 5, "D REF" );
	pMDU->mvprint( 7, 6, "q" );
	pMDU->Line( 70, 84, 80, 84 );
	pMDU->Delta( 7, 7 );
	pMDU->mvprint( 9, 7, "AZ" );
	//pMDU->mvprint( 7, 9, "LO ENRGY" );
	//pMDU->mvprint( 7, 10, "3" );
	pMDU->mvprint( 38, 15, "NY" );
	pMDU->mvprint( 38, 16, "NY TRIM" );
	pMDU->mvprint( 38, 17, "AIL" );
	pMDU->mvprint( 38, 18, "RUD" );
	pMDU->mvprint( 37, 19, "ZERO H BIAS 2" );
	pMDU->DotCharacter( 42, 19 );
	pMDU->mvprint( 38, 20, "H BIAS" );
	pMDU->DotCharacter( 38, 20 );
	pMDU->mvprint( 41, 21, "REF" );
	pMDU->mvprint( 36, 22, "ROLL REF" );
	pMDU->mvprint( 41, 23, "CMD" );
	pMDU->mvprint( 34, 2, "10D" );
	pMDU->mvprint( 40, 2, "8D" );
	pMDU->mvprint( 46, 2, "6D" );
	pMDU->mvprint( 23, 6, "15D" );
	pMDU->mvprint( 16, 9, "20D" );
	pMDU->mvprint( 10, 14, "25D" );
	pMDU->mvprint( 41, 13, "-40" );
	pMDU->mvprint( 30, 18, "-70" );
	pMDU->mvprint( 4, 23, "-180" );
	pMDU->mvprint( 19, 23, "-100" );

	// phugoid scale lines
	pMDU->Line( 70, 30, 250, 30 );
	pMDU->Line( 70, 30, 70, 36 );
	pMDU->Line( 160, 30, 160, 36 );
	pMDU->Line( 250, 30, 250, 36 );

	// alpha/D scale lines
	pMDU->Line( 34, 34, 34, 314 );
	pMDU->Line( 30, 34, 38, 34 );
	pMDU->Line( 30, 45, 38, 45 );
	pMDU->Line( 30, 56, 38, 56 );
	pMDU->Line( 30, 68, 38, 68 );
	pMDU->Line( 30, 79, 38, 79 );
	pMDU->Line( 30, 90, 38, 90 );
	pMDU->Line( 30, 101, 38, 101 );
	pMDU->Line( 30, 112, 38, 112 );
	pMDU->Line( 30, 124, 38, 124 );
	pMDU->Line( 30, 135, 38, 135 );
	pMDU->Line( 30, 146, 38, 146 );
	pMDU->Line( 30, 157, 38, 157 );
	pMDU->Line( 30, 168, 38, 168 );
	pMDU->Line( 30, 180, 38, 180 );
	pMDU->Line( 30, 191, 38, 191 );
	pMDU->Line( 30, 202, 38, 202 );
	pMDU->Line( 30, 213, 38, 213 );
	pMDU->Line( 30, 224, 38, 224 );
	pMDU->Line( 30, 236, 38, 236 );
	pMDU->Line( 30, 247, 38, 247 );
	pMDU->Line( 30, 258, 38, 258 );
	pMDU->Line( 30, 269, 38, 269 );
	pMDU->Line( 30, 280, 38, 280 );
	pMDU->Line( 30, 292, 38, 292 );
	pMDU->Line( 30, 303, 38, 303 );
	pMDU->Line( 30, 314, 38, 314 );

	// vel/rng lines
	pMDU->Line( 348, 48, 174, 176 );
	pMDU->Line( 174, 176, 50, 308 );

	pMDU->Line( 394, 45, 192, 190 );
	pMDU->Line( 192, 190, 50, 308 );

	pMDU->Line( 444, 50, 204, 202 );
	pMDU->Line( 204, 202, 74, 311 );

	pMDU->Line( 488, 54, 178, 246 );
	pMDU->Line( 178, 246, 110, 308 );

	pMDU->Line( 508, 120, 282, 205 );
	pMDU->Line( 282, 205, 176, 277 );
	pMDU->Line( 176, 277, 150, 308 );

	// drag lines
	pMDU->Line( 110, 210, 70, 313, dps::DEUATT_DASHED );

	pMDU->Line( 170, 142, 118, 269, dps::DEUATT_DASHED );
	pMDU->Line( 118, 269, 90, 305, dps::DEUATT_DASHED );

	pMDU->Line( 244, 100, 182, 314, dps::DEUATT_DASHED );

	pMDU->Line( 350, 40, 298, 142, dps::DEUATT_DASHED );
	pMDU->Line( 298, 142, 298, 238, dps::DEUATT_DASHED );

	pMDU->Line( 412, 45, 406, 75, dps::DEUATT_DASHED );
	pMDU->Line( 406, 75, 372, 143, dps::DEUATT_DASHED );
	pMDU->Line( 372, 143, 374, 199, dps::DEUATT_DASHED );

	pMDU->Line( 476, 39, 424, 140, dps::DEUATT_DASHED );
	pMDU->Line( 424, 140, 424, 176, dps::DEUATT_DASHED );

	// digital data
	sprintf_s( cbuf, 8, "%5.1f", STS()->GetDynPressure() * PA2PSF );
	pMDU->mvprint( 12, 6, cbuf );

	sprintf_s( cbuf, 8, "%+5.1f", GetdeltaAZ() );
	pMDU->mvprint( 12, 7, cbuf );

	VECTOR3 lift, drag, gravity;
	STS()->GetLiftVector(lift);
	STS()->GetDragVector(drag);
	STS()->GetWeightVector(gravity);
	double tmp = (lift.x+drag.x+gravity.x)/(OrbiterMass * G);
	sprintf_s( cbuf, 8, "%.3f", fabs( tmp ) );
	if (tmp > 0) cbuf[0] = 'R';
	else if (tmp < 0) cbuf[0] = 'L';
	else cbuf[0] = ' ';
	pMDU->mvprint( 46, 16, cbuf );

	// scale data
	char att = dps::DEUATT_OVERBRIGHT;
	double alpha = STS()->GetAOA() * DEG;
	double alphacmd = CalculateTargetAOA(STS()->GetMachNumber());
	if (fabs( alpha - alphacmd ) > 2) att |= dps::DEUATT_FLASHING;
	int pos;
	if (alpha > 50)
	{
		pos = 34;
		att |= dps::DEUATT_FLASHING;
	}
	else if (alpha < 25)
	{
		pos = 314;
		att |= dps::DEUATT_FLASHING;
	}
	else pos = 594 - Round( 11.2 * alpha );
	pMDU->Line( 33, pos, 25, pos + 6, att );
	pMDU->Line( 25, pos + 6, 25, pos - 6, att );
	pMDU->Line( 25, pos - 6, 33, pos, att );

	att = dps::DEUATT_OVERBRIGHT;
	if (alphacmd > 50)
	{
		pos = 34;
		att |= dps::DEUATT_FLASHING;
	}
	else if (alphacmd < 25)
	{
		pos = 314;
		att |= dps::DEUATT_FLASHING;
	}
	else pos = 594 - Round( 11.2 * alphacmd );
	pMDU->Line( 33, pos, 27, pos + 6, att );
	pMDU->Line( 27, pos + 6, 27, pos + 2, att );
	pMDU->Line( 27, pos + 2, 15, pos + 2, att );
	pMDU->Line( 15, pos + 2, 15, pos - 2, att );
	pMDU->Line( 15, pos - 2, 27, pos - 2, att );
	pMDU->Line( 27, pos - 2, 27, pos - 6, att );
	pMDU->Line( 27, pos - 6, 33, pos, att );

	att = dps::DEUATT_OVERBRIGHT;
	tmp = (length( drag ) / OrbiterMass) * MPS2FPS;
	if (tmp > 50)
	{
		pos = 34;
		att |= dps::DEUATT_FLASHING;
	}
	else if (tmp < 0)
	{
		pos = 314;
		att |= dps::DEUATT_FLASHING;
	}
	else pos = 314 - Round( 5.6 * tmp );
	pMDU->Line( 35, pos, 43, pos + 6, att );
	pMDU->Line( 43, pos + 6, 43, pos - 6, att );
	pMDU->Line( 43, pos - 6, 35, pos, att );

	att = dps::DEUATT_OVERBRIGHT;
	tmp = refDrag * MPS2FPS;
	if (tmp > 50)
	{
		pos = 34;
		att |= dps::DEUATT_FLASHING;
	}
	else if (tmp < 0)
	{
		pos = 314;
		att |= dps::DEUATT_FLASHING;
	}
	else pos = 314 - Round( 5.6 * tmp );
	pMDU->Line( 35, pos, 41, pos + 6, att );
	pMDU->Line( 41, pos + 6, 41, pos + 2, att );
	pMDU->Line( 41, pos + 2, 53, pos + 2, att );
	pMDU->Line( 53, pos + 2, 53, pos - 2, att );
	pMDU->Line( 53, pos - 2, 41, pos - 2, att );
	pMDU->Line( 41, pos - 2, 41, pos - 6, att );
	pMDU->Line( 41, pos - 6, 35, pos, att );

	// orbiter symbol
	int x = Round( (ETVS_Range * 0.430666) - 308.266666 - (ETVS_Range * ETVS_Range * 0.0000566666) );
	int y = Round( 1034.1333 - (ET_Mach * 41.06667) );
	att = dps::DEUATT_OVERBRIGHT;
	if (0) att |= dps::DEUATT_FLASHING;// TODO on roll reversal
	pMDU->OrbiterSymbolSide( x, y, 0, att );

	// orbiter trailers
	for (int i = 5; i--;)
	{
		if (ET_RangeHistory[i] != 0)
		{
			x = Round( (ET_RangeHistory[i] * 0.430666) - 308.266666 - (ET_RangeHistory[i] * ET_RangeHistory[i] * 0.0000566666) );
			y = Round( 1034.1333 - (ET_MachHistory[i] * 41.06667) );
			pMDU->Line( x, y + 4, x - 6, y - 4, dps::DEUATT_OVERBRIGHT );
			pMDU->Line( x - 6, y - 4, x + 6, y - 4, dps::DEUATT_OVERBRIGHT );
			pMDU->Line( x + 6, y - 4, x, y + 4, dps::DEUATT_OVERBRIGHT );
		}
	}
	return;
}

void AerojetDAP::PaintENTRYTRAJ2Display( vc::MDU* pMDU ) const
{
	char cbuf[8];
	PrintCommonHeader( "  ENTRY TRAJ 2", pMDU );

	pMDU->Alpha( 1, 1 );
	pMDU->mvprint( 0, 2, "50" );
	pMDU->mvprint( 0, 6, "45" );
	pMDU->mvprint( 0, 10, "40" );
	pMDU->mvprint( 0, 14, "35" );
	pMDU->mvprint( 0, 18, "30" );
	pMDU->mvprint( 0, 22, "25" );
	pMDU->mvprint( 4, 1, "D" );
	pMDU->mvprint( 4, 2, "50" );
	pMDU->mvprint( 4, 6, "40" );
	pMDU->mvprint( 4, 10, "30" );
	pMDU->mvprint( 4, 14, "20" );
	pMDU->mvprint( 4, 18, "10" );
	pMDU->mvprint( 4, 22, "0" );
	pMDU->mvprint( 7, 4, "1 BIAS" );
	pMDU->mvprint( 7, 5, "D REF" );
	pMDU->mvprint( 7, 6, "q" );
	pMDU->Line( 70, 84, 80, 84 );
	pMDU->Delta( 7, 7 );
	pMDU->mvprint( 9, 7, "AZ" );
	//pMDU->mvprint( 7, 9, "LO ENRGY" );
	//pMDU->mvprint( 7, 10, "3" );
	pMDU->mvprint( 38, 15, "NY" );
	pMDU->mvprint( 38, 16, "NY TRIM" );
	pMDU->mvprint( 38, 17, "AIL" );
	pMDU->mvprint( 38, 18, "RUD" );
	pMDU->mvprint( 37, 19, "ZERO H BIAS 2" );
	pMDU->DotCharacter( 42, 19 );
	pMDU->mvprint( 38, 20, "H BIAS" );
	pMDU->DotCharacter( 38, 20 );
	pMDU->mvprint( 41, 21, "REF" );
	pMDU->mvprint( 36, 22, "ROLL REF" );
	pMDU->mvprint( 41, 23, "CMD" );
	pMDU->mvprint( 31, 2, "35D" );
	pMDU->mvprint( 35, 2, "25D" );
	pMDU->mvprint( 39, 2, "20D" );
	pMDU->mvprint( 15, 13, "45D" );
	pMDU->mvprint( 7, 19, "52D" );
	pMDU->mvprint( 19, 23, "-125" );
	pMDU->mvprint( 31, 23, "-190" );

	// phugoid scale lines
	pMDU->Line( 70, 30, 250, 30 );
	pMDU->Line( 70, 30, 70, 36 );
	pMDU->Line( 160, 30, 160, 36 );
	pMDU->Line( 250, 30, 250, 36 );

	// alpha/D scale lines
	pMDU->Line( 34, 34, 34, 314 );
	pMDU->Line( 30, 34, 38, 34 );
	pMDU->Line( 30, 45, 38, 45 );
	pMDU->Line( 30, 56, 38, 56 );
	pMDU->Line( 30, 68, 38, 68 );
	pMDU->Line( 30, 79, 38, 79 );
	pMDU->Line( 30, 90, 38, 90 );
	pMDU->Line( 30, 101, 38, 101 );
	pMDU->Line( 30, 112, 38, 112 );
	pMDU->Line( 30, 124, 38, 124 );
	pMDU->Line( 30, 135, 38, 135 );
	pMDU->Line( 30, 146, 38, 146 );
	pMDU->Line( 30, 157, 38, 157 );
	pMDU->Line( 30, 168, 38, 168 );
	pMDU->Line( 30, 180, 38, 180 );
	pMDU->Line( 30, 191, 38, 191 );
	pMDU->Line( 30, 202, 38, 202 );
	pMDU->Line( 30, 213, 38, 213 );
	pMDU->Line( 30, 224, 38, 224 );
	pMDU->Line( 30, 236, 38, 236 );
	pMDU->Line( 30, 247, 38, 247 );
	pMDU->Line( 30, 258, 38, 258 );
	pMDU->Line( 30, 269, 38, 269 );
	pMDU->Line( 30, 280, 38, 280 );
	pMDU->Line( 30, 292, 38, 292 );
	pMDU->Line( 30, 303, 38, 303 );
	pMDU->Line( 30, 314, 38, 314 );

	// vel/rng lines
	pMDU->Line( 338, 42, 108, 266 );
	pMDU->Line( 108, 266, 82, 308 );

	pMDU->Line( 356, 42, 206, 308 );

	pMDU->Line( 396, 42, 218, 308 );

	pMDU->Line( 478, 53, 368, 229 );
	pMDU->Line( 368, 229, 320, 297 );

	// drag lines
	pMDU->Line( 158, 193, 110, 314, dps::DEUATT_DASHED );

	pMDU->Line( 326, 42, 192, 313, dps::DEUATT_DASHED );

	pMDU->Line( 366, 44, 228, 314, dps::DEUATT_DASHED );

	pMDU->Line( 406, 44, 298, 303, dps::DEUATT_DASHED );

	// digital data
	sprintf_s( cbuf, 8, "%5.1f", STS()->GetDynPressure() * PA2PSF );
	pMDU->mvprint( 12, 6, cbuf );

	sprintf_s( cbuf, 8, "%+5.1f", GetdeltaAZ() );
	pMDU->mvprint( 12, 7, cbuf );

	VECTOR3 lift, drag, gravity;
	STS()->GetLiftVector(lift);
	STS()->GetDragVector(drag);
	STS()->GetWeightVector(gravity);
	double tmp = (lift.x+drag.x+gravity.x)/(OrbiterMass * G);
	sprintf_s( cbuf, 8, "%.3f", fabs( tmp ) );
	if (tmp > 0) cbuf[0] = 'R';
	else if (tmp < 0) cbuf[0] = 'L';
	else cbuf[0] = ' ';
	pMDU->mvprint( 46, 15, cbuf );

	// scale data
	char att = dps::DEUATT_OVERBRIGHT;
	double alpha = STS()->GetAOA() * DEG;
	double alphacmd = CalculateTargetAOA(STS()->GetMachNumber());
	if (fabs( alpha - alphacmd ) > 2) att |= dps::DEUATT_FLASHING;
	int pos;
	if (alpha > 50)
	{
		pos = 34;
		att |= dps::DEUATT_FLASHING;
	}
	else if (alpha < 25)
	{
		pos = 314;
		att |= dps::DEUATT_FLASHING;
	}
	else pos = 594 - Round( 11.2 * alpha );
	pMDU->Line( 33, pos, 25, pos + 6, att );
	pMDU->Line( 25, pos + 6, 25, pos - 6, att );
	pMDU->Line( 25, pos - 6, 33, pos, att );

	att = dps::DEUATT_OVERBRIGHT;
	if (alphacmd > 50)
	{
		pos = 34;
		att |= dps::DEUATT_FLASHING;
	}
	else if (alphacmd < 25)
	{
		pos = 314;
		att |= dps::DEUATT_FLASHING;
	}
	else pos = 594 - Round( 11.2 * alphacmd );
	pMDU->Line( 33, pos, 27, pos + 6, att );
	pMDU->Line( 27, pos + 6, 27, pos + 2, att );
	pMDU->Line( 27, pos + 2, 15, pos + 2, att );
	pMDU->Line( 15, pos + 2, 15, pos - 2, att );
	pMDU->Line( 15, pos - 2, 27, pos - 2, att );
	pMDU->Line( 27, pos - 2, 27, pos - 6, att );
	pMDU->Line( 27, pos - 6, 33, pos, att );

	att = dps::DEUATT_OVERBRIGHT;
	tmp = (length( drag ) / OrbiterMass) * MPS2FPS;
	if (tmp > 50)
	{
		pos = 34;
		att |= dps::DEUATT_FLASHING;
	}
	else if (tmp < 0)
	{
		pos = 314;
		att |= dps::DEUATT_FLASHING;
	}
	else pos = 314 - Round( 5.6 * tmp );
	pMDU->Line( 35, pos, 43, pos + 6, att );
	pMDU->Line( 43, pos + 6, 43, pos - 6, att );
	pMDU->Line( 43, pos - 6, 35, pos, att );

	att = dps::DEUATT_OVERBRIGHT;
	tmp = refDrag * MPS2FPS;
	if (tmp > 50)
	{
		pos = 34;
		att |= dps::DEUATT_FLASHING;
	}
	else if (tmp < 0)
	{
		pos = 314;
		att |= dps::DEUATT_FLASHING;
	}
	else pos = 314 - Round( 5.6 * tmp );
	pMDU->Line( 35, pos, 41, pos + 6, att );
	pMDU->Line( 41, pos + 6, 41, pos + 2, att );
	pMDU->Line( 41, pos + 2, 53, pos + 2, att );
	pMDU->Line( 53, pos + 2, 53, pos - 2, att );
	pMDU->Line( 53, pos - 2, 41, pos - 2, att );
	pMDU->Line( 41, pos - 2, 41, pos - 6, att );
	pMDU->Line( 41, pos - 6, 35, pos, att );

	// orbiter symbol
	int x = x = Round( (ETVS_Range * 1.731918) - 615.746938 - (ETVS_Range * ETVS_Range * 0.000666122) );
	int y = Round( 1773.3333 - ET_Mach * 102.6667 );
	att = dps::DEUATT_OVERBRIGHT;
	if (0) att |= dps::DEUATT_FLASHING;// TODO on roll reversal
	pMDU->OrbiterSymbolSide( x, y, 0, att );

	// orbiter trailers
	for (int i = 5; i--;)
	{
		if (ET_RangeHistory[i] != 0)
		{
			x = Round( (ET_RangeHistory[i] * 1.731918) - 615.746938 - (ET_RangeHistory[i] * ET_RangeHistory[i] * 0.000666122) );
			y = Round( 1773.3333 - ET_MachHistory[i] * 102.6667 );
			pMDU->Line( x, y + 4, x - 6, y - 4, dps::DEUATT_OVERBRIGHT );
			pMDU->Line( x - 6, y - 4, x + 6, y - 4, dps::DEUATT_OVERBRIGHT );
			pMDU->Line( x + 6, y - 4, x, y + 4, dps::DEUATT_OVERBRIGHT );
		}
	}
	return;
}

void AerojetDAP::PaintENTRYTRAJ3Display( vc::MDU* pMDU ) const
{
	char cbuf[8];
	PrintCommonHeader( "  ENTRY TRAJ 3", pMDU );

	pMDU->Alpha( 1, 1 );
	pMDU->mvprint( 0, 2, "45" );
	pMDU->mvprint( 0, 6, "40" );
	pMDU->mvprint( 0, 10, "35" );
	pMDU->mvprint( 0, 14, "30" );
	pMDU->mvprint( 0, 18, "25" );
	pMDU->mvprint( 0, 22, "20" );
	pMDU->mvprint( 4, 1, "D" );
	pMDU->mvprint( 4, 2, "50" );
	pMDU->mvprint( 4, 6, "40" );
	pMDU->mvprint( 4, 10, "30" );
	pMDU->mvprint( 4, 14, "20" );
	pMDU->mvprint( 4, 18, "10" );
	pMDU->mvprint( 4, 22, "0" );
	pMDU->mvprint( 7, 4, "1 BIAS" );
	pMDU->mvprint( 7, 5, "D REF" );
	pMDU->mvprint( 7, 6, "q" );
	pMDU->Line( 70, 84, 80, 84 );
	pMDU->Delta( 7, 7 );
	pMDU->mvprint( 9, 7, "AZ" );
	//pMDU->mvprint( 7, 9, "LO ENRGY" );
	//pMDU->mvprint( 7, 10, "3" );
	pMDU->mvprint( 38, 15, "NY" );
	pMDU->mvprint( 38, 16, "NY TRIM" );
	pMDU->mvprint( 38, 17, "AIL" );
	pMDU->mvprint( 38, 18, "RUD" );
	pMDU->mvprint( 37, 19, "ZERO H BIAS 2" );
	pMDU->DotCharacter( 42, 19 );
	pMDU->mvprint( 38, 20, "H BIAS" );
	pMDU->DotCharacter( 38, 20 );
	pMDU->mvprint( 41, 21, "REF" );
	pMDU->mvprint( 36, 22, "ROLL REF" );
	pMDU->mvprint( 41, 23, "CMD" );
	pMDU->mvprint( 28, 2, "45D" );
	pMDU->mvprint( 37, 2, "35D" );
	pMDU->mvprint( 42, 2, "25D" );
	pMDU->mvprint( 16, 23, "-210" );
	pMDU->mvprint( 31, 23, "-130" );

	// phugoid scale lines
	pMDU->Line( 70, 30, 250, 30 );
	pMDU->Line( 70, 30, 70, 36 );
	pMDU->Line( 160, 30, 160, 36 );
	pMDU->Line( 250, 30, 250, 36 );

	// alpha/D scale lines
	pMDU->Line( 34, 34, 34, 314 );
	pMDU->Line( 30, 34, 38, 34 );
	pMDU->Line( 30, 45, 38, 45 );
	pMDU->Line( 30, 56, 38, 56 );
	pMDU->Line( 30, 68, 38, 68 );
	pMDU->Line( 30, 79, 38, 79 );
	pMDU->Line( 30, 90, 38, 90 );
	pMDU->Line( 30, 101, 38, 101 );
	pMDU->Line( 30, 112, 38, 112 );
	pMDU->Line( 30, 124, 38, 124 );
	pMDU->Line( 30, 135, 38, 135 );
	pMDU->Line( 30, 146, 38, 146 );
	pMDU->Line( 30, 157, 38, 157 );
	pMDU->Line( 30, 168, 38, 168 );
	pMDU->Line( 30, 180, 38, 180 );
	pMDU->Line( 30, 191, 38, 191 );
	pMDU->Line( 30, 202, 38, 202 );
	pMDU->Line( 30, 213, 38, 213 );
	pMDU->Line( 30, 224, 38, 224 );
	pMDU->Line( 30, 236, 38, 236 );
	pMDU->Line( 30, 247, 38, 247 );
	pMDU->Line( 30, 258, 38, 258 );
	pMDU->Line( 30, 269, 38, 269 );
	pMDU->Line( 30, 280, 38, 280 );
	pMDU->Line( 30, 292, 38, 292 );
	pMDU->Line( 30, 303, 38, 303 );
	pMDU->Line( 30, 314, 38, 314 );

	// vel/rng lines
	pMDU->Line( 244, 40, 56, 300 );

	pMDU->Line( 396, 40, 192, 299 );

	pMDU->Line( 498, 44, 388, 190 );
	pMDU->Line( 388, 190, 296, 300 );

	// drag lines
	pMDU->Line( 292, 44, 84, 319, dps::DEUATT_DASHED );

	pMDU->Line( 380, 40, 174, 311, dps::DEUATT_DASHED );

	pMDU->Line( 440, 40, 232, 311, dps::DEUATT_DASHED );

	// digital data
	sprintf_s( cbuf, 8, "%5.1f", STS()->GetDynPressure() * PA2PSF );
	pMDU->mvprint( 12, 6, cbuf );

	sprintf_s( cbuf, 8, "%+5.1f", GetdeltaAZ() );
	pMDU->mvprint( 12, 7, cbuf );

	VECTOR3 lift, drag, gravity;
	STS()->GetLiftVector(lift);
	STS()->GetDragVector(drag);
	STS()->GetWeightVector(gravity);
	double tmp = (lift.x+drag.x+gravity.x)/(OrbiterMass * G);
	sprintf_s( cbuf, 8, "%.3f", fabs( tmp ) );
	if (tmp > 0) cbuf[0] = 'R';
	else if (tmp < 0) cbuf[0] = 'L';
	else cbuf[0] = ' ';
	pMDU->mvprint( 46, 15, cbuf );

	// scale data
	char att = dps::DEUATT_OVERBRIGHT;
	double alpha = STS()->GetAOA() * DEG;
	double alphacmd = CalculateTargetAOA(STS()->GetMachNumber());
	if (fabs( alpha - alphacmd ) > 2) att |= dps::DEUATT_FLASHING;
	int pos;
	if (alpha > 45)
	{
		pos = 34;
		att |= dps::DEUATT_FLASHING;
	}
	else if (alpha < 20)
	{
		pos = 314;
		att |= dps::DEUATT_FLASHING;
	}
	else pos = 538 - Round( 11.2 * alpha );
	pMDU->Line( 33, pos, 25, pos + 6, att );
	pMDU->Line( 25, pos + 6, 25, pos - 6, att );
	pMDU->Line( 25, pos - 6, 33, pos, att );

	att = dps::DEUATT_OVERBRIGHT;
	if (alphacmd > 45)
	{
		pos = 34;
		att |= dps::DEUATT_FLASHING;
	}
	else if (alphacmd < 20)
	{
		pos = 314;
		att |= dps::DEUATT_FLASHING;
	}
	else pos = 538 - Round( 11.2 * alphacmd );
	pMDU->Line( 33, pos, 27, pos + 6, att );
	pMDU->Line( 27, pos + 6, 27, pos + 2, att );
	pMDU->Line( 27, pos + 2, 15, pos + 2, att );
	pMDU->Line( 15, pos + 2, 15, pos - 2, att );
	pMDU->Line( 15, pos - 2, 27, pos - 2, att );
	pMDU->Line( 27, pos - 2, 27, pos - 6, att );
	pMDU->Line( 27, pos - 6, 33, pos, att );

	att = dps::DEUATT_OVERBRIGHT;
	tmp = (length( drag ) / OrbiterMass) * MPS2FPS;
	if (tmp > 50)
	{
		pos = 34;
		att |= dps::DEUATT_FLASHING;
	}
	else if (tmp < 0)
	{
		pos = 314;
		att |= dps::DEUATT_FLASHING;
	}
	else pos = 314 - Round( 5.6 * tmp );
	pMDU->Line( 35, pos, 43, pos + 6, att );
	pMDU->Line( 43, pos + 6, 43, pos - 6, att );
	pMDU->Line( 43, pos - 6, 35, pos, att );

	att = dps::DEUATT_OVERBRIGHT;
	tmp = refDrag * MPS2FPS;
	if (tmp > 50)
	{
		pos = 34;
		att |= dps::DEUATT_FLASHING;
	}
	else if (tmp < 0)
	{
		pos = 314;
		att |= dps::DEUATT_FLASHING;
	}
	else pos = 314 - Round( 5.6 * tmp );
	pMDU->Line( 35, pos, 41, pos + 6, att );
	pMDU->Line( 41, pos + 6, 41, pos + 2, att );
	pMDU->Line( 41, pos + 2, 53, pos + 2, att );
	pMDU->Line( 53, pos + 2, 53, pos - 2, att );
	pMDU->Line( 53, pos - 2, 41, pos - 2, att );
	pMDU->Line( 41, pos - 2, 41, pos - 6, att );
	pMDU->Line( 41, pos - 6, 35, pos, att );

	// orbiter symbol
	int x = Round( (ETVS_Range * 3.46902) - 877.60761 - (ETVS_Range * ETVS_Range * 0.00216814) );
	int y = 1260 - Round( ET_Mach * 88 );
	att = dps::DEUATT_OVERBRIGHT;
	if (0) att |= dps::DEUATT_FLASHING;// TODO on roll reversal
	pMDU->OrbiterSymbolSide( x, y, 0, att );

	// orbiter trailers
	for (int i = 5; i--;)
	{
		if (ET_RangeHistory[i] != 0)
		{
			x = Round( (ET_RangeHistory[i] * 3.46902) - 877.60761 - (ET_RangeHistory[i] * ET_RangeHistory[i] * 0.00216814) );
			y = 1260 - Round( ET_MachHistory[i] * 88 );
			pMDU->Line( x, y + 4, x - 6, y - 4, dps::DEUATT_OVERBRIGHT );
			pMDU->Line( x - 6, y - 4, x + 6, y - 4, dps::DEUATT_OVERBRIGHT );
			pMDU->Line( x + 6, y - 4, x, y + 4, dps::DEUATT_OVERBRIGHT );
		}
	}
	return;
}

void AerojetDAP::PaintENTRYTRAJ4Display( vc::MDU* pMDU ) const
{
	char cbuf[8];
	PrintCommonHeader( "  ENTRY TRAJ 4", pMDU );

	pMDU->Alpha( 1, 1 );
	pMDU->mvprint( 0, 2, "45" );
	pMDU->mvprint( 0, 6, "40" );
	pMDU->mvprint( 0, 10, "35" );
	pMDU->mvprint( 0, 14, "30" );
	pMDU->mvprint( 0, 18, "25" );
	pMDU->mvprint( 0, 22, "20" );
	pMDU->mvprint( 4, 1, "D" );
	pMDU->mvprint( 4, 2, "50" );
	pMDU->mvprint( 4, 6, "40" );
	pMDU->mvprint( 4, 10, "30" );
	pMDU->mvprint( 4, 14, "20" );
	pMDU->mvprint( 4, 18, "10" );
	pMDU->mvprint( 4, 22, "0" );
	pMDU->mvprint( 7, 4, "1 BIAS" );
	pMDU->mvprint( 7, 5, "D REF" );
	pMDU->mvprint( 7, 6, "q" );
	pMDU->Line( 70, 84, 80, 84 );
	pMDU->Delta( 7, 7 );
	pMDU->mvprint( 9, 7, "AZ" );
	//pMDU->mvprint( 7, 9, "LO ENRGY" );
	//pMDU->mvprint( 7, 10, "3" );
	pMDU->mvprint( 38, 15, "NY" );
	pMDU->mvprint( 38, 16, "NY TRIM" );
	pMDU->mvprint( 38, 17, "AIL" );
	pMDU->mvprint( 38, 18, "RUD" );
	pMDU->mvprint( 37, 19, "ZERO H BIAS 2" );
	pMDU->DotCharacter( 42, 19 );
	pMDU->mvprint( 38, 20, "H BIAS" );
	pMDU->DotCharacter( 38, 20 );
	pMDU->mvprint( 41, 21, "REF" );
	pMDU->mvprint( 36, 22, "ROLL REF" );
	pMDU->mvprint( 41, 23, "CMD" );
	pMDU->mvprint( 42, 2, "40D" );
	pMDU->mvprint( 47, 2, "30D" );
	pMDU->mvprint( 48, 11, "20D" );
	pMDU->mvprint( 42, 13, "-210" );
	pMDU->mvprint( 13, 23, "-255" );
	pMDU->mvprint( 30, 23, "-250" );

	// phugoid scale lines
	pMDU->Line( 70, 30, 250, 30 );
	pMDU->Line( 70, 30, 70, 36 );
	pMDU->Line( 160, 30, 160, 36 );
	pMDU->Line( 250, 30, 250, 36 );

	// alpha/D scale lines
	pMDU->Line( 34, 34, 34, 314 );
	pMDU->Line( 30, 34, 38, 34 );
	pMDU->Line( 30, 45, 38, 45 );
	pMDU->Line( 30, 56, 38, 56 );
	pMDU->Line( 30, 68, 38, 68 );
	pMDU->Line( 30, 79, 38, 79 );
	pMDU->Line( 30, 90, 38, 90 );
	pMDU->Line( 30, 101, 38, 101 );
	pMDU->Line( 30, 112, 38, 112 );
	pMDU->Line( 30, 124, 38, 124 );
	pMDU->Line( 30, 135, 38, 135 );
	pMDU->Line( 30, 146, 38, 146 );
	pMDU->Line( 30, 157, 38, 157 );
	pMDU->Line( 30, 168, 38, 168 );
	pMDU->Line( 30, 180, 38, 180 );
	pMDU->Line( 30, 191, 38, 191 );
	pMDU->Line( 30, 202, 38, 202 );
	pMDU->Line( 30, 213, 38, 213 );
	pMDU->Line( 30, 224, 38, 224 );
	pMDU->Line( 30, 236, 38, 236 );
	pMDU->Line( 30, 247, 38, 247 );
	pMDU->Line( 30, 258, 38, 258 );
	pMDU->Line( 30, 269, 38, 269 );
	pMDU->Line( 30, 280, 38, 280 );
	pMDU->Line( 30, 292, 38, 292 );
	pMDU->Line( 30, 303, 38, 303 );
	pMDU->Line( 30, 314, 38, 314 );

	// alt*vel/rng lines
	pMDU->Line( 358, 51, 192, 208 );
	pMDU->Line( 192, 208, 36, 319 );

	pMDU->Line( 438, 64, 350, 171 );
	pMDU->Line( 350, 171, 164, 302 );

	pMDU->Line( 486, 64, 452, 128 );
	pMDU->Line( 452, 128, 356, 224 );
	pMDU->Line( 356, 224, 192, 320 );

	// drag lines
	pMDU->Line( 422, 37, 258, 180, dps::DEUATT_DASHED );
	pMDU->Line( 258, 180, 212, 216, dps::DEUATT_DASHED );
	pMDU->Line( 212, 216, 24, 330, dps::DEUATT_DASHED );

	pMDU->Line( 484, 44, 358, 151, dps::DEUATT_DASHED );
	pMDU->Line( 358, 151, 272, 208, dps::DEUATT_DASHED );
	pMDU->Line( 272, 208, 66, 330, dps::DEUATT_DASHED );

	pMDU->Line( 464, 145, 292, 252, dps::DEUATT_DASHED );
	pMDU->Line( 292, 252, 168, 319, dps::DEUATT_DASHED );

	// digital data
	sprintf_s( cbuf, 8, "%5.1f", STS()->GetDynPressure() * PA2PSF );
	pMDU->mvprint( 12, 6, cbuf );

	sprintf_s( cbuf, 8, "%+5.1f", GetdeltaAZ() );
	pMDU->mvprint( 12, 7, cbuf );

	VECTOR3 lift, drag, gravity;
	STS()->GetLiftVector(lift);
	STS()->GetDragVector(drag);
	STS()->GetWeightVector(gravity);
	double tmp = (lift.x+drag.x+gravity.x)/(OrbiterMass * G);
	sprintf_s( cbuf, 8, "%.3f", fabs( tmp ) );
	if (tmp > 0) cbuf[0] = 'R';
	else if (tmp < 0) cbuf[0] = 'L';
	else cbuf[0] = ' ';
	pMDU->mvprint( 46, 15, cbuf );

	// scale data
	char att = dps::DEUATT_OVERBRIGHT;
	double alpha = STS()->GetAOA() * DEG;
	double alphacmd = CalculateTargetAOA(STS()->GetMachNumber());
	if (fabs( alpha - alphacmd ) > 2) att |= dps::DEUATT_FLASHING;
	int pos;
	if (alpha > 45)
	{
		pos = 34;
		att |= dps::DEUATT_FLASHING;
	}
	else if (alpha < 20)
	{
		pos = 314;
		att |= dps::DEUATT_FLASHING;
	}
	else pos = 538 - Round( 11.2 * alpha );
	pMDU->Line( 33, pos, 25, pos + 6, att );
	pMDU->Line( 25, pos + 6, 25, pos - 6, att );
	pMDU->Line( 25, pos - 6, 33, pos, att );

	att = dps::DEUATT_OVERBRIGHT;
	if (alphacmd > 45)
	{
		pos = 34;
		att |= dps::DEUATT_FLASHING;
	}
	else if (alphacmd < 20)
	{
		pos = 314;
		att |= dps::DEUATT_FLASHING;
	}
	else pos = 538 - Round( 11.2 * alphacmd );
	pMDU->Line( 33, pos, 27, pos + 6, att );
	pMDU->Line( 27, pos + 6, 27, pos + 2, att );
	pMDU->Line( 27, pos + 2, 15, pos + 2, att );
	pMDU->Line( 15, pos + 2, 15, pos - 2, att );
	pMDU->Line( 15, pos - 2, 27, pos - 2, att );
	pMDU->Line( 27, pos - 2, 27, pos - 6, att );
	pMDU->Line( 27, pos - 6, 33, pos, att );

	att = dps::DEUATT_OVERBRIGHT;
	tmp = (length( drag ) / OrbiterMass) * MPS2FPS;
	if (tmp > 50)
	{
		pos = 34;
		att |= dps::DEUATT_FLASHING;
	}
	else if (tmp < 0)
	{
		pos = 314;
		att |= dps::DEUATT_FLASHING;
	}
	else pos = 314 - Round( 5.6 * tmp );
	pMDU->Line( 35, pos, 43, pos + 6, att );
	pMDU->Line( 43, pos + 6, 43, pos - 6, att );
	pMDU->Line( 43, pos - 6, 35, pos, att );

	att = dps::DEUATT_OVERBRIGHT;
	tmp = refDrag * MPS2FPS;
	if (tmp > 50)
	{
		pos = 34;
		att |= dps::DEUATT_FLASHING;
	}
	else if (tmp < 0)
	{
		pos = 314;
		att |= dps::DEUATT_FLASHING;
	}
	else pos = 314 - Round( 5.6 * tmp );
	pMDU->Line( 35, pos, 41, pos + 6, att );
	pMDU->Line( 41, pos + 6, 41, pos + 2, att );
	pMDU->Line( 41, pos + 2, 53, pos + 2, att );
	pMDU->Line( 53, pos + 2, 53, pos - 2, att );
	pMDU->Line( 53, pos - 2, 41, pos - 2, att );
	pMDU->Line( 41, pos - 2, 41, pos - 6, att );
	pMDU->Line( 41, pos - 6, 35, pos, att );

	// orbiter symbol
	int x = Round( (ETVS_Range * 4.362664) - 537.03943 - (ETVS_Range * ETVS_Range * 0.00454444) );
	int y = Round( 555.9999 - (ET_Mach * ETVS_Altitude * 0.000293333) );
	att = dps::DEUATT_OVERBRIGHT;
	if (0) att |= dps::DEUATT_FLASHING;// TODO on roll reversal
	pMDU->OrbiterSymbolSide( x, y, 0, att );

	// orbiter trailers
	for (int i = 5; i--;)
	{
		if (ET_RangeHistory[i] != 0)
		{
			x = Round( (ET_RangeHistory[i] * 4.362664) - 537.03943 - (ET_RangeHistory[i] * ET_RangeHistory[i] * 0.00454444) );
			y = Round( 555.9999 - (ET_MachHistory[i] * ET_AltitudeHistory[i] * 0.000293333) );
			pMDU->Line( x, y + 4, x - 6, y - 4, dps::DEUATT_OVERBRIGHT );
			pMDU->Line( x - 6, y - 4, x + 6, y - 4, dps::DEUATT_OVERBRIGHT );
			pMDU->Line( x + 6, y - 4, x, y + 4, dps::DEUATT_OVERBRIGHT );
		}
	}
	return;
}

void AerojetDAP::PaintENTRYTRAJ5Display( vc::MDU* pMDU ) const
{
	char cbuf[8];
	PrintCommonHeader( "  ENTRY TRAJ 5", pMDU );

	pMDU->Alpha( 1, 1 );
	pMDU->mvprint( 0, 2, "30" );
	pMDU->mvprint( 0, 6, "25" );
	pMDU->mvprint( 0, 10, "20" );
	pMDU->mvprint( 0, 14, "15" );
	pMDU->mvprint( 0, 18, "10" );
	pMDU->mvprint( 0, 22, "5" );
	pMDU->mvprint( 4, 1, "D" );
	pMDU->mvprint( 4, 2, "50" );
	pMDU->mvprint( 4, 6, "40" );
	pMDU->mvprint( 4, 10, "30" );
	pMDU->mvprint( 4, 14, "20" );
	pMDU->mvprint( 4, 18, "10" );
	pMDU->mvprint( 4, 22, "0" );
	pMDU->mvprint( 7, 4, "1 BIAS" );
	pMDU->mvprint( 7, 5, "D REF" );
	pMDU->mvprint( 7, 6, "q" );
	pMDU->Line( 70, 84, 80, 84 );
	pMDU->Delta( 7, 7 );
	pMDU->mvprint( 9, 7, "AZ" );
	//pMDU->mvprint( 7, 9, "LO ENRGY" );
	//pMDU->mvprint( 7, 10, "3" );
	pMDU->mvprint( 38, 15, "NY" );
	pMDU->mvprint( 38, 16, "NY TRIM" );
	pMDU->mvprint( 38, 17, "AIL" );
	pMDU->mvprint( 38, 18, "RUD" );
	pMDU->mvprint( 37, 19, "ZERO H BIAS 2" );
	pMDU->DotCharacter( 42, 19 );
	pMDU->mvprint( 38, 20, "H BIAS" );
	pMDU->DotCharacter( 38, 20 );
	pMDU->mvprint( 41, 21, "REF" );
	pMDU->mvprint( 36, 22, "ROLL REF" );
	pMDU->mvprint( 41, 23, "CMD" );
	pMDU->mvprint( 42, 2, "30D" );
	pMDU->mvprint( 47, 2, "20D" );
	pMDU->mvprint( 44, 14, "-275" );
	pMDU->mvprint( 13, 23, "-245" );
	pMDU->mvprint( 30, 23, "-275" );

	// phugoid scale lines
	pMDU->Line( 70, 30, 250, 30 );
	pMDU->Line( 70, 30, 70, 36 );
	pMDU->Line( 160, 30, 160, 36 );
	pMDU->Line( 250, 30, 250, 36 );

	// alpha/D scale lines
	pMDU->Line( 34, 34, 34, 314 );
	pMDU->Line( 30, 34, 38, 34 );
	pMDU->Line( 30, 45, 38, 45 );
	pMDU->Line( 30, 56, 38, 56 );
	pMDU->Line( 30, 68, 38, 68 );
	pMDU->Line( 30, 79, 38, 79 );
	pMDU->Line( 30, 90, 38, 90 );
	pMDU->Line( 30, 101, 38, 101 );
	pMDU->Line( 30, 112, 38, 112 );
	pMDU->Line( 30, 124, 38, 124 );
	pMDU->Line( 30, 135, 38, 135 );
	pMDU->Line( 30, 146, 38, 146 );
	pMDU->Line( 30, 157, 38, 157 );
	pMDU->Line( 30, 168, 38, 168 );
	pMDU->Line( 30, 180, 38, 180 );
	pMDU->Line( 30, 191, 38, 191 );
	pMDU->Line( 30, 202, 38, 202 );
	pMDU->Line( 30, 213, 38, 213 );
	pMDU->Line( 30, 224, 38, 224 );
	pMDU->Line( 30, 236, 38, 236 );
	pMDU->Line( 30, 247, 38, 247 );
	pMDU->Line( 30, 258, 38, 258 );
	pMDU->Line( 30, 269, 38, 269 );
	pMDU->Line( 30, 280, 38, 280 );
	pMDU->Line( 30, 292, 38, 292 );
	pMDU->Line( 30, 303, 38, 303 );
	pMDU->Line( 30, 314, 38, 314 );

	// alt*vel/rng lines
	pMDU->Line( 378, 45, 222, 196 );
	pMDU->Line( 222, 196, 72, 308 );

	pMDU->Line( 450, 54, 358, 146 );
	pMDU->Line( 358, 146, 202, 252 );
	pMDU->Line( 202, 252, 92, 308 );

	pMDU->Line( 492, 50, 414, 143 );
	pMDU->Line( 414, 143, 298, 216 );
	pMDU->Line( 298, 216, 116, 308 );

	pMDU->Line( 510, 73, 460, 143 );
	pMDU->Line( 460, 143, 354, 218 );
	pMDU->Line( 354, 218, 160, 308 );

	// drag lines
	pMDU->Line( 420, 37, 260, 184, dps::DEUATT_DASHED );
	pMDU->Line( 260, 184, 170, 252, dps::DEUATT_DASHED );
	pMDU->Line( 170, 252, 26, 328, dps::DEUATT_DASHED );

	pMDU->Line( 484, 42, 358, 154, dps::DEUATT_DASHED );
	pMDU->Line( 358, 154, 274, 210, dps::DEUATT_DASHED );
	pMDU->Line( 274, 210, 68, 328, dps::DEUATT_DASHED );

	// digital data
	sprintf_s( cbuf, 8, "%5.1f", STS()->GetDynPressure() * PA2PSF );
	pMDU->mvprint( 12, 6, cbuf );

	sprintf_s( cbuf, 8, "%+5.1f", GetdeltaAZ() );
	pMDU->mvprint( 12, 7, cbuf );

	VECTOR3 lift, drag, gravity;
	STS()->GetLiftVector(lift);
	STS()->GetDragVector(drag);
	STS()->GetWeightVector(gravity);
	double tmp = (lift.x+drag.x+gravity.x)/(OrbiterMass * G);
	sprintf_s( cbuf, 8, "%.3f", fabs( tmp ) );
	if (tmp > 0) cbuf[0] = 'R';
	else if (tmp < 0) cbuf[0] = 'L';
	else cbuf[0] = ' ';
	pMDU->mvprint( 46, 15, cbuf );

	// scale data
	char att = dps::DEUATT_OVERBRIGHT;
	double alpha = STS()->GetAOA() * DEG;
	double alphacmd = CalculateTargetAOA(STS()->GetMachNumber());
	if (fabs( alpha - alphacmd ) > 2) att |= dps::DEUATT_FLASHING;
	int pos;
	if (alpha > 30)
	{
		pos = 34;
		att |= dps::DEUATT_FLASHING;
	}
	else if (alpha < 5)
	{
		pos = 314;
		att |= dps::DEUATT_FLASHING;
	}
	else pos = 370 - Round( 11.2 * alpha );
	pMDU->Line( 33, pos, 25, pos + 6, att );
	pMDU->Line( 25, pos + 6, 25, pos - 6, att );
	pMDU->Line( 25, pos - 6, 33, pos, att );

	att = dps::DEUATT_OVERBRIGHT;
	if (alphacmd > 30)
	{
		pos = 34;
		att |= dps::DEUATT_FLASHING;
	}
	else if (alphacmd < 5)
	{
		pos = 314;
		att |= dps::DEUATT_FLASHING;
	}
	else pos = 370 - Round( 11.2 * alphacmd );
	pMDU->Line( 33, pos, 27, pos + 6, att );
	pMDU->Line( 27, pos + 6, 27, pos + 2, att );
	pMDU->Line( 27, pos + 2, 15, pos + 2, att );
	pMDU->Line( 15, pos + 2, 15, pos - 2, att );
	pMDU->Line( 15, pos - 2, 27, pos - 2, att );
	pMDU->Line( 27, pos - 2, 27, pos - 6, att );
	pMDU->Line( 27, pos - 6, 33, pos, att );

	att = dps::DEUATT_OVERBRIGHT;
	tmp = (length( drag ) / OrbiterMass) * MPS2FPS;
	if (tmp > 50)
	{
		pos = 34;
		att |= dps::DEUATT_FLASHING;
	}
	else if (tmp < 0)
	{
		pos = 314;
		att |= dps::DEUATT_FLASHING;
	}
	else pos = 314 - Round( 5.6 * tmp );
	pMDU->Line( 35, pos, 43, pos + 6, att );
	pMDU->Line( 43, pos + 6, 43, pos - 6, att );
	pMDU->Line( 43, pos - 6, 35, pos, att );

	att = dps::DEUATT_OVERBRIGHT;
	tmp = refDrag * MPS2FPS;
	if (tmp > 50)
	{
		pos = 34;
		att |= dps::DEUATT_FLASHING;
	}
	else if (tmp < 0)
	{
		pos = 314;
		att |= dps::DEUATT_FLASHING;
	}
	else pos = 314 - Round( 5.6 * tmp );
	pMDU->Line( 35, pos, 41, pos + 6, att );
	pMDU->Line( 41, pos + 6, 41, pos + 2, att );
	pMDU->Line( 41, pos + 2, 53, pos + 2, att );
	pMDU->Line( 53, pos + 2, 53, pos - 2, att );
	pMDU->Line( 53, pos - 2, 41, pos - 2, att );
	pMDU->Line( 41, pos - 2, 41, pos - 6, att );
	pMDU->Line( 41, pos - 6, 35, pos, att );

	// orbiter symbol
	int x = Round( (ETVS_Range * 8.242424) - 396.666666 - (ETVS_Range * ETVS_Range * 0.01873278) );
	int y = 448 - Round( ET_Mach * ETVS_Altitude * 0.00056 );
	att = dps::DEUATT_OVERBRIGHT;
	if (0) att |= dps::DEUATT_FLASHING;// TODO on roll reversal
	pMDU->OrbiterSymbolSide( x, y, 0, att );

	// orbiter trailers
	for (int i = 5; i--;)
	{
		if (ET_RangeHistory[i] != 0)
		{
			x = Round( (ET_RangeHistory[i] * 8.242424) - 396.666666 - (ET_RangeHistory[i] * ET_RangeHistory[i] * 0.01873278) );
			y = 448 - Round( ET_MachHistory[i] * ET_AltitudeHistory[i] * 0.00056 );
			pMDU->Line( x, y + 4, x - 6, y - 4, dps::DEUATT_OVERBRIGHT );
			pMDU->Line( x - 6, y - 4, x + 6, y - 4, dps::DEUATT_OVERBRIGHT );
			pMDU->Line( x + 6, y - 4, x, y + 4, dps::DEUATT_OVERBRIGHT );
		}
	}
	return;
}

void AerojetDAP::PaintVERTSIT1Display( vc::MDU* pMDU ) const
{
	char cbuf[8];
	PrintCommonHeader( "  VERT SIT 1", pMDU );

	pMDU->mvprint( 39, 5, "272" );
	pMDU->mvprint( 40, 7, "240" );
	pMDU->mvprint( 14, 11, "269" );
	pMDU->mvprint( 23, 14, "214" );
	pMDU->mvprint( 13, 15, "240" );
	pMDU->mvprint( 4, 16, "266" );
	pMDU->mvprint( 9, 18, "227" );

	pMDU->Theta( 42, 8 );
	pMDU->mvprint( 48, 8, "E/W" );
	pMDU->mvprint( 48, 12, "STN" );
	pMDU->mvprint( 48, 15, "NOM" );
	pMDU->mvprint( 48, 20, "MEP" );

	pMDU->mvprint( 36, 12, "NOSE HI" );
	pMDU->mvprint( 28, 14, "SPD BK" );
	pMDU->mvprint( 31, 15, "CMD" );
	pMDU->mvprint( 36, 20, "NOSE LO" );

	pMDU->mvprint( 20, 17, "NY" );
	pMDU->mvprint( 20, 18, "NY TRIM" );
	pMDU->mvprint( 20, 19, "AIL" );
	pMDU->mvprint( 20, 20, "RUD" );
	//pMDU->mvprint( 20, 21, "TGT NZ" );// OPS 6 only

	// alt/rng lines
	pMDU->Line( 404, 90, 180, 160 );
	pMDU->Line( 180, 160, 74, 230 );
	pMDU->Line( 74, 230, 24, 308 );

	pMDU->Line( 400, 112, 216, 162 );
	pMDU->Line( 216, 162, 60, 311 );

	pMDU->Line( 404, 121, 238, 188 );
	pMDU->Line( 238, 188, 104, 300 );

	// RTLS alpha/mach transition
	pMDU->Line( 214, 92, 42, 112 );
	pMDU->Line( 42, 112, 10, 126 );
	pMDU->Line( 210, 62, 10, 98, dps::DEUATT_DASHED );

	// theta / E/W scale
	pMDU->Line( 464, 115, 464, 339 );
	pMDU->Line( 460, 179, 470, 179 );
	pMDU->Line( 462, 210, 476, 210, dps::DEUATT_OVERBRIGHT );
	pMDU->Line( 460, 213, 470, 213, dps::DEUATT_OVERBRIGHT );
	pMDU->Line( 462, 216, 476, 216, dps::DEUATT_OVERBRIGHT );
	pMDU->Line( 460, 292, 470, 292 );

	// digital data
	sprintf_s( cbuf, 8, "%3.0f", STS()->aerosurfaces.speedbrake );
	pMDU->mvprint( 36, 14, cbuf );
	sprintf_s( cbuf, 8, "%3.0f", STS()->spdb_tgt * 100 );
	pMDU->mvprint( 36, 15, cbuf );

	VECTOR3 lift, drag, gravity;
	STS()->GetLiftVector(lift);
	STS()->GetDragVector(drag);
	STS()->GetWeightVector(gravity);
	double tmp = (lift.x+drag.x+gravity.x)/(OrbiterMass * G);
	sprintf_s( cbuf, 8, "%.3f", fabs( tmp ) );
	if (tmp > 0) cbuf[0] = 'R';
	else if (tmp < 0) cbuf[0] = 'L';
	else cbuf[0] = ' ';
	pMDU->mvprint( 28, 17, cbuf );

	// orbiter symbol
	int x = Round( (ETVS_Range - 10) * 8.5 );
	int y = Round( 467.999999999933 - (ETVS_Altitude * 0.004399999956) );
	VECTOR3 vel;
	STS()->GetHorizonAirspeedVector( vel );
	double rot = atan( vel.y / (0.192 * sqrt( vel.x * vel.x + vel.z * vel.z )) );
	pMDU->OrbiterSymbolSide( x, y, rot, dps::DEUATT_OVERBRIGHT );
	return;
}

void AerojetDAP::PaintVERTSIT2Display( vc::MDU* pMDU ) const
{
	char cbuf[8];
	PrintCommonHeader( "  VERT SIT 2", pMDU );

	pMDU->mvprint( 17, 2, "317" );
	pMDU->mvprint( 26, 2, "255" );
	pMDU->mvprint( 42, 3, "214" );
	pMDU->mvprint( 15, 11, "270" );
	pMDU->mvprint( 14, 16, "214" );
	pMDU->mvprint( 5, 17, "317" );
	pMDU->mvprint( 4, 20, "285" );

	pMDU->Theta( 43, 6 );
	pMDU->mvprint( 48, 6, "E/W" );
	pMDU->mvprint( 48, 11, "STN" );
	pMDU->mvprint( 48, 15, "NOM" );
	pMDU->mvprint( 48, 20, "MEP" );

	pMDU->mvprint( 38, 11, "NOSE HI" );
	pMDU->mvprint( 29, 14, "SPD BK" );
	pMDU->mvprint( 32, 15, "CMD" );
	pMDU->mvprint( 38, 20, "NOSE LO" );

	pMDU->mvprint( 18, 16, "ACCEL" );
	pMDU->mvprint( 21, 17, "NY" );
	pMDU->mvprint( 21, 18, "NY TRIM" );
	pMDU->mvprint( 21, 19, "AIL" );
	pMDU->mvprint( 21, 20, "RUD" );
	//pMDU->mvprint( 21, 21, "TGT NZ" );// OPS 6 only

	if (GetApproachAndLandState() == true) pMDU->mvprint( 38, 17, "A/L", dps::DEUATT_FLASHING );

	// alt/rng lines
	pMDU->Line( 188, 45, 72, 269 );

	pMDU->Line( 266, 42, 72, 277 );
	
	pMDU->Line( 422, 54, 132, 227 );
	pMDU->Line( 132, 227, 72, 292 );

	pMDU->Line( 72, 269, 72, 292 );

	// theta / E/W scale
	pMDU->Line( 464, 82, 464, 339 );
	pMDU->Line( 460, 160, 470, 160 );
	pMDU->Line( 462, 207, 476, 207, dps::DEUATT_OVERBRIGHT );
	pMDU->Line( 460, 216, 470, 216, dps::DEUATT_OVERBRIGHT );
	pMDU->Line( 462, 221, 476, 221, dps::DEUATT_OVERBRIGHT );
	pMDU->Line( 460, 286, 470, 286 );

	// digital data
	sprintf_s( cbuf, 8, "%3.0f", STS()->aerosurfaces.speedbrake );
	pMDU->mvprint( 38, 14, cbuf );
	sprintf_s( cbuf, 8, "%3.0f", STS()->spdb_tgt * 100 );
	pMDU->mvprint( 38, 15, cbuf );

	VECTOR3 lift, drag, gravity;
	STS()->GetLiftVector(lift);
	STS()->GetDragVector(drag);
	STS()->GetWeightVector(gravity);
	double tmp = (lift.x+drag.x+gravity.x)/(OrbiterMass * G);
	sprintf_s( cbuf, 8, "%.3f", fabs( tmp ) );
	if (tmp > 0) cbuf[0] = 'R';
	else if (tmp < 0) cbuf[0] = 'L';
	else cbuf[0] = ' ';
	pMDU->mvprint( 29, 17, cbuf );

	// orbiter symbol
	int x = Round( (ETVS_Range - 5) * 25.5 );
	int y = 448 - Round( ETVS_Altitude * 0.014 );
	VECTOR3 vel;
	STS()->GetHorizonAirspeedVector( vel );
	double rot = atan( vel.y / (0.181 * sqrt( vel.x * vel.x + vel.z * vel.z )) );
	pMDU->OrbiterSymbolSide( x, y, rot, dps::DEUATT_OVERBRIGHT );
	return;
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
	else {
		rudderPos = 0.0;
	}
	ElevonCommand.SetLine(static_cast<float>(-elevonPos)); // PID controller output has opposite sign of required elevon direction
	AileronCommand.SetLine(static_cast<float>(-aileronPos));
	RudderCommand.SetLine(static_cast<float>(rudderPos));
}

void AerojetDAP::SetSpeedbrakeCommand(double range, double DeltaT)
{
	double speedbrakeCMD = 0.0;

	// calc auto comand
	if (bWOW) AutoSpeedbrakeCommand = 1.0;
	else AutoSpeedbrakeCommand = CalculateSpeedbrakeCommand( range, DeltaT ) / 100.0;

	// AUTO/MAN logic
	if (SpeedbrakeState == AUTO)
	{
		if (pSBTC_SOP->GetCDRTakeover() == true)
		{
			// go cdr
			SpeedbrakeState = MAN_CDR;
			CDR_SPDBK_THROT_AUTO_LT.ResetLine();
			PLT_SPDBK_THROT_AUTO_LT.ResetLine();
			CDR_SPDBK_THROT_MAN_LT.SetLine();
			PLT_SPDBK_THROT_MAN_LT.ResetLine();
			speedbrakeCMD = pSBTC_SOP->GetManSpeedbrakeCommand() / 98.6;
		}
		else if (pSBTC_SOP->GetPLTTakeover() == true)
		{
			// go plt
			SpeedbrakeState = MAN_PLT;
			CDR_SPDBK_THROT_AUTO_LT.ResetLine();
			PLT_SPDBK_THROT_AUTO_LT.ResetLine();
			CDR_SPDBK_THROT_MAN_LT.ResetLine();
			PLT_SPDBK_THROT_MAN_LT.SetLine();
			speedbrakeCMD = pSBTC_SOP->GetManSpeedbrakeCommand() / 98.6;
		}
		else
		{
			// auto
			speedbrakeCMD = AutoSpeedbrakeCommand;
		}
	}
	else
	{
		if (CDR_SPDBK_THROT.IsSet() || PLT_SPDBK_THROT.IsSet())
		{
			// go auto
			SpeedbrakeState = AUTO;
			CDR_SPDBK_THROT_AUTO_LT.SetLine();
			PLT_SPDBK_THROT_AUTO_LT.SetLine();
			CDR_SPDBK_THROT_MAN_LT.ResetLine();
			PLT_SPDBK_THROT_MAN_LT.ResetLine();
			speedbrakeCMD = AutoSpeedbrakeCommand;
		}
		else if (pSBTC_SOP->GetCDRTakeover() == true)
		{
			// go cdr
			SpeedbrakeState = MAN_CDR;
			CDR_SPDBK_THROT_AUTO_LT.ResetLine();
			PLT_SPDBK_THROT_AUTO_LT.ResetLine();
			CDR_SPDBK_THROT_MAN_LT.SetLine();
			PLT_SPDBK_THROT_MAN_LT.ResetLine();
			speedbrakeCMD = pSBTC_SOP->GetManSpeedbrakeCommand() / 98.6;
		}
		else if (pSBTC_SOP->GetPLTTakeover() == true)
		{
			// go plt
			SpeedbrakeState = MAN_PLT;
			CDR_SPDBK_THROT_AUTO_LT.ResetLine();
			PLT_SPDBK_THROT_AUTO_LT.ResetLine();
			CDR_SPDBK_THROT_MAN_LT.ResetLine();
			PLT_SPDBK_THROT_MAN_LT.SetLine();
			speedbrakeCMD = pSBTC_SOP->GetManSpeedbrakeCommand() / 98.6;
		}
		else
		{
			// man
			speedbrakeCMD = pSBTC_SOP->GetManSpeedbrakeCommand() / 98.6;
		}
	}

	// limit limit to >=15% if M>0.6
	if ((STS()->GetMachNumber() > 0.6) && (STS()->GetMachNumber() < 9.814815))// use M9.8 instead of M10 so initial ramp occurs
		speedbrakeCMD = max( speedbrakeCMD, 0.15 );

	// set command
	STS()->SetSpeedbrake( speedbrakeCMD );
	return;
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
	return (pRHC_SOP->GetPitchCommand() / 24.3) * 6.0 + STS()->GetControlSurfaceLevel( AIRCTRL_ELEVATORTRIM ) * 2.5;
}


double AerojetDAP::CSSRollInput(double DeltaT)
{
	return (pRHC_SOP->GetRollCommand() / 24.3) * 5.0;
}

void AerojetDAP::GetAttitudeData(double DeltaT)
{
	//double lastSideslip = degCurrentAttitude.data[YAW];
	//VECTOR3 degLastAttitude = degCurrentAttitude;

	// get AOA, sideslip and bank
	if(GetMajorMode()==304)
		degCurrentAttitude.data[PITCH] = STS()->GetAOA()*DEG;
	else
		degCurrentAttitude.data[PITCH] = STS()->GetPitch()*DEG;
	degCurrentAttitude.data[YAW] = STS()->GetSlipAngle()*DEG;
	// for AUTO control, we are interested in bank around lift vector; for CSS control, use actual bank
	if (AutoFCSRoll == false)
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
	GetShuttleVerticalAeroCoefficients(mach, targetAOA, STS()->GetAltitude(), &(STS()->aerosurfaces), &cl, &cm, &cd);
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
		
		//last_vel = vec.y;

		//double actBank = CalculateCurrentLiftBank();
		tgtBank = tgtBankSign*CalculateRequiredLiftBank(target_vacc);
		//if(ThrustersActive[PITCH]) {
		/*if(STS()->GetMachNumber() > 15.0) {
		// limit bank angle (after initial roll) to > 50 degrees
		// this limits wild roll oscillations
		if(abs(degCurrentAttitude.data[ROLL]) > 40.0 && abs(tgtBank) < 50.0) tgtBank = tgtBankSign*50.0;
		}*/
			
	}
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
	//const double GS2 = 0.0001;
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

	if(SimT >= vspeedUpdateSimT) {
		double dt = SimT - (vspeedUpdateSimT-0.2);
		double ref_vspeed = range(-250.0, (tgtAltAveraging.GetAvgValue()-lastTgtAltitude)/dt, 250.0);
		//if(lastTargetAltitudes.size() <= 1) ref_vspeed = 0.0;
		vspeedAveraging.NewValue(ref_vspeed, SimT);

		vspeedUpdateSimT = SimT + 0.2;
		lastTgtAltitude = tgtAltAveraging.GetAvgValue();
	}

	if(SimT >= vaccUpdateSimT) {
		double dt = SimT - (vaccUpdateSimT-1.0);
		double ref_vacc = range(-10.0, (vspeedAveraging.GetAvgValue()-lastRefVSpeed)/dt, 10.0);
		//if(lastVspeeds.size() <= 1) ref_vacc = 0.0;
		vaccAveraging.NewValue(ref_vacc, SimT);

		vaccUpdateSimT = SimT + 1.0;
		lastRefVSpeed = vspeedAveraging.GetAvgValue();
	}
}

void AerojetDAP::UpdateRollDirection(double mach, double delaz)
{
	if(EntryGuidanceMode == PREENTRY) {
		tgtBankSign = -sign(delaz);
	}
	else {
		if(abs(delaz) >= GetdeltaAZLimit( mach )) {
			tgtBankSign = -sign(delaz);
			performedFirstRollReversal = true;
		}
	}
}

double AerojetDAP::GetdeltaAZLimit( double mach ) const
{
	if (!performedFirstRollReversal) return 10.5;
	if (mach < 4.0) return range(10, (mach * 6.25) - 7.5, 17.5);
	return 17.5;
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
	DistanceToHACCenter = radius;// save distance
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
		
		TimeToHAC = (radius - 1.1*HAC_TurnRadius) / ( 0.6 * length( _V( horz_airspeed.x, 0.0, horz_airspeed.z )) );// HACK crude estimation of time to HAC
		if (TimeToHAC < 0) TimeToHAC = 0;
		if (HACSide == L) TimeToHAC = -TimeToHAC;// use sign to tell which direction to turn
	}
	else {
		double rdot = -(velocity.x*(HAC_CENTER_X-TgtPos.x) + velocity.y*(HAC_CENTER_Y-TgtPos.y))/radius;
		double phi_ref = DEG*(velocity.x*velocity.x+velocity.y*velocity.y - rdot*rdot)/(G*HAC_TurnRadius);
		double rdotrf = -DEG*sqrt(velocity.x*velocity.x+velocity.y*velocity.y)*(R1+2.0*R2*turn_angle)/HAC_TurnRadius;
		TargetBank = YSGN*max(0, phi_ref + (radius-HAC_TurnRadius)*GR + (rdot-rdotrf)*GRDOT);
	}
	
	NZCommand = CalculateNZCommand(velocity, TotalRange, -TgtPos.z, DeltaT);

	if(TotalRange < (-HAC_CENTER_X + DR3)) {
		//oapiWriteLog("Starting PRFNL phase");
		TAEMGuidanceMode = PRFNL;
	}
}

void AerojetDAP::CalculateTargetGlideslope(const VECTOR3& TgtPos, double DeltaT)
{
	//double HeadingError = atan2(TgtPos.y, -TgtPos.x)*DEG;
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
			// cubic alt profile
			tgtAlt += rangeToALI*rangeToALI*(CUBIC_C3 + rangeToALI*CUBIC_C4);
			tgtGs = AL_GS - rangeToALI*(2*CUBIC_C3 + 3*rangeToALI*CUBIC_C4);
			tgtGs = range(AL_GS, tgtGs, -LINEAR_GLIDESLOPE);
		}
		else {
			// for the moment, assume we're on final
			tgtGs = AL_GS;
		}
	}

	// convert altitude/glideslope errors to NZ command
	double horzSpeed = sqrt(velocity.x*velocity.x + velocity.y*velocity.y);

	double gain = range(0.3, 2.0 - (7.0e-5)*(curAlt*MPS2FPS), 1.0);
	double refHDot = horzSpeed*tgtGs;
	double HDotErr = refHDot + velocity.z;

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
		double QBarError = refQbar-filteredQBar;

		double command = 65.0 - QBar_Speedbrake.Step(QBarError, DeltaT);
		return range(LOWER_LIM, command, UPPER_LIM);
	}
	else if(STS()->GetAltitude() > 3000.0/MPS2FPS) {
		return range(LOWER_LIM, 65.0 - Vel_Speedbrake.Step(300.0-STS()->GetKEAS(), DeltaT), UPPER_LIM);
	}
	else {
		// what should happen is speedbrake is set to constant value below 3000 feet, and then updated at 500 feet
		// for the moment, just used fixed value of 15% (updated to 15% when S/B = NOM, 28% when S/B = SHORT, and 48% when S/B = ELS)
		if (SBControlLogic == SHORT) return 28.0;// SCOM reports about 12-13% extra for short field option
		else if (SBControlLogic == ELS) return 48.0;// ELS adds 20º to SHORT setting
		else return 15.0;
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
	
	degTargetGlideslope = min(degTargetGlideslope+(DeltaT*DEG*flareRate), -1.5);
	double NZ = (PREFLARE_RADIUS*flareRate*flareRate)/G; // NZ required to accelerate in circle
	NZ += (degTargetGlideslope-DEG*curGlideslope)*0.2; // correct for error in glideslope
	return NZ;
}

void AerojetDAP::InitializeRunwayData()
{
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
	double radLat, radLong, AMSL;
	vLandingSites[SITE_ID].GetRwyPosition(!SEC, radLat, radLong, AMSL); // get lat/long for end we want to land on
	RwyStart_EarthLocal = GetPositionVector(hEarth, radLat, radLong, oapiGetSize(hEarth) + AMSL);
	VECTOR3 end1 = RwyStart_EarthLocal/length(RwyStart_EarthLocal);

	degRwyHeading = vLandingSites[SITE_ID].GetRwyHeading(!SEC);
	VECTOR3 EastDir = _V(-sin(radLong), 0, cos(radLong));
	VECTOR3 x = RotateVector(end1, RAD*(degRwyHeading-90.0), EastDir);
	RwyEnd_EarthLocal = RwyStart_EarthLocal + x*vLandingSites[SITE_ID].GetRwyLength(!SEC);

	VECTOR3 y = crossp(end1, x);
	RwyRotMatrix = _M(x.x, x.y, x.z,
		y.x, y.y, y.z,
		-end1.x, -end1.y, -end1.z);
	RwyPos = mul(RwyRotMatrix, RwyStart_EarthLocal);
	
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
	double tgtLong, tgtLat, AMSL;
	vLandingSites[SITE_ID].GetRwyPosition(!SEC, tgtLat, tgtLong, AMSL);
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
	if (delaz < -180) delaz += 360;
	else if (delaz > 180) delaz -= 360;
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
	// (using a modified STS-115 table, where the numbers within brackets are the changes)
	vLandingSites.push_back(LandingSiteData(28.633112*RAD, -80.706234*RAD, 3.0, 28.596660*RAD, -80.682413*RAD, 3.0, 150.2505, 330.2505, "KSC15", "KSC33"));// 1
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "BEN36", "BEN18" ) );// 2
	vLandingSites.push_back( LandingSiteData( 37.201981 * RAD, -5.618836 * RAD, 0, 37.171786 * RAD, -5.632640 * RAD, 0.0, 200, 20, "MRN20", "MRN02", 11730/MPS2FPS, 60.96 ) );// 3
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "ZZA30L", "ZZA12R" ) );// 4
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "MYR36", "MYR18" ) );// 5
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "ILM06", "ILM24" ) );// 6
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "NKT32L", "NKT23R" ) );// 7
	vLandingSites.back().SetSecRunwayParameters( 4000, 90 );// length/width data above is for primary runway
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "NTU32R", "NTU23L" ) );// 8
	vLandingSites.back().SetSecRunwayParameters( 4000, 90 );// length/width data above is for primary runway
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "WAL28", "WAL04" ) );// 9
	vLandingSites.back().SetSecRunwayParameters( 4000, 90 );// length/width data above is for primary runway
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "DOV32", "DOV19" ) );// 10
	vLandingSites.back().SetSecRunwayParameters( 4000, 90 );// length/width data above is for primary runway
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "ACY31", "ACY13" ) );// 11
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "BEN36", "BEN18" ) );// 12
	vLandingSites.push_back( LandingSiteData( 37.201852 * RAD, -5.619978 * RAD, 0.0, 37.171246 * RAD, -5.632307 * RAD, 0.0, 197.8, 17.8, "MRN20", "MRN02", 11730/MPS2FPS, 60.96 ) );// 13
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "ZZA30L", "ZZA12R" ) );// 14
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "FOK06", "FOK24" ) );// 15
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "FMH32", "FMH23" ) );// 16
	vLandingSites.back().SetSecRunwayParameters( 4000, 90 );// length/width data above is for primary runway
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "PSM34", "PSM16" ) );// 17
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "YHZ23", "YHZ32" ) );// 18
	vLandingSites.back().SetSecRunwayParameters( 4000, 90 );// length/width data above is for primary runway
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "YJT09", "YJT27" ) );// 19
	vLandingSites.push_back( LandingSiteData( 47.625375 * RAD, -52.737635 * RAD, 0.0, 47.622485 * RAD, -52.771932 * RAD, 0.0, 262.9, 82.9, "YYT29", "YYT11", 8500/MPS2FPS, 60.96 ) );// 20
	vLandingSites.push_back( LandingSiteData( 48.946994 * RAD, -54.560357 * RAD, 0.0, 48.946406 * RAD, -54.554009 * RAD, 0.0, 191.0, 288.0, "YQX21", "YQX31", 9640 / MPS2FPS, 200 / MPS2FPS ) );// 21
	vLandingSites.back().SetSecRunwayParameters( 8040 / MPS2FPS, 200 / MPS2FPS );// length/width data above is for primary runway
	//vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "YYR26", "YYR34" ) );// 22
	//vLandingSites.back().SetSecRunwayParameters( 4000, 90 );// length/width data above is for primary runway
	vLandingSites.push_back( LandingSiteData( 13.323423 * RAD, -16.643910 * RAD, 0.0, 13.348584 * RAD, -16.664848 * RAD, 0.0, 321.0, 141.0, "BYD32", "BYD14", 11811 / MPS2FPS, 148 / MPS2FPS ) );// [22]
	vLandingSites.push_back( LandingSiteData( 38.766205 * RAD, -27.102996 * RAD, 0.0, 38.742958 * RAD, -27.079116 * RAD, 0.0, 141.3, 321.3, "LAJ15", "LAJ33", 10870/MPS2FPS, 91.44 ) );// 23
	//vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "BEJ01L", "BEJ19R" ) );// 24
	vLandingSites.push_back( LandingSiteData( 34.720606 * RAD, -120.567103 * RAD, 0.0, 34.750617 * RAD, -120.601304 * RAD, 0.0, 316.5, 136.5, "VBG30", "VBG12", 15000 / MPS2FPS, 200 / MPS2FPS ) );// [24]
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "IKF20", "IKF29" ) );// 25
	vLandingSites.back().SetSecRunwayParameters( 4000, 90 );// length/width data above is for primary runway
	vLandingSites.push_back( LandingSiteData( 52.694418 * RAD, -8.940019 * RAD, 0.0, 52.710227 * RAD, -8.910320 * RAD, 0.0, 48.7, 228.7, "INN06", "INN24", 9539 / MPS2FPS, 148 / MPS2FPS ) );// 26
	vLandingSites.back().SetSecRunwayParameters( 9699 / MPS2FPS, 148 / MPS2FPS );// length/width data above is for primary runway
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "FFA27", "FFA09" ) );// 27
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "KBO14L", "KBO32R" ) );// 28
	vLandingSites.push_back( LandingSiteData( 43.511129 * RAD, 4.932931 * RAD, 0.0, 43.540647 * RAD, 4.910444 * RAD, 0.0, 331.1, 151.1, "FMI33", "FMI15", 12300/MPS2FPS, 60.0456 ) );// 29
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "ESN03R", "ESN21L" ) );// 30
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "KKI15R", "KKI33L" ) );// 31
	vLandingSites.push_back( LandingSiteData( -7.319984 * RAD, 72.415794 * RAD, 0.0, -7.303044 * RAD, 72.387373 * RAD, 0.0, 301, 121, "JDG31", "JDG13", 12000/MPS2FPS, 60.96 ) );// 32
	vLandingSites.push_back( LandingSiteData( -27.624551 * RAD, 152.712429 * RAD, 0.0, -14.512784 * RAD, 132.367147 * RAD, 0.0, 159.3, 135, "AMB15", "PTN14", 10000 / MPS2FPS, 150 / MPS2FPS ) );// 33
	vLandingSites.back().SetSecRunwayParameters( 9003 / MPS2FPS, 150 / MPS2FPS );// length/width data above is for primary runway
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "JTY36", "JTY18" ) );// 34
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "GUA06L", "GUA24R" ) );// 35
	//vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "WAK28", "WAK10" ) );// 36
	vLandingSites.push_back( LandingSiteData( 32.358907 * RAD, -64.661495 * RAD, 0.0, 32.364412 * RAD, -64.692156 * RAD, 0.0, 282, 102, "BDA30", "BDA12", 8892 / MPS2FPS, 150 / MPS2FPS ) );// [36]
	vLandingSites.back().SetSecRunwayParameters( 9212 / MPS2FPS, 150 / MPS2FPS );// length/width data above is for primary runway [36]
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "HNL08R", "HNL26L" ) );// 37
	//vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "EDF24", "EDF06" ) );// 38
	vLandingSites.push_back( LandingSiteData( -27.173447 * RAD, -109.406801 * RAD, 0.0, -27.159263 * RAD, -109.436664 * RAD, 0.0, 298.1, 118.1, "EIP28", "EIP10", 10000 / MPS2FPS, 60.96 ) );// [38]
	vLandingSites.push_back( LandingSiteData( -18.053836 * RAD, -140.978030 * RAD, 0.0, -18.084687 * RAD, -140.944897 * RAD, 0.0, 134.4, 314.4, "HAO12", "HAO30", 10390/MPS2FPS, 44.8056 ) );// 39
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "AWG25", "AWG07" ) );// 40
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "HAW13", "HAW31" ) );// 41
	vLandingSites.push_back( LandingSiteData( 32.960364 * RAD, -106.417364 * RAD, 0.0, 32.950714 * RAD, -106.403254 * RAD, 0.0, 187.2, 244.4, "NOR17", "NOR23", 15000 / MPS2FPS, 300 / MPS2FPS ) );// 42
	vLandingSites.back().SetSecRunwayParameters( 15000 / MPS2FPS, 300 / MPS2FPS );// length/width data above is for primary runway
	vLandingSites.push_back( LandingSiteData( 32.932950 * RAD, -106.447422 * RAD, 0.0, 32.919583 * RAD, -106.423504 * RAD, 0.0, 64.4, 7.2, "NOR05", "NOR35", 15000 / MPS2FPS, 300 / MPS2FPS ) );// 43
	vLandingSites.back().SetSecRunwayParameters( 15000 / MPS2FPS, 300 / MPS2FPS );// length/width data above is for primary runway
	vLandingSites.push_back( LandingSiteData( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "EDW15", "EDW18L" ) );// 44
	vLandingSites.back().SetSecRunwayParameters( 4000, 90 );// length/width data above is for primary runway
	vLandingSites.push_back(LandingSiteData(34.9173476*RAD, -117.8595079*RAD, 704.0, 34.8941050*RAD, -117.9051869*RAD, 704.0, 238.16, 58.16, "EDW22", "EDW04"));// 45
}

bool AerojetDAP::GetAutoPitchState( void ) const
{
	return AutoFCSPitch;
}

bool AerojetDAP::GetAutoRollYawState( void ) const
{
	return AutoFCSRoll;
}

bool AerojetDAP::GetAutoSpeedbrakeState( void ) const
{
	return (SpeedbrakeState == AUTO);
}

double AerojetDAP::GetAutoSpeedbrakeCommand( void ) const
{
	return AutoSpeedbrakeCommand * 100.0;
}

const std::string& AerojetDAP::GetSelectedRunway( void ) const
{
	if (SEC == true) return vLandingSites[SITE_ID].GetSecRwyName();
	else return vLandingSites[SITE_ID].GetPriRwyName();
}

double AerojetDAP::GetRangeToRunway( void ) const
{
	if ((GetMajorMode() == 305) || (GetMajorMode() == 603)) return TotalRange / NMI2M;

	// code comes from CalculateRangeAndDELAZ(), can't use it that as it changes HAC_TurnRadius and messes up range/HAC size at A/L
	const double YSGN = (HACSide==L) ? -1.0 : 1.0;
	
	double lng, lat, rad;
	double tgtLong, tgtLat, AMSL;
	vLandingSites[SITE_ID].GetRwyPosition(!SEC, tgtLat, tgtLong, AMSL);
	STS()->GetEquPos(lng, lat, rad);
	VECTOR3 shuttlePos = GetPositionVector(hEarth, lat, lng, rad);

	double cosGreatCircle = dotp(shuttlePos/length(shuttlePos), HAC_Center/length(HAC_Center)); // CTHVC
	double sinGreatCircle = sqrt(1 - cosGreatCircle*cosGreatCircle);
	double SINB = HAC_TurnRadius/oapiGetSize(hEarth);
	double CTVWP1 = cosGreatCircle + 0.5*cosGreatCircle*SINB*SINB;
	double SBARCR = SINB/sinGreatCircle;
	double temp = range(-1, CTVWP1*SBARCR, 1);
	double A2 = acos(temp);
	temp = range(-1, CTVWP1, 1);
	double rangeToWP1 = acos(temp)*oapiGetSize(hEarth);
	double headingToHACCenter = atan2(sin(HAC_Long-lng)*cos(HAC_Lat), cos(lat)*sin(HAC_Lat) - sin(lat)*cos(HAC_Lat)*cos(HAC_Long-lng));
	double A3 = 0.5*PI - A2 + YSGN*(vLandingSites[SITE_ID].GetRwyHeading(!SEC)*RAD - headingToHACCenter);
	if(A3 < 0.0) A3 += 2*PI;
	double HAC_TurnAngle = A3*DEG;
	double HAC_range = FINAL_RADIUS*HAC_TurnAngle + 0.5*R1*HAC_TurnAngle*HAC_TurnAngle + (1/3)*R2*HAC_TurnAngle*HAC_TurnAngle*HAC_TurnAngle;
	return (rangeToWP1 + HAC_range*RAD - HAC_CENTER_X) / NMI2M;
}

double AerojetDAP::GetdeltaAZ( void ) const
{
	// code comes from CalculateRangeAndDELAZ(), can't use it that as it changes HAC_TurnRadius and messes up range/HAC size at A/L
	const double YSGN = (HACSide==L) ? -1.0 : 1.0;
	
	double lng, lat, rad;
	double tgtLong, tgtLat, AMSL;
	vLandingSites[SITE_ID].GetRwyPosition(!SEC, tgtLat, tgtLong, AMSL);
	STS()->GetEquPos(lng, lat, rad);
	VECTOR3 shuttlePos = GetPositionVector(hEarth, lat, lng, rad);
	
	VECTOR3 v;
	STS()->GetHorizonAirspeedVector(v);
	double actualHeading = atan2(v.x, v.z);

	double cosGreatCircle = dotp(shuttlePos/length(shuttlePos), HAC_Center/length(HAC_Center)); // CTHVC
	double sinGreatCircle = sqrt(1 - cosGreatCircle*cosGreatCircle);
	double SINB = HAC_TurnRadius/oapiGetSize(hEarth);
	double SBARCR = SINB/sinGreatCircle;
	double temp = range(-1, SBARCR, 1);
	double T8 = asin(temp);
	double headingToHACCenter = atan2(sin(HAC_Long-lng)*cos(HAC_Lat), cos(lat)*sin(HAC_Lat) - sin(lat)*cos(HAC_Lat)*cos(HAC_Long-lng));
	double headingToWP1 = headingToHACCenter - YSGN*T8; // this is not quite the same as in documents, but should work

	double dAZ = DEG*(actualHeading-headingToWP1);
	if (dAZ < -180) dAZ += 360;
	else if (dAZ > 180) dAZ -= 360;
	return dAZ;
}

bool AerojetDAP::GetOnHACState( void ) const
{
	return (TAEMGuidanceMode >= HDG);
}

bool AerojetDAP::GetPrefinalState( void ) const
{
	return (TAEMGuidanceMode >= PRFNL);
}

bool AerojetDAP::GetApproachAndLandState( void ) const
{
	return (TAEMGuidanceMode >= OGS);
}

double AerojetDAP::GetVacc( void ) const
{
	// code below is heavily based on Hielor's post (http://www.orbiter-forum.com/showthread.php?t=5072)

	VECTOR3 force_vec, acc_vec, spd_vec;
 
	// Get the vectors we need
	STS()->GetShipAirspeedVector(spd_vec);
	STS()->GetForceVector(force_vec);
 
	// Normalize the speed vector
	spd_vec = spd_vec / length(spd_vec);
 
	// Calculate the acceleration vector
	acc_vec = force_vec / STS()->GetMass();

	double vacc, lon, lat, radius, mag;
	VECTOR3 horacc_vec;
	VECTOR3 spd_vec2, glob_vpos, glob_rvel, loc_rvel;
 
	// VACC
	STS()->HorizonRot(acc_vec, horacc_vec);
	vacc = horacc_vec.y;
 
	// Account for "centrifugal acceleration"
	// Get the relative velocity in the local frame
	STS()->GetGlobalPos(glob_vpos);
	STS()->GetRelativeVel(STS()->GetSurfaceRef(), glob_rvel);
	STS()->Global2Local((glob_rvel + glob_vpos), loc_rvel);
 
	// Transform to horizon reference frame
	STS()->HorizonRot(loc_rvel, spd_vec2);
 
	STS()->GetEquPos(lon, lat, radius);
 
	// Determine the centrifugal acceleration
	spd_vec2.y = 0;
	mag = length(spd_vec2);
	vacc += mag * mag / radius;
	return vacc * MPS2FPS;
}

bool AerojetDAP::GetWOW( void ) const
{
	return bWONG;
}

VECTOR3 AerojetDAP::GetAttitudeErrors( void ) const
{
	// HACK this is not the the attitude error (but it's better than nothing...?)
	return degTargetRates - degCurrentRates;
}

double AerojetDAP::GetYRunwayPositionError( void ) const
{
	return GetRunwayRelPos().y * MPS2FPS;
}

double AerojetDAP::GetTimeToHAC( void ) const
{
	return TimeToHAC;
}

double AerojetDAP::GetDistanceToHACCenter( void ) const
{
	return DistanceToHACCenter / NMI2M;
}

double AerojetDAP::GetHACRadialError( void ) const
{
	// use sign to tell which direction to turn
	if (HACSide == L) return -(DistanceToHACCenter - HAC_TurnRadius) * MPS2FPS;
	else return (DistanceToHACCenter - HAC_TurnRadius) * MPS2FPS;
}

double AerojetDAP::GetNZError( void ) const
{
	return NZErr / G;
}

double AerojetDAP::GetHTA( void ) const
{
	VECTOR3 pos = GetRunwayRelPos();
	double YSGN = (HACSide==L) ? -1.0 : 1.0;
	double HAC_CENTER_Y = YSGN * FINAL_RADIUS;

	pos.x -= HAC_CENTER_X;
	pos.y -= HAC_CENTER_Y;
	double hta = atan2( -pos.x, -YSGN * pos.y ) * DEG;
	if (hta < 0) hta += 360;
	return hta;
}

double AerojetDAP::GetGlideSlopeDistance( void ) const
{
	double dis;
	if (GetApproachAndLandState() == true)
	{
		// A/L
		VECTOR3 pos = GetRunwayRelPos();
		dis = -(pos.z + (AL_GS * (pos.x - OGS_AIMPOINT)));
	}
	else
	{
		// TODO TAEM
		dis = 0;
	}
	return dis * MPS2FPS;
}

double AerojetDAP::GetNZ( void ) const
{
	VECTOR3 lift, drag, gravity;
	STS()->GetLiftVector(lift);
	STS()->GetDragVector(drag);
	STS()->GetWeightVector(gravity);
	return (lift.y+drag.y)/length(gravity);
}

double AerojetDAP::GetSelectedRunwayHeading( void ) const
{
	return degRwyHeading;
}


};
