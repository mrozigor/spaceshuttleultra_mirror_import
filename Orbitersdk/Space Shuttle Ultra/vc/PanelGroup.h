#pragma once

#include "OrbiterAPI.h"
#include <vector>
#include "vc_defs.h"

namespace vc {
	using namespace std;

	class PanelGroup {
		vector<BasicPanel*> panels;
	public:
		PanelGroup();
		virtual ~PanelGroup();

		virtual bool AddPanel(BasicPanel* pPanel);
		void RegisterVC();
		void DefineVCAnimations (UINT vcidx);
		void DefineVC();
		bool HasPanel(const string& panelname) const;
		bool ParsePanelBlock(const string& panelname, FILEHANDLE scn);
		void Realize();
		bool OnVCMouseEvent (int id, int _event, VECTOR3 &p);
		bool OnVCRedrawEvent (int id, int _event, SURFHANDLE surf);
		void OnPreStep (double fSimT, double fDeltaT, double fMJD);
		void OnPostStep (double fSimT, double fDeltaT, double fMJD);
		void OnPropagate(double fSimT, double fDeltaT, double fMJD);
		bool OnParseLine (char *line);
		void OnSaveState (FILEHANDLE scn);
		bool EnableCoordinateDisplayMode();
		bool DisableCoordinateDisplayMode();
		bool ToggleCoordinateDisplayMode();
	};
};