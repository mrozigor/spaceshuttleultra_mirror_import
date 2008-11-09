#include "PanelGroup.h"
#include "BasicPanel.h"

namespace vc {

	PanelGroup::PanelGroup()
	{
	}

	PanelGroup::~PanelGroup()
	{
		while(!panels.empty()) {
			BasicPanel* panel=panels.back();
			if(panel) delete panel;
			panels.pop_back();
		}
	}

	bool PanelGroup::AddPanel(BasicPanel* pPanel)
	{
		panels.push_back(pPanel);
		return true;
	}

	void PanelGroup::DefineVCAnimations(UINT vcidx)
	{
		for(unsigned int i = 0; i<panels.size(); i++)
			panels.at(i)->DefineVCAnimations(vcidx);
	}

	void PanelGroup::DefineVC()
	{
		for(unsigned int i = 0; i<panels.size(); i++)
			panels.at(i)->DefineVC();
	}

	bool PanelGroup::HasPanel(const string& panelname) const {
		for(unsigned int i = 0; i<panels.size(); i++) {
			if(panels.at(i)->GetIdentifier() == panelname) {
				return true;
			}
		}
		return false;
	}

	bool PanelGroup::ParsePanelBlock(const string& panelname, FILEHANDLE scn) {
		
		for(unsigned int i = 0; i<panels.size(); i++) {
			if(panels.at(i)->GetIdentifier() == panelname) {
				return panels.at(i)->OnReadState(scn);
			}
		}
		oapiWriteLog("\tNo Panel found here!");
		return false;
	}

	void PanelGroup::RegisterVC()
	{
		for(unsigned int i = 0; i<panels.size(); i++)
			panels.at(i)->RegisterVC();
	}

	void PanelGroup::Realize()
	{
		for(unsigned int i = 0; i<panels.size(); i++)
			panels.at(i)->Realize();
	}

	bool PanelGroup::OnParseLine(char *line)
	{
		for(unsigned int i = 0; i<panels.size(); i++)
		{
			//Test syntax of line
			panels.at(i)->OnParseLine(line);
		}
		return false;
	}

	void PanelGroup::OnPostStep(double fSimT, double fDeltaT, double fMJD)
	{
		for(unsigned int i = 0; i<panels.size(); i++)
			panels.at(i)->OnPostStep(fSimT, fDeltaT, fMJD);
	}

	void PanelGroup::OnPreStep(double fSimT, double fDeltaT, double fMJD)
	{
		for(unsigned int i = 0; i<panels.size(); i++)
			panels.at(i)->OnPreStep(fSimT, fDeltaT, fMJD);
	}

	void PanelGroup::OnPropagate(double fSimT, double fDeltaT, double fMJD)
	{
		for(unsigned int i = 0; i<panels.size(); i++)
			panels.at(i)->OnPropagate(fSimT, fDeltaT, fMJD);
	}

	void PanelGroup::OnSaveState(FILEHANDLE scn)
	{
		char pszBuffer[256];
		if(panels.empty()) {
			oapiWriteLog("\tNo panels to be saved here...");
			return;
		}
		for(unsigned int i = 0; i<panels.size(); i++) {
			sprintf_s(pszBuffer, 255, "\tSave panel \"%s\" ...",
				panels.at(i)->GetQualifiedIdentifier());
			oapiWriteLog(pszBuffer);
			panels.at(i)->OnSaveState(scn);
		}
	}

	bool PanelGroup::OnVCMouseEvent(int id, int _event, VECTOR3 &p)
	{
		bool r = false;
		for(unsigned int i = 0; i<panels.size(); i++)
			r |= panels.at(i)->OnVCMouseEvent(id, _event, p);
		return r;
	}

	bool PanelGroup::OnVCRedrawEvent(int id, int _event, SURFHANDLE surf)
	{
		bool r = false;
		for(unsigned int i = 0; i<panels.size(); i++)
			r |= panels.at(i)->OnVCRedrawEvent(id, _event, surf);
		return r;
	}

	bool PanelGroup::EnableCoordinateDisplayMode()
	{
		for(unsigned int i = 0; i<panels.size(); i++)
			panels.at(i)->EnableCoordinateDisplayMode();
		return true;
	}

	bool PanelGroup::DisableCoordinateDisplayMode()
	{
		for(unsigned int i = 0; i<panels.size(); i++)
			panels.at(i)->DisableCoordinateDisplayMode();
		return true;
	}

	bool PanelGroup::ToggleCoordinateDisplayMode()
	{
		for(unsigned int i = 0; i<panels.size(); i++)
			panels.at(i)->ToggleCoordinateDisplayMode();
		return true;
	}

};
