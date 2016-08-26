/****************************************************************************
  This file is part of Space Shuttle Ultra

  Master Events Controller definition



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
#ifndef __MEC_H_
#define __MEC_H_


#include "AtlantisSubsystem.h"
#include "DiscInPort.h"


// MEC criitical commands
const unsigned short MEC_CR_SRB_IGNITION_ARM = 0x3EAC;
const unsigned short MEC_CR_SRB_IGNITION_FIRE_1 = 0x3E6A;
const unsigned short MEC_CR_SRB_IGNITION_FIRE_2 = 0x3E42;
const unsigned short MEC_CR_SRB_SEP_ARM = 0x3154;
const unsigned short MEC_CR_SRB_SEP_FIRE_1 = 0x316A;
const unsigned short MEC_CR_SRB_SEP_FIRE_2 = 0x319B;
const unsigned short MEC_CR_ETORB_STRUCT_SEP_ARM = 0xE117;
const unsigned short MEC_CR_ETORB_STRUCT_SEP_FIRE_1 = 0xE168;
const unsigned short MEC_CR_ETORB_STRUCT_SEP_FIRE_2 = 0xE199;
const unsigned short MEC_CR_T0_UMB_RELEASE_ARM = 0xCEDC;
const unsigned short MEC_CR_T0_UMB_RELEASE_FIRE_1 = 0xCE6A;
const unsigned short MEC_CR_T0_UMB_RELEASE_FIRE_2 = 0xCE9A;

// MEC non-critical commands
// ....

// CR buffer index
const int SRB_IGNITION_ARM = 0;
const int SRB_IGNITION_FIRE_1 = 1;
const int SRB_IGNITION_FIRE_2 = 2;
const int SRB_SEP_ARM = 3;
const int SRB_SEP_FIRE_1 = 4;
const int SRB_SEP_FIRE_2 = 5;
const int ETORB_STRUCT_SEP_ARM = 6;
const int ETORB_STRUCT_SEP_FIRE_1 = 7;
const int ETORB_STRUCT_SEP_FIRE_2 = 8;
const int T0_UMB_RELEASE_ARM = 9;
const int T0_UMB_RELEASE_FIRE_1 = 10;
const int T0_UMB_RELEASE_FIRE_2 = 11;
const int CRsize = 12;


using namespace discsignals;


class MasterEventsController:public AtlantisSubsystem
{
	private:
		static bool PIC_SRM_IGNITION;
		static bool PIC_UMBHDPETVAS;
		static bool PIC_SRB_SEP;
		static bool PIC_ET_SEP;

		short ID;
		DiscInPort power;

		bool CR[CRsize];
		
	public:
		MasterEventsController( AtlantisSubsystemDirector* _director, const string& _ident, short nID );
		~MasterEventsController( void );

		//virtual bool SingleParamParseLine() const {return true;};
		bool OnParseLine( const char* line );
		void OnSaveState( FILEHANDLE scn ) const;
		void Realize( void );
		void OnPreStep( double fSimT, double fDeltaT, double fMJD );

		void SetCriticalCmd( unsigned short cmd );
		void MasterResetCmd( void );
};


#endif// __MEC_H_