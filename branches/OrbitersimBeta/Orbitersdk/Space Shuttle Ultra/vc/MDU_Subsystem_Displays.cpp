#include "../Atlantis.h"
#include "MDU.h"
#include "../dps/IDP.h"


namespace vc
{
	void MDU::OMSMPS( oapi::Sketchpad* skp )
	{
		int nPos, nLoc, EngConvert[3]={1, 0, 2};
		double dNum;
		char cbuf[255];

		skp->SetTextColor( CR_DARK_GREEN );
		skp->SetFont( skpArialFont_h13w6 );
		skp->SetPen( skpDarkGreenPen );
		skp->SetBrush( skpBlackBrush );

		skp->Text( 34, 0, "OMS", 3);
		skp->Text( 150, 0, "MPS", 3);
		skp->Line( 80, 6, 80, 250 );
		skp->Line( 1, 6, 20, 6 ); 
		skp->Line( 67, 6, 145, 6 );
		skp->Line( 180, 6, 255, 6 );
		skp->Line( 0, 6, 0, 30 );
		skp->Line( 255, 6, 255, 30 );
		
		skp->SetTextColor( CR_WHITE );
		
		//OMS
		skp->Text( 23, 15, "L", 1 );
		skp->Text( 58, 15, "R", 1 );
		skp->Rectangle( 12, 30, 42, 46 );
		skp->Rectangle( 47, 30, 77, 46 );
		skp->Rectangle( 12, 95, 42, 111 );
		skp->Rectangle( 47, 95, 77, 111 );
		skp->Rectangle( 14, 167, 40, 183 );
		skp->Rectangle( 49, 167, 75, 183 );
		//MPS
		skp->Text( 159, 10, "L/2", 3 );
		skp->Text( 194, 6, "C/1", 3 );
		skp->Text( 229, 10, "R/3", 3 );
		//Tank P
		skp->Rectangle( 100, 24, 129, 40 );
		skp->Rectangle( 153, 24, 182, 40 );
		skp->Rectangle( 188, 19, 217, 35 );
		skp->Rectangle( 223, 24, 252, 40 );
		//Reg Press
		skp->Rectangle( 100, 92, 129, 108 );
		skp->Rectangle( 153, 92, 182, 108 );
		skp->Rectangle( 188, 87, 217, 103 );
		skp->Rectangle( 223, 92, 252, 108 );
		//Engine Pc
		skp->Text( 159, 152, "L/2", 3 );
		skp->Text( 194, 147, "C/1", 3 );
		skp->Text( 229, 152, "R/3", 3 );
		skp->Rectangle( 155, 167, 181, 183 );
		skp->Rectangle( 190, 162, 216, 178 );
		skp->Rectangle( 225, 167, 251, 183 );
		// ENG MANF
		skp->Rectangle( 83, 179, 109, 194 );
		skp->Rectangle( 116, 179, 142, 194 );

		skp->SetPen( skpWhitePen );

		//OMS
		skp->Text( 0, 50, "He", 2 );
		skp->Text( 0, 60, "TK", 2 );
		skp->Text( 3, 70, "P", 1 );
		skp->Rectangle( 19, 50, 34, 90 );
		skp->Rectangle( 54, 50, 69, 90 );
		skp->Line( 34, 62, 40, 62 );
		skp->Line( 69, 62, 75, 62 );

		skp->Text( 0, 115, "N2", 2 );
		skp->Text( 0, 125, "TK", 2 );
		skp->Text( 3, 135, "P", 1 );
		skp->Rectangle( 19, 115, 34, 155 );
		skp->Rectangle( 54, 115, 69, 155 );
		skp->Line( 34, 139, 40, 139 );
		skp->Line( 69, 139, 75, 139 );

		skp->Text( 23, 155, "L", 1 );
		skp->Text( 58, 155, "R", 1 );
		skp->Text( 0, 202, "Pc", 2 );
		skp->Text( 3, 217, "%", 1 );
		skp->Rectangle( 19, 187, 34, 250 );
		skp->Rectangle( 54, 187, 69, 250 );
		skp->Line( 34, 208, 40, 208 );
		skp->Line( 69, 208, 75, 208 );

		//MPS
		skp->Text( 100, 10, "PNEU", 4 );
		skp->Text( 80, 42, "TANK", 4 );
		skp->Text( 88, 52, "P", 1);
		skp->Rectangle( 108, 42, 123, 82 );
		skp->Text( 140, 42, "He", 2 );
		skp->Text( 132, 52, "TANK", 4 );
		skp->Text( 139, 62, "P", 1 );
		skp->Rectangle( 161, 42, 176, 82 );
		skp->Rectangle( 196, 42, 211, 82 );
		skp->Rectangle( 230, 42, 245, 82 );
		skp->Line( 123, 66, 129, 66 );
		skp->Line( 176, 80, 182, 80 );
		skp->Line( 211, 80, 217, 80 );
		skp->Line( 245, 80, 251, 80 );

		skp->Text( 83, 110, "REG", 3 );
		skp->Text( 90, 120, "P", 1 );
		skp->Rectangle( 108, 110, 123, 145 );
		skp->Text( 135, 110, "He", 2 );
		skp->Text( 125, 120, "REG A", 5 );
		skp->Text( 138, 130, "P", 1 );
		skp->Rectangle( 161, 110, 176, 145 );
		skp->Rectangle( 196, 110, 211, 145 );
		skp->Rectangle( 230, 110, 245, 145 );
		skp->Line( 123, 120, 129, 120 );
		skp->Line( 123, 136, 129, 136 );
		skp->Line( 176, 120, 182, 120 );
		skp->Line( 176, 136, 182, 136 );
		skp->Line( 211, 120, 217, 120 );
		skp->Line( 211, 136, 217, 136 );
		skp->Line( 245, 120, 251, 120 );
		skp->Line( 245, 136, 251, 136 );

		skp->Text( 178, 203, "Pc", 2 );
		skp->Text( 182, 213, "%", 1 );
		skp->Text( 212, 203, "Pc", 2 );
		skp->Text( 217, 213, "%", 1 );
		skp->Rectangle( 161, 187, 176, 250 );
		skp->Rectangle( 196, 187, 211, 250 );
		skp->Rectangle( 231, 187, 246, 250 );
		skp->Line( 176, 193, 182, 193 );
		skp->Line( 211, 193, 217, 193 );
		skp->Line( 245, 193, 251, 193 );
		skp->Line( 176, 228, 182, 228 );
		skp->Line( 211, 228, 217, 228 );
		skp->Line( 245, 228, 251, 228 );

		skp->Text( 85, 152, "ENG MANF", 8 );
		skp->Text( 85, 164, "LO2", 3 );
		skp->Text( 118, 164, "LH2", 3 );
		skp->Text( 109, 191, "P", 1 );
		skp->Text( 109, 201, "S", 1 );
		skp->Text( 111, 211, "I", 1 );// font isn't monospace so must "center" this letter
		skp->Text( 109, 221, "A", 1 );
		skp->Rectangle( 88, 200, 103, 239 );
		skp->Rectangle( 121, 200, 136, 239 );
		skp->Line( 102, 206, 108, 206 );
		skp->Line( 136, 213, 142, 213 );
	
		//OMS
		for(nPos=0;nPos<2;nPos++) {
			dNum=STS()->GetPropellantMass(STS()->oms_helium_tank[nPos])*228.6;
			sprintf_s(cbuf, 255, "%04.0f", dNum);
			skp->Text( 13+35*nPos, 31, cbuf, strlen(cbuf) );
			if (dNum >= 1500)
			{
				skp->SetBrush( skpLightGreenBrush );
				skp->SetPen( skpLightGreenPen );
				if (dNum > 5000) dNum = 5000;
			}
			else
			{
				skp->SetBrush( skpRedBrush );
				skp->SetPen( skpRedPen );
				if (dNum < 0) dNum = 0;
			}
			skp->Rectangle( 20+35*nPos, Round( (89-0.0076*dNum) ), 33+35*nPos, 89 );

			dNum = 0;// TODO get val
			sprintf_s(cbuf, 255, "%04.0f", dNum);
			skp->Text( 13+35*nPos, 96, cbuf, strlen(cbuf) );
			if (dNum >= 1200)
			{
				skp->SetBrush( skpLightGreenBrush );
				skp->SetPen( skpLightGreenPen );
				if (dNum > 3000) dNum = 3000;
			}
			else
			{
				skp->SetBrush( skpRedBrush );
				skp->SetPen( skpRedPen );
				if (dNum < 0) dNum = 0;
			}
			skp->Rectangle( 20+35*nPos, Round( (154-0.012667*dNum) ), 33+35*nPos, 154 );
		
			dNum=100.0*STS()->GetThrusterLevel(STS()->th_oms[nPos]) + (STS()->GetAtmPressure() * 0.00011603);// HACK should have this in the sensor
			sprintf_s(cbuf, 255, "%03.0f", dNum);
			skp->Text( 16+35*nPos, 168, cbuf, strlen(cbuf) );
			if (dNum >= 80)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->SetPen( skpWhitePen );
				if (dNum > 120) dNum = 120;
			}
			else if (dNum >= 4)
			{
				skp->SetBrush( skpRedBrush );
				skp->SetPen( skpRedPen );
			}
			else
			{
				skp->SetBrush( skpBlackBrush );
				skp->SetPen( skpBlackPen );
				if (dNum < 0) dNum = 0;
			}
			skp->Rectangle( 20+35*nPos, Round( 249 - (0.508333 * dNum) ), 33+35*nPos, 249 );
		}

