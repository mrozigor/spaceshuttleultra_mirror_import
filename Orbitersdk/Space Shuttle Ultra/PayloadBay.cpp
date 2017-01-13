#include "PayloadBay.h"
#include <orbitersdk.h>
#include "AtlantisSubsystem.h"
#include "ParameterValues.h"


PayloadBay::PayloadBay( AtlantisSubsystemDirector* _director ):AtlantisSubsystem( _director, "PayloadBay" )
{
	// Cargo bay doors
	PayloadBayDoor[0].Set (AnimState::CLOSED, 0);
	PayloadBayDoor[1].Set (AnimState::CLOSED, 0);
	for(int i = 0; i < 8; i++) PLBDLatch[i].Set(AnimState::CLOSED, 0);

	// Radiators
	RadiatorStatus[0].Set( AnimState::CLOSED, 0 );
	RadiatorStatus[1].Set( AnimState::CLOSED, 0 );
	RadLatchStatus[0].Set( AnimState::CLOSED, 0 );
	RadLatchStatus[1].Set( AnimState::CLOSED, 0 );

	// Ku-band antenna
	KuAntennaStatus.Set (AnimState::CLOSED, 0);

	hasAntenna = true;
	return;
}

PayloadBay::~PayloadBay( void )
{
}

bool PayloadBay::OnParseLine( const char* line )
{
	if (!_strnicmp (line, "DOOR_PORT", 9)) {
		sscan_state ((char*)(line+9), PayloadBayDoor[0]);
		return true;
	} else if (!_strnicmp (line, "DOOR_STBD", 9)) {
		sscan_state ((char*)(line+9), PayloadBayDoor[1]);
		return true;
	} else if (!_strnicmp (line, "RADIATOR_PORT", 13)) {
		sscan_state ((char*)(line+13), RadiatorStatus[0]);
		return true;
	} else if (!_strnicmp (line, "RADIATOR_STBD", 13)) {
		sscan_state ((char*)(line+13), RadiatorStatus[1]);
		return true;
	} else if (!_strnicmp (line, "RADLATCH_PORT", 13)) {
		sscan_state ((char*)(line+13), RadLatchStatus[0]);
		return true;
	} else if (!_strnicmp (line, "RADLATCH_PSTBD", 13)) {
		sscan_state ((char*)(line+13), RadLatchStatus[1]);
		return true;
	} else if (!_strnicmp (line, "DOORLATCH", 9)) {
		int latch;
		sscanf_s (line+9, "%d", &latch);
		assert( (latch >= 0) && (latch <= 7) && "PayloadBay::OnParseLine.DOORLATCH" );
		sscan_state ((char*)(line+10), PLBDLatch[latch]);
	} else if (!_strnicmp (line, "KUBAND", 6)) {
		sscan_state ((char*)(line+6), KuAntennaStatus);
		return true;
	}
	return false;
}

void PayloadBay::OnSaveState( FILEHANDLE scn ) const
{
	char cbuf[256];

	WriteScenario_state( scn, "DOOR_PORT", PayloadBayDoor[0] );
	WriteScenario_state( scn, "DOOR_STBD", PayloadBayDoor[1] );
	WriteScenario_state (scn, "RADIATOR_PORT", RadiatorStatus[0]);
	WriteScenario_state (scn, "RADIATOR_STBD", RadiatorStatus[1]);
	WriteScenario_state (scn, "RADLATCH_PORT", RadLatchStatus[0]);
	WriteScenario_state (scn, "RADLATCH_STBD", RadLatchStatus[1]);
	for(int i=0;i<8;i++) {
		sprintf_s(cbuf, 255, "DOORLATCH%d", i);
		WriteScenario_state (scn, cbuf, PLBDLatch[i]);
	}
	if (hasAntenna == true) WriteScenario_state (scn, "KUBAND", KuAntennaStatus);
	return;
}

