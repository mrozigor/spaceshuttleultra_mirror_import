#include "AerojetDAP.h"
#include "../Atlantis.h"
#include <UltraMath.h>
#include "../ParameterValues.h"


namespace dps
{

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

AerojetDAP::AerojetDAP(SimpleGPCSystem* _gpc)
: SimpleGPCSoftware(_gpc, "AerojetDAP"),
bFirstStep(true), bSecondStep(false), bWONG(false), OrbiterMass(1.0),
//AOA_ElevonPitch(0.25, 0.10, 0.01, -1.0, 1.0, -50.0, 50.0), //NOTE: may be better to reduce integral limits and increase i gain
//Rate_ElevonPitch(0.75, 0.001, 0.005, -0.75, 0.75, -60.0, 60.0),
//Pitch_ElevonPitch(0.25, 0.10, 0.01, -1.0, 1.0, -50.0, 50.0),
ElevonPitch(0.25, 0.10, 0.01, -1.0, 1.0, -50.0, 50.0), //NOTE: may be better to reduce integral limits and increase i gain
//Roll_AileronRoll(0.15, 0.05, 0.00, -1.0, 1.0),
Roll_AileronRoll(0.15, 0.15, 0.00, -1.0, 1.0),
Yaw_RudderYaw(0.15, 0.05, 0.00, -1.0, 1.0),
QBar_Speedbrake(1.5, 0.0, 0.1),
TAEMGuidanceMode(HDG), HACSide(L),
degTargetGlideslope(-20.0), // default OGS glideslope
prfnlBankFader(5.0), HAC_TurnRadius(20000.0/MPS2FPS), TotalRange(0.0),
lastNZUpdateTime(-1.0), averageNZ(0.0), curNZValueCount(0),
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
		CSSInitialized[i]=false;
	}

	degTargetAttitude = _V(0.0, 0.0, 0.0);
	degCurrentAttitude = _V(0.0, 0.0, 0.0);
	degCurrentRates = _V(0.0, 0.0, 0.0);
	//degTargetRates = _V(0.0, 0.0, 0.0);

	for(int i=0;i<NZ_VALUE_COUNT;i++) NZValues[i] = 0.0;

	LoadLandingSiteList(); // this might be done later in creation process

	dTable = new DragTable();
	first_roll = false;
	roll_command = false;
	last_vel = 0;
	last_h_error = 0;
	target_bank = 0;
	target_vspeed = 0;
	target_vacc = 0;

	
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
	RollYawAuto.Connect(pBundle, 2);

	pBundle=STS()->BundleManager()->CreateBundle("SBDBKTHROT_CONTROLS", 16);
	SpeedbrakeAuto.Connect(pBundle, 0);
	
	hEarth = STS()->GetGravityRef();
	InitializeRunwayData();
}

