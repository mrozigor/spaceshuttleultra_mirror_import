// AP101S.h: Schnittstelle für die Klasse AP101S.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AP101S_H__01BE9789_9E06_4F6B_9FAD_F48EA99C5161__INCLUDED_)
#define AFX_AP101S_H__01BE9789_9E06_4F6B_9FAD_F48EA99C5161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\AtlantisSubsystem.h"

namespace dps {

class AP101S : public AtlantisSubsystem  
{
	
	// ===================================================
	// Random access memory
	// Important variables are stored inside the lower 
	// 64 KB (SECTION 0) of the memory
	// ===================================================
	long lMemory[262144];
#if defined(DO_AP101S_EMULATION)
	// ===================================================
	// Emulation registers and memory
	// not needed as we don't do a emulation yet
	// ===================================================
	
	long lR[16];	//GP registers
	double fF[8];	//Floating Point registers
	long LX;	//Index register
	long lB;	//Base register
	long lPSW[2];
#else
	// ===================================================
	// Simulation variables
	// software modules and their scheduling
	// ===================================================
#endif
	// ===================================================
	// General submodules: IOP
	// ===================================================
	
	// IOP software is storing data in the memory and
	// transmits data half words from the memory over 
	// the MIAs. 
protected:
	void LoadMemoryConfiguration(const GPCMemoryConfiguration* pMemConfig);
	void LoadSoftware(const GPCSoftware* pSoftware);
public:
	AP101S(SubsystemDirector* _director, const string& _ident);
	virtual ~AP101S();
	
	

};

};

#endif // !defined(AFX_AP101S_H__01BE9789_9E06_4F6B_9FAD_F48EA99C5161__INCLUDED_)
