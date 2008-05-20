#include "../Atlantis.h"
#include "BasicPanel.h"
#include "BasicVCComponent.h"


namespace vc {

	BasicPanel::BasicPanel(Atlantis* sts, const string& _name)
	{
		name = _name;
		psts = sts;
		bCoordinateDisplayMode = false;
	}

	BasicPanel::~BasicPanel()
	{
		DeleteAllComponents();
	}

	BasicVCComponent* BasicPanel::Add(BasicVCComponent* pNew)
	{
		components.push_back(pNew);
		return pNew;
	}

	void BasicPanel::DeleteAllComponents() 
	{
		unsigned long i = 0;
		for(; i<components.size(); i++)
		{
			delete components.at(i);
		}
	}

	Atlantis* BasicPanel::STS() const
	{
		return psts;
	}

	void BasicPanel::DefineSwitches()
	{
	}

	void BasicPanel::DefineVCAnimations(UINT vcidx)
	{
	}

	void BasicPanel::RegisterVC()
	{
	}

	void BasicPanel::Realize()
	{
		vector<BasicVCComponent*>::iterator iter = components.begin();
		while(iter != components.end())
		{
			(*iter)->Realize();
			iter++;
		}
	}

	bool BasicPanel::OnParseLine(char *line)
	{
		return false;
	}

	void BasicPanel::OnSaveState(FILEHANDLE scn)
	{
		vector<BasicVCComponent*>::iterator iter = components.begin();
		while(iter != components.end())
		{
			(*iter)->OnSaveState(scn);
			iter++;
		}
	}

	bool BasicPanel::OnVCMouseEvent(int id, int _event, VECTOR3 &p)
	{
		if(availableForMouse.find(id) != availableForMouse.end())
		{
			if(bCoordinateDisplayMode)
			{
				sprintf_s(oapiDebugString(), 255, "PANEL %s MOUSEEVENT (%d, %f, %f, %f)",
					name.c_str(), _event, p.x, p.y, p.z);
				return true;
			}
			return true;
		}
		else {
			return false;
		}
	}

	bool BasicPanel::OnVCRedrawEvent(int id, int _event, SURFHANDLE surf)
	{
		return false;
	}

	void BasicPanel::DefinePanelMouseArea()
	{
	}

	void BasicPanel::OnPreStep(double fSimT, double fDeltaT, double fMJD)
	{
	}

	void BasicPanel::OnPostStep(double fSimT, double fDeltaT, double fMJD)
	{
	}

	void BasicPanel::OnPropagate(double fSimT, double fDeltaT, double fMJD)
	{
	}

	bool BasicPanel::HasSwitch(const string& id)
	{
		return false;
	}

	const string& BasicPanel::GetIdentifier() const
	{
		static string null = "";
		return name;
	}

	const string& BasicPanel::GetQualifiedIdentifier() const
	{
		static string null = "";
		return null;
	}

	void BasicPanel::ReplaceComponent(unsigned long ulIndex, BasicVCComponent* pReplacement)
	{
		if(ulIndex < components.size())
		{
			components.at(ulIndex) = pReplacement;
		}
	}

	void BasicPanel::AddAIDToMouseEventList(UINT aid)
	{
		if(availableForMouse.empty())
		{
			availableForMouse.insert(aid);
			return;
		}
		if(availableForMouse.find(aid) == availableForMouse.end())
		{
			availableForMouse.insert(aid);
		}
	}

};