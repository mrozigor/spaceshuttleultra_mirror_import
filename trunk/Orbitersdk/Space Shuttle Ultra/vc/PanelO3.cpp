#include "PanelO3.h"
#include "../Atlantis.h"
#include "../Atlantis_defs.h"
#include "../meshres_vc_additions.h"
#include "../dps/MasterTimingUnit.h"


extern GDIParams g_Param;

namespace vc
{
	PanelO3::PanelO3( Atlantis *_sts ):AtlantisPanel( _sts, "O3" )
	{
		Add( pRCSOMS_PRESS = new RotaryDemuxSwitch( _sts, "RCS/OMS/PRESS", 3 ) );
		pRCSOMS_PRESS->SetLabel( 0, "RCS He X10" );
		pRCSOMS_PRESS->SetLabel( 1, "RCS PRPLT" );
		pRCSOMS_PRESS->SetLabel( 2, "OMS PRPLT" );

		Add( pRCSOMS_PRPLTQTY = new RotaryDemuxSwitch( _sts, "RCS/OMS PRPLT QTY", 5 ) );
		pRCSOMS_PRPLTQTY->SetLabel( 0, "RCS OXID" );
		pRCSOMS_PRPLTQTY->SetLabel( 1, "RCS FUEL" );
		pRCSOMS_PRPLTQTY->SetLabel( 2, "LOWEST" );
		pRCSOMS_PRPLTQTY->SetLabel( 3, "OMS OXID" );
		pRCSOMS_PRPLTQTY->SetLabel( 4, "OMS FUEL" );

		Add( pMissionTimer = new StdSwitch3( _sts, "Mission Timer" ) );
		pMissionTimer->SetLabel( 0, "TEST" );
		pMissionTimer->SetLabel( 1, "MET" );
		pMissionTimer->SetLabel( 2, "GMT" );
		
		sMETSeconds = 0;
		sMETMinutes = 0;
		sMETHours = 0;
		sMETDays = 0;

		for (int i = 0; i < 3; i++) sOMSFuel[i] = 99;
	}

	PanelO3::~PanelO3()
	{
	}

	void PanelO3::DefineVC()
	{
		VECTOR3 switch_rot = _V( 1, 0, 0 );
		VECTOR3 rotary_switch_rot = _V( 0, 0.1984, 0.9801 );

		AddAIDToMouseEventList( AID_O3 );

		pRCSOMS_PRESS->DefineSwitchGroup( GRP_O3S10_VC );
		pRCSOMS_PRESS->DefineRotationAngle( 64.0f );
		pRCSOMS_PRESS->SetOffset( -148.0f );
		pRCSOMS_PRESS->SetInitialAnimState( 1.0f );
		pRCSOMS_PRESS->SetReference( _V( 0.2147, 2.7921, 14.3509 ), rotary_switch_rot );
		pRCSOMS_PRESS->SetMouseRegion( 0.142551f, 0.704583f, 0.202544f, 0.829816f );

		pRCSOMS_PRPLTQTY->DefineSwitchGroup( GRP_O3S11_VC );
		pRCSOMS_PRPLTQTY->DefineRotationAngle( 130.0f );
		pRCSOMS_PRPLTQTY->SetOffset( -115.0f );
		pRCSOMS_PRPLTQTY->SetInitialAnimState( 0.75f );
		pRCSOMS_PRPLTQTY->SetReference( _V( 0.3653, 2.7950, 14.3503 ), rotary_switch_rot );
		pRCSOMS_PRPLTQTY->SetMouseRegion( 0.398181f, 0.692892f, 0.454926f, 0.814155f );

		pMissionTimer->DefineSwitchGroup( GRP_O3S12_VC );
		pMissionTimer->SetInitialAnimState( 0.5f );
		pMissionTimer->SetReference( _V( 0.5121638,  2.799416,  14.35993 ), switch_rot );
		pMissionTimer->SetMouseRegion( 0.655579f, 0.695221f, 0.687731f, 0.794568f );
		pMissionTimer->SetSpringLoaded( true, 0 );
		return;
	}

