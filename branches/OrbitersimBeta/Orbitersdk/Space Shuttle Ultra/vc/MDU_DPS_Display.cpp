#include "../Atlantis.h"
#include "MDU.h"
#include "../dps/IDP.h"


extern GDIParams g_Param;


namespace vc
{
	//find location on bitmap of letter
	void BitmapLocation(char ch, int &x, int &y)
	{
		int row=ch/16;
		int col=ch-(16*row);
		x=1+18*col;
		y=1+33*row;
	}

	void MDU::DPS( HDC hDC )
	{
		UpdateTextBuffer();
		
		int Save=SaveDC(hDC);
		bool flash=GetFlash();

		//draw stuff
		for(int i=0;i<51;i++) {
			for(int j=0;j<26;j++) {
				if(textBuffer[i][j].cSymbol>='!') {
					if (((textBuffer[i][j].cAttr & dps::DEUATT_FLASHING) == 0) || (flash == true))
					{
						int x, y;
						vc::BitmapLocation(textBuffer[i][j].cSymbol, x, y);

						if ((textBuffer[i][j].cAttr & dps::DEUATT_OVERBRIGHT) != 0)
						{
							// overbright intensity
							BitBlt(hDC, i*5, j*9, 5, 9, g_Param.DeuCharOvrBrgtBitmapDC, (int)(x*0.278), (int)(y*0.272), SRCCOPY);
						}
						else
						{
							// default
							// normal intensity
							BitBlt(hDC, i*5, j*9, 5, 9, g_Param.DeuCharBitmapDC, (int)(x*0.278), (int)(y*0.272), SRCCOPY);
						}
					}
				}
			}
		}

		for(unsigned int i=0;i<lines.size();i++) {
			if (((lines[i].cAttr & dps::DEUATT_FLASHING) == 0) || flash) {
				if ((lines[i].cAttr & dps::DEUATT_OVERBRIGHT) != 0) SelectObject(hDC, gdiOverbrightPen);
				else if ((lines[i].cAttr & dps::DEUATT_DASHED) != 0) SelectObject( hDC, gdiDashedNormalPen );
				else SelectObject(hDC, gdiNormalPen);
				MoveToEx(hDC, lines[i].x0, lines[i].y0, NULL);
				LineTo(hDC, lines[i].x1, lines[i].y1);
			}
		}
		for(unsigned int i=0;i<ellipses.size();i++) {
			if(ellipses[i].cAttr != dps::DEUATT_FLASHING || flash) {
				if(ellipses[i].cAttr == dps::DEUATT_OVERBRIGHT) SelectObject(hDC, gdiOverbrightPen);
				else SelectObject(hDC, gdiNormalPen);
				int startX = (ellipses[i].xLeft+ellipses[i].xRight)/2;
				int startY = ellipses[i].yTop;
				Arc(hDC, ellipses[i].xLeft, ellipses[i].yTop, ellipses[i].xRight, ellipses[i].yBottom, startX, startY, startX, startY);
			}
		}

		// driving IDP and active keyboards display
		SelectObject( hDC, gdiNormalPen );
		Rectangle( hDC, 118, 235, 138, 255 );
		SelectObject( hDC, gdiTahomaFont_h17w6 );
		SetTextColor( hDC, CR_DPS_NORMAL );
		char cbuf[2];
		sprintf_s( cbuf, 2, "%d", GetDrivingIDP() );
		TextOut( hDC, 124, 236, cbuf, 1 );
		int kb = GetIDP()->GetActiveKeyboard();
		if ((kb & 1) == 1)// CDR
		{
			SelectObject( hDC, gdiRedPen );
			Rectangle( hDC, 54, 249, 118, 251 );
		}
		if ((kb & 2) == 2)// PLT
		{
			SelectObject( hDC, gdiYellowPen );
			Rectangle( hDC, 138, 249, 202, 251 );
		}

		RestoreDC(hDC, Save);
		return;
	}

	void MDU::DPS( oapi::Sketchpad* skp )
	{
		UpdateTextBuffer();

		bool flash=GetFlash();
		SURFHANDLE sfh = skp->GetSurface();

		//draw stuff
		for(int i=0;i<51;i++) {
			for(int j=0;j<26;j++) {
				if(textBuffer[i][j].cSymbol>='!') {
					if (((textBuffer[i][j].cAttr & dps::DEUATT_FLASHING) == 0) || (flash == true))
					{
						int x, y;
						vc::BitmapLocation(textBuffer[i][j].cSymbol, x, y);

						if ((textBuffer[i][j].cAttr & dps::DEUATT_OVERBRIGHT) != 0)
						{
							// overbright intensity
							oapiBlt( sfh, g_Param.deu_characters_overbrightSH, i * 5, j * 9, (int)(x * 0.278), (int)(y * 0.272), 5, 9 );
						}
						else
						{
							// default
							// normal intensity
							oapiBlt( sfh, g_Param.deu_charactersSH, i * 5, j * 9, (int)(x * 0.278), (int)(y * 0.272), 5, 9 );
						}
					}
				}
			}
		}

		for(unsigned int i=0;i<lines.size();i++) {
			if (((lines[i].cAttr & dps::DEUATT_FLASHING) == 0) || flash) {
				if ((lines[i].cAttr & dps::DEUATT_OVERBRIGHT) != 0) skp->SetPen( skpOverbrightPen );
				else if ((lines[i].cAttr & dps::DEUATT_DASHED) != 0) skp->SetPen( skpDashedNormalPen );
				else skp->SetPen( skpNormalPen );
				skp->Line( lines[i].x0, lines[i].y0, lines[i].x1, lines[i].y1 );
			}
		}
		for(unsigned int i=0;i<ellipses.size();i++) {
			if(ellipses[i].cAttr != dps::DEUATT_FLASHING || flash) {
				if(ellipses[i].cAttr == dps::DEUATT_OVERBRIGHT) skp->SetPen( skpOverbrightPen );
				else skp->SetPen( skpNormalPen );
				skp->Ellipse( ellipses[i].xLeft, ellipses[i].yTop, ellipses[i].xRight, ellipses[i].yBottom );
			}
		}

		// driving IDP and active keyboards display
		skp->SetPen( skpNormalPen );
		skp->Rectangle( 118, 235, 138, 255 );
		skp->SetFont( skpTahomaFont_h17w6 );
		skp->SetTextColor( CR_DPS_NORMAL );
		char cbuf[2];
		sprintf_s( cbuf, 2, "%d", GetDrivingIDP() );
		skp->Text( 124, 236, cbuf, 1 );
		int kb = GetIDP()->GetActiveKeyboard();
		if ((kb & 1) == 1)// CDR
		{
			skp->SetPen( skpRedPen );
			skp->Rectangle( 54, 249, 118, 251 );
		}
		if ((kb & 2) == 2)// PLT
		{
			skp->SetPen( skpYellowPen );
			skp->Rectangle( 138, 249, 202, 251 );
		}
		return;
	}
};
