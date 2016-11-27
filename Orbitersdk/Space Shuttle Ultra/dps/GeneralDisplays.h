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


const double DROOP_ALT = 265000;// ft


using namespace discsignals; 


namespace dps
{
	class AscentDAP;
	class SSME_Operations;
	class SRBSepSequence;
	//class ETSepSequence;

	class GeneralDisplays:public SimpleGPCSoftware
	{
		private:
			AscentDAP* pAscentDAP;
			SSME_Operations* pSSME_Operations;
			SRBSepSequence* pSRBSepSequence;
			//ETSepSequence* pETSepSequence;

			DiscInPort dipBFCCRTDisplay;
			DiscInPort dipBFCCRTSelect[2];

			DiscInPort dipLO2UllagePressureSensor[3];
			DiscInPort dipLH2UllagePressureSensor[3];

			DiscInPort dipHeSysPressureSensor[12];
			DiscInPort dipMPSManifPressureSensor[2];

			DiscInPort LeftRHC[9];
			DiscInPort LeftTHC[18];
			DiscInPort RightRHC[9];
			DiscInPort AftRHC[9];
			DiscInPort AftTHC[18];
			DiscInPort LeftRPTA[3];
			DiscInPort RightRPTA[3];

			double He_dPdT[3];
			double He_P[3];
			double He_T;

			// PASS displays
			void OnPaint_DISP18_PASS( vc::MDU* pMDU ) const;
			void OnPaint_DISP19_PASS( vc::MDU* pMDU ) const;
			void OnPaint_SPEC25_PASS( vc::MDU* pMDU ) const;
			void OnPaint_SPEC43_PASS( vc::MDU* pMDU ) const;
			void OnPaint_SPEC51_PASS( vc::MDU* pMDU ) const;
			void OnPaint_SPEC53_PASS( vc::MDU* pMDU ) const;
			void OnPaint_SPEC55_PASS( vc::MDU* pMDU ) const;
			void OnPaint_DISP99_PASS( vc::MDU* pMDU ) const;
			void OnPaint_SPEC112_PASS( vc::MDU* pMDU ) const;
			void OnPaint_SPEC113_PASS( vc::MDU* pMDU ) const;
			void OnPaint_LAUNCHTRAJ1_PASS( vc::MDU* pMDU ) const;
			void OnPaint_ASCENTTRAJ1_PASS( vc::MDU* pMDU ) const;
			void OnPaint_ASCENTTRAJ2_PASS( vc::MDU* pMDU ) const;

			void ItemInput_SPEC112( int item, const char* Data );
			void ItemInput_SPEC113( int item, const char* Data );

			// BFS dislays
			void OnPaint_DISP18_BFS( vc::MDU* pMDU ) const;
			void OnPaint_DISP19_BFS( vc::MDU* pMDU ) const;
			void OnPaint_SPEC51_BFS( vc::MDU* pMDU ) const;
			void OnPaint_SPEC55_BFS( vc::MDU* pMDU ) const;
			void OnPaint_DISP99_BFS( vc::MDU* pMDU ) const;

			// item state lists for SPEC displays
			bool ITEM_STATE_SPEC112[32];
			bool ITEM_STATE_SPEC113[44];

			void SPEC25_SPEC43_printTHC( vc::MDU* pMDU, double axis_plus, double axis_minus, int x, int y ) const;
			void SPEC25_SPEC43_printRHC_P( vc::MDU* pMDU, double val, int x, int y ) const;
			void SPEC25_SPEC43_printRHC_RY( vc::MDU* pMDU, double val, int x, int y ) const;
			void SPEC43_printRPTA( vc::MDU* pMDU, double val, int x, int y ) const;
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