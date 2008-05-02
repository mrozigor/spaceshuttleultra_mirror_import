// DiscreteBundle.h: Schnittstelle für die Klasse DiscreteBundle.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISCRETEBUNDLE_H__A928D10B_004B_495D_87CD_3A0309B3EFDA__INCLUDED_)
#define AFX_DISCRETEBUNDLE_H__A928D10B_004B_495D_87CD_3A0309B3EFDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

namespace discsignals
{
	using namespace std;

	class DiscretePort;

/**
 * class for handling multiple discrete data lines or analog inputs
 * State for each discrete can be -28V to +28V
 * One bundle can have up to 16 lines. 
 * one line has a beginning and a end point.
 * 0 = ground (GND)
 */
class DiscreteBundle  
{
	string ident;
	short snLines;
	float fState;
	float fLines[16];
	/**
	 * Indicate that a line is broken and does always return 0V. 
	 */
	bool bBrokenLine[16];
public:
	DiscreteBundle(const string& _ident, int nLines = 16);
	virtual ~DiscreteBundle();

	/**
	 * Set the mode how the Discrete bundle class handles logging information
	 */
	void SetLogMode(int iMode);
	float GetVoltage(int iIndex);
	const string& GetIdentity() const;
	void SetDiscrete(int iIndex, float fVoltage);
};

};

#endif // !defined(AFX_DISCRETEBUNDLE_H__A928D10B_004B_495D_87CD_3A0309B3EFDA__INCLUDED_)
