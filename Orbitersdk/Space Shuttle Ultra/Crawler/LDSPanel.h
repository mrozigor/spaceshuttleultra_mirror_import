#include "orbitersdk.h"
#include "Crawler_vc_defs.h"
#include <vector>

const int OFF = 0;
const int ON = 1;

namespace vc
{


	class LDS : public CrawlerPanel
	{
		std::vector<MGROUP_ROTATE*> vpAnimations;
	public:
		LDS(Crawler* _v, const std::string& _ident, CRAWLER_CAB _cab);
		virtual ~LDS();

		UINT LeftKnobAnim, CenterKnobAnim, RightKnobAnim;
		int LeftKnobState, CenterKnobState, RightKnobState;

		void ShowAlignmentError(double lateral);
		void CalculateDistanceBetweenAttachments(VECTOR3 MLP_ATTACH_GLOBAL, MATRIX3 RotationMatrix);
		virtual void RegisterVC();
		virtual void DefineVC();
		virtual void Realize();

		virtual void DefineVCAnimations(UINT vcidx);
		virtual bool OnVCMouseEvent(int id, int _event, VECTOR3 &p);
	};
};