void PayloadBay::Realize( void )
{
	DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "PayloadBayDoorControl", 6 );
	PLBayDoorSYS_ENABLE[0].Connect( pBundle, 0 );
	PLBayDoorSYS_ENABLE[1].Connect( pBundle, 1 );
	PLBayDoor_CLOSE.Connect( pBundle, 2 );
	PLBayDoor_OPEN.Connect( pBundle, 3 );
	PLBayDoorTB_OP.Connect( pBundle, 4 );
	PLBayDoorTB_CL.Connect( pBundle, 5 );

	pBundle = STS()->BundleManager()->CreateBundle( "RadiatorControlSW", 10 );
	PLBayMECHPWRSYS_ON[0].Connect( pBundle, 0 );
	PLBayMECHPWRSYS_ON[1].Connect( pBundle, 1 );
	LatchControlSYS_LATCH[0].Connect( pBundle, 2 );
	LatchControlSYS_RELEASE[0].Connect( pBundle, 3 );
	LatchControlSYS_LATCH[1].Connect( pBundle, 4 );
	LatchControlSYS_RELEASE[1].Connect( pBundle, 5 );
	RadiatorControlSYS_STOW[0].Connect( pBundle, 6 );
	RadiatorControlSYS_DEPLOY[0].Connect( pBundle, 7 );
	RadiatorControlSYS_STOW[1].Connect( pBundle, 8 );
	RadiatorControlSYS_DEPLOY[1].Connect( pBundle, 9 );

	pBundle = STS()->BundleManager()->CreateBundle( "RadiatorControlTB", 8 );
	LatchSTBDTB_LAT.Connect( pBundle, 0 );
	LatchSTBDTB_REL.Connect( pBundle, 1 );
	LatchPORTTB_LAT.Connect( pBundle, 2 );
	LatchPORTTB_REL.Connect( pBundle, 3 );
	RadiatorSTBDTB_STO.Connect( pBundle, 4 );
	RadiatorSTBDTB_DPY.Connect( pBundle, 5 );
	RadiatorPORTTB_STO.Connect( pBundle, 6 );
	RadiatorPORTTB_DPY.Connect( pBundle, 7 );

	pBundle = STS()->BundleManager()->CreateBundle( "KUAntennaControl", 16 );
	KUAntennaDirectStow_ON.Connect( pBundle, 0 );
	KUAntenna_STO.Connect( pBundle, 1 );// BOOM
	KUAntenna_DPY.Connect( pBundle, 2 );
	KUAntennaTB_STO.Connect( pBundle, 3 );
	KUAntennaTB_DPY.Connect( pBundle, 4 );// XMIT
	BoomStowEnableI.Connect( pBundle, 5 );
	BoomStowEnableII.Connect( pBundle, 6 );

	hasAntenna = STS()->pMission->HasKUBand();

	// set boom position indications
	if (KuAntennaStatus.Closed())
	{
		KuRndz_Radar_Stow_Ind[0] = true;
		KuRndz_Radar_Stow_Ind[1] = true;
		KuRndz_Radar_Dpy_Ind[0] = false;
		KuRndz_Radar_Dpy_Ind[1] = false;
	}
	else if (KuAntennaStatus.Open())
	{
		KuRndz_Radar_Stow_Ind[0] = false;
		KuRndz_Radar_Stow_Ind[1] = false;
		KuRndz_Radar_Dpy_Ind[0] = true;
		KuRndz_Radar_Dpy_Ind[1] = true;
	}
	else
	{
		KuRndz_Radar_Stow_Ind[0] = false;
		KuRndz_Radar_Stow_Ind[1] = false;
		KuRndz_Radar_Dpy_Ind[0] = false;
		KuRndz_Radar_Dpy_Ind[1] = false;
	}

	SetTalkbacks();
	return;
}

