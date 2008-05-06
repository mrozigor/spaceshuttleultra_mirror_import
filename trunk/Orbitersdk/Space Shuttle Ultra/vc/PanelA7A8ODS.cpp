#include "PanelA7A8ODS.h"
#include "../Atlantis.h"

namespace vc {

	PanelA7A8ODS::PanelA7A8ODS(Atlantis* _sts)
		: BasicPanel(_sts, "A7A3/A8A3"),
		SystemPowerMNA(_sts, "SYSTEM POWER MNA"),
		SystemPowerMNB(_sts, "SYSTEM POWER MNB"),
		PyroPowerMNA(_sts, "PYRO POWER MNA"),
		PyroPowerMNC(_sts, "PYRO POWER MNC"),
		Sys1VentIsol(_sts, "SYS1 VENT ISOL"),
		Sys1Vent(_sts, "SYS1 VENT"),
		Sys2VentIsol(_sts, "SYS2 VENT ISOL"),
		Sys2Vent(_sts, "SYS2 VENT"),
		PSUPowerMNA(_sts, "PSU POWER MNA"),
		PSUPowerMNB(_sts, "PSU POWER MNB"),
		LightsAirlock14(_sts, "LIGHTS AIRLOCK 1-4"),
		LightsAirlock23(_sts, "LIGHTS AIRLOCK 2-3"),
		LightsDockingFWD(_sts, "LIGHTS DOCKING TRUSS FWD"),
		LightsDockingAFT(_sts, "LIGHTS DOCKING TRUSS AFT"),
		ARLKTNLFanA(_sts, "ARLK/TNL FAN A"),
		ARLKTNLFanB(_sts, "ARLK/TNL FAN B"),
		LightsCLVestPORT(_sts, "LIGHTS C/L VESTIBULE PORT"),
		LightsCLVestSTBD(_sts, "LIGHTS C/L VESTIBULE STBD")
	{
	}

	PanelA7A8ODS::~PanelA7A8ODS()
	{
	}

	void PanelA7A8ODS::AddMeshes(const VECTOR3& ofs)
	{
		SetHasOwnVCMesh();
		midx_odspanel = STS()->AddMesh(ODSPANEL_MESHNAME);
		STS()->SetMeshVisibilityMode(midx_odspanel, MESHVIS_VC);
	}

	void PanelA7A8ODS::Realize()
	{	
		//Define switch connections
		BasicPanel::Realize();
	}

	void PanelA7A8ODS::DefineVC()
	{
		//Define VC regions
		AddAIDToMouseEventList(AID_A7A3);
		AddAIDToMouseEventList(AID_A8A3);
		//first is A7A3, second region A8A3. 

		//Panel A8A3
		//-0.265916, 2.5048, 12.399
		//0.254716, 2.12746, 12.5175
		//register mouse event regions in BasicPanel

	}

	void PanelA7A8ODS::DefineVCAnimations(UINT vc_idx)
	{
		BasicPanel::DefineVCAnimations(midx_odspanel);
	}
		
};