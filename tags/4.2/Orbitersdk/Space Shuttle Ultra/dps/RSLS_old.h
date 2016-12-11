/****************************************************************************
  This file is part of Space Shuttle Ultra

  RSLS software definition



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
#ifndef _RSLS_OLD_H
#define _RSLS_OLD_H
#pragma once


#include "SimpleGPCSoftware.h"
#include "../Atlantis.h"
#include "dps_defs.h"
#include "DiscInPort.h"


using discsignals::DiscInPort;


namespace dps
{
	class SSME_SOP;
	class IO_Control;
	class ATVC_SOP;

	class RSLS_old:public SimpleGPCSoftware
	{
	public:
		RSLS_old( SimpleGPCSystem* _gpc );
		~RSLS_old();

		virtual void OnPostStep(double simT, double dT, double mjd);

		//Communication with LCC
		bool SychronizeCountdown(double mjd);
		void StartRSLSSequence();

		void Abort( void );

		void Realize();
		bool OnMajorModeChange( unsigned int newMajorMode );

		bool GetRSLSAbortFlag( void ) const;
	private:
		SSME_SOP* pSSME_SOP;
		IO_Control* pIO_Control;
		ATVC_SOP* pATVC_SOP;
		double launch_mjd, timeToLaunch, lastTTL, RSLSAbortTime;
		bool Active,Aborted;
		char* RSLSAbortCause;
		int RSLSAbortData;
		bool RSLSAbort;
		bool abortfirstrun;
		bool launchconfiggimbal;
		short engineSD;
		double eng1SDtime;
		double eng2SDtime;
		double eng3SDtime;
		double PV123CLtime[3];
		double PV456CLtime[3];

		DiscInPort PV19_CLInd[2];
		DiscInPort PV4_OPInd[2];
		DiscInPort PV5_OPInd[2];
		DiscInPort PV6_OPInd[2];
		DiscInPort PV20_OPInd;
		DiscInPort PV21_OPInd;
	};
};


#endif //_RSLS_OLD_H
