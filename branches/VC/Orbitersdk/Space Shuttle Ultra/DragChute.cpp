#include "DragChute.h"
#include "Atlantis.h"
#include "Atlantis_defs.h"
#include "ParameterValues.h"
#include "CommonDefs.h"
#include <UltraMath.h>


DragChute::DragChute( AtlantisSubsystemDirector* _director ):AtlantisSubsystem( _director, "DragChute" )
{
	mesh_index = MESH_UNDEFINED;
	hMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_CHUTE );

	DragChuteState = STOWED;
	DragChuteSize = 0.0;
	as_spin.Set( AnimState::OPENING, 0.0 );
	return;
}

DragChute::~DragChute()
{
	return;
}

void DragChute::Realize()
{
	DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "DRAG_CHUTE", 16 );
	DragChuteARM[0].Connect( pBundle, 0 );
	DragChuteARM[1].Connect( pBundle, 1 );
	DragChuteDPY[0].Connect( pBundle, 2 );
	DragChuteDPY[1].Connect( pBundle, 3 );
	DragChuteJETT[0].Connect( pBundle, 4 );
	DragChuteJETT[1].Connect( pBundle, 5 );

	pBundle = STS()->BundleManager()->CreateBundle( "ACA2_1", 16 );
	DragChuteDPY2LT[1].Connect( pBundle, 6 );
	DragChuteARM2LT[1].Connect( pBundle, 10 );
	DragChuteJETT2LT[1].Connect( pBundle, 14 );
	
	pBundle = STS()->BundleManager()->CreateBundle( "ACA2_2", 16 );
	DragChuteARM2LT[0].Connect( pBundle, 4 );
	DragChuteDPY2LT[0].Connect( pBundle, 8 );
	DragChuteJETT2LT[0].Connect( pBundle, 12 );

	pBundle = STS()->BundleManager()->CreateBundle( "ACA3_1", 16 );
	DragChuteJETT1LT[0].Connect( pBundle, 4 );
	DragChuteDPY1LT[0].Connect( pBundle, 8 );
	DragChuteARM1LT[0].Connect( pBundle, 12 );

	pBundle = STS()->BundleManager()->CreateBundle( "ACA3_5", 16 );
	DragChuteARM1LT[1].Connect( pBundle, 2 );
	DragChuteJETT1LT[1].Connect( pBundle, 10 );
	DragChuteDPY1LT[1].Connect( pBundle, 14 );

	DefineAnimations();
	return;
}

void DragChute::OnPreStep( double SimT, double DeltaT, double MJD )
{
	double airspeed = STS()->GetAirspeed();
	double groundspeed = STS()->GetGroundspeed();

	switch (DragChuteState)
	{
		case STOWED:
			if ((((airspeed <= CHUTE_DEPLOY_SPEED) && (airspeed > CHUTE_JETTISON_SPEED) && STS()->GroundContact()) ||
				DragChuteARM[0].IsSet() || DragChuteARM[1].IsSet() || DragChuteDPY[0].IsSet() || DragChuteDPY[1].IsSet()))
			{
				STS()->SetAnimation( anim_deploy, 1.0 );
				STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_EXTERNAL );

				DragChuteSize = 0.0;
				DragChuteDeployTime = SimT;

				DragChuteARM1LT[0].SetLine();
				DragChuteARM1LT[1].SetLine();
				DragChuteARM2LT[0].SetLine();
				DragChuteARM2LT[1].SetLine();
				DragChuteDPY1LT[0].SetLine();
				DragChuteDPY1LT[1].SetLine();
				DragChuteDPY2LT[0].SetLine();
				DragChuteDPY2LT[1].SetLine();

				DragChuteState = DEPLOYING;
			}
			break;
		case DEPLOYING:
			if ((SimT - DragChuteDeployTime) > CHUTE_DEPLOY_TIME)
			{
				DragChuteSize = min( 0.4, DragChuteSize + CHUTE_DEPLOY_RATE * DeltaT );
				STS()->SetAnimation( anim_deploy, 1 - DragChuteSize );
				SpinChute( DeltaT );

				if (Eq( DragChuteSize, 0.4, 0.001 )) DragChuteState = REEFED;
			}
			break;
		case REEFED:
			SpinChute( DeltaT );
			if ((SimT - DragChuteDeployTime) > CHUTE_INFLATE_TIME) DragChuteState = INFLATED;
			break;
		case INFLATED:
			if ((groundspeed < CHUTE_JETTISON_SPEED) || (airspeed > CHUTE_FAIL_SPEED) || DragChuteJETT[0].IsSet() || DragChuteJETT[1].IsSet())
			{
				STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_NEVER );

				DragChuteState = JETTISONED;
				DragChuteSize = 0.0;

				//add chute vessel
				VESSELSTATUS2 vs;
				memset( &vs, 0, sizeof( vs ) );
				vs.version = 2;
				STS()->GetStatusEx( &vs );
				vs.status = 0;
				VECTOR3 chute_ofs;
				STS()->Local2Rel( STS()->GetOrbiterCoGOffset() + CHUTE_ATTACH_POINT, chute_ofs );
				vs.rpos = chute_ofs;
				char name[255];
				strcpy_s( name, STS()->GetName() );
				strcat_s( name, "-Chute" );
				oapiCreateVesselEx( name, "SSU_Chute", &vs );

				// HACK the light shouldn't turn on when drag chute fails, but only when the JETT PBI is used
				DragChuteJETT1LT[0].SetLine();
				DragChuteJETT1LT[1].SetLine();
				DragChuteJETT2LT[0].SetLine();
				DragChuteJETT2LT[1].SetLine();
			}
			else
			{
				if (DragChuteSize < 1.0)
				{
					DragChuteSize = min( 1.0, DragChuteSize + CHUTE_INFLATE_RATE * DeltaT );
					STS()->SetAnimation( anim_deploy, 1 - DragChuteSize );
				}
				SpinChute( DeltaT );
			}
			break;
		/*case JETTISONED:
			break;*/
	}
	return;
}

