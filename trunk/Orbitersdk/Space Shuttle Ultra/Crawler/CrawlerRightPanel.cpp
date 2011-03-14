#include "CrawlerRightPanel.h"
#include "Crawler.h"

extern GlobalHandles g_Resources; 

namespace vc
{

CrawlerRightPanel::CrawlerRightPanel(Crawler* _v, const std::string& _ident, CRAWLER_CAB _cab)
: CrawlerPanel(_v, _ident, _cab)
{
	Add(pIndependent = new CrawlerPBI(_v, "Independent"));
	Add(pCrab = new CrawlerPBI(_v, "Crab"));
	Add(pGreatCircle = new CrawlerPBI(_v, "GreatCircle"));
	Add(pMode = new CrawlerPBI(_v, "Mode"));

	Add(pDesSteeringAngleValue = new CrawlerDigitalDisplay(_v, "DesiredSteeringAngleValue"));
	Add(pDesSteeringAngleGauge = new CrawlerBarGauge(_v, "DesiredSteeringAngleGauge", false));
	Add(pLeftSteeringAngleValue = new CrawlerDigitalDisplay(_v, "LeftSteeringAngleValue"));
	Add(pLeftSteeringAngleGauge = new CrawlerBarGauge(_v, "LeftSteeringAngleGauge", false));
	Add(pRightSteeringAngleValue = new CrawlerDigitalDisplay(_v, "RightSteeringAngleValue"));
	Add(pRightSteeringAngleGauge = new CrawlerBarGauge(_v, "RightSteeringAngleGauge", false));

	Add(pAverageHeightValue = new CrawlerDigitalDisplay(_v, "RightSteeringAngleValue"));
	Add(pAverageHeightGauge = new CrawlerBarGauge(_v, "AverageHeightGauge", true));
}

CrawlerRightPanel::~CrawlerRightPanel()
{
}

void CrawlerRightPanel::RegisterVC()
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

	oapiVCRegisterArea(AID_RIGHT_MIN+aid_ofs, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN);
	oapiVCSetAreaClickmode_Quadrilateral(AID_RIGHT_MIN+aid_ofs,
		TransformVector(_V(-0.267, 0.18, -0.194))+ofs, 
		TransformVector(_V(-0.420, 0.18, -0.026))+ofs, 
		TransformVector(_V(-0.267, -0.040, -0.108))+ofs,
		TransformVector(_V(-0.420, -0.040, 0.073))+ofs);

	SURFHANDLE panels_tex = oapiGetTextureHandle(V()->GetVCMesh(cabID), 1);
	oapiVCRegisterArea(AID_GCIR_PBI+aid_ofs, _R(728, 840, 788, 870), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panels_tex);
	oapiVCRegisterArea(AID_INDEP_PBI+aid_ofs, _R(863, 840, 923, 870), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panels_tex);
	oapiVCRegisterArea(AID_CRAB_PBI+aid_ofs, _R(795, 840, 855, 870), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panels_tex);
	oapiVCRegisterArea(AID_MODE_PBI+aid_ofs, _R(728, 911, 788, 941), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panels_tex);

	SURFHANDLE gauges_tex = oapiGetTextureHandle(V()->GetVCMesh(cabID), 2);
	oapiVCRegisterArea(AID_STEERING_DES_VALUE+aid_ofs, _R(48, 304, 198, 346), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, gauges_tex);
	oapiVCRegisterArea(AID_STEEEING_DES_BAR+aid_ofs, _R(47, 366, 528, 394), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, gauges_tex);
	oapiVCRegisterArea(AID_STEERING_L_VALUE+aid_ofs, _R(50, 488, 200, 530), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, gauges_tex);
	oapiVCRegisterArea(AID_STEERING_L_BAR+aid_ofs, _R(49, 550, 530, 578), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, gauges_tex);
	oapiVCRegisterArea(AID_STEERING_R_VALUE+aid_ofs, _R(50, 870, 200, 912), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, gauges_tex);
	oapiVCRegisterArea(AID_STEERING_R_BAR+aid_ofs, _R(49, 932, 530, 960), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, gauges_tex);
	oapiVCRegisterArea(AID_AVG_HEIGHT_VALUE+aid_ofs, _R(767, 822, 917, 864), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, gauges_tex);
	oapiVCRegisterArea(AID_AVG_HEIGHT_BAR+aid_ofs, _R(827, 351, 859, 805), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, gauges_tex);

	oapiWriteLog("CrawlerRightPanel::RegisterVC() called");
}

