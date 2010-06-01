#include "CrawlerCenterPanel.h"
#include "Crawler.h"
#include "meshres_crawler_vc.h"

namespace vc
{

CrawlerCenterPanel::CrawlerCenterPanel(Crawler* _v, const std::string& _ident, CRAWLER_CAB _cab)
: CrawlerPanel(_v, _ident, _cab)
{
	hVCMesh = NULL;
}

CrawlerCenterPanel::~CrawlerCenterPanel()
{
	for(vector<MGROUP_TRANSFORM*>::const_iterator i=vpAnimations.begin(); i!=vpAnimations.end();++i)
		delete (*i);
}

void CrawlerCenterPanel::RegisterVC()
{
	BasicPanel::RegisterVC();
}

void CrawlerCenterPanel::DefineVC()
{
}

void CrawlerCenterPanel::DefineVCAnimations(UINT vcidx)
{	
	static UINT SteeringWheel[2] = {GRP_GROUP_31_VC, GRP_GROUP_8_VC};
	MGROUP_ROTATE *pSteeringWheel = new MGROUP_ROTATE(vcidx, SteeringWheel, 2, TransformVector(_V(0.0, 0.043, -0.158)),
		TransformVector(_V(0, -0.50104, -0.86543)), (float)(180.0*RAD));
	anim_steering = V()->CreateAnimation(0.5);
	V()->AddAnimationComponent(anim_steering, 0.0, 1.0, pSteeringWheel);
	vpAnimations.push_back(pSteeringWheel);
}

void CrawlerCenterPanel::Realize()
{
	BasicPanel::Realize();

	// connect disc ports
	DiscreteBundle* pBundle = V()->BundleManager()->CreateBundle("CRAWLER_STEERING", 5);
	steering.Connect(pBundle, cabID);
}

void CrawlerCenterPanel::OnPreStep(double SimT, double SimDT, double MJD)
{
	BasicPanel::OnPreStep(SimT, SimDT, MJD);

	double steeringAnim = steering.GetVoltage()/2.0 + 0.5;
	V()->SetAnimation(anim_steering, steeringAnim);
}

};