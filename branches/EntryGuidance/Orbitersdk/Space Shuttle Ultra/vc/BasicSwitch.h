/****************************************************************************
  This file is part of Space Shuttle Ultra

  Basic switch VC Component definition



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
#if !defined(AFX_BASICSWITCH_H__3790BCEE_A9B9_492A_A7F4_B0DA5E454AFB__INCLUDED_)
#define AFX_BASICSWITCH_H__3790BCEE_A9B9_492A_A7F4_B0DA5E454AFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtlantisVCComponent.h"
#include <vector>
#include <string>

namespace vc {

using namespace std;

class BasicSwitch : public AtlantisVCComponent  
{
protected:
	bool bOrientation;
	bool bSpringLoaded;
	unsigned short usNumPositions;
	vector<string> labels;
	vector<bool> vbSpringLoaded;
	unsigned short usCurrentPosition;
public:
	BasicSwitch(Atlantis* _sts, unsigned short usNumPositions, const string& _ident);
	virtual ~BasicSwitch();
	const string& GetLabel(int iPosition) const;
	void SetLabel(int iPosition, const string& _label);
	/**
	 * Sets initial position of switch
	 * Has to be called in DefineVC function
	 */
	void SetInitialPosition(unsigned short usInPos);
	void SetOrientation(bool bHorizontal);
	/**
	 * Fully spring loaded switch
	 */
	void SetSpringLoaded(bool IsSpringLoaded);
	/**
	 * Set a single switch position to be spring loaded
	 */
	void SetSpringLoaded(bool IsSpringLoaded, unsigned short usPos);

	virtual void DefineVCAnimations(UINT vc_idx);
	virtual bool GetStateString(unsigned long ulBufferSize, char* pszBuffer);

	virtual bool OnMouseEvent(int _event, float x, float y);

	virtual bool OnParseLine(const char* line);
	virtual void OnPositionUp();
	virtual void OnPositionDown();
	virtual void OnPositionChange(unsigned short usNewPosition);

	virtual void OnSaveState(FILEHANDLE scn) const;

};

};

#endif // !defined(AFX_BASICSWITCH_H__3790BCEE_A9B9_492A_A7F4_B0DA5E454AFB__INCLUDED_)
