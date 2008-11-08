// AP101S.h: Schnittstelle für die Klasse AP101S.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AP101S_H__01BE9789_9E06_4F6B_9FAD_F48EA99C5161__INCLUDED_)
#define AFX_AP101S_H__01BE9789_9E06_4F6B_9FAD_F48EA99C5161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\AtlantisSubsystem.h"
#include "GPC.h"

namespace dps {

	class GPCMemoryConfiguration;
	class GPCSoftware;

	class AP101S : public GPC  
	{
	
	// ===================================================
	// Random access memory
	// Important variables are stored inside the lower 
	// 64 KB (SECTION 0) of the memory
	// ===================================================
		long lMemory[262144];
	// ===================================================
	// General submodules: IOP
	// ===================================================
	
	// IOP software is storing data in the memory and
	// transmits data half words from the memory over 
	// the MIAs. 
	protected:
		virtual void GeneratePROMData();
		//void LoadMemoryConfiguration(const GPCMemoryConfiguration* pMemConfig);
		//void LoadSoftware(const GPCSoftware* pSoftware);
	public:
		AP101S(SubsystemDirector* _director, const string& _ident, unsigned short usGPCID);
		virtual ~AP101S();
	};

};

#endif // !defined(AFX_AP101S_H__01BE9789_9E06_4F6B_9FAD_F48EA99C5161__INCLUDED_)
