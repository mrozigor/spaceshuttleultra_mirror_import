#include "StandardLight.h"
#include "../Atlantis.h"
#include <assert.h>


namespace vc
{
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
		//char pszBuffer[256];
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

	bool StandardLight::Connect(unsigned short usPort, discsignals::DiscreteBundle* pBundle, unsigned short usLine)
	{
		if(usPort == 1) test.Connect(pBundle, usLine);
		else input.Connect(pBundle, usLine);
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

	///////////////////////////////////////

	StandardSingleLight::StandardSingleLight( Atlantis* _sts, const string& _ident ):BasicLight_2( _sts, _ident )
	{
		for (int i = 0; i < 2; i++)
		{
			offsetU[i] = 0.0;
			offsetV[i] = 0.0;
		}
	}

	StandardSingleLight::~StandardSingleLight()
	{
	}

	void StandardSingleLight::SetDefaultState( int _state )
	{
		assert( (_state >= 0) && (_state < 2) );
		default_state = _state;
		state = _state;
		return;
	}

	void StandardSingleLight::SetStateOffset( int _state, float _U, float _V )
	{
		assert( (_state >= 0) && (_state < 2) );
		offsetU[_state] = _U;
		offsetV[_state] = _V;
		return;
	}
	
	bool StandardSingleLight::ConnectLight( unsigned short usPort, DiscreteBundle* pBundle, unsigned short usLine )
	{
		assert( (usPort >= 0) && (usPort < 2) );
		input[usPort].Connect( pBundle, usLine );
		return true;
	}

	void StandardSingleLight::OnPostStep( double SimT, double DeltaT, double MJD )
	{
		unsigned short tmp;

		if (input[0].IsSet() || input[1].IsSet()) tmp = 1;
		else tmp = 0;

		if (tmp != state)
		{
			next_state = tmp;
			UpdateUV();
		}
		return;
	}

	void StandardSingleLight::UpdateUV( void )
	{
		if (STS()->vis == 0) return;
		MESHHANDLE panelTemplate = STS()->GetMeshTemplate( panelmesh );
		MESHGROUPEX* mg = oapiMeshGroupEx( panelTemplate, grpIndex );
		DEVMESHHANDLE hDevpanelmesh = STS()->GetDevMesh( STS()->vis, panelmesh );

		static NTVERTEX Vtx[32];
		for (unsigned short i = 0; i < mg->nVtx; i++)
		{
			Vtx[i].tu = mg->Vtx[i].tu + (offsetU[next_state] - offsetU[default_state]);
			Vtx[i].tv = mg->Vtx[i].tv + (offsetV[next_state] - offsetV[default_state]);
		}
		state = next_state;

		GROUPEDITSPEC grpSpec;
		grpSpec.flags = GRPEDIT_VTXTEX;
		grpSpec.Vtx = Vtx;
		grpSpec.nVtx = mg->nVtx;
		grpSpec.vIdx = NULL;

		oapiEditMeshGroup( hDevpanelmesh, grpIndex, &grpSpec );
		return;
	}

	void StandardSingleLight::DefineMeshGroup( UINT _panelmesh, UINT _grpIndex )
	{
		panelmesh = _panelmesh;
		grpIndex = _grpIndex;
		return;
	}

	void StandardSingleLight::UpdateUVState( void )
	{
		state = default_state;// return state to default
		UpdateUV();
		return;
	}

	StandardDoubleLight::StandardDoubleLight( Atlantis* _sts, const string& _ident ):BasicLight_2( _sts, _ident )
	{
		for (int i = 0; i < 4; i++)
		{
			offsetU[i] = 0.0;
			offsetV[i] = 0.0;
		}
	}

	StandardDoubleLight::~StandardDoubleLight()
	{
	}

	void StandardDoubleLight::SetDefaultState( int _state )
	{
		assert( (_state >= 0) && (_state < 4) );
		default_state = _state;
		state = _state;
		return;
	}

	void StandardDoubleLight::SetStateOffset( int _state, float _U, float _V )
	{
		assert( (_state >= 0) && (_state < 4) );
		offsetU[_state] = _U;
		offsetV[_state] = _V;
		return;
	}
	
	bool StandardDoubleLight::ConnectLight( unsigned short usPort, DiscreteBundle* pBundle, unsigned short usLine )
	{
		assert( (usPort >= 0) && (usPort < 4) );
		input[usPort].Connect( pBundle, usLine );
		return true;
	}

	void StandardDoubleLight::OnPostStep( double SimT, double DeltaT, double MJD )
	{
		unsigned short tmp;

		if (input[0].IsSet() || input[1].IsSet())
		{
			if (input[2].IsSet() || input[3].IsSet()) tmp = 3;
			else tmp = 1;
		}
		else
		{
			if (input[2].IsSet() || input[3].IsSet()) tmp = 2;
			else tmp = 0;
		}

		if (tmp != state)
		{
			next_state = tmp;
			UpdateUV();
		}
		return;
	}

	void StandardDoubleLight::UpdateUV( void )
	{
		if (STS()->vis == 0) return;
		MESHHANDLE panelTemplate = STS()->GetMeshTemplate( panelmesh );
		MESHGROUPEX* mg = oapiMeshGroupEx( panelTemplate, grpIndex );
		DEVMESHHANDLE hDevpanelmesh = STS()->GetDevMesh( STS()->vis, panelmesh );
		
		static NTVERTEX Vtx[32];
		for (unsigned short i = 0; i < mg->nVtx; i++)
		{
			Vtx[i].tu = mg->Vtx[i].tu + (offsetU[next_state] - offsetU[default_state]);
			Vtx[i].tv = mg->Vtx[i].tv + (offsetV[next_state] - offsetV[default_state]);
		}
		state = next_state;

		GROUPEDITSPEC grpSpec;
		grpSpec.flags = GRPEDIT_VTXTEX;
		grpSpec.Vtx = Vtx;
		grpSpec.nVtx = mg->nVtx;
		grpSpec.vIdx = NULL;

		oapiEditMeshGroup( hDevpanelmesh, grpIndex, &grpSpec );
		return;
	}

	void StandardDoubleLight::DefineMeshGroup( UINT _panelmesh, UINT _grpIndex )
	{
		panelmesh = _panelmesh;
		grpIndex = _grpIndex;
		return;
	}

	void StandardDoubleLight::UpdateUVState( void )
	{
		state = default_state;// return state to default
		UpdateUV();
		return;
	}
}
