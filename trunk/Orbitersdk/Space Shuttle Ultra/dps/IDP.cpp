#include "IDP.h"

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
			if(cScratchPadLine[i-1]==SSU_KEY_EXEC || cScratchPadLine[i-1]==SSU_KEY_PRO) return true;
		}
		return false;
	}

	bool IDP::PutKey(unsigned short usKeyboardID, char cKey) {
		
		//TODO: Implement checking of active keyboard
		switch(cKey) {
			case SSU_KEY_CLEAR:
				OnClear();
				break;
			case SSU_KEY_EXEC:
				OnExec();
			case SSU_KEY_ITEM:
			case SSU_KEY_SPEC:
			case SSU_KEY_OPS:
				if(IsCompleteLine()) ClearScratchPadLine();
			default:
				AppendScratchPadLine(cKey);
				break;
		}
		/*sprintf_s(oapiDebugString(), 255, "IDP %d|PutKey(%d, %02X)| %s", 
			usIDPID, usKeyboardID, cKey, 
			this->GetScratchPadLineString());*/
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
			STS()->Input(GetIDPID()-1, 10, NULL);
		}
		else {
			std::string scratchPad=GetScratchPadLineString();
			int ITEM=scratchPad.find("ITEM ");
			int OPS=scratchPad.find("OPS ");
			int SPEC=scratchPad.find("SPEC ");

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
				STS()->ItemInput(GetIDPID()-1, item, Data.c_str());
				Data=""; //clear string
				while(i<scratchPad.length()) {
					if(scratchPad[i]=='+' || scratchPad[i]=='-') {
						if(Data.length()>0) {
							item++;
							STS()->ItemInput(GetIDPID()-1, item, Data.c_str());
						}
						Data=""; //clear string
					}
					Data+=scratchPad[i];
					i++;
				}
				if(Data.length()>0) {
					item++;
					STS()->ItemInput(GetIDPID()-1, item, Data.c_str());
				}
			}
		}
	}

	void IDP::OnFaultSummary() {
	}


	void IDP::OnMMChange(unsigned short usMM) {
		ClearScratchPadLine();
	}

	bool IDP::OnPaint(vc::MDU* pMDU) {

		//Clear text buffer, if needed

		PrintTime(pMDU);
		//delegate painting to software


		return true;
	}

	void IDP::OnSysSummary() {
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
				case SSU_KEY_GPCIDP:
					strcat_s(pszBuffer, "GPC/IDP ");
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
				default:
					break;
			}
			i++;
		}

		return pszBuffer;
	}
};
