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
	RadiatorStatus.Set (AnimState::CLOSED, 0);
	RadLatchStatus.Set (AnimState::CLOSED, 0);

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
	} else if (!_strnicmp (line, "RADIATOR", 8)) {
		sscan_state ((char*)(line+8), RadiatorStatus);
		return true;
	} else if (!_strnicmp (line, "RADLATCH", 8)) {
		sscan_state ((char*)(line+8), RadLatchStatus);
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
	WriteScenario_state (scn, "RADIATOR", RadiatorStatus);
	WriteScenario_state (scn, "RADLATCH", RadLatchStatus);
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

	pBundle = STS()->BundleManager()->CreateBundle( "KUAntennaControl", 5 );
	KUAntennaDirectStow_ON.Connect( pBundle, 0 );
	KUAntenna_STO.Connect( pBundle, 1 );
	KUAntenna_DPY.Connect( pBundle, 2 );
	KUAntennaTB_STO.Connect( pBundle, 3 );
	KUAntennaTB_DPY.Connect( pBundle, 4 );

	hasAntenna = STS()->pMission->HasKUBand();

	SetTalkbacks();
	return;
}

void PayloadBay::OnPostStep( double fSimT, double fDeltaT, double fMJD )
{
	// payload bay doors
	if (PLBayDoorSYS_ENABLE[0].IsSet() && PLBayDoorSYS_ENABLE[1].IsSet())
	{
		if (PLBayDoor_CLOSE.IsSet())
		{
			if (!BayDoorStatus.Closed())
			{
				double da = fDeltaT * DOOR_OPERATING_SPEED;
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
				double da=fDeltaT*DOORLATCH_OPERATING_SPEED;
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
				double da=fDeltaT*DOORLATCH_OPERATING_SPEED;
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
				double da=fDeltaT*DOORLATCH_OPERATING_SPEED;
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
				double da=fDeltaT*DOORLATCH_OPERATING_SPEED;
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
				double da = fDeltaT * DOOR_OPERATING_SPEED;
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
	if (PLBayMECHPWRSYS_ON[0].IsSet() && PLBayMECHPWRSYS_ON[1].IsSet())
	{
		// latches
		double da;
		if (LatchControlSYS_LATCH[0].IsSet() == LatchControlSYS_LATCH[1].IsSet()) da = fDeltaT * RADLATCH_OPERATING_SPEED;
		else da = fDeltaT * RADLATCH_OPERATING_SPEED * 0.5; //only one motor working

		if ((LatchControlSYS_LATCH[0].IsSet() || LatchControlSYS_LATCH[1].IsSet()) && (!LatchControlSYS_RELEASE[0].IsSet() && !LatchControlSYS_RELEASE[1].IsSet()))
		{
			// latch
			if (RadLatchStatus.pos > 0.0)
			{
				RadLatchStatus.action = AnimState::CLOSING;
				RadLatchStatus.pos = max (0.0, RadLatchStatus.pos-da);
			}
			else RadLatchStatus.action = AnimState::CLOSED;
		}
		else if ((LatchControlSYS_RELEASE[0].IsSet() || LatchControlSYS_RELEASE[1].IsSet()) && (!LatchControlSYS_LATCH[0].IsSet() && !LatchControlSYS_LATCH[1].IsSet()))
		{
			// release
			if (RadLatchStatus.pos < 1.0)
			{
				RadLatchStatus.action = AnimState::OPENING;
				RadLatchStatus.pos = min (1.0, RadLatchStatus.pos+da);
			}
			else RadLatchStatus.action = AnimState::OPEN;
		}
		else
		{
			// stop
			if (RadLatchStatus.Moving()) RadLatchStatus.action = AnimState::STOPPED;
		}
		STS()->SetRadLatchPosition (RadLatchStatus.pos);

		// radiators
		if (RadiatorControlSYS_STOW[0].IsSet() == RadiatorControlSYS_STOW[1].IsSet()) da = fDeltaT * RAD_OPERATING_SPEED;
		else da = fDeltaT * RAD_OPERATING_SPEED * 0.5;

		if ((RadiatorControlSYS_STOW[0].IsSet() || RadiatorControlSYS_STOW[1].IsSet()) && (!RadiatorControlSYS_DEPLOY[0].IsSet() && !RadiatorControlSYS_DEPLOY[1].IsSet()) && BayDoorStatus.Open())
		{
			// stow
			if (RadiatorStatus.pos > 0.0)
			{
				RadiatorStatus.action = AnimState::CLOSING;
				RadiatorStatus.pos = max (0.0, RadiatorStatus.pos-da);
			}
			else RadiatorStatus.action = AnimState::CLOSED;
		}
		else if ((RadiatorControlSYS_DEPLOY[0].IsSet() || RadiatorControlSYS_DEPLOY[1].IsSet()) && (!RadiatorControlSYS_STOW[0].IsSet() && !RadiatorControlSYS_STOW[1].IsSet()) && BayDoorStatus.Open() && RadLatchStatus.Open())
		{
			// deploy
			if (RadiatorStatus.pos < 1.0)
			{
				RadiatorStatus.action = AnimState::OPENING;
				RadiatorStatus.pos = min (1.0, RadiatorStatus.pos+da);
			}
			else RadiatorStatus.action = AnimState::OPEN;
		}
		else
		{
			// stop
			if (RadiatorStatus.Moving()) RadiatorStatus.action = AnimState::STOPPED;
		}
		STS()->SetRadiatorPosition (RadiatorStatus.pos);
	}
	else
	{
		// stop everything
		if (RadiatorStatus.Moving()) RadiatorStatus.action = AnimState::STOPPED;
		if (RadLatchStatus.Moving()) RadLatchStatus.action = AnimState::STOPPED;
	}

	// ku antenna
	if (PLBayMECHPWRSYS_ON[0].IsSet() && PLBayMECHPWRSYS_ON[1].IsSet() && BayDoorStatus.Open())
	{
		double da = fDeltaT * KU_OPERATING_SPEED;
		if (KUAntennaDirectStow_ON.IsSet() || KUAntenna_STO.IsSet())
		{
			// stow
			if (KuAntennaStatus.pos > 0.0)
			{
				KuAntennaStatus.action = AnimState::CLOSING;
				KuAntennaStatus.pos = max (0.0, KuAntennaStatus.pos-da);
			}
			else KuAntennaStatus.action = AnimState::CLOSED;
		}
		else if (KUAntenna_DPY.IsSet())
		{
			// deploy
			if (KuAntennaStatus.pos < 1.0)
			{
				KuAntennaStatus.action = AnimState::OPENING;
				KuAntennaStatus.pos = min (1.0, KuAntennaStatus.pos+da);
			}
			else KuAntennaStatus.action = AnimState::OPEN;
		}
		else
		{
			// stop
			if (KuAntennaStatus.Moving()) KuAntennaStatus.action = AnimState::STOPPED;
		}
		STS()->SetKuAntennaPosition (KuAntennaStatus.pos);
	}
	else
	{
		// stop
		if (KuAntennaStatus.Moving()) KuAntennaStatus.action = AnimState::STOPPED;
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

	if (RadLatchStatus.Closed())
	{
		LatchSTBDTB_LAT.SetLine();
		LatchSTBDTB_REL.ResetLine();
		LatchPORTTB_LAT.SetLine();
		LatchPORTTB_REL.ResetLine();
	}
	else if (RadLatchStatus.Open())
	{
		LatchSTBDTB_LAT.ResetLine();
		LatchSTBDTB_REL.SetLine();
		LatchPORTTB_LAT.ResetLine();
		LatchPORTTB_REL.SetLine();
	}
	else
	{
		LatchSTBDTB_LAT.ResetLine();
		LatchSTBDTB_REL.ResetLine();
		LatchPORTTB_LAT.ResetLine();
		LatchPORTTB_REL.ResetLine();
	}

	if (RadiatorStatus.Closed())
	{
		RadiatorSTBDTB_STO.SetLine();
		RadiatorSTBDTB_DPY.ResetLine();
		RadiatorPORTTB_STO.SetLine();
		RadiatorPORTTB_DPY.ResetLine();
	}
	else if (RadiatorStatus.Open())
	{
		RadiatorSTBDTB_STO.ResetLine();
		RadiatorSTBDTB_DPY.SetLine();
		RadiatorPORTTB_STO.ResetLine();
		RadiatorPORTTB_DPY.SetLine();
	}
	else
	{
		RadiatorSTBDTB_STO.ResetLine();
		RadiatorSTBDTB_DPY.ResetLine();
		RadiatorPORTTB_STO.ResetLine();
		RadiatorPORTTB_DPY.ResetLine();
	}

	if (hasAntenna == true)
	{
		if (KuAntennaStatus.Closed())
		{
			KUAntennaTB_STO.SetLine();
			KUAntennaTB_DPY.ResetLine();
		}
		else if (KuAntennaStatus.Open())
		{
			KUAntennaTB_STO.ResetLine();
			KUAntennaTB_DPY.SetLine();
		}
		else
		{
			KUAntennaTB_STO.ResetLine();
			KUAntennaTB_DPY.ResetLine();
		}
	}
	else
	{
		// no antenna -> no animation -> have talkback always as barberpole
		KUAntennaTB_STO.ResetLine();
		KUAntennaTB_DPY.ResetLine();
	}
	return;
}