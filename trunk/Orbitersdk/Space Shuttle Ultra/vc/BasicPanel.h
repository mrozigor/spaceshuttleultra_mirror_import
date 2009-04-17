/****************************************************************************
  This file is part of Space Shuttle Ultra

  Basic panel definitions



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
#ifndef ___BASIC_PANEL_H_INCLUDED___
#define ___BASIC_PANEL_H_INCLUDED___

#include "OrbiterAPI.h"
#include <set>
#include <string>
#include <vector>
#include "vc_defs.h"
//#include "../ISubsystem.h"

class Atlantis;

namespace vc 
{

using namespace std;

class BasicVCComponent;
class BasicSwitch;

class BasicPanel
{
	UINT local_vcidx;
	Atlantis* psts;
	unsigned long own_aid;
	string name;
	
	set<UINT> availableForMouse;
	vector<BasicVCComponent*> components;

	bool bHasOwnVCMesh;
	bool bCoordinateDisplayMode;
protected:
	//bool AddSwitch(BasicSwitch* pSwitch);
	BasicSwitch* CreateSwitch2(const string& _name, const VECTOR3& _RefPos, UINT _GrpNum, RECT _r);
	BasicSwitch* CreateSwitch3(const string& _name, const VECTOR3& _RefPos, UINT _GrpNum, RECT _r);
	BasicSwitch* CreateSwitch3H(const string& _name, const VECTOR3& _RefPos, UINT _GrpNum, RECT _r);

	virtual void DefinePanelMouseArea();
	virtual void DefineSwitches();
	void SetHasOwnVCMesh() {bHasOwnVCMesh = true;};
	void AddAIDToMouseEventList(UINT aid);
	
	void DeleteAllComponents();
	bool DistributeMouseEvent(int _event, const VECTOR3& p);
	bool FindComponent(const VECTOR3& p, BasicVCComponent** foundElement) const;
public:
	BasicPanel(Atlantis* sts, const string& _name);
	virtual ~BasicPanel();
	Atlantis* STS() const;
	virtual void DefineVCAnimations (UINT vcidx);
	virtual void RegisterVC();
	virtual void DefineVC() = 0;
	virtual UINT GetVCMeshIndex() const;
	virtual void ReplaceComponent(unsigned long ulIndex, BasicVCComponent* pReplacement);

	void SetAID(unsigned long aid) {own_aid = aid;};

	virtual BasicVCComponent* Add(BasicVCComponent* pNew);
	
	
	virtual void AddMeshes(const VECTOR3& ofs) {};
	virtual void SetMeshVisibility(bool visible) {};
	virtual BasicVCComponent* GetSwitch(const string& switchID) const;
	virtual void Realize();
	virtual bool OnVCMouseEvent (int id, int _event, VECTOR3 &p);
	virtual bool OnVCRedrawEvent (int id, int _event, SURFHANDLE surf);
	virtual void OnPreStep (double fSimT, double fDeltaT, double fMJD);
	virtual void OnPostStep (double fSimT, double fDeltaT, double fMJD);
	virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD);
	virtual bool OnParseLine (char *line);
	virtual bool OnReadState (FILEHANDLE scn);
	virtual void OnSaveState (FILEHANDLE scn);
	
	virtual const string& GetIdentifier() const;
	virtual const string& GetQualifiedIdentifier() const;

	virtual bool HasSwitch(const string& switchID);

	bool HasOwnVCMesh() const {return bHasOwnVCMesh;};


	bool EnableCoordinateDisplayMode() {bCoordinateDisplayMode = true; return true;};
	bool DisableCoordinateDisplayMode() {bCoordinateDisplayMode = false; return true;};
	bool ToggleCoordinateDisplayMode() {bCoordinateDisplayMode = !bCoordinateDisplayMode; return true;};


};

};

#endif