#ifndef _mps_MPS_H_
#define _mps_MPS_H_


#include <orbitersdk.h>
#include "..\AtlantisSubsystem.h"
//#include "..\BasicValve.h"


namespace mps
{
	typedef enum _MPS_VALVE
	{
		PV1,
		PV2,
		PV3,
		PV4,
		PV5,
		PV6,
		PV7,
		PV8,
		PV9,
		PV10,
		PV11,
		PV12,
		PV13,
		PV17,
		PV18,
		PV19,
		PV20,
		PV21,
		PV22,
		PD1,
		PD2,
		PD3
	} MPS_VALVE;

	class MPS:public AtlantisSubsystem
	{
	private:
		// PV
		//BasicValve* ptrPV1;// ME-1 LOX prevalve
		//BasicValve* ptrPV2;// ME-2 LOX prevalve
		//BasicValve* ptrPV3;// ME-3 LOX prevalve

		bool posPV1;// ME-1 LOX prevalve
		bool posPV2;// ME-2 LOX prevalve
		bool posPV3;// ME-3 LOX prevalve
		bool posPV4;// ME-1 LH2 prevalve
		bool posPV5;// ME-2 LH2 prevalve
		bool posPV6;// ME-3 LH2 prevalve
		bool posPV7;// LOX Fdln Rlf
		bool posPV8;// LH2 Fdln Rlf
		bool posPV9;// LOX O/B F/D
		bool posPV10;// LOX I/B F/D
		bool posPV11;// LH2 O/B F/D
		bool posPV12;// LH2 I/B F/D
		bool posPV13;// LH2 topping
		bool posPV17;// LH2 B/U Inbd DV
		bool posPV18;// LH2 B/U Otbd DV
		bool posPV19;// LOX Ovbd B/V
		bool posPV20;// LOX POGO Recrc 1
		bool posPV21;// LOX POGO Recrc 2
		bool posPV22;// LH2 Hi Pt Bl

		// PD
		bool posPD1;// LOX 17in Disc
		bool posPD2;// LH2 17in Disc
		bool posPD3;// LH2 4in Disc

		// RV

		// RP

	public:
		MPS( SubsystemDirector* );
		~MPS( void );

		// vlv ops
		bool MPSValveOpen( MPS_VALVE );
		bool MPSValveClose( MPS_VALVE );
		bool MPSValveStatus( MPS_VALVE );

		// heart beat
		virtual void OnPostStep( double, double, double );
	};
}


#endif// _mps_MPS_H_