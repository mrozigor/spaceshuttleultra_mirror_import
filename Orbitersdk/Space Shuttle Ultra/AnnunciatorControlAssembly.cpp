#include "AnnunciatorControlAssembly.h"
#include <orbitersdk.h>
#include "Atlantis.h"
#include "assert.h"


AnnunciatorControlAssembly::AnnunciatorControlAssembly( AtlantisSubsystemDirector* _director, const string& _ident, short nID ):AtlantisSubsystem( _director, _ident )
{
	assert( (nID >= 1) && (nID <= 5) && "AnnunciatorControlAssembly::AnnunciatorControlAssembly.nID" );
	ID = nID;
	return;
}

AnnunciatorControlAssembly::~AnnunciatorControlAssembly( void )
{
}

void AnnunciatorControlAssembly::Realize( void )
{
	char bundlename[8];
	int k = 0;

	sprintf_s( bundlename, 8, "ACA%d_1", ID );
	DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( bundlename, 16 );
	for (int i = 0; i <= 15; i += 2, k++)
	{
		cmd[k].Connect( pBundle, i );
		output[k].Connect( pBundle, i + 1 );
	}

	sprintf_s( bundlename, 8, "ACA%d_2", ID );
	pBundle = STS()->BundleManager()->CreateBundle( bundlename, 16 );
	for (int i = 0; i <= 15; i += 2, k++)
	{
		cmd[k].Connect( pBundle, i );
		output[k].Connect( pBundle, i + 1 );
	}

	sprintf_s( bundlename, 8, "ACA%d_3", ID );
	pBundle = STS()->BundleManager()->CreateBundle( bundlename, 16 );
	for (int i = 0; i <= 15; i += 2, k++)
	{
		cmd[k].Connect( pBundle, i );
		output[k].Connect( pBundle, i + 1 );
	}

	sprintf_s( bundlename, 8, "ACA%d_4", ID );
	pBundle = STS()->BundleManager()->CreateBundle( bundlename, 16 );
	for (int i = 0; i <= 15; i += 2, k++)
	{
		cmd[k].Connect( pBundle, i );
		output[k].Connect( pBundle, i + 1 );
	}

	sprintf_s( bundlename, 8, "ACA%d_5", ID );
	pBundle = STS()->BundleManager()->CreateBundle( bundlename, 16 );
	for (int i = 0; i <= 15; i += 2, k++)
	{
		cmd[k].Connect( pBundle, i );
		output[k].Connect( pBundle, i + 1 );
	}

	pBundle = STS()->BundleManager()->CreateBundle( "ACA", 16 );
	switch (ID)
	{
		case 1:
			power[0].Connect( pBundle, 0 );
			power[1].Connect( pBundle, 1 );
			odd[0].Connect( pBundle, 6 );
			even[0].Connect( pBundle, 7 );
			odd[1].Connect( pBundle, 8 );
			even[1].Connect( pBundle, 9 );
			break;
		case 2:
		case 3:
			power[0].Connect( pBundle, 2 );
			power[1].Connect( pBundle, 3 );
			odd[0].Connect( pBundle, 6 );
			even[0].Connect( pBundle, 7 );
			odd[1].Connect( pBundle, 8 );
			even[1].Connect( pBundle, 9 );
			break;
		case 4:
		case 5:
			power[0].Connect( pBundle, 4 );
			power[1].Connect( pBundle, 5 );
			odd[0].Connect( pBundle, 10 );
			even[0].Connect( pBundle, 11 );
			break;
	}
	return;
}

void AnnunciatorControlAssembly::OnPreStep( double fSimT, double fDeltaT, double fMJD )
{
	if (!power[0].IsSet() && !power[1].IsSet())
	{
		// all lights off
		for (int i = 0; i < 40; i++) output[i].ResetLine();
		return;
	}
	
	if (odd[0].IsSet() || odd[1].IsSet())
	{
		// odd lights on
		for (int i = 0; i < 40; i += 2) output[i].SetLine();
	}
	else
	{
		// check cmd for odd lights
		for (int i = 0; i < 40; i += 2) output[i].SetLine( (int)cmd[i].IsSet() * 5.0f );
	}

	if (even[0].IsSet() || even[1].IsSet())
	{
		// even lights on
		for (int i = 1; i < 40; i += 2) output[i].SetLine();
	}
	else
	{
		// check cmd for even lights
		for (int i = 1; i < 40; i += 2) output[i].SetLine( (int)cmd[i].IsSet() * 5.0f );
	}
	return;
}
