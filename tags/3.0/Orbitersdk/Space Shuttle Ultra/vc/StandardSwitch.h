/****************************************************************************
  This file is part of Space Shuttle Ultra

  Standard Switch VC Component



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
#if !defined(AFX_STANDARDSWITCH_H__A87D233D_E5CF_4343_90D8_BF65F16C4DB4__INCLUDED_)
#define AFX_STANDARDSWITCH_H__A87D233D_E5CF_4343_90D8_BF65F16C4DB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DiscreteBundle.h"
#include "DiscOutPort.h"
#include "DiscInPort.h"
#include "BasicSwitch.h"

namespace vc {

	using ::discsignals::DiscreteBundle;
	using ::discsignals::DiscOutPort;
	using ::discsignals::DiscInPort;

	class LockableLever: public BasicSwitch 
	{
		UINT grpIndex;
		MGROUP_ROTATE* pswitchrot;
		MGROUP_TRANSLATE* pswitchpull;
		UINT anim_switch;
		UINT anim_pull;
		VECTOR3 dummy_vec; // needed to create animation

		VECTOR3 pullDir;
		bool bHasPullDir;

		bool bIsPulled;
		double fInitialMousePos;
		DiscInPort input;
	public:
		LockableLever(Atlantis* psts, unsigned short usNumPositions, 
			const string& _ident);
		virtual ~LockableLever();

		void DefineSwitchGroup(UINT _grpIndex);
		/**
		 * @param _pullDir Normalized vector in direction along which lever can be pulled
		 */
		//void SetReference(const VECTOR3& _ref, const VECTOR3& _dir, const VECTOR3& _pullDir);
		void SetPullDirection(VECTOR3 _pullDir);

		virtual void Realize();
		virtual void DefineVCAnimations(UINT vc_idx);
		virtual bool OnMouseEvent(int _event, float x, float y);
		virtual bool ConnectPort(unsigned short usPort, DiscreteBundle* pBundle, unsigned short usLine);
		virtual void OnPositionChange(unsigned short usNewPosition);
		virtual void OnPull();
		virtual void OnRelease();
	};

	class LockableLever2 : public LockableLever
	{
		DiscOutPort output;
	public:
		LockableLever2(Atlantis* psts, const string& _ident);
		virtual ~LockableLever2();

		virtual void OnPositionChange(unsigned short usNewPosition);
		virtual bool ConnectPort(unsigned short usPort, DiscreteBundle* pBundle, unsigned short usLine);
	};

	class LockableLever3 : public LockableLever
	{
	private:
		DiscOutPort outputA, outputB;
		unsigned short usA, usB;
	public:
		LockableLever3(Atlantis* psts, const string& _ident);
		virtual ~LockableLever3();

		virtual void OnPositionChange(unsigned short usNewPosition);
		virtual bool ConnectPort(unsigned short usPort, DiscreteBundle* pBundle, unsigned short usLine);

		bool ConnectSwitchPosition(unsigned short usPos, unsigned short usPort);
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
	unsigned short usOnPosition; // position at which output DiscPort is set
public:
	StdSwitch2(Atlantis* _sts, const string& _ident);
	virtual ~StdSwitch2();
	virtual void OnPositionChange(unsigned short usNewPosition);
	virtual bool ConnectPort(unsigned short usPort, DiscreteBundle* pBundle, unsigned short usLine);

	void SetOnPosition(unsigned short _usPos);

	DiscOutPort output;
	
};

class StdSwitch3: public StandardSwitch
{
protected:
	unsigned short usA, usB; // switch positions where ports A and B are set
public:
	StdSwitch3(Atlantis* _sts, const string& _ident);
	virtual ~StdSwitch3();
	virtual void OnPositionChange(unsigned short usNewPosition);
	virtual bool ConnectPort(unsigned short usPort, DiscreteBundle* pBundle, unsigned short usLine);
	/**
	 * Sets switch position corresponding to particular port
	 * \param usPos switch position (0-2)
	 * \param usPort port (1-2)
	 */
	bool ConnectSwitchPosition(unsigned short usPos, unsigned short usPort);

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