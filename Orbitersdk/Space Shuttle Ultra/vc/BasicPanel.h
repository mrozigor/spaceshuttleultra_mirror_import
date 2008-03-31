#ifndef ___BASIC_PANEL_H_INCLUDED___
#define ___BASIC_PANEL_H_INCLUDED___

#include "OrbiterAPI.h"
#include <set>
#include <string>
#include <vector>
#include "ISubsystem.h"

using namespace std;

class BasicVCComponent;

class BasicPanel
{
	string name;
	set<int> availableForRendering;
	set<int> availableForMouse;
	vector<BasicVCComponent*> components;
protected:
	//bool AddSwitch(BasicSwitch* pSwitch);
	BasicSwitch* CreateSwitch2(const string& _name, const VECTOR3& _RefPos, UINT _GrpNum, RECT _r);
	BasicSwitch* CreateSwitch3(const string& _name, const VECTOR3& _RefPos, UINT _GrpNum, RECT _r);
	BasicSwitch* CreateSwitch3H(const string& _name, const VECTOR3& _RefPos, UINT _GrpNum, RECT _r);

	virtual void DefinePanelMouseArea();
	virtual void DefineSwitches();
public:
	BasicPanel(Atlantis* sts, const string& _name);
	virtual ~BasicPanel();
	virtual void DefineVCAnimations (UINT vcidx);
	virtual void RegisterVC();
	virtual void DefineVC() = 0;
	
	virtual void Realize();
	virtual bool OnVCMouseEvent (int id, int event, VECTOR3 &p);
	virtual bool OnVCRedrawEvent (int id, int event, SURFHANDLE surf);
	virtual void OnPreStep (double fSimT, double fDeltaT, double fMJD);
	virtual void OnPostStep (double fSimT, double fDeltaT, double fMJD);
	virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD);
	virtual bool OnParseLine (char *line);
	virtual void OnSaveState (FILEHANDLE scn);
	
	virtual const string& GetIdentfier() const;
	virtual const string& GetQualifiedIdentfier() const;

	virtual bool HasSwitch(const string& switchID);
};

#endif