/****************************************************************************
  This file is part of Space Shuttle Ultra

  Multi-Function Display Unit  definition



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
#pragma once

#include <vector>
#include "../dps/dps_defs.h"
#include "vc_defs.h"
#include "AtlantisVCComponent.h"
#include <UltraMath.h>
#include "gcAPI.h"
#include "Sketchpad2.h"


#define CR_BLACK RGB( 0, 0, 0 )//10, 18, 61
#define CR_DARK_GRAY RGB( 60, 60, 80 )
#define CR_LIGHT_GRAY RGB( 180, 180, 200 )
#define CR_WHITE RGB( 255, 255, 255 )
//#define CR_ORANGE RGB( 255, 128, 0 )
#define CR_RED RGB( 255, 0, 0 )
#define CR_YELLOW RGB( 255, 255, 0 )
#define CR_CYAN RGB( 0, 255, 255 )
#define CR_MAGENTA RGB( 255, 0, 255 )
#define CR_LIGHT_GREEN RGB( 0, 255, 0 )
#define CR_DARK_GREEN RGB( 0, 160, 0 )
#define CR_BLUE RGB( 0, 0, 255 )
//#define CR_PINK RGB( 220, 150, 220 )
//#define CR_BROWN RGB( 190, 50, 30 )

#define CR_DPS_NORMAL RGB( 128, 255, 0 )
#define CR_DPS_OVERBRIGHT RGB( 255, 255, 0 )

const int DPS_DISPLAY_VERTICAL_OFFSET = 32;

const int ORBITER_TOP_COUNT = 66;
// horizontal "slices" left to right, nose to tail
const int ORBITER_TOP_X[ORBITER_TOP_COUNT] = {
	0, 1,
	-1, 0, 1, 2,
	-1, 0, 1, 2,
	-1, 2,
	-2, -1, 2, 3,
	-2, -1, 2, 3,
	-2, -1, 2, 3,
	-2, -1, 2, 3,
	-3, -2, 3, 4,
	-4, -3, 4, 5,
	-5, -4, 5, 6,
	-5, -4, 5, 6,
	-5, -4, -3, -2, -1, 2, 3, 4, 5, 6,
	-4, -3, -2, -1, 0, 1, 2, 3, 4, 5,
	0, 1
	};
const int ORBITER_TOP_Y[ORBITER_TOP_COUNT] = {
	0, 0,
	1, 1, 1, 1,
	2, 2, 2, 2,
	3, 3,
	4, 4, 4, 4,
	5, 5, 5, 5,
	6, 6, 6, 6,
	7, 7, 7, 7,
	8, 8, 8, 8,
	9, 9, 9, 9,
	10, 10, 10, 10,
	11, 11, 11, 11,
	12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
	13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
	14, 14
	};

const int ORBITER_SIDE_COUNT = 66;
// vertical "slices" top down, nose to tail
const int ORBITER_SIDE_X[ORBITER_SIDE_COUNT] = {
	-7, -7,
	-6, -6, -6, -6,
	-5, -5, -5, -5,
	-4, -4, -4, -4,
	-3, -3, -3, -3,
	-2, -2, -2, -2,
	-1, -1, -1, -1,
	0, 0, 0, 0,
	1, 1, 1, 1,
	2, 2, 2, 2,
	3, 3, 3, 3,
	4, 4, 4, 4,
	5, 5, 5, 5, 5, 5,
	6, 6, 6, 6, 6, 6, 6, 6,
	7, 7, 7, 7, 7,
	8
	};
const int ORBITER_SIDE_Y[ORBITER_SIDE_COUNT] = {
	-3, -2,
	-4, -3, -2, -1,
	-5, -4, -1, 0,
	-6, -5, -1, 0,
	-6, -5, -1, 0,
	-6, -5, -1, 0,
	-6, -5, -1, 0,
	-6, -5, -1, 0,
	-6, -5, -1, 0,
	-6, -5, -1, 0,
	-7, -6, -1, 0,
	-8, -7, -1, 0,
	-9, -8, -6, -5, -1, 0,
	-9, -8, -7, -6, -5, -4, -2, -1,
	-9, -8, -4, -3, -2,
	-3
	};


namespace vc {


	class MDU: public AtlantisVCComponent
	{	
		double t0;
		bool counting;

		// MEDS pens and brushes
		static HBRUSH gdiBlackBrush;
		static HBRUSH gdiDarkGrayBrush;
		static HBRUSH gdiLightGrayBrush;
		static HBRUSH gdiWhiteBrush;
		static HBRUSH gdiRedBrush;
		static HBRUSH gdiYellowBrush;
		static HBRUSH gdiCyanBrush;
		static HBRUSH gdiMagentaBrush;
		static HBRUSH gdiLightGreenBrush;
		static HBRUSH gdiBlueBrush;

		static oapi::Brush* skpBlackBrush;
		static oapi::Brush* skpDarkGrayBrush;
		static oapi::Brush* skpLightGrayBrush;
		static oapi::Brush* skpWhiteBrush;
		static oapi::Brush* skpRedBrush;
		static oapi::Brush* skpYellowBrush;
		static oapi::Brush* skpCyanBrush;
		static oapi::Brush* skpMagentaBrush;
		static oapi::Brush* skpLightGreenBrush;
		static oapi::Brush* skpBlueBrush;
		
		static HPEN gdiBlackPen;
		static HPEN gdiDarkGrayPen;
		static HPEN gdiLightGrayPen;
		static HPEN gdiLightGrayThickPen;
		static HPEN gdiWhitePen;
		static HPEN gdiRedPen;
		static HPEN gdiYellowPen;
		static HPEN gdiCyanPen;
		static HPEN gdiMagentaPen;
		static HPEN gdiLightGreenPen;
		static HPEN gdiDarkGreenPen;
		static HPEN gdiLightGreenThickPen;

		static oapi::Pen* skpBlackPen;
		static oapi::Pen* skpDarkGrayPen;
		static oapi::Pen* skpLightGrayThickPen;
		static oapi::Pen* skpLightGrayPen;
		static oapi::Pen* skpWhitePen;
		static oapi::Pen* skpRedPen;
		static oapi::Pen* skpYellowPen;
		static oapi::Pen* skpCyanPen;
		static oapi::Pen* skpMagentaPen;
		static oapi::Pen* skpLightGreenPen;
		static oapi::Pen* skpDarkGreenPen;
		static oapi::Pen* skpLightGreenThickPen;

		// DPS pens
		static HPEN gdiOverbrightPen;
		static HPEN gdiNormalPen;
		static HPEN gdiDashedNormalPen;

		static oapi::Pen* skpOverbrightPen;
		static oapi::Pen* skpNormalPen;
		static oapi::Pen* skpDashedNormalPen;

		// fonts
		static HFONT gdiSSUAFont_h20w17;
		static HFONT gdiSSUAFont_h10w10bold;
		static HFONT gdiSSUAFont_h11w9;
		static HFONT gdiSSUBFont_h18w9;
		static HFONT gdiSSUBFont_h12w7;
		static HFONT gdiSSUBFont_h16w9;

		static oapi::Font* skpSSUAFont_h20;
		static oapi::Font* skpSSUAFont_h10bold;
		static oapi::Font* skpSSUAFont_h11;
		static oapi::Font* skpSSUBFont_h18;
		static oapi::Font* skpSSUBFont_h12;
		static oapi::Font* skpSSUBFont_h16;

		void CreateGDIObjects();
		void DestroyGDIObjects();
		void CreateSketchpadObjects();
		void DestroySketchpadObjects();

		static HDC hDC_Tape_MACHV;
		static HDC hDC_Tape_KEAS;
		static HDC hDC_Tape_Alpha;
		static HDC hDC_Tape_H;
		static HDC hDC_Tape_Hdot;
		HDC hDC_ADI;
		HDC hDC_ADI_ORBIT;
		static HDC hDC_ADIMASK;
		static HDC hDC_ADIMASK_ORBIT;

		static HBITMAP hBM_Tape_MACHV_tmp;
		static HBITMAP hBM_Tape_KEAS_tmp;
		static HBITMAP hBM_Tape_Alpha_tmp;
		static HBITMAP hBM_Tape_H_tmp;
		static HBITMAP hBM_Tape_Hdot_tmp;
		HBITMAP hBM_ADI_tmp;
		HBITMAP hBM_ADI_ORBIT_tmp;
		static HBITMAP hBM_ADIMASK_tmp;
		static HBITMAP hBM_ADIMASK_ORBIT_tmp;

		int save_ADI;
		int save_ADI_ORBIT;

		static SURFHANDLE sfh_Tape_MACHV;
		static SURFHANDLE sfh_Tape_KEAS;
		static SURFHANDLE sfh_Tape_Alpha;
		static SURFHANDLE sfh_Tape_H;
		static SURFHANDLE sfh_Tape_Hdot;

		SKETCHMESH hADIball;

		void CreateTapes( void );
		void DestroyTapes( void );
		void CreateADI( void );
		void DestroyADI( void );

		/** 
		 * Paints the DPS display.
		 */
		void DPS( HDC hDC );
		void DPS( oapi::Sketchpad2* skp );

		/**
		 * MEDS Display functions
		 */
		void SystemStatusDisplay_CSTMenu( HDC hDC );
		void SystemStatusDisplay_CSTMenu( oapi::Sketchpad2* skp );
		void SystemStatusDisplay_IDPInteractiveCST( HDC hDC );
		void SystemStatusDisplay_IDPInteractiveCST( oapi::Sketchpad2* skp );
		void AEPFD( HDC hDC );
		void AEPFD( oapi::Sketchpad2* skp );
		void ORBITPFD( HDC hDC );
		void ORBITPFD( oapi::Sketchpad2* skp );
		void OMSMPS( HDC hDC );
		void OMSMPS( oapi::Sketchpad2* skp );
		void APUHYD( HDC hDC );
		void APUHYD( oapi::Sketchpad2* skp );
		void SPI( HDC hDC );
		void SPI( oapi::Sketchpad2* skp );

		void Tape_Alpha( HDC hDC, double MachNumber );
		void Tape_Alpha( oapi::Sketchpad2* skp, double MachNumber );
		void Tape_KEAS_MVR( HDC hDC, double MachNumber );
		void Tape_KEAS_MVR( oapi::Sketchpad2* skp, double MachNumber );
		void Tape_MV_KEAS( HDC hDC, char label, double vel );
		void Tape_MV_KEAS( oapi::Sketchpad2* skp, char label, double vel );
		void Tape_H_Hdot( HDC hDC, double Altitude_ft, double Hdot );
		void Tape_H_Hdot( oapi::Sketchpad2* skp, double Altitude_ft, double Hdot );
		void Tapes_Invalid( HDC hDC );
		void Tapes_Invalid( oapi::Sketchpad2* skp );

		void ADI_STATIC( HDC hDC );
		void ADI_STATIC( oapi::Sketchpad2* skp );
		void ADI_STATIC_ORBIT( HDC hDC );
		void ADI_STATIC_ORBIT( oapi::Sketchpad2* skp );
		void ADI( HDC hDC, double pitch, double roll, double yaw );
		void ADI( oapi::Sketchpad2* skp, double pitch, double roll, double yaw );
		void ADI_ORBIT( HDC hDC, double pitch, double roll, double yaw );
		void ADI_ORBIT( oapi::Sketchpad2* skp, double pitch, double roll, double yaw );
		void ADI_RATE_A( HDC hDC, double pitch, double roll, double yaw, int adirate );// 10/5/1
		void ADI_RATE_A( oapi::Sketchpad2* skp, double pitch, double roll, double yaw, int adirate );// 10/5/1
		void ADI_RATE_B( HDC hDC, double pitch, double roll, double yaw, int adirate, double Altitude_ft );// 5/(5/etc)/5
		void ADI_RATE_B( oapi::Sketchpad2* skp, double pitch, double roll, double yaw, int adirate, double Altitude_ft );// 5/(5/etc)/5
		void ADI_RATE_ORBIT( HDC hDC, double pitch, double roll, double yaw, int adirate );// 10/5/1
		void ADI_RATE_ORBIT( oapi::Sketchpad2* skp, double pitch, double roll, double yaw, int adirate );// 10/5/1
		void ADI_ERROR_A( HDC hDC, double pitch, double roll, double yaw, int adierr );// 10/5/1
		void ADI_ERROR_A( oapi::Sketchpad2* skp, double pitch, double roll, double yaw, int adierr );// 10/5/1
		void ADI_ERROR_B( HDC hDC, double pitch, double roll, double yaw, int adierr );// 25/25/10 5/2/1 2.5/2.5/2.5
		void ADI_ERROR_B( oapi::Sketchpad2* skp, double pitch, double roll, double yaw, int adierr );// 25/25/10 5/2/1 2.5/2.5/2.5
		void ADI_ERROR_C( HDC hDC, double pitch, double roll, double yaw, int adierr );// 25/25/10 1.25/1.25/0.5 2.5/2.5/2.5
		void ADI_ERROR_C( oapi::Sketchpad2* skp, double pitch, double roll, double yaw, int adierr );// 25/25/10 1.25/1.25/0.5 2.5/2.5/2.5
		void ADI_ERROR_D( HDC hDC, double pitch, double roll, double yaw, int adierr );// 20/5/1 10/5/1 2.5/2.5/2.5
		void ADI_ERROR_D( oapi::Sketchpad2* skp, double pitch, double roll, double yaw, int adierr );// 20/5/1 10/5/1 2.5/2.5/2.5
		void ADI_ERROR_ORBIT( HDC hDC, double pitch, double roll, double yaw, int adierr );// 10/5/1
		void ADI_ERROR_ORBIT( oapi::Sketchpad2* skp, double pitch, double roll, double yaw, int adierr );// 10/5/1

		void HSI_A( HDC hDC, double heading, double roll, bool arrowon, double arrowheading );
		void HSI_A( oapi::Sketchpad2* skp, double heading, double roll, bool arrowon, double arrowheading );
		void HSI_E( HDC hDC, double heading, bool arrowon, double arrowheading );
		void HSI_E( oapi::Sketchpad2* skp, double heading, bool arrowon, double arrowheading );
		void HSI_Arrow( HDC hDC, double heading );
		void HSI_Arrow( oapi::Sketchpad2* skp, double heading );

		void AEPFD_Header_AscentDAP( HDC hDC, int MM, int adiatt );
		void AEPFD_Header_AscentDAP( oapi::Sketchpad2* skp, int MM, int adiatt );
		void AEPFD_Header_TransDAP( HDC hDC, int MM, int adiatt );
		void AEPFD_Header_TransDAP( oapi::Sketchpad2* skp, int MM, int adiatt );
		void AEPFD_Header_AerojetDAP( HDC hDC, int MM, double MachNumber );
		void AEPFD_Header_AerojetDAP( oapi::Sketchpad2* skp, int MM, double MachNumber );
		void AEPFD_BETA( HDC hDC );
		void AEPFD_BETA( oapi::Sketchpad2* skp );
		void AEPFD_GMETER_STATIC( HDC hDC );
		void AEPFD_GMETER_STATIC( oapi::Sketchpad2* skp );
		void AEPFD_GMETER_ACCEL( HDC hDC );
		void AEPFD_GMETER_ACCEL( oapi::Sketchpad2* skp );
		void AEPFD_GMETER_NZ( HDC hDC );
		void AEPFD_GMETER_NZ( oapi::Sketchpad2* skp );
		void AEPFD_HACCEL( HDC hDC );
		void AEPFD_HACCEL( oapi::Sketchpad2* skp );
		void AEPFD_RANGERW( HDC hDC );
		void AEPFD_RANGERW( oapi::Sketchpad2* skp );
		void AEPFD_RANGEHACC( HDC hDC );
		void AEPFD_RANGEHACC( oapi::Sketchpad2* skp );
		void AEPFD_dAZ_HTA( HDC hDC, double MachNumber );
		void AEPFD_dAZ_HTA( oapi::Sketchpad2* skp, double MachNumber );
		void AEPFD_dXTRK( HDC hDC );
		void AEPFD_dXTRK( oapi::Sketchpad2* skp );
		void AEPFD_XTRK( HDC hDC );
		void AEPFD_XTRK( oapi::Sketchpad2* skp );
		void AEPFD_dINC( HDC hDC );
		void AEPFD_dINC( oapi::Sketchpad2* skp );
		void AEPFD_TGTINC( HDC hDC );
		void AEPFD_TGTINC( oapi::Sketchpad2* skp );
		void AEPFD_GSI( HDC hDC, double Altitude_ft );
		void AEPFD_GSI( oapi::Sketchpad2* skp, double Altitude_ft );

		inline bool GetFlash( void ) const
		{
			int SimT=(int)(oapiGetSimTime() * 2);
			return (SimT%2)==1;
		}

	protected:
		unsigned short usMDUID;
		SURFHANDLE shLabelTex;
		dps::DEUCHAR textBuffer[51][26];
		dps::IDP* prim_idp;
		dps::IDP* sec_idp;
		bool bInverseX;
		bool bUseSecondaryPort;
		EXTMFDSPEC mfdspec;
		double fBrightness;
		bool bPower;

		int display;
		int menu;
		
		std::vector<dps::DEU_LINE> lines;
		std::vector<dps::DEU_ELLIPSE> ellipses;
		std::vector<dps::DEU_PIXEL> pixels;
		
		float btnPwrXmin, btnPwrXmax, btnPwrYmin, btnPwrYmax;
		float btnBrtXmin, btnBrtXmax, btnBrtYmin, btnBrtYmax;
		float edgekeyXmin, edgekeyXmax, edgekeyYmin, edgekeyYmax;

		//Use a paint buffer for storing primitives?
		virtual void RegisterMFDContext(int id);
		//void DrawCommonHeader(const char* cDispTitle);
		virtual void PrintToBuffer(const char* string, int length, int col, int row, char attributes);

		void DrawMenuButton( HDC hDC, int x );
		void DrawMenuButton( oapi::Sketchpad* skp, int x );

	public:
		MDU(Atlantis* _sts, const string& _ident, unsigned short usMDUID, bool _bUseCRTMFD = true);
		virtual ~MDU();

		virtual bool OnReadState( FILEHANDLE scn );
		virtual void OnSaveState( FILEHANDLE scn ) const;
		virtual bool IsMultiLineSaveState() const { return true; };

		//bool PrintChar(unsigned short x, unsigned short y, DEUCHAR c);
		//bool PrintString(unsigned short x, unsigned short y, char* pText, short sLength, char cAttr = DEUATT_NORMAL);
		//DEUCHAR GetTextBuffer(unsigned short x, unsigned short y) const;
		bool SetPrimaryIDP(dps::IDP* idp);
		bool SetSecondaryIDP(dps::IDP* idp);
		inline dps::IDP* GetIDP() const {
			if(bUseSecondaryPort)
				return sec_idp;
			else
				return prim_idp;
		}
		/**
		 * Does nothing as it currently has no animations.
		 * @sa BasicVCComponent::DefineVCAnimations
		 */
		virtual void DefineVCAnimations(UINT vc_idx);
		/**
		 * handle mouse events
		 * @sa BasicVCComponent::OnMouseEvent
		 */
		virtual bool OnMouseEvent(int _event, float x, float y);

		/** 
		 * Paint the edge menu area on this HDC.
		 */
		virtual void PaintEdgeMenu( HDC hDC );
		/** 
		 * Paint the edge menu area on this Sketchpad.
		 */
		virtual void PaintEdgeMenu( oapi::Sketchpad* skp );
		/**
		 * Register the MFD area for Orbiter. Does nothing when MFD rendering 
		 * is bypassed.
		 */
		virtual bool RealizeMFD(int id);

		/**
		 * define the Area ID of the MDU screen inside the VC. when in 
		 * CRTMFD mode, it paints only the edge key menu area. 
		 */
		virtual bool DefineRegionAID(UINT aid);
		virtual bool DefineVCGroup(UINT mgrp);

		/**
		 * Sets location of power button.
		 * If this function is not called, power button location will be set to default values
		 */
		void SetPowerButtonArea(float xmin, float ymin, float xmax, float ymax);
		/**
		 * Sets location of brightness button.
		 * If this function is not called, brightness button location will be set to default values
		 */
		void SetBrightnessButtonArea(float xmin, float ymin, float xmax, float ymax);
		/**
		 * Sets location of edgekeys.
		 * Parameters specify row containing all 6 edgekeys.
		 * If this function is not called, edgekey location will be set to default values
		 */
		void SetEdgekeyArea(float xmin, float ymin, float xmax, float ymax);

		virtual void UpdateTextBuffer();
		

		//IDP interface functions
		/** 
		 * Act like the curses function
		 */
		inline void mvprint(int x, int y, const char* pszLine, char attributes = 0) 
		{
			//
			PrintToBuffer(pszLine, strlen(pszLine), x, y, attributes);
		}

		/**
		 * Draw line on DEU.
		 * Coordinates should be between 0 and 511
		 */
		inline void Line(int x1, int y1, int x2, int y2, char attributes = 0) 
		{
			dps::DEU_LINE line;
			line.x0 = x1;
			line.y0 = y1 + DPS_DISPLAY_VERTICAL_OFFSET;
			line.x1 = x2;
			line.y1 = y2 + DPS_DISPLAY_VERTICAL_OFFSET;
			line.cAttr = attributes;
			lines.push_back(line);
		}

		/**
		 * Draw ellipse on DEU.
		 * Coordinates should be between 0 and 511
		 */
		inline void Ellipse(int xLeft, int yTop, int xRight, int yBottom, char attributes = 0)
		{
			dps::DEU_ELLIPSE ellipse;
			ellipse.xLeft = xLeft;
			ellipse.yTop = yTop + DPS_DISPLAY_VERTICAL_OFFSET;
			ellipse.xRight = xRight;
			ellipse.yBottom = yBottom + DPS_DISPLAY_VERTICAL_OFFSET;
			ellipse.cAttr = attributes;
			ellipses.push_back(ellipse);
		}

		/**
		 * Draw circle on DEU.
		 * Coordinates should be between 0 and 511
		 */
		inline void Circle(int xCenter, int yCenter, int radius, char attributes = 0)
		{
			Ellipse(xCenter-radius, yCenter-radius, xCenter+radius, yCenter+radius, attributes);
		}

		/**
		 * Draws delta character at specified location on MDU.
		 */
		inline void Delta(int x, int y, char attributes = 0)
		{
			textBuffer[x][y].cSymbol = 255;
			textBuffer[x][y].cAttr = attributes;
		}

		/**
		 * Draws theta character at specified location on MDU.
		 */
		inline void Theta(int x, int y, char attributes = 0)
		{
			textBuffer[x][y].cSymbol = 253;
			textBuffer[x][y].cAttr = attributes;
		}

		/**
		 * Adds dot above specified character on MDU.
		 * This is usually used to signify rates (i.e. rdot or hdot).
		 */
		inline void DotCharacter(int x, int y, char attributes = 0)
		{
			dps::DEU_PIXEL pixel;
			pixel.cAttr = attributes;
			pixel.x = (10 * x) + 5;
			pixel.y = (14 * y) + DPS_DISPLAY_VERTICAL_OFFSET;
			pixels.push_back( pixel );

			pixel.x = (10 * x) + 4;
			//pixel.y = (14 * y) + DPS_DISPLAY_VERTICAL_OFFSET;
			pixels.push_back( pixel );

			pixel.x = (10 * x) + 5;
			pixel.y = (14 * y) - 1 + DPS_DISPLAY_VERTICAL_OFFSET;
			pixels.push_back( pixel );

			pixel.x = (10 * x) + 4;
			//pixel.y = (14 * y) - 1 + DPS_DISPLAY_VERTICAL_OFFSET;
			pixels.push_back( pixel );
		}

		/**
		 * Draws alpha character at specified location on MDU.
		 */
		inline void Alpha(int x, int y, char attributes = 0)
		{
			textBuffer[x][y].cSymbol = 254;
			textBuffer[x][y].cAttr = attributes;
		}

		/**
		 * Draws sigma character at specified location on MDU.
		 */
		inline void Sigma(int x, int y, char attributes = 0)
		{
			textBuffer[x][y].cSymbol = 252;
			textBuffer[x][y].cAttr = attributes;
		}

		/**
		 * Draws up arrow character at specified location on MDU.
		 */
		inline void UpArrow(int x, int y, char attributes = 0)
		{
			textBuffer[x][y].cSymbol = 247;
			textBuffer[x][y].cAttr = attributes;
		}

		/**
		 * Draws down arrow character at specified location on MDU.
		 */
		inline void DownArrow(int x, int y, char attributes = 0)
		{
			textBuffer[x][y].cSymbol = 248;
			textBuffer[x][y].cAttr = attributes;
		}

		/**
		 * Draws left arrow character at specified location on MDU.
		 */
		inline void LeftArrow( int x, int y, char attributes = 0 )
		{
			textBuffer[x][y].cSymbol = 246;
			textBuffer[x][y].cAttr = attributes;
		}

		/**
		 * Draws right arrow character at specified location on MDU.
		 */
		inline void RightArrow( int x, int y, char attributes = 0 )
		{
			textBuffer[x][y].cSymbol = 245;
			textBuffer[x][y].cAttr = attributes;
		}

		/**
		 * Draws the orbiter symbol, as viewed from the top at specified location on MDU.
		 */
		inline void OrbiterSymbolTop( int x, int y, char attributes = 0 )
		{
			dps::DEU_PIXEL pixel;
			pixel.cAttr = attributes;

			for (int i = 0; i < ORBITER_TOP_COUNT; i++)
			{
				pixel.x = ORBITER_TOP_X[i] + x;
				pixel.y = ORBITER_TOP_Y[i] + y + DPS_DISPLAY_VERTICAL_OFFSET;
				pixels.push_back( pixel );
			}
		}

		inline void OrbiterSymbolSide( int x, int y, double rotation, char attributes = 0 )
		{
			double sinrot = sin( rotation );
			double cosrot = cos( rotation );

			dps::DEU_PIXEL pixel;
			pixel.cAttr = attributes;

			for (int i = 0; i < ORBITER_SIDE_COUNT; i++)
			{
				pixel.x = Round( (ORBITER_SIDE_X[i] * cosrot) - (ORBITER_SIDE_Y[i] * sinrot) ) + x ;
				pixel.y = Round( (ORBITER_SIDE_X[i] * sinrot) + (ORBITER_SIDE_Y[i] * cosrot) ) + y + DPS_DISPLAY_VERTICAL_OFFSET;
				pixels.push_back( pixel );
			}
		}

		/**
		 * Draw line on DEU.
		 */
		inline void LeftArrowFull( int x, int y, char attributes = 0 )
		{
			dps::DEU_LINE line;
			line.cAttr = attributes;
			line.x0 = x + 3;
			line.y0 = y - 2 + DPS_DISPLAY_VERTICAL_OFFSET;
			line.x1 = x + 12;
			line.y1 = line.y0;
			lines.push_back( line );

			//line.x0 = x + 3;
			line.y0 = y + DPS_DISPLAY_VERTICAL_OFFSET;
			//line.x1 = x + 12;
			line.y1 = line.y0;
			lines.push_back( line );

			//line.x0 = x + 3;
			line.y0 = y + 2 + DPS_DISPLAY_VERTICAL_OFFSET;
			//line.x1 = x + 12;
			line.y1 = line.y0;
			lines.push_back( line );

			dps::DEU_PIXEL pixel;
			pixel.cAttr = attributes;
			pixel.x = x;
			pixel.y = y + DPS_DISPLAY_VERTICAL_OFFSET;
			pixels.push_back( pixel );

			//pixel.x = x;
			pixel.y = y - 1 + DPS_DISPLAY_VERTICAL_OFFSET;
			pixels.push_back( pixel );
			
			pixel.x = x + 1;
			pixel.y = y + 1 + DPS_DISPLAY_VERTICAL_OFFSET;
			pixels.push_back( pixel );
			
			//pixel.x = x + 1;
			pixel.y = y + DPS_DISPLAY_VERTICAL_OFFSET;
			pixels.push_back( pixel );
			
			//pixel.x = x + 1;
			pixel.y = y - 1 + DPS_DISPLAY_VERTICAL_OFFSET;
			pixels.push_back( pixel );
			
			//pixel.x = x + 1;
			pixel.y = y - 2 + DPS_DISPLAY_VERTICAL_OFFSET;
			pixels.push_back( pixel );
			
			pixel.x = x + 2;
			pixel.y = y + 2 + DPS_DISPLAY_VERTICAL_OFFSET;
			pixels.push_back( pixel );
			
			//pixel.x = x + 2;
			pixel.y = y + 1 + DPS_DISPLAY_VERTICAL_OFFSET;
			pixels.push_back( pixel );
			
			//pixel.x = x + 2;
			pixel.y = y + DPS_DISPLAY_VERTICAL_OFFSET;
			pixels.push_back( pixel );
			
			//pixel.x = x + 2;
			pixel.y = y - 1 + DPS_DISPLAY_VERTICAL_OFFSET;
			pixels.push_back( pixel );
			
			//pixel.x = x + 2;
			pixel.y = y - 2 + DPS_DISPLAY_VERTICAL_OFFSET;
			pixels.push_back( pixel );
			
			//pixel.x = x + 2;
			pixel.y = y - 3 + DPS_DISPLAY_VERTICAL_OFFSET;
			pixels.push_back( pixel );
			
			pixel.x = x + 3;
			pixel.y = y + 3 + DPS_DISPLAY_VERTICAL_OFFSET;
			pixels.push_back( pixel );
			
			//pixel.x = x + 3;
			pixel.y = y + 2 + DPS_DISPLAY_VERTICAL_OFFSET;
			pixels.push_back( pixel );
			
			//pixel.x = x + 3;
			pixel.y = y + 1 + DPS_DISPLAY_VERTICAL_OFFSET;
			pixels.push_back( pixel );
			
			//pixel.x = x + 3;
			pixel.y = y + DPS_DISPLAY_VERTICAL_OFFSET;
			pixels.push_back( pixel );
			
			//pixel.x = x + 3;
			pixel.y = y - 1 + DPS_DISPLAY_VERTICAL_OFFSET;
			pixels.push_back( pixel );
			
			//pixel.x = x + 3;
			pixel.y = y - 2 + DPS_DISPLAY_VERTICAL_OFFSET;
			pixels.push_back( pixel );
			
			//pixel.x = x + 3;
			pixel.y = y - 3 + DPS_DISPLAY_VERTICAL_OFFSET;
			pixels.push_back( pixel );

			//pixel.x = x + 3;
			pixel.y = y - 4 + DPS_DISPLAY_VERTICAL_OFFSET;
			pixels.push_back( pixel );
		}

		/**
		 * Draws sign of number at specified location on MDU.
		 */
		inline void NumberSign( int x, int y, double number, char attributes = 0 )
		{
			if (number > 0) mvprint( x, y, "+", attributes );
			else if (number < 0) mvprint( x, y, "-", attributes );
			else mvprint( x, y, " ", attributes );
		}

		/**
		 * Draws sign of number with brackets at specified location on MDU.
		 */
		inline void NumberSignBracket( int x, int y, double number, char attributes = 0 )
		{
			if (number > 0) mvprint( x, y, "+", attributes );
			else if (number < 0) mvprint( x, y, "-", attributes );
			else mvprint( x, y, " ", attributes );

			Line( x * 10, (y * 14) + 1, (x * 10) + 3, (y * 14) + 1, attributes );
			Line( x * 10, (y * 14) + 12, (x * 10) + 3, (y * 14) + 12, attributes );

			Line( (x * 10) + 9, (y * 14) + 1, (x * 10) + 6, (y * 14) + 1, attributes );
			Line( (x * 10) + 9, (y * 14) + 12, (x * 10) + 6, (y * 14) + 12, attributes );

			Line( x * 10, y * 14, x * 10, (y * 14) + 12, attributes );
			Line( (x * 10) + 9, y * 14, (x * 10) + 9, (y * 14) + 12, attributes );
		}

		/**
		 * Draws underline the character at the specified location on MDU.
		 */
		inline void Underline( int x, int y, char attributes = 0 )
		{
			Line( (x * 10) + 1, (y * 14) + 14, (x * 10) + 9, (y * 14) + 14, attributes );
		}

		virtual bool GetViewAngle() const;
		virtual short GetPortConfig() const;
		virtual bool GetSelectedPort() const;

		/**
		 * Return the identification of the IDP driving this
		 * display. 
		 * @return 0 if not connected to IDP or IDP failed
		 */
		virtual unsigned short GetDrivingIDP() const;

		virtual void PaintDisplay( oapi::Sketchpad* skp );
		virtual bool NavigateMenu( DWORD key );
		virtual char* ButtonLabel( int bt );
		virtual int ButtonMenu( const MFDBUTTONMENU **menu ) const;
	};
};
