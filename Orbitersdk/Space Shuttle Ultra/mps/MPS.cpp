#include "MPS.h"

namespace mps
{
	MPS::MPS( SubsystemDirector* _director ):AtlantisSubsystem( _director, "mpsMPS" )
	{
		// mostly speculation...
		posPV1 = true;
		posPV2 = true;
		posPV3 = true;
		posPV4 = true;
		posPV5 = true;
		posPV6 = true;
		posPV7 = false;
		posPV8 = false;
		posPV9 = true;
		posPV10 = true;
		posPV11 = true;
		posPV12 = true;
		posPV13 = true;
		posPV17 = false;
		posPV18 = false;
		posPV19 = false;
		posPV20 = true;
		posPV21 = true;
		posPV22 = false;
		posPD1 = true;
		posPD2 = true;
		posPD3 = true;
		return;
	}

	MPS::~MPS( void )
	{
		// end
	}

	bool MPS::MPSValveOpen( MPS_VALVE vlv )
	{
		switch (vlv)
		{
			case PV1:
				posPV1 = true;
				return true;
			case PV2:
				posPV2 = true;
				return true;
			case PV3:
				posPV3 = true;
				return true;
			case PV4:
				posPV4 = true;
				return true;
			case PV5:
				posPV5 = true;
				return true;
			case PV6:
				posPV6 = true;
				return true;
			case PV7:
				posPV7 = true;
				return true;
			case PV8:
				posPV8 = true;
				return true;
			case PV9:
				posPV9 = true;
				return true;
			case PV10:
				posPV10 = true;
				return true;
			case PV11:
				posPV11 = true;
				return true;
			case PV12:
				posPV12 = true;
				return true;
			case PV13:
				posPV13 = true;
				return true;
			case PV17:
				posPV17 = true;
				return true;
			case PV18:
				posPV18 = true;
				return true;
			case PV19:
				posPV19 = true;
				return true;
			case PV20:
				posPV20 = true;
				return true;
			case PV21:
				posPV21 = true;
				return true;
			case PV22:
				posPV22 = true;
				return true;
			case PD1:
				posPD1 = true;
				return true;
			case PD2:
				posPD2 = true;
				return true;
			case PD3:
				posPD3 = true;
				return true;
		}
		return false;
	}

	bool MPS::MPSValveClose( MPS_VALVE vlv )
	{
		switch (vlv)
		{
			case PV1:
				posPV1 = false;
				return true;
			case PV2:
				posPV2 = false;
				return true;
			case PV3:
				posPV3 = false;
				return true;
			case PV4:
				posPV4 = false;
				return true;
			case PV5:
				posPV5 = false;
				return true;
			case PV6:
				posPV6 = false;
				return true;
			case PV7:
				posPV7 = false;
				return true;
			case PV8:
				posPV8 = false;
				return true;
			case PV9:
				posPV9 = false;
				return true;
			case PV10:
				posPV10 = false;
				return true;
			case PV11:
				posPV11 = false;
				return true;
			case PV12:
				posPV12 = false;
				return true;
			case PV13:
				posPV13 = false;
				return true;
			case PV17:
				posPV17 = false;
				return true;
			case PV18:
				posPV18 = false;
				return true;
			case PV19:
				posPV19 = false;
				return true;
			case PV20:
				posPV20 = false;
				return true;
			case PV21:
				posPV21 = false;
				return true;
			case PV22:
				posPV22 = false;
				return true;
			case PD1:
				posPD1 = false;
				return true;
			case PD2:
				posPD2 = false;
				return true;
			case PD3:
				posPD3 = false;
				return true;
		}
		return false;
	}

	bool MPS::MPSValveStatus( MPS_VALVE vlv )
	{
		switch (vlv)
		{
			case PV1:
				return posPV1;
			case PV2:
				return posPV2;
			case PV3:
				return posPV3;
			case PV4:
				return posPV4;
			case PV5:
				return posPV5;
			case PV6:
				return posPV6;
			case PV7:
				return posPV7;
			case PV8:
				return posPV8;
			case PV9:
				return posPV9;
			case PV10:
				return posPV10;
			case PV11:
				return posPV11;
			case PV12:
				return posPV12;
			case PV13:
				return posPV13;
			case PV17:
				return posPV17;
			case PV18:
				return posPV18;
			case PV19:
				return posPV19;
			case PV20:
				return posPV20;
			case PV21:
				return posPV21;
			case PV22:
				return posPV22;
			case PD1:
				return posPD1;
			case PD2:
				return posPD2;
			case PD3:
				return posPD3;
		}
		return false;// ?????
	}

	void MPS::OnPostStep( double fSimT, double fDeltaT, double fMJD )
	{
		return;
	}
}