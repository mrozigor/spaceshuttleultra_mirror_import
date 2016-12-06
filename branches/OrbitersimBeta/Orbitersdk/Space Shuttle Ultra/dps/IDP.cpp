#include "IDP.h"
#include "SimpleGPCSystem.h"
#include "IO_Control.h"
#include "SSME_Operations.h"
#include "AscentDAP.h"
#include "AerojetDAP.h"
#include "Landing_SOP.h"
#include "OMSBurnSoftware.h"


namespace dps {

	IDP::IDP(AtlantisSubsystemDirector* _direct, const std::string& _ident, 
		unsigned short _usIDPID)
		: AtlantisSubsystem(_direct, _ident), usIDPID(_usIDPID),
		usGPCDay(0), usGPCHour(0), usGPCMinute(0), usGPCSecond(0),
		usTimerDay(0), usTimerHour(0), usTimerMinute(0), usTimerSecond(0)
	{
		usSPEC=dps::MODE_UNDEFINED;
		usDISP=dps::MODE_UNDEFINED;
		majfunc=GNC;
		cScratchPadLine[0] = '\0';

		dk_channel.Init(this, this, "DK", 10);
		fc_channel[0].Init(this, this, "FC1", 9+usIDPID);
		fc_channel[1].Init(this, this, "FC2", 9+usIDPID);
		fc_channel[2].Init(this, this, "FC3", 9+usIDPID);
		fc_channel[3].Init(this, this, "FC4", 9+usIDPID);
	}

	IDP::~IDP()
	{
	}