	void PanelO3::RegisterVC()
	{
		AtlantisPanel::RegisterVC();
		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_O3, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP );
		oapiVCSetAreaClickmode_Quadrilateral( AID_O3, 
			_V( 0.110078, 3.0096, 14.3151 ) + ofs, _V( 0.713058, 3.0096, 14.3151 ) + ofs, 
			_V( 0.110078, 2.72832, 14.3729 ) + ofs , _V( 0.713058, 2.72832, 14.3729 ) + ofs );

		SURFHANDLE digit_tex = oapiGetTextureHandle( STS()->hOrbiterVCMesh, TEX_DIGITS_VC );
		oapiVCRegisterArea( AID_O3_METTMR1, _R( 320, 0, 512, 64 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digit_tex );	
		oapiVCRegisterArea( AID_O3_METTMR2, _R( 0, 64, 384, 128 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digit_tex );	
		oapiVCRegisterArea( AID_O3_RCS, _R( 0, 256, 384, 320 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digit_tex );
		return;
	}

	void PanelO3::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "O3_Timer_OMSRCS", 10 );
		pMissionTimer->ConnectPort( 1, pBundle, 0 );
		pMissionTimer->ConnectPort( 2, pBundle, 1 );
		pMissionTimerGMT.Connect( pBundle, 1 );
		pMissionTimerTEST.Connect( pBundle, 0 );

		pRCSOMS_PRESS->ConnectOutputSignal( 0, pBundle, 2 );
		pRCSOMS_PRESS->ConnectOutputSignal( 1, pBundle, 3 );
		pRCSOMS_PRESS->ConnectOutputSignal( 2, pBundle, 4 );
		pRCSOMS_PRESS_RCSHeX10.Connect( pBundle, 2 );
		pRCSOMS_PRESS_RCSPRPLT.Connect( pBundle, 3 );
		pRCSOMS_PRESS_OMSPRPLT.Connect( pBundle, 4 );

		pRCSOMS_PRPLTQTY->ConnectOutputSignal( 0, pBundle, 5 );
		pRCSOMS_PRPLTQTY->ConnectOutputSignal( 1, pBundle, 6 );
		pRCSOMS_PRPLTQTY->ConnectOutputSignal( 2, pBundle, 7 );
		pRCSOMS_PRPLTQTY->ConnectOutputSignal( 3, pBundle, 8 );
		pRCSOMS_PRPLTQTY->ConnectOutputSignal( 4, pBundle, 9 );
		pRCSOMS_PRPLTQTY_RCSOXID.Connect( pBundle, 5 );
		pRCSOMS_PRPLTQTY_RCSFUEL.Connect( pBundle, 6 );
		pRCSOMS_PRPLTQTY_LOWEST.Connect( pBundle, 7 );
		pRCSOMS_PRPLTQTY_OMSOXID.Connect( pBundle, 8 );
		pRCSOMS_PRPLTQTY_OMSFUEL.Connect( pBundle, 9 );

		AtlantisPanel::Realize();
		return;
	}

