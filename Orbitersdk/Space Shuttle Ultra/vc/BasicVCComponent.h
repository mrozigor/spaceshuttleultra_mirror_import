// BasicVCComponent.h: Schnittstelle für die Klasse BasicVCComponent.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASICVCCOMPONENT_H__B74A68D7_2919_4C3F_AB0C_4B4FBA3AA2B7__INCLUDED_)
#define AFX_BASICVCCOMPONENT_H__B74A68D7_2919_4C3F_AB0C_4B4FBA3AA2B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vc_defs.h"
#include "../Atlantis.h"
#include "../ISubsystem.h"
#include <string>

class Atlantis;

namespace vc {

	using namespace std;



class BasicVCComponent: public ISubsystem
{
	Atlantis* sts;
	string ident;
	VECTOR3 reference;
	VECTOR3 dir;
	VCMOUSEEVENTPOINT p_a, p_b, p_c, p_d;

	mutable float cache_lastx;
	mutable float cache_lasty;
	mutable float cache_mx;
	mutable float cache_my;
protected:
	Atlantis* STS() const;
	
	
public:
	BasicVCComponent(Atlantis* _sts);
	BasicVCComponent(Atlantis* _sts, const string& _ident);
	virtual ~BasicVCComponent();

	
	//bool SetMouseRegion(VCMOUSEEVENTPOINT _a, VCMOUSEEVENTPOINT _b, VCMOUSEEVENTPOINT _c, VCMOUSEEVENTPOINT _d);

	/**
	 * Set the mouse region of the switch inside the area. 
	 * Only allows rectangular regions currently. 
	 * @param xmin The x coordinate of the left side
	 * @param xmax The x coordinate of the right side
	 * @param ymin The y coordinate of the top side
	 * @param ymax The y coordinate of the bottom side.
	 */
	bool SetMouseRegion(float xmin, float ymin, float xmax, float ymax);

	/**
	 * Get identification of this component
	 * @return The identification string, used for locating this component inside a panel
	 */
	const string& Ident() const;

	virtual void DefineAnimations(UINT vc_idx);
	virtual bool OnMouseEvent(int _event, float x, float y);
	virtual double GetState() const;
	virtual const VECTOR3& GetReference() const;
	virtual const VECTOR3& GetDirection() const;
	bool IsPointOver(float x, float y) const;
	bool ProjectMouse(float x, float y, float &mx, float &my) const;

	/**
	 * Set reference position
	 */
	
	void SetReference(const VECTOR3& _ref);
	/**
	 * Set reference position and direction
	 */
	void SetReference(const VECTOR3& _ref, const VECTOR3& dir);
	void SetDirection(const VECTOR3& _ref);
	
	
	virtual void Realize();
	virtual bool RegisterComponent(SubsystemComponent* pComponent);
	virtual bool DelComponent(SubsystemComponent* pComponent);
	virtual unsigned long CountComponents() const;
	virtual void OnPreStep(double fSimT, double fDeltaT, double fMJD);
	virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);
	virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD);
	virtual void OnPlaybackEvent(double fSimT, double fEventT, const char* event_t, const char* event);
	virtual bool OnParseLine(const char* line);
	virtual void OnSaveState(FILEHANDLE scn) const;
	virtual void OnDumpToLog() const;
	virtual const string& GetQualifiedIdentifier() const;
	virtual const string& GetIdentifier() const;
	virtual bool OnFail(const string& strFailureCode);
};

};

#endif // !defined(AFX_BASICVCCOMPONENT_H__B74A68D7_2919_4C3F_AB0C_4B4FBA3AA2B7__INCLUDED_)