bool DragChute::OnParseLine( const char* line )
{
	if (!_strnicmp( line, "JETTISONED", 10 ))
	{
		DragChuteState = JETTISONED;
		DragChuteSize = 0.0;
		DragChuteARM1LT[0].SetLine();
		DragChuteARM1LT[1].SetLine();
		DragChuteARM2LT[0].SetLine();
		DragChuteARM2LT[1].SetLine();
		DragChuteDPY1LT[0].SetLine();
		DragChuteDPY1LT[1].SetLine();
		DragChuteDPY2LT[0].SetLine();
		DragChuteDPY2LT[1].SetLine();
		DragChuteJETT1LT[0].SetLine();
		DragChuteJETT1LT[1].SetLine();
		DragChuteJETT2LT[0].SetLine();
		DragChuteJETT2LT[1].SetLine();
		return true;
	}
	else return false;
}

void DragChute::OnSaveState( FILEHANDLE scn ) const
{
	if (DragChuteState != STOWED) oapiWriteLine( scn, "JETTISONED" );// if deployed, assume jettisoned
	return;
}

void DragChute::AddMesh()
{
	VECTOR3 chute_ofs = CHUTE_ATTACH_POINT;
	mesh_index = STS()->AddMesh( hMesh, &chute_ofs );
	STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_NEVER );
	return;
}

void DragChute::DefineAnimations()
{
	ANIMATIONCOMPONENT_HANDLE parent;
	
	static UINT ChuteCords[1] = { 0 };
	static MGROUP_SCALE ScaleCords( mesh_index, ChuteCords, 1, _V( 0.0, 0.0, -11.53 ), _V( 0.5, 0.5, 1.1 ) );
	static UINT Chute[3] = { 1, 2, 3 };
	static MGROUP_SCALE ScaleChute( mesh_index, Chute, 3, _V( 0.0, 0.0, -29.03 ), _V( 0.5, 0.5, 1.2 ) );
	static UINT DragChute[4] = { 0, 1, 2, 3 };
	static MGROUP_SCALE ScaleAll( mesh_index, DragChute, 4, _V( 0.0, 0.0, 0.0 ), _V( 0.01, 0.01, 0.01 ) );
	anim_deploy = STS()->CreateAnimation( 0.0 );
	
	STS()->AddAnimationComponent( anim_deploy, 0.0, 0.6, &ScaleCords );
	STS()->AddAnimationComponent( anim_deploy, 0.0, 0.6, &ScaleChute );
	parent = STS()->AddAnimationComponent( anim_deploy, 0.6, 0.9, &ScaleAll );
	anim_spin = STS()->CreateAnimation( 0.0 );
	
	static MGROUP_ROTATE SpinChute1( mesh_index, DragChute, 4,
		_V( 0, 0, 0 ), _V( 0.00609621, -0.0146035, -0.999875 ), (float)(400.0 * RAD) );
	STS()->AddAnimationComponent( anim_spin, 0.0, 0.5, &SpinChute1, parent );
	static MGROUP_ROTATE SpinChute2( mesh_index, DragChute, 4,
		_V( 0, 0, 0 ), _V(-0.00965286, -0.00698363, -0.999929), (float)(400.0 * RAD) );
	STS()->AddAnimationComponent( anim_spin, 0.5, 1.0, &SpinChute2, parent );
	return;
}

void DragChute::AddAero( void ) const
{
	STS()->CreateVariableDragElement( &DragChuteSize, 22.5, _V( 0, 4.6, -12.03 ) );
	return;
}

void DragChute::SpinChute( double DeltaT )
{
	if (as_spin.Opening())
	{
		as_spin.pos = min( 1.0, as_spin.pos + CHUTE_SPIN_RATE * DeltaT );
		if (Eq( as_spin.pos, 1.0, 0.01 )) as_spin.action = AnimState::CLOSING;
	}
	else
	{
		as_spin.pos = max( 0.0, as_spin.pos - CHUTE_SPIN_RATE * DeltaT );
		if (Eq( as_spin.pos, 0.0, 0.01 )) as_spin.action = AnimState::OPENING;
	}
	STS()->SetAnimation( anim_spin, as_spin.pos );
	return;
}