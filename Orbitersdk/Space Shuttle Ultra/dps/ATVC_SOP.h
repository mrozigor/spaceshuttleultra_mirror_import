/****************************************************************************
  This file is part of Space Shuttle Ultra

  Ascent Thrust Vector Control Subsystem Operating Program definition



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
#ifndef _dps_ATVC_SOP_H_
#define _dps_ATVC_SOP_H_


#include "SimpleGPCSoftware.h"
#include "discsignals.h"


const double STARTCONFIG_1P = -3;
const double STARTCONFIG_2P = -9;
const double STARTCONFIG_3P = -9;
const double STARTCONFIG_1Y = 0;
const double STARTCONFIG_2Y = 0;
const double STARTCONFIG_3Y = 0;

const double LAUNCHCONFIG_1P = STARTCONFIG_1P;
const double LAUNCHCONFIG_2P = STARTCONFIG_2P;
const double LAUNCHCONFIG_3P = STARTCONFIG_3P;
const double LAUNCHCONFIG_1Y = STARTCONFIG_1Y;
const double LAUNCHCONFIG_2Y = 3.5;
const double LAUNCHCONFIG_3Y = -3.5;

const double MPSDUMPCONFIG_1P = 0;
const double MPSDUMPCONFIG_2P = -10.5;
const double MPSDUMPCONFIG_3P = -10.5;
const double MPSDUMPCONFIG_1Y = 0;
const double MPSDUMPCONFIG_2Y = 5;
const double MPSDUMPCONFIG_3Y = -5;

const double ENTRYSTOWCONFIG_1P = 0;
const double ENTRYSTOWCONFIG_2P = -10.5;
const double ENTRYSTOWCONFIG_3P = -10.5;
const double ENTRYSTOWCONFIG_1Y = 0;
const double ENTRYSTOWCONFIG_2Y = 5;
const double ENTRYSTOWCONFIG_3Y = -5;

const double ENTRYSTOWCHUTECONFIG_1P = 10;
const double ENTRYSTOWCHUTECONFIG_2P = -0.5;
const double ENTRYSTOWCHUTECONFIG_3P = -0.5;
const double ENTRYSTOWCHUTECONFIG_1Y = ENTRYSTOWCONFIG_1Y;
const double ENTRYSTOWCHUTECONFIG_2Y = ENTRYSTOWCONFIG_2Y;
const double ENTRYSTOWCHUTECONFIG_3Y = ENTRYSTOWCONFIG_3Y;


using namespace discsignals;


namespace dps
{
	class ATVC_SOP:public SimpleGPCSoftware
	{
		private:
			DiscOutPort dopPpos[3];
			DiscOutPort dopYpos[3];
			DiscOutPort dopRpos[2];
			DiscOutPort dopTpos[2];

			double Ppos[3];
			double Ypos[3];
			double Rpos[2];
			double Tpos[2];
		public:
			ATVC_SOP( SimpleGPCSystem* _gpc );
			~ATVC_SOP( void );

			void OnPostStep( double SimT, double DeltaT, double MJD );

			void Realize( void );

			virtual bool OnParseLine( const char* keyword, const char* value );
			virtual void OnSaveState( FILEHANDLE scn ) const;

			bool OnMajorModeChange( unsigned int newMajorMode );

			void SetSSMEActPos( int num, double Ppos, double Ypos );
			void SetSRBActPos( int num, double Rpos, double Tpos );
	};
}


#endif// _dps_ATVC_SOP_H_