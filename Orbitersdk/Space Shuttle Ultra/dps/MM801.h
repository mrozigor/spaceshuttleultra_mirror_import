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
			std::string ElevonVoltage2DegreesString(float voltage) const;
			std::string RudderVoltage2DegreesString(float voltage) const;

		private:
			DiscOutPort ElevonCommand;
			DiscOutPort AileronCommand;
			DiscInPort ElevonCommandRead;
			DiscInPort AileronCommandRead;
			bool bFCSTestActive;
			bool bElevonMoveUpwards;
			bool bParkElevons;
			
	};


};