/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel Group definition



  Space Shuttle Ultra is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Ultra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Ultra; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://spaceshuttleultra.sourceforge.net/license/ for more details.

  **************************************************************************/
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
		void HidePanels();
		void ShowPanels();
	};
};