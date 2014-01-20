#include "EIU.h"
#include "SSME.h"
#include "SSMEController.h"
#include "MPSdefs.h"
#include "assert.h"


namespace mps
{
	EIU::EIU( AtlantisSubsystemDirector* _director, const string& _ident, int ID, SSME* eng ):AtlantisSubsystem( _director, _ident )
	{
#ifdef _MPSDEBUG
		char buffer[100];
		sprintf_s( buffer, 100, " EIU::EIU in || name:%s|ID:%d", _ident.c_str(), ID );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG

		this->ID = ID;
		this->eng = eng;

		ACchA = new DiscInPort;
		ACchB = new DiscInPort;

		mia[0].Init( this, this, "MIA1", 14 + ID );
		mia[1].Init( this, this, "MIA2", 14 + ID );
		mia[2].Init( this, this, "MIA3", 14 + ID );
		mia[3].Init( this, this, "MIA4", 14 + ID );

		// zeroing
		memset( DataPri, 0, 32 * sizeof(unsigned short) );
		memset( DataSec, 0, 6 * sizeof(unsigned short) );

		// give ref
		eng->Controller->GetEIURef( this );

#ifdef _EIU_DATA_RECORDER
		char fname[32];
		sprintf_s( fname, 32, "EIU_data_ch%d.txt", ID );
		fp = fopen( fname, "w" );// empty file
		fclose( fp );
		fp = fopen( fname, "a" );
#endif// _EIU_DATA_RECORDER

#ifdef _MPSDEBUG
		sprintf_s( buffer, 100, " EIU::EIU out" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG
		return;
	}

	EIU::~EIU( void )
	{
#ifdef _EIU_DATA_RECORDER
		fclose( fp );
#endif// _EIU_DATA_RECORDER
		delete ACchA;
		delete ACchB;
		return;
	}

	void EIU::busCommandPhase( dps::BusController* biu )
	{
		return;
	}

	dps::BUS_COMMAND_WORD EIU::busCommand( dps::BusTerminal* biu, dps::BUS_COMMAND_WORD cw, unsigned long num_words, dps::word16 *cdw )
	{
		return cw;
	}

	void EIU::busReadPhase( dps::BusController* biu )
	{
		return;
	}

	void EIU::CIA( int num, unsigned short* data )
	{
		assert( (num >= 1) && (num <= 2) && "EIU::CIA.num" );
		// TODO create Status Override Switch
		// output to OPS Recorder, FM Signal Processor and T-0 Umbilical
		if (num == 1)
		{
			// pri data
			memcpy( DataPri, data, 32 * sizeof(unsigned short) );// partial data for GPCs
#ifdef _EIU_DATA_RECORDER
			DataRecorder( data, "PRI" );// save all telemetry
#endif// _EIU_DATA_RECORDER
		}
		else
		{
			// sec data
			memcpy( DataSec, data, 6 * sizeof(unsigned short) );// partial data for GPCs
#ifdef _EIU_DATA_RECORDER
			DataRecorder( data, "SEC" );// save all telemetry
#endif// _EIU_DATA_RECORDER
		}
		return;
	}

	void EIU::Realize( void )
	{
		// connect to panel O17 EIU power switches
		DiscreteBundle* O17_to_EIU_AC = BundleManager()->CreateBundle( "O17_to_EIU_AC", 3 );

		switch (ID)
		{
			case 1:
				// C
				// AC1/AC2
				ACchA->Connect( O17_to_EIU_AC, 1 );
				ACchB->Connect( O17_to_EIU_AC, 0 );
				break;
			case 2:
				// L
				// AC2/AC3
				ACchA->Connect( O17_to_EIU_AC, 0 );
				ACchB->Connect( O17_to_EIU_AC, 2 );
				break;
			case 3:
				// R
				// AC3/AC1
				ACchA->Connect( O17_to_EIU_AC, 2 );
				ACchB->Connect( O17_to_EIU_AC, 1 );
				break;
		}
		return;
	}

	void EIU::OnPostStep( double fSimT, double fDeltaT, double fMJD )
	{
		//eng->Controller->VIE->CommandDataConverter_write( 1, cmd[0] );
		//eng->Controller->VIE->CommandDataConverter_write( 2, cmd[1] );
		//eng->Controller->VIE->CommandDataConverter_write( 3, cmd[2] );


		// MIA 1 command
		//eng->Controller->VIE->CommandDataConverter( Command );
		
		// MIA 2 command
		//eng->Controller->VIE->Command[1] = 0;

		// MIA 3 and MIA 4 commands
		// TODO choose MIA 3 or MIA 4
		//eng->Controller->VIE->Command[2] = 0;
		return;
	}

	void EIU::readpri( unsigned short* data )
	{// HACK
		if ((ACchA->IsSet() == false) && (ACchB->IsSet() == false)) return;

		memcpy( data, DataPri, 32 * sizeof(unsigned short) );
		return;
	}

	void EIU::readsec( unsigned short* data )
	{// HACK
		if ((ACchA->IsSet() == false) && (ACchB->IsSet() == false)) return;

		memcpy( data, DataSec, 6 * sizeof(unsigned short) );
		return;
	}

	/*
	FR A5-107
	(...) One example of a transient failure is an EIU power-on-reset failure. A power-on-reset failure causes the
	temporary loss of a single power supply in an EIU. Since the EIU MIA ports 1 and 3 are on the same
	power source (MIA ports 2 and 4 are on another), this failure will cause the temporary loss of command
	and data transfer capability on two MIA ports (ground data only shows the status of MIA ports 1 and 4;
	therefore, MIA ports 3 and 2 are assumed bypassed with ports 1 and 4, respectively). After this failure has
	occurred, the command transfer capability will be regained automatically while the data transfer
	capability will be regained via an I/O Reset. The following scenario is an example of the concern that this
	failure raises: If an engine has a command channel B failure and a power-on-reset failure on MIA ports 1
	and 3, the engine will be considered to have suspect command capability even if the engine responds to
	throttle commands and will be shut down pre-MECO.
	*/

	void EIU::command( unsigned short cmd )
	{// HACK
		if ((ACchA->IsSet() == false) && (ACchB->IsSet() == false)) return;

		eng->Controller->VIE_CommandDataConverter_write( chA, cmd );
		eng->Controller->VIE_CommandDataConverter_write( chB, cmd );
		eng->Controller->VIE_CommandDataConverter_write( chC, cmd );
		return;
	}

#ifdef _EIU_DATA_RECORDER
	void EIU::DataRecorder( unsigned short* data, char* type )
	{
		int count = 0;
		
		fprintf( fp, "%s,", type );
		while (count < 128)
		{
			fprintf( fp, "%d,", data[count] );
			count++;
		}
		fprintf( fp, "\n" );
		return;
	}
#endif// _EIU_DATA_RECORDER
}