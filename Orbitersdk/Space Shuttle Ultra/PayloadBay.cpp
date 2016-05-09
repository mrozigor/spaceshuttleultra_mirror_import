#include "PayloadBay.h"
#include <orbitersdk.h>
#include "AtlantisSubsystem.h"
#include "ParameterValues.h"


PayloadBay::PayloadBay( AtlantisSubsystemDirector* _director ):AtlantisSubsystem( _director, "PayloadBay" )
{
	// Cargo bay doors
	BayDoorStatus.Set (AnimState::CLOSED, 0);
	for(int i = 0; i < 4; i++) CLBayDoorLatch[i].Set(AnimState::CLOSED, 0);

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
	if (!_strnicmp (line, "CARGODOOR", 9)) {
		sscan_state ((char*)(line+9), BayDoorStatus);
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
	} else if (!_strnicmp (line, "KUBAND", 6)) {
		sscan_state ((char*)(line+6), KuAntennaStatus);
		return true;
	} else if (!_strnicmp (line, "BAYDOORLATCH", 12)) {
		int latch;
		sscanf_s (line+12, "%d", &latch);
		sscan_state ((char*)(line+13), CLBayDoorLatch[latch]);
	}
	return false;
}

void PayloadBay::OnSaveState( FILEHANDLE scn ) const
{
	char cbuf[256];

	WriteScenario_state (scn, "CARGODOOR", BayDoorStatus);
	WriteScenario_state (scn, "RADIATOR_PORT", RadiatorStatus[0]);
	WriteScenario_state (scn, "RADIATOR_STBD", RadiatorStatus[1]);
	WriteScenario_state (scn, "RADLATCH_PORT", RadLatchStatus[0]);
	WriteScenario_state (scn, "RADLATCH_STBD", RadLatchStatus[1]);
	if (hasAntenna == true) WriteScenario_state (scn, "KUBAND", KuAntennaStatus);
	for(int i=0;i<4;i++) {
		sprintf_s(cbuf, 255, "BAYDOORLATCH%d", i);
		WriteScenario_state (scn, cbuf, CLBayDoorLatch[i]);
	}
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
			if (!BayDoorStatus.Closed())
			{
				double da = fDeltaT * DOOR_OPERATING_SPEED * 0.5 * ((int)PLBayDoorSYS_ENABLE[0] + (int)PLBayDoorSYS_ENABLE[1]);
				// close doors
				if (BayDoorStatus.pos > 0.0)
				{
					BayDoorStatus.action = AnimState::CLOSING;
					BayDoorStatus.pos = max (0.0, BayDoorStatus.pos-da);
				}
				else BayDoorStatus.action = AnimState::CLOSED;
				STS()->SetBayDoorPosition (BayDoorStatus.pos);
			}
			else if (!CLBayDoorLatch[0].Closed())
			{
				double da=fDeltaT*DOORLATCH_OPERATING_SPEED * 0.5 * ((int)PLBayDoorSYS_ENABLE[0] + (int)PLBayDoorSYS_ENABLE[1]);
				// close latches 1 and 4
				if (CLBayDoorLatch[0].pos > 0.0)
				{
					CLBayDoorLatch[0].action=AnimState::CLOSING;
					CLBayDoorLatch[3].action=AnimState::CLOSING;
					CLBayDoorLatch[0].pos=max(0.0, CLBayDoorLatch[0].pos-da);
					CLBayDoorLatch[3].pos = CLBayDoorLatch[0].pos;
				}
				else
				{
					CLBayDoorLatch[0].action=AnimState::CLOSED;
					CLBayDoorLatch[3].action=AnimState::CLOSED;
				}
				STS()->SetBayDoorLatchPosition(0, CLBayDoorLatch[0].pos);
				STS()->SetBayDoorLatchPosition(3, CLBayDoorLatch[3].pos);
			}
			else
			{
				double da=fDeltaT*DOORLATCH_OPERATING_SPEED * 0.5 * ((int)PLBayDoorSYS_ENABLE[0] + (int)PLBayDoorSYS_ENABLE[1]);
				// close latches 2 and 3
				if (CLBayDoorLatch[1].pos > 0.0)
				{
					CLBayDoorLatch[1].action=AnimState::CLOSING;
					CLBayDoorLatch[2].action=AnimState::CLOSING;
					CLBayDoorLatch[1].pos=max(0.0, CLBayDoorLatch[1].pos-da);
					CLBayDoorLatch[2].pos = CLBayDoorLatch[1].pos;
				}
				else
				{
					CLBayDoorLatch[1].action=AnimState::CLOSED;
					CLBayDoorLatch[2].action=AnimState::CLOSED;
				}
				STS()->SetBayDoorLatchPosition(1, CLBayDoorLatch[1].pos);
				STS()->SetBayDoorLatchPosition(2, CLBayDoorLatch[2].pos);
			}
		}
		else if (PLBayDoor_OPEN.IsSet())
		{
			if (!CLBayDoorLatch[1].Open())
			{
				double da=fDeltaT*DOORLATCH_OPERATING_SPEED * 0.5 * ((int)PLBayDoorSYS_ENABLE[0] + (int)PLBayDoorSYS_ENABLE[1]);
				// open latches 2 and 3
				if(CLBayDoorLatch[1].pos < 1.0)
				{
					CLBayDoorLatch[1].action=AnimState::OPENING;
					CLBayDoorLatch[2].action=AnimState::OPENING;
					CLBayDoorLatch[1].pos=min(1.0, CLBayDoorLatch[1].pos+da);
					CLBayDoorLatch[2].pos = CLBayDoorLatch[1].pos;
				}
				else
				{
					CLBayDoorLatch[1].action=AnimState::OPEN;
					CLBayDoorLatch[2].action=AnimState::OPEN;
				}
				STS()->SetBayDoorLatchPosition(1, CLBayDoorLatch[1].pos);
				STS()->SetBayDoorLatchPosition(2, CLBayDoorLatch[2].pos);
			}
			else if (!CLBayDoorLatch[0].Open())
			{
				double da=fDeltaT*DOORLATCH_OPERATING_SPEED * 0.5 * ((int)PLBayDoorSYS_ENABLE[0] + (int)PLBayDoorSYS_ENABLE[1]);
				// open latches 1 and 4
				if(CLBayDoorLatch[0].pos < 1.0)
				{
					CLBayDoorLatch[0].action=AnimState::OPENING;
					CLBayDoorLatch[3].action=AnimState::OPENING;
					CLBayDoorLatch[0].pos=min(1.0, CLBayDoorLatch[0].pos+da);
					CLBayDoorLatch[3].pos = CLBayDoorLatch[0].pos;
				}
				else
				{
					CLBayDoorLatch[0].action=AnimState::OPEN;
					CLBayDoorLatch[3].action=AnimState::OPEN;
				}
				STS()->SetBayDoorLatchPosition(0, CLBayDoorLatch[0].pos);
				STS()->SetBayDoorLatchPosition(3, CLBayDoorLatch[3].pos);
			}
			else
			{
				double da = fDeltaT * DOOR_OPERATING_SPEED * 0.5 * ((int)PLBayDoorSYS_ENABLE[0] + (int)PLBayDoorSYS_ENABLE[1]);
				// open doors
				if (BayDoorStatus.pos < 1.0)
				{
					BayDoorStatus.action = AnimState::OPENING;
					BayDoorStatus.pos = min (1.0, BayDoorStatus.pos+da);
				}
				else BayDoorStatus.action = AnimState::OPEN;
				STS()->SetBayDoorPosition (BayDoorStatus.pos);
			}
		}
		else
		{
			// stop everything
			if (BayDoorStatus.Moving()) BayDoorStatus.action = AnimState::STOPPED;
			if (CLBayDoorLatch[0].Moving())
			{
				CLBayDoorLatch[0].action=AnimState::STOPPED;
				CLBayDoorLatch[3].action=AnimState::STOPPED;
			}
			if (CLBayDoorLatch[1].Moving())
			{
				CLBayDoorLatch[1].action=AnimState::STOPPED;
				CLBayDoorLatch[2].action=AnimState::STOPPED;
			}
		}
	}
	else
	{
		// stop everything
		if (BayDoorStatus.Moving()) BayDoorStatus.action = AnimState::STOPPED;
		if (CLBayDoorLatch[0].Moving())
		{
			CLBayDoorLatch[0].action=AnimState::STOPPED;
			CLBayDoorLatch[3].action=AnimState::STOPPED;
		}
		if (CLBayDoorLatch[1].Moving())
		{
			CLBayDoorLatch[1].action=AnimState::STOPPED;
			CLBayDoorLatch[2].action=AnimState::STOPPED;
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
		if ((da > 0) && BayDoorStatus.Open() && RadLatchStatus[0].Open())
		{
			// deploy
			RadiatorStatus[0].action = AnimState::OPENING;
			RadiatorStatus[0].Move( da );
		}
		else if ((da < 0) && BayDoorStatus.Open())
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
		if ((da > 0) && BayDoorStatus.Open() && RadLatchStatus[1].Open())
		{
			// deploy
			RadiatorStatus[1].action = AnimState::OPENING;
			RadiatorStatus[1].Move( da );
		}
		else if ((da < 0) && BayDoorStatus.Open())
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
	if (BayDoorStatus.Open())
	{
		PLBayDoorTB_OP.SetLine();
		PLBayDoorTB_CL.ResetLine();
	}
	else if (BayDoorStatus.Closed())
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