/****************************************************************************
  This file is part of Space Shuttle Ultra

  General Displays definition



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
#ifndef _dps_GENDISP_H_
#define _dps_GENDISP_H_


#include "SimpleGPCSoftware.h"
#include "discsignals.h"


using namespace discsignals; 


namespace dps
{
	class GeneralDisplays:public SimpleGPCSoftware
	{
		private:
			DiscInPort dipBFCCRTDisplay;
			DiscInPort dipBFCCRTSelect[2];

			DiscInPort dipLO2UllagePressureSensor[3];
			DiscInPort dipLH2UllagePressureSensor[3];

			DiscInPort dipHeSysPressureSensor[12];
			DiscInPort dipMPSManifPressureSensor[2];

			double He_dPdT[3];
			double He_P[3];
			double He_T;

			// PASS displays
			void DISP18_PASS( vc::MDU* pMDU ) const;
			void DISP19_PASS( vc::MDU* pMDU ) const;
			void SPEC51_PASS( vc::MDU* pMDU ) const;
			void SPEC53_PASS( vc::MDU* pMDU ) const;
			void SPEC55_PASS( vc::MDU* pMDU ) const;
			void DISP99_PASS( vc::MDU* pMDU ) const;

			// BFS dislays
			void DISP18_BFS( vc::MDU* pMDU ) const;
			void DISP19_BFS( vc::MDU* pMDU ) const;
			void SPEC51_BFS( vc::MDU* pMDU ) const;
			void SPEC55_BFS( vc::MDU* pMDU ) const;
			void DISP99_BFS( vc::MDU* pMDU ) const;


		public:
			GeneralDisplays( SimpleGPCSystem* _gpc );
			~GeneralDisplays( void );

			void Realize();

			void OnPreStep( double SimT, double DeltaT, double MJD );

			bool OnMajorModeChange( unsigned int newMajorMode );

			bool ItemInput( int spec, int item, const char* Data );

			bool OnPaint( int spec, vc::MDU* pMDU ) const;
	};
}


#endif// _dps_GENDISP_H_