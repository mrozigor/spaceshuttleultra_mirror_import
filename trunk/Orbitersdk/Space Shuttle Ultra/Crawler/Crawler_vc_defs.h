#ifndef _CRAWLER_VC_DEFS_H_9D94C752_88E7_4ecd_8F96_10FBC0914FDE
#define _CRAWLER_VC_DEFS_H_9D94C752_88E7_4ecd_8F96_10FBC0914FDE
#pragma once

#include "BasicPanel.h"

class Crawler;

const VECTOR3 CRAWLER_REAR_VC_OFFSET = _V(-15.242, 4.544, -18.35);
const VECTOR3 CRAWLER_FWD_VC_OFFSET = _V(15.242, 4.544, 18.35);

namespace vc
{
	//typedef BasicPanel<Crawler> CrawlerPanel;
	typedef enum {FWD = 0, REAR = 1} CRAWLER_CAB;

	const int PBI_OFF_RED = 0;
	const int PBI_ON_RED = 31;
	const int PBI_GO = 62;
	const int PBI_BLACK = 93;

	const int LIGHT_WIDTH = 87;
	const int LIGHT_HEIGHT = 67;
	const int LIGHT_ON_X = 0;
	const int LIGHT_OFF_X = 88;
	const int LIGHT_CAB_SELECTED_Y	= 0;
	const int LIGHT_MAX_ANGLE_Y		= 69;
	const int LIGHT_IN_CONTROL_Y	= 138;
	const int LIGHT_MOTOR_FIELD_Y	= 207;
	const int LIGHT_MOTOR_CHKS_Y	= 276;
	const int LIGHT_EXCITER_ON_Y	= 345;
	const int LIGHT_GROUND_FAULT_Y	= 414;
	const int LIGHT_BRAKES_ON_Y		= 483;

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