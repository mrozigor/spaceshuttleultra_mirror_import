/****************************************************************************
  This file is part of Space Shuttle Ultra

  Video Control Unit definition



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
#ifndef __VCU_H_
#define __VCU_H_


#include "AtlantisSubsystem.h"
#include "DiscInPort.h"
#include "DiscOutPort.h"


using namespace discsignals;

// input from PB and output to light
const int VideoInputA = 0;
const int VideoInputB = 1;
const int VideoInputC = 2;
const int VideoInputD = 3;
const int VideoInputRMS = 4;
const int VideoInputFltDeck = 5;
const int VideoInputMidDeck = 6;
const int VideoInputPL1 = 7;
const int VideoInputPL2 = 8;
const int VideoInputPL3 = 9;
const int VideoInputMUX1 = 10;
const int VideoInputMUX2 = 11;
const int VideoInputTest = 12;
const int VideoOutputMon1 = 13;
const int VideoOutputMon2 = 14;
const int VideoOutputDownLink = 15;
const int VideoOutputDTV = 16;
const int VideoOutputMUX1L = 17;
const int VideoOutputMUX1R = 18;
const int VideoOutputMUX2L = 19;
const int VideoOutputMUX2R = 20;
const int MenuALC = 21;
const int MenuManGain = 22;
const int MenuColorBalLTLevel = 23;
const int Mode1 = 24;
const int Mode2 = 25;
const int Mode3 = 26;

// input from switches
const int CameraPowerOnA = 27;
const int CameraPowerOffA = 28;
const int CameraPowerOnB = 29;
const int CameraPowerOffB = 30;
const int CameraPowerOnC = 31;
const int CameraPowerOffC = 32;
const int CameraPowerOnD = 33;
const int CameraPowerOffD = 34;
const int CameraPowerOnRMS = 35;
const int CameraPowerOffRMS = 36;

const int CameraCommandReset = 37;
const int CameraCommandLowRate = 38;
const int CameraCommandTiltUp = 39;
const int CameraCommandTiltDown = 40;
const int CameraCommandPanLeft = 41;
const int CameraCommandPanRight = 42;

// output to tb
const int CameraPowerA_TB = 27;
const int CameraPowerB_TB = 28;
const int CameraPowerC_TB = 29;
const int CameraPowerD_TB = 30;
const int CameraPowerRMS_TB = 31;

// output to PTUs
const int PTU_HighRate = 32;
const int PanLeftCameraA = 33;
const int PanRightCameraA = 34;
const int TiltUpCameraA = 35;
const int TiltDownCameraA = 36;
const int PanLeftCameraB = 37;
const int PanRightCameraB = 38;
const int TiltUpCameraB = 39;
const int TiltDownCameraB = 40;
const int PanLeftCameraC = 41;
const int PanRightCameraC = 42;
const int TiltUpCameraC = 43;
const int TiltDownCameraC = 44;
const int PanLeftCameraD = 45;
const int PanRightCameraD = 46;
const int TiltUpCameraD = 47;
const int TiltDownCameraD = 48;
const int PanLeftCameraRMS = 49;
const int PanRightCameraRMS = 50;
const int TiltUpCameraRMS = 51;
const int TiltDownCameraRMS = 52;


// video outputs
const int OUT_MON1 = 0;
const int OUT_MON2 = 1;
const int OUT_DOWNLINK = 2;
const int OUT_DTV = 3;
const int OUT_MUX1L = 4;
const int OUT_MUX1R = 5;
const int OUT_MUX2L = 6;
const int OUT_MUX2R = 7;

// video inputs
const int IN_A = 0;
const int IN_B = 1;
const int IN_C = 2;
const int IN_D = 3;
const int IN_RMS = 4;
const int IN_FD = 5;
const int IN_MD = 6;
const int IN_PL1 = 7;
const int IN_PL2 = 8;
const int IN_PL3 = 9;
const int IN_MUX1 = 10;
const int IN_MUX2 = 11;
const int IN_TEST = 12;


class VideoControlUnit:public AtlantisSubsystem
{
	private:
		DiscInPort input[43];
		DiscOutPort output[53];

		DiscInPort ManPanLeft;
		DiscInPort ManPanRight;
		DiscInPort ManTiltUp;
		DiscInPort ManTiltDown;

		int outsel;// current selected output
		int outsel_in[8];// selected input for each output
		
		bool camerapowerA;
		bool camerapowerB;
		bool camerapowerC;
		bool camerapowerD;
		bool camerapowerRMS;

	public:
		VideoControlUnit( AtlantisSubsystemDirector* _director, const string& _ident );
		virtual ~VideoControlUnit( void );

		virtual void Realize( void );
		virtual bool OnParseLine( const char* line );
		virtual void OnSaveState( FILEHANDLE scn ) const;
		virtual bool SingleParamParseLine( void ) const {return true;};
		virtual void OnPreStep( double fSimT, double fDeltaT, double fMJD );
};


#endif// __VCU_H_