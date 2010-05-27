#ifndef _CRAWLERVC_H_0473345F_48A7_4b64_851E_609BA1380DD3
#define _CRAWLERVC_H_0473345F_48A7_4b64_851E_609BA1380DD3
#pragma once

#include <orbitersdk.h>
#include <vector>
#include "BasicPanel.h"
#include "Subsystem.h"
#include "DiscInPort.h"
//#include "DiscreteBundle.h"

class Crawler;


const VECTOR3 CRAWLER_REAR_VC_OFFSET = _V(-15.77, 4.544, -18.0);
const VECTOR3 CRAWLER_FWD_VC_OFFSET = _V(15.59, 4.544, 18.14);

namespace vc
{

class CrawlerVC : public BasicPanel<Crawler>
{
public:
	typedef enum {FWD = 0, REAR = 1} CRAWLER_CAB;

	CrawlerVC(Crawler* _v, const std::string& _ident, CRAWLER_CAB _cab);
	~CrawlerVC();

	virtual void DefineVCAnimations (UINT vcidx);
	virtual void RegisterVC();
	virtual void DefineVC();
	virtual UINT GetVCMeshIndex() const { return meshIdx; };
	virtual void AddMeshes(const VECTOR3& ofs);

	virtual void OnPreStep(double SimT, double SimDT, double MJD);
	virtual void Realize();
private:
	/**
	 * Rotates vector for Fwd cab
	 */
	VECTOR3 TransformVector(const VECTOR3& v) {
		if(cabID == FWD) return _V(-v.x, v.y, -v.z);
		else return v;
	};
	/*VECTOR3 TransformDirVector(const VECTOR3& v) {
		if(cabID == FWD) return _V(v.x, -v.y, v.z);
		else return v;
	};*/

	vector<MGROUP_TRANSFORM*> vpAnimations;

	CRAWLER_CAB cabID;

	MESHHANDLE hVCMesh;
	UINT meshIdx;

	UINT anim_steering;

	DiscInPort steering;
};

};

#endif