void AerojetDAP::OnPreStep(double SimT, double DeltaT, double MJD)
{
	// on first step, Orbiter gives some incorrect data, so ignore this step
	if(bFirstStep) {
		VECTOR3 gravity;
		STS()->GetWeightVector(gravity);
		gravity_force = length(gravity);
		filteredQBar = STS()->GetDynPressure()*PA2PSF;
		bFirstStep = false;
		bSecondStep = true;
		return;
	}

	if(HUDFlashTime <= SimT) {
		HUDFlashTime = SimT+0.1;
		bHUDFlasher = !bHUDFlasher;
	}

	//double distToRwy, delaz; // only used in MM304
	switch(GetMajorMode()) {
	case 304:
		UpdateOrbiterData();
		GetAttitudeData(DeltaT);

		CheckControlActivation();
		//SetThrusterLevels();

		// control yaw regardless of AUTO/CSS mode
		//if(ThrustersActive[YAW])
			//ThrusterCommands[YAW].SetLine(GetThrusterCommand(YAW));

		if(PitchAuto) {
			CSSInitialized[PITCH] = false;
			degTargetAttitude.data[PITCH] = CalculateTargetAOA(STS()->GetMachNumber());
			
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
			CSSPitchGuidance(DeltaT);
		}

		// roll AP isn't implemented yet, so just CSS guidance
		if(RollYawAuto)
		{
			CSSInitialized[ROLL] = false;
			CalculateTargetRoll(DeltaT);
			degTargetAttitude.data[ROLL] = target_bank;
		}
		else
		{
			CSSRollGuidance(DeltaT);
			double speed = STS()->GetAirspeed();
			double r, az;
			CalculateRangeAndDELAZ(r,az);
			double target_drag = dTable->TargetDrag(r,STS()->GetAirspeed());
			double target_altitude = dTable->TargetAltitude(target_drag,speed,STS()->GetAOA()*DEG,STS()->GetMass(),STS()->drag_coeff);
			sprintf(oapiDebugString(),"Target drag: %lf, Actual drag: %lf, range: %lf, Target altitude: %lf, Altitude error: %lf",target_drag,STS()->GetDrag()/STS()->GetMass(),r,target_altitude,target_altitude-STS()->GetAltitude());
		}

		// set thruster and aerosurface commands
		for(int i=0;i<3;i++) {
			if(ThrustersActive[i])
				ThrusterCommands[i].SetLine(static_cast<float>(GetThrusterCommand(static_cast<AXIS>(i), DeltaT)));
			else
				ThrusterCommands[i].SetLine(0.0f);
		}
		SetAerosurfaceCommands(DeltaT);

		//double tgtDrag = CalculateTargetDrag();
		//sprintf_s(oapiDebugString(), 255, "Tgt Drag: %f", tgtDrag);

		/*for(int i=0;i<3;i++) {
			if(AerosurfacesActive[i]) {
				double Error = degTargetAttitude.data[i] - degCurrentAttitude.data[i];
				double elevonPos = Pitch_ElevonPitch.Step(PitchError, DeltaT);
				ElevonCommand.SetLine(static_cast<float>(elevonPos));
			}
		}*/
		//sprintf_s(oapiDebugString(), 255, "Range: %f Delaz: %f", distToRwy, delaz);
		break;
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
				for(int i=0;i<NZ_VALUE_COUNT-1;i++) NZValues[i] = NZValues[i+1];

				VECTOR3 lift, drag;
				STS()->GetLiftVector(lift);
				STS()->GetDragVector(drag);
				NZValues[NZ_VALUE_COUNT-1] = (lift.y+drag.y)/gravity_force;
				if(curNZValueCount<NZ_VALUE_COUNT) curNZValueCount++;

				double total = 0.0;
				for(int i=0;i<NZ_VALUE_COUNT;i++) total+=NZValues[i];
				averageNZ = total/NZ_VALUE_COUNT;
			}

			switch(TAEMGuidanceMode) {
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

			CSSPitchGuidance(DeltaT);
			CSSRollGuidance(DeltaT);

			// only yaw thrusters should be active at this point
			if(ThrustersActive[YAW])
				ThrusterCommands[YAW].SetLine(static_cast<float>(GetThrusterCommand(YAW, DeltaT)));
			else
				ThrusterCommands[YAW].SetLine(0.0f);
			ThrusterCommands[PITCH].SetLine(0.0f);
			ThrusterCommands[ROLL].SetLine(0.0f);

			SetAerosurfaceCommands(DeltaT);
			if(SpeedbrakeAuto) STS()->SetSpeedbrake(CalculateSpeedbrakeCommand(TotalRange, DeltaT)/100.0);
			else STS()->SetSpeedbrake(1.0f-SpdbkThrotPort.GetVoltage()); // full throttle corresponds to closed speedbrake

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
			// initialize filtered qbar value
			filteredQBar = STS()->GetDynPressure()*PA2PSF;
			Roll_AileronRoll.SetGains(0.25, 0.05, 0.0); // set gains for TAEM phase
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
			return true;
		}
		else if(item == 4) {
			SEC = true;
			return true;
		}
		else if(item == 41) {
			int nNew;
			sscanf_s(Data, "%d", &nNew);
			if(nNew>=0 && nNew<=vLandingSites.size()) SITE_ID = nNew;
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
		dOut = 661.47 * STS()->GetMachNumber() * sqrt(STS()->GetAtmPressure()/101325.0);
		sprintf(cbuf, "KEAS:%.0f", dOut);
		skp->Text(hps->W-100,(hps->H)/2-25,cbuf,strlen(cbuf));
		dOut=(STS()->GetAltitude()*3.280833)-17;
		sprintf(cbuf,"ALT:%.0f",dOut);
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
	else if(!_strnicmp(keyword, "SIDE", 4)) {
		if(!_strnicmp(value, "L", 1)) HACSide = L;
		else HACSide = R;
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
}

void AerojetDAP::SetAerosurfaceCommands(double DeltaT)
{
	/*double elevonPos = 0.0;
	double aileronPos = 0.0;
	double rudderPos = 0.0;*/
	if(AerosurfacesActive[PITCH]) {
		elevonPos = range(elevonPos-1.0*DeltaT, ElevonPitch.Step(degTargetAttitude.data[PITCH]-degCurrentAttitude.data[PITCH], DeltaT), elevonPos+1.0*DeltaT);
		elevonPos = range(-1.0, elevonPos, 1.0);
		//elevonPos = range(-1.0, ElevonPitch.Step(degTargetAttitude.data[PITCH]-degCurrentAttitude.data[PITCH], DeltaT), 1.0);
	}
	if(AerosurfacesActive[ROLL]) {
		aileronPos = range(-1.0, Roll_AileronRoll.Step(degTargetAttitude.data[ROLL]-degCurrentAttitude.data[ROLL], DeltaT), 1.0);
	}
	if(AerosurfacesActive[YAW]) {
		rudderPos = Yaw_RudderYaw.Step(-STS()->GetSlipAngle()*DEG, DeltaT);
	}
	ElevonCommand.SetLine(static_cast<float>(-elevonPos)); // PID controller output has opposite sign of required elevon direction
	AileronCommand.SetLine(static_cast<float>(aileronPos));
	STS()->SetControlSurfaceLevel(AIRCTRL_RUDDER, rudderPos);

	//sprintf_s(oapiDebugString(), 255, "Roll: %f Target Roll: %f Roll Rate: %f Commanded Aileron: %f",
		//degCurrentAttitude.data[ROLL], degTargetAttitude.data[ROLL], degCurrentRates.data[ROLL], aileronPos);
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
	// values in degrees
	const double ATT_DEADBAND = 0.25;
	const double RATE_DEADBAND = max(0.05, RotationRateChange(OrbiterMass, PMI.data[axis], RCSTorque.data[axis], DeltaT));
	const double MIN_ROTATION_RATE = 0.2;
	const double MAX_ROTATION_RATE = 2.0;

	double attError = degTargetAttitude.data[axis]-degCurrentAttitude.data[axis];

	if(RotatingAxis[axis]) {
		if(abs(attError) < 0.10 && abs(degCurrentRates.data[axis]) < RotationRateChange(OrbiterMass, PMI.data[axis], RCSTorque.data[axis], DeltaT)) { // stopped at target attitude
			RotatingAxis[axis] = false;
			return 0.0;
		}
		else if(abs(attError) <= NullStartAngle(abs(degCurrentRates.data[axis]*RAD), OrbiterMass, PMI.data[axis], RCSTorque.data[axis])) { // null rates
			return -sign(degCurrentRates.data[axis]);
		}
		else { // set rotation rate
			double rotationRate = range(MIN_ROTATION_RATE, 4.0*(abs(attError) - 2.0*ATT_DEADBAND), MAX_ROTATION_RATE);
			double rateError = rotationRate*sign(attError)-degCurrentRates.data[axis];
			//sprintf_s(oapiDebugString(), 255, "AErr: %f RErr: %f Rate: %f", attError, rateError, degCurrentRates.data[axis]);
			if(abs(rateError) < 0.025) return 0.0;
			else return sign(rateError);
		}
	}
	else if(abs(attError) > ATT_DEADBAND) {
		RotatingAxis[axis] = true;
	}
	else if(abs(degCurrentRates.data[axis]) > RATE_DEADBAND) {
		//sprintf_s(oapiDebugString(), 255, "Rate Deadband: %f %f", attError, degCurrentRates.data[axis]);
		return -sign(degCurrentRates.data[axis]);
	}

	return 0.0;
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

void AerojetDAP::CSSPitchGuidance(double DeltaT)
{
	if(!CSSInitialized[PITCH]) {
		degTargetAttitude.data[PITCH] = degCurrentAttitude.data[PITCH];
		CSSInitialized[PITCH] = true;
	}
	else {
		//double PitchRateCommand = STS()->GetControlSurfaceLevel(AIRCTRL_ELEVATOR)*2.0 + STS()->GetControlSurfaceLevel(AIRCTRL_ELEVATORTRIM)*5.0;
		double PitchRateCommand = RHCInput[PITCH].GetVoltage()*2.0 + STS()->GetControlSurfaceLevel(AIRCTRL_ELEVATORTRIM)*5.0;
		//if(degTargetRates.data[PITCH] < PitchRateCommand) degTargetRates.data[PITCH] = max(degTargetRates.data[PITCH]+1.0*DeltaT, PitchRateCommand);
		//else if(degTargetRates.data[PITCH] > PitchRateCommand) degTargetRates.data[PITCH] = min(degTargetRates.data[PITCH]-1.0*DeltaT, PitchRateCommand);
		degTargetAttitude.data[PITCH]+=PitchRateCommand*DeltaT;
		//degTargetAttitude.data[PITCH]+=degTargetRates.data[PITCH]*DeltaT;
		/*double PitchError = degTargetAttitude.data[PITCH] - degCurrentAttitude.data[PITCH];

		double elevonPos = Pitch_ElevonPitch.Step(PitchError, DeltaT);
		ElevonCommand.SetLine(static_cast<float>(elevonPos));*/
		//LeftElevonCommand.SetLine(static_cast<float>(elevonPos));
		//RightElevonCommand.SetLine(static_cast<float>(elevonPos));

		//sprintf_s(oapiDebugString(), 255, "RHC Input: %f elevon: %f rates: %f error: %f",
			//RHCInput[PITCH].GetVoltage(), range(-33.0, elevonPos*-33.0, 18.0), PitchRateCommand, PitchError);
	}
}

void AerojetDAP::CSSRollGuidance(double DeltaT)
{
	if(!CSSInitialized[ROLL]) {
		degTargetAttitude.data[ROLL] = degCurrentAttitude.data[ROLL];
		CSSInitialized[ROLL] = true;
	}
	else {
		//double RollRateCommand = -STS()->GetControlSurfaceLevel(AIRCTRL_AILERON)*5.0;
		double RollRateCommand = -RHCInput[ROLL].GetVoltage()*5.0;
		if(GetMajorMode() == 305) RollRateCommand*=2.0;
		degTargetAttitude.data[ROLL]+=RollRateCommand*DeltaT;
		
		double deltaLimit = 1.0;
		if(ThrustersActive[ROLL]) deltaLimit = 2.0;
		degTargetAttitude.data[ROLL] = range(degCurrentAttitude.data[ROLL]-deltaLimit, degTargetAttitude.data[ROLL], degCurrentAttitude.data[ROLL]+deltaLimit);
		/*double RollError = degTargetAttitude.data[ROLL] - degCurrentAttitude.data[ROLL];

		double aileronPos = Roll_AileronRoll.Step(RollError, DeltaT);
		AileronCommand.SetLine(static_cast<float>(aileronPos));*/
	}
}

void AerojetDAP::GetAttitudeData(double DeltaT)
{
	//double lastSideslip = degCurrentAttitude.data[YAW];
	VECTOR3 degLastAttitude = degCurrentAttitude;

	// get AOA, sideslip and bank
	if(STS()->GetGPCMajorMode()==304)
		degCurrentAttitude.data[PITCH] = STS()->GetAOA()*DEG;
	else
		degCurrentAttitude.data[PITCH] = STS()->GetPitch()*DEG;
	degCurrentAttitude.data[YAW] = STS()->GetSlipAngle()*DEG;
	degCurrentAttitude.data[ROLL] = STS()->GetBank()*DEG;

	STS()->GetAngularVel(degCurrentRates);
	degCurrentRates*=DEG;
	// rotating frame, so calculate rates by differentiating attitude changes
	if(!bFirstStep) {
		//degCurrentRates.data[YAW] = (degCurrentAttitude.data[YAW]-lastSideslip)/DeltaT;
		for(unsigned int i=0;i<3;i++) {
			degCurrentRates.data[i] = (degCurrentAttitude.data[i]-degLastAttitude.data[i])/DeltaT;
		}
	}
	else {
		//degCurrentRates.data[YAW] = 0.0;
		degCurrentRates = _V(0.0, 0.0, 0.0);
	}
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

double AerojetDAP::CalculateTargetDrag()
{
	return -10.0; // indicate the target drag is not known
}

void AerojetDAP::CalculateHACGuidance(double DeltaT)
{
	const double YSGN = (HACSide==L) ? -1.0 : 1.0;
	const double DR3 = 8000.0/MPS2FPS;
	const double FINAL_RADIUS = 14000.0/MPS2FPS;
	//const double HAC_RADIUS = 20000/MPS2FPS;
	const double HAC_CENTER_X = OGS_AIMPOINT - 33020.0/MPS2FPS;
	//const double HAC_CENTER_Y = -FINAL_RADIUS;
	const double HAC_CENTER_Y = YSGN * FINAL_RADIUS;
	const double R1 = 0.0;
	const double R2 = 0.093/MPS2FPS;
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
	double radius = length(_V(TgtPos.x-HAC_CENTER_X, TgtPos.y-HAC_CENTER_Y, 0.0));
	double turn_angle = DEG*atan2(HAC_CENTER_Y-TgtPos.y, HAC_CENTER_X-TgtPos.x);
	if(radius > HAC_TurnRadius) {
		double rtan = sqrt(radius*radius - HAC_TurnRadius*HAC_TurnRadius);
		turn_angle -= DEG*YSGN*atan2(HAC_TurnRadius, rtan);
	}
	else {
		turn_angle -= YSGN*90.0;
	}
	if(turn_angle < -180.0) turn_angle+=360.0;
	else if(turn_angle > 180.0) turn_angle-=360.0;
	turn_angle = -turn_angle*YSGN;
	if(turn_angle < 0.0) turn_angle+=360.0;
	HAC_TurnRadius = FINAL_RADIUS + R1*turn_angle + R2*turn_angle*turn_angle;
	double HAC_range = FINAL_RADIUS*turn_angle + 0.5*R1*turn_angle*turn_angle + (1/3)*R2*turn_angle*turn_angle*turn_angle;
	TotalRange = HAC_range*RAD - HAC_CENTER_X;
	//double tgt_turn_rate = airspeed/HAC_RADIUS;
	//double rad_turn_rate = airspeed/radius; // turn rate for circle with current radius

	double rdot = -(velocity.x*(HAC_CENTER_X-TgtPos.x) + velocity.y*(HAC_CENTER_Y-TgtPos.y))/radius;
	double phi_ref = DEG*(velocity.x*velocity.x+velocity.y*velocity.y - rdot*rdot)/(G*HAC_TurnRadius);
	double rdotrf = -DEG*sqrt(velocity.x*velocity.x+velocity.y*velocity.y)*(R1+2.0*R2*turn_angle)/HAC_TurnRadius;
	TargetBank = YSGN*max(0, phi_ref + (radius-HAC_TurnRadius)*GR + (rdot-rdotrf)*GRDOT);
	
	NZCommand = CalculateNZCommand(velocity, TotalRange, -TgtPos.z, DeltaT);

	sprintf_s(oapiDebugString(), 255, "X: %f Y: %f TgtRadius: %f Radius: %f NZ: %f Bank: %f Angle: %f Range: %f", TgtPos.x-HAC_CENTER_X, TgtPos.y-HAC_CENTER_Y,
		HAC_TurnRadius, radius, NZCommand, TargetBank, turn_angle, TotalRange);
	//sprintf_s(oapiDebugString(), 255, "PX: %f PY: %f PZ: %f VX: %f VY: %f VZ: %f", TgtPos.x, TgtPos.y, TgtPos.z,
		//velocity.x, velocity.y, velocity.z);

	double headingToRwy = atan2(TgtPos.y, -TgtPos.x)*DEG;
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
	sprintf_s(oapiDebugString(), 255, "TPos X: %f Y: %f Z: %f", TgtPos.x, TgtPos.y, TgtPos.z);
	//sprintf_s(oapiDebugString(), 255, "X: %f Y: %f Z: %f HeadingError: %f", TgtPos.x, TgtPos.y, TgtPos.z, HeadingError);
	TotalRange = sqrt(TgtPos.x*TgtPos.x + TgtPos.y*TgtPos.y);
	if(TAEMGuidanceMode == PRFNL) {
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
		//TargetHeading = RwyHeading-HeadingError*3.0;
		if(STS()->GetAltitude() < 10000.0/MPS2FPS)
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
		sprintf_s(oapiDebugString(), 255, "Fader bank: %f Actual target: %f", TargetBank+dBank, bank);
		return TargetBank+dBank*oapiGetSimStep();
	}
	return bank;
}

double AerojetDAP::CalculateNZCommand(const VECTOR3& velocity, double predRange, double curAlt, double DeltaT) const
{
	/**
	 * Range above which linear altitude reference is used
	 */
	const double CUBIC_ALT_REF_CUTOFF_RANGE = 256527.82/MPS2FPS;
	/**
	 * Reference height corresponding to range == CUBIC_ALT_REF_CUTOFF_RANGE
	 */
	const double CUBIC_ALT_REF_CUTOFF_HEIGHT = 78161.826/MPS2FPS;
	const double LINEAR_GLIDESLOPE = 0.1112666;
	/**
	 * Constants for cubic altitude reference
	 */
	const double CUBIC_C3 = -4.7714787e-7 * MPS2FPS;
	const double CUBIC_C4 = -2.4291527e-13 * MPS2FPS*MPS2FPS;
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
			sprintf_s(oapiDebugString(), 255, "Cubic alt profile");
			tgtAlt += rangeToALI*rangeToALI*(CUBIC_C3 + rangeToALI*CUBIC_C4);
			tgtGs = AL_GS - rangeToALI*(2*CUBIC_C3 + 3*rangeToALI*CUBIC_C4);
			tgtGs = range(AL_GS, tgtGs, -LINEAR_GLIDESLOPE);
		}
		else {
			sprintf_s(oapiDebugString(), 255, "Final approach alt profile");
			// for the moment, assume we're on final
			tgtGs = AL_GS;
		}
	}

	// convert altitude/glideslope errors to NZ command
	double horzSpeed = sqrt(velocity.x*velocity.x + velocity.y*velocity.y);

	double gain = range(0.3, 2.0 - (7.0e-5)*(curAlt*MPS2FPS), 1.0);
	double refHDot = horzSpeed*tgtGs;
	double HDotErr = refHDot + velocity.z;

	sprintf_s(oapiDebugString(), 255, "%s Gain: %f TgtAlt: %f refHDot: %f HDotErr: %f DNZC: %f", oapiDebugString(), gain, tgtAlt,
		refHDot, HDotErr, gain*0.01*(HDotErr + 0.1*gain*(tgtAlt-curAlt)));

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
	if(STS()->GetMachNumber() > 0.95) return 65.0;

	// NOTE: this an older version of the speedbrake command algorithm
	// newer versions also include energy term
	const double MAX_SPEEDBRAKE = 98.6;
	const double LOWER_LIM = max(0.0, 65.0-650*(0.95-STS()->GetMachNumber()));
	const double UPPER_LIM = min(MAX_SPEEDBRAKE, 65.0+336*(0.95-STS()->GetMachNumber()));
	const double PBRCQ = 89971.082/MPS2FPS;
	const double QBC1 = 3.6086999E-4*MPS2FPS;
	const double QBC2 = -1.1613301E-3*MPS2FPS;

	double rangeToALI = predRange + X_AL_INTERCEPT; // positive in TAEM, negative on final approach
	double refQbar = 0.0;
	if(rangeToALI < PBRCQ) {
		refQbar = range(180.0, 285.0+QBC2*rangeToALI, 285.0);
	}
	else {
		refQbar = range(180.0, 180.0+QBC1*(rangeToALI-PBRCQ), 220.0);
	}
	sprintf_s(oapiDebugString(), 255, "Ref QBar: %f QBar: %f Error: %f", refQbar, filteredQBar, refQbar-filteredQBar);
	double QBarError = refQbar-filteredQBar;

	return range(LOWER_LIM, 65.0 - QBar_Speedbrake.Step(QBarError, DeltaT), UPPER_LIM);
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
	 * Offset between IGS aimpoint and rwy threshold
	 */
	const double IGS_AIMPOINT = -1000.0/MPS2FPS;
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
	VECTOR3 TgtPos = _V(RwyPos.x+IGS_AIMPOINT, RwyPos.y, RwyPos.z);
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
}

VECTOR3 AerojetDAP::GetRunwayRelPos() const
{
	double dLat, dLong, dRad;
	STS()->GetEquPos(dLong, dLat, dRad);
	VECTOR3 v = GetPositionVector(hEarth, dLat, dLong, dRad);
	return mul(RwyRotMatrix, v)-RwyPos;
}

void AerojetDAP::CalculateRangeAndDELAZ(double& range, double& delaz) const
{
	double lng, lat, rad;
	double tgtLong, tgtLat;
	STS()->GetEquPos(lng, lat, rad);
	vLandingSites[SITE_ID].GetRwyPosition(!SEC, tgtLat, tgtLong);

	double avgRadius = oapiGetSize(hEarth) + STS()->GetAltitude()/2.0;
	double angle = oapiOrthodome(lng, lat, tgtLong, tgtLat);
	range = angle*avgRadius;

	VECTOR3 v;
	STS()->GetHorizonAirspeedVector(v);
	double actualHeading = atan2(v.x, v.z);

	double tgtHeading = atan2(sin(tgtLong-lng)*cos(tgtLat), cos(lat)*sin(tgtLat) - sin(lat)*cos(tgtLat)*cos(tgtLong-lng));
	//sprintf_s(oapiDebugString(), 255, "Lng: %f Lat: %f Range: %f Heading: %f Tgt: %f", lng, lat, range, DEG*actualHeading, DEG*tgtHeading);
	delaz = DEG*(actualHeading-tgtHeading);
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
	vLandingSites.push_back(LandingSiteData(28.632944*RAD, -80.706035*RAD, 28.5970420*RAD, -80.6826540*RAD, 150.2505, "KSC15", "KSC33"));
}

void AerojetDAP::CalculateTargetRoll(double dT)
{
	double delAZ, range;
	CalculateRangeAndDELAZ(range,delAZ);
	double speed = STS()->GetAirspeed()*3.28/1000;
	double alt = STS()->GetAltitude()/1000;
	VECTOR3 vec;
	VECTOR3 force, f;
	STS()->GetForceVector(f);
	STS()->HorizonRot(f,force);
	//double v_acc = force.y/STS()->GetMass();
	double v_acc = (last_vel - vec.y)*dT;
	STS()->GetHorizonAirspeedVector(vec);
	double target_height = dTable->Interpolate(speed)/3.28;
	

	

	double delAZlimit = 10.5;

	//EXECUTE FIRST ROLL
	if(!first_roll && !roll_command && vec.y >= -73)
	{
		if(delAZ > 0)
			target_bank = 79; //to the left
		else
			target_bank = -79;//to the right

		roll_command = true;
		first_roll = true;
	}

	//CHECK IF WE HAVE TO DO ROLL REVERSAL
	if(first_roll && fabs(delAZ) >= delAZlimit)
	{
		target_bank = -target_bank;
		roll_command = true;
	}

	if(first_roll && !roll_command)
	{
		target_vspeed = CalculateTargetVSpeed(STS()->GetAltitude(),target_height,vec.y,dT);
		target_vacc = CalculateTargetVAcc(vec.y,target_vspeed,v_acc,dT);

		//BANK PID
		double vacc_err = target_vacc - v_acc;
		if(vacc_err > 0) 
		{
			if(STS()->GetBank() > 0)
				target_bank = max(target_bank - 1, 0);
			else
				target_bank = min(target_bank + 1, 0);

			roll_command = true;
		}
		else
		{
			if(STS()->GetBank() > 0)
				target_bank = min(target_bank + 1, 79);
			else
				target_bank = max(target_bank - 1, -79);

			roll_command = true;
		}

			
	}

	//AT THE END CHECK IF WE ARE STILL MANEUVRING
	if(fabs(target_bank - STS()->GetBank()*DEG) <= 0.2 && roll_command)
		roll_command = false;

	sprintf(oapiDebugString(),"Targets: Bank: %lf, VAcc: %lf,\n VSpeed: %lf, Altitude: %lf; A Vacc: %lf",target_bank,target_vacc,target_vspeed,target_height,v_acc);
	//sprintf(oapiDebugString(),"Altitude error: %lf",target_height-alt);
	last_vel = vec.y;

}

double AerojetDAP::CalculateTargetVAcc(double actual_vspeed, double target_vspeed, double actual_vacc, double dT)
{
	double ref_vacc = 40; //when vertical speed difference equals this value, error will be corrected at vertical acceleration of 1m/s*s
	//if our vertical speed is more negative than target one, we need to gain vertical acceleration to drop the error
	//and vice versa
	//the more error in vertical speed we have, the faster change in vertical speed will occur
	double error = fabs(target_vspeed - actual_vspeed);
	if(target_vspeed > actual_vspeed)
		return min(actual_vacc + error/ref_vacc,1);
	else return max(actual_vacc - error/ref_vacc, -1);
}

double AerojetDAP::CalculateTargetVSpeed(double actual_altitude, double target_altitude, double current_vspeed, double dT)
{
	double error = fabs(actual_altitude - target_altitude);
	double correction_time = 360; //this value shows in how much time we will try to correct altitude error, the higher value, the longer correction time

	if(target_altitude > actual_altitude)
		return min(current_vspeed + error/correction_time,100);
	
	else if(target_altitude < actual_altitude)
		return max(current_vspeed - error/correction_time,-100);
}

};