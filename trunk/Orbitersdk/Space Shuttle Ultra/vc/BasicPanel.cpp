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

	BasicVCComponent* BasicPanel::GetSwitch(const string& switchID) const {
		unsigned long i;
		for(i = 0; i<components.size(); i++)
		{
			BasicVCComponent* currentElement = components.at(i);
			if(currentElement->GetIdentifier() == switchID)
			{
				return currentElement;
			}
		}
		return NULL;
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

	bool BasicPanel::OnReadState (FILEHANDLE scn) {
		char* line;
		char pszBuffer[256];
		char pszBuffer2[256];
		sprintf_s(pszBuffer, 255, "\t\tParse panel block for panel \"%s\"...",
			GetQualifiedIdentifier().c_str());
		oapiWriteLog(pszBuffer);
		while(oapiReadScenario_nextline(scn, line)) {
			if(!_strnicmp(line, "@ENDPANEL", 9)) {
				oapiWriteLog("\t\tDone.");
				return true;
			} else if(!_strnicmp(line, "@OBJECT", 7)) {
				//oapiWriteLog("\t\tEnter Multiline switch block...");
				//Multi line object block
				//Get identifier of object or switch
				*line += 8;
				unsigned long i = 0;
				bool bStringFlag = false;
				while((*line != '\0' && *line != ' ') || bStringFlag) {
					if(*line == '\"') {
						bStringFlag = !bStringFlag;
					} else {
						pszBuffer[i] = *line;
						i++;
					}
					line++;
				}

				pszBuffer[i] = '\0';
				//Look up object
				sprintf_s(pszBuffer2, "\tLook up switch \"%s\"...", 
					pszBuffer);
				oapiWriteLog(pszBuffer2);
				//
				if(HasSwitch(pszBuffer)) {
					BasicVCComponent* pT = this->GetSwitch(pszBuffer);
					pT->OnReadState(scn);
				} else {
					//skip block
					while(oapiReadScenario_nextline(scn, line)) {
						if(!_strnicmp(line, "@ENDOBJECT", 10))
							break;
					}
				}
				//oapiWriteLog("\t\tLeave Multiline switch block...");
			} else {
				//single line object block
				//oapiWriteLog("\t\tEnter single line switch block...");
				unsigned long i = 0;
				bool bStringFlag = false;
				while((*line != '\0' && *line != ' ') || bStringFlag) {
					if(*line == '\"') {
						bStringFlag = !bStringFlag;
					} else {
						pszBuffer[i] = *line;
						i++;
					}
					line++;
				}

				pszBuffer[i] = '\0';
				line++;

				//Look up object
				sprintf_s(pszBuffer2, "\tLook up switch \"%s\"...\t\t(%s)", 
					pszBuffer, line);
				oapiWriteLog(pszBuffer2);
				

				if(HasSwitch(pszBuffer)) {
					oapiWriteLog("\tFound switch.");
					BasicVCComponent* pT = this->GetSwitch(pszBuffer);
					if(pT != NULL) {
						if(!pT->IsMultiLineSaveState()) {
							
							oapiWriteLog("\tRead switch state.");
							pT->OnParseLine(line);
						}
					}
				}

				//oapiWriteLog("\t\tLeave single line switch block...");
			}	
		}
		oapiWriteLog("\t\tParsing error.");
		return false;
	}

	void BasicPanel::OnSaveState(FILEHANDLE scn)
	{
		char pszBuffer[256];
		char pszBuffer2[256];

		if(components.empty()) {
			return;
		}

		strcpy_s(pszBuffer, 255, GetIdentifier().c_str());
		oapiWriteScenario_string(scn, "@PANEL", pszBuffer);
		vector<BasicVCComponent*>::iterator iter = components.begin();
		while(iter != components.end()) {
			if((*iter)->IsMultiLineSaveState()) {
				sprintf_s(pszBuffer, 255, "\"%s\"", 
						(*iter)->GetIdentifier().c_str());
				oapiWriteScenario_string(scn, "@OBJECT", 
					pszBuffer);
				(*iter)->OnSaveState(scn);
				oapiWriteLine(scn, "@ENDOBJECT");
			} else {
				if((*iter)->GetStateString(255, pszBuffer)) {
					sprintf_s(pszBuffer2, 255, "\"%s\"", 
						(*iter)->GetIdentifier().c_str());
					oapiWriteScenario_string(scn, 
						pszBuffer2,
						pszBuffer);
				}
			}
			iter++;
		}

		oapiWriteScenario_string(scn, "@ENDPANEL", "");
	}

	bool BasicPanel::OnVCMouseEvent(int id, int _event, VECTOR3 &p)
	{
		if(availableForMouse.find(id) != availableForMouse.end()) {
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
		bool bRedraw=false;

		vector<BasicVCComponent*>::iterator iter = components.begin();
		while(iter != components.end())
		{
			BasicVCComponent* comp = (*iter);
			if(comp->IsOwnRegion(id)) {
				if(comp->OnVCRedrawEvent(id, _event, surf)) bRedraw=true;
			}

			iter++;
		}
		return bRedraw;
	}

	void BasicPanel::DefinePanelMouseArea()
	{
	}

	void BasicPanel::OnPreStep(double fSimT, double fDeltaT, double fMJD)
	{
		vector<BasicVCComponent*>::iterator iter = components.begin();
		while(iter != components.end())
		{
			(*iter)->OnPreStep(fSimT, fDeltaT, fMJD);
			iter++;
		}
	}

	void BasicPanel::OnPostStep(double fSimT, double fDeltaT, double fMJD)
	{
		vector<BasicVCComponent*>::iterator iter = components.begin();
		while(iter != components.end())
		{
			(*iter)->OnPostStep(fSimT, fDeltaT, fMJD);
			iter++;
		}
	}

	void BasicPanel::OnPropagate(double fSimT, double fDeltaT, double fMJD)
	{
		vector<BasicVCComponent*>::iterator iter = components.begin();
		while(iter != components.end())
		{
			(*iter)->OnPropagate(fSimT, fDeltaT, fMJD);
			iter++;
		}
	}

	bool BasicPanel::HasSwitch(const string& id)
	{
		unsigned long i;
		for(i = 0; i<components.size(); i++)
		{
			BasicVCComponent* currentElement = components.at(i);
			if(currentElement->GetIdentifier() == id)
			{
				return true;
			}
		}
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