void PayloadBay::OnPostStep( double fSimT, double fDeltaT, double fMJD )
{
	// payload bay doors
	if (PLBayDoorSYS_ENABLE[0].IsSet() || PLBayDoorSYS_ENABLE[1].IsSet())
	{
		if (PLBayDoor_CLOSE.IsSet())
		{
			if (!PayloadBayDoor[0].Closed())// port door
			{
				double da = fDeltaT * PLBD_OPERATING_SPEED * 0.5 * ((int)PLBayDoorSYS_ENABLE[0] + (int)PLBayDoorSYS_ENABLE[1]);
				
				if (PayloadBayDoor[0].pos > 0.0)
				{
					PayloadBayDoor[0].action = AnimState::CLOSING;
					PayloadBayDoor[0].pos = max (0.0, PayloadBayDoor[0].pos-da);
				}
				else PayloadBayDoor[0].action = AnimState::CLOSED;
				STS()->SetPayloadBayDoorPosition( 0, PayloadBayDoor[0].pos );
			}
			else if (!PLBDLatch[4].Closed())// port fwd, port aft
			{
				double da = fDeltaT * PLBD_CENTERLINE_LATCH_OPERATING_SPEED * 0.5 * ((int)PLBayDoorSYS_ENABLE[0] + (int)PLBayDoorSYS_ENABLE[1]);
				
				if (PLBDLatch[4].pos > 0.0)
				{
					PLBDLatch[4].action=AnimState::CLOSING;
					PLBDLatch[5].action=AnimState::CLOSING;
					PLBDLatch[4].pos=max(0.0, PLBDLatch[4].pos-da);
					PLBDLatch[5].pos = PLBDLatch[5].pos;
				}
				else
				{
					PLBDLatch[4].action=AnimState::CLOSED;
					PLBDLatch[5].action=AnimState::CLOSED;
				}
			}
			else if (!PayloadBayDoor[1].Closed())// stbd door
			{
				double da = fDeltaT * PLBD_OPERATING_SPEED * 0.5 * ((int)PLBayDoorSYS_ENABLE[0] + (int)PLBayDoorSYS_ENABLE[1]);
				
				if (PayloadBayDoor[1].pos > 0.0)
				{
					PayloadBayDoor[1].action = AnimState::CLOSING;
					PayloadBayDoor[1].pos = max (0.0, PayloadBayDoor[1].pos-da);
				}
				else PayloadBayDoor[1].action = AnimState::CLOSED;
				STS()->SetPayloadBayDoorPosition( 1, PayloadBayDoor[1].pos );
			}
			else if (!PLBDLatch[6].Closed())// stbd fwd, stbd aft
			{
				double da = fDeltaT * PLBD_CENTERLINE_LATCH_OPERATING_SPEED * 0.5 * ((int)PLBayDoorSYS_ENABLE[0] + (int)PLBayDoorSYS_ENABLE[1]);
				
				if (PLBDLatch[6].pos > 0.0)
				{
					PLBDLatch[6].action=AnimState::CLOSING;
					PLBDLatch[7].action=AnimState::CLOSING;
					PLBDLatch[6].pos=max(0.0, PLBDLatch[6].pos-da);
					PLBDLatch[7].pos = PLBDLatch[6].pos;
				}
				else
				{
					PLBDLatch[6].action=AnimState::CLOSED;
					PLBDLatch[7].action=AnimState::CLOSED;
				}
			}
			else if (!PLBDLatch[0].Closed())// c/l 1-4, c/l 13-16
			{
				double da = fDeltaT * PLBD_CENTERLINE_LATCH_OPERATING_SPEED * 0.5 * ((int)PLBayDoorSYS_ENABLE[0] + (int)PLBayDoorSYS_ENABLE[1]);
				
				if (PLBDLatch[0].pos > 0.0)
				{
					PLBDLatch[0].action=AnimState::CLOSING;
					PLBDLatch[3].action=AnimState::CLOSING;
					PLBDLatch[0].pos=max(0.0, PLBDLatch[0].pos-da);
					PLBDLatch[3].pos = PLBDLatch[0].pos;
				}
				else
				{
					PLBDLatch[0].action=AnimState::CLOSED;
					PLBDLatch[3].action=AnimState::CLOSED;
				}
				STS()->SetPayloadBayDoorLatchPosition(0, PLBDLatch[0].pos);
				STS()->SetPayloadBayDoorLatchPosition(3, PLBDLatch[3].pos);
			}
			else// c/l 5-8, c/l 9-12
			{
				double da = fDeltaT * PLBD_CENTERLINE_LATCH_OPERATING_SPEED * 0.5 * ((int)PLBayDoorSYS_ENABLE[0] + (int)PLBayDoorSYS_ENABLE[1]);
				
				if (PLBDLatch[1].pos > 0.0)
				{
					PLBDLatch[1].action=AnimState::CLOSING;
					PLBDLatch[2].action=AnimState::CLOSING;
					PLBDLatch[1].pos=max(0.0, PLBDLatch[1].pos-da);
					PLBDLatch[2].pos = PLBDLatch[1].pos;
				}
				else
				{
					PLBDLatch[1].action=AnimState::CLOSED;
					PLBDLatch[2].action=AnimState::CLOSED;
				}
				STS()->SetPayloadBayDoorLatchPosition(1, PLBDLatch[1].pos);
				STS()->SetPayloadBayDoorLatchPosition(2, PLBDLatch[2].pos);
			}
		}
		else if (PLBayDoor_OPEN.IsSet())
		{
			if (!PLBDLatch[1].Open())// c/l 5-8, c/l 9-12
			{
				double da = fDeltaT * PLBD_CENTERLINE_LATCH_OPERATING_SPEED * 0.5 * ((int)PLBayDoorSYS_ENABLE[0] + (int)PLBayDoorSYS_ENABLE[1]);
				
				if(PLBDLatch[1].pos < 1.0)
				{
					PLBDLatch[1].action=AnimState::OPENING;
					PLBDLatch[2].action=AnimState::OPENING;
					PLBDLatch[1].pos=min(1.0, PLBDLatch[1].pos+da);
					PLBDLatch[2].pos = PLBDLatch[1].pos;
				}
				else
				{
					PLBDLatch[1].action=AnimState::OPEN;
					PLBDLatch[2].action=AnimState::OPEN;
				}
				STS()->SetPayloadBayDoorLatchPosition(1, PLBDLatch[1].pos);
				STS()->SetPayloadBayDoorLatchPosition(2, PLBDLatch[2].pos);
			}
			else if (!PLBDLatch[0].Open())// c/l 1-4, c/l 13-16
			{
				double da = fDeltaT * PLBD_CENTERLINE_LATCH_OPERATING_SPEED * 0.5 * ((int)PLBayDoorSYS_ENABLE[0] + (int)PLBayDoorSYS_ENABLE[1]);
				
				if(PLBDLatch[0].pos < 1.0)
				{
					PLBDLatch[0].action=AnimState::OPENING;
					PLBDLatch[3].action=AnimState::OPENING;
					PLBDLatch[0].pos=min(1.0, PLBDLatch[0].pos+da);
					PLBDLatch[3].pos = PLBDLatch[0].pos;
				}
				else
				{
					PLBDLatch[0].action=AnimState::OPEN;
					PLBDLatch[3].action=AnimState::OPEN;
				}
				STS()->SetPayloadBayDoorLatchPosition(0, PLBDLatch[0].pos);
				STS()->SetPayloadBayDoorLatchPosition(3, PLBDLatch[3].pos);
			}
			else if (!PLBDLatch[6].Open())// stbd fwd, stbd aft
			{
				double da = fDeltaT * PLBD_BULKHEAD_LATCH_OPERATING_SPEED * 0.5 * ((int)PLBayDoorSYS_ENABLE[0] + (int)PLBayDoorSYS_ENABLE[1]);
				
				if(PLBDLatch[6].pos < 1.0)
				{
					PLBDLatch[6].action=AnimState::OPENING;
					PLBDLatch[7].action=AnimState::OPENING;
					PLBDLatch[6].pos=min(1.0, PLBDLatch[6].pos+da);
					PLBDLatch[7].pos = PLBDLatch[6].pos;
				}
				else
				{
					PLBDLatch[6].action=AnimState::OPEN;
					PLBDLatch[7].action=AnimState::OPEN;
				}
			}
			else if (!PayloadBayDoor[1].Open())// stbd door
			{
				double da = fDeltaT * PLBD_OPERATING_SPEED * 0.5 * ((int)PLBayDoorSYS_ENABLE[0] + (int)PLBayDoorSYS_ENABLE[1]);
				
				if (PayloadBayDoor[1].pos < 1.0)
				{
					PayloadBayDoor[1].action = AnimState::OPENING;
					PayloadBayDoor[1].pos = min (1.0, PayloadBayDoor[1].pos+da);
				}
				else PayloadBayDoor[1].action = AnimState::OPEN;
				STS()->SetPayloadBayDoorPosition( 1, PayloadBayDoor[1].pos );
			}
			else if (!PLBDLatch[4].Open())// port fwd, port aft
			{
				double da = fDeltaT * PLBD_BULKHEAD_LATCH_OPERATING_SPEED * 0.5 * ((int)PLBayDoorSYS_ENABLE[0] + (int)PLBayDoorSYS_ENABLE[1]);
				
				if(PLBDLatch[4].pos < 1.0)
				{
					PLBDLatch[4].action=AnimState::OPENING;
					PLBDLatch[5].action=AnimState::OPENING;
					PLBDLatch[4].pos=min(1.0, PLBDLatch[4].pos+da);
					PLBDLatch[5].pos = PLBDLatch[5].pos;
				}
				else
				{
					PLBDLatch[4].action=AnimState::OPEN;
					PLBDLatch[5].action=AnimState::OPEN;
				}
			}
			else// port door
			{
				double da = fDeltaT * PLBD_OPERATING_SPEED * 0.5 * ((int)PLBayDoorSYS_ENABLE[0] + (int)PLBayDoorSYS_ENABLE[1]);
				
				if (PayloadBayDoor[0].pos < 1.0)
				{
					PayloadBayDoor[0].action = AnimState::OPENING;
					PayloadBayDoor[0].pos = min (1.0, PayloadBayDoor[0].pos+da);
				}
				else PayloadBayDoor[0].action = AnimState::OPEN;
				STS()->SetPayloadBayDoorPosition( 0, PayloadBayDoor[0].pos );
			}
		}
		else
		{
			// stop everything
			if (PayloadBayDoor[0].Moving()) PayloadBayDoor[0].action = AnimState::STOPPED;
			if (PayloadBayDoor[1].Moving()) PayloadBayDoor[1].action = AnimState::STOPPED;
			if (PLBDLatch[0].Moving())
			{
				PLBDLatch[0].action=AnimState::STOPPED;
				PLBDLatch[3].action=AnimState::STOPPED;
			}
			if (PLBDLatch[1].Moving())
			{
				PLBDLatch[1].action=AnimState::STOPPED;
				PLBDLatch[2].action=AnimState::STOPPED;
			}
			if (PLBDLatch[4].Moving())
			{
				PLBDLatch[4].action=AnimState::STOPPED;
				PLBDLatch[5].action=AnimState::STOPPED;
			}
			if (PLBDLatch[6].Moving())
			{
				PLBDLatch[6].action=AnimState::STOPPED;
				PLBDLatch[7].action=AnimState::STOPPED;
			}
		}
	}
	else
	{
		// stop everything
		if (PayloadBayDoor[0].Moving()) PayloadBayDoor[0].action = AnimState::STOPPED;
		if (PayloadBayDoor[1].Moving()) PayloadBayDoor[1].action = AnimState::STOPPED;
		if (PLBDLatch[0].Moving())
		{
			PLBDLatch[0].action=AnimState::STOPPED;
			PLBDLatch[3].action=AnimState::STOPPED;
		}
		if (PLBDLatch[1].Moving())
		{
			PLBDLatch[1].action=AnimState::STOPPED;
			PLBDLatch[2].action=AnimState::STOPPED;
		}
		if (PLBDLatch[4].Moving())
		{
			PLBDLatch[4].action=AnimState::STOPPED;
			PLBDLatch[5].action=AnimState::STOPPED;
		}
		if (PLBDLatch[6].Moving())
		{
			PLBDLatch[6].action=AnimState::STOPPED;
			PLBDLatch[7].action=AnimState::STOPPED;
		}
	}

	// radiators
	if (PLBayMECHPWRSYS_ON[0] || PLBayMECHPWRSYS_ON[1])
	{
		// port latches
		double da = fDeltaT * RADLATCH_OPERATING_SPEED * 0.5 * 
			(((int)PLBayMECHPWRSYS_ON[0] * ((int)LatchControlSYS_RELEASE[0] - (int)LatchControlSYS_LATCH[0])) + 
			((int)PLBayMECHPWRSYS_ON[1] * ((int)LatchControlSYS_RELEASE[1] - (int)LatchControlSYS_LATCH[1])));
		if (da > 0)
		{
			// release
			RadLatchStatus[0].action = AnimState::OPENING;
			RadLatchStatus[0].Move( da );
		}
		else if (da < 0)
		{
			// latch
			RadLatchStatus[0].action = AnimState::CLOSING;
			RadLatchStatus[0].Move( -da );
		}
		else
		{
			// stop
			if (RadLatchStatus[0].Moving()) RadLatchStatus[0].action = AnimState::STOPPED;
		}

		// stbd latches
		da = fDeltaT * RADLATCH_OPERATING_SPEED * 0.5 * 
			(((int)PLBayMECHPWRSYS_ON[0] * ((int)LatchControlSYS_RELEASE[1] - (int)LatchControlSYS_LATCH[1])) + 
			((int)PLBayMECHPWRSYS_ON[1] * ((int)LatchControlSYS_RELEASE[0] - (int)LatchControlSYS_LATCH[0])));
		if (da > 0)
		{
			// release
			RadLatchStatus[1].action = AnimState::OPENING;
			RadLatchStatus[1].Move( da );
		}
		else if (da < 0)
		{
			// latch
			RadLatchStatus[1].action = AnimState::CLOSING;
			RadLatchStatus[1].Move( -da );
		}
		else
		{
			// stop
			if (RadLatchStatus[1].Moving()) RadLatchStatus[1].action = AnimState::STOPPED;
		}

		// port drive
		da = fDeltaT * RAD_OPERATING_SPEED * 0.5 * 
			(((int)PLBayMECHPWRSYS_ON[0] * ((int)RadiatorControlSYS_DEPLOY[0] - (int)RadiatorControlSYS_STOW[0])) + 
			((int)PLBayMECHPWRSYS_ON[1] * ((int)RadiatorControlSYS_DEPLOY[1] - (int)RadiatorControlSYS_STOW[1])));
		if ((da > 0) && PayloadBayDoor[0].Open() && RadLatchStatus[0].Open())
		{
			// deploy
			RadiatorStatus[0].action = AnimState::OPENING;
			RadiatorStatus[0].Move( da );
		}
		else if ((da < 0) && PayloadBayDoor[0].Open())
		{
			// stow
			RadiatorStatus[0].action = AnimState::CLOSING;
			RadiatorStatus[0].Move( -da );
		}
		else
		{
			// stop
			if (RadiatorStatus[0].Moving()) RadiatorStatus[0].action = AnimState::STOPPED;
		}
		STS()->SetRadiatorPosition( RadiatorStatus[0].pos, 0 );

		// stbd drive
		if ((da > 0) && PayloadBayDoor[1].Open() && RadLatchStatus[1].Open())
		{
			// deploy
			RadiatorStatus[1].action = AnimState::OPENING;
			RadiatorStatus[1].Move( da );
		}
		else if ((da < 0) && PayloadBayDoor[1].Open())
		{
			// stow
			RadiatorStatus[1].action = AnimState::CLOSING;
			RadiatorStatus[1].Move( -da );
		}
		else
		{
			// stop
			if (RadiatorStatus[1].Moving()) RadiatorStatus[1].action = AnimState::STOPPED;
		}
		STS()->SetRadiatorPosition( RadiatorStatus[1].pos, 1 );
	}
	else
	{
		// stop everything
		if (RadiatorStatus[0].Moving()) RadiatorStatus[0].action = AnimState::STOPPED;
		if (RadiatorStatus[1].Moving()) RadiatorStatus[1].action = AnimState::STOPPED;
		if (RadLatchStatus[0].Moving()) RadLatchStatus[0].action = AnimState::STOPPED;
		if (RadLatchStatus[1].Moving()) RadLatchStatus[1].action = AnimState::STOPPED;
	}

	// ku antenna boom
	bool KUAntenna_GND = !(KUAntenna_DPY | KUAntenna_STO);
	bool cmd_V54K0003N = false;
	bool cmd_V54K0013N = false;
	bool cmd_V54K0014N = false;
	bool cmd_V54K0004N = false;

	// motor 1
	double m1 = (int)(((KUAntenna_DPY | KUAntenna_GND) & (!KuRndz_Radar_Dpy_Ind[0] & (KUAntenna_DPY | cmd_V54K0003N))) & // K72
		((KUAntenna_DPY | KUAntenna_GND) & (!KuRndz_Radar_Dpy_Ind[0] & (KUAntenna_DPY | cmd_V54K0013N)))) - // K70
		(int)(((KUAntenna_STO | KUAntennaDirectStow_ON) & (!KuRndz_Radar_Stow_Ind[0] & (BoomStowEnableII | KUAntennaDirectStow_ON))) & // K14
		((KUAntenna_STO | KUAntennaDirectStow_ON) & (!KuRndz_Radar_Stow_Ind[0] & (BoomStowEnableI | KUAntennaDirectStow_ON))));// K68

	// motor 2
	double m2 = (int)(((KUAntenna_DPY | KUAntenna_GND) & (!KuRndz_Radar_Dpy_Ind[1] & (KUAntenna_DPY | cmd_V54K0014N))) & // K27
		((KUAntenna_DPY | KUAntenna_GND) & (!KuRndz_Radar_Dpy_Ind[1] & (KUAntenna_DPY | cmd_V54K0004N)))) - // K37
		(int)(((KUAntenna_STO | KUAntennaDirectStow_ON) & (!KuRndz_Radar_Stow_Ind[1] & (BoomStowEnableI | KUAntennaDirectStow_ON))) & // K25
		((KUAntenna_STO | KUAntennaDirectStow_ON) & (!KuRndz_Radar_Stow_Ind[1] & (BoomStowEnableII | KUAntennaDirectStow_ON))));// K2

	double da = (m1 * (int)PLBayMECHPWRSYS_ON[0] + m2 * (int)PLBayMECHPWRSYS_ON[1]) * KU_OPERATING_SPEED * fDeltaT;
	if (da > 0)
	{
		// deploy
		KuAntennaStatus.action = AnimState::OPENING;
		KuAntennaStatus.Move( da );
		STS()->SetKuAntennaDAPosition( KuAntennaStatus.pos );
	}
	else if (da < 0)
	{
		// stow
		KuAntennaStatus.action = AnimState::CLOSING;
		KuAntennaStatus.Move( -da );
		STS()->SetKuAntennaDAPosition( KuAntennaStatus.pos );
	}
	else
	{
		// stop
		if (KuAntennaStatus.Moving()) KuAntennaStatus.action = AnimState::STOPPED;
	}

	// set boom position indications
	if (KuAntennaStatus.Closed())
	{
		KuRndz_Radar_Stow_Ind[0] = true;
		KuRndz_Radar_Stow_Ind[1] = true;
		KuRndz_Radar_Dpy_Ind[0] = false;
		KuRndz_Radar_Dpy_Ind[1] = false;
	}
	else if (KuAntennaStatus.Open())
	{
		KuRndz_Radar_Stow_Ind[0] = false;
		KuRndz_Radar_Stow_Ind[1] = false;
		KuRndz_Radar_Dpy_Ind[0] = true;
		KuRndz_Radar_Dpy_Ind[1] = true;
	}
	else
	{
		KuRndz_Radar_Stow_Ind[0] = false;
		KuRndz_Radar_Stow_Ind[1] = false;
		KuRndz_Radar_Dpy_Ind[0] = false;
		KuRndz_Radar_Dpy_Ind[1] = false;
	}

	
	SetTalkbacks();
	return;
}

