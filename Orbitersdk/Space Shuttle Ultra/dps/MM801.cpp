#include "MM801.h"
#include "../Atlantis.h"

#define ELEVON_MOVE_SPEED 0.01

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
		PrintCommonHeader("MM801 TEST",pMDU);
		return true;
	}

	bool MM801::ItemInput(int spec, int item, const char* Data)
	{
		if(item == 10)
		{
			bFCSTestActive = true;
			bElevonMoveUpwards = true;
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
		
		if(bFCSTestActive)
		{
			if(bElevonMoveUpwards)
				ElevonCommand.SetLine(actual_voltage+ELEVON_MOVE_SPEED*DeltaT);

			if(!bElevonMoveUpwards)
				ElevonCommand.SetLine(actual_voltage-ELEVON_MOVE_SPEED*DeltaT);

			if(bParkElevons)
			{
				if(actual_voltage > 0)
					ElevonCommand.SetLine(max(0,actual_voltage-ELEVON_MOVE_SPEED*DeltaT));

				if(actual_voltage < 0)
					ElevonCommand.SetLine(min(0,actual_voltage+ELEVON_MOVE_SPEED*DeltaT));
			}

			//WHY 0.1f? Because that way elevons imitating "stop motion" when fully up/down.
			if(actual_voltage >= 0.1f)
				bElevonMoveUpwards = false;

			if(actual_voltage <= -0.1f)
				bElevonMoveUpwards = true;

			if(actual_voltage == 0.0f && bParkElevons)
			{
				bParkElevons=false;
				bFCSTestActive=false;
			}
		}
		
	}
};