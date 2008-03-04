#include "Atlantis.h"
#include "CommModeHandler.h"

CommModeHandler::CommModeHandler(Atlantis* _pSTS)
: pSTS(_pSTS)
{
	bCommMode = false;
	iMenu = 0;
	fRecDisplayT0 = 0.0;
}

void CommModeHandler::DefineAnnotations()
{
	nhLastReceived = oapiCreateAnnotation(true, 1.5, _V(1.0, 1.0, 1.0));

	oapiAnnotationSetPos(nhLastReceived, 0.05, 0.05, 0.75, 0.1);

	oapiAnnotationSetText(nhLastReceived, "[MCC]:Atlantis, Houston, go for launch.");

	nhOptionPos = oapiCreateAnnotation(true, 1.0, _V(0.3, 1.0, 0.3));

	oapiAnnotationSetPos(nhOptionPos, 0.35, 0.15, 0.55, 0.2);

}

bool CommModeHandler::ConsumeBufferedKey(DWORD key, bool pressed, char* keymap)
{
	if(KEYMOD_SHIFT(keymap))
		return false;

	if(KEYMOD_CONTROL(keymap))
	{
	}
	else if(KEYMOD_ALT(keymap))
	{
	} else 
	{
		switch(key)
		{
		case OAPI_KEY_TAB:
			LeaveCommMode();
			return true;
		}
	}
	
	return false;
}

void CommModeHandler::PostStep(double fSimT, double fDeltaT)
{
	
	if(fRecDisplayT0 + 30.0 < fSimT)
	{
		BlankReceiveAnnotation();
	}
}

bool CommModeHandler::IsInCommMode() const
{
	return bCommMode;
}

void CommModeHandler::EnterCommMode()
{
	bCommMode = true;

	//Display all options for replies

	oapiAnnotationSetText(nhOptionPos, "Copy,...");

	fRecDisplayT0 = oapiGetSimTime();
}


void CommModeHandler::LeaveCommMode()
{
	bCommMode = false;
	fSendDisplayT0 = 0.0;
	strCompA2G = "";

	BlankSendAnnotations();
}

void CommModeHandler::BlankSendAnnotations()
{
	oapiAnnotationSetText(nhOptionPos, NULL);
}

void CommModeHandler::BlankReceiveAnnotation()
{
	oapiAnnotationSetText(nhLastReceived, NULL);	
}

void CommModeHandler::OnReceiveFocus()
{
	if(fRecDisplayT0 + 30.0 > oapiGetSimTime())
	{
		//DecompressLastRecMessage();
	}
}

void CommModeHandler::OnLooseFocus()
{
	BlankReceiveAnnotation();
	LeaveCommMode();
}