void PayloadBay::SetTalkbacks( void )
{
	// talkback output
	if (PayloadBayDoor[0].Open() && PayloadBayDoor[1].Open() && 
		PLBDLatch[0].Open() && PLBDLatch[1].Open() && PLBDLatch[2].Open() && PLBDLatch[3].Open() &&
		PLBDLatch[4].Open() && PLBDLatch[5].Open() && PLBDLatch[6].Open() && PLBDLatch[7].Open())
	{
		PLBayDoorTB_OP.SetLine();
		PLBayDoorTB_CL.ResetLine();
	}
	else if (PayloadBayDoor[0].Closed() && PayloadBayDoor[1].Closed() && 
		PLBDLatch[0].Closed() && PLBDLatch[1].Closed() && PLBDLatch[2].Closed() && PLBDLatch[3].Closed() &&
		PLBDLatch[4].Closed() && PLBDLatch[5].Closed() && PLBDLatch[6].Closed() && PLBDLatch[7].Closed())
	{
		PLBayDoorTB_OP.ResetLine();
		PLBayDoorTB_CL.SetLine();
	}
	else
	{
		PLBayDoorTB_OP.ResetLine();
		PLBayDoorTB_CL.ResetLine();
	}

	if (RadLatchStatus[0].Closed())
	{
		LatchPORTTB_LAT.SetLine();
		LatchPORTTB_REL.ResetLine();
	}
	else if (RadLatchStatus[0].Open())
	{
		LatchPORTTB_LAT.ResetLine();
		LatchPORTTB_REL.SetLine();
	}
	else
	{
		LatchPORTTB_LAT.ResetLine();
		LatchPORTTB_REL.ResetLine();
	}
	if (RadLatchStatus[1].Closed())
	{
		LatchSTBDTB_LAT.SetLine();
		LatchSTBDTB_REL.ResetLine();
	}
	else if (RadLatchStatus[1].Open())
	{
		LatchSTBDTB_LAT.ResetLine();
		LatchSTBDTB_REL.SetLine();
	}
	else
	{
		LatchSTBDTB_LAT.ResetLine();
		LatchSTBDTB_REL.ResetLine();
	}

	if (RadiatorStatus[0].Closed())
	{
		RadiatorPORTTB_STO.SetLine();
		RadiatorPORTTB_DPY.ResetLine();
	}
	else if (RadiatorStatus[0].Open())
	{
		RadiatorPORTTB_STO.ResetLine();
		RadiatorPORTTB_DPY.SetLine();
	}
	else
	{
		RadiatorPORTTB_STO.ResetLine();
		RadiatorPORTTB_DPY.ResetLine();
	}
	if (RadiatorStatus[1].Closed())
	{
		RadiatorSTBDTB_STO.SetLine();
		RadiatorSTBDTB_DPY.ResetLine();
	}
	else if (RadiatorStatus[1].Open())
	{
		RadiatorSTBDTB_STO.ResetLine();
		RadiatorSTBDTB_DPY.SetLine();
	}
	else
	{
		RadiatorSTBDTB_STO.ResetLine();
		RadiatorSTBDTB_DPY.ResetLine();
	}

	if (hasAntenna == true)
	{
		KUAntennaTB_STO.SetLine( 5.0f * (int)(KuRndz_Radar_Stow_Ind[0] | KuRndz_Radar_Stow_Ind[1]) );
		KUAntennaTB_DPY.SetLine( 5.0f * (int)(KuRndz_Radar_Dpy_Ind[0] | KuRndz_Radar_Dpy_Ind[1]) );
	}
	else
	{
		// no antenna -> no animation -> have talkback always as barberpole
		KUAntennaTB_STO.ResetLine();
		KUAntennaTB_DPY.ResetLine();
	}
	return;
}