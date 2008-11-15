#include "StandardLight.h"

namespace vc {

	StandardLight::StandardLight(Atlantis* _sts, const string& _ident)
		:BasicLight(_sts, _ident)
	{
		sourceOn.x = 0;
		sourceOn.y = 0;
		sourceOff.x = 0;
		sourceOff.y = 0;
		usWidth = 1;
		usHeight = 1;
		bHasSourceImage = false;
		bOldState = false;
		bCurrentState = false;
	}

	StandardLight::~StandardLight() {
	}

	
	void StandardLight::OnPostStep(double fSimT, double fDeltaT, double fMJD)
	{
		bCurrentState = input.IsSet() || test.IsSet();
		if(bCurrentState != bOldState) {

			//oapiTriggerRedrawArea(-1, -1, AID_ODSLIGHTS);
			TriggerRedraw();
			bOldState = bCurrentState;
		}
	}

	bool StandardLight::OnVCRedrawEvent(int id, int _event, SURFHANDLE surf)
	{
		char pszBuffer[256];
		if(!bHasSourceImage) {
			return false;
		}
		
		if(bCurrentState) {
			//oapiWriteLog("StandardLight::OnVCRedrawEvent::On");
			//oapiColourFill(surf, 0xFF0000);
			oapiBlt(surf, shSource, redrawBase.x, redrawBase.y, 
				sourceOn.x, sourceOn.y, usWidth, usHeight);
			/*sprintf_s(pszBuffer, 255, "\toapiBlt(%08X, %08X, %d, %d, %d, %d, %d, %d)",
				surf, shSource, redrawBase.x, redrawBase.y, 
				sourceOn.x, sourceOn.y, usWidth, usHeight);
			oapiWriteLog(pszBuffer);*/
		} else {
			//oapiWriteLog("StandardLight::OnVCRedrawEvent::Off");
			//oapiColourFill(surf, 0x000000);
			oapiBlt(surf, shSource, redrawBase.x, redrawBase.y, 
				sourceOff.x, sourceOff.y, usWidth, usHeight);
			/*sprintf_s(pszBuffer, 255, "\toapiBlt(%08X, %08X, %d, %d, %d, %d, %d, %d)",
				surf, shSource, redrawBase.x, redrawBase.y, 
				sourceOff.x, sourceOff.y, usWidth, usHeight);
			oapiWriteLog(pszBuffer);*/
		}
		return true;
	}

	void StandardLight::SetSourceCoords(bool fState, unsigned short usX, unsigned short usY) {
		if(fState) {
			sourceOn.x = usX;
			sourceOn.y = usY;
		} else {
			sourceOff.x = usX;
			sourceOff.y = usY;
		}
	}

	void StandardLight::SetSourceImage(SURFHANDLE _shSource) {
		shSource = _shSource;
		if(_shSource != NULL) 
			bHasSourceImage = true;
		else
			bHasSourceImage = false;
	}

	void StandardLight::SetDimensions(unsigned short usWidth, unsigned short usHeight) {
		this->usWidth = usWidth;
		this->usHeight = usHeight;
	}


};