void CrawlerRightPanel::DefineVC()
{
	int aid_ofs;
	if(cabID==FWD) aid_ofs = 0;
	else  aid_ofs = AID_REAR_OFFSET;

	AddAIDToMouseEventList(AID_RIGHT_MIN+aid_ofs);

	DefineSteeringModePBI(pGreatCircle);
	pGreatCircle->AddAIDToRedrawEventList(AID_GCIR_PBI+aid_ofs);
	pGreatCircle->SetMouseRegion(0.254695f, 0.774857f, 0.384179f, 0.901405f);
	DefineSteeringModePBI(pCrab);
	pCrab->AddAIDToRedrawEventList(AID_CRAB_PBI+aid_ofs);
	pCrab->SetMouseRegion(0.350553f, 0.791195f, 0.477232f, 0.914307f);
	DefineSteeringModePBI(pIndependent);
	pIndependent->AddAIDToRedrawEventList(AID_INDEP_PBI+aid_ofs);
	pIndependent->SetMouseRegion(0.443358f, 0.808174f, 0.594612f, 0.943099f);

	pMode->SetBaseCoords(10, 10);
	pMode->SetDimensions(60, 30);
	pMode->AllowReset(true);
	pMode->SetSourceImage(g_Resources.pbi_lights);
	pMode->SetSourceCoords(false, 0, PBI_6DEG);
	pMode->SetSourceCoords(true, 0, PBI_2DEG);
	pMode->AddAIDToRedrawEventList(AID_MODE_PBI+aid_ofs);
	pMode->SetMouseRegion(0.0, 0.0, 0.118125f, 0.192183f);

	pDesSteeringAngleValue->AddAIDToRedrawEventList(AID_STEERING_DES_VALUE+aid_ofs);
	DefineSteeringAngleDisplay(pDesSteeringAngleValue);

	pDesSteeringAngleGauge->AddAIDToRedrawEventList(AID_STEEEING_DES_BAR+aid_ofs);
	DefineSteeringAngleGauge(pDesSteeringAngleGauge);

	pLeftSteeringAngleValue->AddAIDToRedrawEventList(AID_STEERING_L_VALUE+aid_ofs);
	DefineSteeringAngleDisplay(pLeftSteeringAngleValue);

	pLeftSteeringAngleGauge->AddAIDToRedrawEventList(AID_STEERING_L_BAR+aid_ofs);
	DefineSteeringAngleGauge(pLeftSteeringAngleGauge);

	pRightSteeringAngleValue->AddAIDToRedrawEventList(AID_STEERING_R_VALUE+aid_ofs);
	DefineSteeringAngleDisplay(pRightSteeringAngleValue);
	pRightSteeringAngleGauge->AddAIDToRedrawEventList(AID_STEERING_R_BAR+aid_ofs);
	DefineSteeringAngleGauge(pRightSteeringAngleGauge);

	pAverageHeightGauge->SetScaleValue(6.0);
	pAverageHeightGauge->SetRange(0.0, 6.0);
	pAverageHeightGauge->SetBaseCoords(0, 0);
	pAverageHeightGauge->SetDimensions(32, 454, 37);
	pAverageHeightGauge->AddAIDToRedrawEventList(AID_AVG_HEIGHT_BAR+aid_ofs);

	pAverageHeightValue->SetScaleValue(6.0);
	pAverageHeightValue->SetBaseCoords(0, 0);
	pAverageHeightValue->SetDimensions(150, 42, 5);
	pAverageHeightValue->AddAIDToRedrawEventList(AID_AVG_HEIGHT_VALUE+aid_ofs);
}

void CrawlerRightPanel::DefineSteeringModePBI(CrawlerPBI* pPBI) const
{
	pPBI->SetBaseCoords(0, 0);
	pPBI->SetDimensions(60, 30);
	pPBI->SetSourceImage(g_Resources.pbi_lights);
	pPBI->SetSourceCoords(false, 0, PBI_BLACK);
	pPBI->SetSourceCoords(true, 0, PBI_GO);
}

void CrawlerRightPanel::DefineSteeringAngleGauge(CrawlerBarGauge* pGauge) const
{
	pGauge->SetScaleValue(static_cast<float>(MAX_TURN_ANGLE*DEG));
	pGauge->SetRange(-7.0, 7.0);
	pGauge->SetBaseCoords(0, 0);
	pGauge->SetDimensions(481, 28, 37);
}

void CrawlerRightPanel::DefineSteeringAngleDisplay(CrawlerDigitalDisplay* pDisplay) const
{
	pDisplay->SetScaleValue(static_cast<float>(MAX_TURN_ANGLE*DEG));
	pDisplay->SetBaseCoords(0, 0);
	pDisplay->SetDimensions(150, 42, 5);
}

void CrawlerRightPanel::Realize()
{
	DiscreteBundle* pBundle = V()->BundleManager()->CreateBundle("CRAWLER_STEERING", 8);

	pDesSteeringAngleValue->ConnectPort(pBundle, cabID);
	pDesSteeringAngleGauge->ConnectPort(pBundle, cabID);
	pLeftSteeringAngleValue->ConnectPort(pBundle, 5+cabID);
	pLeftSteeringAngleGauge->ConnectPort(pBundle, 5+cabID);
	pRightSteeringAngleValue->ConnectPort(pBundle, 6-cabID);
	pRightSteeringAngleGauge->ConnectPort(pBundle, 6-cabID);

	pGreatCircle->ConnectPort(pBundle, 2);
	pCrab->ConnectPort(pBundle, 3);
	pIndependent->ConnectPort(pBundle, 4);
	pMode->ConnectPort(pBundle, 7);

	// setup group ports
	pGreatCircle->ConnectGroupPort(pBundle, 3);
	pGreatCircle->ConnectGroupPort(pBundle, 4);
	pCrab->ConnectGroupPort(pBundle, 2);
	pCrab->ConnectGroupPort(pBundle, 4);
	pIndependent->ConnectGroupPort(pBundle, 2);
	pIndependent->ConnectGroupPort(pBundle, 3);

	pBundle = V()->BundleManager()->CreateBundle("CRAWLER_JEL", 1);
	pAverageHeightGauge->ConnectPort(pBundle, 0);
	pAverageHeightValue->ConnectPort(pBundle, 0);

	CrawlerPanel::Realize();
}

};