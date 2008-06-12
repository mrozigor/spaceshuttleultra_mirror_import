#include "EIU.h"

namespace mps
{

	EIU::EIU( SubsystemDirector* _director, SSME* ptr ):AtlantisSubsystem( _director, "mpsEIU" )
	{
		engine = ptr;
		BUFFER_SSME_DATA_TABLE = new DATA_TABLE[3];
		return;
	}

	EIU::~EIU( void )
	{
		delete BUFFER_SSME_DATA_TABLE;
		// end
	}

	int EIU::RequestCMD( int ME, int cmd, void* data )
	{
		switch (cmd)
		{
			case 1:// cmd seq
				if (engine->cmdPurgeSequence1( ME - 1 ) == true) return 1;
				return 2;
			case 2:
				if (engine->cmdPurgeSequence2( ME - 1 ) == true) return 1;
				return 2;
			case 3:
				if (engine->cmdPurgeSequence3( ME - 1 ) == true) return 1;
				return 2;
			case 4:
				if (engine->cmdPurgeSequence4( ME - 1 ) == true) return 1;
				return 2;
			case 5:
				if (engine->cmdStartEnable( ME - 1 ) == true) return 1;
				return 2;
			case 6:
				if (engine->cmdStart( ME - 1 ) == true) return 1;
				return 2;
			case 7:
				break;
			case 8:
				break;
			case 9:
				if (engine->cmdTerminateSequence( ME - 1 ) == true) return 1;
				return 2;
			case 10:
				if (engine->cmdShutdownEnable( ME - 1 ) == true) return 1;
				return 2;
			case 11:
				if (engine->cmdShutdown( ME - 1 ) == true) return 1;
				return 2;
			case 12:
				break;
			case 13:
				if (engine->cmdOxidizerDump( ME - 1 ) == true) return 1;
				return 2;
			case 14:
				break;
			case 15:
				break;
			case 16:
				break;
			case 17:
				{
					double PC = *((double*)data);
					if (engine->cmdThrottle( ME - 1, PC ) == true) return 1;
					return 2;
				}
		}

		return 0;
	}

	int EIU::RequestDATA( int ME, DATA_TABLE* ptrDataTable )
	{
		memcpy( ptrDataTable, &BUFFER_SSME_DATA_TABLE[ME - 1], sizeof(DATA_TABLE) );
		return 0;
	}


	void EIU::OnPostStep( double fSimT, double fDeltaT, double fMJD )
	{
		int ME = 0;
		while (ME <= 2)
		{
			engine->dataDataTable( ME - 1, &BUFFER_SSME_DATA_TABLE[ME - 1] );
			ME++;
		}
		return;
	}
}