	bool PanelO3::OnVCRedrawEvent( int id, int _event, SURFHANDLE surf )
	{
		const int NUMX[10] = {64, 0, 64, 128, 192, 256, 320, 384, 448, 0};
		const int NUMY[10] = {384, 448, 448, 448, 448, 448, 448, 448, 448, 384};
		int i;
		int digit[8];

		if(id == AID_O3_METTMR1)
		{
			digit[0] = sMETDays /100;
			digit[1] = (sMETDays / 10) % 10;
			digit[2] = sMETDays %10;
		

			for(i = 0; i<3; i++)
			{
				oapiBlt(surf, g_Param.digits_7seg, i*64, 0, NUMX[digit[i]], NUMY[digit[i]], 64, 64);
			}
			return true;
		}
		else if(id == AID_O3_METTMR2)
		{
			digit[0] = sMETHours / 10;
			digit[1] = sMETHours % 10;
			digit[2] = sMETMinutes / 10;
			digit[3] = sMETMinutes % 10;
			digit[4] = sMETSeconds / 10;
			digit[5] = sMETSeconds % 10;

			for(i = 0; i<6; i++)
			{
				oapiBlt(surf, g_Param.digits_7seg, i*64, 0, NUMX[digit[i]], NUMY[digit[i]], 64, 64);
			}
			return true;
		
		}
		else if(id == AID_O3_RCS)
		{
			digit[0] = sOMSFuel[0] / 10;
			digit[1] = sOMSFuel[0] % 10;
			digit[2] = sOMSFuel[1] / 10;
			digit[3] = sOMSFuel[1] % 10;
			digit[4] = sOMSFuel[2] / 10;
			digit[5] = sOMSFuel[2] % 10;

			for(i = 0; i<6; i++)
			{
				oapiBlt(surf, g_Param.digits_7seg, i*64, 0, NUMX[digit[i]], NUMY[digit[i]], 64, 64);
			}
			return true;
		}
		return AtlantisPanel::OnVCRedrawEvent(id, _event, surf);
	}

