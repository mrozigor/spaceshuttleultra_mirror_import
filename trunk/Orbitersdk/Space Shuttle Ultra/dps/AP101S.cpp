// AP101S.cpp: Implementierung der Klasse AP101S.
//
//////////////////////////////////////////////////////////////////////

#include "AP101S.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

namespace dps {

AP101S::AP101S(AtlantisSubsystemDirector* _director, const string& _ident, unsigned short usGPCID)
:GPC(_director, _ident, usGPCID)
{

}

AP101S::~AP101S()
{

}

void AP101S::GeneratePROMData() {

}

};