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
		TransformVector(_V(-0.310, 0.020, -0.076))+ofs,
		TransformVector(_V(-0.404, 0.020, 0.048))+ofs, 
		TransformVector(_V(-0.297, -0.040, -0.071))+ofs,
		TransformVector(_V(-0.398, -0.040, 0.052))+ofs);

	SURFHANDLE panels_tex = oapiGetTextureHandle(V()->GetVCMesh(cabID), 1);
	oapiVCRegisterArea(AID_GCIR_PBI+aid_ofs, _R(728, 840, 788, 870), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panels_tex);
	oapiVCRegisterArea(AID_INDEP_PBI+aid_ofs, _R(863, 840, 923, 870), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panels_tex);
	oapiVCRegisterArea(AID_CRAB_PBI+aid_ofs, _R(795, 840, 855, 870), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panels_tex);

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
	pGreatCircle->SetMouseRegion(0.0f, 0.45f, 0.130f, 0.88f);
	DefineSteeringModePBI(pCrab);
	pCrab->AddAIDToRedrawEventList(AID_CRAB_PBI+aid_ofs);
	pCrab->SetMouseRegion(0.134f, 0.510f, 0.279f, 0.867f);
	DefineSteeringModePBI(pIndependent);
	pIndependent->AddAIDToRedrawEventList(AID_INDEP_PBI+aid_ofs);
	pIndependent->SetMouseRegion(0.289f, 0.516f, 0.431f, 0.872f);
}

void CrawlerRightPanel::DefineSteeringModePBI(CrawlerPBI* pPBI) const
{
	pPBI->SetBaseCoords(0, 0);
	pPBI->SetDimensions(60, 30);
	pPBI->SetSourceImage(g_Resources.pbi_lights);
	pPBI->SetSourceCoords(false, 0, PBI_BLACK);
	pPBI->SetSourceCoords(true, 0, PBI_GO);
}

void CrawlerRightPanel::Realize()
{
	DiscreteBundle* pBundle = V()->BundleManager()->CreateBundle("CRAWLER_STEERING", 5);

	pGreatCircle->ConnectPort(pBundle, 2);
	pCrab->ConnectPort(pBundle, 3);
	pIndependent->ConnectPort(pBundle, 4);

	// setup group ports
	pGreatCircle->ConnectGroupPort(pBundle, 3);
	pGreatCircle->ConnectGroupPort(pBundle, 4);
	pCrab->ConnectGroupPort(pBundle, 2);
	pCrab->ConnectGroupPort(pBundle, 4);
	pIndependent->ConnectGroupPort(pBundle, 2);
	pIndependent->ConnectGroupPort(pBundle, 3);
}

};