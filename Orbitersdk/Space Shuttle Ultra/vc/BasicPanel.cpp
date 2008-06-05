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
		pNew->SetParentPanel(this);
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
		char pszBuffer[256];
		sprintf_s(pszBuffer, 255, "BasicPanel[%s]:\tDefine VC Animations. %d components", 
			GetQualifiedIdentifier().c_str(), components.size());
		//local_vcidx = vcidx;
		oapiWriteLog(pszBuffer);
		vector<BasicVCComponent*>::iterator iter = components.begin();
		while(iter != components.end())
		{
			//oapiWriteLog("+");
			(*iter)->DefineVCAnimations(vcidx);
			iter++;
		}
	}

	bool BasicPanel::DistributeMouseEvent(int _event, const VECTOR3& p)
	{
		unsigned long i;
		float mx = 0, my = 0;
		for(i = 0; i<components.size(); i++)
		{
			BasicVCComponent* currentElement = components.at(i);
			if(currentElement->IsPointOver((float)p.x, (float)p.y))
			{
				currentElement->ProjectMouse((float)p.x, (float)p.y, mx, my);	
				if(currentElement->OnMouseEvent(_event, mx, my))
					return true;
			}
		}
		return false;
	}

	bool BasicPanel::FindComponent(const VECTOR3& p, BasicVCComponent** foundElement) const
	{
		unsigned long i;
		for(i = 0; i<components.size(); i++)
		{
			BasicVCComponent* currentElement = components.at(i);
			if(currentElement->IsPointOver((float)p.x, (float)p.y))
			{
				*foundElement = currentElement;
				return true;
			}
		}
		return false;
	}

	UINT BasicPanel::GetVCMeshIndex() const
	{
		return local_vcidx;
	}

	void BasicPanel::RegisterVC()
	{
		vector<BasicVCComponent*>::iterator iter = components.begin();
		while(iter != components.end())
		{
			(*iter)->RegisterVC();
			iter++;
		}
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
				BasicVCComponent* foundElement;
				if(FindComponent(p, &foundElement))
				{
					sprintf_s(oapiDebugString(), 255, "PANEL %s MOUSEEVENT (%d, %f, %f, %f) FOR %s",
						name.c_str(), _event, p.x, p.y, p.z,
						foundElement->GetQualifiedIdentifier().c_str());
				} else 
				{
					sprintf_s(oapiDebugString(), 255, "PANEL %s MOUSEEVENT (%d, %f, %f, %f)",
						name.c_str(), _event, p.x, p.y, p.z);
				}
				return DistributeMouseEvent(_event, p);
			} else {
				
				return DistributeMouseEvent(_event, p);
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
		//static string null = "";
		return name;
	}

	const string& BasicPanel::GetQualifiedIdentifier() const
	{
		//static string null = "";
		return name;
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