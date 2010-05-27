#include "CrawlerVC.h"
#include "Crawler.h"
#include "meshres_crawler_vc.h"

namespace vc
{

CrawlerVC::CrawlerVC(Crawler* _v, const std::string& _ident, CRAWLER_CAB _cab)
: BasicPanel(_v, _ident), cabID(_cab)
{
	hVCMesh = NULL;
}

CrawlerVC::~CrawlerVC()
{
	for(vector<MGROUP_TRANSFORM*>::const_iterator i=vpAnimations.begin(); i!=vpAnimations.end();++i)
		delete (*i);
}

void CrawlerVC::AddMeshes(const VECTOR3 &ofs)
{
	VECTOR3 crawler_vc_offset;
	if(cabID==FWD) {
		crawler_vc_offset = CRAWLER_FWD_VC_OFFSET + ofs;
		meshIdx = V()->AddMesh(oapiLoadMeshGlobal("SSU\\Crawler_VC_panels_reversed"), &crawler_vc_offset);
	}
	else {
		crawler_vc_offset = CRAWLER_REAR_VC_OFFSET;
		meshIdx = V()->AddMesh(oapiLoadMeshGlobal("SSU\\Crawler_VC_panels"), &crawler_vc_offset);
	}
	V()->SetMeshVisibilityMode(meshIdx, MESHVIS_COCKPIT | MESHVIS_VC | MESHVIS_EXTERNAL);
}

void CrawlerVC::RegisterVC()
{
	BasicPanel::RegisterVC();
}

void CrawlerVC::DefineVC()
{
}

void CrawlerVC::DefineVCAnimations(UINT)
{	
	static UINT SteeringWheel[2] = {GRP_GROUP_31_VC, GRP_GROUP_8_VC};
	MGROUP_ROTATE *pSteeringWheel = new MGROUP_ROTATE(meshIdx, SteeringWheel, 2, TransformVector(_V(0.0, 0.043, -0.158)),
		TransformVector(_V(0, -0.50104, -0.86543)), (float)(180.0*RAD));
	anim_steering = V()->CreateAnimation(0.5);
	V()->AddAnimationComponent(anim_steering, 0.0, 1.0, pSteeringWheel);
	vpAnimations.push_back(pSteeringWheel);
}

void CrawlerVC::Realize()
{
	BasicPanel::Realize();

	// connect disc ports
	DiscreteBundle* pBundle = V()->BundleManager()->CreateBundle("CRAWLER_STEERING", 4);
	steering.Connect(pBundle, cabID);
}

void CrawlerVC::OnPreStep(double SimT, double SimDT, double MJD)
{
	BasicPanel::OnPreStep(SimT, SimDT, MJD);

	double steeringAnim = steering.GetVoltage()/2.0 + 0.5;
	V()->SetAnimation(anim_steering, steeringAnim);
}

};