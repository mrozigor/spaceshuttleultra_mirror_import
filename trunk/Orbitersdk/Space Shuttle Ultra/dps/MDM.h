// MDM.h: Schnittstelle für die Klasse MDM.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDM_H__BC1B13B9_53D9_4DC9_A6D6_2E7EA9F155A6__INCLUDED_)
#define AFX_MDM_H__BC1B13B9_53D9_4DC9_A6D6_2E7EA9F155A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../AtlantisSubsystem.h"

namespace dps {
class MDMIOModule;

/**
 * Possible modules per MDM (maximal 16 modules):
 *    
 * Discrete output high (DOH), 28 volts, three l6-bit channels
 * Discrete output low (DOL), 5 volts, three l6-bit channels
 * Discrete input high (DIH), 28 volts, three l6-bit channels
 * Discrete input low (DIL), 5 volts, three l6-bit channels
 * Analog output differential (AOD), 16 channels
 * Analog input differential (AID), 16 channels
 * Analog input single-ended (AIS), 32 channels
 * Serial input/output (SIO), four channels
 * Tacan/radar altimeter (special-purpose I/O)
 * 
 * FF MDM:
 * Modules 5  and 13 connect to the RJDF - likely DOH/DIL modules
 *
 */
class MDM : public AtlantisSubsystem  
{
protected:
	short SCU_PROM[2][16384];
	MDMIOModule* pmodules[16];
public:
	MDM(SubsystemDirector* _director, const string& _ident);
	virtual ~MDM();

};

};

#endif // !defined(AFX_MDM_H__BC1B13B9_53D9_4DC9_A6D6_2E7EA9F155A6__INCLUDED_)
