#include "../Atlantis.h"
#include "MDU.h"
#include "../dps/IDP.h"


namespace vc
{
	void MDU::SystemStatusDisplay_CSTMenu( oapi::Sketchpad* skp )
	{
		char cbuf[8];

		skp->SetFont( skpTahomaFont_h10w4 );
		skp->SetTextColor( CR_WHITE );
		skp->SetPen( skpWhitePen );
		skp->SetBrush( skpBlackBrush );

		// ADC1A
		if (0)
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 3, 3, 35, 39 );
			sprintf_s( cbuf, 8, "%04X", 0 );// Built In Test Summary
			skp->Text( 10, 14, cbuf, strlen( cbuf ) );
			if (0) skp->Text( 5, 27, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 7, 27, "EDUMP", 5 );
			else if (0) skp->Text( 7, 27, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 8, "%06X", 0 );
				skp->Text( 6, 27, cbuf, strlen( cbuf ) );
			}
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 3, 3, 35, 39 );
		}
		skp->Line( 3, 12, 35, 12 );
		skp->Line( 3, 25, 35, 25 );
		skp->Text( 7, 3, "ADC1A", 5 );

		// ADC1B
		if (0)
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 39, 3, 71, 39 );
			sprintf_s( cbuf, 8, "%04X", 0 );// Built In Test Summary
			skp->Text( 46, 14, cbuf, strlen( cbuf ) );
			if (0) skp->Text( 41, 27, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 44, 27, "EDUMP", 5 );
			else if (0) skp->Text( 44, 27, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 8, "%06X", 0 );
				skp->Text( 42, 27, cbuf, strlen( cbuf ) );
			}
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 39, 3, 71, 39 );
		}
		skp->Line( 39, 12, 71, 12 );
		skp->Line( 39, 25, 71, 25 );
		skp->Text( 43, 3, "ADC1B", 5 );

		// ADC2A
		if (0)
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 183, 3, 215, 39 );
			sprintf_s( cbuf, 8, "%04X", 0 );// Built In Test Summary
			skp->Text( 190, 14, cbuf, strlen( cbuf ) );
			if (0) skp->Text( 185, 27, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (1) skp->Text( 187, 27, "EDUMP", 5 );
			else if (0) skp->Text( 187, 27, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 8, "%06X", 0 );
				skp->Text( 186, 27, cbuf, strlen( cbuf ) );
			}
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 183, 3, 215, 39 );
		}
		skp->Line( 183, 12, 215, 12 );
		skp->Line( 183, 25, 215, 25 );
		skp->Text( 187, 3, "ADC2A", 5 );

		// ADC2B
		if (0)
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 219, 3, 251, 39 );
			sprintf_s( cbuf, 8, "%04X", 0 );// Built In Test Summary
			skp->Text( 226, 14, cbuf, strlen( cbuf ) );
			if (0) skp->Text( 221, 27, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 223, 27, "EDUMP", 5 );
			else if (0) skp->Text( 223, 27, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 8, "%06X", 0 );
				skp->Text( 222, 27, cbuf, strlen( cbuf ) );
			}
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 219, 3, 251, 39 );
		}
		skp->Line( 219, 12, 251, 12 );
		skp->Line( 219, 25, 251, 25 );
		skp->Text( 223, 3, "ADC2B", 5 );

		// CDR1
		if (0)// IDP is commanding
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 3, 61, 35, 125 );
			if (1 != 0)// FC bus
			{
				sprintf_s( cbuf, 8, "FC%d", 1 );
				skp->Text( 13, 79, cbuf, 3 );
			}
			if (1) skp->Text( 9, 88, "AUTO", 4 );// MDU port reconfiguration mode
			else skp->Text( 11, 88, "MAN", 3 );
			sprintf_s( cbuf, 8, "%04X", 0 );// Built In Test Summary
			skp->Text( 10, 97, cbuf, strlen( cbuf ) );
			if (1) skp->Text( 5, 106, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 7, 106, "EDUMP", 5 );
			else if (0) skp->Text( 7, 106, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 8, "%06X", 0 );
				skp->Text( 6, 106, cbuf, strlen( cbuf ) );
			}
			sprintf_s( cbuf, 8, "V %02d%02d", 1, 2 );// SW version number
			skp->Text( 6, 115, cbuf, strlen( cbuf ) );
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 3, 61, 35, 125 );
		}
		skp->Line( 3, 70, 35, 70 );
		skp->Line( 3, 79, 35, 79 );
		skp->Line( 3, 88, 35, 88 );
		skp->Line( 3, 97, 35, 97 );
		skp->Line( 3, 106, 35, 106 );
		skp->Line( 3, 115, 35, 115 );
		skp->Line( 19, 70, 19, 79 );
		skp->Text( 10, 61, "CDR1", 4 );
		skp->Text( 9, 70, "3", 1 );
		skp->Text( 25, 70, "1", 1 );

		// CDR2
		if (0)// IDP is commanding
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 39, 61, 71, 125 );
			if (2 != 0)// FC bus
			{
				sprintf_s( cbuf, 8, "FC%d", 2 );
				skp->Text( 49, 79, cbuf, 3 );
			}
			if (1) skp->Text( 45, 88, "AUTO", 4 );// MDU port reconfiguration mode
			else skp->Text( 47, 88, "MAN", 3 );
			sprintf_s( cbuf, 8, "%04X", 0 );// Built In Test Summary
			skp->Text( 46, 97, cbuf, strlen( cbuf ) );
			if (1) skp->Text( 41, 106, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 43, 106, "EDUMP", 5 );
			else if (0) skp->Text( 43, 106, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 8, "%06X", 0 );
				skp->Text( 42, 106, cbuf, strlen( cbuf ) );
			}
			sprintf_s( cbuf, 8, "V %02d%02d", 1, 2 );// SW version number
			skp->Text( 42, 115, cbuf, strlen( cbuf ) );
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 39, 61, 71, 125 );
		}
		skp->Line( 39, 70, 71, 70 );
		skp->Line( 39, 79, 71, 79 );
		skp->Line( 39, 88, 71, 88 );
		skp->Line( 39, 97, 71, 97 );
		skp->Line( 39, 106, 71, 106 );
		skp->Line( 39, 115, 71, 115 );
		skp->Line( 55, 70, 55, 79 );
		skp->Text( 46, 61, "CDR2", 4 );
		skp->Text( 45, 70, "1", 1 );
		skp->Text( 61, 70, "2", 1 );

		// CRT1
		if (0)// IDP is commanding
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 75, 16, 107, 80 );
			if (3 != 0)// FC bus
			{
				sprintf_s( cbuf, 8, "FC%d", 3 );
				skp->Text( 85, 34, cbuf, 3 );
			}
			if (1) skp->Text( 81, 43, "AUTO", 4 );// MDU port reconfiguration mode
			else skp->Text( 83, 43, "MAN", 3 );
			sprintf_s( cbuf, 8, "%04X", 0 );// Built In Test Summary
			skp->Text( 82, 52, cbuf, strlen( cbuf ) );
			if (1) skp->Text( 77, 61, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 79, 61, "EDUMP", 5 );
			else if (0) skp->Text( 79, 61, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 8, "%06X", 0 );
				skp->Text( 78, 61, cbuf, strlen( cbuf ) );
			}
			sprintf_s( cbuf, 8, "V %02d%02d", 1, 2 );// SW version number
			skp->Text( 78, 70, cbuf, strlen( cbuf ) );
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 75, 16, 107, 80 );
		}
		skp->Line( 75, 25, 107, 25 );
		skp->Line( 75, 34, 107, 34 );
		skp->Line( 75, 43, 107, 43 );
		skp->Line( 75, 52, 107, 52 );
		skp->Line( 75, 61, 107, 61 );
		skp->Line( 75, 70, 107, 70 );
		skp->Line( 91, 25, 91, 34 );
		skp->Text( 82, 16, "CRT1", 4 );
		skp->Text( 81, 25, "1", 1 );

		// CRT2
		if (0)// IDP is commanding
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 147, 16, 179, 80 );
			if (4 != 0)// FC bus
			{
				sprintf_s( cbuf, 8, "FC%d", 4 );
				skp->Text( 157, 34, cbuf, 3 );
			}
			if (1) skp->Text( 153, 43, "AUTO", 4 );// MDU port reconfiguration mode
			else skp->Text( 155, 43, "MAN", 3 );
			sprintf_s( cbuf, 8, "%04X", 0 );// Built In Test Summary
			skp->Text( 154, 52, cbuf, strlen( cbuf ) );
			if (1) skp->Text( 149, 61, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 151, 61, "EDUMP", 5 );
			else if (0) skp->Text( 151, 61, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 8, "%06X", 0 );
				skp->Text( 150, 61, cbuf, strlen( cbuf ) );
			}
			sprintf_s( cbuf, 8, "V %02d%02d", 1, 2 );// SW version number
			skp->Text( 150, 70, cbuf, strlen( cbuf ) );
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 147, 16, 179, 80 );
		}
		skp->Line( 147, 25, 179, 25 );
		skp->Line( 147, 34, 179, 34 );
		skp->Line( 147, 43, 179, 43 );
		skp->Line( 147, 52, 179, 52 );
		skp->Line( 147, 61, 179, 61 );
		skp->Line( 147, 70, 179, 70 );
		skp->Line( 163, 25, 163, 34 );
		skp->Text( 154, 16, "CRT2", 4 );
		skp->Text( 153, 25, "2", 1 );

		// CRT3
		if (0)// IDP is commanding
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 111, 61, 143, 125 );
			if (1 != 0)// FC bus
			{
				sprintf_s( cbuf, 8, "FC%d", 1 );
				skp->Text( 121, 79, cbuf, 3 );
			}
			if (1) skp->Text( 117, 88, "AUTO", 4 );// MDU port reconfiguration mode
			else skp->Text( 119, 88, "MAN", 3 );
			sprintf_s( cbuf, 8, "%04X", 0 );// Built In Test Summary
			skp->Text( 118, 97, cbuf, strlen( cbuf ) );
			if (1) skp->Text( 113, 106, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 114, 106, "EDUMP", 5 );
			else if (0) skp->Text( 114, 106, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 8, "%06X", 0 );
				skp->Text( 114, 106, cbuf, strlen( cbuf ) );
			}
			sprintf_s( cbuf, 8, "V %02d%02d", 1, 2 );// SW version number
			skp->Text( 114, 115, cbuf, strlen( cbuf ) );
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 111, 61, 143, 125 );
		}
		skp->Line( 111, 70, 143, 70 );
		skp->Line( 111, 79, 143, 79 );
		skp->Line( 111, 88, 143, 88 );
		skp->Line( 111, 97, 143, 97 );
		skp->Line( 111, 106, 143, 106 );
		skp->Line( 111, 115, 143, 115 );
		skp->Line( 127, 70, 127, 79 );
		skp->Text( 118, 61, "CRT3", 4 );
		skp->Text( 117, 70, "3", 1 );

		// MFD1
		if (0)// IDP is commanding
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 75, 88, 107, 152 );
			if (2 != 0)// FC bus
			{
				sprintf_s( cbuf, 8, "FC%d", 2 );
				skp->Text( 85, 106, cbuf, 3 );
			}
			if (1) skp->Text( 81, 115, "AUTO", 4 );// MDU port reconfiguration mode
			else skp->Text( 83, 115, "MAN", 3 );
			sprintf_s( cbuf, 8, "%04X", 0 );// Built In Test Summary
			skp->Text( 82, 124, cbuf, strlen( cbuf ) );
			if (1) skp->Text( 77, 133, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 79, 133, "EDUMP", 5 );
			else if (0) skp->Text( 79, 133, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 8, "%06X", 0 );
				skp->Text( 78, 133, cbuf, strlen( cbuf ) );
			}
			sprintf_s( cbuf, 8, "V %02d%02d", 1, 2 );// SW version number
			skp->Text( 78, 142, cbuf, strlen( cbuf ) );
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 75, 88, 107, 152 );
		}
		skp->Line( 75, 97, 107, 97 );
		skp->Line( 75, 106, 107, 106 );
		skp->Line( 75, 115, 107, 115 );
		skp->Line( 75, 124, 107, 124 );
		skp->Line( 75, 133, 107, 133 );
		skp->Line( 75, 142, 107, 142 );
		skp->Line( 91, 97, 91, 106 );
		skp->Text( 82, 88, "MFD1", 4 );
		skp->Text( 81, 97, "2", 1 );
		skp->Text( 97, 97, "3", 1 );

		// MFD2
		if (0)// IDP is commanding
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 147, 88, 179, 152 );
			if (3 != 0)// FC bus
			{
				sprintf_s( cbuf, 8, "FC%d", 3 );
				skp->Text( 157, 106, cbuf, 3 );
			}
			if (1) skp->Text( 153, 115, "AUTO", 4 );// MDU port reconfiguration mode
			else skp->Text( 155, 115, "MAN", 3 );
			sprintf_s( cbuf, 8, "%04X", 0 );// Built In Test Summary
			skp->Text( 154, 124, cbuf, strlen( cbuf ) );
			if (1) skp->Text( 149, 133, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 151, 133, "EDUMP", 5 );
			else if (0) skp->Text( 151, 133, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 8, "%06X", 0 );
				skp->Text( 150, 133, cbuf, strlen( cbuf ) );
			}
			sprintf_s( cbuf, 8, "V %02d%02d", 1, 2 );// SW version number
			skp->Text( 150, 142, cbuf, strlen( cbuf ) );
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 147, 88, 179, 152 );
		}
		skp->Line( 147, 97, 179, 97 );
		skp->Line( 147, 106, 179, 106 );
		skp->Line( 147, 115, 179, 115 );
		skp->Line( 147, 124, 179, 124 );
		skp->Line( 147, 133, 179, 133 );
		skp->Line( 147, 142, 179, 142 );
		skp->Line( 163, 97, 163, 106 );
		skp->Text( 154, 88, "MFD2", 4 );
		skp->Text( 153, 97, "1", 1 );
		skp->Text( 169, 97, "3", 1 );

		// PLT1
		if (0)// IDP is commanding
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 183, 61, 215, 125 );
			if (4 != 0)// FC bus
			{
				sprintf_s( cbuf, 8, "FC%d", 4 );
				skp->Text( 193, 79, cbuf, 3 );
			}
			if (1) skp->Text( 189, 88, "AUTO", 4 );// MDU port reconfiguration mode
			else skp->Text( 191, 88, "MAN", 3 );
			sprintf_s( cbuf, 8, "%04X", 0 );// Built In Test Summary
			skp->Text( 190, 97, cbuf, strlen( cbuf ) );
			if (1) skp->Text( 185, 106, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 187, 106, "EDUMP", 5 );
			else if (0) skp->Text( 187, 106, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 8, "%06X", 0 );
				skp->Text( 186, 106, cbuf, strlen( cbuf ) );
			}
			sprintf_s( cbuf, 8, "V %02d%02d", 1, 2 );// SW version number
			skp->Text( 186, 115, cbuf, strlen( cbuf ) );
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 183, 61, 215, 125 );
		}
		skp->Line( 183, 70, 215, 70 );
		skp->Line( 183, 79, 215, 79 );
		skp->Line( 183, 88, 215, 88 );
		skp->Line( 183, 97, 215, 97 );
		skp->Line( 183, 106, 215, 106 );
		skp->Line( 183, 115, 215, 115 );
		skp->Line( 199, 70, 199, 79 );
		skp->Text( 190, 61, "PLT1", 4 );
		skp->Text( 189, 70, "2", 1 );
		skp->Text( 205, 70, "1", 1 );

		// PLT2
		if (0)// IDP is commanding
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 219, 61, 251, 125 );
			if (1 != 0)// FC bus
			{
				sprintf_s( cbuf, 8, "FC%d", 1 );
				skp->Text( 229, 79, cbuf, 3 );
			}
			if (1) skp->Text( 225, 88, "AUTO", 4 );// MDU port reconfiguration mode
			else skp->Text( 227, 88, "MAN", 3 );
			sprintf_s( cbuf, 8, "%04X", 0 );// Built In Test Summary
			skp->Text( 226, 97, cbuf, strlen( cbuf ) );
			if (1) skp->Text( 221, 106, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 223, 106, "EDUMP", 5 );
			else if (0) skp->Text( 223, 106, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 8, "%06X", 0 );
				skp->Text( 222, 106, cbuf, strlen( cbuf ) );
			}
			sprintf_s( cbuf, 8, "V %02d%02d", 1, 2 );// SW version number
			skp->Text( 222, 115, cbuf, strlen( cbuf ) );
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 219, 61, 251, 125 );
		}
		skp->Line( 219, 70, 251, 70 );
		skp->Line( 219, 79, 251, 79 );
		skp->Line( 219, 88, 251, 88 );
		skp->Line( 219, 97, 251, 97 );
		skp->Line( 219, 106, 251, 106 );
		skp->Line( 219, 115, 251, 115 );
		skp->Line( 235, 70, 235, 79 );
		skp->Text( 226, 61, "PLT2", 4 );
		skp->Text( 225, 70, "3", 1 );
		skp->Text( 241, 70, "2", 1 );

		// CRT4
		if (0)// IDP is commanding
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 183, 161, 215, 225 );
			if (2 != 0)// FC bus
			{
				sprintf_s( cbuf, 8, "FC%d", 2 );
				skp->Text( 193, 179, cbuf, 3 );
			}
			if (1) skp->Text( 189, 188, "AUTO", 4 );// MDU port reconfiguration mode
			else skp->Text( 191, 188, "MAN", 3 );
			sprintf_s( cbuf, 8, "%04X", 0 );// Built In Test Summary
			skp->Text( 190, 197, cbuf, strlen( cbuf ) );
			if (1) skp->Text( 185, 206, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 187, 206, "EDUMP", 5 );
			else if (0) skp->Text( 187, 206, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 8, "%06X", 0 );
				skp->Text( 186, 206, cbuf, strlen( cbuf ) );
			}
			sprintf_s( cbuf, 8, "V %02d%02d", 1, 2 );// SW version number
			skp->Text( 186, 215, cbuf, strlen( cbuf ) );
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 183, 161, 215, 225 );
		}
		skp->Line( 183, 170, 215, 170 );
		skp->Line( 183, 179, 215, 179 );
		skp->Line( 183, 188, 215, 188 );
		skp->Line( 183, 197, 215, 197 );
		skp->Line( 183, 206, 215, 206 );
		skp->Line( 183, 215, 215, 215 );
		skp->Line( 199, 170, 199, 179 );
		skp->Text( 190, 161, "CRT4", 4 );
		skp->Text( 189, 170, "4", 1 );

		// AFD1
		if (0)// IDP is commanding
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 219, 161, 251, 225 );
			if (3 != 0)// FC bus
			{
				sprintf_s( cbuf, 8, "FC%d", 3 );
				skp->Text( 229, 179, cbuf, 3 );
			}
			if (1) skp->Text( 225, 188, "AUTO", 4 );// MDU port reconfiguration mode
			else skp->Text( 227, 188, "MAN", 3 );
			sprintf_s( cbuf, 8, "%04X", 0 );// Built In Test Summary
			skp->Text( 226, 197, cbuf, strlen( cbuf ) );
			if (1) skp->Text( 221, 206, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 223, 206, "EDUMP", 5 );
			else if (0) skp->Text( 223, 206, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 8, "%06X", 0 );
				skp->Text( 222, 206, cbuf, strlen( cbuf ) );
			}
			sprintf_s( cbuf, 8, "V %02d%02d", 1, 2 );// SW version number
			skp->Text( 222, 215, cbuf, strlen( cbuf ) );
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 219, 161, 251, 225 );
		}
		skp->Line( 219, 170, 251, 170 );
		skp->Line( 219, 179, 251, 179 );
		skp->Line( 219, 188, 251, 188 );
		skp->Line( 219, 197, 251, 197 );
		skp->Line( 219, 206, 251, 206 );
		skp->Line( 219, 215, 251, 215 );
		skp->Line( 235, 170, 235, 179 );
		skp->Text( 226, 161, "AFD1", 4 );
		skp->Text( 225, 170, "4", 1 );
		skp->Text( 241, 170, "2", 1 );

		// IDP1
		if (GetDrivingIDP() == 1)// commanding IDP
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 3, 179, 39, 225 );
			sprintf_s( cbuf, 8, "%04X", 0 );// Built In Test Summary
			skp->Text( 13, 188, cbuf, strlen( cbuf ) );
			if (1) skp->Text( 7, 197, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 9, 197, "EDUMP", 5 );
			else if (0) skp->Text( 9, 197, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 8, "%06X", 0 );
				skp->Text( 8, 197, cbuf, strlen( cbuf ) );
			}
			if (1)// flight-critical buses selected (1)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 4, 207, 12, 215 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 6, 206, "1", 1 );
			if (0)// flight-critical buses selected (2)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 12, 207, 20, 215 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 14, 206, "2", 1 );
			if (1)// flight-critical buses selected (3)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 20, 207, 29, 215 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 22, 206, "3", 1 );
			if (0)// flight-critical buses selected (4)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 29, 207, 38, 215 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 31, 206, "4", 1 );
			skp->SetTextColor( CR_WHITE );
			sprintf_s( cbuf, 8, "V %02d%02d", 1, 2 );// SW version number
			skp->Text( 8, 215, cbuf, strlen( cbuf ) );
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 3, 179, 39, 225 );
			skp->SetTextColor( CR_WHITE );
			skp->Text( 6, 206, "1", 1 );
			skp->Text( 14, 206, "2", 1 );
			skp->Text( 22, 206, "3", 1 );
			skp->Text( 31, 206, "4", 1 );
		}
		skp->Line( 3, 188, 39, 188 );
		skp->Line( 3, 197, 39, 197 );
		skp->Line( 3, 206, 39, 206 );
		skp->Line( 3, 215, 39, 215 );
		skp->Text( 13, 179, "IDP1", 4 );

		// IDP2
		if (GetDrivingIDP() == 2)// commanding IDP
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 43, 179, 79, 225 );
			sprintf_s( cbuf, 8, "%04X", 0 );// Built In Test Summary
			skp->Text( 53, 188, cbuf, strlen( cbuf ) );
			if (0) skp->Text( 47, 197, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (1) skp->Text( 49, 197, "EDUMP", 5 );
			else if (0) skp->Text( 49, 197, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 8, "%06X", 0 );
				skp->Text( 48, 197, cbuf, strlen( cbuf ) );
			}
			if (0)// flight-critical buses selected (1)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 44, 207, 52, 215 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 46, 206, "1", 1 );
			if (1)// flight-critical buses selected (2)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 52, 207, 60, 215 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 54, 206, "2", 1 );
			if (0)// flight-critical buses selected (3)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 60, 207, 69, 215 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 62, 206, "3", 1 );
			if (1)// flight-critical buses selected (4)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 69, 207, 78, 215 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 71, 206, "4", 1 );
			skp->SetTextColor( CR_WHITE );
			sprintf_s( cbuf, 8, "V %02d%02d", 1, 2 );// SW version number
			skp->Text( 48, 215, cbuf, strlen( cbuf ) );
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 43, 179, 79, 225 );
			skp->SetTextColor( CR_WHITE );
			skp->Text( 46, 206, "1", 1 );
			skp->Text( 54, 206, "2", 1 );
			skp->Text( 62, 206, "3", 1 );
			skp->Text( 71, 206, "4", 1 );
		}
		skp->Line( 43, 188, 79, 188 );
		skp->Line( 43, 197, 79, 197 );
		skp->Line( 43, 206, 79, 206 );
		skp->Line( 43, 215, 79, 215 );
		skp->Text( 53, 179, "IDP2", 4 );

		// IDP3
		if (GetDrivingIDP() == 3)// commanding IDP
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 83, 179, 119, 225 );
			sprintf_s( cbuf, 8, "%04X", 0 );// Built In Test Summary
			skp->Text( 93, 188, cbuf, strlen( cbuf ) );
			if (0) skp->Text( 87, 197, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 89, 197, "EDUMP", 5 );
			else if (1) skp->Text( 89, 197, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 8, "%06X", 0 );
				skp->Text( 88, 197, cbuf, strlen( cbuf ) );
			}
			if (1)// flight-critical buses selected (1)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 84, 207, 92, 215 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 86, 206, "1", 1 );
			if (0)// flight-critical buses selected (2)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 92, 207, 100, 215 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 94, 206, "2", 1 );
			if (1)// flight-critical buses selected (3)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 100, 207, 109, 215 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 102, 206, "3", 1 );
			if (0)// flight-critical buses selected (4)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 109, 207, 118, 215 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 111, 206, "4", 1 );
			skp->SetTextColor( CR_WHITE );
			sprintf_s( cbuf, 8, "V %02d%02d", 1, 2 );// SW version number
			skp->Text( 88, 215, cbuf, strlen( cbuf ) );
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 83, 179, 119, 225 );
			skp->SetTextColor( CR_WHITE );
			skp->Text( 86, 206, "1", 1 );
			skp->Text( 94, 206, "2", 1 );
			skp->Text( 102, 206, "3", 1 );
			skp->Text( 111, 206, "4", 1 );
		}
		skp->Line( 83, 188, 119, 188 );
		skp->Line( 83, 197, 119, 197 );
		skp->Line( 83, 206, 119, 206 );
		skp->Line( 83, 215, 119, 215 );
		skp->Text( 93, 179, "IDP3", 4 );

		// IDP4
		if (GetDrivingIDP() == 4)// commanding IDP
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 123, 179, 159, 225 );
			sprintf_s( cbuf, 8, "%04X", 0 );// Built In Test Summary
			skp->Text( 133, 188, cbuf, strlen( cbuf ) );
			if (0) skp->Text( 127, 197, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 129, 197, "EDUMP", 5 );
			else if (0) skp->Text( 129, 197, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 8, "%06X", 0 );
				skp->Text( 128, 197, cbuf, strlen( cbuf ) );
			}
			if (0)// flight-critical buses selected (1)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 124, 207, 132, 215 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 126, 206, "1", 1 );
			if (1)// flight-critical buses selected (2)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 132, 207, 140, 215 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 134, 206, "2", 1 );
			if (0)// flight-critical buses selected (3)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 140, 207, 149, 215 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 142, 206, "3", 1 );
			if (1)// flight-critical buses selected (4)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 149, 207, 158, 215 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 151, 206, "4", 1 );
			skp->SetTextColor( CR_WHITE );
			sprintf_s( cbuf, 8, "V %02d%02d", 1, 2 );// SW version number
			skp->Text( 128, 215, cbuf, strlen( cbuf ) );
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 123, 179, 159, 225 );
			skp->SetTextColor( CR_WHITE );
			skp->Text( 126, 206, "1", 1 );
			skp->Text( 134, 206, "2", 1 );
			skp->Text( 142, 206, "3", 1 );
			skp->Text( 151, 206, "4", 1 );
		}
		skp->Line( 123, 188, 159, 188 );
		skp->Line( 123, 197, 159, 197 );
		skp->Line( 123, 206, 159, 206 );
		skp->Line( 123, 215, 159, 215 );
		skp->Text( 133, 179, "IDP4", 4 );

		return;
	}

	void MDU::SystemStatusDisplay_IDPInteractiveCST( oapi::Sketchpad* skp )
	{
		char cbuf[16];

		skp->SetFont( skpTahomaFont_h10w4 );
		skp->SetTextColor( CR_WHITE );
		skp->SetPen( skpWhitePen );
		skp->SetBrush( skpBlackBrush );

		// ADC1A
		if (0)
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 3, 170, 35, 206 );
			sprintf_s( cbuf, 16, "%04X", 0 );// Built In Test Summary
			skp->Text( 10, 181, cbuf, strlen( cbuf ) );
			if (0) skp->Text( 5, 194, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 7, 194, "EDUMP", 5 );
			else if (0) skp->Text( 7, 194, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 16, "%06X", 0 );
				skp->Text( 6, 194, cbuf, strlen( cbuf ) );
			}
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 3, 170, 35, 206 );
		}
		skp->Line( 3, 179, 35, 179 );
		skp->Line( 3, 192, 35, 192 );
		skp->Text( 7, 170, "ADC1A", 5 );

		// ADC1B
		if (0)
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 39, 170, 71, 206 );
			sprintf_s( cbuf, 16, "%04X", 0 );// Built In Test Summary
			skp->Text( 46, 181, cbuf, strlen( cbuf ) );
			if (0) skp->Text( 41, 194, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 44, 194, "EDUMP", 5 );
			else if (0) skp->Text( 44, 194, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 16, "%06X", 0 );
				skp->Text( 42, 194, cbuf, strlen( cbuf ) );
			}
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 39, 170, 71, 206 );
		}
		skp->Line( 39, 179, 71, 179 );
		skp->Line( 39, 192, 71, 192 );
		skp->Text( 43, 170, "ADC1B", 5 );

		// ADC2A
		if (0)
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 75, 170, 107, 206 );
			sprintf_s( cbuf, 16, "%04X", 0 );// Built In Test Summary
			skp->Text( 82, 181, cbuf, strlen( cbuf ) );
			if (0) skp->Text( 77, 194, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (1) skp->Text( 79, 194, "EDUMP", 5 );
			else if (0) skp->Text( 79, 194, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 16, "%06X", 0 );
				skp->Text( 78, 194, cbuf, strlen( cbuf ) );
			}
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 75, 170, 107, 206 );
		}
		skp->Line( 75, 179, 107, 179 );
		skp->Line( 75, 192, 107, 192 );
		skp->Text( 79, 170, "ADC2A", 5 );

		// ADC2B
		if (0)
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 111, 170, 143, 206 );
			sprintf_s( cbuf, 16, "%04X", 0 );// Built In Test Summary
			skp->Text( 118, 181, cbuf, strlen( cbuf ) );
			if (0) skp->Text( 113, 194, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 115, 194, "EDUMP", 5 );
			else if (0) skp->Text( 115, 194, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 16, "%06X", 0 );
				skp->Text( 114, 194, cbuf, strlen( cbuf ) );
			}
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 111, 170, 143, 206 );
		}
		skp->Line( 111, 179, 143, 179 );
		skp->Line( 111, 192, 143, 192 );
		skp->Text( 115, 170, "ADC2B", 5 );

		// CDR1
		if (0)// IDP is commanding
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 3, 48, 35, 103 );
			if (1 != 0)// FC bus
			{
				sprintf_s( cbuf, 16, "FC%d", 1 );
				skp->Text( 13, 66, cbuf, strlen( cbuf ) );
			}
			if (1) skp->Text( 9, 75, "AUTO", 4 );// MDU port reconfiguration mode
			else skp->Text( 11, 75, "MAN", 3 );
			sprintf_s( cbuf, 16, "%04X", 0 );// Built In Test Summary
			skp->Text( 10, 84, cbuf, strlen( cbuf ) );
			if (1) skp->Text( 5, 93, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 7, 93, "EDUMP", 5 );
			else if (0) skp->Text( 7, 93, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 16, "%06X", 0 );
				skp->Text( 6, 93, cbuf, strlen( cbuf ) );
			}
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 3, 48, 35, 103 );
		}
		skp->Line( 3, 57, 35, 57 );
		skp->Line( 3, 66, 35, 66 );
		skp->Line( 3, 75, 35, 75 );
		skp->Line( 3, 84, 35, 84 );
		skp->Line( 3, 93, 35, 93 );
		skp->Line( 19, 57, 19, 66 );
		skp->Text( 10, 48, "CDR1", 4 );
		skp->Text( 9, 57, "3", 1 );
		skp->Text( 25, 57, "1", 1 );

		// CDR2
		if (0)// IDP is commanding
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 39, 48, 71, 103 );
			if (2 != 0)// FC bus
			{
				sprintf_s( cbuf, 16, "FC%d", 2 );
				skp->Text( 49, 66, cbuf, strlen( cbuf ) );
			}
			if (1) skp->Text( 45, 75, "AUTO", 4 );// MDU port reconfiguration mode
			else skp->Text( 47, 75, "MAN", 3 );
			sprintf_s( cbuf, 16, "%04X", 0 );// Built In Test Summary
			skp->Text( 46, 84, cbuf, strlen( cbuf ) );
			if (1) skp->Text( 41, 93, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 43, 93, "EDUMP", 5 );
			else if (0) skp->Text( 43, 93, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 16, "%06X", 0 );
				skp->Text( 42, 93, cbuf, strlen( cbuf ) );
			}
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 39, 48, 71, 103 );
		}
		skp->Line( 39, 57, 71, 57 );
		skp->Line( 39, 66, 71, 66 );
		skp->Line( 39, 75, 71, 75 );
		skp->Line( 39, 84, 71, 84 );
		skp->Line( 39, 93, 71, 93 );
		skp->Line( 55, 57, 55, 66 );
		skp->Text( 46, 48, "CDR2", 4 );
		skp->Text( 45, 57, "1", 1 );
		skp->Text( 61, 57, "2", 1 );

		// CRT1
		if (0)// IDP is commanding
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 75, 3, 107, 58 );
			if (3 != 0)// FC bus
			{
				sprintf_s( cbuf, 16, "FC%d", 3 );
				skp->Text( 85, 21, cbuf, strlen( cbuf ) );
			}
			if (1) skp->Text( 81, 30, "AUTO", 4 );// MDU port reconfiguration mode
			else skp->Text( 83, 30, "MAN", 3 );
			sprintf_s( cbuf, 16, "%04X", 0 );// Built In Test Summary
			skp->Text( 82, 39, cbuf, strlen( cbuf ) );
			if (1) skp->Text( 77, 48, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 79, 48, "EDUMP", 5 );
			else if (0) skp->Text( 79, 48, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 16, "%06X", 0 );
				skp->Text( 78, 48, cbuf, strlen( cbuf ) );
			}
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 75, 3, 107, 58 );
		}
		skp->Line( 75, 12, 107, 12 );
		skp->Line( 75, 21, 107, 21 );
		skp->Line( 75, 30, 107, 30 );
		skp->Line( 75, 39, 107, 39 );
		skp->Line( 75, 48, 107, 48 );
		skp->Line( 91, 12, 91, 21 );
		skp->Text( 82, 3, "CRT1", 4 );
		skp->Text( 81, 12, "1", 1 );

		// CRT2
		if (0)// IDP is commanding
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 147, 3, 179, 58 );
			if (4 != 0)// FC bus
			{
				sprintf_s( cbuf, 16, "FC%d", 4 );
				skp->Text( 157, 21, cbuf, strlen( cbuf ) );
			}
			if (1) skp->Text( 153, 30, "AUTO", 4 );// MDU port reconfiguration mode
			else skp->Text( 155, 30, "MAN", 3 );
			sprintf_s( cbuf, 16, "%04X", 0 );// Built In Test Summary
			skp->Text( 154, 39, cbuf, strlen( cbuf ) );
			if (1) skp->Text( 149, 48, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 151, 48, "EDUMP", 5 );
			else if (0) skp->Text( 151, 48, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 16, "%06X", 0 );
				skp->Text( 150, 48, cbuf, strlen( cbuf ) );
			}
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 147, 3, 179, 58 );
		}
		skp->Line( 147, 12, 179, 12 );
		skp->Line( 147, 21, 179, 21 );
		skp->Line( 147, 30, 179, 30 );
		skp->Line( 147, 39, 179, 39 );
		skp->Line( 147, 48, 179, 48 );
		skp->Line( 163, 12, 163, 21 );
		skp->Text( 154, 3, "CRT2", 4 );
		skp->Text( 153, 12, "2", 1 );

		// CRT3
		if (0)// IDP is commanding
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 111, 48, 143, 103 );
			if (1 != 0)// FC bus
			{
				sprintf_s( cbuf, 16, "FC%d", 1 );
				skp->Text( 121, 66, cbuf, strlen( cbuf ) );
			}
			if (1) skp->Text( 117, 75, "AUTO", 4 );// MDU port reconfiguration mode
			else skp->Text( 119, 75, "MAN", 3 );
			sprintf_s( cbuf, 16, "%04X", 0 );// Built In Test Summary
			skp->Text( 118, 84, cbuf, strlen( cbuf ) );
			if (1) skp->Text( 113, 93, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 114, 93, "EDUMP", 5 );
			else if (0) skp->Text( 114, 93, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 16, "%06X", 0 );
				skp->Text( 114, 93, cbuf, strlen( cbuf ) );
			}
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 111, 48, 143, 103 );
		}
		skp->Line( 111, 57, 143, 57 );
		skp->Line( 111, 66, 143, 66 );
		skp->Line( 111, 75, 143, 75 );
		skp->Line( 111, 84, 143, 84 );
		skp->Line( 111, 93, 143, 93 );
		skp->Line( 127, 57, 127, 66 );
		skp->Text( 118, 48, "CRT3", 4 );
		skp->Text( 117, 57, "3", 1 );

		// MFD1
		if (0)// IDP is commanding
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 75, 75, 107, 130 );
			if (2 != 0)// FC bus
			{
				sprintf_s( cbuf, 16, "FC%d", 2 );
				skp->Text( 85, 93, cbuf, strlen( cbuf ) );
			}
			if (1) skp->Text( 81, 102, "AUTO", 4 );// MDU port reconfiguration mode
			else skp->Text( 83, 102, "MAN", 3 );
			sprintf_s( cbuf, 16, "%04X", 0 );// Built In Test Summary
			skp->Text( 82, 111, cbuf, strlen( cbuf ) );
			if (1) skp->Text( 77, 120, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 79, 120, "EDUMP", 5 );
			else if (0) skp->Text( 79, 120, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 16, "%06X", 0 );
				skp->Text( 78, 120, cbuf, strlen( cbuf ) );
			}
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 75, 75, 107, 130 );
		}
		skp->Line( 75, 84, 107, 84 );
		skp->Line( 75, 93, 107, 93 );
		skp->Line( 75, 102, 107, 102 );
		skp->Line( 75, 111, 107, 111 );
		skp->Line( 75, 120, 107, 120 );
		skp->Line( 91, 84, 91, 93 );
		skp->Text( 82, 75, "MFD1", 4 );
		skp->Text( 81, 84, "2", 1 );
		skp->Text( 97, 84, "3", 1 );

		// MFD2
		if (0)// IDP is commanding
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 147, 75, 179, 130 );
			if (3 != 0)// FC bus
			{
				sprintf_s( cbuf, 16, "FC%d", 3 );
				skp->Text( 157, 93, cbuf, strlen( cbuf ) );
			}
			if (1) skp->Text( 153, 102, "AUTO", 4 );// MDU port reconfiguration mode
			else skp->Text( 155, 102, "MAN", 3 );
			sprintf_s( cbuf, 16, "%04X", 0 );// Built In Test Summary
			skp->Text( 154, 111, cbuf, strlen( cbuf ) );
			if (1) skp->Text( 149, 120, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 151, 120, "EDUMP", 5 );
			else if (0) skp->Text( 151, 120, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 16, "%06X", 0 );
				skp->Text( 150, 120, cbuf, strlen( cbuf ) );
			}
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 147, 75, 179, 130 );
		}
		skp->Line( 147, 84, 179, 84 );
		skp->Line( 147, 93, 179, 93 );
		skp->Line( 147, 102, 179, 102 );
		skp->Line( 147, 111, 179, 111 );
		skp->Line( 147, 120, 179, 120 );
		skp->Line( 163, 84, 163, 93 );
		skp->Text( 154, 75, "MFD2", 4 );
		skp->Text( 153, 84, "1", 1 );
		skp->Text( 169, 84, "3", 1 );

		// PLT1
		if (0)// IDP is commanding
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 183, 48, 215, 103 );
			if (4 != 0)// FC bus
			{
				sprintf_s( cbuf, 16, "FC%d", 4 );
				skp->Text( 193, 66, cbuf, strlen( cbuf ) );
			}
			if (1) skp->Text( 189, 75, "AUTO", 4 );// MDU port reconfiguration mode
			else skp->Text( 191, 75, "MAN", 3 );
			sprintf_s( cbuf, 16, "%04X", 0 );// Built In Test Summary
			skp->Text( 190, 84, cbuf, strlen( cbuf ) );
			if (1) skp->Text( 185, 93, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 187, 93, "EDUMP", 5 );
			else if (0) skp->Text( 187, 93, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 16, "%06X", 0 );
				skp->Text( 186, 93, cbuf, strlen( cbuf ) );
			}
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 183, 48, 215, 103 );
		}
		skp->Line( 183, 57, 215, 57 );
		skp->Line( 183, 66, 215, 66 );
		skp->Line( 183, 75, 215, 75 );
		skp->Line( 183, 84, 215, 84 );
		skp->Line( 183, 93, 215, 93 );
		skp->Line( 199, 57, 199, 66 );
		skp->Text( 190, 48, "PLT1", 4 );
		skp->Text( 189, 57, "2", 1 );
		skp->Text( 205, 57, "1", 1 );

		// PLT2
		if (0)// IDP is commanding
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 219, 48, 251, 103 );
			if (1 != 0)// FC bus
			{
				sprintf_s( cbuf, 16, "FC%d", 1 );
				skp->Text( 229, 66, cbuf, strlen( cbuf ) );
			}
			if (1) skp->Text( 225, 75, "AUTO", 4 );// MDU port reconfiguration mode
			else skp->Text( 227, 75, "MAN", 3 );
			sprintf_s( cbuf, 16, "%04X", 0 );// Built In Test Summary
			skp->Text( 226, 84, cbuf, strlen( cbuf ) );
			if (1) skp->Text( 221, 93, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 223, 93, "EDUMP", 5 );
			else if (0) skp->Text( 223, 93, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 16, "%06X", 0 );
				skp->Text( 222, 93, cbuf, strlen( cbuf ) );
			}
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 219, 48, 251, 103 );
		}
		skp->Line( 219, 57, 251, 57 );
		skp->Line( 219, 66, 251, 66 );
		skp->Line( 219, 75, 251, 75 );
		skp->Line( 219, 84, 251, 84 );
		skp->Line( 219, 93, 251, 93 );
		skp->Line( 235, 57, 235, 66 );
		skp->Text( 226, 48, "PLT2", 4 );
		skp->Text( 225, 57, "3", 1 );
		skp->Text( 241, 57, "2", 1 );

		// CRT4
		if (0)// IDP is commanding
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 183, 170, 215, 225 );
			if (2 != 0)// FC bus
			{
				sprintf_s( cbuf, 16, "FC%d", 2 );
				skp->Text( 193, 188, cbuf, strlen( cbuf ) );
			}
			if (1) skp->Text( 189, 197, "AUTO", 4 );// MDU port reconfiguration mode
			else skp->Text( 191, 197, "MAN", 3 );
			sprintf_s( cbuf, 16, "%04X", 0 );// Built In Test Summary
			skp->Text( 190, 206, cbuf, strlen( cbuf ) );
			if (1) skp->Text( 185, 215, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 187, 215, "EDUMP", 5 );
			else if (0) skp->Text( 187, 215, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 16, "%06X", 0 );
				skp->Text( 186, 215, cbuf, strlen( cbuf ) );
			}
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 183, 170, 215, 225 );
		}
		skp->Line( 183, 179, 215, 179 );
		skp->Line( 183, 188, 215, 188 );
		skp->Line( 183, 197, 215, 197 );
		skp->Line( 183, 206, 215, 206 );
		skp->Line( 183, 215, 215, 215 );
		skp->Line( 199, 179, 199, 188 );
		skp->Text( 190, 170, "CRT4", 4 );
		skp->Text( 189, 179, "4", 1 );

		// AFD1
		if (0)// IDP is commanding
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 219, 170, 251, 225 );
			if (3 != 0)// FC bus
			{
				sprintf_s( cbuf, 16, "FC%d", 3 );
				skp->Text( 229, 188, cbuf, strlen( cbuf ) );
			}
			if (1) skp->Text( 225, 197, "AUTO", 4 );// MDU port reconfiguration mode
			else skp->Text( 227, 197, "MAN", 3 );
			sprintf_s( cbuf, 16, "%04X", 0 );// Built In Test Summary
			skp->Text( 226, 206, cbuf, strlen( cbuf ) );
			if (1) skp->Text( 221, 215, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 223, 215, "EDUMP", 5 );
			else if (0) skp->Text( 223, 215, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 16, "%06X", 0 );
				skp->Text( 222, 215, cbuf, strlen( cbuf ) );
			}
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 219, 170, 251, 225 );
		}
		skp->Line( 219, 179, 251, 179 );
		skp->Line( 219, 188, 251, 188 );
		skp->Line( 219, 197, 251, 197 );
		skp->Line( 219, 206, 251, 206 );
		skp->Line( 219, 215, 251, 215 );
		skp->Line( 235, 179, 235, 188 );
		skp->Text( 226, 170, "AFD1", 4 );
		skp->Text( 225, 179, "4", 1 );
		skp->Text( 241, 179, "2", 1 );

		// IDP1
		if (GetDrivingIDP() == 1)// commanding IDP
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 49, 131, 85, 168 );
			sprintf_s( cbuf, 16, "%04X", 0 );// Built In Test Summary
			skp->Text( 59, 140, cbuf, strlen( cbuf ) );
			if (1) skp->Text( 53, 149, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 55, 149, "EDUMP", 5 );
			else if (0) skp->Text( 55, 149, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 16, "%06X", 0 );
				skp->Text( 54, 149, cbuf, strlen( cbuf ) );
			}
			if (1)// flight-critical buses selected (1)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 50, 159, 58, 167 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 52, 158, "1", 1 );
			if (0)// flight-critical buses selected (2)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 58, 159, 66, 167 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 60, 158, "2", 1 );
			if (1)// flight-critical buses selected (3)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 66, 159, 75, 167 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 68, 158, "3", 1 );
			if (0)// flight-critical buses selected (4)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 75, 159, 84, 167 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 77, 158, "4", 1 );
			skp->SetTextColor( CR_WHITE );
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 49, 131, 85, 168 );
			skp->SetTextColor( CR_WHITE );
			skp->Text( 54, 158, "1", 1 );
			skp->Text( 60, 158, "2", 1 );
			skp->Text( 68, 158, "3", 1 );
			skp->Text( 77, 158, "4", 1 );
		}
		skp->Line( 49, 140, 85, 140 );
		skp->Line( 49, 149, 85, 149 );
		skp->Line( 49, 158, 85, 158 );
		skp->Text( 59, 131, "IDP1", 4 );

		// IDP2
		if (GetDrivingIDP() == 2)// commanding IDP
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 89, 131, 125, 168 );
			sprintf_s( cbuf, 16, "%04X", 0 );// Built In Test Summary
			skp->Text( 99, 140, cbuf, strlen( cbuf ) );
			if (0) skp->Text( 93, 149, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (1) skp->Text( 95, 149, "EDUMP", 5 );
			else if (0) skp->Text( 95, 149, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 16, "%06X", 0 );
				skp->Text( 94, 149, cbuf, strlen( cbuf ) );
			}
			if (0)// flight-critical buses selected (1)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 90, 159, 98, 167 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 92, 158, "1", 1 );
			if (1)// flight-critical buses selected (2)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 98, 159, 106, 167 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 100, 158, "2", 1 );
			if (0)// flight-critical buses selected (3)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 106, 159, 115, 167 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 108, 158, "3", 1 );
			if (1)// flight-critical buses selected (4)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 115, 159, 124, 167 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 117, 158, "4", 1 );
			skp->SetTextColor( CR_WHITE );
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 89, 131, 125, 168 );
			skp->SetTextColor( CR_WHITE );
			skp->Text( 92, 158, "1", 1 );
			skp->Text( 100, 158, "2", 1 );
			skp->Text( 108, 158, "3", 1 );
			skp->Text( 117, 158, "4", 1 );
		}
		skp->Line( 89, 140, 125, 140 );
		skp->Line( 89, 149, 125, 149 );
		skp->Line( 89, 158, 125, 158 );
		skp->Text( 99, 131, "IDP2", 4 );

		// IDP3
		if (GetDrivingIDP() == 3)// commanding IDP
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 129, 131, 165, 168 );
			sprintf_s( cbuf, 16, "%04X", 0 );// Built In Test Summary
			skp->Text( 139, 140, cbuf, strlen( cbuf ) );
			if (0) skp->Text( 133, 149, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 135, 149, "EDUMP", 5 );
			else if (1) skp->Text( 135, 149, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 16, "%06X", 0 );
				skp->Text( 134, 149, cbuf, strlen( cbuf ) );
			}
			if (1)// flight-critical buses selected (1)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 130, 159, 138, 167 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 132, 158, "1", 1 );
			if (0)// flight-critical buses selected (2)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 138, 159, 146, 167 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 140, 158, "2", 1 );
			if (1)// flight-critical buses selected (3)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 146, 159, 155, 167 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 148, 158, "3", 1 );
			if (0)// flight-critical buses selected (4)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 155, 159, 164, 167 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 157, 158, "4", 1 );
			skp->SetTextColor( CR_WHITE );
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 129, 131, 165, 168 );
			skp->SetTextColor( CR_WHITE );
			skp->Text( 132, 158, "1", 1 );
			skp->Text( 140, 158, "2", 1 );
			skp->Text( 148, 158, "3", 1 );
			skp->Text( 157, 158, "4", 1 );
		}
		skp->Line( 129, 140, 165, 140 );
		skp->Line( 129, 149, 165, 149 );
		skp->Line( 129, 158, 165, 158 );
		skp->Text( 139, 131, "IDP3", 4 );

		// IDP4
		if (GetDrivingIDP() == 4)// commanding IDP
		{
			skp->SetBrush( skpBlueBrush );
			skp->Rectangle( 169, 131, 205, 168 );
			sprintf_s( cbuf, 16, "%04X", 0 );// Built In Test Summary
			skp->Text( 179, 140, cbuf, strlen( cbuf ) );
			if (0) skp->Text( 173, 149, "NO-CST", 6 );// Dump Status or Comprehensive Self Test Results
			else if (0) skp->Text( 175, 149, "EDUMP", 5 );
			else if (0) skp->Text( 179, 149, "RDUMP", 5 );
			else
			{
				sprintf_s( cbuf, 16, "%06X", 0 );
				skp->Text( 174, 149, cbuf, strlen( cbuf ) );
			}
			if (0)// flight-critical buses selected (1)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 170, 159, 178, 167 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 172, 158, "1", 1 );
			if (1)// flight-critical buses selected (2)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 178, 159, 186, 167 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 180, 158, "2", 1 );
			if (0)// flight-critical buses selected (3)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 186, 159, 195, 167 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 188, 158, "3", 1 );
			if (1)// flight-critical buses selected (4)
			{
				skp->SetBrush( skpWhiteBrush );
				skp->Rectangle( 195, 159, 204, 167 );
				skp->SetTextColor( CR_BLACK );
			}
			else skp->SetTextColor( CR_WHITE );
			skp->Text( 197, 158, "4", 1 );
			skp->SetTextColor( CR_WHITE );
		}
		else
		{
			skp->SetBrush( skpBlackBrush );
			skp->Rectangle( 169, 131, 205, 168 );
			skp->SetTextColor( CR_WHITE );
			skp->Text( 172, 158, "1", 1 );
			skp->Text( 180, 158, "2", 1 );
			skp->Text( 188, 158, "3", 1 );
			skp->Text( 197, 158, "4", 1 );
		}
		skp->Line( 169, 140, 205, 140 );
		skp->Line( 169, 149, 205, 149 );
		skp->Line( 169, 158, 205, 158 );
		skp->Text( 179, 131, "IDP4", 4 );

		// additional output
		skp->Text( 22, 225, "MAJOR FUNC:", 11 );// major function
		if (GetIDP()->GetMajfunc() == dps::GNC) skp->Text( 72, 225, "GNC", 3 );
		else if (GetIDP()->GetMajfunc() == dps::PL) skp->Text( 72, 225, "PL", 2 );
		else skp->Text( 72, 225, "SM", 2 );

		skp->Text( 22, 235, "IDP LOAD    :", 13 );
		if (0) skp->Text( 72, 235, "LOAD", 4 );// IDP loading

		skp->Text( 97, 225, "LEFT IDP SEL:", 13 );
		if (1) skp->Text( 147, 225, "1", 1 );// switch position
		else skp->Text( 147, 225, "3", 1 );
		
		skp->Text( 97, 235, "RIGHT IDP SEL:", 14 );
		if (0) skp->Text( 153, 235, "2", 1 );// switch position
		else skp->Text( 153, 235, "3", 1 );

		skp->Text( 167, 225, "KYBD SEL A:", 11 );
		if (1) skp->Text( 214, 225, "ON", 2 );// keyboard port? related to switch position above?
		else skp->Text( 214, 225, "OFF", 3 );

		skp->Text( 167, 235, "KYBD SEL B:", 11 );
		if (0) skp->Text( 214, 235, "ON", 2 );// keyboard port? related to switch position above?
		else skp->Text( 214, 235, "OFF", 3 );

		skp->Text( 51, 245, "ACTIVE KYBD:", 12 );
		sprintf_s( cbuf, 16, "%d", 2 );// ????
		skp->Text( 105, 245, cbuf, strlen( cbuf ) );

		skp->Text( 129, 245, "KEYSTROKE:", 10 );
		sprintf_s( cbuf, 16, "%s", "SYS SUMM" );// latest keyboard input
		skp->Text( 180, 245, cbuf, strlen( cbuf ) );

		return;
	}
};