	void PanelO3::OnPreStep( double SimT, double DeltaT, double MJD )
	{
		AtlantisPanel::OnPreStep( SimT, DeltaT, MJD );

		bool update = false;
		short fuel;
		//Check forward event timer for changes and update clock if needed

		if (pMissionTimerGMT.IsSet())
		{
			if(STS()->pMTU->GetGMTDay(0) != sMETDays || 
				STS()->pMTU->GetGMTHour(0) != sMETHours ||
				STS()->pMTU->GetGMTSec(0) != sMETSeconds ||
				STS()->pMTU->GetGMTMin(0) != sMETMinutes) 
			{
				if(STS()->pMTU->GetGMTDay(0) != sMETDays) 
				{
					sMETDays = STS()->pMTU->GetGMTDay(0);
					oapiVCTriggerRedrawArea(-1, AID_O3_METTMR1);
				}
				//sMETDays = STS()->pMTU->GetGMTDay(0);
				sMETHours = STS()->pMTU->GetGMTHour(0);
				sMETMinutes = STS()->pMTU->GetGMTMin(0);
				sMETSeconds = STS()->pMTU->GetGMTSec(0);
				oapiVCTriggerRedrawArea(-1, AID_O3_METTMR2);
			}
		}
		else if (pMissionTimerTEST.IsSet())
		{
			sMETDays = 888;
			sMETHours = 88;
			sMETMinutes = 88;
			sMETSeconds = 88;
			oapiVCTriggerRedrawArea(-1, AID_O3_METTMR1);
			oapiVCTriggerRedrawArea(-1, AID_O3_METTMR2);
		}
		else
		{
			if(STS()->pMTU->GetMETDay(0) != sMETDays || 
				STS()->pMTU->GetMETHour(0) != sMETHours ||
				STS()->pMTU->GetMETSec(0) != sMETSeconds ||
				STS()->pMTU->GetMETMin(0) != sMETMinutes) 
			{
				if(STS()->pMTU->GetMETDay(0) != sMETDays) 
				{
					sMETDays = STS()->pMTU->GetMETDay(0);
					oapiVCTriggerRedrawArea(-1, AID_O3_METTMR1);
				}
				//sMETDays = STS()->pMTU->GetMETDay(0);
				sMETHours = STS()->pMTU->GetMETHour(0);
				sMETMinutes = STS()->pMTU->GetMETMin(0);
				sMETSeconds = STS()->pMTU->GetMETSec(0);
			
				oapiVCTriggerRedrawArea(-1, AID_O3_METTMR2);
			}
		}

		if (pRCSOMS_PRPLTQTY_RCSOXID.IsSet())
		{
			bool UpdateRCSgauge=false;
			fuel=min(99, static_cast<short>(STS()->GetPropellantLevel(STS()->ph_lrcs)));
			if(sOMSFuel[0]!=fuel) {
				sOMSFuel[0]=fuel;
				UpdateRCSgauge=true;
			}
			fuel=min(99, static_cast<short>(STS()->GetPropellantLevel(STS()->ph_frcs)));
			if(sOMSFuel[1]!=fuel) {
				sOMSFuel[1]=fuel;
				UpdateRCSgauge=true;
			}
			fuel=min(99, static_cast<short>(STS()->GetPropellantLevel(STS()->ph_rrcs)));
			if(sOMSFuel[2]!=fuel) {
				sOMSFuel[2]=fuel;
				UpdateRCSgauge=true;
			}
			if(UpdateRCSgauge) oapiVCTriggerRedrawArea(-1, AID_O3_RCS);
		}
		else if (pRCSOMS_PRPLTQTY_RCSFUEL.IsSet())
		{
			bool UpdateRCSgauge=false;
			fuel=min(99, static_cast<short>(STS()->GetPropellantLevel(STS()->ph_lrcs)));
			if(sOMSFuel[0]!=fuel) {
				sOMSFuel[0]=fuel;
				UpdateRCSgauge=true;
			}
			fuel=min(99, static_cast<short>(STS()->GetPropellantLevel(STS()->ph_frcs)));
			if(sOMSFuel[1]!=fuel) {
				sOMSFuel[1]=fuel;
				UpdateRCSgauge=true;
			}
			fuel=min(99, static_cast<short>(STS()->GetPropellantLevel(STS()->ph_rrcs)));
			if(sOMSFuel[2]!=fuel) {
				sOMSFuel[2]=fuel;
				UpdateRCSgauge=true;
			}
			if(UpdateRCSgauge) oapiVCTriggerRedrawArea(-1, AID_O3_RCS);
		}
		else if (pRCSOMS_PRPLTQTY_LOWEST.IsSet())
		{
			// TODO display lowest RCS fuel or oxid
			bool UpdateRCSgauge=false;
			fuel=min(99, static_cast<short>(STS()->GetPropellantLevel(STS()->ph_lrcs)));
			if(sOMSFuel[0]!=fuel) {
				sOMSFuel[0]=fuel;
				UpdateRCSgauge=true;
			}
			fuel=min(99, static_cast<short>(STS()->GetPropellantLevel(STS()->ph_frcs)));
			if(sOMSFuel[1]!=fuel) {
				sOMSFuel[1]=fuel;
				UpdateRCSgauge=true;
			}
			fuel=min(99, static_cast<short>(STS()->GetPropellantLevel(STS()->ph_rrcs)));
			if(sOMSFuel[2]!=fuel) {
				sOMSFuel[2]=fuel;
				UpdateRCSgauge=true;
			}
			if(UpdateRCSgauge) oapiVCTriggerRedrawArea(-1, AID_O3_RCS);
		}
		else if (pRCSOMS_PRPLTQTY_OMSOXID.IsSet())
		{
			fuel=min(99, static_cast<short>(STS()->GetPropellantLevel(STS()->ph_oms)));
			if(sOMSFuel[0]!=fuel || sOMSFuel[1]!=0 || sOMSFuel[2]!=fuel) {
				sOMSFuel[0]=fuel;
				sOMSFuel[1]=0;
				sOMSFuel[2]=fuel;
				oapiVCTriggerRedrawArea(-1, AID_O3_RCS);
			}
		}
		else if (pRCSOMS_PRPLTQTY_OMSFUEL.IsSet())
		{
			fuel=min(99, static_cast<short>(STS()->GetPropellantLevel(STS()->ph_oms)));
			if(sOMSFuel[0]!=fuel || sOMSFuel[1]!=0 || sOMSFuel[2]!=fuel) {
				sOMSFuel[0]=fuel;
				sOMSFuel[1]=0;
				sOMSFuel[2]=fuel;
				oapiVCTriggerRedrawArea(-1, AID_O3_RCS);
			}
		}
		return;
	}
}
