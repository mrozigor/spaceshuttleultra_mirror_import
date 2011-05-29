#include "CrawlerLeftPanel.h"
#include "Crawler.h"
#include "meshres_crawler_vc.h"

extern GlobalHandles g_Resources; 

namespace vc
{

CrawlerLeftPanel::CrawlerLeftPanel(Crawler *_v, const std::string &_ident, vc::CRAWLER_CAB _cab)
: CrawlerPanel(_v, _ident, _cab), pSpeedKnobAnimation(NULL)
{
	Add(pParkingBrakePBI = new CrawlerPBI(_v, "BRAKE"));
	Add(pFwdPBI = new CrawlerPBI(_v, "FWD"));
	Add(pRevPBI = new CrawlerPBI(_v, "REV"));
	Add(pNeutPBI = new CrawlerPBI(_v, "NEUT"));
	Add(pParkingBrakeLight = new CrawlerPanelLight(_v, "BrakeLight"));
	Add(pCommandVoltage = new CrawlerDigitalDisplay(_v, "CommandVoltage"));
}

CrawlerLeftPanel::~CrawlerLeftPanel()
{
	delete pSpeedKnobAnimation;
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
		TransformVector(_V(0.566, 0.187, 0.214))+ofs,
		TransformVector(_V(0.310, 0.187, -0.236))+ofs, 
		TransformVector(_V(0.525, 0.105, 0.220))+ofs,
		TransformVector(_V(0.275, 0.075, -0.214))+ofs);

	SURFHANDLE panels_tex = oapiGetTextureHandle(V()->GetVCMesh(cabID), 1);
	oapiVCRegisterArea(AID_BRAKE_PBI+aid_ofs, _R(658, 982, 718, 1012), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panels_tex);
	oapiVCRegisterArea(AID_BRAKE_LIGHT+aid_ofs, _R(805, 720, 892, 787), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panels_tex);
	oapiVCRegisterArea(AID_FWD_PBI+aid_ofs, _R(727, 982, 787, 1012), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panels_tex);
	oapiVCRegisterArea(AID_NEUT_PBI+aid_ofs, _R(795, 982, 855, 1012), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panels_tex);
	oapiVCRegisterArea(AID_REV_PBI+aid_ofs, _R(866, 982, 926, 1012), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panels_tex);
	
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
	pParkingBrakePBI->SetMouseRegion(0.916822f, 0.518648f, 0.972010f, 0.795954f);

	DefineEnginePBI(pFwdPBI);
	pFwdPBI->AddAIDToRedrawEventList(AID_FWD_PBI+aid_ofs);
	pFwdPBI->SetMouseRegion(0.144804f, 0.096366f, 0.199067f, 0.360672f);

	DefineEnginePBI(pRevPBI);
	pRevPBI->AddAIDToRedrawEventList(AID_REV_PBI+aid_ofs);
	pRevPBI->SetMouseRegion(0.246844f, 0.107135f, 0.299727f, 0.377244f);

	DefineEnginePBI(pNeutPBI);
	pNeutPBI->AddAIDToRedrawEventList(AID_NEUT_PBI+aid_ofs);
	pNeutPBI->SetMouseRegion(0.195921f, 0.100198f, 0.249119f, 0.363231f);

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

void CrawlerLeftPanel::DefineVCAnimations(UINT vcidx)
{	
	static UINT SpeedControlKnob[1] = {GRP_SPEED_CONTROL_KNOB_VC};
	pSpeedKnobAnimation = new MGROUP_ROTATE(vcidx, SpeedControlKnob, 1, TransformVector(_V(0.412, 0.077, 0.061)),
		TransformVector(_V(-0.78112, 0.44150, 0.44150)), static_cast<float>((270.0*RAD)));
	anim_SpeedKnob = V()->CreateAnimation(0.0);
	V()->AddAnimationComponent(anim_SpeedKnob, 0.0, 1.0, pSpeedKnobAnimation);
}
void CrawlerLeftPanel::Realize()
{
	DiscreteBundle* pBundle = V()->BundleManager()->CreateBundle("CRAWLER_BRAKE", 2);
	pParkingBrakePBI->ConnectPort(pBundle, 1);
	pParkingBrakeLight->ConnectPort(pBundle, 1);
	
	pBundle = V()->BundleManager()->CreateBundle("CRAWLER_ENGINE", 16);
	pCommandVoltage->ConnectPort(pBundle, 0);
	pNeutPBI->ConnectPort(pBundle, 1);
	pNeutPBI->ConnectGroupPort(pBundle, 2);
	pNeutPBI->ConnectGroupPort(pBundle, 3);
	if(cabID==FWD) {
		pFwdPBI->ConnectPort(pBundle, 2);
		pFwdPBI->ConnectGroupPort(pBundle, 1);
		pFwdPBI->ConnectGroupPort(pBundle, 3);

		pRevPBI->ConnectPort(pBundle, 3);
		pRevPBI->ConnectGroupPort(pBundle, 1);
		pRevPBI->ConnectGroupPort(pBundle, 2);
	}
	else {
		pFwdPBI->ConnectPort(pBundle, 3);
		pFwdPBI->ConnectGroupPort(pBundle, 1);
		pFwdPBI->ConnectGroupPort(pBundle, 2);

		pRevPBI->ConnectPort(pBundle, 2);
		pRevPBI->ConnectGroupPort(pBundle, 1);
		pRevPBI->ConnectGroupPort(pBundle, 3);
	}

	pBundle = V()->BundleManager()->CreateBundle("CRAWLER_SPEED", 16);
	port_TargetSpeed.Connect(pBundle, 1);

	CrawlerPanel::Realize();
}
void CrawlerLeftPanel::OnPreStep(double SimT, double SimDT, double MJD)
{
	CrawlerPanel::OnPreStep(SimT, SimDT, MJD);	

	double speedKnob = port_TargetSpeed.GetVoltage()/MAX_UNLOADED_SPEED;
	V()->SetAnimation(anim_SpeedKnob, speedKnob);
}

void CrawlerLeftPanel::DefineEnginePBI(CrawlerPBI* pPBI)
{
	pPBI->SetBaseCoords(0, 0);
	pPBI->SetDimensions(60, 30);
	pPBI->SetSourceImage(g_Resources.pbi_lights);
	pPBI->SetSourceCoords(false, 0, PBI_BLACK);
	pPBI->SetSourceCoords(true, 0, PBI_GO);
}

};