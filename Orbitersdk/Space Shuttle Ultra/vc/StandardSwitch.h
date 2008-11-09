// StandardSwitch.h: Schnittstelle für die Klasse StandardSwitch.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STANDARDSWITCH_H__A87D233D_E5CF_4343_90D8_BF65F16C4DB4__INCLUDED_)
#define AFX_STANDARDSWITCH_H__A87D233D_E5CF_4343_90D8_BF65F16C4DB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../discsignals/DiscreteBundle.h"
#include "../discsignals/DiscOutPort.h"
#include "../discsignals/DiscInPort.h"
#include "BasicSwitch.h"

namespace vc {

	using class ::discsignals::DiscreteBundle;
	using class ::discsignals::DiscOutPort;
	using class ::discsignals::DiscInPort;

	class LockableLever: public BasicSwitch 
	{
		UINT grpIndex;
		MGROUP_ROTATE* pswitchrot;
		MGROUP_TRANSLATE* pswitchpull;
		bool bIsPulled;
		double fInitialMousePos;
		UINT anim_switch;
		DiscInPort input;
	public:
		LockableLever(Atlantis* psts, unsigned short usNumPositions, 
			const string& _ident);
		virtual ~LockableLever();

	};

	class StandardSwitch : public BasicSwitch  
	{
		UINT grpIndex;
		MGROUP_ROTATE* pswitchrot;

		UINT anim_switch;	
		DiscInPort input;
	public:
		StandardSwitch(Atlantis* psts, unsigned short usNumPositions, const string& _ident);
		virtual ~StandardSwitch();
		void DefineSwitchGroup(UINT _grpIndex);
		virtual void DefineVCAnimations(UINT vc_idx);
		virtual void OnPositionChange(unsigned short usNewPosition);
		virtual void Realize();
		virtual bool ConnectPort(unsigned short usPort, DiscreteBundle* pBundle, unsigned short usLine);
	};

class StdSwitch2: public StandardSwitch
{
protected:
public:
	StdSwitch2(Atlantis* _sts, const string& _ident);
	virtual ~StdSwitch2();
	virtual void OnPositionChange(unsigned short usNewPosition);
	virtual bool ConnectPort(unsigned short usPort, DiscreteBundle* pBundle, unsigned short usLine);

	DiscOutPort output;
	
};

class StdSwitch3: public StandardSwitch
{
protected:
	
public:
	StdSwitch3(Atlantis* _sts, const string& _ident);
	virtual ~StdSwitch3();
	virtual void OnPositionChange(unsigned short usNewPosition);
	virtual bool ConnectPort(unsigned short usPort, DiscreteBundle* pBundle, unsigned short usLine);

	DiscOutPort outputA;	//Connection Position A
	DiscOutPort outputB;	//Connection Position B
};

class RussianCB: public StandardSwitch
{
protected:
	//Connect to Power bus
public:
	RussianCB(Atlantis* _sts, const string& _ident);
	virtual ~RussianCB();
	
	virtual void DefineVCAnimations(UINT vc_idx);
};


};

#endif // !defined(AFX_STANDARDSWITCH_H__A87D233D_E5CF_4343_90D8_BF65F16C4DB4__INCLUDED_)
