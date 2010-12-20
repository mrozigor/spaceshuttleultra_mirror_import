#include "CrawlerLeftPanel.h"
#include "Crawler.h"

extern GlobalHandles g_Resources; 

namespace vc
{

CrawlerLeftPanel::CrawlerLeftPanel(Crawler *_v, const std::string &_ident, vc::CRAWLER_CAB _cab)
: CrawlerPanel(_v, _ident, _cab)
{
	Add(pParkingBrakePBI = new CrawlerPBI(_v, "BRAKE"));
	Add(pParkingBrakeLight = new CrawlerPanelLight(_v, "BrakeLight"));
	Add(pCommandVoltage = new CrawlerDigitalDisplay(_v, "CommandVoltage"));
}

CrawlerLeftPanel::~CrawlerLeftPanel()
{
}

void CrawlerLeftPanel::RegisterVC()
{
	BasicPanel::RegisterVC();

	VECTOR3 ofs;
	int aid_ofs;
	if(cabID==FWD) {
		ofs = CRAWLER_FWD_VC_OFFSET;
		aid_ofs = 0;
	}
	else {
		ofs = CRAWLER_REAR_VC_OFFSET;
		aid_ofs = AID_REAR_OFFSET;
	}

	oapiVCRegisterArea(AID_LEFT_MIN+aid_ofs, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN);
	oapiVCSetAreaClickmode_Quadrilateral(AID_LEFT_MIN+aid_ofs,
		TransformVector(_V(0.579, 0.119, 0.173))+ofs,
		TransformVector(_V(0.284, 0.119, -0.174))+ofs, 
		TransformVector(_V(0.579, 0.075, 0.173))+ofs,
		TransformVector(_V(0.284, 0.075, -0.174))+ofs);

	SURFHANDLE panels_tex = oapiGetTextureHandle(V()->GetVCMesh(cabID), 1);
	oapiVCRegisterArea(AID_BRAKE_PBI+aid_ofs, _R(658, 982, 718, 1012), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panels_tex);
	oapiVCRegisterArea(AID_BRAKE_LIGHT+aid_ofs, _R(805, 720, 892, 787), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panels_tex);
	
	SURFHANDLE gauges_tex = oapiGetTextureHandle(V()->GetVCMesh(cabID), 2);
	oapiVCRegisterArea(AID_COMMAND_VOLTAGE+aid_ofs, _R(199, 139, 331, 169), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, gauges_tex);
}

void CrawlerLeftPanel::DefineVC()
{
	int aid_ofs;
	if(cabID==FWD) aid_ofs = 0;
	else aid_ofs = AID_REAR_OFFSET;

	AddAIDToMouseEventList(AID_LEFT_MIN+aid_ofs);

	pParkingBrakePBI->SetBaseCoords(0, 0);
	pParkingBrakePBI->SetDimensions(60, 30);
	pParkingBrakePBI->SetSourceImage(g_Resources.pbi_lights);
	pParkingBrakePBI->SetSourceCoords(false, 0, PBI_OFF_RED);
	pParkingBrakePBI->SetSourceCoords(true, 0, PBI_ON_RED);
	pParkingBrakePBI->AddAIDToRedrawEventList(AID_BRAKE_PBI+aid_ofs);
	pParkingBrakePBI->AllowReset(true);
	pParkingBrakePBI->SetMouseRegion(0.956645f, 0.565480f, 1.0f, 1.0f);

	pParkingBrakeLight->SetBaseCoords(0, 0);
	pParkingBrakeLight->SetDimensions(LIGHT_WIDTH, LIGHT_HEIGHT);
	pParkingBrakeLight->SetSourceImage(g_Resources.lights);
	pParkingBrakeLight->SetSourceCoords(false, LIGHT_OFF_X, LIGHT_BRAKES_ON_Y);
	pParkingBrakeLight->SetSourceCoords(true, LIGHT_ON_X, LIGHT_BRAKES_ON_Y);
	pParkingBrakeLight->AddAIDToRedrawEventList(AID_BRAKE_LIGHT+aid_ofs);

	pCommandVoltage->SetScaleValue(100.0f);
	pCommandVoltage->SetBaseCoords(0, 0);
	pCommandVoltage->SetDimensions(132, 30, 5);
	pCommandVoltage->AddAIDToRedrawEventList(AID_COMMAND_VOLTAGE+aid_ofs);
}

void CrawlerLeftPanel::Realize()
{
	DiscreteBundle* pBundle = V()->BundleManager()->CreateBundle("CRAWLER_BRAKE", 2);
	pParkingBrakePBI->ConnectPort(pBundle, 1);
	pParkingBrakeLight->ConnectPort(pBundle, 1);
	
	pBundle = V()->BundleManager()->CreateBundle("CRAWLER_ENGINE", 16);
	pCommandVoltage->ConnectPort(pBundle, 0);

	CrawlerPanel::Realize();
}

};