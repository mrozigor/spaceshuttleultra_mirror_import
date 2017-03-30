/****************************************************************************
  This file is part of Space Shuttle Ultra

  7-Segment Display VC Component



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
#ifndef __7SD_H
#define __7SD_H


#include "AtlantisVCComponent.h"


namespace vc
{
	const int _7SD_STATE_NUM0 = 0;
	const int _7SD_STATE_NUM1 = 1;
	const int _7SD_STATE_NUM2 = 2;
	const int _7SD_STATE_NUM3 = 3;
	const int _7SD_STATE_NUM4 = 4;
	const int _7SD_STATE_NUM5 = 5;
	const int _7SD_STATE_NUM6 = 6;
	const int _7SD_STATE_NUM7 = 7;
	const int _7SD_STATE_NUM8 = 8;
	const int _7SD_STATE_NUM9 = 9;
	const int _7SD_STATE_NUMOFF = 10;

	const int _7SD_STATE_NUM0_DOTON = 11;
	const int _7SD_STATE_NUM1_DOTON = 12;
	const int _7SD_STATE_NUM2_DOTON = 13;
	const int _7SD_STATE_NUM3_DOTON = 14;
	const int _7SD_STATE_NUM4_DOTON = 15;
	const int _7SD_STATE_NUM5_DOTON = 16;
	const int _7SD_STATE_NUM6_DOTON = 17;
	const int _7SD_STATE_NUM7_DOTON = 18;
	const int _7SD_STATE_NUM8_DOTON = 19;
	const int _7SD_STATE_NUM9_DOTON = 20;
	const int _7SD_STATE_NUMOFF_DOTON = 21;

	const int _7SD_STATE_NUM0_DOTOFF = 22;
	const int _7SD_STATE_NUM1_DOTOFF = 23;
	const int _7SD_STATE_NUM2_DOTOFF = 24;
	const int _7SD_STATE_NUM3_DOTOFF = 25;
	const int _7SD_STATE_NUM4_DOTOFF = 26;
	const int _7SD_STATE_NUM5_DOTOFF = 27;
	const int _7SD_STATE_NUM6_DOTOFF = 28;
	const int _7SD_STATE_NUM7_DOTOFF = 29;
	const int _7SD_STATE_NUM8_DOTOFF = 30;
	const int _7SD_STATE_NUM9_DOTOFF = 31;
	const int _7SD_STATE_NUMOFF_DOTOFF = 32;

	const int _7SD_STATE_SIGNPLUS = 33;
	const int _7SD_STATE_SIGNMINUS = 34;
	const int _7SD_STATE_SIGNOFF = 35;

	const int _7SD_STATE_SIGN2PLUS = 36;
	const int _7SD_STATE_SIGN2MINUS = 37;
	const int _7SD_STATE_SIGN2OFF = 38;
	const int _7SD_STATE_SIGN2ON = 39;

	const int _7SD_STATE_MAX = 40;

	const float _7SD_U_OFFSET[_7SD_STATE_MAX] = {0.0f, 0.0625f, 0.125f, 0.1875f, 0.25f, 0.3125f, 0.375f, 0.4375f, 0.5f, 0.5625f, 0.625f,
					0.0f, 0.0625f, 0.125f, 0.1875f, 0.25f, 0.3125f, 0.375f, 0.4375f, 0.5f, 0.5625f, 0.625f,
					0.0f, 0.0625f, 0.125f, 0.1875f, 0.25f, 0.3125f, 0.375f, 0.4375f, 0.5f, 0.5625f, 0.625f,
					0.75f, 0.75f, 0.75f,
					0.6875f, 0.6875f, 0.6875f, 0.875f};
	const float _7SD_V_OFFSET[_7SD_STATE_MAX] = {0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f,
					0.0625f, 0.0625f, 0.0625f, 0.0625f, 0.0625f, 0.0625f, 0.0625f, 0.0625f, 0.0625f, 0.0625f, 0.0625f,
					0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
					0.0625f, 0.125f, 0.0f,
					0.0625f, 0.125f, 0.0f, 0.0f};


	class _7SegDisp:public AtlantisVCComponent
	{
		protected:
			std::vector<int> default_state;
			std::vector<int> state;
			std::vector<int> next_state;
			//std::vector<bool> IsDoubleSign;
			//std::vector<bool> HasDecimalPoint;
			//std::vector<bool> IsNumber;
			std::vector<UINT> grpIndex;

			UINT panelmesh;

			void UpdateUV( int component );

		public:
			_7SegDisp( Atlantis* _sts, const std::string& _ident );
			virtual ~_7SegDisp();

			virtual void OnPreStep( double SimT, double DeltaT, double MJD );

			void DefineMesh( UINT _panelmesh );
			void DefineComponent( UINT _grpIndex, bool _IsNumber = true, bool _HasDecimalPoint = false, bool _IsDoubleSign = false, int _default_state = _7SD_STATE_NUM0 );

			virtual void VisualCreated( void );

			virtual void Controller( void ) = 0;
	};
};

#endif// __7SD_H