	void IDP::Realize()
	{
		pIO_Control =  static_cast<IO_Control*> (STS()->pSimpleGPC->FindSoftware( "IO_Control" ));
		pSSME_Operations =  static_cast<SSME_Operations*> (STS()->pSimpleGPC->FindSoftware( "SSME_Operations" ));
		pAscentDAP =  static_cast<AscentDAP*> (STS()->pSimpleGPC->FindSoftware( "AscentDAP" ));
		pAerojetDAP =  static_cast<AerojetDAP*> (STS()->pSimpleGPC->FindSoftware( "AerojetDAP" ));
		pLanding_SOP =  static_cast<Landing_SOP*> (STS()->pSimpleGPC->FindSoftware( "Landing_SOP" ));
		pOMSBurnSoftware =  static_cast<OMSBurnSoftware*> (STS()->pSimpleGPC->FindSoftware( "OMSBurnSoftware" ));

		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "C2_R11_IDP", 14 );
		MajorFuncPL.Connect( pBundle, usIDPID + 3 );
		MajorFuncGNC.Connect( pBundle, usIDPID + 7 );
		KeybSelectA.Connect( pBundle, 12 );// not used by IDP4
		KeybSelectB.Connect( pBundle, 13 );// not used by IDP4
		return;
	}

	BUS_COMMAND_WORD IDP::busCommand(BusTerminal* biu, BUS_COMMAND_WORD cw, unsigned long num_data, word16* cdw)
	{
		if(biu == &dk_channel)
		{
			char* command = reinterpret_cast<char*>(cdw);
			if(command[0] == '@')
			{
				//special command
			}
			else
			{
				//just print it directly.
				
			}
			//reply with status

		}
		return cw;
	}

	void IDP::busCommandPhase(BusController* biu)
	{
	}

	void IDP::busReadPhase(BusController* biu)
	{
	}

	unsigned short IDP::GetIDPID() const {
		return usIDPID;
	}

	unsigned short IDP::GetDisp() const
	{
		return usDISP;
	}

	unsigned short IDP::GetOps() const
	{
		return usOPS;
	}

	unsigned short IDP::GetSpec() const
	{
		return usSPEC;
	}

	MAJORFUNCTION IDP::GetMajfunc() const
	{
		return majfunc;
	}

	void IDP::IPL() {

		memstate = MS_IPL;

		//clear all data tables
		//rebuild all data tables
	}

	bool IDP::IsBFS() const {
		return false;
	}

	bool IDP::IsCompleteLine() const {
		unsigned short i=0;
		while(i < 120 && cScratchPadLine[i] != '\0') 
			i++;
		if(i>0) {
			if(cScratchPadLine[i-1]==SSU_KEY_EXEC || 
				cScratchPadLine[i-1]==SSU_KEY_PRO || 
				cScratchPadLine[i-1]==SSU_KEY_RESUME ||
				cScratchPadLine[i-1]==SSU_KEY_SYSSUMM ||
				cScratchPadLine[i-1]==SSU_KEY_FAULTSUMM) return true;
		}
		return false;
	}

	int IDP::GetActiveKeyboard( void ) const
	{
		int kb = 0;

		switch (usIDPID)
		{
			case 1:
				if (KeybSelectA.IsSet()) kb = 1;
				break;
			case 2:
				if (KeybSelectB.IsSet() == false) kb = 2;
				break;
			case 3:
				if (KeybSelectA.IsSet() == false) kb = 1;
				if (KeybSelectB.IsSet()) kb += 2;
				break;
		}
		return kb;
	}

	bool IDP::IsKeyboardSelected( unsigned short usKeyboardID ) const
	{
		switch (usIDPID)
		{
			case 1:
				if ((usKeyboardID == 1) && (KeybSelectA.IsSet())) return true;
				else return false;
			case 2:
				if ((usKeyboardID == 2) && (KeybSelectB.IsSet() == false)) return true;
				else return false;
			case 3:
				if ((usKeyboardID == 1) && (KeybSelectA.IsSet() == false)) return true;
				else if ((usKeyboardID == 2) && (KeybSelectB.IsSet())) return true;
				else return false;
			case 4:
				if (usKeyboardID == 3) return true;
				else return false;
			default:
				return false;
		}
	}

	bool IDP::PutKey(unsigned short usKeyboardID, char cKey)
	{
		if (IsKeyboardSelected( usKeyboardID ) == false) return false;

		switch(cKey) {
			case SSU_KEY_RESUME:
				OnResume();
				ClearScratchPadLine();
				AppendScratchPadLine( cKey );
				break;
			case SSU_KEY_CLEAR:
				OnClear();
				break;
			case SSU_KEY_EXEC:
				if(IsCompleteLine()) ClearScratchPadLine();
				OnExec();
				AppendScratchPadLine(cKey);
				break;
			case SSU_KEY_PRO:
				OnPro();
				AppendScratchPadLine(cKey);
				break;
			case SSU_KEY_ITEM:
			case SSU_KEY_SPEC:
			case SSU_KEY_OPS:
			case SSU_KEY_GPCCRT:
			case SSU_KEY_IORESET:
				if(IsCompleteLine()) ClearScratchPadLine();
				AppendScratchPadLine(cKey);
				break;
			case SSU_KEY_SYSSUMM:
				OnSysSummary();
				ClearScratchPadLine();
				AppendScratchPadLine( cKey );
				break;
			case SSU_KEY_FAULTSUMM:
				OnFaultSummary( false );
				ClearScratchPadLine();
				AppendScratchPadLine( cKey );
				break;
			default:
				AppendScratchPadLine(cKey);
				break;
		}
		return true;
	}

	void IDP::SetDisp(unsigned short disp)
	{
		usDISP=disp;
	}

	void IDP::SetSpec(unsigned short spec)
	{
		usSPEC=spec;
	}

	void IDP::SetMajFunc(MAJORFUNCTION func)
	{
		majfunc=func;
	}

	void IDP::ClearScratchPadLine() {
		cScratchPadLine[0] = '\0';
	}

	void IDP::ConnectToMDU(vc::PMDU pMDU, bool bPrimary)
	{
		if(pMDU) {
			if(bPrimary) {
				pMDU->SetPrimaryIDP(this);
			}
			else pMDU->SetSecondaryIDP(this);
		}
	}

	void IDP::OnSaveState(FILEHANDLE scn) const
	{
		char cbuf[255];
		sprintf_s(cbuf, 255, "IDP%d SPEC", usIDPID);
		oapiWriteScenario_int(scn, cbuf, usSPEC);
		sprintf_s(cbuf, 255, "IDP%d DISP", usIDPID);
		oapiWriteScenario_int(scn, cbuf, usDISP);
	}

	bool IDP::OnParseLine(const char* line)
	{
		char IDP[10];
		sprintf_s(IDP, 10, "IDP%d", usIDPID);
		if(!_strnicmp(IDP, line, 4)) {
			if(!_strnicmp(line+5, "SPEC", 4)) {
				sscanf_s(line+9, "%d", &usSPEC);
				return true;
			}
			else if(!_strnicmp(line+5, "DISP", 4)) {
				sscanf_s(line+9, "%d", &usDISP);
				return true;
			}
		}
		return false;
	}

	void IDP::OnAck() {
	}

	void IDP::OnClear() {

		if(IsCompleteLine()) {
			ClearScratchPadLine();
		} else {
			DelFromScratchPadLine();
		}
		
	}

	void IDP::OnExec() {
		// check if EXEC was pressed without any ITEM input
		if(cScratchPadLine[0]=='\0' || IsCompleteLine()) {
			//STS()->Input(GetIDPID()-1, 10, NULL);
			STS()->pSimpleGPC->ExecPressed(GetSpec());
		}
		else {
			std::string scratchPad=GetScratchPadLineString();
			int ITEM=scratchPad.find("ITEM ");

			if(ITEM!=string::npos) {
				scratchPad=scratchPad.erase(ITEM, 5);

				//parse entry
				unsigned int i;
				bool delim=false;
				string Data="", Name="";
				for(i=0;i<scratchPad.length();i++) {
					if(scratchPad[i]=='+' || scratchPad[i]=='-') {
						if(delim) break;
						delim=true;
					}
					if(!delim) Name+=scratchPad[i];
					else {
						Data+=scratchPad[i];
					}
				}
				//STS()->Input(GetIDPID(), 1, Name.c_str(), Data.c_str());
				unsigned int item=atoi(Name.c_str());
				STS()->pSimpleGPC->ItemInput(GetSpec(), item, Data.c_str());
				Data=""; //clear string
				while(i<scratchPad.length()) {
					if(scratchPad[i]=='+' || scratchPad[i]=='-') {
						if(Data.length()>1) {// needs to be at least 2 chars: sign and one number
							item++;
							STS()->pSimpleGPC->ItemInput(GetSpec(), item, Data.c_str());
						}
						else if (Data.length() == 1) item++;// skip item
						Data=""; //clear string
					}
					Data+=scratchPad[i];
					i++;
				}
				if(Data.length()>1) {// needs to be at least 2 chars: sign and one number
					item++;
					STS()->pSimpleGPC->ItemInput(GetSpec(), item, Data.c_str());
				}
			}
		}
	}

	void IDP::OnPro()
	{
		std::string scratchPad=GetScratchPadLineString();
		int OPS=scratchPad.find("OPS ");
		int SPEC=scratchPad.find("SPEC ");

		if(OPS != std::string::npos) { // OPS entered
			//STS()->Input(GetIDPID()-1, 0, scratchPad.substr(OPS+4).c_str());
			unsigned int newMM = static_cast<unsigned int>(atoi(scratchPad.substr(OPS+4).c_str()));
			if(STS()->pSimpleGPC->IsValidMajorModeTransition(newMM)) {
				STS()->pSimpleGPC->SetMajorMode(newMM);
			}
		}
		else if(SPEC != std::string::npos) { // SPEC entered
			//STS()->Input(GetIDPID()-1, 2, scratchPad.substr(SPEC+5).c_str());
			int newSpec = atoi(scratchPad.substr(SPEC+5).c_str());
			if(STS()->pSimpleGPC->IsValidSPEC( newSpec ))
			{
				// choose between DISP and SPEC
				if (IsDisp( newSpec ) == true)
				{
					SetSpec( dps::MODE_UNDEFINED );
					SetDisp( static_cast<unsigned short>(newSpec) );
				}
				else
				{
					SetSpec(static_cast<unsigned short>(newSpec));
					SetDisp( dps::MODE_UNDEFINED );
				}
			}
		}
	}

	void IDP::OnResume()
	{
		if(GetDisp() != dps::MODE_UNDEFINED) {
			SetDisp(dps::MODE_UNDEFINED);
		}
		else if(GetSpec() != dps::MODE_UNDEFINED) {
			SetSpec(dps::MODE_UNDEFINED);
		}
	}

	void IDP::OnFaultSummary( bool ClearList )
	{
		if (ClearList){} // TODO clear list
		SetDisp( 99 );
		return;
	}


	void IDP::OnMMChange(unsigned short usMM) {
		ClearScratchPadLine();
	}

	bool IDP::OnPaint(vc::MDU* pMDU) {

		//Clear text buffer, if needed

		//PrintTime(pMDU);
		//delegate painting to software

		if(GetDisp() != dps::MODE_UNDEFINED)
			return STS()->pSimpleGPC->OnPaint(GetDisp(), pMDU);
		else
			return STS()->pSimpleGPC->OnPaint(GetSpec(), pMDU);
	}

	void IDP::OnSysSummary()
	{
		// TODO check here if DISP valid in current OPS?
		if (GetMajfunc() == dps::GNC)
		{
			if (GetDisp() == 18) SetDisp( 19 );
			else SetDisp( 18 );
			SetSpec( dps::MODE_UNDEFINED );
		}
		else if (GetMajfunc() == dps::SM)
		{
			if (GetDisp() == 78) SetDisp( 79 );
			else SetDisp( 78 );
			SetSpec( dps::MODE_UNDEFINED );
		}
		return;
	}


	void IDP::OnMsgReset() {
	}

	
	void IDP::PrintTime(vc::MDU* mdu) {
		char pszBuffer[15];

		sprintf_s(pszBuffer, 15, "%03d/%02d:%02d:%02d",
			usGPCDay, usGPCHour, usGPCMinute, usGPCSecond);
		mdu->mvprint(39, 1, pszBuffer, 0);

		if(bGPCTimerActive) {
			sprintf_s(pszBuffer, 15, "%03d/%02d:%02d:%02d",
				usTimerDay, usTimerHour, usTimerMinute, usTimerSecond);
			mdu->mvprint(39, 2, pszBuffer, 0);
		}
	}


	const char* IDP::GetScratchPadLineScan() const {
		return cScratchPadLine;
	}

	void IDP::AppendScratchPadLine(char cKey) {
		unsigned short i = 0;
		while(cScratchPadLine[i] != '\0' && i < 119) 
			i++;
		cScratchPadLine[i] = cKey;
		cScratchPadLine[i+1] = '\0';
		
	}

	void IDP::DelFromScratchPadLine() {
		unsigned short i = 0;
		while(cScratchPadLine[i] != '\0' && i < 119) 
			i++;
		if(i>0) {
			cScratchPadLine[i-1] = '\0';
		}
	}

	const char* IDP::GetScratchPadLineString() const {
		static char pszBuffer[250];

		pszBuffer[0] = '\0';

		unsigned short i = 0;
		while(cScratchPadLine[i] != '\0' && i<120) {
			switch(cScratchPadLine[i]) {
				case SSU_KEY_ITEM:
					strcat_s(pszBuffer, "ITEM ");
					break;
				case SSU_KEY_GPCCRT:
					strcat_s(pszBuffer, "GPC/CRT ");
					break;
				case SSU_KEY_OPS:
					strcat_s(pszBuffer, "OPS ");
					break;
				case SSU_KEY_SPEC:
					strcat_s(pszBuffer, "SPEC ");
					break;
				case SSU_KEY_A:
					strcat_s(pszBuffer, "A");
					break;
				case SSU_KEY_B:
					strcat_s(pszBuffer, "B");
					break;
				case SSU_KEY_C:
					strcat_s(pszBuffer, "C");
					break;
				case SSU_KEY_D:
					strcat_s(pszBuffer, "D");
					break;
				case SSU_KEY_E:
					strcat_s(pszBuffer, "E");
					break;
				case SSU_KEY_F:
					strcat_s(pszBuffer, "F");
					break;
				case SSU_KEY_1:
					strcat_s(pszBuffer, "1");
					break;
				case SSU_KEY_2:
					strcat_s(pszBuffer, "2");
					break;
				case SSU_KEY_3:
					strcat_s(pszBuffer, "3");
					break;
				case SSU_KEY_4:
					strcat_s(pszBuffer, "4");
					break;
				case SSU_KEY_5:
					strcat_s(pszBuffer, "5");
					break;
				case SSU_KEY_6:
					strcat_s(pszBuffer, "6");
					break;
				case SSU_KEY_7:
					strcat_s(pszBuffer, "7");
					break;
				case SSU_KEY_8:
					strcat_s(pszBuffer, "8");
					break;
				case SSU_KEY_9:
					strcat_s(pszBuffer, "9");
					break;
				case SSU_KEY_0:
					strcat_s(pszBuffer, "0");
					break;
				case SSU_KEY_DOT:
					strcat_s(pszBuffer, ".");
					break;
				case SSU_KEY_PLUS:
					strcat_s(pszBuffer, "+");
					break;
				case SSU_KEY_MINUS:
					strcat_s(pszBuffer, "-");
					break;
				case SSU_KEY_EXEC:
					strcat_s(pszBuffer, " EXEC");
					break;
				case SSU_KEY_PRO:
					strcat_s(pszBuffer, " PRO");
					break;
				case SSU_KEY_ACK:
					strcat_s(pszBuffer, "ACK");
					break;
				case SSU_KEY_IORESET:
					strcat_s(pszBuffer, "I/O RESET");
					break;
				case SSU_KEY_RESUME:
					strcat_s( pszBuffer, "RESUME" );
					break;
				case SSU_KEY_SYSSUMM:
					strcat_s( pszBuffer, "SYS SUMM" );
					break;
				case SSU_KEY_FAULTSUMM:
					strcat_s( pszBuffer, "FAULT SUMM" );
					break;
				default:
					break;
			}
			i++;
		}

		return pszBuffer;
	}

	const char* IDP::GetScratchPadLineString_B( void ) const
	{
		const char* cbufin = GetScratchPadLineString();
		static char cbufout[250];
		int len = strlen( cbufin );
		int j = 0;
		bool firstsign = false;
		int item = 0;

		for (int i = 0; i <= len; i++)
		{
			if ((cbufin[i] == '+') || (cbufin[i] == '-'))
			{
				if (firstsign == false)
				{// first item number behind
					firstsign = true;

					if ((cbufout[j - 1] >= '0') && (cbufout[j - 1] <= '9'))
					{
						if ((cbufout[j - 2] >= '0') && (cbufout[j - 2] <= '9'))
						{
							// two-digit
							item = (cbufout[j - 1] - 48) + ((cbufout[j - 2] - 48) * 10);

							sprintf_s( cbufout + j - 2, 32, "(%d)", item );
							j += 2;
						}
						else
						{
							// one-digit
							item = cbufout[j - 1] - 48;

							sprintf_s( cbufout + j - 1, 32, "(0%d)", item );
							j += 3;
						}
					}
				}
				else
				{// data behind... or sign

					if ((cbufin[i - 1] == '+') || (cbufin[i - 1] == '-'))
					{
						//_(24)+
						item++;
						sprintf_s( cbufout + j - 4, 32, "%02d)", item );
						j -= 1;
					}
					else
					{
						item++;
						sprintf_s( cbufout + j, 32, " (%02d)", item );
						j += 5;
					}
				}
			}
			cbufout[j++] = cbufin[i];
		}

		return cbufout;
	}

	int IDP::GetFlashingEntry( void ) const
	{
		if (IsCompleteLine()) return 0;
		
		switch (cScratchPadLine[0])
		{
			case SSU_KEY_OPS:
				return 3;
			case SSU_KEY_SPEC:
			case SSU_KEY_ITEM:
				return 4;
			case SSU_KEY_GPCCRT:
				return 7;
			case SSU_KEY_IORESET:
				return 9;
			default:
				return 0;
		}
	}

	bool IDP::IsDisp( int code ) const
	{
		switch (code)
		{
			case 6:
			case 18:
			case 19:
			case 66:
			case 67:
			case 68:
			case 69:
			case 76:
			case 77:
			case 78:
			case 79:
			case 86:
			case 87:
			case 88:
			case 89:
			case 95:
			case 96:
			case 97:
			case 99:
			case 106:
			case 167:
			case 168:
			case 169:
			case 177:
			case 179:
				return true;
			default:
				return false;
		}
	}

	int IDP::GetADIAttitude( void )
	{
		switch (usIDPID)
		{
			case 1:
				return pIO_Control->GetSWPos( SW_ADI_ATTITUDE_F6 );
			case 2:
				return pIO_Control->GetSWPos( SW_ADI_ATTITUDE_F8 );
			case 4:
				return pIO_Control->GetSWPos( SW_ADI_ATTITUDE_A6U );
			default:
				return 1;// switch in LVLH
		}
	}

	int IDP::GetADIError( void )
	{
		switch (usIDPID)
		{
			case 1:
				return pIO_Control->GetSWPos( SW_ADI_ERROR_F6 );
			case 2:
				return pIO_Control->GetSWPos( SW_ADI_ERROR_F8 );
			case 4:
				return pIO_Control->GetSWPos( SW_ADI_ERROR_A6U );
			default:
				return 1;// switch in MED
		}
	}

	int IDP::GetADIRate( void )
	{
		switch (usIDPID)
		{
			case 1:
				return pIO_Control->GetSWPos( SW_ADI_RATE_F6 );
			case 2:
				return pIO_Control->GetSWPos( SW_ADI_RATE_F8 );
			case 4:
				return pIO_Control->GetSWPos( SW_ADI_RATE_A6U );
			default:
				return 1;// switch in MED
		}
	}

	bool IDP::GetMECOConfirmedFlag( void ) const
	{
		return pSSME_Operations->GetMECOConfirmedFlag();
	}

	bool IDP::GetAutoThrottleState( void ) const
	{
		return pAscentDAP->GetAutoThrottleState();
	}

	VECTOR3 IDP::GetAttitudeErrors_AscentDAP( void ) const
	{
		return pAscentDAP->GetAttitudeErrors();
	}

	VECTOR3 IDP::GetAttitudeErrors_AerojetDAP( void ) const
	{
		return pAerojetDAP->GetAttitudeErrors();
	}
	
	VECTOR3 IDP::GetAttitudeCommandErrors( void ) const
	{
		return pOMSBurnSoftware->GetAttitudeCommandErrors();
	}

	bool IDP::GetAutoPitchState( void ) const
	{
		return pAerojetDAP->GetAutoPitchState();
	}

	bool IDP::GetAutoRollYawState( void ) const
	{
		return pAerojetDAP->GetAutoRollYawState();
	}

	bool IDP::GetAutoSpeedbrakeState( void ) const
	{
		return pAerojetDAP->GetAutoSpeedbrakeState();
	}

	double IDP::GetAutoSpeedbrakeCommand( void ) const
	{
		return pAerojetDAP->GetAutoSpeedbrakeCommand();
	}

	bool IDP::GetWOW( void ) const
	{
		return pLanding_SOP->GetWOWLON();
	}

	double IDP::GetNZError( void ) const
	{
		return pAerojetDAP->GetNZError();
	}

	bool IDP::GetPrefinalState( void ) const
	{
		return pAerojetDAP->GetPrefinalState();
	}

	double IDP::GetYRunwayPositionError( void ) const
	{
		return pAerojetDAP->GetYRunwayPositionError();
	}

	bool IDP::GetOnHACState( void ) const
	{
		return pAerojetDAP->GetOnHACState();
	}

	double IDP::GetHACRadialError( void ) const
	{
		return pAerojetDAP->GetHACRadialError();
	}

	double IDP::GetTimeToHAC( void ) const
	{
		return pAerojetDAP->GetTimeToHAC();
	}

	double IDP::GetdeltaAZ( void ) const
	{
		return pAerojetDAP->GetdeltaAZ();
	}

	double IDP::GetDistanceToHACCenter( void ) const
	{
		return pAerojetDAP->GetDistanceToHACCenter();
	}

	const std::string& IDP::GetSelectedRunway( void ) const
	{
		return pAerojetDAP->GetSelectedRunway();
	}

	double IDP::GetRangeToRunway( void ) const
	{
		return pAerojetDAP->GetRangeToRunway();
	}

	bool IDP::GetApproachAndLandState( void ) const
	{
		return pAerojetDAP->GetApproachAndLandState();
	}

	double IDP::GetVacc( void ) const
	{
		return pAerojetDAP->GetVacc();
	}

	double IDP::GetHTA( void ) const
	{
		return pAerojetDAP->GetHTA();
	}

	double IDP::GetGlideSlopeDistance( void ) const
	{
		return pAerojetDAP->GetGlideSlopeDistance();
	}

	double IDP::GetNZ( void ) const
	{
		return pAerojetDAP->GetNZ();
	}

	double IDP::GetdeltaAZLimit( double mach ) const
	{
		return pAerojetDAP->GetdeltaAZLimit( mach );
	}

	double IDP::GetSelectedRunwayHeading( void ) const
	{
		return pAerojetDAP->GetSelectedRunwayHeading();
	}

	double IDP::GetTargetHeading( void ) const
	{
		return pAscentDAP->GetTargetHeading();
	}

	bool IDP::GetFCSmode( void ) const
	{
		return pAscentDAP->GetFCSmode();
	}
};
