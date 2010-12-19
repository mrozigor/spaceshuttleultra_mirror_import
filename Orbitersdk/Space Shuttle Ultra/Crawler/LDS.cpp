#include "LDS.h"

LDS::LDS()
{
	
	//something here
}

LDS::~LDS()
{
	//something here
}

void LDS::ShowAlignmentError(double lateral)
{
	sprintf(oapiDebugString(),"%lf",lateral);
}

void LDS::CalculateDistanceBetweenAttachments(VECTOR3 MLP_ATTACH_GLOBAL, MATRIX3 RotationMatrix)
{
	bool found = false;
	OBJHANDLE hv = NULL;
	VESSEL *pv = NULL;
	std::string name;
	name.clear();

	for(DWORD i=0; i<oapiGetVesselCount(); i++)
	{
		hv = oapiGetVesselByIndex(i);
		pv = oapiGetVesselInterface(hv);
		name = pv->GetClassNameA();
		if(name.c_str() == "SSU_Pad")
		{
			oapiWriteLog((char*)name.c_str());
			oapiWriteLog("ZNALAZLEM PAD");
		}
		else
		{
			//COKOLWIEK
		}
	}
}
