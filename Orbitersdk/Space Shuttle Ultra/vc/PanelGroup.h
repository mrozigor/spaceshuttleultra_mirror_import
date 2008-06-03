#pragma once

#include <vector>
#include "vc_defs.h"

namespace vc {
	using namespace std;

	class PanelGroup {
		vector<BasicPanel*> panels;
	public:
		PanelGroup();
		virtual ~PanelGroup();
		virtual void DefineVCAnimations (UINT vcidx);
		virtual void DefineVC();
		virtual void Realize();
		virtual bool OnVCMouseEvent (int id, int _event, VECTOR3 &p);
		virtual bool OnVCRedrawEvent (int id, int _event, SURFHANDLE surf);
		virtual void OnPreStep (double fSimT, double fDeltaT, double fMJD);
		virtual void OnPostStep (double fSimT, double fDeltaT, double fMJD);
		virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD);
		virtual bool OnParseLine (char *line);
		virtual void OnSaveState (FILEHANDLE scn);
	};
};