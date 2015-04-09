#include "SimpleGPCSoftware.h"
#include <discsignals.h>

namespace dps
{
	class MM801 : public SimpleGPCSoftware
	{
		public: 
			MM801(SimpleGPCSystem* _gpc);
			virtual ~MM801();
			virtual void Realize();
			virtual void OnPreStep(double SimT, double DeltaT, double MJD);
			//virtual void OnPostStep(double SimT, double DeltaT, double MJD);
			//virtual void OnPropagate(double SimT, double DeltaT, double MJD);

			virtual bool OnMajorModeChange(unsigned int newMajorMode);
			//virtual bool OnParseLine(const char* keyword, const char* value);
			//virtual void OnSaveState(FILEHANDLE scn) const;
			virtual bool ItemInput(int spec, int item, const char* Data);
			//virtual bool ExecPressed(int spec);
			virtual bool OnPaint(int spec, vc::MDU* pMDU) const;

		private:
			enum AEROSURFACE_DRIVE_TARGET {FV1, FV2, FV3}; // during aerosurface drive, aerosurfaces cycle between FV-1 and FV-2 positions, and are driven to FV-3 position at end of test

			double GetAerosurfaceCommand(double curTarget, double DeltaT, AEROSURFACE_DRIVE_TARGET& targetIdx, const double RATE, const double* POSITIONS);
			void PrintElevonPos(double pos, char* buff) const;
			void PrintRudderPos(double pos, char* buff) const;
			void PrintSpeedbrakePos(double pos, char* buff) const;


			DiscOutPort ElevonCommand;
			DiscInPort ElevonCommandRead;
			//DiscInPort AileronCommandRead;
			DiscOutPort AileronCommand;
			DiscOutPort RudderCommand;

			bool bFCSTestActive, bFCSTestEnding;
			AEROSURFACE_DRIVE_TARGET ElevonTargetIdx;
			AEROSURFACE_DRIVE_TARGET RudderTargetIdx;
			AEROSURFACE_DRIVE_TARGET SpeedbrakeTargetIdx;
			double ElevonTarget, RudderTarget, SpeedbrakeTarget;
	};


};
