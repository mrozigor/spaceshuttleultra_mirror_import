#include "AtlantisSubsystem.h"
#include <DiscInPort.h>
#include <DiscOutPort.h>

namespace comm {
	class GCIL: public AtlantisSubsystem {
	public:
		GCIL(AtlantisSubsystemDirector* _direct);
		virtual ~GCIL();

		void OnPropagate(double fSimT, double fDeltaT, double fMJD);

		discsignals::DiscInPort command_enable;
		discsignals::DiscInPort panel_enable;

		discsignals::DiscInPort KU_Power_on_pnl;
		discsignals::DiscInPort KU_Power_off_pnl;
		discsignals::DiscInPort KU_Mode_RDR_COOP_pnl;
		discsignals::DiscInPort KU_Mode_COMM_pnl;
		discsignals::DiscDemultiplex16 KUAntennaSteeringMode_pnl;
		discsignals::DiscDemultiplex16 KUSIGPROCHDR_pnl;
		discsignals::DiscDemultiplex16 KUSIGPROCLDR_pnl;

		
		discsignals::DiscDemultiplex16 KUPower_and_Mode_cmd;
		discsignals::DiscDemultiplex16 KUAntennaSteeringMode_cmd;
		discsignals::DiscDemultiplex16 KUSIGPROCHDR_cmd;
		discsignals::DiscDemultiplex16 KUSIGPROCLDR_cmd;

		discsignals::DiscMultiplex16 KUPower_and_Mode_out;
		discsignals::DiscMultiplex16 KUAntennaSteeringMode_out;
		discsignals::DiscMultiplex16 KUSIGPROCHDR_out;
		discsignals::DiscMultiplex16 KUSIGPROCLDR_out;

	};
};