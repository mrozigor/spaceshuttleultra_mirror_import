#ifndef _CRAWLER_VC_DEFS_H_9D94C752_88E7_4ecd_8F96_10FBC0914FDE
#define _CRAWLER_VC_DEFS_H_9D94C752_88E7_4ecd_8F96_10FBC0914FDE
#pragma once

#include "BasicPanel.h"

class Crawler;

const VECTOR3 CRAWLER_REAR_VC_OFFSET = _V(-15.77, 4.544, -18.0);
const VECTOR3 CRAWLER_FWD_VC_OFFSET = _V(15.59, 4.544, 18.14);

namespace vc
{
	//typedef BasicPanel<Crawler> CrawlerPanel;
	typedef enum {FWD = 0, REAR = 1} CRAWLER_CAB;

	class CrawlerPanel : public BasicPanel<Crawler>
	{
	protected:
		CRAWLER_CAB cabID;
	public:
		CrawlerPanel(Crawler* _v, const std::string& _ident, CRAWLER_CAB _cab)
			: BasicPanel(_v, _ident), cabID(_cab) {};
		virtual ~CrawlerPanel() {};
	protected:
		/**
		 * Rotates vector for Fwd cab
		 */
		inline VECTOR3 TransformVector(const VECTOR3& v) {
			if(cabID == FWD) return _V(-v.x, v.y, -v.z);
			//if(cabID == FWD) return v;
			else return v;
		}
	};
};

#endif