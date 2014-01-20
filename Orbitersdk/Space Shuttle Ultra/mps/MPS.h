/****************************************************************************
  This file is part of Space Shuttle Ultra

  Main Propulsion System definition



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
#include "..\Atlantis.h"
#include "SolenoidValve.h"
#include "PressureActuatedValve.h"


namespace mps
{
	class HeSysPneu;

	class MPS:public AtlantisSubsystem
	{
		private:
			HeSysPneu* HeSys;

			double LOXManifPress;
			double LH2ManifPress;

			double LOXinitpress;
			double LH2initpress;

			// PV
			PressureActuatedValve* ptrPV1;// ME-1 LOX prevalve
			PressureActuatedValve* ptrPV2;// ME-2 LOX prevalve
			PressureActuatedValve* ptrPV3;// ME-3 LOX prevalve
			PressureActuatedValve* ptrPV4;// ME-1 LH2 prevalve
			PressureActuatedValve* ptrPV5;// ME-2 LH2 prevalve
			PressureActuatedValve* ptrPV6;// ME-3 LH2 prevalve
			/*ValveTypeBool* ptrPV7;// LOX Fdln Rlf SOV (NO)
			ValveTypeBool* ptrPV8;// LH2 Fdln Rlf SOV (NO)*/
			PressureActuatedValve* ptrPV9;// LOX O/B F/D
			PressureActuatedValve* ptrPV10;// LOX I/B F/D
			PressureActuatedValve* ptrPV11;// LH2 O/B F/D
			PressureActuatedValve* ptrPV12;// LH2 I/B F/D
			PressureActuatedValve* ptrPV13;// LH2 topping
			PressureActuatedValve* ptrPV17;// LH2 B/U Inbd DV
			PressureActuatedValve* ptrPV18;// LH2 B/U Otbd DV
			PressureActuatedValve* ptrPV19;// LOX Ovbd B/V (NO)
			PressureActuatedValve* ptrPV20;// LOX POGO Recrc 1 (NO)
			PressureActuatedValve* ptrPV21;// LOX POGO Recrc 2 (NO)
			//ValveTypeBool* ptrPV22;// LH2 Hi Pt Bl (NC)
			
			
			// LV
			/*LV1;// E1 He Iso Vlv A OP Pwr
			LV2;// E1 He Iso Vlv B OP Pwr
			LV3;// E2 He Iso Vlv A OP Pwr
			LV4;// E2 He Iso Vlv B OP Pwr
			LV5;// E3 He Iso Vlv A OP Pwr
			LV6;// E3 He Iso Vlv B OP Pwr
			LV7;// Pneu He Iso Vlv 1 OP Pwr
			LV8;// Pneu He Iso Vlv 2 OP Pwr
			LV10;// Reg He Xover Vlv OP Pw*/
			SolenoidValve* ptrLV12;// E1 LO2 Prevlv OP Pwr 1
			SolenoidValve* ptrLV13;// E1 LO2 Prevlv CL Pwr 1
			SolenoidValve* ptrLV14;// E2 LO2 Prevlv OP Pwr 1
			SolenoidValve* ptrLV15;// E2 LO2 Prevlv CL Pwr 1
			SolenoidValve* ptrLV16;// E3 LO2 Prevlv OP Pwr 1
			SolenoidValve* ptrLV17;// E3 LO2 Prevlv CL Pwr 1
			SolenoidValve* ptrLV18;// E1 LH2 Prevlv OP Pwr
			SolenoidValve* ptrLV19;// E1 LH2 Prevlv CL Pwr
			SolenoidValve* ptrLV20;// E2 LH2 Prevlv OP Pwr
			SolenoidValve* ptrLV21;// E2 LH2 Prevlv CL Pwr
			SolenoidValve* ptrLV22;// E3 LH2 Prevlv OP Pwr
			SolenoidValve* ptrLV23;// E3 LH2 Prevlv CL Pwr
			/*LV26;// He Sply Blwdwn 1 OP Pwr
			LV27;// He Sply Blwdwn 2 OP Pwr*/
			SolenoidValve* ptrLV28;
			SolenoidValve* ptrLV29;
			SolenoidValve* ptrLV30;// LO2 Inbd F/D Vlv OP Pwr
			SolenoidValve* ptrLV31;// LO2 Inbd F/D Vlv CL Pwr
			SolenoidValve* ptrLV32;
			SolenoidValve* ptrLV33;
			SolenoidValve* ptrLV34;// LH2 Inbd F/D Vlv OP Pwr
			SolenoidValve* ptrLV35;// LH2 Inbd F/D Vlv CL Pwr
			SolenoidValve* ptrLV39;// MPS LH2 Topping Vlv OP pwr
			SolenoidValve* ptrLV40;// LO2 Manf Reprss 1 OP Pwr
			SolenoidValve* ptrLV41;// LO2 Manf Reprss 2 OP Pwr
			/*LV42;// LH2 Manf Reprss 1 OP Pwr
			LV43;// LH2 Manf Reprss 2 OP Pwr
			LV46;// LO2 17in Disc Vlv OP Pwr
			LV47;// LO2 17in Disc Vlv CL Pwr
			LV48;// LH2 17in Disc Vlv OP Pwr
			LV49;// LH2 17in Disc Vlv CL Pwr
			LV50;// LH2 4in Disc Vlv Op Pwr
			LV51;// LH2 4in Disc Vlv Cl Pwr
			LV56;// GH2 Press FCV 1 CL Pwr (FCV are NO)
			LV57;// GH2 Press FCV 2 CL Pwr
			LV58;// GH2 Press FCV 3 CL Pwr
			LV59;// E1 He Intcn In OP Pwr
			LV60;// E1 He Intcn Out OP Pwr
			LV61;// E2 He Intcn In OP Pwr
			LV62;// E2 He Intcn Out OP Pwr
			LV63;// E3 He Intcn In OP Pwr
			LV64;// E3 He Intcn Out OP Pwr
			LV65;// LO2 17in Disc Lock Pwr
			LV66;// LO2 17in Disc Unlock Pwr
			LV67;// LH2 17in Disc Lock Pwr
			LV68;// LH2 17in Disc Unlock Pwr*/
			SolenoidValve* ptrLV72;// LH2 B/U Inbd DV OP Pwr
			SolenoidValve* ptrLV73;// LH2 B/U Otbd DV OP Pwr
			/*LV74;// LH2 RTLS Reprss 1 OP Pwr
			LV75;// LH2 RTLS Reprss 2 OP Pwr*/
			SolenoidValve* ptrLV76;// LOX Ovbd B/V CL
			SolenoidValve* ptrLV77;// LO2 Pogo Recrc 1 Cl Pwr
			SolenoidValve* ptrLV78;// LO2 Pogo RECRC 2 Cl PWR
			//LV79;// Helium solenoid valve (PV22 OP ???)
			SolenoidValve* ptrLV80;// E1 LO2 Prevlv CL Pwr 2
			SolenoidValve* ptrLV81;// E2 LO2 Prevlv CL Pwr 2
			SolenoidValve* ptrLV82;// E3 LO2 Prevlv CL Pwr 2
			SolenoidValve* ptrLV83;// E1 LO2 Prevlv OP Pwr 2
			SolenoidValve* ptrLV84;// E2 LO2 Prevlv OP Pwr 2
			SolenoidValve* ptrLV85;// E3 LO2 Prevlv OP Pwr 2
			
			//SolenoidValve* ptrLVXX;// LH2 Ullage pressure

		public:
			MPS( AtlantisSubsystemDirector* _director, HeSysPneu* HeSys );
			~MPS( void );

			void Realize( void );
			virtual void OnPostStep( double fSimT, double fDeltaT, double fMJD );

			void OnSaveState( FILEHANDLE scn ) const;
			bool OnParseLine( const char* line );

			double GetLOXManifPress( void ) const;
			double GetLH2ManifPress( void ) const;
			double GetLOXPVPos( int eng ) const;
			double GetLH2PVPos( int eng ) const;
	};
}


#endif// _mps_MPS_H_