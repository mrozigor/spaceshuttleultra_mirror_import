#include "PanelR11.h"
#include "../Atlantis.h"
#include "../meshres_vc.h"
#include "../Atlantis_defs.h"

namespace vc
{
	PanelR11::PanelR11(Atlantis *_sts)
		: AtlantisPanel(_sts, "R11")
	{
		Add(pCRT4 = new MDU(_sts, "CRT4", MDUID_CRT4, true));

		Add( pKeyboardAft = new Keyboard( _sts, "Aft Keyboard", 3 ) );

		Add( pIDPCRTPower = new StdSwitch2( _sts, "IDP/CRT 4 Power" ) );
		pIDPCRTPower->SetLabel( 0, "OFF" );
		pIDPCRTPower->SetLabel( 1, "ON" );
		pIDPCRTPower->SetInitialPosition( 1 );

		Add( pIDPCRTMajFunc = new StdSwitch3( _sts, "IDP/CRT 4 Maj Func" ) );
		pIDPCRTMajFunc->SetLabel( 0, "PL" );
		pIDPCRTMajFunc->SetLabel( 1, "SM" );
		pIDPCRTMajFunc->SetLabel( 2, "GNC" );
		pIDPCRTMajFunc->SetInitialPosition( 2 );
	}

	PanelR11::~PanelR11()
	{
	}

	void PanelR11::DefineVC()
	{
		VECTOR3 switch_rot = _V( 0, 0, -1 );

		AddAIDToMouseEventList(AID_R11);

		pCRT4->DefineVCGroup(MFDGROUPS[MDUID_CRT4]);
		pCRT4->SetMouseRegion(0.036582f, 0.020135f, 0.521159f, 0.402556f);

		pKeyboardAft->SetMouseRegion( 0.092359f, 0.429578f, 0.275403f, 0.780348f );

		pIDPCRTPower->SetInitialAnimState( 0.5f );
		pIDPCRTPower->DefineSwitchGroup( GRP_R11LS1_VC );
		pIDPCRTPower->SetReference( _V( 1.2491, 2.2434, 13.4210 ), switch_rot );
		pIDPCRTPower->SetMouseRegion( 0.350223f, 0.567250f, 0.385167f, 0.609089f );


		pIDPCRTMajFunc->SetInitialAnimState( 0.5f );
		pIDPCRTMajFunc->DefineSwitchGroup( GRP_R11LS2_VC );
		pIDPCRTMajFunc->SetReference( _V( 1.1616, 2.1864, 13.4197 ), switch_rot );
		pIDPCRTMajFunc->SetMouseRegion( 0.352497f, 0.743870f, 0.387197f, 0.785950f );
	}

	void PanelR11::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs=STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea(AID_R11, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		oapiVCSetAreaClickmode_Quadrilateral(AID_R11,
			_V(1.539, 2.431, 13.608)+ofs, _V(1.539, 2.433, 13.086)+ofs,
			_V(1.047, 2.109, 13.608)+ofs, _V(1.047, 2.110, 13.086)+ofs);
	}

	void PanelR11::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "C2_R11_IDP", 14 );
		pIDPCRTPower->output.Connect( pBundle, 3 );

		pIDPCRTMajFunc->ConnectPort( 1, pBundle, 7 );

		pIDPCRTMajFunc->ConnectPort( 2, pBundle, 11 );


		pKeyboardAft->ConnectIDP( 0, STS()->GetIDP( 4 ) );

		AtlantisPanel::Realize();
		return;
	}
};
