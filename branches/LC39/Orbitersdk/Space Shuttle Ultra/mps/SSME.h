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
#include "ValveTypeBool.h"


namespace mps
{
	const double MAX_RATE_CCV = 120;// DONO max rate
	const double MAX_RATE_MOV = 120;// DONO max rate
	const double MAX_RATE_MFV = 150;// DONO max rate
	const double MAX_RATE_FPOV = 170;// DONO max rate
	const double MAX_RATE_OPOV = 210;// DONO max rate
	const double MAX_RATE_OBV = 50;// DONO max rate
	const double MAX_RATE_FBV = 50;// DONO max rate
	const double MAX_RATE_AFV = 50;// DONO max rate

	class SSMEController;

	class SSME:public AtlantisSubsystem
	{
		friend class EIU;
		friend class SSMEController;
		friend class OutputElectronics_BLOCK_II;
		friend class InputElectronics_BLOCK_II;

		protected:
			unsigned short ID;// engine ID (1 - C, 2 - L, 3 - R)

			BasicValve* ptrCCV;
			BasicValve* ptrMFV;
			BasicValve* ptrMOV;
			BasicValve* ptrFPOV;
			BasicValve* ptrOPOV;
			ValveTypeBool* ptrFBV;
			ValveTypeBool* ptrOBV;
			ValveTypeBool* ptrAFV;

			SSMEController* Controller;

			bool Igniter_MCC[2];
			bool Igniter_FPB[2];
			bool Igniter_OPB[2];

			double ISP0;
			double ISP1;
			double FPL_THRUST;
			double RPL_PC_PRESS;
			int MPL;// % RPL
			int FPL;// % RPL

			// sfx for lox dump
			PROPELLANT_HANDLE phLOXdump;
			THRUSTER_HANDLE thLOXdump;
			VECTOR3 posLOXdump;
			VECTOR3 dirLOXdump;

			// sensor table
			double* SENSOR_P;// pressure sensors
			double* SENSOR_T;// temperature sensors
			double* SENSOR_S;// speed sensors
			double* SENSOR_F;// flow sensors
			int numP;// number of pressure sensors
			int numT;// number of temperature sensors
			int numS;// number of speed sensors
			int numF;// number of flow sensors

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

		public:
			/**
			 * Create a new SSME object.
			 * @param _director reference to the subsystem director
			 * @param _ident identification of the subsystem inside the model
			 * @param nID identification number of the SSME
			 * @param controllertype identification of the SSME controller version
			 * @param sw identification of the software version of the SSME controller
			 * @param numP number of presure sensors
			 * @param numT number of temperature sensors
			 * @param numS number of speed sensors
			 * @param numF number of flow sensors
			 */
			SSME( AtlantisSubsystemDirector* _director, const string& _ident, unsigned short nID, int controllertype, const string& sw, int numP, int numT, int numS, int numF );
			virtual ~SSME( void );

			/**
			 * Create the thruster reference of the SSME 
			 * and link the cable connections of the SSME.
			 * @sa AtlantisSubsystem::Realize
			 */
			void Realize( void );

			void OnPostStep( double fSimT, double fDeltaT, double fMJD );
			//void OnPreStep( double fSimT, double fDeltaT, double fMJD );

			bool SingleParamParseLine() const {return true;};

			void OnSaveState( FILEHANDLE scn ) const;
			bool OnParseLine( const char* line );

			// for use by the derived class
			virtual void __OnSaveState( FILEHANDLE scn ) const = 0;
			virtual bool __OnParseLine( const char* line ) = 0;

			void PCA( void );
	};
}


#endif// _mps_SSME_H_
