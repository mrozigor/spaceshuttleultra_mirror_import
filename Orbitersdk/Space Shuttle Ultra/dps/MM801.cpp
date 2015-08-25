#include "MM801.h"
#include "IDP.h"
#include "../Atlantis.h"
#include <UltraMath.h>

const double ELEVON_RATE = 5.8; // rate in deg/sec
const double RUDDER_RATE = 5.0; // rate in deg/sec
const double SPEEDBRAKE_RATE = 2.7/.986; // rate in %/sec
const double ELEVON_POSITIONS[3] = {18.0, -33.0, -7.5}; // angles in degrees
const double RUDDER_POSITIONS[3] = {5.0, -5.0, 0.0}; // angles in degrees
const double SPEEDBRAKE_POSITIONS[3] = {10.0/.986, 2.0/.986, 10.0/.986}; // deflection in percentage

namespace dps
{
	MM801::MM801(SimpleGPCSystem* _gpc)
	: SimpleGPCSoftware(_gpc,"MM801"),
	  ElevonTarget(0.0), RudderTarget(0.0), SpeedbrakeTarget(0.0)
	{
		bFCSTestActive = false;
	}

	MM801::~MM801()
	{

	}

	void MM801::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle("AEROSURFACE_CMD",16);
		ElevonCommand.Connect(pBundle,0);
		AileronCommand.Connect(pBundle,1);
		RudderCommand.Connect(pBundle,2);
		ElevonCommandRead.Connect(pBundle,0);
		//AileronCommandRead.Connect(pBundle,1);
	}

	bool MM801::OnMajorModeChange(unsigned int newMajorMode)
	{
		if(newMajorMode == 801)
			return true;
		else return false;
	}

	bool MM801::OnPaint(int spec, vc::MDU* pMDU) const
	{
		if (spec != dps::MODE_UNDEFINED) return false;

		PrintCommonHeader(" FCS/DED DIS C/O",pMDU);

		//DED DIS SECTION
		pMDU->mvprint(2,4,"DED DIS");
		pMDU->mvprint(1,5,"FWD 1");
		pMDU->mvprint(1,6,"AFT 2");
		pMDU->mvprint(9,5,"HIGH 3");
		pMDU->mvprint(9,6,"LOW  4");
		pMDU->mvprint(9,7,"FLAG 5");
		pMDU->mvprint(9,8,"OFF  6");

		//BODY FLP DES SECTION
		pMDU->mvprint(2,10,"BODY FLP DES");
		pMDU->mvprint(1,11,"VLV");
		pMDU->mvprint(5,11,"1");
		pMDU->mvprint(5,12,"2");
		pMDU->mvprint(5,13,"3");
		pMDU->mvprint(11,11,"12");
		pMDU->mvprint(11,12,"13");
		pMDU->mvprint(11,13,"14");

		//SEC ACT CK SECTION
		pMDU->mvprint(2,15,"SEC ACT CK");
		pMDU->mvprint(1,16,"CH 1  15");
		pMDU->mvprint(1,17,"CH 2  16");
		pMDU->mvprint(1,18,"CH 3  17");
		pMDU->mvprint(1,19,"CH 4  18");
		pMDU->mvprint(1,20,"STOP  19");
		pMDU->mvprint(1,21,"POS STIM");
		pMDU->mvprint(3,22,"ENA 20");

		//MODE LT SECTION
		pMDU->mvprint(21,4,"MODE LT");
		pMDU->mvprint(20,5,"ON   7");
		pMDU->mvprint(20,6,"OFF  8");

		//AVG G ON SECTION
		pMDU->mvprint(36,4,"AVG G  ON  9");

		//SURF DR SECTION
		pMDU->mvprint(21,9,"SURF DR");
		pMDU->mvprint(30,9,"START 10");
		pMDU->mvprint(41,9,"STOP 11");

		pMDU->mvprint(18,12,"SURF");
		pMDU->mvprint(29,12,"CMD");
		pMDU->mvprint(36,12,"POS");
		pMDU->mvprint(42,12,"1 2 3 4");

		pMDU->mvprint(17,13,"ELEV L OB");
		pMDU->mvprint(28,13,"000.0");		
		pMDU->mvprint(35,13,"     ");

		pMDU->mvprint(24,14,"IB");
		pMDU->mvprint(28,14,"000.0");
		pMDU->mvprint(35,14,"     ");

		pMDU->mvprint(22,15,"R IB");
		pMDU->mvprint(28,15,"000.0");
		pMDU->mvprint(35,15,"     ");

		pMDU->mvprint(24,16,"OB");
		pMDU->mvprint(28,16,"000.0");
		pMDU->mvprint(35,16,"     ");

		pMDU->mvprint(17,17,"RUD");
		pMDU->mvprint(28,17,"000.0");
		pMDU->mvprint(35,17,"000.0");

		pMDU->mvprint(17,18,"SPD BRK");
		pMDU->mvprint(28,18,"000.0");
		pMDU->mvprint(35,18,"000.0");

		pMDU->mvprint(17,19,"BDY FLP");
		pMDU->mvprint(31,19,"  ");//two sings here
		pMDU->mvprint(35,19,"000.0");

		//SEC ACT SECTION
		pMDU->mvprint(37,21,"SEC ACT");
		pMDU->mvprint(36,22,"21 BYPASS");
		pMDU->mvprint(36,23,"22 RESET");



		//ACTIVE STRING HERE
		if(bFCSTestActive) {
			if(ElevonTargetIdx != FV3) pMDU->mvprint(38,9,"*");
			else pMDU->mvprint(48,9,"*");
		}

		
		
		
		//FCS COMMAND
		char buff[50];
		PrintElevonPos(ElevonTarget, buff);
		pMDU->mvprint(28, 13, buff);
		pMDU->mvprint(28, 14, buff);
		pMDU->mvprint(28, 15, buff);
		pMDU->mvprint(28, 16, buff);
		PrintRudderPos(RudderTarget, buff);
		pMDU->mvprint(28, 17, buff);
		PrintSpeedbrakePos(SpeedbrakeTarget, buff);
		pMDU->mvprint(28, 18, buff);
		//FCS ACTUAL POS		
		PrintElevonPos(STS()->aerosurfaces.leftElevon, buff);
		pMDU->mvprint(35,13,buff);
		pMDU->mvprint(35,14,buff);
		PrintElevonPos(STS()->aerosurfaces.rightElevon, buff);
		pMDU->mvprint(35,15,buff);
		pMDU->mvprint(35,16,buff);
		PrintRudderPos(STS()->aerosurfaces.rudder, buff);
		pMDU->mvprint(35,17,buff);
		PrintSpeedbrakePos(STS()->GetActSpeedbrakePosition()*100.0, buff);
		pMDU->mvprint(35,18,buff);

		return true;
	}

	bool MM801::ItemInput(int spec, int item, const char* Data)
	{
		if(item == 10 && STS()->HydraulicsOK())
		{
			bFCSTestActive = true;
			bFCSTestEnding = false;
			ElevonTargetIdx = FV1;
			RudderTargetIdx = FV1;
			SpeedbrakeTargetIdx = FV1;

			ElevonTarget = ElevonCommandRead.GetVoltage()*33.0;
			RudderTarget = STS()->aerosurfaces.rudder;
			SpeedbrakeTarget = STS()->GetActSpeedbrakePosition()*100.0;
			return true;
		}

		if(item == 11)
		{
			bFCSTestEnding = true;
			ElevonTargetIdx = FV3;
			RudderTargetIdx = FV3;
			SpeedbrakeTargetIdx = FV3;
			return true;
		}
		return false;
	}

	void MM801::OnPreStep(double SimT, double DeltaT, double MJD)
	{
		//double ElevonPos = ElevonCommandRead.GetVoltage()*33.0;
		//double RudderPos = RudderCommandRead.GetVoltage()*27.1;
		//double SpeedbrakePos = SpeedbrakeCommandRead.GetVoltage()*100.0;
		
		if(bFCSTestActive && STS()->HydraulicsOK())
		{
			ElevonTarget = GetAerosurfaceCommand(ElevonTarget, DeltaT, ElevonTargetIdx, ELEVON_RATE, ELEVON_POSITIONS);
			ElevonCommand.SetLine(static_cast<float>(ElevonTarget/33.0));

			RudderTarget = GetAerosurfaceCommand(RudderTarget, DeltaT, RudderTargetIdx, RUDDER_RATE, RUDDER_POSITIONS);
			RudderCommand.SetLine(static_cast<float>(RudderTarget/27.1));
			
			SpeedbrakeTarget = GetAerosurfaceCommand(SpeedbrakeTarget, DeltaT, SpeedbrakeTargetIdx, SPEEDBRAKE_RATE, SPEEDBRAKE_POSITIONS);
			STS()->SetSpeedbrake(SpeedbrakeTarget/100.0);
			
			AileronCommand.SetLine(0.0f);

			// if all aerosurfaces have reached their final position, set test state to inactive
			if(bFCSTestEnding) {
				double elevonPos = STS()->aerosurfaces.leftElevon;
				double rudderPos = STS()->aerosurfaces.rudder;
				double speedbrakePos = STS()->GetActSpeedbrakePosition()*100.0;
				if(Eq(elevonPos, ELEVON_POSITIONS[FV3], 0.01) && Eq(rudderPos, RUDDER_POSITIONS[FV3], 0.01) && Eq(speedbrakePos, SPEEDBRAKE_POSITIONS[FV3], 0.001)) {
					bFCSTestActive = false;
					bFCSTestEnding = false;
				}
			}
		}
	}

	double MM801::GetAerosurfaceCommand(double curTarget, double DeltaT, AEROSURFACE_DRIVE_TARGET& targetIdx, const double RATE, const double* POSITIONS)
	{
		double dir = sign(POSITIONS[targetIdx] - curTarget);
		double newTarget = curTarget + dir*RATE*DeltaT;
		if(!Eq(dir, sign(POSITIONS[targetIdx] - newTarget))) {
			newTarget = POSITIONS[targetIdx];
			if(targetIdx == FV1) targetIdx = FV2;
			else if(targetIdx == FV2) targetIdx = FV1;
		}
		return newTarget;
	}

	void MM801::PrintElevonPos(double pos, char* buff) const
	{
		if(pos >= 0) sprintf_s(buff, 6, "D%04.1f",pos);
		else sprintf_s(buff, 6, "U%04.1f",-pos);
	}

	void MM801::PrintRudderPos(double pos, char* buff) const
	{
		if(pos >= 0) sprintf_s(buff, 6, "R%04.1f",pos);
		else sprintf_s(buff, 6, "L%04.1f",-pos);
	}

	void MM801::PrintSpeedbrakePos(double pos, char* buff) const
	{
		sprintf_s(buff, 6, "%05.1f", pos);
	}

	bool MM801::OnParseLine( const char* keyword, const char* value )
	{
		if(!_strnicmp( keyword, "SURF_DR", 7 ))
		{
			if (!_strnicmp( value, "ACTIVE", 6 ))
			{
				bFCSTestActive = true;
				ElevonTargetIdx = FV1;
				RudderTargetIdx = FV1;
				SpeedbrakeTargetIdx = FV1;
				return true;
			}
		}
		return false;
	}

	void MM801::OnSaveState( FILEHANDLE scn ) const
	{
		if (bFCSTestActive == true) oapiWriteScenario_string( scn, "SURF_DR", "ACTIVE" );
		else  oapiWriteScenario_string( scn, "SURF_DR", "INACTIVE" );
	}
};