		//MPS
		for(nPos=0;nPos<3;nPos++) {
			nLoc=35*EngConvert[nPos];
			if (STS()->status <= 2) dNum = STS()->GetSSMEPress( nPos + 1 );
			else dNum = 0.0;
			sprintf_s(cbuf, 255, "%03.0f", dNum);
			if(EngConvert[nPos]!=1) skp->Text( 157+nLoc, 168, cbuf, strlen(cbuf) );
			else skp->Text( 157+nLoc, 163, cbuf, strlen(cbuf) );
			if (dNum >= 65)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->SetPen( skpWhitePen );
				if (dNum > 109) dNum = 109;
			}
			else
			{
				skp->SetBrush( skpRedBrush );
				skp->SetPen( skpRedPen );
				if (dNum < 45) dNum = 45;
			}
			skp->Rectangle( 162 + nLoc, Round( 291.890625 - (0.953125 * dNum) ), 175 + nLoc, 249 );
		}
	
		// He Tank Press Pneu
		dNum = STS()->GetHeTankPress( 0 );
		sprintf_s( cbuf, 255, "%04.0f", dNum );
		skp->Text( 100, 25, cbuf, strlen( cbuf ) );
		if (dNum >= 3800)
		{
			skp->SetBrush( skpLightGreenBrush );
			skp->SetPen( skpLightGreenPen );
			if (dNum > 5000) dNum = 5000;
		}
		else
		{
			skp->SetBrush( skpRedBrush );
			skp->SetPen( skpRedPen );
			if (dNum < 3000) dNum = 3000;
		}
		skp->Rectangle( 109, Round( 138 - (0.019 * dNum) ), 122, 81 );

		// He Tank Press Eng 2
		dNum = STS()->GetHeTankPress( 2 );
		sprintf_s( cbuf, 255, "%04.0f", dNum );
		skp->Text( 153, 25, cbuf, strlen( cbuf ) );
		if (dNum >= 1150)
		{
			skp->SetBrush( skpLightGreenBrush );
			skp->SetPen( skpLightGreenPen );
			if (dNum > 5000) dNum = 5000;
		}
		else
		{
			skp->SetBrush( skpRedBrush );
			skp->SetPen( skpRedPen );
			if (dNum < 1000) dNum = 1000;
		}
		skp->Rectangle( 162, Round( 90.5 - (0.0095 * dNum) ), 175, 81 );

		// He Tank Press Eng 1
		dNum = STS()->GetHeTankPress( 1 );
		sprintf_s( cbuf, 255, "%04.0f", dNum );
		skp->Text( 188, 20, cbuf, strlen( cbuf ) );
		if (dNum >= 1150)
		{
			skp->SetBrush( skpLightGreenBrush );
			skp->SetPen( skpLightGreenPen );
			if (dNum > 5000) dNum = 5000;
		}
		else
		{
			skp->SetBrush( skpRedBrush );
			skp->SetPen( skpRedPen );
			if (dNum < 1000) dNum = 1000;
		}
		skp->Rectangle( 197, Round( 90.5 - (0.0095 * dNum) ), 210, 81 );

		// He Tank Press Eng 3
		dNum = STS()->GetHeTankPress( 3 );
		sprintf_s( cbuf, 255, "%04.0f", dNum );
		skp->Text( 223, 25, cbuf, strlen( cbuf ) );
		if (dNum >= 1150)
		{
			skp->SetBrush( skpLightGreenBrush );
			skp->SetPen( skpLightGreenPen );
			if (dNum > 5000) dNum = 5000;
		}
		else
		{
			skp->SetBrush( skpRedBrush );
			skp->SetPen( skpRedPen );
			if (dNum < 1000) dNum = 1000;
		}
		skp->Rectangle( 231, Round( 90.5 - (0.0095 * dNum) ), 244, 81 );

		// He Reg Press Pneu
		dNum = STS()->GetHeRegPress( 0 );
		sprintf_s( cbuf, 255, "%04.0f", dNum );
		skp->Text( 100, 93, cbuf, strlen( cbuf ) );
		if ((dNum >= 680) && (dNum <= 810))
		{
			skp->SetBrush( skpLightGreenBrush );
			skp->SetPen( skpLightGreenPen );
		}
		else
		{
			skp->SetBrush( skpRedBrush );
			skp->SetPen( skpRedPen );
			if (dNum < 600) dNum = 600;
			if (dNum > 900) dNum = 900;
		}
		skp->Rectangle( 109, Round( 210 - (0.11 * dNum) ), 122, 144 );

		// He Reg Press Eng 2
		dNum = STS()->GetHeRegPress( 2 );
		sprintf_s( cbuf, 255, "%04.0f", dNum );
		skp->Text( 153, 93, cbuf, strlen( cbuf ) );
		if ((dNum >= 680) && (dNum <= 810))
		{
			skp->SetBrush( skpLightGreenBrush );
			skp->SetPen( skpLightGreenPen );
		}
		else
		{
			skp->SetBrush( skpRedBrush );
			skp->SetPen( skpRedPen );
			if (dNum < 600) dNum = 600;
			if (dNum > 900) dNum = 900;
		}
		skp->Rectangle( 162, Round( 210 - (0.11 * dNum) ), 175, 144 );

		// He Reg Press Eng 1
		dNum = STS()->GetHeRegPress( 1 );
		sprintf_s( cbuf, 255, "%04.0f", dNum );
		skp->Text( 188, 88, cbuf, strlen( cbuf ) );
		if ((dNum >= 680) && (dNum <= 810))
		{
			skp->SetBrush( skpLightGreenBrush );
			skp->SetPen( skpLightGreenPen );
		}
		else
		{
			skp->SetBrush( skpRedBrush );
			skp->SetPen( skpRedPen );
			if (dNum < 600) dNum = 600;
			if (dNum > 900) dNum = 900;
		}
		skp->Rectangle( 197, Round( 210 - (0.11 * dNum) ), 210, 144 );

		// He Reg Press Eng 3
		dNum = STS()->GetHeRegPress( 3 );
		sprintf_s( cbuf, 255, "%04.0f", dNum );
		skp->Text( 223, 93, cbuf, strlen( cbuf ) );
		if ((dNum >= 680) && (dNum <= 810))
		{
			skp->SetBrush( skpLightGreenBrush );
			skp->SetPen( skpLightGreenPen );
		}
		else
		{
			skp->SetBrush( skpRedBrush );
			skp->SetPen( skpRedPen );
			if (dNum < 600) dNum = 600;
			if (dNum > 900) dNum = 900;
		}
		skp->Rectangle( 231, Round( 210 - (0.11 * dNum) ), 244, 144 );

		// ENG MANF LO2
		dNum = STS()->GetLOXManifPress();
		sprintf_s( cbuf, 255, "%03.0f", dNum );
		skp->Text( 85, 180, cbuf, strlen( cbuf ) );
		if (dNum >= 250)
		{
			skp->SetBrush( skpRedBrush );
			skp->SetPen( skpRedPen );
			if (dNum > 300) dNum = 300;
		}
		else
		{
			skp->SetBrush( skpLightGreenBrush );
			skp->SetPen( skpLightGreenPen );
			if (dNum < 0) dNum = 0;
		}
		skp->Rectangle( 89, Round( 238 - (0.123333 * dNum) ), 102, 238 );

		// ENG MANF LH2
		dNum = STS()->GetLH2ManifPress();
		sprintf_s( cbuf, 255, "%03.0f", dNum );
		skp->Text( 118, 180, cbuf, strlen( cbuf ) );
		if (dNum >= 66)
		{
			skp->SetBrush( skpRedBrush );
			skp->SetPen( skpRedPen );
			if (dNum > 100) dNum = 100;
		}
		else
		{
			skp->SetBrush( skpLightGreenBrush );
			skp->SetPen( skpLightGreenPen );
			if (dNum < 0) dNum = 0;
		}
		skp->Rectangle( 122, Round( 238 - (0.37 * dNum) ), 135, 238 );
	}

	void MDU::APUHYD( oapi::Sketchpad* skp )
	{
		int nPos;
		double dNum;
		char cbuf[255];

		skp->SetFont( skpArialFont_h13w6 );
		skp->SetPen( skpDarkGreenPen );
		skp->SetBrush( skpBlackBrush );

		skp->SetTextColor( CR_DARK_GREEN );
		skp->Text( 127, 0, "APU", 3 );
		skp->Line( 1, 6, 123, 6 );
		skp->Line( 156, 6, 255, 6 );
		skp->Line( 0, 6, 0, 16 );
		skp->Line( 255, 6, 255, 16 );

		skp->Text( 115, 165, "HYDRAULIC", 9 );
		skp->Line( 1, 171, 110, 171 );
		skp->Line( 185, 171, 255, 171 );
		skp->Line( 0, 171, 0, 181 );
		skp->Line( 255, 171, 255, 181 );

		skp->SetTextColor( CR_WHITE );

		// APU
		skp->Text( 37, 7, "1", 1 );
		skp->Text( 67, 7, "2", 1 );
		skp->Text( 97, 7, "3", 1 );
		skp->Text( 0, 45, "FUEL", 4 );
		skp->Text( 2, 55, "QTY", 3 );
		skp->Text( 10, 65, "%", 1 );
		skp->Rectangle( 27, 20, 53, 36 );
		skp->Rectangle( 57, 20, 83, 36 );
		skp->Rectangle( 87, 20, 113, 36 );

		skp->Text( 0, 120, "H2O", 3 );
		skp->Text( 0, 130, "QTY", 3 );
		skp->Text( 8, 140, "%", 1 );
		skp->Rectangle( 27, 95, 53, 111 );
		skp->Rectangle( 57, 95, 83, 111 );
		skp->Rectangle( 87, 95, 113, 111 );

		skp->Text( 168, 7, "1", 1 );
		skp->Text( 201, 7, "2", 1 );
		skp->Text( 234, 7, "3", 1 );
		skp->Text( 134, 50, "FUEL", 4 );
		skp->Text( 144, 60, "P", 1 );
		skp->Rectangle( 158, 20, 187, 36 );
		skp->Rectangle( 191, 20, 220, 36 );
		skp->Rectangle( 224, 20, 253, 36 );

		skp->Text( 132, 120, "OIL", 3 );
		skp->Text( 113, 130, "IN TEMP", 7 );
		skp->SetPen( skpWhitePen );
		skp->Ellipse( 134, 144, 137, 147 );// draw circle instead of "º" as it causes problems in current font in D3D9
		skp->SetPen( skpDarkGreenPen );
		skp->Text( 137, 140, "F", 1 );
		skp->Rectangle( 158, 95, 187, 111 );
		skp->Rectangle( 191, 95, 220, 111 );
		skp->Rectangle( 224, 95, 253, 111 );

		// HYD
		skp->Text( 37, 174, "1", 1 );
		skp->Text( 67, 174, "2", 1 );
		skp->Text( 97, 174, "3", 1 );
		skp->Text( 0, 210, "QTY", 3 );
		skp->Text( 8, 220, "%", 1 );
		skp->Rectangle( 27, 187, 53, 203 );
		skp->Rectangle( 57, 187, 83, 203 );
		skp->Rectangle( 87, 187, 113, 203 );

		skp->Text( 168, 174, "1", 1 );
		skp->Text( 201, 174, "2", 1 );
		skp->Text( 234, 174, "3", 1 );
		skp->Text( 125, 215, "PRESS", 5 );
		skp->Rectangle( 158, 187, 187, 203 );
		skp->Rectangle( 191, 187, 220, 203 );
		skp->Rectangle( 224, 187, 253, 203 );
		skp->Text( 185, 239, "L", 1 );
		skp->Text( 218, 239, "L", 1 );
		skp->Text( 250, 239, "L", 1 );

		skp->SetPen( skpWhitePen );
	
		//Fuel Qty
		skp->Rectangle( 32, 40, 47, 83 );
		skp->Rectangle( 62, 40, 77, 83 );
		skp->Rectangle( 92, 40, 107, 83 );
		skp->Line( 47, 74, 53, 74 );
		skp->Line( 77, 74, 83, 74 );
		skp->Line( 107, 74, 113, 74 );

		//H2O Qty
		skp->Rectangle( 32, 115, 47, 158 );
		skp->Rectangle( 62, 115, 77, 158 );
		skp->Rectangle( 92, 115, 107, 158 );
		skp->Line( 47, 141, 53, 141 );
		skp->Line( 77, 141, 83, 141 );
		skp->Line( 107, 141, 113, 141 );

		//Fuel P
		skp->Rectangle( 165, 40, 180, 83 );
		skp->Rectangle( 198, 40, 213, 83 );
		skp->Rectangle( 231, 40, 246, 83 );

		//Oil In Temp
		skp->Rectangle( 165, 115, 180, 158 );
		skp->Rectangle( 198, 115, 213, 158 );
		skp->Rectangle( 231, 115, 246, 158 );
		skp->Line( 180, 153, 186, 153 );
		skp->Line( 213, 153, 219, 153 );
		skp->Line( 246, 153, 252, 153 );
		skp->Line( 180, 133, 186, 133 );
		skp->Line( 213, 133, 219, 133 );
		skp->Line( 246, 133, 252, 133 );

		//Hydraulic Qty
		skp->Rectangle( 32, 207, 47, 250 );
		skp->Rectangle( 62, 207, 77, 250 );
		skp->Rectangle( 92, 207, 107, 250 );
		skp->Line( 47, 233, 53, 233 );
		skp->Line( 77, 233, 83, 233 );
		skp->Line( 107, 233, 113, 233 );
		skp->Line( 47, 210, 53, 210 );
		skp->Line( 77, 210, 83, 210 );
		skp->Line( 107, 210, 113, 210 );

		//Hydraulic Press
		skp->Rectangle( 165, 207, 180, 250 );
		skp->Rectangle( 198, 207, 213, 250 );
		skp->Rectangle( 231, 207, 246, 250 );
		skp->Line( 180, 244, 186, 244 );
		skp->Line( 213, 244, 219, 244 );
		skp->Line( 246, 244, 252, 244 );
		skp->Line( 180, 239, 186, 239 );
		skp->Line( 213, 239, 219, 239 );
		skp->Line( 246, 239, 252, 239 );
		skp->Line( 180, 224, 186, 224 );
		skp->Line( 213, 224, 219, 224 );
		skp->Line( 246, 224, 252, 224 );


		for(nPos=0;nPos<3;nPos++) {
			//Fuel Qty
			dNum=(STS()->pAPU[nPos]->GetFuelLevel()/APU_FUEL_TANK_MASS)*100.0;
			sprintf_s(cbuf, 10, "%03.0f", dNum);
			skp->Text( 29+30*nPos, 22, cbuf, strlen(cbuf) );
			if (dNum >= 20)
			{
				skp->SetBrush( skpLightGreenBrush );
				skp->SetPen( skpLightGreenPen );
				if (dNum > 100) dNum = 100;
			}
			else
			{
				skp->SetBrush( skpRedBrush );
				skp->SetPen( skpRedPen );
				if (dNum < 0) dNum = 0;
			}
			skp->Rectangle( 33+30*nPos, static_cast<int>(82-0.41*dNum), 46+30*nPos, 82 );

			//H2O Qty
			dNum = 0;// TODO get real value
			sprintf_s(cbuf, 10, "%03.0f", dNum);
			skp->Text( 29+30*nPos, 97, cbuf, strlen(cbuf) );
			if (dNum >= 40)
			{
				skp->SetBrush( skpLightGreenBrush );
				skp->SetPen( skpLightGreenPen );
				if (dNum > 100) dNum = 100;
			}
			else
			{
				skp->SetBrush( skpRedBrush );
				skp->SetPen( skpRedPen );
				if (dNum < 0) dNum = 0;
			}
			skp->Rectangle( 33+30*nPos, static_cast<int>(157-0.41*dNum), 46+30*nPos, 157 );

			//Fuel P
			dNum = STS()->pAPU[nPos]->GetFuelPressure();
			sprintf_s(cbuf, 10, "%04.0f", dNum);
			skp->Text( 159+33*nPos, 22, cbuf, strlen(cbuf) );
			skp->SetBrush( skpLightGreenBrush );
			skp->SetPen( skpLightGreenPen );
			if (dNum > 500) dNum = 500;
			if (dNum < 0) dNum = 0;
			skp->Rectangle( 166+33*nPos, static_cast<int>(82-0.082*dNum), 179+33*nPos, 82 );

			//Oil In Temp
			dNum = 0;// TODO get real value
			sprintf_s(cbuf, 10, "%04.0f", dNum);
			skp->Text( 159+33*nPos, 97, cbuf, strlen(cbuf) );
			if (dNum >= 291)
			{
				skp->SetBrush( skpRedBrush );
				skp->SetPen( skpRedPen );
				if (dNum > 500) dNum = 500;
			}
			else if (dNum >= 45)
			{
				skp->SetBrush( skpLightGreenBrush );
				skp->SetPen( skpLightGreenPen );
			}
			else
			{
				skp->SetBrush( skpRedBrush );
				skp->SetPen( skpRedPen );
				if (dNum < 0) dNum = 0;
			}
			skp->Rectangle( 166+33*nPos, static_cast<int>(157-0.082*dNum), 179+33*nPos, 157 );

			//Hydraulic Qty
			dNum = 0;// TODO get real value
			sprintf_s(cbuf, 10, "%03.0f", dNum);
			skp->Text( 29+30*nPos, 189, cbuf, strlen(cbuf) );
			if (dNum >= 96)
			{
				skp->SetBrush( skpRedBrush );
				skp->SetPen( skpRedPen );
				if (dNum > 100) dNum = 100;
			}
			else if (dNum >= 40)
			{
				skp->SetBrush( skpLightGreenBrush );
				skp->SetPen( skpLightGreenPen );
			}
			else
			{
				skp->SetBrush( skpRedBrush );
				skp->SetPen( skpRedPen );
				if (dNum < 0) dNum = 0;
			}
			skp->Rectangle( 33+30*nPos, static_cast<int>(249-0.41*dNum), 46+30*nPos, 249 );

			//Hydraulic Press
			dNum = STS()->pAPU[nPos]->GetHydraulicPressure();
			sprintf_s(cbuf, 10, "%04.0f", dNum);
			skp->Text( 159+33*nPos, 189, cbuf, strlen(cbuf) );
			if (dNum >= 2400)
			{
				skp->SetBrush( skpLightGreenBrush );
				skp->SetPen( skpLightGreenPen );
				if (dNum > 4000) dNum = 4000;
			}
			else if (dNum >= 1001)
			{
				skp->SetBrush( skpRedBrush );
				skp->SetPen( skpRedPen );
			}
			else if (dNum >= 501)
			{
				skp->SetBrush( skpLightGreenBrush );
				skp->SetPen( skpLightGreenPen );
			}
			else
			{
				skp->SetBrush( skpRedBrush );
				skp->SetPen( skpRedPen );
				if (dNum < 0) dNum = 0;
			}
			skp->Rectangle( 166+33*nPos, static_cast<int>(249-0.01025*dNum), 179+33*nPos, 249 );
		}
		return;
	}

	void MDU::SPI( oapi::Sketchpad* skp )
	{
		oapi::IVECTOR2 tri[3];
	
		int nPos;
		double dNum;
		char cbuf[255];
		//Elevons
		skp->SetFont( skpArialFont_h15w5 );
		skp->SetPen( skpWhitePen );
		skp->SetTextColor( CR_WHITE );
		skp->Text( 0, 5, "ELEVONS", 7 );
		skp->Text( 12, 15, "DEG", 3 );
		skp->Line( 10, 55, 10, 221 );
		skp->Line( 21, 55, 21, 221 );
		skp->Line( 55, 55, 55, 221 );
		skp->Line( 66, 55, 66, 221 );
		for(nPos=70;nPos<=130;nPos+=30) {
			skp->Line( 4, nPos, 10, nPos );
			skp->Line( 26, nPos, 20, nPos );
			skp->Line( 49, nPos, 55, nPos );
			skp->Line( 71, nPos, 65, nPos );
			sprintf_s(cbuf, 255, "%+d", 10*((nPos-55)/30)-30);
			skp->Text( 27, nPos-7, cbuf, strlen(cbuf) );
		}
		for(nPos=190;nPos<=220;nPos+=30) {
			skp->Line( 4, nPos, 10, nPos );
			skp->Line( 26, nPos, 20, nPos );
			skp->Line( 49, nPos, 55, nPos );
			skp->Line( 71, nPos, 65, nPos );
			sprintf_s(cbuf, 255, "%+d", 10*((nPos-55)/30)-30);
			skp->Text( 27, nPos-7, cbuf, strlen(cbuf) );
		}
		for(nPos=55;nPos<=220;nPos+=30) {
			skp->Line( 7, nPos, 10, nPos );
			skp->Line( 23, nPos, 20, nPos );
			skp->Line( 52, nPos, 55, nPos );
			skp->Line( 68, nPos, 65, nPos );
		}
		// yellow 0 and line
		skp->SetTextColor( CR_YELLOW );
		skp->SetPen( skpYellowPen );
		skp->Text( 34, 153, "0", 1 );
		skp->Line( 4, 160, 10, 160 );
		skp->Line( 26, 160, 20, 160 );
		skp->Line( 49, 160, 55, 160 );
		skp->Line( 71, 160, 65, 160 );

		//DRAW INSIDE OF BOXES
		skp->SetBrush( skpDarkGrayBrush );
		skp->SetPen( skpDarkGrayPen );
		skp->Rectangle( 12, 50, 19, 226 );
		skp->Rectangle( 57, 50, 64, 226 );
		skp->SetPen( skpDarkGreenPen );
		skp->SetBrush( skpBlackBrush );

		skp->SetPen( skpYellowPen );
		skp->SetBrush( skpYellowBrush );

		if(STS()->HydraulicsOK()) dNum=STS()->aerosurfaces.leftElevon;
		else dNum=0.0;
		nPos= static_cast<int>(160 + (3.0 * dNum));
		tri[0].x = 6;
		tri[0].y = nPos;
		tri[1].x = 14;
		tri[1].y = nPos - 4;
		tri[2].x = 14;
		tri[2].y = nPos + 4;
		skp->Polygon( tri, 3 );
		tri[0].x = 24;
		tri[0].y = nPos;
		tri[1].x = 16;
		tri[1].y = nPos - 4;
		tri[2].x = 16;
		tri[2].y = nPos + 4;
		skp->Polygon( tri, 3 );
		if(STS()->HydraulicsOK()) dNum=STS()->aerosurfaces.rightElevon;
		else dNum=0.0;
		nPos= static_cast<int>(160 + (3.0 * dNum));
		tri[0].x = 51;
		tri[0].y = nPos;
		tri[1].x = 59;
		tri[1].y = nPos - 4;
		tri[2].x = 59;
		tri[2].y = nPos + 4;
		skp->Polygon( tri, 3 );
		tri[0].x = 69;
		tri[0].y = nPos;
		tri[1].x = 61;
		tri[1].y = nPos - 4;
		tri[2].x = 61;
		tri[2].y = nPos + 4;
		skp->Polygon( tri, 3 );

		//DRAW TE UP TE DW
		skp->SetPen( skpDarkGreenPen );
		skp->Line( 5, 45, 5, 35 );
		skp->Line( 5, 35, 35, 35 );
		skp->SetTextColor( CR_DARK_GREEN );
		skp->Text( 40,28,"TE UP",5 );
		skp->Line( 80, 35, 115, 35 );
		skp->Line( 115, 35, 115, 45 );

		//TE DW
		skp->Line( 5, 230, 5, 240 );
		skp->Line( 5, 240, 35, 240 );
		skp->SetTextColor( CR_DARK_GREEN );
		skp->Text( 40,233,"TE DN",5 );
		skp->Line( 80, 240, 115, 240 );
		skp->Line( 115, 240, 115, 230 );
	
		//Body flap
		skp->SetTextColor( CR_WHITE );
		skp->Text( 60, 5, "BODY FLAP", 9 );
		skp->Text( 87, 15, "%", 1 );
		skp->SetPen( skpWhitePen );
		skp->Line( 96, 55, 96, 221 );
		for(nPos=70;nPos<=220;nPos+=33) {
			skp->Line( 98, nPos, 95, nPos );
		}
		for(nPos=55;nPos<=220;nPos+=33) {
			skp->Line( 101, nPos, 95, nPos );
			sprintf_s(cbuf, 255, "%d", 20*((nPos-40)/33));
			skp->Text( 102, nPos-7, cbuf, strlen(cbuf) );
		}
		skp->SetBrush( skpDarkGrayBrush );
		skp->SetPen( skpDarkGrayPen );
		skp->Rectangle( 87, 50, 94, 226 );

		skp->SetPen( skpYellowPen );
		skp->SetBrush( skpYellowBrush );
		tri[0].x = 99;
		tri[0].y = 111;
		tri[1].x = 107;
		tri[1].y = 107;
		tri[2].x = 107;
		tri[2].y = 115;
		skp->Polygon( tri, 3 );
		skp->Rectangle( 107, 107, 111, 115 );
	
		if(STS()->HydraulicsOK()) dNum=STS()->aerosurfaces.bodyFlap;
		else dNum=0.0;
		nPos=static_cast<int>(111.4525 + (4.825 * dNum));// assuming dNum ranges from -11.7º to +22.5º
		tri[0].x = 99;
		tri[0].y = nPos;
		tri[1].x = 91;
		tri[1].y = nPos - 4;
		tri[2].x = 91;
		tri[2].y = nPos + 4;
		skp->Polygon( tri, 3 );

		//Rudder
		skp->SetTextColor( CR_WHITE );
		skp->Text( 160, 25, "RUDDER-DEG", 10 );
		skp->SetPen( skpDarkGrayPen );
		skp->SetBrush( skpDarkGrayBrush );
		skp->Rectangle( 125, 62, 256, 71 );
		skp->SetPen( skpWhitePen );
		skp->Line( 130, 60, 251, 60 );
		for(nPos=130;nPos<=170;nPos+=20) {
			skp->Line( nPos, 53, nPos, 60 );
			sprintf_s(cbuf, 255, "%d", abs(30-10*((nPos-130)/20)));
			skp->Text( nPos-7, 40, cbuf, strlen(cbuf) );
		}
		for(nPos=210;nPos<=250;nPos+=20) {
			skp->Line( nPos, 53, nPos, 60 );
			sprintf_s(cbuf, 255, "%d", abs(30-10*((nPos-130)/20)));
			skp->Text( nPos-7, 40, cbuf, strlen(cbuf) );
		}
		for(nPos=140;nPos<=250;nPos+=20) {
			skp->Line( nPos, 57, nPos, 60 );
		}
		// yellow 0 and line
		skp->SetTextColor( CR_YELLOW );
		skp->SetPen( skpYellowPen );
		skp->SetBrush( skpYellowBrush );
		skp->Text( 187, 40, "0", 1 );
		skp->Line( 190, 53, 190, 60 );

		if(STS()->HydraulicsOK()) dNum=STS()->aerosurfaces.rudder;
		else dNum=0.0;
		nPos=static_cast<int>(190.0+dNum);
		tri[0].x = nPos;
		tri[0].y = 56;
		tri[1].x = nPos - 4;
		tri[1].y = 64;
		tri[2].x = nPos + 4;
		tri[2].y = 64;
		skp->Polygon( tri, 3 );

		skp->SetTextColor( CR_DARK_GREEN );
		skp->Text( 125,73,"L RUD",5 );
		skp->Text( 210,73,"R RUD",5 );
		//Aileron
		skp->SetPen( skpWhitePen );
		skp->Line( 130, 125, 251, 125 );
		skp->SetPen( skpDarkGrayPen );
		skp->SetBrush( skpDarkGrayBrush );
		skp->SetTextColor( CR_WHITE );
		skp->Text( 160, 90, "AILERON-DEG", 11 );
		skp->Rectangle( 125, 127, 256, 135 );
		skp->Text( 123, 105, "5", 1 );
		skp->Text( 245, 105, "5", 1 );
		skp->SetPen( skpWhitePen );
		for(nPos=130;nPos<=178;nPos+=12) {
			skp->Line( nPos, 119, nPos, 125 );
		}
		for(nPos=202;nPos<=250;nPos+=12) {
			skp->Line( nPos, 119, nPos, 125 );
		}
		for(nPos=136;nPos<=250;nPos+=12) {
			skp->Line( nPos, 122, nPos, 125 );
		}

		// yellow 0 and line
		skp->SetTextColor( CR_YELLOW );
		skp->SetPen( skpYellowPen );
		skp->SetBrush( skpYellowBrush );
		skp->Text( 187, 105, "0", 1 );
		skp->Line( 190, 119, 190, 125 );

		if(STS()->HydraulicsOK()) dNum = range( -5, (STS()->aerosurfaces.leftElevon - STS()->aerosurfaces.rightElevon) * 0.5, 5 );
		else dNum=0.0;

		nPos=static_cast<int>(190.0+12.0*dNum);
		tri[0].x = nPos;
		tri[0].y = 121;
		tri[1].x = nPos - 4;
		tri[1].y = 129;
		tri[2].x = nPos + 4;
		tri[2].y = 129;
		skp->Polygon( tri, 3 );
	
		skp->SetTextColor( CR_DARK_GREEN );
		skp->Text( 125,137,"L AIL",5 );
		skp->Text( 220,137,"R AIL",5 );

		//SPEEDBRAKE
		skp->SetTextColor( CR_WHITE );
		skp->Text( 140, 160, "SPEEDBRAKE %", 12 );
		skp->SetTextColor( CR_YELLOW );
		skp->Text( 160, 175,"ACTUAL",6 );
		skp->SetBrush( skpBlackBrush );
		skp->SetPen( skpWhitePen );
		skp->Rectangle( 220, 175, 250, 190 );
		double Actual = STS()->aerosurfaces.speedbrake;
		char ActualBuf[4];
		sprintf_s(ActualBuf, 4, "%03.0lf",Actual);
		skp->Text( 223,175,ActualBuf,strlen(ActualBuf) );
		skp->SetPen( skpWhitePen );
		skp->SetTextColor( CR_WHITE );
		double capt = 0;
		for(int i=140; i<=240; i+=20)
		{
			char captbuf[4];
			sprintf_s(captbuf, 4, "%.0lf",capt);
			skp->Text( i-7,190,captbuf, strlen( captbuf ) );
			capt+=20;
		}

		for(int i=140; i<240; i+=20)
		{
			skp->MoveTo( i, 203 );
			skp->LineTo( i, 208 );
			skp->LineTo( i + 10, 208 );
			skp->MoveTo( i + 10, 205 );
			skp->LineTo( i + 10, 208 );
			skp->LineTo( i + 20, 208 );
			skp->LineTo( i + 20, 203 );
		}

		skp->SetPen( skpDarkGrayPen );
		skp->SetBrush( skpDarkGrayBrush );
		skp->Rectangle( 135, 210, 245, 218 );

		skp->SetPen( skpWhitePen );
		for(int i=140; i<240; i+=20)
		{
			skp->MoveTo( i, 224 );
			skp->LineTo( i, 219 );
			skp->LineTo( i + 10, 219 );
			skp->MoveTo( i + 10, 221 );
			skp->LineTo( i + 10, 219 );
			skp->LineTo( i + 20, 219 );
			skp->LineTo( i + 20, 224 );
		}

		skp->SetTextColor( CR_CYAN );
		skp->Text( 155, 230,"COMMAND",7 );
		skp->SetBrush( skpBlackBrush );
		skp->Rectangle( 220, 230, 250, 245 );
		double Command = STS()->spdb_tgt*100;
		char CommandBuf[4];
		sprintf_s(CommandBuf, 4, "%03.0lf",Command);
		skp->Text( 223,230,CommandBuf,strlen(CommandBuf) );

		skp->SetPen( skpYellowPen );
		skp->SetBrush( skpYellowBrush );
		tri[0].x = static_cast<int>(140+((Actual/100)*100));
		tri[0].y = 206;
		tri[1].x = static_cast<int>(135+((Actual/100)*100));
		tri[1].y = 212;
		tri[2].x = static_cast<int>(145+((Actual/100)*100));
		tri[2].y = 212;
		skp->Polygon( tri, 3 );
	
		skp->SetPen( skpCyanPen );
		skp->SetBrush( skpCyanBrush );
		tri[0].x = static_cast<int>(140+((Command/100)*100));
		tri[0].y = 221;
		tri[1].x = static_cast<int>(135+((Command/100)*100));
		tri[1].y = 214;
		tri[2].x = static_cast<int>(145+((Command/100)*100));
		tri[2].y = 214;
		skp->Polygon( tri, 3 );
		return;
	}
};
