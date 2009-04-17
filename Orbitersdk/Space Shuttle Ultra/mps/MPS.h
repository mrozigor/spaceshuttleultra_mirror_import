/****************************************************************************
  This file is part of Space Shuttle Ultra

  Main propulsion System definition



  Space Shuttle Ultra is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Ultra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Ultra; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://spaceshuttleultra.sourceforge.net/license/ for more details.

  **************************************************************************/
#ifndef _mps_MPS_H_
#define _mps_MPS_H_


#include <orbitersdk.h>
#include "..\AtlantisSubsystem.h"
#include "..\ValveTypeBool.h"


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
		ValveTypeBool* ptrPV1;// ME-1 LOX prevalve
		ValveTypeBool* ptrPV2;// ME-2 LOX prevalve
		ValveTypeBool* ptrPV3;// ME-3 LOX prevalve
		ValveTypeBool* ptrPV4;// ME-1 LH2 prevalve
		ValveTypeBool* ptrPV5;// ME-2 LH2 prevalve
		ValveTypeBool* ptrPV6;// ME-3 LH2 prevalve
		ValveTypeBool* ptrPV7;// LOX Fdln Rlf
		ValveTypeBool* ptrPV8;// LH2 Fdln Rlf
		ValveTypeBool* ptrPV9;// LOX O/B F/D
		ValveTypeBool* ptrPV10;// LOX I/B F/D
		ValveTypeBool* ptrPV11;// LH2 O/B F/D
		ValveTypeBool* ptrPV12;// LH2 I/B F/D
		ValveTypeBool* ptrPV13;// LH2 topping
		ValveTypeBool* ptrPV17;// LH2 B/U Inbd DV
		ValveTypeBool* ptrPV18;// LH2 B/U Otbd DV
		ValveTypeBool* ptrPV19;// LOX Ovbd B/V
		ValveTypeBool* ptrPV20;// LOX POGO Recrc 1
		ValveTypeBool* ptrPV21;// LOX POGO Recrc 2
		ValveTypeBool* ptrPV22;// LH2 Hi Pt Bl

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
		double MPSValveStatus( MPS_VALVE );

		// heart beat
		virtual void OnPostStep( double, double, double );

		void OnSaveState( FILEHANDLE ) const;
		bool OnParseLine( const char* );
	};
}


#endif// _mps_MPS_H_