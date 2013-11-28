#include "SSMEController.h"
#include "SSME.h"
#include "VehicleInterfaceElectronics.h"
#include "PowerSupplyElectronics.h"
#include "InputElectronics.h"
#include "OutputElectronics.h"
#include "DigitalComputerUnit.h"
#include "ComputerInterfaceElectronics.h"
#include "EIU.h"
#include "MPSdefs.h"


namespace mps
{
	SSMEController::SSMEController( SSME* eng )
	{
#ifdef _MPSDEBUG
		char buffer[100];
		sprintf_s( buffer, 100, " SSMEController::SSMEController in" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG

		this->eng = eng;

#ifdef _MPSDEBUG
		sprintf_s( buffer, 100, " SSMEController::SSMEController out" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG
		return;
	}

	SSMEController::~SSMEController( void )
	{
		return;
	}

	void SSMEController::__OnSaveState( FILEHANDLE scn ) const
	{
		VIE->OnSaveState( scn );// write VIE

		PSE[chA]->OnSaveState( scn );// write PSE chA
		PSE[chB]->OnSaveState( scn );// write PSE chB

		IE[chA]->OnSaveState( scn );// write IE chA
		IE[chB]->OnSaveState( scn );// write IE chB

		OE[chA]->OnSaveState( scn );// write OE chA
		OE[chB]->OnSaveState( scn );// write OE chB

		DCU[chA]->OnSaveState( scn );// write DCU chA
		DCU[chB]->OnSaveState( scn );// write DCU chB

		CIE[chA]->OnSaveState( scn );// write CIE chA
		CIE[chB]->OnSaveState( scn );// write CIE chB
		return;
	}

	bool SSMEController::__OnParseLine( const char* line )
	{
		if (VIE->OnParseLine( line )) return true;// check if VIE wants line

		if (PSE[chA]->OnParseLine( line )) return true;// check if PSE chA wants line
		if (PSE[chB]->OnParseLine( line )) return true;// check if PSE chB wants line

		if (IE[chA]->OnParseLine( line )) return true;// check if IE chA wants line
		if (IE[chB]->OnParseLine( line )) return true;// check if IE chB wants line

		if (OE[chA]->OnParseLine( line )) return true;// check if OE chA wants line
		if (OE[chB]->OnParseLine( line )) return true;// check if OE chB wants line

		if (DCU[chA]->OnParseLine( line )) return true;// check if DCU chA wants line
		if (DCU[chB]->OnParseLine( line )) return true;// check if DCU chB wants line

		if (CIE[chA]->OnParseLine( line )) return true;// check if CIE chA wants line
		if (CIE[chB]->OnParseLine( line )) return true;// check if CIE chB wants line
		return false;
	}

	void SSMEController::Realize( void )
	{
		// connect AC to PSE
		// TODO change bundle config?
		discsignals::DiscreteBundle* R2_AC;

		switch (eng->ID)
		{
			case 1:
				R2_AC = eng->BundleManager()->CreateBundle( "SSMEC_R2_SWITCHES", 4 );
				break;
			case 2:
				R2_AC = eng->BundleManager()->CreateBundle( "SSMEL_R2_SWITCHES", 4 );
				break;
			case 3:
				R2_AC = eng->BundleManager()->CreateBundle( "SSMER_R2_SWITCHES", 4 );
				break;
		}

		PSE[chA]->ConnectPower( R2_AC );
		PSE[chB]->ConnectPower( R2_AC );
		return;
	}

	void SSMEController::GetEIURef( EIU* pEIU )
	{
		this->pEIU = pEIU;
		return;
	}

	bool SSMEController::PSE_Power( int ch )
	{
		if (ch == chC) return (PSE[chA]->Power() || PSE[chB]->Power());
		return PSE[ch]->Power();// TODO check bounds
	}

	bool SSMEController::CIE_CheckWDTOwn( int CIEch, int nWDT )
	{
		return CIE[CIEch]->CheckWDTOwn( nWDT );// TODO check bounds
	}

	void SSMEController::DCU_PowerFailureSense( int DCUch )
	{
		DCU[DCUch]->PowerFailureSense();// TODO check bounds
		return;
	}

	void SSMEController::OE_GetSH( int OEch, double* data )
	{
		OE[OEch]->GetSH( data );// TODO check bounds
		return;
	}

	void SSMEController::OE_GetPOS( int OEch, double* data )
	{
		OE[OEch]->GetPOS( data );// TODO check bounds
		return;
	}

	void SSMEController::OE_StorageRegister_write( int OEch, unsigned short data, int ch )
	{
		OE[OEch]->StorageRegister_write( data, ch );
		return;
	}

	unsigned short SSMEController::OE_StorageRegister_read( int OEch )
	{
		return OE[OEch]->StorageRegister_read();
	}

	unsigned short SSMEController::OE_ONOFFCommandRegister_read( int OEch, int num )
	{
		return OE[OEch]->ONOFFCommandRegister_read( num );
	}

	void SSMEController::VIE_CommandDataConverter_write( int ch, unsigned short cmd )
	{
		VIE->CommandDataConverter_write( ch, cmd );
		return;
	}

	unsigned short SSMEController::VIE_CommandDataConverter_read( int ch )
	{
		return VIE->CommandDataConverter_read( ch );
	}

	void SSMEController::VIE_RecorderDataConverter_chA( unsigned short* data, int ch )
	{
		VIE->RecorderDataConverter_chA( data, ch );
		return;
	}

	void SSMEController::VIE_RecorderDataConverter_chB( unsigned short* data, int ch )
	{
		VIE->RecorderDataConverter_chB( data, ch );
		return;
	}

	void SSMEController::VIE_SwitchVRC( void )
	{
		VIE->SwitchVRC();
		return;
	}

	void SSMEController::VIE_RestoreVRC( void )
	{
		VIE->RestoreVRC();
		return;
	}

	void SSMEController::EIU_CIA( int num, unsigned short* data )
	{
		pEIU->CIA( num, data );
		return;
	}

	void SSMEController::PCA( void )
	{
		eng->PCA();
		return;
	}
}