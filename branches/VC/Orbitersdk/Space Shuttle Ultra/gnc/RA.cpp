#include "RA.h"
#include "..\Atlantis.h"
#include <UltraMath.h>


namespace gnc
{
	RadarAltimeter::RadarAltimeter( AtlantisSubsystemDirector* _director, const string& _ident, int ID ):AtlantisSubsystem( _director, _ident )
	{
		this->ID = ID;
		return;
	}

	RadarAltimeter::~RadarAltimeter( void )
	{
		return;
	}

	void RadarAltimeter::Realize( void )
	{
		DiscreteBundle* bundle = BundleManager()->CreateBundle( "RDR_ALT", 16 );
		power.Connect( bundle, ID - 1 );
		return;
	}

	void RadarAltimeter::OnPostStep( double fSimT, double fDeltaT, double fMJD )
	{
		if (power == false) return;

		// calc alt (assumes the beam target is at the same elevation as what is directly below the vehicle)
		double tmp_alt = STS()->GetAltitude( ALTMODE_GROUND );
		VECTOR3 ofs = STS()->GetOrbiterCoGOffset();
		double cosp = cos( STS()->GetPitch() );

		tmp_alt += ((RADAR_POS_Z + ofs.y) * cosp) + ((RADAR_POS_X + ofs.z) * sin( STS()->GetPitch() ));// correct for radar position
		tmp_alt /= (cosp * cos( STS()->GetBank() ));// correct for vehicle attitude
		int alt = Round( tmp_alt * MPS2FPS );

		// assemble data
		unsigned short data = 1;// control

		if ((alt >= 0) && (alt < 8192))
		{
			data |= 2;// validity
			data |= alt << 2;// data
		}

		// odd parity
		unsigned short tmp = data;
		tmp ^= tmp >> 8;
		tmp ^= tmp >> 4;
		tmp ^= tmp >> 2;
		tmp ^= tmp >> 1;
		data |= ((~tmp) & 1) << 15;

		sprintf_s( oapiDebugString(), 255, "RA%d valid:%d alt:%dft data:%04X", ID, (data >> 1) & 0x1, (data >> 2) & 0x1FFF, data );
		return;
	}
}