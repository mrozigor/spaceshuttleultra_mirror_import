// BasicSwitch.cpp: Implementierung der Klasse BasicSwitch.
//
//////////////////////////////////////////////////////////////////////

#include "BasicSwitch.h"
#include "../Atlantis.h"
#include <OrbiterSoundSDK40.h>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

namespace vc {

BasicSwitch::BasicSwitch(Atlantis* _sts, unsigned short usNumPositions, const string& _ident, int switchSoundID)
: AtlantisVCComponent(_sts, _ident), bOrientation(false), bSpringLoaded(false), usCurrentPosition(0),
  vbSpringLoaded(usNumPositions, false),
  switchSound(switchSoundID)
{
	this->usNumPositions = usNumPositions;
	labels.resize(usNumPositions);
	for(int i = 0; i<usNumPositions; i++) {
		labels[i] = "";
	}
}

BasicSwitch::~BasicSwitch()
{

}
void BasicSwitch::DefineVCAnimations(UINT vc_idx)
{
	
}

const string& BasicSwitch::GetLabel(int iPosition) const {
	return labels.at(iPosition);
}

bool BasicSwitch::GetStateString(unsigned long ulBufferSize, char* pszBuffer) {

	if(bSpringLoaded) return false; // no need to save state, position is always 1
	try {
		if(labels.at(usCurrentPosition).compare("")) {
			sprintf_s(pszBuffer, ulBufferSize, "%s", 
				labels.at(usCurrentPosition).c_str());
		} else {
			sprintf_s(pszBuffer, ulBufferSize, "[%d]", 
				usCurrentPosition);
		}
	}
	catch(...) {
		sprintf_s(pszBuffer, ulBufferSize, "Switch %s: exception thrown", GetIdentifier().c_str());
	}
	return true;
}

bool BasicSwitch::OnMouseEvent(int _event, float x, float y)
{
	/*
	sprintf_s(oapiDebugString(), 255, "BASICSWITCH %s [%d/%d] (%d, %f, %f)",
		GetQualifiedIdentifier().c_str(), 
		usCurrentPosition,
		usNumPositions,
		_event, x, y);

	*/

	// only process mouse down events (and mouse up events if this is a spring loaded switch)
	if(!(_event & PANEL_MOUSE_LBDOWN) && (!bSpringLoaded || !(_event & PANEL_MOUSE_LBUP)))
		return false;

	if(bOrientation) {
		if(x > 0.6) {
			if(_event & PANEL_MOUSE_LBDOWN) OnPositionDown();
			else if(vbSpringLoaded.at(usCurrentPosition)) {
				OnPositionUp();
			}
			return true;
		}
		else if(x < 0.4) {
			if(_event & PANEL_MOUSE_LBDOWN) OnPositionUp();
			else if(vbSpringLoaded.at(usCurrentPosition)) {
				OnPositionDown();
			}
			return true;
		}
	}
	else {
		if(y > 0.6) {
			if(_event & PANEL_MOUSE_LBDOWN) OnPositionDown();
			else if(vbSpringLoaded.at(usCurrentPosition)) {
				OnPositionUp();
			}
			return true;
		}
		else if( y < 0.4) {
			if(_event & PANEL_MOUSE_LBDOWN) OnPositionUp();
			else if(vbSpringLoaded.at(usCurrentPosition)) {
				OnPositionDown();
			}
			return true;
		}
	}
	return false;
}

bool BasicSwitch::OnParseLine(const char* line) {
	//
	char pszBuffer[256];
	sprintf_s(pszBuffer, 255, "\t\tSet switch \"%s\" to state \"%s\".",
		GetQualifiedIdentifier().c_str(), line);
	if(line[0] == '[') {
		usCurrentPosition = atoi(line+1);
		OnPositionChange(usCurrentPosition);
		return true;
	} else {
		for(unsigned short i = 0; i<usNumPositions; i++) {
			if(labels.at(i) == line) 
			{
				usCurrentPosition = i;
				OnPositionChange(usCurrentPosition);
				return true;
			}
		}
	}
	return false;
}

void BasicSwitch::OnPositionChange(unsigned short usNewPosition)
{
	if(switchSound > 0) PlayVesselWave(STS()->GetSoundID(), switchSound);
}

void BasicSwitch::OnPositionUp()
{
	if(usCurrentPosition < usNumPositions - 1)
	{
		usCurrentPosition++;
		OnPositionChange(usCurrentPosition);
	}
}

void BasicSwitch::OnPositionDown()
{
	if(usCurrentPosition > 0)
	{
		usCurrentPosition--;
		OnPositionChange(usCurrentPosition);
	}
}



void BasicSwitch::OnSaveState(FILEHANDLE scn) const
{
	
}

void BasicSwitch::SetLabel(int iPosition, const string& _label) {
	labels.at(iPosition) = _label;
}

void BasicSwitch::SetInitialPosition(unsigned short usPos)
{
	usCurrentPosition=usPos;
}

void BasicSwitch::SetOrientation(bool bHorizontal)
{
	bOrientation = bHorizontal;
}

void BasicSwitch::SetSpringLoaded(bool IsSpringLoaded, unsigned short usPos)
{
	if(usPos>=usNumPositions) return;

	bSpringLoaded = bSpringLoaded || IsSpringLoaded;
	vbSpringLoaded.at(usPos) = IsSpringLoaded;
}

void BasicSwitch::SetSpringLoaded(bool IsSpringLoaded)
{
	unsigned short usMidPosition = (usNumPositions - 1)/2;
	//bSpringLoaded=IsSpringLoaded;
	for(unsigned short i=0;i<usNumPositions;i++) {
		SetSpringLoaded(IsSpringLoaded && (i!=usMidPosition), i);
	}
}

};
