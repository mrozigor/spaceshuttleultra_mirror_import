/****************************************************************************
  This file is part of Space Shuttle Ultra

  Space Shuttle Main Engine definition



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
#ifndef _mps_SSME_H_
#define _mps_SSME_H_


#include <orbitersdk.h>
#include "..\AtlantisSubsystem.h"
#include "..\Atlantis.h"
#include "SolenoidValve.h"
#include "PressureActuatedValve.h"
#include "HydraulicActuatedValve.h"


namespace mps
{
	// HACK very little ideia about the real numbers
	const double MAX_RATE_CCV = 120;
	const double MAX_RATE_MOV = 120;
	const double MAX_RATE_MFV = 150;
	const double MAX_RATE_FPOV = 170;
	const double MAX_RATE_OPOV = 210;
	const double RATE_AFV = 100;
	const double RATE_HPV_SV = 1000;
	const double RATE_OBV = 100;
	const double RATE_FBV = 100;
	const double RATE_HPV = 100;
	const double RATE_GCV = 100;
	const double RATE_RIV = 100;


	class HeSysEng;
	class MPS;

	using discsignals::DiscreteBundle;

	class SSME:public AtlantisSubsystem
	{
		friend class EIU;
		friend class SSMEController;
		friend class OutputElectronics;
		friend class OutputElectronics_BLOCK_II;
		friend class InputElectronics_BLOCK_II;
		friend class PneumaticControlAssembly;

		protected:
			unsigned short ID;// engine ID (1 - C, 2 - L, 3 - R)

			HydraulicActuatedValve* ptrCCV;
			HydraulicActuatedValve* ptrMFV;
			HydraulicActuatedValve* ptrMOV;
			HydraulicActuatedValve* ptrFPOV;
			HydraulicActuatedValve* ptrOPOV;
			SolenoidValve* ptrAFV;
			SolenoidValve* ptrHPV_SV;
			PressureActuatedValve* ptrFBV;
			PressureActuatedValve* ptrOBV;
			PressureActuatedValve* ptrHPV;
			PressureActuatedValve* ptrGCV;
			PressureActuatedValve* ptrRIV;

			SSMEController* Controller;
			
			PneumaticControlAssembly* PCA;

			MPS* pMPS;

			bool Igniter_MCC[2];
			bool Igniter_FPB[2];
			bool Igniter_OPB[2];

			double ISP0;
			double ISP1;
			double FPL_THRUST;
			double RPL_PC_PRESS;
			int MPL;// % RPL
			int FPL;// % RPL

			/**
			 * Converts engine level from percentage to chamber pressure (psi)
			 * @param pcPCT engine level (percentage)
			 * @return engine level (chamber pressure)
			 */
			double PCfromPCTtoPSI( double pcPCT ) const;

			/**
			 * Converts engine level from chamber pressure (psi) to percentage
			 * @param pcPSI engine level (chamber pressure)
			 * @return engine level (percentage)
			 */
			double PCfromPSItoPCT( double pcPSI ) const;

			// engine model
			// gets inputs from valve positions and internal values and outputs sensor readings and thrust
			void SSMERUN( double time, double dt );
			virtual void RUN1( double time, double dt ) = 0;// pre-start
			virtual void RUN2( double time, double dt ) = 0;// ignition
			virtual void RUN3( double time, double dt ) = 0;// mainstage
			virtual void RUN4( double time, double dt ) = 0;// cutoff
			virtual void RUN5( double time, double dt ) = 0;// post-shutdown

			short modelmode;
			double modeltime;

			// data cookup fncts
			double dcPC_ESC( double );
			double dcPC_CO( double );

			double AdjCOTime( double pc );

			virtual void ConnectSensors( DiscreteBundle* IEchA_Press, DiscreteBundle* IEchB_Press, DiscreteBundle* IEchA_Temp, DiscreteBundle* IEchB_Temp, DiscreteBundle* IEchA_Flow, DiscreteBundle* IEchB_Flow, DiscreteBundle* IEchA_Speed, DiscreteBundle* IEchB_Speed ) = 0;

		public:
			/**
			 * Create a new SSME object.
			 * @param _director reference to the subsystem director
			 * @param _ident identification of the subsystem inside the model
			 * @param nID identification number of the SSME
			 * @param controllertype identification of the SSME controller version
			 * @param sw identification of the software version of the SSME controller
			 * @param HeSys Helium System supplying the engine
			 */
			SSME( AtlantisSubsystemDirector* _director, const string& _ident, unsigned short nID, int controllertype, const string& sw, HeSysEng* HeSys );
			virtual ~SSME( void );

			/**
			 * Create the thruster reference of the SSME 
			 * and link the cable connections of the SSME.
			 * @sa AtlantisSubsystem::Realize
			 */
			void Realize( void );

			void OnPostStep( double fSimT, double fDeltaT, double fMJD );

			bool SingleParamParseLine() const {return true;};

			void OnSaveState( FILEHANDLE scn ) const;
			bool OnParseLine( const char* line );

			// for use by the derived class
			virtual void __OnSaveState( FILEHANDLE scn ) const = 0;
			virtual bool __OnParseLine( const char* line ) = 0;
	};
}


#endif// _mps_SSME_H_
