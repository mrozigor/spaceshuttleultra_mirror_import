#include "MM801.h"
#include "../Atlantis.h"

#define ELEVON_MOVE_SPEED 0.275

namespace dps
{
	MM801::MM801(SimpleGPCSystem* _gpc)
		: SimpleGPCSoftware(_gpc,"MM801")
	{
		bFCSTestActive = false;
		bElevonMoveUpwards = false;
		bParkElevons = false;

	}

	MM801::~MM801()
	{

	}

	void MM801::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle("AEROSURFACE_CMD",16);
		ElevonCommand.Connect(pBundle,0);
		AileronCommand.Connect(pBundle,1);
		ElevonCommandRead.Connect(pBundle,0);
		AileronCommandRead.Connect(pBundle,1);
	}

	bool MM801::OnMajorModeChange(unsigned int newMajorMode)
	{
		if(newMajorMode == 801)
			return true;
		else return false;
	}

	bool MM801::OnPaint(int spec, vc::MDU* pMDU) const
	{
		PrintCommonHeader("FCS/DED DIS C/O",pMDU);

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
		if(bFCSTestActive && !bParkElevons)
			pMDU->mvprint(38,9,"*");

		if(bParkElevons)
			pMDU->mvprint(48,9,"*");

		
		
		
		//FCS COMMAND
		if(bElevonMoveUpwards && !bParkElevons && bFCSTestActive)
		{
			pMDU->mvprint(28,13,"U18.0");		
			pMDU->mvprint(28,14,"U18.0");
			pMDU->mvprint(28,15,"U18.0");
			pMDU->mvprint(28,16,"U18.0");
			pMDU->mvprint(28,17,"R45.0");
			pMDU->mvprint(28,18,"100.0");
			pMDU->mvprint(31,19,"UP");
		}

		if(!bElevonMoveUpwards && !bParkElevons && bFCSTestActive)
		{
			pMDU->mvprint(28,13,"D33.0");		
			pMDU->mvprint(28,14,"D33.0");
			pMDU->mvprint(28,15,"D33.0");
			pMDU->mvprint(28,16,"D33.0");
			pMDU->mvprint(28,17,"L45.0");
			pMDU->mvprint(28,18,"000.0");
			pMDU->mvprint(31,19,"DN");
		}

		//FCS ACTUAL POS		
		pMDU->mvprint(35,13,ElevonVoltage2DegreesString(ElevonCommandRead.GetVoltage()).c_str());
		pMDU->mvprint(35,14,ElevonVoltage2DegreesString(ElevonCommandRead.GetVoltage()).c_str());
		pMDU->mvprint(35,15,ElevonVoltage2DegreesString(ElevonCommandRead.GetVoltage()).c_str());
		pMDU->mvprint(35,16,ElevonVoltage2DegreesString(ElevonCommandRead.GetVoltage()).c_str());
		pMDU->mvprint(35,17,RudderVoltage2DegreesString(ElevonCommandRead.GetVoltage()).c_str());





		return true;
	}

	bool MM801::ItemInput(int spec, int item, const char* Data)
	{
		if(item == 10 && STS()->HydraulicsOK())
		{
			bFCSTestActive = true;
			bElevonMoveUpwards = false;
			return true;
		}

		if(item == 11)
		{
			bParkElevons = true;
			return true;
		}
		return false;
	}

	void MM801::OnPreStep(double SimT, double DeltaT, double MJD)
	{
		double actual_voltage = ElevonCommandRead.GetVoltage();
		
		if(bFCSTestActive && STS()->HydraulicsOK())
		{
			if(bElevonMoveUpwards)
				ElevonCommand.SetLine(static_cast<float>(actual_voltage+ELEVON_MOVE_SPEED*DeltaT));

			if(!bElevonMoveUpwards)
				ElevonCommand.SetLine(static_cast<float>(actual_voltage-ELEVON_MOVE_SPEED*DeltaT));

			if(bParkElevons)
			{
				if(actual_voltage > 0)
					ElevonCommand.SetLine(static_cast<float>(max(0,actual_voltage-ELEVON_MOVE_SPEED*DeltaT)));

				if(actual_voltage < 0)
					ElevonCommand.SetLine(static_cast<float>(min(0,actual_voltage+ELEVON_MOVE_SPEED*DeltaT)));
			}

			//WHY 0.1f? Because that way elevons imitating "stop motion" when fully up/down.
			if(actual_voltage >= 1.0f)
				bElevonMoveUpwards = false;

			if(actual_voltage <= -1.0f)
				bElevonMoveUpwards = true;

			if(actual_voltage == 0.0f && bParkElevons)
			{
				bParkElevons=false;
				bFCSTestActive=false;
			}
		}
		
	}

	std::string MM801::ElevonVoltage2DegreesString(float voltage) const
	{
		if(voltage > 0)
		{
			float x = abs(voltage)*18.0f;
			char buff[255];
			std::string x_str;
			sprintf(buff,"%.1lf",x);
			x_str = buff;


			if(x >= 10)
				return "U" + x_str;

			else
				return "U0" + x_str;
		}

		else if(voltage < 0)
		{
			float x = abs(voltage)*33.0f;
			char buff[255];
			std::string x_str;
			sprintf(buff,"%.1lf",x);
			x_str = buff;


			if(x >= 10)
				return "D" + x_str;

			else
				return "D0" + x_str;
		}

		else
			return "000.0";
	}

	std::string MM801::RudderVoltage2DegreesString(float voltage) const
	{
		if(voltage > 0)
		{
			float x = abs(voltage)*45.0f;
			char buff[255];
			std::string x_str;
			sprintf(buff,"%.1lf",x);
			x_str = buff;


			if(x >= 10)
				return "R" + x_str;

			else
				return "R0" + x_str;
		}

		else if(voltage < 0)
		{
			float x = abs(voltage)*45.0f;
			char buff[255];
			std::string x_str;
			sprintf(buff,"%.1lf",x);
			x_str = buff;


			if(x >= 10)
				return "L" + x_str;

			else
				return "L0" + x_str;
		}

		else
			return "000.0";
	}

};