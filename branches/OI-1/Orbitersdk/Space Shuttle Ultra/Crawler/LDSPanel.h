#include <orbitersdk.h>
#include "Crawler_vc_defs.h"
#include <vector>

const int OFF = 0;
const int ON = 1;
const VECTOR3 VAB_HIGHBAY1 = _V(29,7.5,49.25);
const VECTOR3 VAB_HIGHBAY2 = _V(-29,7.5,49.25);

namespace vc
{


	class LDS : public CrawlerPanel
	{
		std::vector<MGROUP_ROTATE*> vpAnimations;
		std::vector<MGROUP_TRANSLATE*> LDSAnimation;
	public:
		LDS(Crawler* _v, const std::string& _ident, CRAWLER_CAB _cab);
		virtual ~LDS();

		UINT LeftKnobAnim, CenterKnobAnim, RightKnobAnim, LDSBarAnim;
		int LeftKnobState, CenterKnobState, RightKnobState;

		std::string target;

		void ShowAlignmentError(double lateral);
		double CalculateDistanceBetweenAttachments();
		virtual void RegisterVC();
		virtual void DefineVC();
		virtual void Realize();
		virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);
		double CalculateOffset(double accuracy, double distance);

		virtual void DefineVCAnimations(UINT vcidx);
		virtual bool OnVCMouseEvent(int id, int _event, VECTOR3 &p);
	};
};