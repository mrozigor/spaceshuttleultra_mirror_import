#include "CrawlerRightPanel.h"
#include "Crawler.h"

namespace vc
{

CrawlerRightPanel::CrawlerRightPanel(Crawler* _v, const std::string& _ident, CRAWLER_CAB _cab)
: CrawlerPanel(_v, _ident, _cab)
{
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
		//ofs = TransformVector(CRAWLER_REAR_VC_OFFSET);
		aid_ofs = 0;
	}
	else {
		ofs = CRAWLER_REAR_VC_OFFSET;
		aid_ofs = AID_REAR_OFFSET;
	}

	//oapiVCRegisterArea(AID_GCIR_PBI+aid_ofs, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN);
	//oapiVCSetAreaClickmode_Quadrilateral(AID_GCIR_PBI+aid_ofs, _V(-0.303, -0.017, -0.076)+ofs, _V(-0.312, -0.017, -0.064)+ofs,
		//_V(-0.297, -0.031, -0.072)+ofs, _V(-0.306, -0.031, -0.06)+ofs);

	oapiVCRegisterArea(AID_RIGHT_MIN+aid_ofs, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN);
	/*oapiVCSetAreaClickmode_Quadrilateral(AID_RIGHT_MIN+aid_ofs,
		TransformVector(_V(-0.303, -0.017, -0.076))+ofs,
		TransformVector(_V(-0.312, -0.017, -0.064))+ofs,
		TransformVector(_V(-0.297, -0.031, -0.072))+ofs,
		TransformVector(_V(-0.306, -0.031, -0.06))+ofs);
	/*oapiVCSetAreaClickmode_Quadrilateral(AID_RIGHT_MIN+aid_ofs,
		TransformVector(_V(-0.245, 0.143, -0.248))+ofs, 
		TransformVector(_V(-0.652, 0.143, 0.241))+ofs,
		TransformVector(_V(-0.232, -0.054, -0.155))+ofs,
		TransformVector(_V(-0.547, -0.054, 0.241))+ofs);*/
	oapiVCSetAreaClickmode_Quadrilateral(AID_RIGHT_MIN+aid_ofs,
		TransformVector(_V(-0.264, 0.142, -0.253))+ofs, 
		TransformVector(_V(-0.653, 0.142, 0.241))+ofs,
		TransformVector(_V(-0.231, -0.052, 0.256))+ofs,
		TransformVector(_V(-0.547, -0.052, 0.241))+ofs);

	oapiWriteLog("CrawlerRightPanel::RegisterVC() called");
}

void CrawlerRightPanel::DefineVC()
{
	if(cabID==FWD) AddAIDToMouseEventList(AID_RIGHT_MIN);
	else AddAIDToMouseEventList(AID_RIGHT_MIN+AID_REAR_OFFSET);
}

void CrawlerRightPanel::Realize()
{
	DiscreteBundle* pBundle = V()->BundleManager()->CreateBundle("CRAWLER_STEERING", 5);

	pGreatCircle->ConnectPort(pBundle, 2);
}

};